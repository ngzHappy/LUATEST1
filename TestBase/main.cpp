#include "MainWindow.hpp"
#include <QApplication>

#include <QLuaCore.hpp>

#include <iostream>

#include <string>
#include <set>
#include <map>
#include <list>


int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    auto * L=luaL_newstate();
    luaL_openlibs(L);
    

    {
        auto top_=lua_gettop(L);
        const char v[]=u8R"(print "Hellow Word!")";
        luaL_dostring<>(L,v);
        lua_settop(L,top_);
    }

    {
        auto top_=lua_gettop(L);
        lua_newtable(L);
        auto table_=lua_gettop(L);
        lua_pushlstring(L,"a",1);
        lua_pushboolean(L,false);
        lua_settable(L,table_);
        lua_setglobal(L,"t");
        luaL_dostring<>(L,"print(t.a)");
        lua_settop(L,top_);
    }

    {
        lua_register(L,"copyTable",&luaL_copyTable);
        luaL_dostring<>(L,u8R"(a={["t0"]=1,2,3,[9]={xy={"e","f","g"},"a","b","c"},bbx=33,ddy=22})");
        luaL_dostring<>(L,u8R"(a[8]=a[9])");
        luaL_dostring<>(L,"b=copyTable(a)");
        luaL_dostring<>(L," for k,v in pairs(b) do print(k ,v) end ");
        luaL_dostring<>(L," for k,v in pairs(b[9]) do print(k ,v) end ");
        std::cout<<"print xy"<<std::endl;
        luaL_dostring<>(L,u8R"( for k,v in pairs(b[9]["xy"]) do print(k ,v) end )");
        luaL_dostring<>(L,"b[9][1]=98");
        luaL_dostring<>(L," for k,v in pairs(b[9]) do print(k ,v) end ");
        luaL_dostring<>(L," for k,v in pairs(a[9]) do print(k ,v) end ");
    }

    {
        lua_getglobal(L,"t");
        std::cout<< std::boolalpha << lua_istable(L,-1) <<std::endl;
        std::cout<<const_cast<void *>(lua_topointer(L,-1))<<std::endl;
        lua_pushlightuserdata( L, const_cast<void *>(lua_topointer(L,-1)) );
        std::cout<< std::boolalpha << lua_istable(L,-1) <<std::endl;
    }

    MainWindow window;
    window.show();

    return app.exec();
}
