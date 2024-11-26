#include <random>
#include <iostream>
#include <fstream>
#include <math.h>

#include "Network.h"


// Shallow Network

// Public

ShallowNetwork::ShallowNetwork(int _inputNum) {
	inputNum = _inputNum;
	weights = std::vector<float>(inputNum, 0.0f);
}

ShallowNetwork::~ShallowNetwork() {}

ShallowNetwork ShallowNetwork::Copy() const {
	ShallowNetwork copy = ShallowNetwork(inputNum);
	for (int c = 0; c < inputNum; c++) {
		copy.weights[c] = weights[c];
	}
	return copy;
}

void ShallowNetwork::Randomise(float scale) {
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_real_distribution<> dist(0, 1);
	for (int c = 0; c < inputNum; c++) {
		weights[c] = (dist(gen) * 2 - 1) * scale;
	}
}

void ShallowNetwork::Mutate(float chance, float scale) {
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_real_distribution<> dist(0, 1);
	for (int c = 0; c < inputNum; c++) {
		if (dist(gen) < chance) {
			weights[c] += (dist(gen) * 2 - 1) * scale;
		}
	}
}

void ShallowNetwork::SetWeights(std::vector<float> _weights) {
	for (int i = 0; i < inputNum; i++) {
		weights[i] = _weights[i];
	}
}

void ShallowNetwork::SetAll(float value) {
	for (int c = 0; c < inputNum; c++) {
		weights[c] = value;
	}
}

float ShallowNetwork::FeedForward(const std::vector<float>& inputs) const {
	float total = 0;
	for (int c = 0; c < inputNum; c++) {
		total += inputs[c] * weights[c];
	}
	return total;
}

void ShallowNetwork::SaveParameters(std::string file) const {
	std::ofstream wFile(file + ".txt");
	for (int c = 0; c < inputNum; c++) {
		wFile << weights[c] << std::endl;
	}
	wFile.close();
}