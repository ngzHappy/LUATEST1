#include "MainWindow.hpp"
#include <QApplication>

#include <QLuaCore.hpp>
#include <iostream>
class Test : public QObject {
    class RegisterType { public:
        RegisterType();
    };
    static RegisterType registerType;
public:

    void foo() const { std::cout<<"Test:foo()"<<std::endl; }

    Test() { std::cout<<"Test()"<<std::endl; }
    ~Test() { std::cout<<"~Test()"<<std::endl; }
};

Q_DECLARE_METATYPE( std::shared_ptr<QPointer<Test>> )

Test::RegisterType::RegisterType() {
    cct::registerVirtualConverter<QObject,Test>();
    cct::registerConverter<Test,QObject>();
}

Test::RegisterType Test::registerType;

class Test1Base {
public:
    virtual void foo()=0;
    virtual ~Test1Base() = default ;
};

class Test1 : public Test1Base {
    class RegisterType {public:
        RegisterType();
    };
    static RegisterType registerType;
public:
    Test1() {}
    virtual ~Test1() {}
    void foo() override { std::cout<<"Test1:foo"<<std::endl; }
};

Q_DECLARE_METATYPE( std::shared_ptr<Test1Base> )
Q_DECLARE_METATYPE( std::shared_ptr<Test1> )

Test1::RegisterType::RegisterType() {
    cct::registerConverter<Test1,Test1Base>();
    cct::registerVirtualConverter<Test1Base,Test1>();
}

Test1::RegisterType Test1::registerType;

void test1() {

    QVariant v;
    v=cct::from(new Test1);

    auto v1=cct::convert<Test1Base>(v);
    auto v2=cct::convert<Test1>(v1);

    cct::data<Test1>(v2).foo();

}

void test2() {
}

void test() {


    QVariant v;


    v=cct::from(12);
    v=cct::from(new Test);

    QVariant vx(v);

    auto v1 = cct::convert<QObject>(v);
    auto v2 = cct::convert<Test>( v1 );
    cct::const_data<Test>(v2).foo();

}

#if !defined(HPP_QSHARED_POINTER_STD)
#define HPP_QSHARED_POINTER_STD

#include <type_traits>
#include <QObject>
#include <functional>
Q_DECLARE_METATYPE( std::weak_ptr<QObject> )
namespace cct {

void qPointerDeleteFunction(QObject * obj,const QPointer<QObject> & qPointer_) {
    if ((qPointer_)&&obj) {
        if (obj->parent()) {
            /*this object will be deleted by parent*/
            return;
        }
        QVariant doNotDelete=obj->property("@.@~delete");
        if (doNotDelete.isValid()&&doNotDelete.toBool()) {
            /*user set it do not need delete*/
            return;
        }
        /*delete the pointer*/
        obj->deleteLater();
    }
}

void qSharedPointerMarkNeedDelete(const QObject * const & p,bool v=false) {
    if (p==nullptr) { return; }
    v=!v;/**/
    (const_cast<QObject *>(p))->setProperty("@.@~delete",v);
}

void qSharedPointerMarkNeedDelete(const std::shared_ptr<QObject> & p,bool v=false) {
    qSharedPointerMarkNeedDelete(p.get(),v);
}

template<typename T>
void qSharedPointerMarkNeedDelete(const std::shared_ptr<T> & p,bool v=false) {
    qSharedPointerMarkNeedDelete(p.get(),v);
}

template<typename T,typename ... Args>
std::shared_ptr<T> makeQSharedPointer(Args && ... args) {
    T * ans_=new T(std::forward<Args>(args)...);
    QPointer<QObject> ans_QPointer_(ans_);
    std::function<void(QObject *)> delete_=std::bind(&qPointerDeleteFunction,
        std::placeholders::_1,std::move(ans_QPointer_));
    auto ans=std::shared_ptr<T>(ans_,std::move(delete_));
    {
        /*add weak pointer*/
        auto wans=std::weak_ptr<QObject>(ans);
        ans_->setProperty("@.@.weak_ptr",QVariant::fromValue(std::move(wans)));
    }
    return std::move(ans);
}

template<typename T_ >
auto makeQSharedPointer( const T_ * const & pointer_ )->std::shared_ptr< typename std::remove_cv<T_>::type > {
    typedef typename std::remove_cv<T_>::type T;
    if (pointer_==nullptr) { return std::shared_ptr<T>(nullptr); }
    {
        /*check weak pointer*/
        QVariant wPointer=pointer_->property("@.@.weak_ptr");
        if ( wPointer.isValid() ) {
            std::weak_ptr<QObject> wp = wPointer.value<std::weak_ptr<QObject>>();
            auto ans = wp.lock();
            if (ans) { return std::move( std::dynamic_pointer_cast<T>(ans) ); }
        }
    }

    auto * ans_=const_cast<T *>(pointer_);
    QPointer<QObject> ans_QPointer_(ans_);
    std::function<void(QObject *)> delete_=std::bind(&qPointerDeleteFunction,
        std::placeholders::_1,std::move(ans_QPointer_));
    auto ans = std::shared_ptr<T>(ans_,std::move(delete_));
    {
        /*add weak pointer*/
        auto wans=std::weak_ptr<QObject>( ans );
        ans_->setProperty("@.@.weak_ptr",QVariant::fromValue(std::move(wans)));
    }
    return std::move(ans);

}

template<typename T>
std::shared_ptr<T> makeQSharedPointer(const std::shared_ptr<T> & pointer_) {
    if (pointer_) {
        QObject * ans_=pointer_.get();
        auto wans=std::weak_ptr<QObject>( const_cast<std::shared_ptr<T> &>( pointer_ ) );
        ans_->setProperty("@.@.weak_ptr",QVariant::fromValue(std::move(wans)));
    }
    return pointer_;
}

}/*~cct*/

#endif

using namespace cct;

#include <iostream>
void foo( int a,int b ) {
    std::cout<<a<<std::endl;
    std::cout<<b<<std::endl;
}


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    MainWindow win;
    win.show();

    QPointer<QObject> sd;
    auto jjd = makeQSharedPointer<MainWindow>();
    makeQSharedPointer( jjd );
    makeQSharedPointer( jjd.get() );
    qSharedPointerMarkNeedDelete(jjd);

    std::bind(&foo,1,2)();
    std::bind(&foo,std::placeholders::_1,2)(1);
  //  std::function<void(int)> xx = std::bind(&foo,1,std::placeholders::_1) ;

   // std::function<void(QObject *)> delete_(std::bind(
    //    &qPointerDeleteFunction,sd,std::placeholders::_2
     //   ) );

    //auto jjd = makeQSharedPointer<MainWindow>();

    win.setProperty("@.@~delete",QVariant(true));
    QVariant v = win.property("@.@~delete");
    if (v.isValid()&&v.toBool()) {
        std::cout<<"need delete"<<std::endl;
    }

    std::cout << cct::IsShared<int>::value <<std::endl;
    std::cout << cct::IsShared< std::shared_ptr<int> >::value <<std::endl;

    test();
    test1();

    return a.exec();
}
