#pragma once

#include <cstdlib>

template <typename T>
struct IData {
    using dataType = T;
    virtual ~IData() = default;
    IData& fill(const dataType& startValue, const dataType& increment) {
        dataType value = startValue;
        dataType* v = values();
        const size_t n = num();
        for (size_t i = 0; i < n; ++i) {
            v[i] = value;
            value += increment;
        }
        return *this;
    }
    virtual dataType* values() = 0;
    virtual const dataType* values() const = 0;
    virtual size_t num() const = 0;
    dataType average() const {
        return (dataType)round(averageDouble());
    }
    double averageDouble() const {
        const dataType* v = values();
        double sum = 0.0;
        const size_t n = num();
        for (size_t i = 0; i < n; ++i)
            sum += (double)v[i];
        return sum / (double)n;
    }
};

template <typename T, size_t N>
struct Array final : IData<T> {
    Array() { fill(dataType(1), dataType(1)); }
    Array(const dataType& startValue, const dataType& increment) { fill(startValue, increment); }
    dataType* values() override { return values_; }
    const dataType* values() const override { return values_; }
    size_t num() const override { return N; }
private:
    dataType values_[N];
};

template <typename T>
struct HeapArray final : IData<T> {
    HeapArray(size_t num) : values_(nullptr), num_(num)
    {
        values_ = (dataType*)malloc(num * sizeof(dataType));
        fill(dataType(1), dataType(1));
    }
    HeapArray(size_t num, const dataType& startValue, const dataType& increment) : values_(nullptr), num_(num)
    {
        values_ = (dataType*)malloc(num * sizeof(dataType));
        fill(startValue, increment);
    }
    ~HeapArray() override { free(values_); }
    dataType* values() override { return values_; }
    const dataType* values() const override { return values_; }
    size_t num() const override { return num_; }
private:
    dataType* values_;
    size_t num_;
};

