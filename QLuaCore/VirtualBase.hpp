
#if !defined(CCT__TYPE__TRAITS__)
#define CCT__TYPE__TRAITS__

#if defined(_DEBUG)
#include <cassert>
#endif
#include <type_traits>
#include <memory>
#include <QtCore>

//Q_DECLARE_METATYPE( std::shared_ptr< QPointer<QFutureWatcher> > )
Q_DECLARE_METATYPE( std::shared_ptr< QPointer<QObject> > )
Q_DECLARE_METATYPE( std::shared_ptr< QPointer<QAbstractAnimation> > )
Q_DECLARE_METATYPE( std::shared_ptr< QPointer<QAbstractEventDispatcher> > )
Q_DECLARE_METATYPE( std::shared_ptr< QPointer<QAbstractItemModel> > )
Q_DECLARE_METATYPE( std::shared_ptr< QPointer<QAbstractState> > )
Q_DECLARE_METATYPE( std::shared_ptr< QPointer<QAbstractTransition> > )
Q_DECLARE_METATYPE( std::shared_ptr< QPointer<QCoreApplication> > )
Q_DECLARE_METATYPE( std::shared_ptr< QPointer<QEventLoop> > )
Q_DECLARE_METATYPE( std::shared_ptr< QPointer<QFileSelector> > )
Q_DECLARE_METATYPE( std::shared_ptr< QPointer<QFileSystemWatcher> > )
Q_DECLARE_METATYPE( std::shared_ptr< QPointer<QIODevice> > )
Q_DECLARE_METATYPE( std::shared_ptr< QPointer<QItemSelectionModel> > )
Q_DECLARE_METATYPE( std::shared_ptr< QPointer<QLibrary> > )
Q_DECLARE_METATYPE( std::shared_ptr< QPointer<QMimeData> > )
Q_DECLARE_METATYPE( std::shared_ptr< QPointer<QObjectCleanupHandler> > )
Q_DECLARE_METATYPE( std::shared_ptr< QPointer<QPluginLoader> > )
Q_DECLARE_METATYPE( std::shared_ptr< QPointer<QSettings> > )
Q_DECLARE_METATYPE( std::shared_ptr< QPointer<QSharedMemory> > )
Q_DECLARE_METATYPE( std::shared_ptr< QPointer<QSignalMapper> > )
Q_DECLARE_METATYPE( std::shared_ptr< QPointer<QSocketNotifier> > )
Q_DECLARE_METATYPE( std::shared_ptr< QPointer<QThread> > )
Q_DECLARE_METATYPE( std::shared_ptr< QPointer<QThreadPool> > )
Q_DECLARE_METATYPE( std::shared_ptr< QPointer<QTimeLine> > )
Q_DECLARE_METATYPE( std::shared_ptr< QPointer<QTimer> > )
Q_DECLARE_METATYPE( std::shared_ptr< QPointer<QTranslator> > )
Q_DECLARE_METATYPE( std::shared_ptr< QPointer<QWinEventNotifier> > )

