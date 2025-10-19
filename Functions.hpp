#pragma once

#include "template.h"

inline float clamp(float value, float min, float max)
{
	if (value < min) return min;
	if (value > max) return max;
	return value;
}

inline int clamp(int value, int min, int max)
{
	if (value < min) return min;
	if (value > max) return max;
	return value;
}

inline Tmpl8::vec2i clampVec2i(Tmpl8::vec2i value, Tmpl8::vec2i min, Tmpl8::vec2i max)
{
	return { clamp(value.x, min.x, max.x), clamp(value.y, min.y, max.y) };
}

inline Tmpl8::vec2 clampVec2(Tmpl8::vec2 value, Tmpl8::vec2 min, Tmpl8::vec2 max)
{
	return Tmpl8::vec2{ clamp(value.x, min.x, max.x), clamp(value.y, min.y, max.y) };
}

inline Tmpl8::vec2 interpolate(Tmpl8::vec2 a, Tmpl8::vec2 b, Tmpl8::vec2 c, Tmpl8::vec3 bc)
{
	return a * bc.x + b * bc.y + c * bc.z;
}