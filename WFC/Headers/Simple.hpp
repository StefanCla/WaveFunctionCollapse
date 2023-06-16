#pragma once
#include <vector>
#include <stack>

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

	void Initialization();
	void StartWFC();
	void RestartWFC();

	std::vector<std::vector<int>> GetCellGrid();

private:
	void RunWFC();
	int ChooseRandomCell();
	bool CheckCell(int cell);
	bool CheckSides(SideD side, int currentCell, int newCell);


	int GetAdjacentTile(int currentCell, SideD side);
	bool CheckSide(int currenCell, int adjacentCell, SideD side);

private:
	std::vector<std::vector<int>> m_CellGrid;
	//std::vector<int>* m_CellStack;
	Constrain& m_Constrain;

	int m_RestartCount;
	int m_SolvedTiles;
	bool m_Stable;

};