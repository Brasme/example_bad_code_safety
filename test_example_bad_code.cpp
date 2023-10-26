#include "gtest/gtest.h"
#include "stopwatch.h"
#include "testData.h"

int average_0(const int* values, int num)
{
    int sum = 0;
    for (int i = 0; i < num; ++i)
        sum += values[i];
    return (int)round((double)sum / (double)num);
}

int average_1(const int* values, int num)
{
    int sum = 0;
    for (int i = 0; i < num; ++i)
        sum += values[i];
    return sum / num;
}

int average_2(const int* values, int num)
{
    double sum = 0;
    for (int i = 0; i < num; ++i)
        sum += (double)values[i];
    return (int)(sum / (double)num);
}

int average_3(const int* values, int num)
{
    double avg = 0;
    const double scale = 1.0 / num;
    for (int i = 0; i < num; ++i)
        avg += values[i] * scale;
    return (int)avg;
}

TEST(quality, average_0_num_before_overflow)
{
    size_t num = 1000;
    bool ok=true;
    int avg1 = 0;
    int avg2 = 0;
    double avg = 0;
    while (avg1==avg2) {
        num++;
        HeapArray<int> data(num,1000,999);
        avg1 = average_0(data.values(), data.num());
        avg2 = data.average();        
        avg = data.averageDouble();
    }

    EXPECT_EQ(num, 2073);
    EXPECT_EQ(avg1, -1035897);
    EXPECT_EQ(avg2 , 1035964);
    EXPECT_EQ(avg, 1035964.0);
}
TEST(quality, average_1)
{
    Array<int,2000> data(1000, 999);
    
    int result = average_1(data.values(), data.num());

    EXPECT_EQ(result, (int)data.averageDouble());
}

TEST(quality, average_2)
{
    constexpr int N = 1024;
    int values[N];
    for (int i = 0; i < N; ++i)
        values[i] = 1000 + 999 * i;
    int result = average_2(values, N);

    EXPECT_EQ(result, 511988);
}

TEST(quality, average_3)
{
    constexpr int N = 1024;
    int values[N];
    for (int i = 0; i < N; ++i)
        values[i] = 1000 + 999 * i;
    int result = average_3(values, N);

    EXPECT_EQ(result, 511988);
}
TEST(performance, average_1) 
{
    Stopwatch stopwatch;
  
    EXPECT_EQ(1, 1);
    EXPECT_TRUE(true);
}