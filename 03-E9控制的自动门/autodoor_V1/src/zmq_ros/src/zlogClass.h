#pragma once



#include "zlog.h"

#include <stdio.h> 


class zlogClass
{
  public:
  	zlogClass();
	zlogClass( std::string configpath, std::string logfilename);
	~zlogClass();


	void  out_info( std::string log);
	void  out_error( std::string log);
	void out_warn( std::string log);
	void out_notice( std::string log);
	void out_debug( std::string log);
	int init();
	int init( std::string configpath, std::string logfilename);

  private:
	zlog_category_t *c;     
	std::string config;
	std::string logfile;
  
};




