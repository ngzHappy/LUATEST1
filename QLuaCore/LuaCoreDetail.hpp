
#include "lua.hpp"

#if !defined(DETAIL_LUA_CORE__)
#define DETAIL_LUA_CORE__

#include <map>
#include <list>
#include <set>

//[-0, +1, –]
template<size_t N>
inline int luaL_loadstring(lua_State *L, const char (&v)[N] ) {
    return luaL_loadbufferx(L, v, (N-1), v,nullptr);
}

//[-0, +?, –]
//It returns false if there are no errors or true in case of errors.
inline int luaL_dostring(lua_State *L,const char *str,size_t N) {
    return (luaL_loadbufferx(L,str,N,str,nullptr)||lua_pcall(L,0,LUA_MULTRET,0));
}

template<size_t N>
inline int luaL_dostring(lua_State *L,const char (&str)[N] ) {
    return luaL_dostring(L,str,N-1);
}


/* input a table and out put a table */
inline int luaL_copyTable( lua_State * L ) {

    if ( lua_istable(L,-1) ) {
        const auto source_table_top_=lua_gettop(L);

        std::list<int> from_,to_;
        from_.push_back( source_table_top_ );
        std::map< const void *,int > all_tables_;

        lua_newtable(L);/*create temp table*/
        const auto temp_table_=lua_gettop(L);

        lua_pushnil(L);
        lua_copy(L,-2,-1);/*copy soure table*/

        lua_newtable(L);
        const auto target_table_=lua_gettop(L);
        to_.push_back( target_table_ );/*create target table*/

        lua_pushnil(L);
        lua_pushnil(L);
        lua_copy(L,-4,-2);/*copy soure table*/
        lua_copy(L,-3,-1);/*copy target table*/

        int copy_table_count_=0;

    copy_next_table:
        while ( from_.empty()==false ) {
            const auto tf_=*from_.crbegin();
            const auto tt_=*to_.crbegin();
            from_.pop_back(); to_.pop_back();

            if (lua_istable(L,-1)) {
                lua_pushnil(L);
            }

            while (lua_next(L,tf_)!=0) {
                /*-1 value -2 key*/
                if ( lua_istable(L,-1) ) {
                    const void * table_pointer = lua_topointer(L,-1);
                    auto end_ = all_tables_.end();
                    auto pos_ = all_tables_.find( table_pointer );
                    if (end_!=pos_) {
                        /* copy value has exist */
                        lua_pop(L,1);/*remove old one*/
                        const auto ttt_=pos_->second;
                        lua_pushinteger( L,ttt_ );
                        lua_gettable(L,temp_table_);/*set value*/
                        lua_pushnil(L); lua_pushnil(L);
                        lua_copy(L,-3,-1); lua_copy(L,-4,-2);
                        lua_settable(L,tt_);
                        lua_pop(L,1);/*remove the table*/
                    }
                    else {
                        /* copy value not exist */
                        ++copy_table_count_;

                        all_tables_[table_pointer]=copy_table_count_;

                        lua_pushnil(L);
                        lua_pushinteger(L,copy_table_count_);
                        lua_newtable(L);
                        lua_copy(L,-1,-3);

                        /*set temp table*/
                        lua_settable(L,temp_table_);

                        /*set current table to temp table*/
                        lua_pushnil(L); lua_pushnil(L);
                        lua_copy(L,-3,-1);lua_copy(L,-5,-2);
                        lua_settable(L,tt_);

                        from_.push_back(tf_); 
                        to_.push_back(tt_);
                        auto ttt_=lua_gettop(L);
                        to_.push_back(ttt_);
                        from_.push_back(ttt_-1);

                        goto copy_next_table;
                    }
                }
                else {
                    /* copy value not table */
                    lua_pushnil(L); lua_pushnil(L);
                    lua_copy(L,-3,-1);lua_copy(L,-4,-2);
                    lua_settable( L,tt_ );/*remove key value*/
                    lua_pop(L,1);/*remove key*/
                }
            }/*~while*/

            lua_pop(L,2);/*remove source table , target table*/

        }/*all while*/

         /* return ans */
        lua_copy(L,target_table_,source_table_top_);
        lua_settop(L,source_table_top_);
        return 1;

    }/*~if*/

    lua_newtable(L);
    return 1;
}


#endif

