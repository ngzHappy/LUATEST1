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

#include <initializer_list>

void fxx111( const std::initializer_list<QVariant> & v ) {
    for ( const auto & i:v ) {
        qDebug()<<i;
    }
}

void fxx222() {

    QList<int> t{1,2,3};
    
    QVariant v=QVariant::fromValue( t );

    auto it = v.value< QSequentialIterable >();

    for ( const auto & i:it ) {
        qDebug()<<i;
    }

}

void fxx333() {

    std::list<int> t{1,2,3};

    QVariant v=QVariant::fromValue( t );

    auto it = v.value< QSequentialIterable >();

    for ( const auto & i:it ) {
        qDebug()<<i;
    }

}

class AAA {
public:
    AAA() {}
    AAA(int) {}
    ~AAA() {}

    AAA(const AAA &) { std::cout<<"-1"<<std::endl; }
    AAA(AAA &&) { std::cout<<"-2"<<std::endl;  }

    AAA&operator=(const AAA &) { std::cout<<"-3"<<std::endl; return *this; }
    AAA&operator=(AAA &&) { std::cout<<"-4"<<std::endl; return *this; }

};

Q_DECLARE_METATYPE( AAA )

template<typename T>
class NList : 
    public std::shared_ptr< std::list<T> >{
    typedef std::shared_ptr< std::list<T> > Super;

    typedef std::list<T> C_;
    C_ * data_() { return this->get(); }
    const C_ * data_()const { return this->get(); }
    
public:
    typedef typename C_::value_type value_type;
    typedef typename C_::const_iterator const_iterator;
public:
    template<typename ... Args>
    NList( Args && ... args ):Super( std::make_shared<Super>(std::forward<Args>(args)...) ){}
    NList():Super( new std::list<T> ) {}

    //auto size()const { return data_()->size(); }
    auto begin()const { return data_()->begin(); }
    auto end() const { return data_()->end(); }
};

#include <map>
Q_DECLARE_SEQUENTIAL_CONTAINER_METATYPE( NList )

namespace cct{

template<typename T,typename U>
class Map : public std::shared_ptr< std::map<T,U> > {
private:
    typedef std::shared_ptr< std::map<T,U> > Super;
public:

    Map( decltype(nullptr) ) {}
    Map() :Super(new std::map<T,U> ){}
    Map(const Map &)=default;
    Map(Map &&)=default;
    Map(Super && o):Super( std::move(o) ) {}
    Map(const Super & o):Super( o ) {}
    Map(const std::map<T,U> & o):Super( new std::map<T,U>( o ) ) {}
    Map(std::map<T,U> && o):Super( new std::map<T,U>( std::move(o) ) ) {}

    Map&operator=(const Map&)=default;
    Map&operator=(Map&&)=default;

    Map copy() const { return Map( *(*this) ); }
    Map unique_copy() const { if (this->use_count()<2) { return *this; }return copy(); }
private:
    typedef std::map<T,U> AssociativeContainer;
    AssociativeContainer * data_() { return this->get(); }
    const AssociativeContainer * data_()const { return this->get(); }
public:
    typedef typename AssociativeContainer::key_type key_type;
    typedef typename AssociativeContainer::mapped_type mapped_type;
    typedef typename AssociativeContainer::const_iterator const_iterator;
    auto begin()const { return data_()->begin(); }
    auto end()const { return data_()->end(); }
    auto begin() { return data_()->begin(); }
    auto end() { return data_()->end(); }
    auto find(const key_type & v) const { return data_()->find(v); }
};

}

Q_DECLARE_ASSOCIATIVE_CONTAINER_METATYPE( cct::Map )

namespace cct{

template<typename T,typename U>
class Multimap : public std::shared_ptr< std::multimap<T,U> > {
private:
    typedef std::shared_ptr< std::multimap<T,U> > Super;
public:

    Multimap( decltype(nullptr) ) {}
    Multimap() :Super(new std::multimap<T,U> ){}
    Multimap(const Multimap &)=default;
    Multimap(Multimap &&)=default;
    Multimap(Super && o):Super( std::move(o) ) {}
    Multimap(const Super & o):Super( o ) {}
    Multimap(const std::multimap<T,U> & o):Super( new std::multimap<T,U>( o ) ) {}
    Multimap(std::multimap<T,U> && o):Super( new std::multimap<T,U>( std::move(o) ) ) {}

    Multimap&operator=(const Multimap&)=default;
    Multimap&operator=(Multimap&&)=default;

    Multimap copy() const { return Multimap( *(*this) ); }
    Multimap unique_copy() const { if (this->use_count()<2) { return *this; }return copy(); }
private:
    typedef std::multimap<T,U> AssociativeContainer;
    AssociativeContainer * data_() { return this->get(); }
    const AssociativeContainer * data_()const { return this->get(); }
public:
    typedef typename AssociativeContainer::key_type key_type;
    typedef typename AssociativeContainer::mapped_type mapped_type;
    typedef typename AssociativeContainer::const_iterator const_iterator;
    auto begin()const { return data_()->begin(); }
    auto end()const { return data_()->end(); }
    auto begin() { return data_()->begin(); }
    auto end() { return data_()->end(); }
    auto find(const key_type & v) const { return data_()->find(v); }
};

}

Q_DECLARE_ASSOCIATIVE_CONTAINER_METATYPE( cct::Multimap )

#include <QSet>
void fxx777() {
    QSet<int> fse;
    fse.insert(12); fse.insert(13);
    QVariant v=QVariant::fromValue( fse );

    auto it = v.value<QSequentialIterable>();
    for ( const auto & i:it ) {
        qDebug()<< i ;
    }
}

void fxx444() {

    NList<AAA> t ;
    t->push_back(1);
    t->push_back(2);
    t->push_back(3);
  
    QVariant v=QVariant::fromValue( t );

    auto it = v.value< QSequentialIterable >();
    
    int xvalue=0;
    for ( const auto & i:it ) {
        qDebug()<<xvalue++;
    }

}

void fxx5555() {
    cct::Multimap<int,AAA> map;

    QVariant v=QVariant::fromValue( map );
    auto it=v.value<QAssociativeIterable>();
}


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    std::multimap<int,double> ssfe2;
    
    fxx777();
    fxx111({ "123",123,"dsfa" });
    fxx222();
    fxx333();
    fxx444();

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
