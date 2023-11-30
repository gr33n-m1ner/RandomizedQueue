#pragma once

#include <algorithm>
#include <iostream>
#include <random>
#include <vector>

template <class T>
class randomized_queue
{
    template <bool is_const>
    class Iterator
    {
    public:
        using difference_type = std::ptrdiff_t;
        using value_type = std::conditional_t<is_const, const T, T>;
        using pointer = value_type *;
        using reference = value_type &;
        using iterator_category = std::forward_iterator_tag;

        Iterator() {}

    private:
        friend class randomized_queue;
        using origin_type = std::conditional_t<is_const, const std::vector<T> *, std::vector<T> *>;

        origin_type origin;
        std::vector<size_t> permutation;
        std::size_t pos;

        Iterator(origin_type orig, std::size_t size, bool flag, std::mt19937 & rand_engine)
            : origin(orig)
            , permutation(std::vector<std::size_t>(size))
            , pos(flag ? size : 0)
        {
            if (size == 0) {
                return;
            }
            for (std::size_t i = 0; i < size; ++i) {
                permutation[i] = i;
            }

            for (std::size_t i = size - 1; i > 0; --i) {
                std::uniform_int_distribution<std::size_t> distribution(static_cast<std::size_t>(0), i - 1);
                std::size_t ind = distribution(rand_engine);
                std::swap(permutation[ind], permutation[i]);
            }
        }

    public:
        friend bool operator==(const Iterator & left, const Iterator & right)
        {
            return left.pos == right.pos && left.origin == right.origin;
        }

        friend bool operator!=(const Iterator & left, const Iterator & right)
        {
            return !(left == right);
        }

        reference operator*() const
        {
            return (*origin)[permutation[pos]];
        }

        pointer operator->() const
        {
            return &((*origin)[permutation[pos]]);
        }

        Iterator & operator++()
        {
            ++pos;
            return *this;
        }

        Iterator operator++(int)
        {
            auto tmp = *this;
            operator++();
            return tmp;
        }
    };

public:
    using iterator = Iterator<false>;
    using const_iterator = Iterator<true>;

    randomized_queue()
        : m_rand_engine(std::random_device{}())
    {
    }

    bool empty() const { return m_data.empty(); }
    std::size_t size() const { return m_data.size(); }

    void enqueue(T && element)
    {
        m_data.emplace_back(std::move(element));
    }
    void enqueue(const T & element)
    {
        m_data.emplace_back(element);
    }

    T dequeue()
    {
        const size_t index = get_rand_index();
        std::swap(m_data[index], m_data[m_data.size() - 1]);
        T answer = std::move(m_data[m_data.size() - 1]);
        m_data.pop_back();
        return answer;
    }
    const T & sample() const
    {
        return m_data[get_rand_index()];
    }

    iterator begin()
    {
        return Iterator<false>(&m_data, m_data.size(), false, m_rand_engine);
    }
    iterator end()
    {
        return Iterator<false>(&m_data, m_data.size(), true, m_rand_engine);
    }
    const_iterator cbegin() const { return Iterator<true>(&m_data, m_data.size(), false, m_rand_engine); }
    const_iterator cend() const { return Iterator<true>(&m_data, m_data.size(), true, m_rand_engine); }
    const_iterator begin() const { return cbegin(); }
    const_iterator end() const { return cend(); }

private:
    mutable std::mt19937 m_rand_engine;
    std::vector<T> m_data;

    std::size_t get_rand_index() const
    {
        std::uniform_int_distribution<std::size_t> distribution(static_cast<std::size_t>(0), m_data.size() - 1);
        return distribution(m_rand_engine);
    }
};