#include "sabermodel.h"


enum SaberModelControls
{
	XPOS = 0, YPOS, ZPOS, SWORD_TRANSPARENCY,
	ROTATE, SLASH, COSTUME, SLASH_TYPE, COMPLEXITY, NUMCONTROLS
};

#define VAL(x) (ModelerApplication::Instance()->GetControlValue(x))

// We need to make a creator function, mostly because of
// nasty API stuff that we'd rather stay away from.
ModelerView* createSaberModel(int x, int y, int w, int h, char *label)
{
	return new SaberModel(x, y, w, h, label);
}

// We are going to override (is that the right word?) the draw()
// method of ModelerView to draw out SaberModel
void SaberModel::draw()
{
	// This call takes care of a lot of the nasty projection 
	// matrix stuff.  Unless you want to fudge directly with the 
	// projection matrix, don't bother with this ...

		ModelerView::draw();


	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	ChooseCostume(1);
	setComplexity(1);

	/*
	if (animation)
	{

		LocateBody(VAL(XPOS), VAL(YPOS), VAL(ZPOS), VAL(ROTATE));
		
		int SType = VAL(SLASH_TYPE);
		switch (SType){
		case VERTICAL:
			if (step <= STEPMAX)
			{
				double angle = step / double(STEPMAX) * 90;

				RotateExcalibur(0.0, 90.0, LOWER_ARM_SLASH*angle<40.0 ? LOWER_ARM_SLASH*angle : 40.0, "yzx");
				RotateRightUpperArm(1.5*angle, 0.0, 0.0);
				RotateRightLowerArm(LOWER_ARM_SLASH*angle, 0.0, LOWER_ARM_SLASH*angle<40.0 ? -LOWER_ARM_SLASH*angle : -40.0, "zxy");
				RotateLeftUpperArm(1.5*angle, 0.0, 0.0);
				RotateLeftLowerArm(LOWER_ARM_SLASH*angle, 0.0, LOWER_ARM_SLASH*angle<40.0 ? LOWER_ARM_SLASH*angle : 40.0, "zxy");
				step++;
			}
			else if (step <= 1.5 * STEPMAX)
			{
				int temp = 1.5 * STEPMAX - step;
				temp = temp * 2.0;
				double angle = temp / double(STEPMAX) * 40 + 50;

				RotateExcalibur(0.0, 90.0, LOWER_ARM_SLASH*angle<40.0 ? LOWER_ARM_SLASH*angle : 40.0, "yzx");
				RotateRightUpperArm(1.5*angle, 0.0, 0.0);
				RotateRightLowerArm(LOWER_ARM_SLASH*angle, 0.0, LOWER_ARM_SLASH*angle<40.0 ? -LOWER_ARM_SLASH*angle : -40.0, "zxy");
				RotateLeftUpperArm(1.5*angle, 0.0, 0.0);
				RotateLeftLowerArm(LOWER_ARM_SLASH*angle, 0.0, LOWER_ARM_SLASH*angle<40.0 ? LOWER_ARM_SLASH*angle : 40.0, "zxy");
				step+=3;
			}
			else if (step <= 2.5*STEPMAX){
				double angle = 50.0;

				RotateExcalibur(0.0, 90.0, LOWER_ARM_SLASH*angle<40.0 ? LOWER_ARM_SLASH*angle : 40.0, "yzx");
				RotateRightUpperArm(1.5*angle, 0.0, 0.0);
				RotateRightLowerArm(LOWER_ARM_SLASH*angle, 0.0, LOWER_ARM_SLASH*angle<40.0 ? -LOWER_ARM_SLASH*angle : -40.0, "zxy");
				RotateLeftUpperArm(1.5*angle, 0.0, 0.0);
				RotateLeftLowerArm(LOWER_ARM_SLASH*angle, 0.0, LOWER_ARM_SLASH*angle<40.0 ? LOWER_ARM_SLASH*angle : 40.0, "zxy");
				step++;
			}
			else {
				step = 0;
				//break;
			}
			break;
		case STAND:
			if (step <= STEPMAX)
			{
				double angle = step / double(STEPMAX) * 60;

				RotateExcalibur(angle>75 ? -angle * 2 + 25 : -angle*125.0 / 75.0, 0.0, angle < 40.0 ? angle : 40.0, "xyz");
				RotateRightUpperArm(1.1*angle, 0.0, 0.0);
				RotateRightLowerArm(0.6*angle, 0.0, angle < 40.0 ? -angle : -40.0, "zxy");
				RotateLeftUpperArm(1.1*angle, 0.0, 0.0);
				RotateLeftLowerArm(0.6*angle, 0.0, angle < 40.0 ? angle : 40.0, "zxy");
				step+=2;
			}
			else if (step <= 1.5 * STEPMAX)
			{
				int temp = 1.5 * STEPMAX - step;
				temp = temp * 2.0;
				double angle = temp / double(STEPMAX) * 20 + 40;

				RotateExcalibur(angle>75 ? -angle * 2 + 25 : -angle*125.0 / 75.0, 0.0, angle < 40.0 ? angle : 40.0, "xyz");
				RotateRightUpperArm(1.1*angle, 0.0, 0.0);
				RotateRightLowerArm(0.6*angle, 0.0, angle < 40.0 ? -angle : -40.0, "zxy");
				RotateLeftUpperArm(1.1*angle, 0.0, 0.0);
				RotateLeftLowerArm(0.6*angle, 0.0, angle < 40.0 ? angle : 40.0, "zxy");
				step +=3;
			}
			else if (step <= 3.0*STEPMAX){
				double angle = 40.0;

				RotateExcalibur(angle>75 ? -angle * 2 + 25 : -angle*125.0 / 75.0, 0.0, angle < 40.0 ? angle : 40.0, "xyz");
				RotateRightUpperArm(1.1*angle, 0.0, 0.0);
				RotateRightLowerArm(0.6*angle, 0.0, angle < 40.0 ? -angle : -40.0, "zxy");
				RotateLeftUpperArm(1.1*angle, 0.0, 0.0);
				RotateLeftLowerArm(0.6*angle, 0.0, angle < 40.0 ? angle : 40.0, "zxy");
				step++;
			}
			else {
				step = 0;
				//break;
			}
			break;
		case SKEW:
			
			if (step <= STEPMAX)
			{
				double angle = step / double(STEPMAX) * 45;

				RotateExcalibur(0.0, 0.0, 70.0 * angle / 45.0, "xyz");
				RotateRightUpperArm(90.0*angle / 45.0, -30.0 * angle / 45.0, 0.0, "xyz");
				RotateRightLowerArm(110.0*angle / 45.0, 30.0*angle / 45.0, 0.0, "xyz");
				RotateLeftUpperArm(90.0*angle / 45.0, -45.0*angle / 45.0, 0.0, "xyz");
				RotateLeftLowerArm(65.0*angle / 45.0, -15.0*angle / 45.0, 0.0, "xyz");
				
				step++;
			}
			else if (step <= 2.0 * STEPMAX)
			{
				int temp = step - STEPMAX;
				double angle = temp / double(STEPMAX) * 45 + 45;

				RotateExcalibur(0.0, 90.0*(angle - 45.0) / 45.0, 70.0 *(1 - (angle - 45.0) / 45.0), "xyz");
				RotateRightUpperArm(90.0 - 30.0*(angle - 45.0) / 45.0, -30.0 + 55.0 * (angle - 45.0) / 45.0, 0.0, "xyz");
				RotateRightLowerArm(110.0 - 110.0*(angle - 45.0) / 45.0, 30.0 - 30.0*(angle - 45.0) / 45.0, 0.0, "xyz");
				RotateLeftUpperArm(90.0 - 40.0*(angle - 45.0) / 45.0, -45.0 + 30.0*(angle - 45.0) / 45.0, 0.0, "xyz");
				RotateLeftLowerArm(65.0 - 50.0*(angle - 45.0) / 45.0, -15.0 + 15.0*(angle - 45.0) / 45.0, 10.0*(angle - 45.0) / 45.0, "xyz");

				step += 5;
			}
			else if (step <= 3.0*STEPMAX){
				double angle = 90.0;

				RotateExcalibur(0.0, 90.0*(angle - 45.0) / 45.0, 70.0 *(1 - (angle - 45.0) / 45.0), "xyz");
				RotateRightUpperArm(90.0 - 30.0*(angle - 45.0) / 45.0, -30.0 + 55.0 * (angle - 45.0) / 45.0, 0.0, "xyz");
				RotateRightLowerArm(110.0 - 110.0*(angle - 45.0) / 45.0, 30.0 - 30.0*(angle - 45.0) / 45.0, 0.0, "xyz");
				RotateLeftUpperArm(90.0 - 40.0*(angle - 45.0) / 45.0, -45.0 + 30.0*(angle - 45.0) / 45.0, 0.0, "xyz");
				RotateLeftLowerArm(65.0 - 50.0*(angle - 45.0) / 45.0, -15.0 + 15.0*(angle - 45.0) / 45.0, 10.0*(angle - 45.0) / 45.0, "xyz");
				step++;
			}
			else {
				step = 0;
				//break;
			}
			break;
		case SINGLE:
			if (step <= STEPMAX)
			{
				double angle = step / double(STEPMAX) * 45;

				RotateExcalibur(0.0, 0.0, 70.0 * angle / 45.0, "xyz");
				RotateRightUpperArm(90.0*angle / 45.0, -30.0 * angle / 45.0, 0.0, "xyz");
				RotateRightLowerArm(110.0*angle / 45.0, 30.0*angle / 45.0, 0.0, "xyz");

				step+=2;
			}
			else if (step <= 2.0 * STEPMAX)
			{
				int temp = step - STEPMAX;
				double angle = temp / double(STEPMAX) * 45 + 45;


				RotateExcalibur(0.0, -90.0*(angle - 45.0) / 45.0, 70.0 *(1 - (angle - 45.0) / 45.0), "xyz");
				RotateRightUpperArm(90.0, -30.0 + 75.0 * (angle - 45.0) / 45.0, 0.0, "xyz");
				RotateRightLowerArm(110.0 - 110.0*(angle - 45.0) / 45.0, 30.0 + 60.0*(angle - 45.0) / 45.0, 0.0, "xyz");

				step += 4;
			}
			else if (step <= 2.4*STEPMAX){
				double angle = 90.0;


				RotateExcalibur(0.0, -90.0*(angle - 45.0) / 45.0, 70.0 *(1 - (angle - 45.0) / 45.0), "xyz");
				RotateRightUpperArm(90.0, -30.0 + 75.0 * (angle - 45.0) / 45.0, 0.0, "xyz");
				RotateRightLowerArm(110.0 - 110.0*(angle - 45.0) / 45.0, 30.0 + 60.0*(angle - 45.0) / 45.0, 0.0, "xyz");
				step++;
			}
			else {
				step = 0;
				//break;
			}
			break;
		}

		
		//}
	}*/
	/*else
	{*/
		LocateBody(VAL(XPOS), VAL(YPOS), VAL(ZPOS), VAL(ROTATE));
		int SType = VAL(SLASH_TYPE);
		switch (SType){
		case VERTICAL:
			RotateExcalibur(0.0, 90.0, LOWER_ARM_SLASH*VAL(SLASH) < 40.0 ? LOWER_ARM_SLASH*VAL(SLASH) : 40.0, "yzx");
			RotateRightUpperArm(1.5*VAL(SLASH), 0.0, 0.0);
			RotateRightLowerArm(LOWER_ARM_SLASH*VAL(SLASH), 0.0, LOWER_ARM_SLASH*VAL(SLASH) < 40.0 ? -LOWER_ARM_SLASH*VAL(SLASH) : -40.0, "zxy");
			RotateLeftUpperArm(1.5*VAL(SLASH), 0.0, 0.0);
			RotateLeftLowerArm(LOWER_ARM_SLASH*VAL(SLASH), 0.0, LOWER_ARM_SLASH*VAL(SLASH) < 40.0 ? LOWER_ARM_SLASH*VAL(SLASH) : 40.0, "zxy");
			break;
		case STAND:
			RotateExcalibur(VAL(SLASH)>75?-VAL(SLASH)*2+25:-VAL(SLASH)*125.0/75.0, 0.0,VAL(SLASH) < 40.0 ? VAL(SLASH) : 40.0 , "xyz");
			RotateRightUpperArm(1.1*VAL(SLASH), 0.0, 0.0);
			RotateRightLowerArm(0.6*VAL(SLASH), 0.0, VAL(SLASH) < 40.0 ? -VAL(SLASH) : -40.0, "zxy");
			RotateLeftUpperArm(1.1*VAL(SLASH), 0.0, 0.0);
			RotateLeftLowerArm(0.6*VAL(SLASH), 0.0, VAL(SLASH) < 40.0 ? VAL(SLASH) : 40.0, "zxy");
			break;
		case SKEW:
			if (VAL(SLASH) < 45.0){
				RotateExcalibur(0.0, 0.0, 70.0 * VAL(SLASH) / 45.0, "xyz");
				RotateRightUpperArm(90.0*VAL(SLASH) / 45.0, -30.0 * VAL(SLASH) / 45.0, 0.0, "xyz");
				RotateRightLowerArm(110.0*VAL(SLASH) / 45.0, 30.0*VAL(SLASH) / 45.0, 0.0, "xyz");
				RotateLeftUpperArm(90.0*VAL(SLASH) / 45.0, -45.0*VAL(SLASH) / 45.0, 0.0, "xyz");
				RotateLeftLowerArm(65.0*VAL(SLASH) / 45.0, -15.0*VAL(SLASH) / 45.0, 0.0, "xyz");
			}
			else{
				RotateExcalibur(0.0, 90.0*(VAL(SLASH) - 45.0) / 45.0, 70.0 *(1 - (VAL(SLASH) - 45.0) / 45.0), "xyz");
				RotateRightUpperArm(90.0 - 30.0*(VAL(SLASH) - 45.0) / 45.0, -30.0 + 55.0 * (VAL(SLASH) - 45.0) / 45.0, 0.0, "xyz");
				RotateRightLowerArm(110.0 - 110.0*(VAL(SLASH) - 45.0) / 45.0, 30.0 - 30.0*(VAL(SLASH) - 45.0) / 45.0, 0.0, "xyz");
				RotateLeftUpperArm(90.0 - 40.0*(VAL(SLASH) - 45.0) / 45.0, -45.0 + 30.0*(VAL(SLASH) - 45.0) / 45.0, 0.0, "xyz");
				RotateLeftLowerArm(65.0 - 50.0*(VAL(SLASH) - 45.0) / 45.0, -15.0 + 15.0*(VAL(SLASH) - 45.0) / 45.0, 10.0*(VAL(SLASH) - 45.0) / 45.0, "xyz");
				break;
			}
		case SINGLE:
			if (VAL(SLASH) < 45.0){
				RotateExcalibur(0.0, 0.0, 70.0 * VAL(SLASH) / 45.0, "xyz");
				RotateRightUpperArm(90.0*VAL(SLASH) / 45.0, -30.0 * VAL(SLASH) / 45.0, 0.0, "xyz");
				RotateRightLowerArm(110.0*VAL(SLASH) / 45.0, 30.0*VAL(SLASH) / 45.0, 0.0, "xyz");
			}
			else{
				RotateExcalibur(0.0, -90.0*(VAL(SLASH) - 45.0) / 45.0, 70.0 *(1 - (VAL(SLASH) - 45.0) / 45.0), "xyz");
				RotateRightUpperArm(90.0, -30.0 + 75.0 * (VAL(SLASH) - 45.0) / 45.0, 0.0, "xyz");
				RotateRightLowerArm(110.0 - 110.0*(VAL(SLASH) - 45.0) / 45.0, 30.0 + 60.0*(VAL(SLASH) - 45.0) / 45.0, 0.0, "xyz");
			}
				break;
		}
	/*
	}*/
		
		
	treeRoot->Render();

}

int main()
{
	// Initialize the controls
	// Constructor is ModelerControl(name, minimumvalue, maximumvalue, 
	// stepsize, defaultvalue)
	ModelerControl controls[NUMCONTROLS];
	controls[XPOS] = ModelerControl("X Position", -5, 5, 0.1f, 0);
	controls[YPOS] = ModelerControl("Y Position", 0, 5, 0.1f, 0);
	controls[ZPOS] = ModelerControl("Z Position", -5, 5, 0.1f, 0);
	controls[SWORD_TRANSPARENCY] = ModelerControl("SwordTransparency", 0.0, 1.0, 0.1f, 1);
	controls[ROTATE] = ModelerControl("Rotate", -180, 180, 1, 0);
	controls[SLASH] = ModelerControl("Slash", 0, 90, 1, 0);
	controls[COSTUME] = ModelerControl("Costume", 1, 3, 1, 1);
	controls[SLASH_TYPE] = ModelerControl("Slash Type", 1, SLASH_TYPE_NUM-1, 1, 1);
	controls[COMPLEXITY] = ModelerControl("Complexity", 0, 1, 1, 0);

	ModelerApplication::Instance()->Init(&createSaberModel, controls, NUMCONTROLS);
	return ModelerApplication::Instance()->Run();
}
