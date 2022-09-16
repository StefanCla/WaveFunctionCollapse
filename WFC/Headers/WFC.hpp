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
	WFC(const long long* constraints);
	~WFC();

	void Initialization();
	void StartWFC();
	void RestartWFC();

	const std::pair<int, int>* GetMap();

private:
	void CheckCell(int cell);
	void CheckSides(Sides side, int currentCell, int newCell);
	void ChooseRandomCell();

	const long long* m_Constraints; //Rules for each tile
	std::pair<int, int>* m_CellGrid;//First char are the posibilities, second int is the final tile

	int m_GuessedCell;
	int m_GuessedTile;

	int m_SolvedTiles;
};