namespace cct {

class NoCopyQVariant : 
    private QVariant {
public:

    using QVariant::data        ;
    using QVariant::userType    ;
    using QVariant::constData   ;
    using QVariant::typeName    ;
    using QVariant::typeToName  ;
    using QVariant::clear       ;
    using QVariant::isValid     ;
    using QVariant::convert     ;
    
    NoCopyQVariant( ) = default ;
    NoCopyQVariant( const QVariant & v ):QVariant(v){}
    NoCopyQVariant( QVariant && v ):QVariant( std::move( v ) ){}

    NoCopyQVariant(const NoCopyQVariant &) = delete          ;
    NoCopyQVariant(NoCopyQVariant &&) =default               ;
    NoCopyQVariant&operator=(const NoCopyQVariant &) = delete;
    NoCopyQVariant&operator=(NoCopyQVariant &&) =default     ;

    const QVariant & toQVariant() const { return *this; }
};

namespace _private {

template<typename T >
class __TypeDetailBase : public std::integral_constant<int,0> {
    typedef typename std::remove_reference<T>::type RR_;
    typedef typename std::remove_cv<RR_>::type RCVR_;
public:
    typedef RCVR_ type;
};

template<typename T >
class __IsQObject
    :public std::integral_constant</*0*/  bool,
    std::is_base_of< QObject, typename __TypeDetailBase<T>::type >::value
    /*0*/> {};

template<typename T ,bool IsQObject = __IsQObject<T>::value >
class __TypeDetail final : public std::integral_constant<int,1> {
public:
    typedef std::shared_ptr< typename __TypeDetailBase<T>::type > type;
};

template<typename T >
class __TypeDetail<T,true> final: public std::integral_constant<int,2>{
public:
    typedef std::shared_ptr< QPointer<typename __TypeDetailBase<T>::type> > type;
};

template<>class __TypeDetail<QString,false> final: public __TypeDetailBase<QString> {};
template<>class __TypeDetail<QByteArray,false> final: public __TypeDetailBase<QByteArray> {};
template<>class __TypeDetail<bool,false> final: public __TypeDetailBase<bool> {};
template<>class __TypeDetail<int,false> final: public __TypeDetailBase<int> {};
template<>class __TypeDetail<unsigned int,false> final: public __TypeDetailBase<unsigned int> {};
template<>class __TypeDetail<long long,false> final: public __TypeDetailBase<long long> {};
template<>class __TypeDetail<unsigned long long,false> final: public __TypeDetailBase<unsigned long long> {};
template<>class __TypeDetail<long,false> final: public __TypeDetailBase<long> {};
template<>class __TypeDetail<unsigned long,false> final: public __TypeDetailBase<unsigned long> {};
template<>class __TypeDetail<short,false> final: public __TypeDetailBase<short> {};
template<>class __TypeDetail<unsigned short,false> final: public __TypeDetailBase<unsigned short> {};
template<>class __TypeDetail<double,false> final: public __TypeDetailBase<double> {};
template<>class __TypeDetail<float,false> final: public __TypeDetailBase<float> {};

template<typename T>
class __IsSharedPointer :public std::integral_constant<int,0> {};
template<typename T>
class __IsSharedPointer< std::shared_ptr<T> > :public std::integral_constant<int,1> {};
template<typename T>
class __IsSharedPointer< std::shared_ptr< QPointer<T> > > :public std::integral_constant<int,2> {};

template<typename T,int isSharedPointer=__IsSharedPointer<T>::value >
class __GetData {
public:
    template<typename U>static decltype(auto) data(U && u) { return u; }
};

template<typename T>
class __GetData<T,1> {
public:
    template<typename U>static auto & data(U && u) {
        if (u) { return *u; }
        throw QString("data pointer is null");
    }
};

template<typename T>
class __GetData<T,2> {
public:
    template<typename U>static auto & data(U && u) {
        if (u) { auto qp = *u;
        if (qp) { return *qp; }
        throw QString("qpointer is null");
        }
        throw QString("data pointer is null");
    }
};

}/*~_private*/

template<typename T>
class TypeDetail {
    typedef typename std::remove_reference<T>::type RR_;
    typedef typename std::remove_cv<RR_>::type RCVR_;
public:
    typedef typename _private::__TypeDetail<RCVR_>::type type;
    enum {value = _private::__TypeDetail<RCVR_>::value };
};

template<typename T_  >
NoCopyQVariant convert(const QVariant & v) {
    QVariant ans_( v );
    typedef typename TypeDetail<T_>::type T;
    typedef typename std::remove_reference<T>::type RR_;
    typedef typename std::remove_cv<RR_>::type RCVR_;
    if (v.isValid()&&ans_.convert(qMetaTypeId<RCVR_>())) {
        return std::move(ans_);
    }
    const static QString to_typename_=QString(" to type: ")
        +QMetaType::typeName(qMetaTypeId<RCVR_>())
        +" fail ";
    throw "convert from type: "+QString(v.typeName())+to_typename_;
}

template<typename T_  >
NoCopyQVariant convert(const NoCopyQVariant & v) {
    return convert<T_>( v.toQVariant() );
}

template<typename T_ ,typename _lick_QVariant >
auto data(const _lick_QVariant & v) -> typename std::remove_cv<typename std::remove_reference<T_>::type>::type & {
    typedef typename TypeDetail<T_>::type T;
    typedef typename std::remove_reference<T>::type RR_;
    typedef typename std::remove_cv<RR_>::type RCVR_;
    typedef _private::__GetData<RCVR_> _Get;
#if defined(_DEBUG)/*{*/
    const static auto __totypeid__ = qMetaTypeId<RCVR_>();
    assert(v.userType()==__totypeid__);
#endif/*}*/
    void * ans_=const_cast<void *>(v.constData());
    if (ans_) {
        return  _Get::data(*(reinterpret_cast<RCVR_ *>(ans_)));
    }
    throw QString("value is null");
}

template<typename T,typename _lick_QVariant>
decltype(auto) const_data(const _lick_QVariant & v) {
    const auto & ans_=data<T>(v);
    return ans_;
}

template<typename T>
auto fromQObject( const T * data_ ){
    if (data_==nullptr) { throw QString("QObject pointer is null"); }
    typedef typename std::remove_reference<T>::type RR_;
    typedef typename std::remove_cv<RR_>::type RCVR_;
    static_assert( std::is_base_of<QObject,RCVR_>::value,"QObject must be parent of T" );
    typedef std::shared_ptr< QPointer<RCVR_> > AnsType;
    auto * qPointer_ = new QPointer< RCVR_ >( const_cast<RCVR_ *>( data_ ) );
    const auto delete_function_=[]( QPointer< RCVR_ > * d ) {
        if(d){auto qp=*d; if (qp) { qp->deleteLater(); } delete d;}
    };
    return AnsType( qPointer_,delete_function_ );
}

template<typename T>
auto fromType(const T * data_) {
    if (data_==nullptr) { throw QString("data pointer is null"); }
    typedef typename std::remove_reference<T>::type RR_;
    typedef typename std::remove_cv<RR_>::type RCVR_;
    static_assert( std::is_base_of<QObject,RCVR_>::value==false,"QObject must not be parent of T" );
    const auto delete_function_=[](RCVR_ * d) {delete d; };
    return std::shared_ptr<RCVR_>( const_cast<RCVR_ *>(data_),delete_function_);
}

namespace _private {

/*base type such as int double qstring .... */
template<typename T ,int Select = TypeDetail<T>::value > class __From {
public:
    template<typename U>
    static decltype(auto) value(U&& u) { return std::forward<U>(u); }
};

/*type not base an qobject*/
template<typename T > class __From<T,1> {
public:
    template<typename U>
    static decltype(auto) value(const U * u) { return cct::fromType(u); }
};

/*type is qobjct*/
template<typename T > class __From<T,2> {
public:
    template<typename U>
    static decltype(auto) value(const U * u) { return cct::fromQObject(u); }
};

}

namespace __private {

template<typename T1 >
class __IsShared {

