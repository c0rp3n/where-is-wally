#include "NetPBM.hpp"

using namespace NetPBM;

PPM::PPM(int x, int y, uint32_t maxValue, std::vector<Pixel>* data)
{
    this->x = x;
    this->y = y;
    this->maxValue = maxValue;
    this->data = data;
}

NetPBMResult PPM::SaveAscii(std::string filename)
{
    std::ofstream filestream;

    this->logger.Log("Creating PPM Ascii file %s of size x %i, y %i", filename.c_str(), this->x, this->y);
    filestream.open(filename, std::ios::out | std::ios::binary | std::ios::trunc);

    if (!filestream) {
        this->logger.Error("Failed to open file %s", filename.c_str());
        return PBM_FAIL_OPEN;
    }

    filestream << "P3" << std::endl;
    filestream << this->x << " " << this->y << std::endl;
    filestream << this->maxValue << std::endl;

    size_t i = 0;
    Pixel buffer;
    iterator it = this->data->begin();
    while (it != this->data->end())
    {
        buffer = *it;
        if (buffer[0] < 100) { filestream << ' '; }
        else if (buffer[0] < 10) { filestream << ' '; }
        filestream << std::clamp(buffer[0], 0u, this->maxValue) << ' ';

        if (buffer[1] < 100) { filestream << ' '; }
        else if (buffer[1] < 10) { filestream << ' '; }
        filestream << std::clamp(buffer[1], 0u, this->maxValue) << ' ';

        if (buffer[2] < 100) { filestream << ' '; }
        else if (buffer[2] < 10) { filestream << ' '; }
        filestream << std::clamp(buffer[2], 0u, this->maxValue);

        it++;
        i++;
        if (i >= x)
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

NetPBMResult PPM::SaveBinary(std::string filename)
{
    std::ofstream filestream;

    this->logger.Log("Creating PPM Binary file %s of size x %i, y %i", filename.c_str(), this->x, this->y);
    filestream.open(filename, std::ios::out | std::ios::binary | std::ios::trunc);

    if (!filestream) {
        this->logger.Error("Failed to open file: %s", filename.c_str());
        return PBM_FAIL_OPEN;
    }

    filestream << "P6" << std::endl;
    filestream << this->x << " " << this->y << std::endl;
    filestream << this->maxValue << std::endl;

    Pixel buffer;
    iterator it = this->data->begin();
    while (it != this->data->end())
    {
        buffer = *it;
        filestream << static_cast<uint8_t>(std::clamp(buffer[0], 0u, this->maxValue));
        filestream << static_cast<uint8_t>(std::clamp(buffer[1], 0u, this->maxValue));
        filestream << static_cast<uint8_t>(std::clamp(buffer[2], 0u, this->maxValue));
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
