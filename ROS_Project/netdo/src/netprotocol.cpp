#include <cstddef>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <cstring>
#include <cstdio>
#include <cstdlib>
#include <netdb.h>
#include <assert.h>

#include <netprotocol.hpp>

struct ProtocolHeader {
    uint32_t check;
    uint32_t packet_length;
};

int NetProtocol::connectServer() {
    int port = 13245;
    struct sockaddr_in cliAddr;
    struct sockaddr_in servAddr;
    cliAddr.sin_family = AF_INET;
    cliAddr.sin_addr.s_addr = htons(0);

    this->sock = socket(AF_INET, SOCK_STREAM, 0);

    bind(this->sock, (struct sockaddr*)&cliAddr, sizeof(struct sockaddr_in));

    servAddr.sin_family = AF_INET;
    struct in_addr sin_addr;
    inet_aton("10.136.117.172", &sin_addr);
    memcpy(&servAddr.sin_addr, &sin_addr, sizeof(struct in_addr));

    servAddr.sin_port = htons(port);
    socklen_t serLen = sizeof(servAddr);

    connect(this->sock, (struct sockaddr*)&servAddr, serLen);
}

static int receive_n_bytes(int sock, void *bytes, int n) {
    int ret;
    char c;
    char *ppp = (char*)bytes;
    do {
        ret = recv(sock, &c, sizeof(c), 0);
        if (ret < 0) {
            return ret;
        }
        *ppp++ = c;
    } while (ret == 1 && --n != 0);
}

int NetProtocol::retrieveData_noblock(void **pdata, int &length) {
    struct ProtocolHeader header;
    receive_n_bytes(sock, &header, sizeof(header));
    printf("receive header: %x, %d\n", header.check, header.packet_length);

    assert(pdata != NULL);
    if (header.check != 0xdeadbeef) {
        return -1;
    }
    length = header.packet_length;
    *pdata = malloc(header.packet_length);
    receive_n_bytes(sock, *pdata, header.packet_length);
}

int NetProtocol::close() {
    ::close(this->sock);
}