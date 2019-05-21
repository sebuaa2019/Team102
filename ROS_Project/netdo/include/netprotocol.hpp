#ifndef __NET_PROTOCOL_HPP__
#define __NET_PROTOCOL_HPP__



struct NetProtocol {
    int sock;
    
    int connectServer();
    int retrieveData_noblock(void **pdata, int &length);
    int close();
};

#endif