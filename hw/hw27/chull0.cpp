#include "svl/svl.h"
#include <vector>
#include <iostream>
#include <algorithm>
using namespace std;

#pragma comment (lib, "svl-dbg.lib")

extern void PrintVector (const vector<Vec2>&);


// find and remove extreme (minimum Y) points 
static Vec2 extreme;
Vec2 RemoveExtreme (vector<Vec2>& points)
{
	int min = 0,miny = points[0][1];
	Vec2 ex ;

	for(int i = 1; i < points.size(); i++){
		if (points[min][1] > points[i][1]){
			min = i;
			miny = points[i][1];
		}
	}
	// extreme: lowest y
	//return extreme;

	ex = points[min];
	points.erase(points.begin()+min);


	return ex;

}

// find the angle [0,2pi) of (p - extreme)
static double angle (const Vec2& p)
{
	double a = atan2(p[1]-extreme[1],p[0]-extreme[0]);

	if(a < 0){
		return a+6.28;
	}else
		return a;

}

// sort in ascending order
static bool anglecompare (const Vec2& p, const Vec2& q)
{
	return angle(p) < angle(q);
}









// return true if (p0-p1-p2) make a left turn; false otherwise
static bool left_turn (const Vec2& p0, const Vec2& p1, const Vec2& p2)
{
	Vec3 r1,r2;
	r1 = Vec3(p1,0.0) - Vec3(p0,0.0);
	r2 = Vec3(p2,0.0) - Vec3(p1,0.0);
	return dot(cross(r1,r2),Vec3(0,0,1)) > 0;

}

static void make_all_left_turn (vector <Vec2>& chs)
{
	// check the last point
	while(1) {
		vector<Vec2>::iterator it = chs.end()-1;
		vector<Vec2>::iterator pit = it-1;
		vector<Vec2>::iterator ppit = it-2;
		if (left_turn (*ppit, *pit, *it))
			break;
		else
			chs.erase (pit);
	} 

}


// main routine:
// compute and return the convex hull of the input "points"
// return a CCW-ordered list of Vec2's
//
vector<Vec2> CHull (const vector<Vec2>& original_points)
{
	// make a copy
	vector<Vec2> points = original_points;
	
	// compute extreme, remove it from points
	extreme = RemoveExtreme (points);
	
	sort (points.begin(), points.end(), anglecompare);
	
	vector <Vec2> chs;
	chs.push_back (extreme);
	chs.push_back (points[0]), chs.push_back (points[1]);
	
	for (int i = 2; i < points.size(); i++) {
		chs.push_back (points[i]);
		make_all_left_turn (chs);
	}
 
	return chs;
}
