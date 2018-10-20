#include <string>
#include <iostream>


#include "zlogClass.h"

#define ZLOG_OUT 1

zlogClass::zlogClass(){


}

zlogClass::zlogClass( std::string configpath, std::string logfilename)
	:config(configpath),
	logfile(logfilename)
	{
	int rc;
	

	rc = zlog_init(config.c_str());
	if (rc) {
		
		std::cout<<"  [zlog_init ] ................................failed"<<std::endl;
		throw -1;
	}

	c = zlog_get_category(logfile.c_str());
	if (!c) {
		
		std::cout<<"[zlog_init ] ..........................get manage failed"<<std::endl;
		zlog_fini();
		throw -2;
	}
	
}

zlogClass::~zlogClass(){
	
	zlog_fini();
	
}


void zlogClass::out_info( std::string log){
	#if ZLOG_OUT
	zlog_info(c, log.c_str());
	#endif

}
void zlogClass::out_error( std::string log){
	#if ZLOG_OUT
	zlog_error(c, log.c_str());
	#endif

}
void zlogClass::out_warn( std::string log){
	#if ZLOG_OUT
	zlog_warn(c, log.c_str());
	#endif

}
void zlogClass::out_notice( std::string log){
	#if ZLOG_OUT
	zlog_notice(c, log.c_str());
	#endif

}
void zlogClass::out_debug( std::string log){
	#if ZLOG_OUT
	zlog_debug(c, log.c_str());
	#endif

}

int zlogClass::init(){

	int rc;
	

	rc = zlog_init(config.c_str());
	if (rc) {
		
		std::cout<<"  [zlog_init ] ................................failed"<<std::endl;
		return -1;
	}

	c = zlog_get_category(logfile.c_str());
	if (!c) {
		
		std::cout<<"[zlog_init ] ..........................get manage failed"<<std::endl;
		zlog_fini();
		return -2;
	}
     return 1;

}
int zlogClass::init( std::string configpath, std::string logfilename){

	int rc;
	
	config = configpath;
	logfile = logfilename;
	
	rc = zlog_init(config.c_str());
	if (rc) {
		
		std::cout<<"  [zlog_init ] ................................failed  "<<config<<std::endl;
		return -1;
	}

	c = zlog_get_category(logfile.c_str());
	if (!c) {
		
		std::cout<<"[zlog_init ] ..........................get "<<logfile<<" .failed"<<std::endl;
		zlog_fini();
		return -2;
	}
     return 1;

}

