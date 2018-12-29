#pragma once

#include <algorithm>
#include <cstdint>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

#include "../Utils/Utils.hpp"

class TextImage
{
public:
	Utils::MLogger logger = Utils::MLogger::MLogger("TextImage");
	int x; 
	int y;
	std::vector<uint32_t> data;

	TextImage();
	TextImage(std::string filepath);
	~TextImage();

private:
	void GetTextImageSize(std::string filepath);
	void ReadTextImage(std::string filepath);
};

