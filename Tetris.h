#pragma once

#include <vector>

#include "Shapes.h"

class Board {

public:

	static const int width = 10, height = 24;

	int state[height][width];
	int points;
	bool lost;

	Board();

	~Board();

	Board Copy() const;

	void Fall(std::vector<Coords> shape, int posX);

	void Scan();

	Board FallWithoutFalling(const std::vector<Coords>& shape, int posX) const;

	bool Placeable(const std::vector<Coords>& shape, Coords coords) const;

	bool CheckLost(const std::vector<Coords>& shape) const;

	// Heuristics 

	int GetMaxHeight() const;

	int GetTotalHeight() const;

	int CountCoveredCells() const;

	int GetBumpiness() const;

	int CountRowTransitions() const;

	int CountRoofCells() const;


	int GetColumnHeight(int x) const;

	int CountColumnCoveredCells(int x) const;


private:

	void Place(std::vector<Coords> shape, Coords coords);

	bool CheckRow(int index) const;

};