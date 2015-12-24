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

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    MainWindow win;
    win.show();

    std::cout << cct::IsShared<int>::value <<std::endl;
    std::cout << cct::IsShared< std::shared_ptr<int> >::value <<std::endl;

    test();
    test1();

    return a.exec();
}
