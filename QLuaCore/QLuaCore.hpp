#if !defined( QLUACORE_HPP )
#define QLUACORE_HPP

#include <memory>
#include <thread>
#include <cstddef>
#include <list>
#include <functional>
#include "qluacore_global.hpp"
#include "VirtualBase.hpp"
#include "LuaCoreDetail.hpp"

class QLUACORESHARED_EXPORT QLuaCore
    :public std::enable_shared_from_this<QLuaCore>
{
    QLuaCore&operator=(const QLuaCore &)=delete;
    QLuaCore&operator=(QLuaCore &&)=delete;
    QLuaCore(const QLuaCore &)=delete;
    QLuaCore(QLuaCore &&)=delete;
private:
    static std::thread::id main_thread_id_;
protected:
    std::thread::id this_thread_id_;
    union{lua_State * state_;lua_State * L;};
protected:
    virtual ~QLuaCore();
public:
    QLuaCore();

public:
    /* get main thread id */
    static std::thread::id mainThreadID(){return main_thread_id_;}

    /* thread_id */
    std::thread::id threadID() const{ return this_thread_id_; }

    /* this will create luacore */
    static std::shared_ptr< QLuaCore > instance();

    /*this will get register metatable*/
    static std::list< std::function<void( const std::shared_ptr<QLuaCore> & )> > & getRegisterMetaTable(){
        static std::list< std::function<void(const std::shared_ptr<QLuaCore> & )> > ans_;
        return ans_;
    }

    /*this will get register functions/types*/
    static std::list< std::function<void( const std::shared_ptr<QLuaCore> & )> > & getRegisterFunctions(){
        static std::list< std::function<void(const std::shared_ptr<QLuaCore> & )> > ans_;
        return ans_;
    }

    auto getTop()const { return lua_gettop(L); }
    auto getDeepth()const { return getTop(); }

    void setTop(int v) { lua_settop(L,v); }
    void setDeepth(int v) { setTop(v); }

    class QLUACORESHARED_EXPORT LuaStateDeepthLock {
        std::shared_ptr<QLuaCore> _this;
        int deepth_;
    public:
        LuaStateDeepthLock&operator=(const LuaStateDeepthLock &)=delete;
        LuaStateDeepthLock&operator=(LuaStateDeepthLock &&)=default;
        LuaStateDeepthLock(const LuaStateDeepthLock &)=delete;
        LuaStateDeepthLock(LuaStateDeepthLock &&)=default;
    public:
        LuaStateDeepthLock() {}
        LuaStateDeepthLock(const std::shared_ptr< QLuaCore> & v):_this(v) { if (v) { deepth_=v->getDeepth(); } }
        LuaStateDeepthLock(std::shared_ptr< QLuaCore> && v):_this(std::move(v)) { if (_this) { deepth_=_this->getDeepth(); } }
        ~LuaStateDeepthLock() { if (_this) { _this->setDeepth(deepth_); } }
    };

    LuaStateDeepthLock getDeepthLock() { return this->shared_from_this() ; }

    /*get lua state*/
    lua_State * getState()const { return L; }


};



#endif // QLUACORE_HPP
