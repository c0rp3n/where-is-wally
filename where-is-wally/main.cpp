#include <cstdint>
#include <iostream>
#include <memory>
#include <string>
#include <thread>
#include <utility>
#include <vector>

#include "Utils/Utils.hpp"
#include "Types/Types.hpp"

int main()
{
    // Initailise a new logger instance then create a module logger of main.
    Utils::Logger::Instance().Init(Utils::Logger::Console, "logs/where-is-wally.log");
    Utils::MLogger logger = Utils::MLogger::MLogger("Main");

    // Instanciating Matrix Objects
    Matrix<uint32_t> wally;
    Matrix<uint32_t> wheresWally;

    // Creating a scope for each image load as to delete the TextImage object after loading in the matrix as it is not neede for the rest of the programs execution.
    {
        // Fetch the image path, this code differs depending on he build type as to save time when debugging.
#if _DEBUG
        std::unique_ptr<TextImage> wallyTextImage(new TextImage("C:\\programming\\where-is-wally\\Wally_grey.txt"));
#else
        std::unique_ptr<TextImage> wallyTextImage(new TextImage(Utils::IO::InputFilePath("Please input the filepath too Wally: ")));
#endif
        wally = Matrix<uint32_t>::Matrix(wallyTextImage->x, wallyTextImage->y, wallyTextImage->data);
    }
    {
#if _DEBUG
        std::unique_ptr<TextImage> wheresWallyTextImage(new TextImage("C:\\programming\\where-is-wally\\Cluttered_scene.txt"));
#else
        std::unique_ptr<TextImage> wheresWallyTextImage(new TextImage(Utils::IO::InputFilePath("Please input the filepath too the Where's Wally: ")));
#endif
        wheresWally = Matrix<uint32_t>::Matrix(wheresWallyTextImage->x, wheresWallyTextImage->y, wheresWallyTextImage->data);
    }

    // Set the current correlation measure for the search,
    CECorrelationMeasure crMeasure = CE_SUM_OF_SQUARED_DIFFERENCES;
    // Set the ranking type for the result set based upon the correlation measure.
    RankedSet<double, std::pair<int, int>> results;
    if (crMeasure == CE_SUM_OF_SQUARED_DIFFERENCES)
    {
        results = RankedSet<double, std::pair<int, int>>::RankedSet(10, RS_SMALLEST_VALUE);
    }
    else if (crMeasure == CE_NORMALISED_CROSS_CORRELATION)
    {
        results = RankedSet<double, std::pair<int, int>>::RankedSet(10, RS_LARGEST_VALUE);
    }
    {
        // Fetch the hardware concurrency/ thread count of the current processor.
        uint32_t threadCount = std::thread::hardware_concurrency();
        // Calculates how to evenly split the work between all of the threads.
        uint32_t threadYStep = (wheresWally.y - wally.y) / threadCount;
        logger.Log("Current Thread Count %i with a set area of %i", threadCount, threadYStep);

        std::vector<ComputeEngine> computeEngines;
        int currentStep = 0;
        // Comparison to make sure the right values are passed to the ComputeEngine based upon which operation is going to be performed.
        if (crMeasure == CE_SUM_OF_SQUARED_DIFFERENCES)
        {
            for (uint32_t i = 0; i < threadCount - 1u; i++)
            {
                // Add a thread to the compute pool this is construct from references to the matrix's as well as the bounds of this threads search.
                computeEngines.push_back(ComputeEngine::ComputeEngine(crMeasure, &wally, &wheresWally, 0, currentStep, wheresWally.x - wally.x, currentStep + threadYStep, 10));
                currentStep += threadYStep;
            }
            computeEngines.push_back(ComputeEngine::ComputeEngine(crMeasure, &wally, &wheresWally, 0, currentStep, wheresWally.x - wally.x, wheresWally.y - wally.y, 10));
        }
        else if (crMeasure == CE_NORMALISED_CROSS_CORRELATION)
        {
            // Calculate the extra values needed for Normalised cross correlation.
            double wallyMean = wally.Mean();
            wally = wally - wallyMean;
            for (uint32_t i = 0; i < threadCount - 1u; i++)
            {
                computeEngines.push_back(ComputeEngine::ComputeEngine(crMeasure, &wally, &wheresWally, 0, currentStep, wheresWally.x - wally.x, currentStep + threadYStep, 10, wallyMean));
                currentStep += threadYStep;
            }
            computeEngines.push_back(ComputeEngine::ComputeEngine(crMeasure, &wally, &wheresWally, 0, currentStep, wheresWally.x - wally.x, wheresWally.y - wally.y, 10, wallyMean));
        }

        {
            // Create a thread for each of the created compute engine.
            std::vector<std::thread> threads;
            std::vector<ComputeEngine>::iterator itc = computeEngines.begin();
            while (itc != computeEngines.end())
            {
                // Creating and pushing a new thread object to the vector,
                // each thread receives the offset / pointer to the compute
                // function in a ComputeEngine object as well as a reference
                // to the pointer of the current compute engine.
                threads.push_back(std::thread::thread(&ComputeEngine::Compute, std::ref(*itc)));
                itc++;
            }

            // Wait for all of the threads to finish by joining them back to the main thread.
            std::vector<std::thread>::iterator itt = threads.begin();
            while (itt != threads.end())
            {
                (*itt).join();
                itt++;
            }

            threads.clear();
        }
        
        {
            // Iterate through all of the results from the compute engines.
            std::vector<ComputeEngine>::iterator itc = computeEngines.begin();
            while (itc != computeEngines.end())
            {
                RankedSet<double, std::pair<int, int>>::iterator itr = (*itc).results.data.begin();
                while (itr != (*itc).results.data.end())
                {
                    // Try to push all of the results into the new result set so only the top n remain.
                    results.Push(*itr);
                    itr++;
                }
                itc++;
            }
        }

        computeEngines.clear();
    }

    // Create a new scope and loop through all of the results.
    {
        // Loop through and print the top n results.
        logger.Log("Results of the Nearest-Neighbour Search");
        int i = 1;
        RankedSet<double, std::pair<int, int>>::iterator it = results.data.begin();
        while (it != results.data.end())
        {
            logger.Log("Result %i with a rank of %f and a location of %i %i", i, (*it).first, (*it).second.first, (*it).second.second);
            it++;
            i++;
        }

        // Create a rectangle which is one pixel wider than wally in all directions.
        // Then iterate along the edge of this new rectangle as to draw a black border
        // around the area that wally is most likely to be located at.
        Rect wallyBorder = Rect::Rect(results[0].second.first - 1, results[0].second.second - 1, wally.x + 1, wally.y + 1);
        for (Vec2 position = wallyBorder.BottomLeft(); position.y < wallyBorder.TopLeft().y; position.y++)
        {
            wheresWally.Set(position.x, position.y, 0);
        }
        for (Vec2 position = wallyBorder.TopLeft(); position.x < wallyBorder.TopRight().x; position.x++)
        {
            wheresWally.Set(position.x, position.y, 0);
        }
        for (Vec2 position = wallyBorder.TopRight(); position.y > wallyBorder.BottomRight().y; position.y--)
        {
            wheresWally.Set(position.x, position.y, 0);
        }
        for (Vec2 position = wallyBorder.BottomRight(); position.x > wallyBorder.BottomLeft().x; position.x--)
        {
            wheresWally.Set(position.x, position.y, 0);
        }
    }

    // Create a new scope for each PGM object as to not retain the data.
    {
        std::unique_ptr<NetPBM::PGM> wheresWallyPGM(new NetPBM::PGM(wheresWally.x, wheresWally.y, 255u, &wheresWally.data));
        wheresWallyPGM->Save("wheresWally.pgm", PBM_BINARY);
    }

    logger.Log("Press any key to close the program...");
    std::cin.clear();
    std::cin.get();

    return 0;
}
