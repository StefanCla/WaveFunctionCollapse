#include "Precomp.hpp"
#include "WFC.hpp"
#include <assert.h>
#include <bitset>

WFC::WFC() :
	m_TileAmount(-1),
	m_GuessedCell(-1)
{
	srand(0); //Seed with 0 for debugging
}

WFC::~WFC()
{}

void WFC::Init(unsigned int tileAmount, std::vector<int> constraints)
{
	m_Constraints = constraints;
	m_TileAmount = tileAmount;

	char possibleTiles = 1 << 0 | 1 << 1 | 1 << 2 | 1 << 3 | 1 << 4 | 1 << 5;

	for (int i = 0; i < GRIDSIZE; i++)
	{
		m_CellGrid.push_back(std::pair<char, int>( possibleTiles, -1));
		//m_CopyCellGrid.push_back(0);
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

	//Begin WFC
	CheckCell(cell);

	FinishWFC();
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
				printf("Error");
		}
	}

	//Choose cell with lowest entropy
	int cell = rand() % lowestEntropyCells.size();

	//Choose tile in the lowest entropy cell
	int tileID = rand() % flagCount + 1;
	int tile = -1;
	for (int i = 0; i < TILEAMOUNT; i++)
	{
		if (m_CellGrid[lowestEntropyCells[cell]].first & (1 << i))
		{
			tileID--;
			if (tileID <= 0)
			{
				tile = i;
				break;
			}
		}
	}

	m_CellGrid[lowestEntropyCells[cell]].first = 0;
	m_CellGrid[lowestEntropyCells[cell]].first = 1 << tile;
	m_CellGrid[lowestEntropyCells[cell]].second = tile;

	m_GuessedCell = lowestEntropyCells[cell];
	m_GuessedTile = tile;

	CheckCell(lowestEntropyCells[cell]);

	////Choose a random cell with a random tile to start the wave
	//int cell = rand() % GRIDSIZE;
	//int tile = rand() % TILEAMOUNT;

	//m_CellGrid[cell].first = 0;
	//m_CellGrid[cell].first = 1 << tile;
	//m_CellGrid[cell].second = tile;

	//m_GuessedCell = cell;

	//CheckCell(cell);
}

////Check constraints of all the tiles
//void WFC::CheckConstraints(int cell)
//{
//	//Get cell
//	//Check order [Top / bottom / left / right]
//	//If any of these have a single tile or no change was made ignore and check other direction.
//	//If tiles conflict, throw an error
//	//Do this until no other single tile can be concluded (All options have at least 2 possible tiles remaining)
//
//	//If all options remaining have at least 2 options, get a new random tile with lowest entropy
//	//Check constraints again.
//
//	//If all cells have a single tile, return that array.
//
//
//
//	//ISSUE
//	//Have to keep track of each tile which direction was already checked.
//	//This is going to be a recursive function
//
//
//
//
//	//Full idea:
//	//Get a cell, and check in the following order [Top / Bottom / Left / Right]
//	//If any of those cells did not change, already have a single tile or are out of bounds, ignore them and continue to the next direction.
//	//If the tile was already changed and now conflicts (Impossible to solve) throw an error.
//	//If the tile now has a single super position left, change that cell to have its second as that tile.
//
//	//If the tile did get changed (6 options > 3 options), start a recursive function with that cell.
//	//Repeat this function within that cell.
//	//If no changes were made, go back to the previous cell and continue checking directions.
//
//	//If all directions have been checked and changed, and tiles still remain in a super position, choose a random tile with lowest entropy.
//	//Once the cell & tile has been chosen, repeat this function.
//
//	//Do this until all cells are no longer in a super position.
//
//	//Result is m_CellGrid where all seconds are no longer -1, but instead have a valid number and where all cells have 1 bit in first remaining.
//
//	int topCell = cell - GRIDSIZEY;
//	if (topCell < 0) //Out of bounds
//		return;
//
//	if (m_CellGrid[topCell].second != -1) //Already has a single position
//		return;
//
//	//Get constraints from all possible tiles of cell.first
//	//Check if the constraints are the same as m_CellGrid[topCell].first, if they are return
//	int bitAllign;
//	int tileSide = 0;
//
//	switch (tileSide)
//	{
//	case 0: bitAllign = 0; bitAllign = (1 << 0);
//		break;
//	case 1:	bitAllign = 0; bitAllign = (1 << 6);
//		break;
//	case 2: bitAllign = 0; bitAllign = (1 << 12);
//		break;
//	case 3: bitAllign = 0; bitAllign = (1 << 18);
//		break;
//	}
//
//	int combinedConstraints = 0;
//	for (int i = 0; i < TILEAMOUNT; i++)
//	{
//		if (m_CellGrid[cell].first == (1 << i))
//		{
//			//Left example
//			combinedConstraints |= (m_Constraints[i] << (bitAllign + 0));
//			combinedConstraints |= (m_Constraints[i] << (bitAllign + 1));
//			combinedConstraints |= (m_Constraints[i] << (bitAllign + 2));
//			combinedConstraints |= (m_Constraints[i] << (bitAllign + 3));
//			combinedConstraints |= (m_Constraints[i] << (bitAllign + 4));
//			combinedConstraints |= (m_Constraints[i] << (bitAllign + 5));
//		}
//	}
//
//	if (combinedConstraints == m_CellGrid[topCell].first) //No change has been made
//		return;
//
//	m_CellGrid[topCell].first &= combinedConstraints;
//
//	if (m_CellGrid[topCell].first == 0)
//		assert(true && "ERROR: Impossible combination");
//
//	//Check if 1 position is left, if so, change second to that tile.
//
//
//
//
//
//
//
//
//	int topCell = cell - GRIDSIZEY;
//	if (topCell >= 0)
//	{
//		//Check cell
//		if (m_CellGrid[topCell].second == -1)
//		{
//			int possibleTiles = m_CellGrid[topCell].first;
//			//Bitwise operator
//			int remainingTiles = m_CellGrid[cell].first;
//
//			if (possibleTiles == remainingTiles)
//			{
//
//			}
//
//			int resultTiles = possibleTiles & remainingTiles;
//
//			if (resultTiles != 0)
//			{
//				m_CellGrid[topCell].first = resultTiles;
//			}
//		}
//
//		//Apply the difference
//		//
//	}
//
//}

