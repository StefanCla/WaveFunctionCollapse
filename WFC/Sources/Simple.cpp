#include "Precomp.hpp"
#include "Simple.hpp"
#include "Constrain.hpp"
#include <algorithm>
#include <assert.h>

Simple::Simple(Constrain& constrain) :
	m_Constrain(constrain),
	m_RestartCount(0),
	m_SolvedTiles(0)
{

#ifdef _DEBUG
	srand(0); //Seed with 0 for debugging
#else
	srand(time(NULL));
#endif
}

Simple::~Simple()
{}

void Simple::Initialization()
{
	for (size_t i = 0; i < GRIDSIZE; i++)
	{
		std::vector<int> tempGrid;
		for (size_t j = 0; j < TILEAMOUNT; j++)
		{
			tempGrid.push_back(j);
		}

		m_CellGrid.push_back(tempGrid);
	}
}

void Simple::StartWFC()
{
	ChooseRandomCell();
}

void Simple::RestartWFC()
{
	m_RestartCount++;

	m_CellGrid.clear();
	Initialization();

	m_SolvedTiles = 0;

	StartWFC();
}

void Simple::ChooseRandomCell()
{
	//Get cell with lowest entropy
	std::vector<int> lowestEntropyCells;
	int lowestEntropy = INT_MAX;
	for (size_t i = 0; i < GRIDSIZE; i++) //should be max grid size
	{
		if (m_CellGrid[i].size() >= 2) //Cells with 1 possible tile left are collapsed
		{
			if (m_CellGrid[i].size() == lowestEntropy)
			{
				lowestEntropyCells.push_back(i);

			}
			else if (m_CellGrid[i].size() < lowestEntropy)
			{
				lowestEntropyCells.clear();
				lowestEntropyCells.push_back(i);
				lowestEntropy = m_CellGrid[i].size();
			}
		}
	}

	//Get random cell & tile
	int cellNumber = rand() % lowestEntropyCells.size();
	int chosenCell = lowestEntropyCells[cellNumber];
	int tileNumber = rand() % m_CellGrid[chosenCell].size();
	int tile = m_CellGrid[chosenCell][tileNumber];

	m_CellGrid[chosenCell].clear();
	m_CellGrid[chosenCell].push_back(tile);

	m_SolvedTiles++;

	//Check cell
	CheckCell(chosenCell);
}

//Check all adjacent sides of the cell
void Simple::CheckCell(int cell)
{
	int topCell = cell - GRIDSIZEX;
	int bottomCell = cell + GRIDSIZEX;
	int leftCell = -1;
	int rightCell = -1;

	if (cell % GRIDSIZEX != 0)
		leftCell = cell - 1;

	if ((cell + 1) % GRIDSIZEX != 0)
		rightCell = cell + 1;

	//Check sides
	if (topCell >= 0)
		CheckSides(SideD::Up, cell, topCell);
	if (bottomCell < GRIDSIZE)
		CheckSides(SideD::Down, cell, bottomCell);
	if (leftCell != -1)
		CheckSides(SideD::Left, cell, leftCell);
	if (rightCell != -1)
		CheckSides(SideD::Right, cell, rightCell);

	//Check if grid has fully collapsed
	if (m_SolvedTiles != GRIDSIZE)
		//if (m_GuessedCell == cell)
		ChooseRandomCell();
}

//Reduce entropy of the adjacent cells
void Simple::CheckSides(SideD side, int currentCell, int newCell)
{
	int startIndex = -1;
	std::vector<int> newConstrains;

	switch (side)
	{
	case SideD::Up: startIndex = TILEAMOUNT * 0;
		break;
	case SideD::Left: startIndex = TILEAMOUNT * 3;
		break;
	case SideD::Down: startIndex = TILEAMOUNT * 2;
		break;
	case SideD::Right: startIndex = TILEAMOUNT * 1;
		break;
	}

	std::vector<int> possibleConstrains;

	//Check for possible constrains
	for (size_t i = 0; i < m_Constrain.m_Constrains[m_CellGrid[currentCell][0]].size(); i++)// 24
	{
		if (m_Constrain.m_Constrains[m_CellGrid[currentCell][0]][i] >= (startIndex + TILEAMOUNT))
			break;

		if (m_Constrain.m_Constrains[m_CellGrid[currentCell][0]][i] >= startIndex)
			possibleConstrains.push_back(m_Constrain.m_Constrains[m_CellGrid[currentCell][0]][i]);
	}

	//Reduce current constrains
	for (size_t i = 0; i < possibleConstrains.size(); i++)
	{
		int tile = possibleConstrains[i] - startIndex;

		std::vector<int>::iterator it = std::find(m_CellGrid[newCell].begin(), m_CellGrid[newCell].end(), tile);

		if (it != m_CellGrid[newCell].end())
			newConstrains.push_back(tile);
	}

	if (newConstrains.size() == 0)
	{
		RestartWFC();
	}

	if (newConstrains.empty())
		assert(true && "ERROR: Impossible position remaining");

	//If more than one constraint was left, overwrite the old constrains
	if (m_CellGrid[newCell].size() != 1)
	{
		m_CellGrid[newCell] = newConstrains;
		if (newConstrains.size() == 1)
		{
			m_SolvedTiles++;
			CheckCell(newCell);
		}
	}

	//CheckCell(newCell);
}

std::vector<std::vector<int>> Simple::GetCellGrid()
{
	return m_CellGrid;
}