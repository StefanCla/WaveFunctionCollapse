#include "Precomp.hpp"
#include "WFC.hpp"
#include <assert.h>
#include <bitset>
#include <chrono>

WFC::WFC(std::vector<int> constraints) :
	m_GuessedCell(-1),
	m_GuessedTile(-1),
	m_SolvedTiles(0)
{
	m_Constraints = constraints;

#ifdef _DEBUG
	srand(0); //Seed with 0 for debugging
#else
	srand(time(NULL));
#endif
}

WFC::~WFC()
{}

void WFC::Initialization()
{
	char possibleTiles = 1 << 0 | 1 << 1 | 1 << 2 | 1 << 3 | 1 << 4 | 1 << 5;

	for (int i = 0; i < GRIDSIZE; i++)
	{
		m_CellGrid.push_back(std::pair<char, int>( possibleTiles, -1));
	}
}

//Start with a random cell & tile
void WFC::StartWFC()
{
	int cell = rand() % GRIDSIZE;
	int tile = rand() % TILEAMOUNT;

	m_CellGrid[cell].first = 0;
	m_CellGrid[cell].first = 1 << tile;
	m_CellGrid[cell].second = tile;

	m_GuessedCell = cell;
	m_GuessedTile = tile;
	m_SolvedTiles++;

	CheckCell(cell);
}

//Reset WFC and start again
void WFC::RestartWFC()
{
	m_CellGrid.clear();
	m_SolvedTiles = 0;

	//Set data
	Initialization();

	//Begin WFC
	StartWFC();
}

//Get a random cell and start the WFC
void WFC::ChooseRandomCell()
{
	//Get Lowest Entropy
	std::vector<int> lowestEntropyCells;
	size_t flagCount = -1;
	for (int i = 0; i < GRIDSIZE; i++)
	{
		std::bitset<32> bits(m_CellGrid[i].first);
		std::size_t num = bits.count();

		if (num >= 2) //Must have at least 2 possibilities
		{
			if (flagCount == -1)
			{
				flagCount = num;
				lowestEntropyCells.push_back(i);
			}
			else if (flagCount == num)
			{
				lowestEntropyCells.push_back(i);
			}
			else if (num < flagCount)
			{
				flagCount = num;
				lowestEntropyCells.clear();
				lowestEntropyCells.push_back(i);
			}

			if (num == 0)
				assert(true && "ERROR: no possibilities left!");
		}
	}

	//Choose random cell with lowest entropy
	int cell = rand() % lowestEntropyCells.size();

	//Choose random tile in the lowest entropy cell
	int tileCount = rand() % flagCount + 1;
	int tile = -1;
	for (int i = 0; i < TILEAMOUNT; i++)
	{
		if (m_CellGrid[lowestEntropyCells[cell]].first & (1 << i))
		{
			tileCount--;
			if (tileCount <= 0)
			{
				tile = i;
				break;
			}
		}
	}

	//Set the tile and cell in the cellgrid
	m_CellGrid[lowestEntropyCells[cell]].first = 0;
	m_CellGrid[lowestEntropyCells[cell]].first = 1 << tile;
	m_CellGrid[lowestEntropyCells[cell]].second = tile;

	m_GuessedCell = lowestEntropyCells[cell];
	m_GuessedTile = tile;
	m_SolvedTiles++;

	CheckCell(lowestEntropyCells[cell]);
}

//Check all the sides of a cell
void WFC::CheckCell(int cell)
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
	if(topCell >= 0)
		CheckSides(Sides::TOP, cell, topCell);
	if(bottomCell < m_CellGrid.size())
		CheckSides(Sides::BOTTOM, cell, bottomCell);
	if (leftCell != -1)
		CheckSides(Sides::LEFT, cell, leftCell);
	if (rightCell != -1)
		CheckSides(Sides::RIGHT, cell, rightCell);

	//Check if grid has fully collapsed
	if (m_SolvedTiles != GRIDSIZE)
		if(m_GuessedCell == cell)
			ChooseRandomCell();
}

//Check if we can reduce the super position of the new cell
void WFC::CheckSides(Sides side, int currentCell, int newCell)
{
	//Avoid checking the sides if the side no longer has a super position
	if (m_CellGrid[newCell].second != -1)
		return;

	//Get bit alignment
	int bitAlign = 0;
	switch (side)
	{
	case Sides::TOP:	bitAlign = 0;
		break;
	case Sides::BOTTOM:	bitAlign = TILEAMOUNT;
		break;
	case Sides::LEFT:	bitAlign = TILEAMOUNT * 2;
		break;
	case Sides::RIGHT:	bitAlign = TILEAMOUNT * 3;
		break;
	}

	//Get all constraints
	int combinedConstraints = 0;
	for (int i = 0; i < TILEAMOUNT; i++)
	{
		if (m_CellGrid[currentCell].first & (1 << i))
		{
			int mask = ((1 << TILEAMOUNT) - 1) << bitAlign;
			mask &= m_Constraints[i];

			combinedConstraints |= mask;
		}
	}
	combinedConstraints = combinedConstraints >> bitAlign;

	int changedConstraints = combinedConstraints & m_CellGrid[newCell].first;
	if (changedConstraints == m_CellGrid[newCell].first)	//Constraints are already the same
		return;

	m_CellGrid[newCell].first &= combinedConstraints;

	if (m_CellGrid[newCell].first == 0)
		assert(true && "ERROR: Impossible combination");

	//Check if we have 1 possible tile left
	int positionsRemaining = 0;
	int tile = -1;
	for (int i = 0; i < TILEAMOUNT; i++)
	{
		if (m_CellGrid[newCell].first & (1 << i))
		{
			positionsRemaining++;
			tile = i;
		}
	}

	if (positionsRemaining == 0)
		assert(true && "ERROR: Impossible position remaining");
	else if (positionsRemaining == 1)
	{
		m_CellGrid[newCell].second = tile;
		m_SolvedTiles++;
	}

	//Check the sides of the cell that just changed
	CheckCell(newCell);
}

//Return cellgrid
const std::vector<std::pair<char, int>>& WFC::GetMap()
{
	return m_CellGrid;
}