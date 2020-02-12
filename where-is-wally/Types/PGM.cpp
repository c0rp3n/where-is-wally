#include "NetPBM.hpp"

using namespace NetPBM;

PGM::PGM(int x, int y, uint32_t maxValue, std::vector<uint32_t>* data)
{
    this->x = x;
    this->y = y;
    this->maxValue = maxValue;
    this->data = data;
}

NetPBMResult PGM::SaveAscii(std::string filename)
{
    std::ofstream filestream;

    this->logger.Log("Creating PGM Ascii file %s of size x %i, y %i", filename.c_str(), this->x, this->y);
    filestream.open(filename, std::ios::out | std::ios::binary | std::ios::trunc);

    if (!filestream) {
        this->logger.Error("Failed to open file %s", filename.c_str());
        return PBM_FAIL_OPEN;
    }

    filestream << "P2" << std::endl;
    filestream << this->x << " " << this->y << std::endl;
    filestream << this->maxValue << std::endl;

    size_t i = 0;
    iterator it = this->data->begin();
    while (it != this->data->end())
    {
        uint32_t buffer = *it;
        if (buffer < 100) { filestream << ' '; }
        else if (buffer < 10) { filestream << ' '; }
        filestream << std::clamp(buffer, 0u, this->maxValue);
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

NetPBMResult PGM::SaveBinary(std::string filename)
{
    std::ofstream filestream;

    this->logger.Log("Creating PGM Binary file %s of size x %i, y %i", filename.c_str(), this->x, this->y);
    filestream.open(filename, std::ios::out | std::ios::binary | std::ios::trunc);

    if (!filestream) {
        this->logger.Error("Failed to open file %s", filename.c_str());
        return PBM_FAIL_OPEN;
    }

    filestream << "P5" << std::endl;
    filestream << this->x << " " << this->y << std::endl;
    filestream << this->maxValue << std::endl;

    iterator it = this->data->begin();
    while (it != this->data->end())
    {
        filestream << static_cast<uint8_t>(std::clamp(*it, 0u, this->maxValue));
        it++;
    }

    if (filestream.fail()) {
        this->logger.Error("Failed to write too file %s", filename.c_str());
        return PBM_FAIL_WRITE;
    }

    filestream.close();
    this->logger.Log("Created file %s", filename.c_str());
    return PBM_SUCCESS;
}
