#pragma once

#include <iostream>
#include <fstream>
#include <string>
#include <stdio.h>

#include "Logger.hpp"

namespace Utils
{
    class MLogger
    {
	public:
		MLogger()
		{
		};

        MLogger(std::string module)
        {
            logger = &Logger::Instance();
            moduleName = module;
        };

		virtual ~MLogger()
		{
		}
        
    private:
        std::string moduleName;
        Logger *logger;
    
    public:
		void Log(std::string msg)
        {
            logger->Log("[" + moduleName + "]: " + msg);
        }
    
        template<typename ... Args>
        void Log(std::string msg, Args ... args)
        {
            logger->Log("[" + moduleName + "]: " + msg, args ...);
        }
        
        void Info(std::string msg)
        {
            logger->Log("[" + moduleName + "] Info: " + msg);
        }
    
        template<typename ... Args>
        void Info(std::string msg, Args ... args)
        {
            logger->Log("[" + moduleName + "] Info: " + msg, args ...);
        }
    
        void Error(std::string msg)
        {
            logger->Log("[" + moduleName + "] Error: " + msg);
        }
    
        template<typename ... Args>
        void Error(std::string msg, Args ... args)
        {
            logger->Log("[" + moduleName + "] Error: " + msg, args ...);
        }
    };
}
