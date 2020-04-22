#include "HexBoard.h"
#include <utility>
#include <vector>
#include <set>
#include <iomanip>
#include <random>
using namespace std;

#define SIMULATION_COUNT 1000
/******************************************************************************/
//HexBoard: Constructor
/******************************************************************************/

HexBoard::HexBoard(uint32 BoardSize):
BoardSize(BoardSize),
nodes(BoardSize*BoardSize)

{
}

HexBoard::HexBoard(HexBoard& SourceBoard):
BoardSize(SourceBoard.BoardSize),
nodes(SourceBoard.nodes)
{
}

ostream& operator << (ostream& os, HexBoard hb)
{
	//print indeces
	cout << "     "; for(uint32 index = 0; index<hb.BoardSize; index++) cout << setw(2) << index << "  "; cout << endl;

	for(uint32 i=0 ; i < hb.BoardSize ; i++)
	{
		for(uint32 indent =0; indent<=i ; indent++) os << "  "; cout << setw(2) << i;
		
		for(uint32 j= (i*hb.BoardSize) ; j < ((i+1)*hb.BoardSize) ; j++)
		{
			if(j == (i*hb.BoardSize)) cout << "  ";
			os << hb.nodes[j];
			if((j!= ((i+1)*hb.BoardSize)-1)) os << " - ";
		}
		
		os << endl << "     ";
		if(i<hb.BoardSize-1) for(uint32 indent =0; indent<=i ; indent++) os << "  ";
		if(i<hb.BoardSize-1) for(uint32 pattern =0; pattern<hb.BoardSize-1 ; pattern++) os << '\\' << " " << "/" << " ";
		if(i<hb.BoardSize-1) os << '\\';
		if(i<hb.BoardSize-1) os << endl ;	
	}
	cout << endl;
	return os;
}

/******************************************************************************/

/******************************************************************************/

NodeValue HexBoard::getNode(uint32 Y, uint32 X)
{
	uint32 position = (Y*BoardSize) + X;
	return nodes[position];
}

/******************************************************************************/

/******************************************************************************/

void HexBoard::setNode(uint32 Y, uint32 X, NodeValue nodeValue)
{
	uint32 position = (Y*BoardSize) + X;
	if(nodes[position] == NodeValue::BLANK)
	{
		nodes[position] = nodeValue;
	}
	else
	{
		cout << "Invalid entry" << endl;
	}
}

/******************************************************************************/

/******************************************************************************/

vector< pair<uint32, uint32> > HexBoard::neighbours (pair<uint32, uint32> node)
{
	vector< pair<uint32, uint32> > neighboursSet;
	pair <uint32, uint32> targetNode;
	
	//Northwest node
	//if node is not in top row
	if(node.first > 0)
	{
		targetNode.first = node.first-1;
		targetNode.second = node.second;
		if(getNode(node.first, node.second) == getNode(targetNode.first, targetNode.second)) neighboursSet.push_back(targetNode);
	}

	//Northeast node
	//if node is not in top row or rightmost column
	if((node.first > 0) && (node.second < BoardSize-1))
	
	{
		targetNode.first = node.first-1;
		targetNode.second = node.second+1;
		if(getNode(node.first, node.second) == getNode(targetNode.first, targetNode.second)) neighboursSet.push_back(targetNode);
	}

	//East node
	//if node is not in top row or rightmost column
	if(node.second < BoardSize-1) 
	{
		targetNode.first = node.first;
		targetNode.second = node.second+1;
		if(getNode(node.first, node.second) == getNode(targetNode.first, targetNode.second)) neighboursSet.push_back(targetNode);
	}

	//Southeast node
	//if node is not in bottom row 
	if(node.first < BoardSize-1)
	{
		targetNode.first = node.first+1;
		targetNode.second = node.second;
		if(getNode(node.first, node.second) == getNode(targetNode.first, targetNode.second)) neighboursSet.push_back(targetNode);
	}

	//Southwest node
	//if node is not in bottom row or leftmost column
	if((node.first < BoardSize-1) && (node.second > 0))	
	{
		targetNode.first = node.first+1;
		targetNode.second = node.second-1;
		if(getNode(node.first, node.second) == getNode(targetNode.first, targetNode.second)) neighboursSet.push_back(targetNode);
	}

	//West node
	//if node is not leftmost column
	if(node.second > 0)
	{
		targetNode.first = node.first;
		targetNode.second = node.second-1;
		if(getNode(node.first, node.second) == getNode(targetNode.first, targetNode.second)) neighboursSet.push_back(targetNode);
	}
	return neighboursSet;
}
/******************************************************************************/

