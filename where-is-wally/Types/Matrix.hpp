#pragma once

#include <cmath>
#include <vector>

template <typename T>
struct Matrix
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

    data_type data;
    int x, y;

    Matrix()
    {
        this->x = 0;
        this->y = 0;
    }

    Matrix(int x, int y)
    {
        this->x = x;
        this->y = y;

        this->data.reserve(x * y);
    }

    Matrix(int x, int y, std::initializer_list<T> data)
    {
        this->x = x;
        this->y = y;

        this->data.reserve(x * y);
        typename std::initializer_list<T>::iterator it = data.begin();
        while (it != data.end())
        {
            this->data.push_back(*it);
            it++;
        }
    }

    Matrix(int x, int y, std::vector<T> data)
    {
        this->x = x;
        this->y = y;

        this->data.reserve(y);
        iterator it = data.begin();
        while (it != data.end())
        {
            this->data.push_back(*it);
            it++;
        }
    }

    Matrix(const Matrix &matrix)
    {
        this->x = matrix.x;
        this->y = matrix.y;
        this->data.reserve(this->y);
        const_iterator it = matrix.data.begin();
        while (it != matrix.data.end())
        {
            this->data.push_back(*it);
            it++;
        }
    }

    virtual ~Matrix()
    {
        this->data.clear();
    }

    T Get(int x, int y)
    {
        return this->data[(y * this->x) + x];
    }

    void Set(int x, int y, T value)
    {
        this->data[(y * this->x) + x] = value;
    }

    void Pow(int power)
    {
        for (int y = 0; y < this->y; y++)
        {
            for (int x = 0; x < this->x; x++)
            {
                this->data[(y * this->x) + x] = (T)std::pow(this->data[(y * this->x) + x], power);
            }
        }
    }

    T Sum()
    {
        T sum = 0;
        for (int y = 0; y < this->y; y++)
        {
            for (int x = 0; x < this->x; x++)
            {
                sum += this->data[(y * this->x) + x];
            }
        }

        return sum;
    }

    double Mean()
    {
        return this->Sum() / (this->x * this->y);
    }

    Matrix<T> operator+(const Matrix<T> &mat)
    {
        if (this->x != mat.x || this->y != mat.y)
        {
            return Matrix();
        }

        Matrix<T> matrix = Matrix(this->x, this->y);
        for (int i = 0; i < this->x * this->y; i++)
        {
            matrix.data.push_back(this->data[i] - mat[i]);
        }

        return matrix;
    }

    Matrix<T> operator+(const double &value)
    {
        Matrix<T> matrix = Matrix(this->x, this->y);
        for (int i = 0; i < this->x * this->y; i++)
        {
            matrix.data.push_back(this->data[i] - value);
        }

        return matrix;
    }

    Matrix<T> operator-(const Matrix<T> &mat)
    {
        if (this->x != mat.x || this->y != mat.y)
        {
            return Matrix();
        }

        Matrix<T> matrix = Matrix(this->x, this->y);
        for (int i = 0; i < this->x * this->y; i++)
        {
            matrix.data.push_back(this->data[i] - mat[i]);
        }

        return matrix;
    }

    Matrix<T> operator-(const double &value)
    {
        Matrix<T> matrix = Matrix(this->x, this->y);
        for (int i = 0; i < this->x * this->y; i++)
        {
            matrix.data.push_back(this->data[i] - value);
        }

        return matrix;
    }

    Matrix<T> operator*(const Matrix<T> &mat)
    {
        Matrix<T> matrix = Matrix(this->x, this->y);
        if (this->x != mat.x || this->y != mat.y)
            return matrix;

        for (int i = 0; i < this->x * this->y; i++)
        {
            matrix.data.push_back(this->data[i] * mat.data[i]);
        }

        return matrix;
    }

    std::vector<T> operator[](int i)
    {
        return this->data[i];
    }
};
