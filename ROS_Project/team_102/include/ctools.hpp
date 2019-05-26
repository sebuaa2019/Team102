
#ifndef __CR_IPC_TOOLS_HPP__
#define __CR_IPC_TOOLS_HPP__

#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <cstring>
#include <cstdio>
#include <cstdlib>
#include <netdb.h>

struct ProtocolHeader {
    uint32_t check;
    uint32_t packet_length;
};

static inline int retrieveData(int fd, void *&pdata, int &length) {

    auto receive_n_bytes = [](int sock, void *bytes, int n) {
            int ret;
            char c;
            char *ppp = (char*)bytes;
            do {
                ret = read(sock, &c, sizeof(c));
                if (ret <= 0) {
                    return -1;
                }
                *ppp++ = c;
            } while (ret == 1 && --n != 0);
            return ret;
    };
    struct ProtocolHeader header;
    if (receive_n_bytes(fd, &header, sizeof(header)) < 0) {
        return -1;
    }

    if (header.check != 0xdeadbeef) {
        return -1;
    }
    length = header.packet_length;
    pdata = malloc(header.packet_length);
    if (receive_n_bytes(fd, pdata, header.packet_length) < 0) {
        return -1;
    }
    return 0;
}

static inline int sendData(int fd, void *pdata, int length) {
    struct ProtocolHeader header;
    header.check = 0xdeadbeef;
    header.packet_length = length;
    int ret = write(fd, static_cast<void*>(&header), sizeof(header));
    if (ret <= 0) {
        return -1;
    }
    ret = write(fd, pdata, length);
    if (ret <= 0) {
        return -1;
    }
    return 0;
}

static inline uint64_t CRHighPrecisionTime() {
    struct timespec ts;
    clock_gettime(CLOCK_MONOTONIC, &ts);
    return (uint64_t)ts.tv_nsec + (uint64_t)ts.tv_sec * 1000000000UL;
}

static int connectServer(int &sock, const char *str, int port) {
    //int port = SERVER_PORT;
    int ret;
    struct sockaddr_in cliAddr;
    struct sockaddr_in servAddr;
    cliAddr.sin_family = AF_INET;
    cliAddr.sin_addr.s_addr = htons(0);

    sock = socket(AF_INET, SOCK_STREAM, 0);

    ret = bind(sock, (struct sockaddr*)&cliAddr, sizeof(struct sockaddr_in));
    if (ret != 0) {
        close(sock);
        return -1;
    }
    servAddr.sin_family = AF_INET;
    struct in_addr sin_addr;
    inet_aton(str, &sin_addr);
    memcpy(&servAddr.sin_addr, &sin_addr, sizeof(struct in_addr));

    servAddr.sin_port = htons(port);
    socklen_t serLen = sizeof(servAddr);

    ret = connect(sock, (struct sockaddr*)&servAddr, serLen);
    if (ret != 0) {
        close(sock);
        return -1;
    }

    return 0;
}

#endif