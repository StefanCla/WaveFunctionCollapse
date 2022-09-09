#include "Precomp.hpp"
#include "WFC.hpp"

WFC::WFC() :
	m_TileAmount(-1),
	m_GuessedCell(-1)
{
	srand(0); //Seed with 0 for debugging
}

WFC::~WFC()
{}

void WFC::Init(unsigned int tileAmount, const std::vector<const int>& constraints)
{
	m_Constraints = constraints;
	m_TileAmount = tileAmount;

	for (int i = 0; i < (GRIDSIZEX * GRIDSIZEY); i++)
	{
		m_CellGrid.push_back(0);
		m_CopyCellGrid.push_back(0);
	}
}

void WFC::ChooseRandomCell()
{
	//Choose a random cell with a random tile to start the wave
	int cell = rand() % GRIDSIZE;
	int tile = rand() % TILEAMOUNT;


}

