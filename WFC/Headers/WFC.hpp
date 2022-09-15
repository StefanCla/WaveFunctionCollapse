#pragma once
#include <vector>

//Sides of a tile
enum class Sides
{
	TOP,
	BOTTOM,
	LEFT,
	RIGHT
};

class WFC
{
public:
	WFC(std::vector<int> constraints);
	~WFC();

	void Initialization();
	void StartWFC();
	void RestartWFC();

	const std::vector<std::pair<char,int>>& GetMap();

private:
	void CheckCell(int cell);
	void CheckSides(Sides side, int currentCell, int newCell);
	void ChooseRandomCell();

	std::vector<int> m_Constraints; //Rules for each tile

	std::vector<std::pair<char, int>> m_CellGrid; //First char are the posibilities, second int is the final tile. 
	std::vector<std::pair<char, int>> m_CopyCellGrid; //Not the best way, but copy the cell grid when starting to guess, to allow backtracking

	int m_GuessedCell;
	int m_GuessedTile;

	int m_SolvedTiles;
};