    class Yes {};
    class No {};

    template<typename U>
    static Yes __select(std::shared_ptr<U> && );

    template<typename T>
    static Yes __select(const std::shared_ptr<T> & );

    template<typename X = void >
    static No __select( ... );

    typedef typename std::remove_reference<T1>::type NR_;
    typedef typename std::remove_cv<NR_>::type NCVR_;
    typedef decltype( __select(*(reinterpret_cast<NCVR_ *>( 0 ))) ) AnsTypeRef;
    typedef typename std::remove_reference<AnsTypeRef>::type AnsType;
public:
    enum { value = std::is_same< Yes,AnsType >::value };
};

}/*~__private*/

template<typename T>
class IsShared :
    public std::integral_constant<bool,
    ((__private::__IsShared<T>::value)==true)
    >{};

namespace __private {
template<bool IsSharedPointer_ = false >
class _From{public:
    template<typename U>
    static QVariant value( const U & data_ ) {return QVariant::fromValue( data_ );}
};

template<>
class _From<true>{public:
    template<typename U>
    static QVariant value( const std::shared_ptr<U> & data_ ) {
        return QVariant::fromValue( data_ );
    }
};

}

template<typename T>
auto from(const T & data_) {
    return __private::_From< IsShared<T>::value >::value( data_ );
}

template<typename T_>
QVariant from( T_* && data_) {
    typedef typename std::remove_pointer<T_>::type T;
    typedef typename std::remove_reference<T>::type RR_;
    typedef typename std::remove_cv<RR_>::type RCVR_;
    static_assert( cct::TypeDetail<RCVR_>::value > 0,"you shold not use basetype pointer" );
    return QVariant::fromValue( _private::__From<RCVR_>::value(data_) );
}

inline QVariant from( const char * const & data_ ) {
    return QVariant::fromValue( QString::fromUtf8(data_) );
}

inline QVariant from( const char16_t * const & data_ ) {
    return QVariant::fromValue( QString::fromUtf16(data_) );
}

inline QVariant from( const char32_t * const & data_ ) {
    return QVariant::fromValue( QString::fromUcs4(data_) );
}

namespace _private {

/* 0,0 */
template<typename From , typename To ,int FromID , int ToID >
class Converter {
public:
    static To converter(const From & v) { return To(v); }
};

/*0,1*/
template<typename From , typename To  >
class Converter<From,To,0,1> {
    template<typename U>
    static void _converter( const From & v,std::shared_ptr<U>  & ans ) {
        ans=std::make_shared<U>( v );
    }
public:
    static To converter(const From & v) { To ans; _converter(v,ans); return std::move(ans); }
};

/*0,2*/
template<typename From , typename To  >
class Converter<From,To,0,2> {
    template<typename U>
    static void _converter( const From & v, std::shared_ptr< QPointer<U> >  & ans ) {
        U * ans_=new U( v );
        auto * qp_ = new QPointer<U>( ans_ );
        ans=std::shared_ptr< QPointer<U> >(qp_,[](QPointer<U> * d) {
            if (d) { {auto & pq=*d; if (pq) { pq->deleteLater(); } } delete d; }
        });
    }
public:
    static To converter(const From & v) { To ans; _converter(v,ans); return std::move(ans); }
};

/* 1,0 */
template<typename From , typename To >
class Converter<From,To,1,0> {
public:
    static To converter(const From & v) { return To( *v ); }
};

/* 1,1 */
template<typename From , typename To  >
class Converter<From,To,1,1> {
    template<typename F,typename T>
    static void _converter( const std::shared_ptr<F> & f,std::shared_ptr<T> & t ) {t=f;}
public:
    static To converter(const From & v) { To ans; _converter(v,ans);return std::move(ans); }
};

/* 1,1 */
template<typename From , typename To  >
class Converter<From,To,1,2> {/*error!!*/};

/* 2,0 */
template<typename From , typename To >
class Converter<From,To,2,0> {
public:
    static To converter(const From & v) { return To( *(*v) ); }
};

/* 2,1 */
template<typename From , typename To  >
class Converter<From,To,2,1> {/*error!!*/};

/* 2,2 */
template<typename From , typename To  >
class Converter<From,To,2,2> {
    template<typename F,typename T>
    static void _converter(
        const std::shared_ptr< QPointer<F> > f,
        std::shared_ptr< QPointer<T> > & t ) {
        if (f) {
            QPointer<F> & f_ = *f;
            T * ans_=f_.data();
            if (ans_) {
                auto * pq=new QPointer<T>(ans_);
                t=std::shared_ptr< QPointer<T> >(pq, [f](QPointer<T> * d) {delete d; } );
            }
        }
    }
public:
    static To converter(const From & v) { To ans; _converter(v,ans);return std::move(ans); }
};

}

/**/
template<typename _From_,typename _To_>
bool registerConverter() {
    typedef typename TypeDetail<_From_>::type From;
    typedef typename TypeDetail<_To_>::type To;
    return QMetaType::registerConverter<From,To>( &(_private::Converter<From,To,TypeDetail<_From_>::value,TypeDetail<_To_>::value>::converter) );
}

namespace _private {
namespace _virtual {

/* 0,0 */
template<typename From , typename To ,int FromID , int ToID >
class Converter {};

template<typename From , typename To>
class Converter<From,To,1,1> {
    template<typename F,typename T>
    static void _converter( std::shared_ptr<F> & f,std::shared_ptr<T> & t ) {
        if (f) { t=std::dynamic_pointer_cast<T>(f); }
    }
public :
    static To converter(const From & v_) {
        auto & v = const_cast<From &>( v_ );
        To ans; _converter(v,ans);
        return std::move(ans);
    }
};

template<typename From , typename To>
class Converter<From,To,2,1> {
    template<typename F,typename T>
    static void _convert( const std::shared_ptr< QPointer<F> > & f,
        std::shared_ptr< T > & t
        ) {
        if ( f ) {
            QPointer<F> & p=*f;
            if ( p ) {
                auto * d = dynamic_cast<T *>(p.data());
                if (d) { t=std::shared_ptr< T >(f,d); }
            }
        }
    }
public:
    static To converter(const From & v ) {
        To ans; _converter(v ,ans);return std::move(ans);
    }
};

template<typename From , typename To>
class Converter<From,To,1,2> {
    template<typename F,typename T>
    static void _converter( const std::shared_ptr<F> f,
        std::shared_ptr< QPointer<T> > & t
        ) {
        if (f) {
            auto * d=dynamic_cast<T *>(f.get());
            if (d) {
                auto * pd=new QPointer<T>(d);
                t=std::shared_ptr< QPointer<T> >(pd,[f](QPointer<T> * _d) {delete _d; });
            }
        }
    }
public:
    static To converter(const From & v ) {
        To ans; _converter(v ,ans);return std::move(ans);
    }
};

template<typename From , typename To>
class Converter<From,To,2,2> {
    template<typename F,typename T>
    static void _converter( std::shared_ptr< QPointer<F> > f,std::shared_ptr< QPointer<T> > & t ) {
        if (f) {
            QPointer<F> & fqp=*f;
            F * data_ = fqp.data();
            if (data_) {
                T * value_=dynamic_cast<T *>(data_);
                if (value_) {
                    auto * qp = new QPointer<T>(value_);
                    t=std::shared_ptr< QPointer<T> >(qp,[f](QPointer<T> * d) {delete d; });
                }
            }
        }
    }
public :
    static To converter(const From & v ) {
        To ans; _converter(v ,ans);return std::move(ans);
    }
};

}
}

template<typename _From_,typename _To_>
bool registerVirtualConverter() {
    typedef typename TypeDetail<_From_>::type From;
    typedef typename TypeDetail<_To_>::type To;
    return QMetaType::registerConverter<From,To>( &(_private::_virtual::Converter<From,To,TypeDetail<_From_>::value,TypeDetail<_To_>::value>::converter) );
}

}/*~cct*/
#endif


