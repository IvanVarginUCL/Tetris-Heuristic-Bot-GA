#pragma once

#include <vector>
#include <string>


class ShallowNetwork {

public:

	int inputNum;

	std::vector<float> weights;

	ShallowNetwork(int _inputNum);

	~ShallowNetwork();

	ShallowNetwork Copy() const;

	void Randomise(float scale);

	void Mutate(float chance, float scale);

	void SetWeights(std::vector<float> _weights);

	void SetAll(float value);

	float FeedForward(const std::vector<float>& inputs) const;

	void SaveParameters(std::string file) const;

};