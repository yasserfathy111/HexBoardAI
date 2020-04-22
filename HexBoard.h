#ifndef HEX_BOARD_H
#define HEX_BOARD_H

using namespace std;

#include "types.h"
#include <iostream>
#include <vector>
#include <set>
#include "NodeValue.h"

ostream& operator << (ostream& os, vector< pair<uint32, uint32> >  Set);
ostream& operator << (ostream& os, set< pair<uint32, uint32> >  Set);
ostream& operator << (ostream& os, pair<uint32, uint32>  node);
void Thinking();

class HexBoard
{
	private:
	vector<NodeValue> nodes;
	
	public:
	uint32 BoardSize;

	HexBoard(uint32 BoardSize);
	HexBoard(HexBoard& SourceBoard);
	
	friend ostream& operator << (ostream& os, HexBoard hb);
	
	void ScanNodes(NodeValue TargetType, set< pair<uint32, uint32> > &ScannedSet);
	
	void GetSpannedSet(pair<uint32, uint32> & initialNode, set< pair<uint32, uint32> > & SpannedSet);

	NodeValue getNode(uint32 Y, uint32 X);
	void setNode(uint32 Y, uint32 X, NodeValue nodeValue);

	vector< pair<uint32, uint32> > neighbours (pair<uint32, uint32> node);

	bool didBlueWin();
	bool didRedWin();
	void Play();

	f32 DoOneMonteCarloSimulation(NodeValue AIPlayer, pair<uint32, uint32> MoveToBeEvaluated, uint32 Simulation_iter_count);
	
	pair<uint32, uint32> GetAIMove(NodeValue AIPlayer);
	
	bool isBoardFull();
		
	bool isGameOver();
};


#endif