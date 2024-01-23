#include "hull-bruteforce.h"
#include <algorithm>
#include <iostream>

bool Point::operator==( Point const& arg2 ) const {
    return ( (x==arg2.x) && (y==arg2.y) );
}

std::ostream& operator<< (std::ostream& os, Point const& p) {
	os << "(" << p.x << " , " << p.y << ") ";
	return os;
}

std::istream& operator>> (std::istream& os, Point & p) {
	os >> p.x >> p.y;
	return os;
}

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
//return value is (on left, on right)
std::pair<bool,bool> get_location(
		float const& p1x, //check which side of the line (p1x,p1y)-->(p2x,p2y) 
		float const& p1y, //point (qx,qy) is on
		float const& p2x,
		float const& p2y,
		float const& qx,
		float const& qy
		) 
{
	Point dir   = {p2x-p1x,p2y-p1y};
	Point norm  = {dir.y, -dir.x};
	Point point = {qx-p1x,qy-p1y};
	//scalar product is positive if on right side
	float scal_prod = norm.x*point.x + norm.y*point.y;
	return std::make_pair( (scal_prod<0), (scal_prod>0));
}

//returns a set of indices of points that form convex hull
std::set<int> hullBruteForce ( std::vector< Point > const& points ) {
	int num_points = points.size();
	//std::cout << "number of points " << num_points << std::endl;
	if ( num_points < 3 ) throw "bad number of points";

	std::set<int> hull_indices;
	for(int i = 0;i < num_points-1;++i)
	{
		for(int j = i+1; j < num_points;++j)
		{
			// check all other points to see if they are on one side of the line
			bool same_side = true;
			std::pair<bool,bool> side;
			// find what side a point is on to the line
			for(int n = 0;n < 3;++n)
			{
				if(i == n || j == n) continue;
				side = get_location(points[i].x,points[i].y,points[j].x,points[j].y,points[n].x,points[n].y);
				break;
			}
			// check other points to see if they are on right side of the line
			for(int k = 0; k < num_points && same_side;++k)
			{
				if(k == j || k == i) continue;

				if(side != get_location(points[i].x,points[i].y,points[j].x,points[j].y,points[k].x,points[k].y))
				{
					same_side = false;
				}
			}
			// add to set if they are all on the same side
			if(same_side)
			{
				hull_indices.insert(i);
				hull_indices.insert(j);
			}
		}		
	}
		
	return hull_indices;
}

std::vector<int> hullBruteForce2 ( std::vector< Point > const& points ) {
	int num_points = points.size();
	if ( num_points < 3 ) throw "bad number of points";

	std::vector<int> hull_indices;
	
	int min_x = points[0].x;
	int start = 0;
	// find point with smallest x
	for(int n = 1;n < num_points;++n)
	{
		// find smallest x
		if(points[n].x < min_x)
		{
			start = n;
			min_x = points[n].x;
		}
	}
	hull_indices.push_back(start);
	for(int i = 0;i < num_points;++i)
	{
		for(int j = 0;j < num_points;++j)
		{
			// finds dupes
			if(j == hull_indices.back()) continue;

			bool same_side = true;
			Point curr = points[hull_indices.back()];
			// check all other points
			for(int k = 0;k < num_points;++k)
			{
				if(j == k) continue;
				// check if on left
				if(get_location(curr.x,curr.y,points[j].x,points[j].y,points[k].x,points[k].y).second == true)
				{
					same_side = false;
					break;
				}
			}
			if(same_side)
			{
				// if loops back to front, return
				if(j == hull_indices.front()) return hull_indices;
				hull_indices.push_back(j);
			}
		}
	}


	return hull_indices;
}
