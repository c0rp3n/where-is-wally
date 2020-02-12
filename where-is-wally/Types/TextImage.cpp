#include "TextImage.hpp"

TextImage::TextImage()
{

}

TextImage::TextImage(std::string filepath)
{
    this->logger.Log("Loading image %s", filepath.c_str());
    this->GetTextImageSize(filepath);
    this->ReadTextImage(filepath);
}

TextImage::~TextImage()
{
    this->data.clear();
}

void TextImage::GetTextImageSize(std::string filepath)
{
    this->x = 0;
    this->y = 0;
    std::ifstream myfile(filepath);
    if (myfile.is_open())
    {
        // Loop through all of the chars in the file
        char buffer;
        char lastChar = ' ';
        while (!myfile.eof())
        {
            // Count the first occurence of an alpha numeric charactor as x until encountering an end line '\n' then only count each end line for the y.
            myfile.get(buffer);
            if (!this->y && isalnum(buffer) && lastChar == ' ')
            {
                this->x++;
            }
            if (buffer == '\n' && buffer != lastChar)
            {
                this->y++;
            }

            lastChar = buffer;
        }

        myfile.close();

        this->logger.Log("Parsed file of size x %i, y %i", this->x, this->y);
    }
    else { this->logger.Error("Unable to open file %s", filepath.c_str()); }
}

void TextImage::ReadTextImage(std::string filepath)
{
    // Calculate the required size needed for the vector then reserve that amount of memory.
    int size = this->x * this->y;
    this->data.reserve(size);

    // Loop through all of the values in the text file and push them back into the vector.
    int i = 0;
    std::ifstream myfile(filepath);
    if (myfile.is_open())
    {
        double buffer;
        while (myfile.good())
        {
            // Push each value into the buffer of type double then convert it too a 
            // unsigned 32bit integer as to care for overflows later on.
            if (i >= size) break;
            myfile >> buffer;
            this->data.push_back((uint32_t)buffer);
            i++;
        }
        myfile.close();

        this->logger.Log("Loaded file %s", filepath.c_str());
    }
    else { this->logger.Error("Unable to open file %s", filepath.c_str()); }
}
