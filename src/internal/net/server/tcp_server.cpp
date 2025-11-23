#include "tcp_server.hpp"

#include <arpa/inet.h>
#include <cerrno>
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>

#include <skeyti/exceptions/net/server.hpp>

namespace skeyti::net {

TcpServer::TcpServer(std::string host, int port)
    : _host(std::move(host)), _real_port(port), _server_fd(-1) {
  set_port();
  create_socket(_host, true);
  bind_host();
}

TcpServer::~TcpServer() {
  if (_server_fd >= 0)
    close(_server_fd);
}

int TcpServer::serve() {
  if (listen(_server_fd, 10) < 0) {
    std::string message;
    switch (errno) {
    case EADDRINUSE:
      message = "Access denied";
      break;
    case EOPNOTSUPP:
      message = "Wrong socket provided";
      break;
    default:
      throw exceptions::UnknownException(errno);
    }
    throw exceptions::net::ListenerFailedException(message);
  }
  return 0;
}

int TcpServer::set_port() {
  if (_real_port < 0 || _real_port > 65535) {
    throw exceptions::net::NetworkException(
        "Port number " + std::to_string(_real_port) + " exceeds the range 0-65535");
  }
  _port = htons(_real_port);
  return 0;
}

TcpServer::IpKind TcpServer::get_ip_type(const std::string &ip) {
  in_addr ipv4;
  in6_addr ipv6;
  if (inet_pton(AF_INET, ip.c_str(), &ipv4) == 1)
    return IpKind::IpV4;
  if (inet_pton(AF_INET6, ip.c_str(), &ipv6) == 1)
    return IpKind::IpV6;
  return IpKind::Invalid;
}

int TcpServer::create_socket(const std::string &ip, bool allow_reuse) {
  switch (get_ip_type(ip)) {
  case IpKind::IpV4:
    _server_fd = socket(AF_INET, SOCK_STREAM, 0);
    break;
  case IpKind::IpV6:
    _server_fd = socket(AF_INET6, SOCK_STREAM, 0);
    break;
  case IpKind::Invalid:
  default:
    throw exceptions::net::NetworkException("Invalid IP address: " + ip);
  }

  if (_server_fd == -1) {
    std::string message;
    switch (errno) {
    case EACCES:
      message = "Access denied";
      break;
    case EMFILE:
      message = "Per-process file descriptor limit reached";
      break;
    case ENFILE:
      message = "System-wide file descriptor limit reached";
      break;
    default:
      throw exceptions::UnknownException(errno);
    }
    throw exceptions::net::SocketCreateException(message);
  }

  if (allow_reuse) {
    int yes = 1;
    if (setsockopt(_server_fd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(yes)) < 0) {
      std::string message;
      switch (errno) {
      case EINVAL:
        message = "Invalid socket length provided";
        break;
      default:
        throw exceptions::UnknownException(errno);
      }
      throw exceptions::net::SocketCreateException("setsockopt failed: " + message);
    }
  }

  return 0;
}

int TcpServer::bind_host() {
  switch (get_ip_type(_host)) {
  case IpKind::IpV4: {
    sockaddr_in addr{};
    addr.sin_family = AF_INET;
    addr.sin_port = _port;
    inet_pton(AF_INET, _host.c_str(), &addr.sin_addr);

    if (bind(_server_fd, (sockaddr *)&addr, sizeof(addr)) < 0) {
      std::string message;
      switch (errno) {
      case EACCES:
        message = "Access denied";
        break;
      case EADDRINUSE:
        message = "Address " + _host + ":" + std::to_string(_real_port) + " already in use";
        break;
      case EADDRNOTAVAIL:
        message = "Address " + _host + ":" + std::to_string(_real_port) + " doesn't exist";
        break;
      default:
        throw exceptions::UnknownException(errno);
      }
      throw exceptions::net::SocketBindFailedException(message);
    }
    return 0;
  }

  case IpKind::IpV6: {
    sockaddr_in6 addr{};
    addr.sin6_family = AF_INET6;
    addr.sin6_port = _port;
    inet_pton(AF_INET6, _host.c_str(), &addr.sin6_addr);

    if (bind(_server_fd, (sockaddr *)&addr, sizeof(addr)) < 0) {
      std::string message;
      switch (errno) {
      case EACCES:
        message = "Access denied";
        break;
      case EADDRINUSE:
        message = "Address " + _host + ":" + std::to_string(_real_port) + " already in use";
        break;
      case EADDRNOTAVAIL:
        message = "Address " + _host + ":" + std::to_string(_real_port) + " doesn't exist";
        break;
      default:
        throw exceptions::UnknownException(errno);
      }
      throw exceptions::net::SocketBindFailedException(message);
    }
    return 0;
  }

  case IpKind::Invalid:
  default:
    throw exceptions::net::NetworkException("Invalid IP address: " + _host);
  }
}

} // namespace skeyti::net
