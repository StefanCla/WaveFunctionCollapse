#pragma once
#include <vector>

class WFC
{
public:
	WFC();
	~WFC();

	void Init(unsigned int tileAmount, std::vector<int> constraints);
	void ChooseRandomCell();
	//void CheckConstraints(int cell);

	void StartWFC();
	void CheckSides(int side, int currentCell, int newCell);
	void CheckCell(int cell);
	void ResetCell();
	void FinishWFC();

	const std::vector<int>& GetMap();

private:
	unsigned int m_TileAmount;
	std::vector<int> m_Constraints; //Rules for each tile

	std::vector<std::pair<char, int>> m_CellGrid; //First int are the posibilities, second int is the final tile. 
	std::vector<std::pair<char, int>> m_CopyCellGrid; //Not the best way, but copy the cell grid when starting to guess, to allow backtracking

	int m_GuessedCell;
	int m_GuessedTile;

	std::vector<int> m_DisplayVector;
};