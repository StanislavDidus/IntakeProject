#include "Random.hpp"

namespace
{
    std::random_device rd;
    std::minstd_rand rng(rd());
}

int Random::randomRange(int min, int max)
{
    std::uniform_int_distribution dist(min, max);
    return dist(rng);
}

float Random::randomRange(float min, float max)
{
    std::uniform_real_distribution dist(min, max);
    return dist(rng);
}

Tmpl8::vec2 Random::randomVector2(const Tmpl8::vec2& min, const Tmpl8::vec2& max)
{
    return Tmpl8::vec2{ randomRange(min.x, max.x), randomRange(min.y, max.y) };
}

bool Random::ff()
{
    std::bernoulli_distribution dist(0.5);
    return dist(rng);
}
