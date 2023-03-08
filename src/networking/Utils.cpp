#include "Utils.h"
#include <random>
int Utils::randomInt(int min, int max)
{
    std::random_device device;
    std::mt19937 generator(device());
    std::uniform_int_distribution<int> distribution(min, max);

    return distribution(generator);
}
int Utils::randomInt(int max)
{
    return randomInt(0, max);
}
unsigned long Utils::randomUL(unsigned long min, unsigned long max)
{
    std::random_device device;
    std::mt19937 generator(device());
    std::uniform_int_distribution<unsigned long> distribution(min, max);

    return distribution(generator);
}