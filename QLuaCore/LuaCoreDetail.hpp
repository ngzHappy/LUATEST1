
#include "lua.hpp"

#if !defined(DETAIL_LUA_CORE__)
#define DETAIL_LUA_CORE__

#include <memory>
#include <functional>
#include <cstddef>
#include <utility>
#include <map>
#include <list>
#include <set>
#include <string>
#include <sstream>

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

inline void luaL_printTable( lua_State * L ,std::function<void(const std::string &)> print_ ) {
    enum DataType{
        Bool,Number,Integer,String,NIL
    };

    class Data {public:
        Data(  ):type(NIL) {}
        Data(bool i):type(Bool),boolean(i) {}
        Data(lua_Integer i):type(Integer),integer(i) {}
        Data(lua_Number i):type(Number),number(i) {}
        Data(const std::string & i):type(String),string(i) {}
        Data(std::string && i):type(String),string(std::move(i)) {}
        Data(const Data & v):type(v.type) {
            switch (type) {
                case Bool:boolean=v.boolean; break;
                case Number:number=v.number; break;
                case Integer:integer=v.integer; break;
                case String: {new(&string) std::string(v.string); }; break;
                case NIL:; break;
            }
        }

        Data( Data && v):type(v.type) {
            switch (type) {
                case Bool:boolean=v.boolean; break;
                case Number:number=v.number; break;
                case Integer:integer=v.integer; break;
                case String: {new(&string) std::string( std::move(v.string) ); }; break;
                case NIL:; break;
            }
        }

        Data & operator=( const Data & v ) {
            if (this==&v) { return *this; }
            if (v.type==String) { 
                if (type==String) { string=v.string; }
                else {
                    type=String;
                    new (&string) std::string( v.string );
                }
            }
            else {
                if (type==String) {
                    typedef std::string _T_;
                    string.~_T_();
                }
                switch (v.type) {
                    case Bool:{type=Bool; boolean=v.boolean; } break;
                    case Number: {type=Number; number=v.number; }break;
                    case Integer: {type=Integer; integer=v.integer; }break;
                    case String: {}break;
                    case NIL: {type=NIL; }break;
                }
            }

            return *this;
        }

        Data & operator=( Data && v ) {
            if (this==&v) { return *this; }
            if (v.type==String) { 
                if (type==String) { string = std::move( v.string ); }
                else {
                    type=String;
                    new (&string) std::string( std::move( v.string ) );
                }
            }
            else {
                if (type==String) {
                    typedef std::string _T_;
                    string.~_T_();
                }
                switch (v.type) {
                    case Bool: {type=Bool; boolean=v.boolean; }break;
                    case Number: {type=Number; number=v.number; }break;
                    case Integer: {type=Integer; integer=v.integer; }break;
                    case String: {}break;
                    case NIL: {type=NIL; }break;
                }
            }

            return *this;
        }

        ~Data() {
            if (type==String) { typedef std::string _T_; string.~_T_(); }
        }

        DataType type;
        union {
            bool boolean;
            lua_Number number;
            lua_Integer integer;
            std::string string;
        };
    };

    const auto to_string=[](auto i) { std::stringstream ss; ss<<i; std::string ans; ss>>ans; return std::move(ans); };

    const auto oprint_key=[&print_,&to_string](const Data & v) {
        const DataType & type=v.type;
        switch (type) {
            case DataType::Number: { print_("["); print_(to_string(v.number)); print_("]"); }; break;
            case DataType::Integer: { print_("["); print_(to_string(v.integer)); print_("]"); }; break;
            case DataType::String: {print_("[\""); print_(v.string); print_("\"]"); }; break;
        }
    };

    const auto string_find=[](const std::string & s,const auto & v) {return (s.find(v)!=s.npos);};

    const auto oprint_value=[&print_,&to_string,&string_find](const Data & v) {
        const DataType & type=v.type;
        switch (type) {
            case DataType::Bool: { if (v.boolean) { print_("true"); } else {print_("false");} }break;
            case DataType::Number: {  print_(to_string(v.number));  }; break;
            case DataType::Integer: { print_(to_string(v.integer));  }; break;
            case DataType::String: {
                std::string op_="==";
                while ((string_find(v.string,"["+op_+"["))||string_find(v.string,"]"+op_+"]")) { op_+="="; }
                print_("["+op_+"[");
                print_(v.string); 
                print_("]"+op_+"]");
            }; break;
            case DataType::NIL: print_("nil"); ; break;
        }
    };

    const auto print_endl=[&print_]() {print_("\n"); };
    const auto print_line_endl=[&print_]() {print_(" ;\n"); };
    const auto print_file_begin=[&print_](const std::string & tbname) {
        print_(tbname);
        print_(" = (function()                    \n\nlocal ");
    };

    const auto print_file_end=[&print_](const std::string & tbname) {
        print_("\nreturn "+tbname);
        print_("; end)()");
    };

    const auto table_name_full=[&to_string]( const std::list< Data > & tables,const Data & ctable )->std::string {
        auto root_name_=tables.begin();
        auto end_name_=tables.end();
        std::string ans;
        ans=root_name_->string;
        for (auto next_name_=root_name_;(++next_name_)!=end_name_; ) {
            const auto &current_table_name=*next_name_;
            switch ( current_table_name.type ) {
                case DataType::Integer: ans+=std::move( "["+to_string(current_table_name.integer)+"]" ); break;
                case DataType::Number: ans+=std::move( "["+to_string(current_table_name.number)+"]" ); ; break;
                case DataType::String: ans+=std::move("[\""+current_table_name.string+"\"]") ; break;
            }
        }
        const auto &current_table_name= ctable;
        switch ( current_table_name.type ) {
            case DataType::Integer: ans+=std::move( "["+to_string(current_table_name.integer)+"]" ); break;
            case DataType::Number: ans+=std::move( "["+to_string(current_table_name.number)+"]" ); ; break;
            case DataType::String: ans+=std::move("[\""+current_table_name.string+"\"]") ; break;
        }
        return std::move(ans);
    };

    const auto is_integer=[]( const std::string & v )->bool {
        if (v.empty()) { return false; }
        const auto i=v[0];if ((i>='0')&&(i<='9')) { return true; }
        return false;
    };

    /*-----------------------------------------------*/
    if (lua_gettop(L)<2) { lua_pushstring(L,"you must input table name and table"); lua_error(L); }
    if (lua_isstring(L,1)==false) {lua_pushstring(L,"table name must a string"); lua_error(L);}
    if (lua_istable(L,2)==false) {lua_pushstring(L,"you must input a table"); lua_error(L);}

    /* copy the table twice */
    lua_pushvalue(L,2);
    lua_pushvalue(L,2);

    const std::string tbname( lua_tostring(L,1) );
    print_file_begin(tbname);

    /*-----------------------------------------------*/
    std::list< Data > print_tables_names{ tbname };
    std::list< int > print_tables{2};
    std::list< std::string > final_print_lines;
    std::map< const void *,std::string > all_tables{ {lua_topointer(L,2),tbname} };

print_next_start:
    while ( print_tables.empty()==false ) {

        int current_table=*print_tables.rbegin();
        Data current_table_name= std::move( *print_tables_names.rbegin() ) ;

        print_tables.pop_back();
        print_tables_names.pop_back();

        if ( lua_istable(L,-1) ) {
            const static constexpr char start_string[]{" = {  --[[  --]]    \n"};
            switch ( current_table_name.type ) {
                case DataType::Integer: print_( "["+to_string(current_table_name.integer)+"]"+start_string ); break;
                case DataType::Number: print_( "["+to_string(current_table_name.number)+"]"+start_string ); ; break;
                case DataType::String: {
                    if (is_integer(current_table_name.string)) {
                        print_("[\""+current_table_name.string+"\"]"+start_string);
                    }
                    else {
                        print_(current_table_name.string+start_string);
                    }
                } break;
            }
            lua_pushnil(L);
        }

        while ( lua_next(L,current_table) != 0) {
            /* uses 'key' (at index -2) and 'value' (at index -1) */
            if ( lua_istable(L,-1) ) {

                Data key_;
                auto type_ = lua_type(L,-2);
                switch (type_) {
                    case LUA_TSTRING: key_=std::string(lua_tostring(L,-2));  break;
                    case LUA_TNUMBER:key_=lua_tointeger(L,-2); break;
                    default: lua_pushstring(L,"key must be number or string"); lua_error(L); ; break;
                }

                const void * table_pointer=lua_topointer(L,-1);
                auto ctable = all_tables.find( table_pointer );
                const auto etable=all_tables.end();

                if ( ctable == etable ) {
                    /*find a new table*/

                    /*save old value*/
                    print_tables.push_back( current_table );
                    print_tables_names.push_back( std::move(current_table_name) );

                    all_tables.insert({table_pointer,table_name_full( print_tables_names,key_ ) });              

                    current_table=lua_gettop(L);
                    current_table_name=std::move(key_);

                    /*set new value*/
                    print_tables.push_back( current_table );
                    print_tables_names.push_back( std::move(current_table_name) );

                    goto print_next_start;
                }
                else {
                    /*circle table*/
                    lua_pop(L,1);/*pop the table*/

                    print_tables_names.push_back( std::move(current_table_name) );
                    auto this_table_name_ = table_name_full(print_tables_names,key_);

                    current_table_name= std::move( * print_tables_names.rbegin() );
                    print_tables_names.pop_back();

                    final_print_lines.push_back(std::move(this_table_name_)+" = "+ ctable->second + "\n");
                }

            }
            else {
                Data key_,value_; 
                bool do_not_support=false;
                auto type_ = lua_type(L,-2);
                switch (type_) {
                    case LUA_TSTRING: key_=std::string(lua_tostring(L,-2));  break;
                    case LUA_TNUMBER:key_=lua_tointeger(L,-2); break;
                    default: lua_pushstring(L,"key must be number or string"); lua_error(L); ; break;
                }

                type_=lua_type(L,-1);
                switch (type_) {
                    case LUA_TNIL: break;
                    case LUA_TBOOLEAN: value_=bool( lua_toboolean(L,-1) ); break;
                    case LUA_TSTRING: value_=std::string(lua_tostring(L,-1)); break;
                    case LUA_TNUMBER: value_=lua_tonumber(L,-1); break;
                    case LUA_TFUNCTION: value_=std::string("function() end"); break;
                    default: do_not_support=true ; break;
                }

                if (do_not_support) { lua_pop(L,1); continue; }

                oprint_key(key_);
                print_(" = ");
                oprint_value(value_);
                print_line_endl();

                lua_pop(L,1);
            }
        }/*while table*/

        if ( print_tables.empty() ) {
            print_("} --[[  --]] \n");
        }
        else {
            print_("} , --[[  --]] \n");
        }

        lua_pop(L,1);
    }

    if ( false == final_print_lines.empty() ) {
        print_endl();
        for (const auto & line:final_print_lines) {
            print_(line);
        }
    }

    print_file_end(tbname);

    print_endl();
    print_("--[[***********************--]]\n");
    print_endl();

    return ;
}

#endif

