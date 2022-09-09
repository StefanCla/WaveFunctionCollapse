#pragma once
#include <vector>

class WFC
{
public:
	WFC();
	~WFC();

	void Init(unsigned int tileAmount, const std::vector<const int>& constraints);
	void ChooseRandomCell();

private:
	unsigned int m_TileAmount;
	std::vector<const int> m_Constraints; //Rules for each tile

	std::vector<int> m_CellGrid; //Int will act as bool for which tiles are able to be placed there
	std::vector<int> m_CopyCellGrid; //Not the best way, but copy the cell grid when starting to guess, to allow backtracking

	int m_GuessedCell;
};