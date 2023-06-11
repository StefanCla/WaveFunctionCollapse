#pragma once
#include <vector>

class Constrain;

enum class SideD
{
	Up,
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
	void ChooseRandomCell();
	void CheckCell(int cell);
	void CheckSides(SideD side, int currentCell, int newCell);

private:
	std::vector<std::vector<int>> m_CellGrid;
	Constrain& m_Constrain;

	int m_RestartCount;
	int m_SolvedTiles;

};