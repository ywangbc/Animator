#include "catmullromevaluator.h"
#include <assert.h>
#include "vec.h"
#include "mat.h"

const float STEPNUM = 10000;

Mat4<float> catBaseMat(-1, 3, -3, 1,
	2, -5, 4, -1,
	-1, 0, 1, 0,
	0, 2, 0, 0
	);

void drawCat(Vec4<Point> evaPoint,
	const std::vector<Point>& ptvCtrlPts,
	std::vector<Point>& ptvEvaluatedCurvePts,
	const float& fAniLength)
{
	//float startX = evaPoint[1].x;
	//float endX = evaPoint[2].x;
	//float dist = endX - startX;

	Vec4<float> xAxis;
	for (int i = 0; i < 4; i++)
	{
		xAxis[i] = evaPoint[i].x;
	}
	Vec4<float> xTransAxis = 0.5*catBaseMat*xAxis;

	Vec4<float> yAxis;
	for (int i = 0; i < 4; i++)
	{
		yAxis[i] = evaPoint[i].y;
	}
	
	Vec4<float> yTransAxis = 0.5*catBaseMat*yAxis;
	for (int i = 0; i <= STEPNUM; i++)
	{
		float tSingle = float(i) / STEPNUM;
		Vec4<float> tTerm( tSingle*tSingle*tSingle, tSingle*tSingle, tSingle, 1.0);
		float xVal = tTerm * xTransAxis;
		float yVal = tTerm * yTransAxis;
		if (xVal<evaPoint[1].x || xVal>evaPoint[2].x)
		{
			continue;
		}
		if (xVal < 0)
		{
			xVal += fAniLength;
		}
		else if (xVal>fAniLength)
		{
			xVal -= fAniLength;
		}
		ptvEvaluatedCurvePts.push_back(Point(xVal, yVal));

	}


	
	
}

void CatmullRomEvaluator::evaluateCurve(const std::vector<Point>& ptvCtrlPts,
	std::vector<Point>& ptvEvaluatedCurvePts,
	const float& fAniLength,
	const bool& bWrap) const
{
	ptvEvaluatedCurvePts.clear();
	int iCtrlPtCount = ptvCtrlPts.size();
	//Copy the control points out so we may repeat the first and last point later
	std::vector<Point> ptvCtrlPtsNew;
	ptvCtrlPtsNew.assign(ptvCtrlPts.begin(), ptvCtrlPts.end());
	if (!bWrap)
	{
		Point tmp0(2 * ptvCtrlPts[0].x - ptvCtrlPts[1].x,
			2 * ptvCtrlPts[0].y - ptvCtrlPts[1].y);
		ptvCtrlPtsNew.insert(ptvCtrlPtsNew.begin(), tmp0);

		//Last
		Point tmp1(2 * ptvCtrlPts[iCtrlPtCount - 1].x - ptvCtrlPts[iCtrlPtCount - 2].x,
			2 * ptvCtrlPts[iCtrlPtCount - 1].y - ptvCtrlPts[iCtrlPtCount - 2].y);
		ptvCtrlPtsNew.push_back(tmp1);
	}
	else
	{
		for (int i = iCtrlPtCount - 1; i >= 0; i--)
		{
			Point newPt(ptvCtrlPts[i].x - fAniLength, ptvCtrlPts[i].y);
			ptvCtrlPtsNew.insert(ptvCtrlPtsNew.begin(), newPt);
		}
		for (int i = 0; i < iCtrlPtCount; i++)
		{
			Point newPt(ptvCtrlPts[i].x + fAniLength, ptvCtrlPts[i].y);
			ptvCtrlPtsNew.push_back(newPt);
		}
	}
	



	
	int iCtrlPtNewCount = ptvCtrlPtsNew.size();

	for (int i = 0; i < iCtrlPtNewCount - 3; i++)
	{
		Vec4<Point> evaPoint(Point(0, 0), Point(0, 0), Point(0, 0), Point(0, 0));
		for (int evaNum = 0; evaNum < 4; evaNum++)
		{
			evaPoint[evaNum] = ptvCtrlPtsNew[i + evaNum];
		}
		drawCat(evaPoint,ptvCtrlPts,ptvEvaluatedCurvePts,fAniLength);
	}

	if (!bWrap)
	{
		float firstX = 0;
		float firstY = ptvCtrlPts[0].y;
		float lastX = fAniLength;
		float lastY = ptvCtrlPts[iCtrlPtCount-1].y;
		Point firstPt(firstX, firstY);
		Point lastPt(lastX, lastY);

		ptvEvaluatedCurvePts.push_back(firstPt);
		ptvEvaluatedCurvePts.push_back(lastPt);
	}

	
}
