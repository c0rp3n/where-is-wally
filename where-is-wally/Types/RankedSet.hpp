#pragma once

#include <deque>
#include <utility>

enum RSRankingType : uint8_t
{
    RS_LARGEST_VALUE = 1 << 0,
    RS_SMALLEST_VALUE = 1 << 1,
};

template <typename Rank, typename Data>
struct RankedSet
{
public:
    using data_type = std::deque<std::pair<Rank, Data>>;
    using value_type = typename std::deque<std::pair<Rank, Data>>::value_type;
    using size_type = typename std::deque<std::pair<Rank, Data>>::size_type;

    using reference = typename std::deque<std::pair<Rank, Data>>::reference;
    using const_reference = typename std::deque<std::pair<Rank, Data>>::const_reference;

    using iterator = typename std::deque<std::pair<Rank, Data>>::iterator;
    using const_iterator = typename std::deque<std::pair<Rank, Data>>::const_iterator;

    using reverse_iterator = typename std::deque<std::pair<Rank, Data>>::reverse_iterator;
    using const_reverse_iterator = typename std::deque<std::pair<Rank, Data>>::const_reverse_iterator;

    data_type data;
    size_t maxSize;
    RSRankingType rankingType;

    RankedSet()
    {
        this->maxSize = 0;
    }

    RankedSet(size_t size, RSRankingType rankingType = RS_SMALLEST_VALUE)
    {
        this->maxSize = size;
        this->rankingType = rankingType;
    }

    void SetRankingType(RSRankingType rankingType)
    {
        this->rankingType = rankingType;
    }

    void Push(value_type value)
    {
        if (this->data.size() == 0)
        {
            this->data.push_front(value);
        }
        else if (this->rankingType == RS_LARGEST_VALUE)
        {
            if (value.first > this->data.back().first)
            {
                iterator it = this->data.begin();
                while (it != this->data.end())
                {
                    if (value.first > (*it).first)
                    {
                        this->data.insert(it, value);
                        if (this->data.size() > this->maxSize)
                        {
                            this->data.pop_back();
                        }
                        return;
                    }
                    it++;
                }
            }
            else if (this->data.size() < this->maxSize)
            {
                this->data.push_back(value);
            }
        }
        else if (this->rankingType == RS_SMALLEST_VALUE)
        {
            if (value.first < this->data.back().first)
            {
                iterator it = this->data.begin();
                while (it != this->data.end())
                {
                    if (value.first < (*it).first)
                    {
                        this->data.insert(it, value);
                        if (this->data.size() > this->maxSize)
                        {
                            this->data.pop_back();
                        }
                        return;
                    }
                    it++;
                }
            }
            else if (this->data.size() < this->maxSize)
            {
                this->data.push_back(value);
            }
        }
    }

    void Pop()
    {
        if (!this->data.empty())
        {
            this->data.pop_back();
        }
    }

    value_type Get(int index)
    {
        return this->data[index];
    }

    value_type operator[](int index)
    {
        return this->data[index];
    }
};
