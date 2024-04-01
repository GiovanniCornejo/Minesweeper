#ifndef RANDOM_H
#define RANDOM_H

#include <random>

struct Random
{
public:
    static std::mt19937 &getGenerator();

private:
    // Use Mersenne Twister pseudo-random number generator for random boards
    static std::mt19937 random;
};

#endif // RANDOM_H