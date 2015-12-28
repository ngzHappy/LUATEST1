
#if !defined( ITETATOR__HPP__CCT__ )
#define ITETATOR__HPP__CCT__

#include <memory>
#include <type_traits>
#include <iterator>

namespace cct {

namespace _private {

template<typename T>
class ____HasKeyValue  {
private:

    typedef typename std::remove_cv< typename std::remove_reference< T >::type >::type T_;

    class Yes ; class No;

    template<typename U >
    static Yes check_key( typename std::remove_reference< decltype( std::declval<U>().key() ) >::type * );
    template<typename U >
    static No check_key( ... );

    template<typename U >
    static Yes check_value( typename std::remove_reference< decltype( std::declval<U>().value() ) >::type * );
    template<typename U >
    static No check_value( ... );

public:
    enum{
        value =
        (std::is_same<Yes ,decltype( check_value<T_>( 0 ) ) >::value) &&
        (std::is_same<Yes ,decltype( check_value<T_>( 0 ) ) >::value)
        };
};

template<typename T>
class __HasKeyValue :
    public std::integral_constant<bool, (____HasKeyValue< typename std::remove_reference<T>::type >::value==true) > {
};

}

template< typename ... ValueType >
class Iterator;

template< typename ValueType >
class Iterator<ValueType> {
private:
    typedef typename std::remove_reference<ValueType>::type type___;
    typedef typename std::remove_cv<type___>::type type____;
    typedef type____ value_type_;
private:
    class IteratorDataCore {
    public:
        virtual void copy_instance(IteratorDataCore **)=0;
        virtual void copy_equal(IteratorDataCore **)=0;
        virtual void add()=0;
        virtual bool not_equal(IteratorDataCore *) const=0;
        virtual const value_type_ & value()=0;
        virtual const value_type_ & value() const=0;
        virtual ~IteratorDataCore()=default;
    };

    IteratorDataCore * data__;

    template<typename U>
    class IteratorData :
        public IteratorDataCore {
    public:
        typedef typename std::remove_cv< typename std::remove_reference<U>::type >::type type_;
        type_ data_;
    public:
        IteratorData(const type_ & v):data_(v) {}
        void copy_instance(IteratorDataCore ** u) override final {
            *u=new IteratorData(data_);
        }
        void copy_equal(IteratorDataCore ** v) override final {
            auto * u_=dynamic_cast<IteratorData *>(*v);
            if (u_) {
                u_->data_=data_;
            }
            else {
                delete *v;
                copy_instance(v);
            }
        }
        void add() override final { ++data_; }
        bool not_equal(IteratorDataCore * u) const override final {
            if (u==nullptr) { return false; }
            auto * u_=dynamic_cast<IteratorData *>(u);
            if (u_) {
                return (u_->data_)!=data_;
            }
            return false;
        }

        const value_type_ & value() override final { return *data_; }
        const value_type_ & value() const override final { return *data_; }

    };

    void release() { delete data__; data__=nullptr; }

public:

    Iterator():data__(nullptr) {}
    ~Iterator() { release(); }

    Iterator(Iterator && v):data__(v.data__) {
        v.data__=nullptr;
    }

    Iterator(const Iterator & v) {
        if (v.data__) {
            v.data__->copy_instance(&data__);
            return;
        }
        data__=nullptr;
    }

    Iterator& operator=(Iterator && v) {
        if (this==&v) { return *this; }
        if (data__==v.data__) { return *this; }
        release();
        data__=v.data__;
        v.data__=nullptr;
        return *this;
    }

    Iterator& operator=(const Iterator & v) {
        if (this==&v) { return *this; }
        if (data__==v.data__) { return *this; }
        if (v.data__) {
            v.data__->copy_equal(&data__);
        }
        else { release(); }
        return *this;
    }

    Iterator & operator++() {
        if (data__) { data__->add(); }return *this;
    }
    Iterator operator++(int) {
        if (data__) {
            Iterator tmp(*this);
            data__->add();
            return std::move(tmp);
        }return *this;
    }

