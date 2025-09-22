#pragma once

#include "Object.h"

struct CollisionHelper
{
	static bool AABB(Object* target, Object* col)
	{
		auto& pos1 = target->getPosition();
		auto& size1 = target->getSize();

		auto& pos2 = col->getPosition();
		auto& size2 = col->getSize();

		if (!(
			pos1.x + size1.x <= pos2.x ||
			pos2.x + size2.x <= pos1.x ||
			pos1.y + size1.y <= pos2.y ||
			pos2.y + size2.y <= pos1.y
			))
			return true;
		return false;
	}
	/*
	Inspiration: https://dyn4j.org/2010/01/sat/#sat-axes
	*/
	static bool SAT(Object* target, Object* col) //Separating Axis Theorem
	{
		//Get separated axes for each collider
		std::vector<Tmpl8::vec2> axes;
		axes.reserve(2);

		std::vector<Tmpl8::vec2> objAxes = target->getAxes();
		std::vector<Tmpl8::vec2> colAxes = col->getAxes();

		axes.insert(axes.end(), objAxes.begin(), objAxes.end());
		axes.insert(axes.end(), colAxes.begin(), colAxes.end());

		//Project vertices to all axess
		for (const auto& axis : axes)
		{
			//Go through each of the verticies
			std::vector<Tmpl8::vec2> objVerticies = target->getVerticies();
			std::vector<Tmpl8::vec2> colVerticies = col->getVerticies();

			float min = objVerticies[0].dot(axis), max = min;
			for (int i = 1; i < 4; i++)
			{
				float proj = objVerticies[i].dot(axis);
				if (proj < min) min = proj;
				if (proj > max) max = proj;
			}

			float min1 = colVerticies[0].dot(axis), max1 = min1;
			for (int i = 1; i < 4; i++)
			{
				float proj = colVerticies[i].dot(axis);
				if (proj < min1) min1 = proj;
				if (proj > max1) max1 = proj;
			}

			//Check instersection
			if (max < min1 || max1 < min)
			{
				return false;
			}
		}

		//Return result
		return true;
	}

};