#include "QLuaCore.hpp"

std::thread::id QLuaCore::main_thread_id_ = std::this_thread::get_id();

QLuaCore::QLuaCore(){
    /* new a state */
    L = luaL_newstate() ;
    /* open standard libraries */
    luaL_openlibs(state_);
}


QLuaCore::~QLuaCore(){lua_close(L);}

std::shared_ptr< QLuaCore > QLuaCore::instance(){
    auto _core_ = new QLuaCore;
    std::shared_ptr< QLuaCore > ans( _core_,[](auto * d){delete d;} );

    /*init data*/
    _core_->this_thread_id_ = std::this_thread::get_id();

    /*register metatable*/
    {
        const auto & data_ = QLuaCore::getRegisterMetaTable();
        for( const auto & i_ : data_ ){if( i_ ){ i_(ans); }}
    }

    /*register type and functions*/
    {
        const auto & data_ = QLuaCore::getRegisterFunctions();
        for( const auto & i_ : data_ ){if( i_ ){ i_(ans); }}
    }

    return std::move( ans );
}

/*
*/
