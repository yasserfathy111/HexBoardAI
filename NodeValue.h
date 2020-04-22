#ifndef NODEVALUE_H
#define NODEVALUE_H

#include <iostream>

using namespace std;

enum class NodeValue {BLANK, BLUE, RED};

ostream& operator << (ostream& os, NodeValue nv);
NodeValue OtherPlayer(NodeValue In);

#endif