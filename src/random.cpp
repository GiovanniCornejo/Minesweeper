#include <ctime>

#include "random.h"

std::mt19937 Random::random(std::time(nullptr));

std::mt19937 &Random::getGenerator() { return random; }