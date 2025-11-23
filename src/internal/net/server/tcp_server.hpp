#pragma once
#include <cstdint>
#include <string>

namespace skeyti::net {

class TcpServer {
public:
  TcpServer(std::string host, int port);
  ~TcpServer();

  int serve();

private:
  enum class IpKind { IpV4,
                      IpV6,
                      Invalid };

  int set_port();
  IpKind get_ip_type(const std::string &ip);
  int create_socket(const std::string &ip, bool allow_reuse);
  int bind_host();

  std::string _host;
  int _real_port;
  uint16_t _port;
  int _server_fd;
};

} // namespace skeyti::net
