#include "NodeValue.h"
#include <iostream>

using namespace std;

ostream& operator << (ostream& os, NodeValue nv)
{	
	if(nv == NodeValue::BLUE) os << "B";
	if(nv == NodeValue::RED) os << "R";
	if(nv == NodeValue::BLANK) os << ".";
	return os;
}

NodeValue OtherPlayer(NodeValue In)
{
	NodeValue Other = NodeValue::BLANK;
	if(In == NodeValue::BLUE)
	{
		Other = NodeValue::RED;
	}
	else if(In == NodeValue::RED)
	{
		Other = NodeValue::BLUE;
	}
	else
	{
		cout << "Invalid node type"<< endl;
	}
	return Other;
}