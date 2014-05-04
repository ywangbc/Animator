#ifndef INCLUDED_BEZIER_CURVE_EVALUATOR_H
#define INCLUDED_BEZIER_CURVE_EVALUATOR_H

#pragma warning(disable : 4786)  

const int STEPLENGTH = 3;
const float FLATEPSILON = 0.00001;

#include "CurveEvaluator.h"

//using namespace std;

class BezierCurveEvaluator : public CurveEvaluator
{
public:
	void evaluateCurve(const std::vector<Point>& ptvCtrlPts,
		std::vector<Point>& ptvEvaluatedCurvePts,
		const float& fAniLength,
		const bool& bWrap) const;
	void displayBezier(const Point v0, const Point v1, const Point v2, const Point v3, std::vector<Point>& EvaluatedPoints) const;
	bool flatEnough(const Point v0, const Point v1, const Point v2, const Point v3) const;

	//To ge the middle point of two points
	Point getAvg(Point p1, Point p2) const;
};

#endif