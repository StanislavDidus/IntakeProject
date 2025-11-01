#pragma once

#include <random>
#include "template.h"

struct Random
{
	static int randomRange(int min, int max);
	static float randomRange(float min, float max);
	static Tmpl8::vec2 randomVector2(const Tmpl8::vec2& min, const Tmpl8::vec2& max);
	static bool ff(); //fifty-fifty
};