#include "HexBoard.h"
#include <iostream>
using namespace std;

int main()
{
	uint32 BoardSize=0;
	cout << "Please enter board size !" << endl;
	cin >> BoardSize;
	
	if(BoardSize<3 || BoardSize>11){BoardSize = 11; cout << "Wrong entry! Assume a size of 11"<< endl;}
	
	HexBoard TestBoard(BoardSize);
	
	TestBoard.Play();
	
}