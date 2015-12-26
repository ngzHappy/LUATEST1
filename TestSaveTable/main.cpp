#include "MainWindow.hpp"
#include <QApplication>
#include <iostream>
#include <QLuaCore.hpp>

#include <functional>
#include <map>
#include <list>
#include <string>
#include <sstream>


#include <fstream>

int printTable(lua_State * L) {
    luaL_printTable(L,[](const std::string & v) { std::cout<<v; });
    return 0;
}

int saveTable(lua_State * L) {
    if (lua_gettop(L)<3) { 
        lua_pushstring(L,"you must input filename tablename and table"); lua_error(L); 
    }
    std::ofstream ofs( lua_tostring(L,1) );
    if ( ofs.is_open() ) {
        lua_remove(L,1);
        luaL_printTable(L,[&ofs](const std::string & v) { ofs<<v; });
    }
    else {
        lua_pushstring(L,"file can not be open"); lua_error(L);
    }
    return 0;
}

union A{
public:
    A() :i(0){}
    ~A() {  }
    A(const std::string & v_):v(v_) {}
    A(const int & i_):i(i_) {}
    std::string v;
    int i;
};

enum DataType{
    Number,Integer,String,NIL
};



int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    {
        A t( std::string("aaa") );
    }

    lua_State * L = luaL_newstate();
    luaL_openlibs(L);
    luaL_dostring(L,"b={c=true}");
    luaL_dostring(L,R"(a={c=true,["7"]=1,"[==[2ss",b={3,"4ee"},3,yy=function()end,4,5})");
    luaL_dostring(L,R"(a.f = a.b)");
    lua_register(L,"printTable",printTable);
    lua_register(L,"saveTable",saveTable);
    luaL_dostring(L,R"(printTable("b",b))");
    luaL_dostring(L,R"(printTable("a",a))");
    luaL_dostring(L,R"(saveTable("a.lua","a",a))");

    lua_close(L);
    
    MainWindow win;
    win.show();

    return a.exec();
}
