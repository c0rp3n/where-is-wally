#pragma once

#include <algorithm>
#include <cstdint>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

#include "../Utils/Utils.hpp"
#include "Pixel.hpp"

enum NetPBMFormatType : uint8_t
{
	PBM_BITMAP = 1 << 0,
	PBM_GRAYMAP = 1 << 1,
	PBM_PIXMAP = 1 << 2,
};

enum NetPBMDataType : uint8_t
{
	PBM_ASCII = 1 << 3,
	PBM_BINARY = 1 << 4,
};

enum NetPBMFormats : uint8_t
{
	PBM_P1 = PBM_BITMAP | PBM_ASCII,
	PBM_P2 = PBM_GRAYMAP | PBM_ASCII,
	PBM_P3 = PBM_PIXMAP | PBM_ASCII,
	PBM_P4 = PBM_BITMAP | PBM_BINARY,
	PBM_P5 = PBM_GRAYMAP | PBM_BINARY,
	PBM_P6 = PBM_PIXMAP | PBM_BINARY,
};

enum NetPBMResult : int32_t
{
	PBM_SUCCESS = 1,
	PBM_FAIL_OPEN = -1,
	PBM_FAIL_WRITE = -2,
};

namespace NetPBM
{
	template <typename T>
	class PBMBase
	{
	public:
		using data_type = std::vector<T>;
		using value_type = typename std::vector<T>::value_type;
		using size_type = typename std::vector<T>::size_type;

		using reference = typename std::vector<T>::reference;
		using const_reference = typename std::vector<T>::const_reference;

		using iterator = typename std::vector<T>::iterator;
		using const_iterator = typename std::vector<T>::const_iterator;

		using reverse_iterator = typename std::vector<T>::reverse_iterator;
		using const_reverse_iterator = typename std::vector<T>::const_reverse_iterator;

		Utils::MLogger logger = Utils::MLogger::MLogger("NetPBM");
		int x, y;
		data_type* data;

		NetPBMResult Save(std::string filename, NetPBMDataType dataSerialization = PBM_BINARY)
		{
			return dataSerialization == PBM_BINARY ? this->SaveBinary(filename) : this->SaveAscii(filename);
		}

	private:
		virtual NetPBMResult SaveAscii(std::string filename)
		{
			return PBM_SUCCESS;
		}

		virtual NetPBMResult SaveBinary(std::string filename)
		{
			return PBM_SUCCESS;
		}

	public:
		T Get(int x, int y)
		{
			return this->data[(y * this->x) + x];
		}

		void Set(int x, int y, T value)
		{
			this->data[(y * this->x) + x] = value;
		}

		std::vector<T> operator[](int index)
		{
			return this->data[index];
		}
	};

	class PBM : public PBMBase<uint32_t>
	{
	public:
		PBM(int x, int y, std::vector<uint32_t>* data);

	private:
		NetPBMResult SaveAscii(std::string filename) override;
		NetPBMResult SaveBinary(std::string filename) override;
	};

	class PGM : public PBMBase<uint32_t>
	{
	public:
		uint32_t maxValue;

		PGM(int x, int y, uint32_t maxValue, std::vector<uint32_t>* data);

	private:
		NetPBMResult SaveAscii(std::string filename) override;
		NetPBMResult SaveBinary(std::string filename) override;
	};

	class PPM : public PBMBase<Pixel>
	{
	public:
		uint32_t maxValue;

		PPM(int x, int y, uint32_t maxValue, std::vector<Pixel>* data);

	private:
		NetPBMResult SaveAscii(std::string filename) override;
		NetPBMResult SaveBinary(std::string filename) override;
	};
};
