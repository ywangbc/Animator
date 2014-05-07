#include "bsplinecurveevaluator.h"
#include "beziercurveevaluator.h"
#include "mat.h"
#include "vec.h"
#include <assert.h>


Point getAvg(Point p1, Point p2)
{
	return Point((p1.x + p2.x) / 2, (p1.y + p2.y) / 2);
}



bool flatEnough(const Point v0, const Point v1, const Point v2, const Point v3)
{
	float dist1 = v0.distance(v1);
	float dist2 = v1.distance(v2);
	float dist3 = v2.distance(v3);
	float dist4 = v0.distance(v3);
	if (dist4 < DBL_EPSILON)
	{
		dist4 = DBL_EPSILON;
	}

	if (((dist1 + dist2 + dist3) / dist4)<(1 + FLATEPSILON))
	{
		return true;
	}
	else
	{
		return false;
	}
}

void displayBezierLocal(const Point v0, const Point v1, const Point v2, const Point v3, std::vector<Point>& EvaluatedPoints, float animateLength)
{
	if (flatEnough(v0, v1, v2, v3))
	{
		Point v0New = v0;
		Point v3New = v3;
		if (v0New.x > 2 * animateLength || v3New.x > 2 * animateLength)
		{
			return;
		}
		if (v0New.x > animateLength)
		{
			v0New.x = v0New.x - animateLength;
		}
		if (v3New.x > animateLength)
		{
			v3New.x = v3New.x - animateLength;
		}

		if ((v0.x - animateLength)*(v3.x - animateLength) < 0)
		{
			EvaluatedPoints.push_back(v0New);
			EvaluatedPoints.insert(EvaluatedPoints.begin(), v3New);
		}
		else if ((v0.x - animateLength)>0 && (v3.x - animateLength) > 0)
		{
			EvaluatedPoints.insert(++EvaluatedPoints.begin(), v0New);
			EvaluatedPoints.insert(++EvaluatedPoints.begin(), v3New);
		}
		else
		{
			EvaluatedPoints.push_back(v0New);
			EvaluatedPoints.push_back(v3New);
		}
	}
	else
	{
		Point v0New = getAvg(v0, v1);
		Point v1New = getAvg(v1, v2);
		Point v2New = getAvg(v2, v3);
		Point v0NewNew = getAvg(v0New, v1New);
		Point v1NewNew = getAvg(v1New, v2New);
		Point Qu = getAvg(v0NewNew, v1NewNew);

		displayBezierLocal(v0, v0New, v0NewNew, Qu, EvaluatedPoints, animateLength);
		displayBezierLocal(Qu, v1NewNew, v2New, v3, EvaluatedPoints, animateLength);
	}
}





void BsplineCurveEvaluator::evaluateCurve(const std::vector<Point>& ptvCtrlPts,
	std::vector<Point>& ptvEvaluatedCurvePts,
	const float& fAniLength,
	const bool& bWrap) const
{
	ptvEvaluatedCurvePts.clear();
	int iCtrlPtCount = ptvCtrlPts.size();


	//Copy the control points out so we may repeat the first and last point later
	vector<Point> ptvCtrlPtsNew;
	ptvCtrlPtsNew.assign(ptvCtrlPts.begin(), ptvCtrlPts.end());

	
	Point startPtModel = ptvCtrlPts[0];
	Point endPtModel = ptvCtrlPts[iCtrlPtCount - 1];

	if (!bWrap)
	{
		ptvCtrlPtsNew.insert(ptvCtrlPtsNew.begin(), startPtModel);
		ptvCtrlPtsNew.push_back(endPtModel);

		ptvCtrlPtsNew.insert(ptvCtrlPtsNew.begin(), startPtModel);
		ptvCtrlPtsNew.push_back(endPtModel);
	}
	else
	{
		int controlNewSize = ptvCtrlPtsNew.size();
		for (int i = 0; i < controlNewSize; i++)
		{
			float xNew = ptvCtrlPtsNew[i].x + fAniLength;
			float yNew = ptvCtrlPtsNew[i].y;
			Point dupPt(xNew, yNew);
			ptvCtrlPtsNew.push_back(dupPt);
		}
		for (int i = 0; i < 2; i++)
		{
			float xNew = ptvCtrlPtsNew[i].x + 2*fAniLength;
			float yNew = ptvCtrlPtsNew[i].y;
			Point dupPt(xNew, yNew);
			ptvCtrlPtsNew.push_back(dupPt);
		}
		
	}



	int iCtrlPtCountNew = ptvCtrlPtsNew.size();


	float x = 0.0;
	float y1;

	Mat4<float> bTov = 1.0/6.0*Mat4<float>(1.0f, 4.0f, 1.0f, 0.0f, 
											0.0f, 4.0f, 2.0f, 0.0f, 
											0.0f, 2.0f, 4.0f, 0.0f, 
											0.0f, 1.0f, 4.0f, 1.0f);

	for (int i = 0; i < iCtrlPtCountNew - 3; i++)
	{
		Vec4<float> bPointsX = Vec4<float>(ptvCtrlPtsNew[i].x, ptvCtrlPtsNew[i + 1].x, ptvCtrlPtsNew[i + 2].x, ptvCtrlPtsNew[i + 3].x);
		Vec4<float> bPointsY = Vec4<float>(ptvCtrlPtsNew[i].y, ptvCtrlPtsNew[i + 1].y, ptvCtrlPtsNew[i + 2].y, ptvCtrlPtsNew[i + 3].y);
		Vec4<float> vPointsX = bTov * bPointsX ;
		Vec4<float> vPointsY = bTov * bPointsY ;
		
		Point pt[4];

		for (int j = 0; j < 4; j++)
		{
			pt[j] = Point(vPointsX[j],vPointsY[j]);
		}

		displayBezierLocal(pt[0], pt[1], pt[2], pt[3], ptvEvaluatedCurvePts, fAniLength);

	}

	if (!bWrap)
	{
		y1 = ptvCtrlPtsNew[0].y;
		Point startPt = Point(x, y1);
		ptvEvaluatedCurvePts.push_back(startPt);
		x = fAniLength;
		float y2 = ptvCtrlPtsNew[iCtrlPtCountNew-1].y;
		Point endPt = Point(x, y2);
		ptvEvaluatedCurvePts.push_back(endPt);
	}
	else
	{
		int evaluatePtCount = ptvEvaluatedCurvePts.size();

		if ((ptvEvaluatedCurvePts[0].x + fAniLength) - ptvEvaluatedCurvePts[evaluatePtCount - 1].x > 0.0f) {
			y1 = (ptvEvaluatedCurvePts[0].y * (fAniLength - ptvEvaluatedCurvePts[evaluatePtCount - 1].x) +
				ptvEvaluatedCurvePts[evaluatePtCount - 1].y * ptvEvaluatedCurvePts[0].x) /
				(ptvEvaluatedCurvePts[0].x + fAniLength - ptvEvaluatedCurvePts[evaluatePtCount - 1].x);

			
		}
		else
		{
			y1 = ptvEvaluatedCurvePts[0].y;
		}
		x = 0.0;
		Point wrapStartPt(x, y1);

		x = fAniLength;
		Point wrapEndPt(x, y1);

		ptvEvaluatedCurvePts.push_back(wrapStartPt);
		ptvEvaluatedCurvePts.push_back(wrapEndPt);
	}
	
	
	
}
