#pragma once

#include <ctime>
#include <chrono>
#include <iomanip>
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <stdio.h>
#include <time.h>
#include <stdarg.h>
#include <memory>

#include "Singleton.hpp"

namespace Utils
{
    class Logger : public Singleton<Logger>
    {
    public:
        enum OutputModes : uint8_t
        {
            None = 0,
            Console = 1 << 0,
            File = 1 << 1,
            Both = Console | File,
        };

    private:
        OutputModes fileOutput;

    public:
        Logger()
        {
        }
     
        virtual ~Logger()
        {
        }

        void Init(OutputModes fileOutput = Console, std::string path = "logs/log.txt")
        {
            this->fileOutput = fileOutput;
            if (this->fileOutput & File)
            {
                logFileStream.open(path);
            }
        }
        
        void Close()
        {
            if (this->fileOutput & File)
            {
                logFileStream.close();
            }
        }
        
        void Log(std::string msg)
        {
            if (this->fileOutput & File)
            {
                logFileStream << GetTimeStamp() << msg << std::endl;
            }
            if (this->fileOutput & Console)
            {
                std::cout << msg << std::endl;
            }
        }
        
        template<typename ... Args>
        void Log(std::string msg, Args ... args)
        {
            if (this->fileOutput & File)
            {
                logFileStream << GetTimeStamp() << Format(msg, args ...) << std::endl;
            }
            if (this->fileOutput & Console)
            {
                std::cout << Format(msg, args ...) << std::endl;
            }
        }

    private:
        std::string GetTimeStamp()
        {
            std::time_t time = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
            std::stringstream sstream;
            sstream << "[" << std::put_time(std::localtime(&time), "%F %T") << "]";
            return sstream.str();
        }
        
        template<typename ... Args>
        std::string Format(const std::string& format, Args ... args)
        {
            size_t size = (size_t)snprintf(nullptr, 0, format.c_str(), args ...) + 1;
            std::unique_ptr<char[]> buf(new char[size]);
            snprintf(buf.get(), size, format.c_str(), args ...);
            return std::string(buf.get(), buf.get() + size - 1);
        }
        
        std::ofstream logFileStream;
    };
}
