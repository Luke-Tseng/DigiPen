#include "closestpair.h"
#include <algorithm>
#include <limits>
#include <cmath>
#include <iostream>
#include <utility>

std::ostream &operator<<(std::ostream &os, Point const &p)
{
	os << "(" << p.x << " , " << p.y << ") ";
	return os;
}

std::istream &operator>>(std::istream &os, Point &p)
{
	os >> p.x >> p.y;
	return os;
}

float distance(Point a, Point b) {
    float dx = a.x - b.x;
    float dy = a.y - b.y;
    return std::sqrt(dx * dx + dy * dy);
}

////////////////////////////////////////////////////////////////////////////////
float closestPair_aux(std::vector<Point> const &points, int low, int high);

////////////////////////////////////////////////////////////////////////////////
float closestPair(std::vector<Point> const &points)
{
	int size = points.size();

	if (size == 0)
		throw "zero size subset";
	if (size == 1)
		return std::numeric_limits<float>::max();

	std::vector<Point> sortedPoints = points;
	std::sort(sortedPoints.begin(), sortedPoints.end(), [](const Point &a, const Point &b)
			  { return a.x < b.x; });

	return closestPair_aux(sortedPoints, 0, size - 1);
}

////////////////////////////////////////////////////////////////////////////////
float closestPair_aux(std::vector<Point> const &points, int low, int high)
{
	// base case
	if (high - low <= 2)
	{
		float minDist = std::numeric_limits<float>::max();
		for (int i = low; i <= high; i++)
		{
			for (int j = i + 1; j <= high; j++)
			{
				minDist = std::min(minDist, distance(points[i], points[j]));
			}
		}
		return minDist;
	}

	int mid = (low + high) / 2;

	float leftDist = closestPair_aux(points, low, mid);
	float rightDist = closestPair_aux(points, mid + 1, high);
	float minDist = std::min(leftDist, rightDist);

	std::vector<Point> strip;
	for (int i = low; i <= high; i++)
	{
		if (std::abs(points[i].x - points[mid].x) < minDist)
		{
			strip.push_back(points[i]);
		}
	}

	// sort strip by y
	std::sort(strip.begin(), strip.end(), [](const Point &a, const Point &b)
			  { return a.y < b.y; });

	// find min dist in pairs
	for (int i = 0; i < static_cast<int>(strip.size()); i++)
	{
		for (int j = i + 1; j < static_cast<int>(strip.size()) && (strip[j].y - strip[i].y) < minDist; j++)
		{
			minDist = std::min(minDist, distance(strip[i], strip[j]));
		}
	}

	return minDist;
}
