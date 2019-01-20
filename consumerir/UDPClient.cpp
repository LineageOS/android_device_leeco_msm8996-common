#include <netinet/in.h>
#include "UDPClient.h"

UDPClient::UDPClient(const std::string& addr, int port)
    : f_port(port)
    , f_addr(addr)
{
    char decimal_port[16];
    snprintf(decimal_port, sizeof(decimal_port), "%d", f_port);
    decimal_port[sizeof(decimal_port) / sizeof(decimal_port[0]) - 1] = '\0';
    struct addrinfo hints;
    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_DGRAM;
    hints.ai_protocol = IPPROTO_UDP;
    int r(getaddrinfo(addr.c_str(), decimal_port, &hints, &f_addrinfo));
    if (r != 0 || f_addrinfo == NULL)
    {
        throw UDPClientRuntimeError(("invalid address or port: \"" + addr + ":" + decimal_port + "\"").c_str());
    }
    f_socket = socket(f_addrinfo->ai_family, SOCK_DGRAM | SOCK_CLOEXEC, IPPROTO_UDP);
    if (f_socket == -1)
    {
        freeaddrinfo(f_addrinfo);
        throw UDPClientRuntimeError(("could not create socket for: \"" + addr + ":" + decimal_port + "\"").c_str());
    }
}

UDPClient::~UDPClient()
{
    freeaddrinfo(f_addrinfo);
    close(f_socket);
}

int UDPClient::get_socket() const
{
    return f_socket;
}

int UDPClient::get_port() const
{
    return f_port;
}

std::string UDPClient::get_addr() const
{
    return f_addr;
}

int UDPClient::send(const char *msg, size_t size)
{
    return sendto(f_socket, msg, size, 0, f_addrinfo->ai_addr, f_addrinfo->ai_addrlen);
}
