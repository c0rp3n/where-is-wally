#pragma once

#include "Matrix.hpp"
#include "RankedSet.hpp"

enum CECorrelationMeasure : uint32_t
{
	CE_SUM_OF_SQUARED_DIFFERENCES = 1 << 0,
	CE_NORMALISED_CROSS_CORRELATION = 1 << 1,
};

class ComputeEngine
{
public:
	using data_type = RankedSet<double, std::pair<int, int>>;

	CECorrelationMeasure correlationType;
	Matrix<uint32_t>* search;
	Matrix<uint32_t>* sample;
	int sampleStartX;
	int sampleStartY;
	int sampleEndX;
	int sampleEndY;
	double searchMean;
	data_type results;

	ComputeEngine();
	ComputeEngine(CECorrelationMeasure correlationType, Matrix<uint32_t>* search, Matrix<uint32_t>* sample, int sampleStartX, int sampleStartY, int sampleEndX, int sampleEndY, int setSize = 5, double searchMean = 0.0);
	~ComputeEngine();

	void Compute();

private:
	void ComputeSampleSoSD();
	void ComputeSampleNCC();
	double ComputeSoSD(int sampleX, int sampleY);
	double ComputeNCC(int sampleX, int sampleY);
};