//Check the sides of each cell
void WFC::CheckCell(int cell)
{
	int topCell = cell - GRIDSIZEY;
	int bottomCell = cell + GRIDSIZEY;
	int leftCell = -1;
	int rightCell = -1;

	if (cell % GRIDSIZEX != 0)
		leftCell = cell - 1;

	if ((cell + 1) % GRIDSIZEX != 0)
		rightCell = cell + 1;

	if(topCell >= 0)
		CheckSides(0, cell, topCell);
	if(bottomCell < m_CellGrid.size())
		CheckSides(1, cell, bottomCell);
	if (leftCell != -1)
		CheckSides(2, cell, leftCell);
	if (rightCell != -1)
		CheckSides(3, cell, rightCell);

	//Check if its solved, if not, get random cell with lowest entropy and choose a random tile. Recurse again

	bool isSolved = true;
	for (int i = 0; i < GRIDSIZE; i++)
	{
		if (m_CellGrid[i].second == -1)
			isSolved = false;
	}

	if (!isSolved)
		ChooseRandomCell();
}

//Check if we can reduce the super position of the new cell
void WFC::CheckSides(int side, int currentCell, int newCell)
{
	if (m_CellGrid[newCell].second != -1)
		return;

	if (newCell == 4)
		printf("break\n");

	//Get bit alignment
	int bitAllign = 0;
	switch (side)
	{
	case 0: bitAllign = 0;
		break;
	case 1:	bitAllign = TILEAMOUNT;
		break;
	case 2: bitAllign = TILEAMOUNT * 2;
		break;
	case 3: bitAllign = TILEAMOUNT * 3;
		break;
	}

	////Test
	//int lowestEntropyCell = 0;
	//size_t flagCount = -1;
	//for (int i = 0; i < GRIDSIZE; i++)
	//{
	//	std::bitset<sizeof(m_CellGrid[i].first)> bits(m_CellGrid[i].first);
	//	std::size_t num = bits.count();
	//	
	//	if (flagCount == -1)
	//	{
	//		flagCount = num;
	//		lowestEntropyCell = i;
	//	}
	//	else if (num < flagCount)
	//	{
	//		flagCount = num;
	//		lowestEntropyCell = i;
	//	}
	//}
	////Test

	//Get all constraints
	int combinedConstraints = 0;
	for (int i = 0; i < TILEAMOUNT; i++)
	{
		if (m_CellGrid[currentCell].first & (1 << i))
		{
			int mask = ((1 << 6) - 1) << bitAllign;
			mask &= m_Constraints[i];

			combinedConstraints |= mask;
		}
	}

	combinedConstraints = combinedConstraints >> bitAllign;

	if (combinedConstraints == m_CellGrid[newCell].first) //No change has been made
		return;

	m_CellGrid[newCell].first &= combinedConstraints;

	if (m_CellGrid[newCell].first == 0)
		printf("ERROR: Impossible combination\n");
		//assert(true && "ERROR: Impossible combination");

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
		printf("ERROR: Impossible combination!\n");
		//assert(true && "ERROR: Impossible");
	else if (positionsRemaining == 1)
		m_CellGrid[newCell].second = tile;
	else
	{
		//Recurse function
		CheckCell(newCell);
		return;
	}
}

void WFC::ResetCell()
{

}

void WFC::FinishWFC()
{
	for (int i = 0; i < GRIDSIZE; i++)
		m_DisplayVector.push_back(m_CellGrid[i].second);
}

const std::vector<int>& WFC::GetMap()
{
	return m_DisplayVector;
}