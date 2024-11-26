#include <vector>
#include <iostream>

#include "Shapes.h"
#include "Tetris.h"

// Public

Board::Board() {
	for (int y = 0; y < height; y++) {
		for (int x = 0; x < width; x++) {
			state[y][x] = 0;
		}
	}
	points = 0;
	lost = false;
}

Board::~Board() {

}

Board Board::Copy() const {
	Board copy = Board();
	for (int y = 0; y < height; y++) {
		for (int x = 0; x < width; x++) {
			copy.state[y][x] = state[y][x];
		}
	}
	copy.points = points;
	copy.lost = lost;
	return copy;
}

void Board::Fall(std::vector<Coords> shape, int posX) {
	if (!Placeable(shape, { 0, posX })) {
		throw std::invalid_argument("Cannot drop shape at given position.");
	}
	int posY = 1;
	while (Placeable(shape, { posY, posX })) posY++;
	posY--;
	Place(shape, { posY, posX });
	points += posY;
}

void Board::Scan() {
	bool rows[height];
	for (int y = 0; y < height; y++) {
		rows[y] = CheckRow(y);
	}

	int rowCount = 0;
	for (int y = 0; y < height; y++) {
		if (rows[y]) {
			rowCount++;
			for (int y2 = y; y2 >= 0; y2--) {
				for (int x = 0; x < width; x++) {
					if (y2 == 0) {
						state[y2][x] = 0;
					}
					else {
						state[y2][x] = state[y2 - 1][x];
					}
				}
			}
		}
	}

	int rewards[5] = { 0, 25, 100, 400, 1600 };
	points += rewards[rowCount];
}

Board Board::FallWithoutFalling(const std::vector<Coords>& shape, int posX) const {
	Board newBoard = Copy();
	newBoard.Fall(shape, posX);
	newBoard.Scan();
	return newBoard;
}

bool Board::Placeable(const std::vector<Coords>& shape, Coords coords) const {
	for (int i = 0; i < shape.size(); i++) {
		if ((coords.x + shape[i].x) > (width - 1) || (coords.y + shape[i].y) > (height - 1)) {
			return false;
		}
		if (state[coords.y + shape[i].y][coords.x + shape[i].x] == 1) {
			return false;
		}
	}
	return true;
}

bool Board::CheckLost(const std::vector<Coords>& shape) const {
	return !Placeable(shape, { 0, 4 });
}

// Heuristics

int Board::GetMaxHeight() const {
	for (int y = 0; y < height; y++) {
		for (int x = 0; x < width; x++) {
			if (state[y][x]) {
				return 24 - y;
			}
		}
	}
	return 0;
}

int Board::GetTotalHeight() const {
	int total = 0;
	for (int x = 0; x < width; x++) {
		for (int y = 0; y < height; y++) {
			if (state[y][x]) {
				total += (24 - y);
				break;
			}
		}
	}
	return total;
}

int Board::CountCoveredCells() const {
	int count = 0;
	for (int x = 0; x < width; x++) {
		bool covered = false;
		for (int y = 0; y < height; y++) {
			if (state[y][x]) {
				covered = true;
				continue;
			}
			if (covered && state[y][x] == 0) {
				count++;
			}
		}
	}
	return count;
}

int Board::GetBumpiness() const {
	int count = 0;
	int last = GetColumnHeight(0);
	for (int x = 1; x < width; x++) {
		int height = GetColumnHeight(x);
		count += std::abs(height - last);
		last = height;
	}
	return count;
}

int Board::CountRowTransitions() const {
	int count = 0;
	for (int y = 0; y < height; y++) {
		for (int x = 1; x < width; x++) {
			if (state[y][x] != state[y][x - 1]) {
				count++;
			}
		}
	}
	return count;
}

int Board::CountRoofCells() const {
	int total = 0;
	for (int x = 0; x < width; x++) {
		bool hole = false;
		for (int y = height - 1; y >= 0; y--) {
			if (state[y][x]) {
				if (hole) total++;
			}
			else {
				hole = true;
			}
		}
	}
	return total;
}

// Auxillary

int Board::GetColumnHeight(int x) const {
	for (int y = 0; y < height; y++) {
		if (state[y][x]) {
			return 24 - y;
		}
	}
	return 0;
}

int Board::CountColumnCoveredCells(int x) const {
	bool covered = false;
	int count = 0;
	for (int y = 0; y < height; y++) {
		if (state[y][x]) {
			covered = true;
			continue;
		}
		if (covered && state[y][x] == 0) {
			count++;
		}
	}
	return count;
}


// Private

void Board::Place(std::vector<Coords> shape, Coords coords) {
	for (int i = 0; i < shape.size(); i++) {
		state[coords.y + shape[i].y][coords.x + shape[i].x] = 1;
	}
}

bool Board::CheckRow(int index) const {
	for (int x = 0; x < width; x++) {
		if (state[index][x] == 0) {
			return false;
		}
	}
	return true;
}