/******************************************************************************/

void HexBoard::ScanNodes(NodeValue TargetType, set< pair<uint32, uint32> > &ScannedSet)
{
	pair <uint32, uint32> node;
	
	//Scan all nodes with target type chips.
	for(uint32 Xiter = 0;Xiter< BoardSize; Xiter++)
	{
		for(uint32 Yiter = 0;Yiter< BoardSize; Yiter++)
		{
			node.first = Yiter;
			node.second = Xiter;	
			if((getNode(node.first, node.second) == TargetType))
			{
				ScannedSet.insert(node);
			}
		}
	}		
}

/******************************************************************************/

/******************************************************************************/

void HexBoard::GetSpannedSet(pair<uint32, uint32> & initialNode, set< pair<uint32, uint32> > & SpannedSet)
{
	//Set of visited nodes where neighbours are scanned.
	set< pair<uint32, uint32> > VisitedSet;
	//Set of neighbours in search region = Spanned - Visited.
	set< pair<uint32, uint32> > SearchSet;

	SpannedSet.insert(initialNode);
	VisitedSet.insert(initialNode);
	SearchSet.insert(initialNode);

	if(VERBOSE)cout << "---------------" << endl << "Initial node: " << initialNode;

	///Loop until all nodes in search domain are visited.
	while(!SearchSet.empty())
	{
		//Get current node from the search domain.
		auto currentNodeIter = SearchSet.begin();
		if(VERBOSE)cout << "Current node: " << *currentNodeIter;
		//Get neighbours of current node.
		vector< pair<uint32, uint32> > nodeNeighbours = neighbours(*currentNodeIter);
		//Add neighbours to spanned domain.
		SpannedSet.insert(nodeNeighbours.begin(), nodeNeighbours.end());
		//Add current node to visited set.
		VisitedSet.insert(*currentNodeIter);
		//Search domain = Spanned domain - Visited domain.
		SearchSet = SpannedSet;
		for(auto node:VisitedSet) SearchSet.erase(node);

		if(VERBOSE)cout << "Spanned set: " << SpannedSet;
		if(VERBOSE)cout << "Visited set: " << VisitedSet;
		if(VERBOSE)cout << "Search set: " << SearchSet;
	}	
}

/******************************************************************************/

/******************************************************************************/

bool HexBoard::didBlueWin()
{
	bool BlueWon = false;
	set< pair<uint32, uint32> > BlueSet;	
	
	//Scan the board for blue chips
	ScanNodes(NodeValue::BLUE, BlueSet);
	
	if(VERBOSE)cout << "Blue Set: " << BlueSet;

	//Loop through all nodes
	for(auto initialNode:BlueSet)
	{
		//Initial nodes has to be in leftmost column, start from a leftmost node and 
		//check if the spanned range include a rightmost node.
		if((initialNode.second == 0) && (BlueWon == false))
		{
			//Spanned set: Set of detected neighbours.	
			set< pair<uint32, uint32> > SpannedSet;
			
			GetSpannedSet(initialNode, SpannedSet);
			
			for(auto node:SpannedSet) if(node.second == BoardSize-1) 
			{
				BlueWon = true;
				break;
			}
		}
	}
	if(VERBOSE) cout << "Did Blue win ? " << BlueWon << endl;
	return BlueWon;
}

/******************************************************************************/

/******************************************************************************/

bool HexBoard::didRedWin()
{
	bool RedWon = false;
	set< pair<uint32, uint32> > RedSet;	
	
	//Scan the board for blue chips
	ScanNodes(NodeValue::RED, RedSet);
	
	if(VERBOSE)cout << "Red Set: " << RedSet;

	//Loop through all nodes
	for(auto initialNode:RedSet)
	{
		//Initial nodes has to be in the top row, start from a top node and 
		//check if the spanned range include a buttom node.
		if((initialNode.first == 0) && (RedWon == false))
		{
			//Spanned set: Set of detected neighbours.	
			set< pair<uint32, uint32> > SpannedSet;
			
			GetSpannedSet(initialNode, SpannedSet);
			
			for(auto node:SpannedSet) if(node.first == BoardSize-1) 
			{
				RedWon = true;
				break;
			}
		}
	}
	if(VERBOSE) cout << "Did Red win ? " << RedWon << endl;
	return RedWon;
}

