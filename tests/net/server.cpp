#include "internal/net/server/tcp_server.hpp"
#include <string>
using namespace skeyti::net;

int main() {
  std::string host = "0.0.0.0";
  auto server = skeyti::net::TcpServer(host, 8080);
  server.serve();
  while (1) {
    int i = 1;
  }
}