    template<typename U>
    Iterator(const U & u):
        data__(new IteratorData< typename std::remove_cv< typename std::remove_reference<U>::type >::type >(u)) {}

    value_type_ & operator*() { return const_cast<value_type_ &>(data__->value()); }
    const value_type_ & operator*() const { return data__->value(); }

    value_type_ * operator->() { return &(const_cast<value_type_ &>(data__->value())); }
    const value_type_ * operator->()const { return &(data__->value()); }

    friend bool operator!=(const Iterator & l,const Iterator & r) {
        if (l.data__==nullptr) { return false; }
        if (l.data__==r.data__) { return false; }
        return l.data__->not_equal(const_cast<IteratorDataCore *>(r.data__));
    }

};

template<
    typename KeyType,
    typename ValueType >
class Iterator<KeyType,ValueType> {
private:
    typedef typename std::remove_reference<ValueType>::type type___;
    typedef typename std::remove_cv<type___>::type type____;
    typedef typename std::remove_reference<KeyType>::type ___type;
    typedef typename std::remove_cv<___type>::type ____type;
    typedef const ____type key_type_;
    typedef type____       value_type_;
private:

    template<typename KV_, bool ISKV_ = false  >
    class HasKeyValue : public KV_ {
        KV_ & data1_() { return *this; }
        const KV_ & data1_()const { return *this; }
    public:
        using KV_::KV_;
        HasKeyValue() {}
        HasKeyValue(const KV_ &v):KV_(v) {}
        HasKeyValue(KV_ &&v):KV_(std::move(v)) {}
        const value_type_ & value() const { return data1_()->second; }
        key_type_ & key() const { return data1_()->first; }
    };

    template<typename KV_ >
    class HasKeyValue<KV_,true >
        :public KV_ {
    public:
        using KV_::KV_;
        HasKeyValue() {}
        HasKeyValue(const KV_ &v):KV_(v) {}
        HasKeyValue(KV_ &&v):KV_(std::move(v)) {}
        key_type_ & key() const { return KV_::key(); }
        const value_type_ & value() const { return KV_::value(); }
    };

    class IteratorDataCore {
    public:
        virtual void copy_instance(IteratorDataCore **)=0;
        virtual void copy_equal(IteratorDataCore **)=0;
        virtual void add()=0;
        virtual bool not_equal(IteratorDataCore *) const=0;
        virtual const value_type_ & value() const=0;
        virtual const key_type_ & key()const=0;
        virtual ~IteratorDataCore()=default;
    };

    IteratorDataCore * __data__;

    template<typename U>
    class IteratorData :
        public IteratorDataCore {
    public:
        typedef typename std::remove_cv< typename std::remove_reference<U>::type >::type type_;
        HasKeyValue< type_ ,_private::__HasKeyValue<type_>::value > data_;
    public:

        IteratorData(const type_ & v):data_(v) {}
        void copy_instance(IteratorDataCore ** u) override final {
            *u=new IteratorData(data_);
        }

        void copy_equal(IteratorDataCore ** v) override final {
            auto * u_=dynamic_cast<IteratorData *>(*v);
            if (u_) {
                u_->data_=data_;
            }
            else {
                delete *v;
                copy_instance(v);
            }
        }

        void add() override final { ++data_; }

        bool not_equal(IteratorDataCore * u) const override final {
            if (u==nullptr) { return false; }
            auto * u_=dynamic_cast<IteratorData *>(u);
            if (u_) {
                return (u_->data_)!=data_;
            }
            return false;
        }

        const value_type_ & value() const override final { return data_.value(); }
        key_type_ & key()const override final { return data_.key(); }
    };

    void release() { delete __data__; __data__=nullptr; }

public:

    Iterator():__data__(nullptr) {}
    ~Iterator() { release(); }

    Iterator(Iterator && v):__data__(v.__data__) {
        v.__data__=nullptr;
    }

    Iterator(const Iterator & v) {
        if (v.__data__) {
            v.__data__->copy_instance(&__data__);
            return;
        }
        __data__=nullptr;
    }

