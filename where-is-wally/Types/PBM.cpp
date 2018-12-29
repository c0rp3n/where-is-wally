#include "NetPBM.hpp"

using namespace NetPBM;

PBM::PBM(int x, int y, std::vector<uint32_t>* data)
{
	this->x = x;
	this->y = y;
	this->data = data;
}

NetPBMResult PBM::SaveAscii(std::string filename)
{
	std::ofstream filestream;

	this->logger.Log("Creating PBM Ascii file %s of size x %i, y %i", filename.c_str(), this->x, this->y);
	filestream.open(filename, std::ios::out | std::ios::binary | std::ios::trunc);

	if (!filestream) {
		this->logger.Error("Failed to open file %s", filename.c_str());
		return PBM_FAIL_OPEN;
	}

	filestream << "P1" << std::endl;
	filestream << this->x << " " << this->y << std::endl;

	size_t i = 0;
	iterator it = this->data->begin();
	while (it != this->data->end())
	{
		filestream << std::clamp(*it, 0u, 1u);
		it++;
		i++;
		if (i >= this->x)
		{
			filestream << '\n';
			i = 0;
		}
		else
		{
			filestream << ' ';
		}
	}

	if (filestream.fail()) {
		this->logger.Error("Failed to write too file %s", filename.c_str());
		return PBM_FAIL_WRITE;
	}

	filestream.close();
	this->logger.Log("Created file %s", filename.c_str());
	return PBM_SUCCESS;
}

NetPBMResult PBM::SaveBinary(std::string filename)
{
	std::ofstream filestream;

	this->logger.Log("Creating PBM Binary file %s of size x %i, y %i", filename.c_str(), this->x, this->y);
	filestream.open(filename, std::ios::out | std::ios::binary | std::ios::trunc);

	if (!filestream) {
		this->logger.Error("Failed to open file %s", filename.c_str());
		return PBM_FAIL_OPEN;
	}

	filestream << "P4" << std::endl;
	filestream << this->x << " " << this->y << std::endl;

	uint8_t buffer;
	iterator it = this->data->begin();
	while (it != this->data->end())
	{
		buffer = 0;
		for (int i = 0; i < 8 && it != this->data->end(); i++)
		{
			buffer += (std::clamp(*it, 0u, 1u)) << (7 - i);
			it++;
		}
		filestream << buffer;
	}

	if (filestream.fail()) {
		this->logger.Error("Failed to write too file %s", filename.c_str());
		return PBM_FAIL_WRITE;
	}

	filestream.close();
	this->logger.Log("Created file %s", filename.c_str());
	return PBM_SUCCESS;
}
