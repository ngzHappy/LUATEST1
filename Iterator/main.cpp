#include "MainWindow.hpp"
#include <QApplication>





#include <map>
#include <vector>
#include <iostream>
#include <QMap>
#include <QSet>



class S {public:
    void key()const {}
    void value() const{}
};

template<typename KV_ , typename ISKV_ = void  >
class HasKeyValue : public KV_ {
    KV_ & data_() { return *this; }
    const KV_ & data_()const { return *this; }
public:
    using KV_::KV_;
    HasKeyValue() {}
    HasKeyValue(const KV_ &v):KV_(v) {}
    HasKeyValue(KV_ &&v):KV_(std::move(v)) {}
    auto & key( ) const { return  data_()->first; }
    const auto & value( ) const { return  data_()->second; }
};

template<typename KV_ >
class HasKeyValue<KV_,std::void_t< decltype(std::declval<KV_>().key()),decltype(std::declval<KV_>().value()) > > 
    :public KV_{
public:
    using KV_::KV_;
    HasKeyValue() {}
    HasKeyValue(const KV_ &v):KV_(v) {}
    HasKeyValue(KV_ &&v):KV_(std::move(v)) {}
    auto & key() const { return KV_::key(); }
    const auto & value() const { return KV_::value(); }
};


#include "Itetator.hpp"
using namespace cct;

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    
    cct::_private::__HasKeyValue<int>::value;
    cct::_private::__HasKeyValue<S>::value;

 //   HasKeyValue<S>::value;

    std::vector<int> v{1,2,3,4,5};
    Iterator<int> begin_( v.begin() );
    Iterator<int> end_(v.end());
    
    std::map<int,double> dsfe;
    dsfe.insert({ 1,2 });
    QMap<int,double> fff;

    Iterator<int,double> xits( fff.begin() );
    
    auto li = makeLinkedIterator( dsfe.begin() );
    

    HasKeyValue< std::map<int,double>::iterator > d( dsfe.begin() );
    d.key();
    d.value();

    dsfe.begin()->first;
    
    QSet<int> ggg;

    Iterator< int > l( ggg.begin() );

    Iterator<  int,double > it( dsfe.begin() );
    it.key();
    
    for (; begin_!=end_;++begin_) {
        std::cout<<*begin_<<std::endl;
    }
       

    MainWindow w;
    w.show();

    return a.exec();
}
