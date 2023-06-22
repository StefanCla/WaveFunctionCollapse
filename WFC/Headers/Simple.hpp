#pragma once
#include <vector>

class Constrain;

enum SideD
{
	Up = 0,
	Left,
	Down,
	Right
};

class Simple
{
public:
	Simple(Constrain& constrain);
	~Simple();

	//Initializes the WFC
	void Initialization();

	//Starts the WFC
	void StartWFC();

	//Restarts the WFC
	void RestartWFC();

	//Returns the cellgrid
	std::vector<std::vector<int>> GetCellGrid();

private:
	//Run the WFC
	void RunWFC();

	//Choose a random cell with the lowest entropy
	int ChooseRandomCell();

	//Get adjectent tiles around the current tile
	int GetAdjacentTile(int currentCell, SideD side);

	//Check the adjectent tile
	bool CheckSide(int currenCell, int adjacentCell, SideD side);

private:
	std::vector<std::vector<int>> m_CellGrid;
	Constrain& m_Constrain;

	int m_RestartCount;
	int m_SolvedTiles;
	bool m_Stable;

};