#ifndef INCLUDED_C2_INTERPOLATING_EVALUATOR_H
#define INCLUDED_C2_INTERPOLATING_EVALUATOR_H
#pragma warning(disable : 4786)  

#include "CurveEvaluator.h"

//using namespace std;

class C2InterpolatingEvaluator : public CurveEvaluator
{
public:
	void evaluateCurve(const std::vector<Point>& ptvCtrlPts,
		std::vector<Point>& ptvEvaluatedCurvePts,
		const float& fAniLength,
		const bool& bWrap) const;
};

#endif