    Iterator& operator=(Iterator && v) {
        if (this==&v) { return *this; }
        if (__data__==v.__data__) { return *this; }
        release();
        __data__=v.__data__;
        v.__data__=nullptr;
        return *this;
    }

    Iterator& operator=(const Iterator & v) {
        if (this==&v) { return *this; }
        if (__data__==v.__data__) { return *this; }
        if (v.__data__) {
            v.__data__->copy_equal(&__data__);
        }
        else { release(); }
        return *this;
    }

    Iterator & operator++() {
        if (__data__) { __data__->add(); }return *this;
    }
    Iterator operator++(int) {
        if (__data__) {
            Iterator tmp(*this);
            __data__->add();
            return std::move(tmp);
        }return *this;
    }

    template<typename U>
    Iterator(const U & u):__data__(nullptr) {
        typedef typename std::remove_cv< typename std::remove_reference<U>::type >::type U__;
        __data__=new IteratorData< U__ >(u);
    }

    key_type_ & key() const { return __data__->key(); }
    value_type_ & value() { return const_cast<value_type_ &>(__data__->value()); }
    const value_type_ & value() const { return __data__.value(); }

    value_type_ & operator*() { return const_cast<value_type_ &>(__data__->value()); }
    const value_type_ & operator*() const { return __data__->value(); }

    value_type_ * operator->() { return &(const_cast<value_type_ &>(__data__->value())); }
    const value_type_ * operator->()const { return &(__data__->value()); }

    friend bool operator!=(const Iterator & l,const Iterator & r) {
        if (l.__data__==nullptr) { return false; }
        if (l.__data__==r.__data__) { return false; }
        return l.__data__->not_equal(const_cast<IteratorDataCore *>(r.__data__));
    }

};

template<typename KV_,bool ISKV_= _private::__HasKeyValue<KV_>::value  >
class LinkedIterator : public KV_ {
    KV_ & data1_() { return *this; }
    const KV_ & data1_()const { return *this; }
    const auto & key_() const { return data1_()->first; }
    const auto & value_() const { return data1_()->second; }
public:
    using KV_::KV_;
    LinkedIterator() {}
    LinkedIterator(const KV_ &v):KV_(v) {}
    LinkedIterator(KV_ &&v):KV_(std::move(v)) {}
    LinkedIterator(const LinkedIterator &)=default;
    LinkedIterator(LinkedIterator &&)=default;
    const auto & value()const { return value_(); }
    const auto & key() const { return key_(); }
    const auto * operator->()const { return &(value_()); }
    auto & value() {
        typedef typename std::remove_cv< typename std::remove_reference< decltype(value_()) >::type >::type A_;
        return const_cast<A_ &>(value_());
    }
    auto * operator->() {
        typedef typename std::remove_cv< typename std::remove_reference< decltype(value_()) >::type >::type A_;
        return  &(const_cast<A_ &>(value_()));
    }
};

template<typename KV_ >
class LinkedIterator<KV_,true >
    :public KV_ {
    const auto & key_() const { return KV_::key(); }
    const auto & value_()const { return KV_::value(); }
public:
    using KV_::KV_;
    LinkedIterator() {}
    LinkedIterator(const KV_ &v):KV_(v) {}
    LinkedIterator(KV_ &&v):KV_(std::move(v)) {}
    LinkedIterator(const LinkedIterator &)=default;
    LinkedIterator(LinkedIterator &&)=default;
    const auto & value()const { return value_(); }
    const auto * operator->()const { return &(value_()); }
    const auto & key() const { return key_(); }
    auto & valule() {
        typedef typename std::remove_cv< typename std::remove_reference< decltype(value_()) >::type >::type A_;
        return const_cast<A_ &>(value_());
    }
    auto * operator->() {
        typedef typename std::remove_cv< typename std::remove_reference< decltype(value_()) >::type >::type A_;
        return  &(const_cast<A_ &>(value_()));
    }
};

template<typename T>
auto makeLinkedIterator(T && i) {
    typedef typename std::remove_cv< typename std::remove_reference<T>::type >::type T_;
    return LinkedIterator<T_>(std::forward<T>(i));
}

}/*~cct*/

#endif



