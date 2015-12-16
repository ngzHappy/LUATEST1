
#if !defined( HPP_LUA__QT__HPP__BASE__TYPE__ )
#define HPP_LUA__QT__HPP__BASE__TYPE__

#include <type_traits>
#include <memory>
#include <QVariant>
#include <QMetaType>
#include <QPointer>

namespace cct {

class VirtualRoot {
public:
    virtual ~VirtualRoot()=default;
    VirtualRoot()=default;
    VirtualRoot&operator=(const VirtualRoot &)=default;
    VirtualRoot&operator=(VirtualRoot &&)=default;
    VirtualRoot(const VirtualRoot &)=default;
    VirtualRoot(VirtualRoot &&)=default;
};

}//~cct

Q_DECLARE_METATYPE(std::shared_ptr<QObject>)
Q_DECLARE_METATYPE(std::shared_ptr<cct::VirtualRoot>)

namespace cct{
namespace private_ {
namespace cast_ {
const static auto T_QOBJECT_ID_=qMetaTypeId< std::shared_ptr<QObject> >();
const static auto T_VIRTUAL_ID_=qMetaTypeId< std::shared_ptr<VirtualRoot> >();

template<typename T_>
std::shared_ptr< T_ > cast(const QVariant & value_) {
    typedef std::shared_ptr< T_ > T;
    if (value_.isValid()==false) { return T{ nullptr }; }

    /*can convert */
    const static auto T_id_=qMetaTypeId< T >();
    if (value_.canConvert(T_id_)) { return value_.value<T>(); }

    /*virtual root*/
    if (value_.userType()==private_::cast_::T_VIRTUAL_ID_) {
        auto v=value_.value< std::shared_ptr<VirtualRoot> >();
        return std::dynamic_pointer_cast<T_>(v);
    }

    /*qobject*/
    if (value_.userType()==private_::cast_::T_QOBJECT_ID_) {
        auto v=value_.value< std::shared_ptr< QObject > >();
        return std::dynamic_pointer_cast<T_>(v);
    }
    return T{ nullptr };
}

template<typename T_>
class Cast {
public:
    static std::shared_ptr<T_> cast(const QVariant & v) { return cast_::cast<T_>(v); }
};

template<>
class Cast<double> {
public:
    static double cast(const QVariant & v) { return v.value<double>(); }
};

template<>
class Cast<QString> {
public:
    static QString cast(const QVariant & v) { return v.value<QString>(); }
};

template<>
class Cast<float> {
public:
    static float cast(const QVariant & v) { return v.value<float>(); }
};

template<>
class Cast<long> {
public:
    static long cast(const QVariant & v) { return v.value<long>(); }
};

template<>
class Cast<QByteArray> {
public:
    static QByteArray cast(const QVariant & v) { return v.value<QByteArray>(); }
};

template<>
class Cast<bool> {
public:
    static bool cast(const QVariant & v) { return v.value<bool>(); }
};

template<>
class Cast<long long> {
public:
    static long long cast(const QVariant & v) { return v.value<long long>(); }
};

template<>
class Cast<int> {
public:
    static int cast(const QVariant & v) { return v.value<int>(); }
};

template<>
class Cast<unsigned short> {
public:
    static unsigned short cast(const QVariant & v) { return v.value<unsigned short>(); }
};

template<>
class Cast<short> {
public:
    static short cast(const QVariant & v) { return v.value<short>(); }
};

template<>
class Cast<unsigned int> {
public:
    static unsigned int cast(const QVariant & v) { return v.value<unsigned int>(); }
};

template<
    typename T_,
    bool IsVirtual=std::has_virtual_destructor<T_>::value,
    bool IsQObject=std::is_base_of<QObject,T_>::value
>//false ,false
class CastTo {
public:
    typedef std::shared_ptr<T_> type;
    static type cast(T_ * v) { return type(v); }
    static type cast(const T_ * v) { return type(const_cast<type *>(v)); }
};

template<bool v0,bool v1>
class CastTo<int,v0,v1> {
public:
    typedef int type;
    static const int & cast(const int & value_) { return value_; }
    static int && cast(int && value_) { return std::move(value_); }
    static int & cast(int & value_) { return value_; }
};

template<bool v0,bool v1>
class CastTo<double,v0,v1> {
public:
    typedef double type;
    static const type & cast(const type & value_) { return value_; }
    static type && cast(type && value_) { return std::move(value_); }
    static type & cast(type & value_) { return value_; }
};

template<typename T_>
class CastTo<T_,true,false> {
public:
    typedef std::shared_ptr<T_> type;
    static type cast(T_ * value_) { return type(value_); }
    static type cast(const T_ * value_) { return type(const_cast<T_ *>(value_)); }
};

template<typename T_>
class CastTo<T_,true,true> {
public:
    typedef std::shared_ptr<T_> type;
    static type cast(T_ * value_) {
        type ans; QPointer<QObject> wp(value_);
        ans.reset(value_,[wp](auto *) {if (wp) { wp->deleteLater(); }});
        return std::move(ans);
    }
    static type cast(const T_ * value__) {
        T_ * value_=const_cast<T_ *>(value__);
        type ans; QPointer<QObject> wp(value_);
        ans.reset(value_,[wp](auto *) {if (wp) { wp->deleteLater(); }});
        return std::move(ans);
    }
};

template<bool v0,bool v1>
class CastTo<QString,v0,v1> {
public:
    typedef QByteArray type;
    static type cast(const QString & value_) { return value_.toUtf8(); }
};

template<bool v0,bool v1>
class CastTo<QByteArray,v0,v1> {
public:
    typedef QByteArray type;
    static const type & cast(const type & value_) { return value_; }
    static type && cast(type && value_) { return std::move(value_); }
    static type & cast(type & value_) { return value_; }
};

template<bool v0,bool v1>
class CastTo<float,v0,v1> {
public:
    typedef float type;
    static const type & cast(const type & value_) { return value_; }
    static type && cast(type && value_) { return std::move(value_); }
    static type & cast(type & value_) { return value_; }
};

template<bool v0,bool v1>
class CastTo<long long,v0,v1> {
public:
    typedef long long type;
    static const type & cast(const type & value_) { return value_; }
    static type && cast(type && value_) { return std::move(value_); }
    static type & cast(type & value_) { return value_; }
};

template<bool v0,bool v1>
class CastTo<short,v0,v1> {
public:
    typedef short type;
    static const type & cast(const type & value_) { return value_; }
    static type && cast(type && value_) { return std::move(value_); }
    static type & cast(type & value_) { return value_; }
};

template<bool v0,bool v1>
class CastTo<unsigned int,v0,v1> {
public:
    typedef unsigned int type;
    static const type & cast(const type & value_) { return value_; }
    static type && cast(type && value_) { return std::move(value_); }
    static type & cast(type & value_) { return value_; }
};

template<bool v0,bool v1>
class CastTo<unsigned short,v0,v1> {
public:
    typedef unsigned short type;
    static const type & cast(const type & value_) { return value_; }
    static type && cast(type && value_) { return std::move(value_); }
    static type & cast(type & value_) { return value_; }
};

template<bool v0,bool v1>
class CastTo<bool,v0,v1> {
public:
    typedef bool type;
    static const type & cast(const type & value_) { return value_; }
    static type && cast(type && value_) { return std::move(value_); }
    static type & cast(type & value_) { return value_; }
};

}//~cast_
}//~private_

template<typename T_>
inline decltype(auto) castTo(T_ && value) {
    typedef typename std::remove_reference<T_>::type NRT_;
    typedef typename std::remove_cv<NRT_>::type NCVRT_;
    typedef typename std::remove_pointer<NCVRT_>::type NPCVRT_;
    return private_::cast_::CastTo<NPCVRT_>::cast(std::forward<T_>(value));
}

template<typename T_>
inline decltype(auto) castTo(T_ * value) {
    typedef typename std::remove_reference<T_>::type NRT_;
    typedef typename std::remove_cv<NRT_>::type NCVRT_;
    typedef typename std::remove_pointer<NCVRT_>::type NPCVRT_;
    return private_::cast_::CastTo<NPCVRT_>::cast( value );
}

template<typename T_>
inline decltype(auto) castTo(const T_ * value) {
    typedef typename std::remove_reference<T_>::type NRT_;
    typedef typename std::remove_cv<NRT_>::type NCVRT_;
    typedef typename std::remove_pointer<NCVRT_>::type NPCVRT_;
    return private_::cast_::CastTo<NPCVRT_>::cast( value );
}

inline const char * castTo(const char * value) { return value; }

template<typename T_>
inline decltype(auto) cast(const QVariant & v) {
    typedef typename std::remove_reference<T_>::type NRT_;
    typedef typename std::remove_cv<NRT_>::type NCVRT_;
    return private_::cast_::Cast<NCVRT_>::cast(v);
}

}//~cct

#endif


