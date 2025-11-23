#include <string>

typedef struct
{
  std::string _base;
  bool _has_dynamic;
  Node _children_nodes[];
} Node;

class RadixTrie
{
public:
  void add_node ();
};
