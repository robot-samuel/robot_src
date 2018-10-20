#pragma once


#include <numeric>
#include <string>
#include <iostream>
#include <boost/array.hpp>
#include <boost/circular_buffer.hpp>
#include <boost/smart_ptr.hpp>
#include <boost/thread.hpp>



class socketcan
{
private:


public:
    
    enum PARAM
        {
        BUF_MAX_SIZE =100,
        };
    
    socketcan(std::string id);
    socketcan(std::string id,int filterid);
    socketcan(std::string id,int *filterid,int filtCnt);
    
    ~socketcan();
    
    int send(const std::string &cmd);
    int send(boost::array <char ,13> &cmd);
    void th_sync_block_read();
    void th_sync_noblock_read();
    int is_recvbuf_empty();
    int is_recvbuf_full();
    int recvbuf_size();
    void print_fram(struct  can_frame frame);
    void pop_cur_toarray();
    void print_fram_back();
    void print_fram_all();
    void print_hex();

    //兼容之前的程序
    void sendRequest(const std::string &cmd);
    int isresvdate();
    void clear();
    int m_recv_datecounter;
    int m_send_datecounter;
    boost::array<char,13>cur_data;
public:
    int canFD; /* can raw socket */ 
    std::string canport; /*can0   can1 */ 
    int filter_id;
    boost::shared_ptr<boost::thread>canrecv_ptr;
    
    boost::shared_ptr<boost::circular_buffer<struct  can_frame> > buf_ptr;

    
};

