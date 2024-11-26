#include <iostream>
#include <random>
#include <fstream>
#include <future>
#include <algorithm>

#include "Network.h"
#include "Tetris.h"

const int stateSize = 6;

struct FutureReward {
	float averageNextR;
	float averageNextQ;
};

struct Specimen {
	ShallowNetwork network;
	float result = -1;
};

std::vector<float> CreateState(const Board& board) {

	std::vector<float> state(stateSize);
	state[0] = std::powf(board.GetMaxHeight() / (float)Board::height, 2.0);
	state[1] = board.GetTotalHeight() / (float)(Board::width * Board::height);
	state[2] = board.CountCoveredCells() / (float)(Board::width * Board::height);
	state[3] = board.GetBumpiness() / (float)(Board::width * Board::height);
	state[4] = board.CountRowTransitions() / (float)(Board::width * Board::height);
	state[5] = board.CountRoofCells() / (float)(Board::width * Board::height);

	return state;
}

int SelectGreedyAction(const Board& board, const std::vector<std::vector<Coords>>& shapeType, const ShallowNetwork& network) {
	float max = -100000000;
	int bestMove;
	for (int i = 0; i < Board::width * 4; i++) {
		int x = i / 4, r = i % 4;
		if (board.Placeable(shapeType[r], { 0, x })) {
			Board next = board.FallWithoutFalling(shapeType[r], x);
			float value = network.FeedForward(CreateState(next));
			if (value > max || max == -100000000) {
				max = value;
				bestMove = i;
			}
		}
	}
	return bestMove;
}

void Test(Specimen* specimen, int numTests) {
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_int_distribution<> shapeDist(0, NUMSHAPES - 1);
	int totalScore = 0;
	for (int test = 0; test < numTests; test++) {
		Board board = Board();
		std::vector<std::vector<Coords>> currentShapeType = SHAPES[shapeDist(gen)];
		int blocks = 0;
		while (true) {
			int move = SelectGreedyAction(board, currentShapeType, (*specimen).network);
			board.Fall(currentShapeType[move % 4], move / 4);
			board.Scan();
			currentShapeType = SHAPES[shapeDist(gen)];
			blocks++;
			if (board.CheckLost(currentShapeType[0]) || blocks == 400) {
				break;
			}
		}
		totalScore += board.points;
	}
	(*specimen).result = (float)totalScore / (float)numTests;
}

bool CompareSpecimens(Specimen a, Specimen b) {
	return a.result > b.result;
}

void TrainGen() {

	int genSize = 100;
	int selectionSize = 10;
	int testNum = 20;
	int genNum = 1000;

	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_int_distribution<> selectionDist(0, selectionSize - 1);

	std::ofstream bests("bests.txt");
	std::ofstream scores("scores.txt");

	std::vector<Specimen> population;
	for (int i = 0; i < genSize; i++) {
		ShallowNetwork net = ShallowNetwork(stateSize);
		population.push_back({net, -1});
	}

	float mutationScale = 1.0f;

	for (int g = 0; g < genNum; g++) {
		std::vector<std::future<void>> futures;
		for (int n = 0; n < genSize; n++) {
			futures.push_back(std::async(std::launch::async, Test, &population[n], testNum));
		}
		for (int i = 0; i < futures.size(); i++) {
			futures[i].get();
		}
		std::sort(population.begin(), population.end(), CompareSpecimens);
		bests << population[0].result << std::endl;
		population[0].network.SaveParameters("weights");
		for (int n = selectionSize; n < genSize; n++) {
			population[n] = { population[selectionDist(gen)].network, -1 };
			population[n].network.Mutate(0.2f, mutationScale);
		}
		std::cout << population[0].result << "    " << mutationScale << std::endl;
	}

	scores.close();
	bests.close();
}

std::ostream& operator<<(std::ostream& stream, const std::vector<float>& vector) {
	stream << "{ ";
	for (int i = 0; i < vector.size() - 1; i++) {
		stream << vector[i] << ", ";
	}
	stream << vector[vector.size() - 1] << "}";
	return stream;
}

int main() {
	TrainGen();
}