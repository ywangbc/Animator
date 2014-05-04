#include "beziercurveevaluator.h"
#include <assert.h>

Point BezierCurveEvaluator::getAvg(Point p1, Point p2) const
{
	return Point((p1.x + p2.x) / 2, (p1.y + p2.y) / 2);
}



void BezierCurveEvaluator::displayBezier(const Point v0, const Point v1, const Point v2, const Point v3, std::vector<Point>& EvaluatedPoints) const
{
	if (flatEnough(v0, v1, v2, v3))
	{
		EvaluatedPoints.push_back(v0);
		EvaluatedPoints.push_back(v3);
	}
	else
	{
		Point v0New = getAvg(v0, v1);
		Point v1New = getAvg(v1, v2);
		Point v2New = getAvg(v2, v3);
		Point v0NewNew = getAvg(v0New, v1New);
		Point v1NewNew = getAvg(v1New, v2New);
		Point Qu = getAvg(v0NewNew,v1NewNew);

		displayBezier(v0, v0New, v0NewNew, Qu, EvaluatedPoints);
		displayBezier(Qu, v1NewNew, v2New, v3, EvaluatedPoints);
	}
}
bool BezierCurveEvaluator::flatEnough(const Point v0, const Point v1, const Point v2, const Point v3) const
{
	float dist1 = v0.distance(v1);
	float dist2 = v1.distance(v2);
	float dist3 = v2.distance(v3);
	float dist4 = v0.distance(v3);

	if (((dist1 + dist2 + dist3) / dist4)<(1 + FLATEPSILON))
	{
		return true;
	}
	else
	{
		return false;
	}
}

void BezierCurveEvaluator::evaluateCurve(const std::vector<Point>& ptvCtrlPts,
	std::vector<Point>& ptvEvaluatedCurvePts,
	const float& fAniLength,
	const bool& bWrap) const
{
	//First we clear the memory of previous render
	ptvEvaluatedCurvePts.clear();



	int iCtrlPtCount = ptvCtrlPts.size();

	Point v0;
	Point v1; 
	Point v2;
	Point v3;

	float x = 0.0;
	float y1;
	//Dvide the points into groups of four

	//ptvEvaluatedCurvePts.push_back(ptvCtrlPts[0]);

	for (int groupNum = 0; groupNum < iCtrlPtCount; groupNum += STEPLENGTH)
	{
		int numPointLeft = iCtrlPtCount - groupNum;
		if (numPointLeft >= 4)
		{
			v0 = ptvCtrlPts[groupNum];
			v1 = ptvCtrlPts[groupNum + 1];
			v2 = ptvCtrlPts[groupNum + 2];
			v3 = ptvCtrlPts[groupNum + 3];
			displayBezier(v0, v1, v2, v3, ptvEvaluatedCurvePts);
		}
		else
		{
			for (int i = 0; i < numPointLeft; i++)
			{
				ptvEvaluatedCurvePts.push_back(ptvCtrlPts[groupNum + i]);
			}
		}
		
		
		y1 = ptvCtrlPts[0].y;

		ptvEvaluatedCurvePts.push_back(Point(x, y1));

		x = fAniLength;

		float y2;
		y2 = y1;
		ptvEvaluatedCurvePts.push_back(Point(x, y2));
	}

	/*
	float x = 0.0;
	float y1;

	if (bWrap) {
		// if wrapping is on, interpolate the y value at xmin and
		// xmax so that the slopes of the lines adjacent to the
		// wraparound are equal.

		if ((ptvCtrlPts[0].x + fAniLength) - ptvCtrlPts[iCtrlPtCount - 1].x > 0.0f) {
			y1 = (ptvCtrlPts[0].y * (fAniLength - ptvCtrlPts[iCtrlPtCount - 1].x) +
				ptvCtrlPts[iCtrlPtCount - 1].y * ptvCtrlPts[0].x) /
				(ptvCtrlPts[0].x + fAniLength - ptvCtrlPts[iCtrlPtCount - 1].x);
		}
		else
			y1 = ptvCtrlPts[0].y;
	}
	else {
		// if wrapping is off, make the first and last segments of
		// the curve horizontal.

		y1 = ptvCtrlPts[0].y;
	}

	ptvEvaluatedCurvePts.push_back(Point(x, y1));

	/// set the endpoint based on the wrap flag.
	float y2;
	x = fAniLength;
	if (bWrap)
		y2 = y1;
	else
		y2 = ptvCtrlPts[iCtrlPtCount - 1].y;

	ptvEvaluatedCurvePts.push_back(Point(x, y2));
	*/
}