/******************************************************************************/

/******************************************************************************/
f32 HexBoard::DoOneMonteCarloSimulation(NodeValue AIPlayer, pair<uint32, uint32> MoveToBeEvaluated, uint32 Simulation_iter_count)
{
	static default_random_engine generator(time(0));
	uniform_int_distribution<uint32> distribution(0,BoardSize-1);
	
	uint32 BlueWonCount = 0;
	uint32 RedWonCount = 0;
	float SuccessRate = 0.0;
	NodeValue CurrentPlayer = AIPlayer;
	
	pair<uint32, uint32> RandomMove;

	for(uint32 SimulationIter = 0; SimulationIter<Simulation_iter_count; SimulationIter++)
	{
		//Reset current player to AI player
		CurrentPlayer = AIPlayer;
		//Take a copy of the current board state to start MonteCarlo simulation off
		HexBoard MonteCarloBoard(*this);
		//Set the position to be tested for the AI player.
		MonteCarloBoard.setNode(MoveToBeEvaluated.first, MoveToBeEvaluated.second, CurrentPlayer);
		//cout << "MonteCarloBoard:"<< endl << MonteCarloBoard;

		//Fill the board randomly from this position
		while(!MonteCarloBoard.isBoardFull())
		{
			//Generate a random position and make sure it's empty.
			do
			{
				RandomMove.first = distribution(generator);
				RandomMove.second = distribution(generator);
			}
			while(MonteCarloBoard.getNode(RandomMove.first, RandomMove.second) != NodeValue::BLANK);

			//Switch players
			CurrentPlayer = OtherPlayer(CurrentPlayer);
			//Set the random position for the current player
			MonteCarloBoard.setNode(RandomMove.first, RandomMove.second, CurrentPlayer);
		}

		//cout << "Full MonteCarloBoard:"<< endl << MonteCarloBoard;
		//Now One run of the Simulation is complete, get the winner.
		if(MonteCarloBoard.didBlueWin())
		{
			BlueWonCount ++; 
		}
		else if(MonteCarloBoard.didRedWin())
		{
			RedWonCount ++; 
		}		
	}	
	//cout << "BW" << BlueWonCount << endl;
	//cout << "RW" << RedWonCount << endl;

	if(AIPlayer == NodeValue::RED)
	{
		SuccessRate = (f32)RedWonCount * 100.0 / (f32)Simulation_iter_count;
	}
	else if(AIPlayer == NodeValue::BLUE)
	{
		SuccessRate = (f32)BlueWonCount * 100.0 / (f32)Simulation_iter_count;		
	}

	//cout << "SR: "<<SuccessRate<<endl;
	Thinking();
		
	return SuccessRate;
}

/******************************************************************************/

/******************************************************************************/

pair<uint32, uint32> HexBoard::GetAIMove(NodeValue AIPlayer)
{
	pair<uint32, uint32> Move;
	pair<uint32, uint32> RecommendedMove;
	f32 BestSuccessRate = 0.0;
	f32 IterSuccessRate = 0.0;

	cout << "Give me a minute, Will you !!"<<endl;
	
	//Loop over all empty nodes
	for(uint32 row=0;row<BoardSize;row++)
	{
		for(uint32 column=0;column<BoardSize;column++)
		{
			if(getNode(row, column) == NodeValue::BLANK)
			{	
				Move.first = row; Move.second = column;	
				IterSuccessRate = DoOneMonteCarloSimulation(AIPlayer, Move, SIMULATION_COUNT);
				if(BestSuccessRate < IterSuccessRate)
				{
					BestSuccessRate = IterSuccessRate;
					RecommendedMove = Move;
				}
				//cout << "Current move is " << Move << "With a success rate of: " << IterSuccessRate << "%"<<endl;
			}
		}
	}
	
	//cout << "Recommended move is " << RecommendedMove << "With a success rate of: " << BestSuccessRate << "%"<<endl;
	
	return RecommendedMove;
}

