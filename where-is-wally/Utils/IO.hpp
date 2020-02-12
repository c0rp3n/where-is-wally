#pragma once

#include <algorithm>
#include <cstdint>
#include <fstream>
#include <iostream>
#include <string>
#include <utility>
#include <vector>

#include "MLogger.hpp"

namespace Utils
{
    class IO
    {
    public:
        static std::string InputFilePath(std::string message = "Please input a filepath: ", std::string errorMessage = "Please input a correct file path.")
        {
            MLogger logger = MLogger::MLogger("IO");

            std::string filepath;
            bool invalidPath = true;
            while (invalidPath)
            {
                logger.Log(message);
                std::cin >> filepath;

                if (filepath.find('\"') != std::string::npos)
                {
                    filepath.erase(std::remove(filepath.begin(), filepath.end(), '\"'), filepath.end());
                }

                std::ifstream file(filepath);
                if (file.is_open())
                {
                    file.close();
                    invalidPath = false;
                }

                std::cin.ignore();
            }

            logger.Error(errorMessage);

            return filepath;
        }
    };
}
