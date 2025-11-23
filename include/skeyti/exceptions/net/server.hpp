#include <skeyti/exceptions/exceptions.hpp>

namespace skeyti::exceptions::net {

class NetworkException : public skeyti::exceptions::Exception {
public:
  using Exception::Exception;
};

class SocketCreateException : public NetworkException {
public:
  SocketCreateException(std::string message) : NetworkException("Socket create failed: " + message) {}
};

class ListenerFailedException : public NetworkException {
public:
  ListenerFailedException(std::string message) : NetworkException("Listener failed to start: " + message) {}
};

class SocketBindFailedException : public NetworkException {
public:
  SocketBindFailedException(std::string message) : NetworkException("Socket bind failed: " + message) {}
};

} // namespace skeyti::exceptions::net
