#pragma once
#include <functional>
#include <string>

namespace skeyti {

class Context;
class Request;
class Response;

enum class Method { GET,
                    POST,
                    PUT,
                    PATCH,
                    DELETE };

using HandlerFunc = std::function<Response(Context&, Request&)>;

class Router {
 public:
  explicit Router(std::string base);
  ~Router();

  void add_handler(Method m, std::string endpoint, HandlerFunc h);
  void add_router(std::string endpoint, Router* child);

 private:
  std::string _base;
  friend class Server;
  struct RouteMatch {
    HandlerFunc* handler;
    Router* router;
  };
  RouteMatch match(std::string_view path);
};

}  // namespace skeyti
