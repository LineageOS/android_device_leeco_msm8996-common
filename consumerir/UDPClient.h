#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <stdexcept>
#include <string.h>
#include <unistd.h>
#include <string>
#include <sstream>
#include <iostream>

class UDPClientRuntimeError : public std::runtime_error
{
public:
  UDPClientRuntimeError(const char *w) : std::runtime_error(w) {}
};

class UDPClient
{
public:
  UDPClient(const std::string& addr, int port);
  ~UDPClient();

  int get_socket() const;
  int get_port() const;
  std::string get_addr() const;

  int send(const char *msg, size_t size);

private:
  int f_socket;
  int f_port;
  std::string f_addr;
  struct addrinfo *f_addrinfo;
};