/******************************************************************************/

/******************************************************************************/

void HexBoard::Play()
{
	bool BlueWon = false;
	bool RedWon = false;
	bool IsBlueAI = false;
	bool IsRedAI = false;
	char YesNo;
	pair<uint32, uint32> Move;
	
	cout << "Blue player; Are you human ? (Y/N)" << endl;
	cin >> YesNo;
	if(tolower(YesNo) == 'n') {IsBlueAI = true; cout << "I don't speak binary!" << endl << endl;}
	else if(tolower(YesNo) == 'y'){cout << "Hello Blue Human, How is it going?" << endl << endl;}
	else cout << "Wrong entry, You must be human!" << endl;	

	cout << "Red player; Are you human ? (Y/N)" << endl;
	cin >> YesNo;
	if(tolower(YesNo) == 'n') {IsRedAI = true; cout << "I don't speak binary!" << endl << endl;}
	else if(tolower(YesNo) == 'y'){cout << "Hello Red Human, What's up?" << endl << endl;}
	else cout << "Wrong entry, You must be human!" << endl;
	
	cout << "Remember! Red Should connect Top to Buttom, Blue, Left to Right." << endl;
	cout << *this;

	while(true)
	{
		if(IsBlueAI)
		{
			cout << "Blue AI is trying to think !" << endl;
			Move = GetAIMove(NodeValue::BLUE);			
		}
		else
		{
			cout << "Blue player: Enter move coordinates (Row, Column); separated by Enter !" << endl;
			cin >> Move.first;
			cin >> Move.second;
		}

		setNode(Move.first, Move.second, NodeValue::BLUE);
		cout << *this;
		if(didBlueWin()) {BlueWon = true; break;}

		
		if(IsRedAI)
		{
			cout << "Red AI is trying to think !" << endl;
			Move = GetAIMove(NodeValue::RED);
		}
		else
		{
			cout << "Red player: Enter move coordinates (Row, Column); separated by Enter !" << endl;
			cin >> Move.first;
			cin >> Move.second;
		}

		setNode(Move.first, Move.second, NodeValue::RED);
		cout << *this;
		if(didRedWin()) {RedWon = true; break;}
	
	}	
	if(BlueWon) cout << "Blue Won!" << endl;
	else if(RedWon) cout << "Red Won!" << endl;
	
}

/******************************************************************************/

/******************************************************************************/
bool HexBoard::isBoardFull()
{
	bool IsFull = true;
	for(auto Node : this->nodes)
	{
		if(Node == NodeValue::BLANK)IsFull = false;
	}
	return IsFull;
}
/******************************************************************************/

/******************************************************************************/


bool HexBoard::isGameOver()
{
	bool BlueWon = didBlueWin();
}

/******************************************************************************/

/******************************************************************************/

ostream& operator << (ostream& os, vector< pair<uint32, uint32> >  Set)
{
		
	for(uint32 i=0 ; i < Set.size() ; i++)
	{
		cout << "(" << Set[i].first << ", " << Set[i].second << "), ";
	}	
	cout << endl;
	return os;
}

ostream& operator << (ostream& os, set< pair<uint32, uint32> >  Set)
{
		
	for(auto iter : Set)
	{
		cout << "(" << iter.first << ", " << iter.second << "), ";
	}	
	cout << endl;
	return os;
}

ostream& operator << (ostream& os, pair<uint32, uint32>  node)
{
		
	cout << "Node: (" << node.first << ", " << node.second << "), ";
	cout << endl;
	return os;
}

void Thinking()
{
	static uint32 state_machine=0;
	
	switch(state_machine)
	{
		case 0:
		{
			cout << ".. :) .." << '\r' << flush;	
			state_machine=1;
			break;
		}

		case 1:
		{
			cout << ".. :| .." << '\r' << flush;	
			state_machine=2;
			break;
		}

		case 2:
		{
			cout << ".. :( .." << '\r' << flush;	
			state_machine=3;
			break;
		}
		case 3:
		{
			cout << ".. :| .." << '\r' << flush;	
			state_machine=0;
			break;
		}
			
	}	
}
