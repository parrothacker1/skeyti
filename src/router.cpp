#include <skeyti/router.hpp>
using namespace skeyti;

Router::Router(std::string base) : _base(base) {}
Router::~Router() {
  // TODO: destroy the trie
}
