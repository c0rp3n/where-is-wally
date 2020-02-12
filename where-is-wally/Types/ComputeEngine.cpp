#include "ComputeEngine.hpp"

#include <memory>

ComputeEngine::ComputeEngine()
{
}

ComputeEngine::ComputeEngine(CECorrelationMeasure correlationType, Matrix<uint32_t>* search, Matrix<uint32_t>* sample, int sampleStartX, int sampleStartY, int sampleEndX, int sampleEndY, int setSize, double searchMean)
{
    this->correlationType = correlationType;
    this->search = search;
    this->sample = sample;
    this->sampleStartX = sampleStartX;
    this->sampleStartY = sampleStartY;
    this->sampleEndX = sampleEndX;
    this->sampleEndY = sampleEndY;
    this->searchMean = searchMean;
    this->results = data_type::RankedSet(setSize);
}


ComputeEngine::~ComputeEngine()
{
}

void ComputeEngine::Compute()
{
    // Choose which function to use for computing.
    switch (this->correlationType)
    {
    case CE_SUM_OF_SQUARED_DIFFERENCES:
        this->ComputeSampleSoSD();
        break;
    case CE_NORMALISED_CROSS_CORRELATION:
        this->ComputeSampleNCC();
        break;
    }
}

void ComputeEngine::ComputeSampleSoSD()
{
    // Set the ranking type then loop through all of the areas in this compute engines
    // sample area and push those values to the ranked set.
    results.SetRankingType(RS_SMALLEST_VALUE);
    for (int y = this->sampleStartY; y < this->sampleEndY; y++)
    {
        for (int x = this->sampleStartX; x < this->sampleEndX; x++)
        {
            results.Push(data_type::value_type::pair(this->ComputeSoSD(x, y), std::pair<int, int>::pair(x, y)));
        }
    }
}

void ComputeEngine::ComputeSampleNCC()
{
    results.SetRankingType(RS_LARGEST_VALUE);
    for (int y = this->sampleStartY; y < this->sampleEndY; y++)
    {
        for (int x = this->sampleStartX; x < this->sampleEndX; x++)
        {
            results.Push(data_type::value_type::pair(this->ComputeNCC(x, y), std::pair<int, int>::pair(x, y)));
        }
    }
}

double ComputeEngine::ComputeSoSD(int sampleX, int sampleY)
{
    // Loop through the wally matrix and from the current sample position for the size of wally
    // to compute the current correlation measure.
    double sosd = 0.0;
    for (int y = 0; y < this->search->y; y++)
    {
        for (int x = 0; x < this->search->x; x++)
        {
            if (this->search->Get(x, y) != 255u)
            {
                sosd += std::pow(static_cast<double>(this->search->Get(x, y)) - static_cast<double>(this->sample->Get(x + sampleX, y + sampleY)), 2.0);
            }
        }
    }

    return sosd;
}

double ComputeEngine::ComputeNCC(int sampleX, int sampleY)
{
    double sampleMean = 0.0;
    Matrix<uint32_t> subSample = Matrix<uint32_t>::Matrix(this->search->x, this->search->y);
    {
        uint32_t temp;
        for (int y = 0; y < this->search->y; y++)
        {
            for (int x = 0; x < this->search->x; x++)
            {
                temp = this->sample->Get(x + sampleX, y + sampleY);
                sampleMean += temp;
                subSample.data.push_back(temp);
            }
        }
    }
    sampleMean /= this->search->x * this->search->y;
    
    subSample = subSample - sampleMean;
    double nccTop = 0.0, nccBottomLeft = 0.0, nccBottomRight = 0.0;
    for (int y = 0; y < this->search->y; y++)
    {
        for (int x = 0; x < this->search->x; x++)
        {
            nccTop += this->search->Get(x, y) * subSample.Get(x, y);
            nccBottomLeft += std::pow(this->search->Get(x, y), 2.0);
            nccBottomRight += std::pow(subSample.Get(x, y), 2.0);
        }
    }

    return nccTop / std::sqrt(nccBottomLeft * nccBottomRight);
}
