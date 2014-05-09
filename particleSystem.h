/***********************
 * ParticleSystem class
 ***********************/

/**
 * The particle system class simply "manages" a collection of particles.
 * Its primary responsibility is to run the simulation, evolving particles
 * over time according to the applied forces using Euler's method.
 * This header file contains the functions that you are required to implement.
 * (i.e. the rest of the code relies on this interface)
 * In addition, there are a few suggested state variables included.
 * You should add to this class (and probably create new classes to model
 * particles and forces) to build your system.
 */

#ifndef __PARTICLE_SYSTEM_H__
#define __PARTICLE_SYSTEM_H__

#include "vec.h"
#include "mat.h"
#include<vector>
#include<map>
#include<algorithm>

enum ParticleType {
	NOTHING = 0,
	GROUND,
	BURST,
	INVISIBLE_AIR,
	EXCALIBUR_PREPARE,
	EXCALIBUR_CAST,
	EXCALIBUR_CAST_CORE,
	PARTICLE_TYPES
};

class Particle{
public:
	Particle();
	Particle(Vec3f POSITION, Vec3f VELOCITY, Vec3f FORCE, float MASS, float AGE_LIMIT, float SIZE, ParticleType TYPE);
	~Particle();
	void render();
	Vec3f getAccel();
	void clearForce();
	Vec3f position, velocity;
	Vec3f force;
	float mass;
	float age;
	float ageLimit;
	float size;
	ParticleType type;
};

class Force{
public:
	Force(){}
	~Force(){}
	virtual void applyForce(vector<Particle>::iterator start, vector<Particle>::iterator end) = 0;
	ParticleType getType(){ return effectType; }
protected:
	ParticleType effectType;
};

class Gravity : public Force{
public:
	Gravity(); 
	Gravity(ParticleType e, float grav, float ran = 0.0);
	~Gravity();
	virtual void applyForce(vector<Particle>::iterator start, vector<Particle>::iterator end);
protected:
	float g;//gravity accleration
	float r;//random quantity to make behavior of particles different
};


class Drag : public Force{
public:
	Drag();
	Drag(ParticleType e, float intensity,float quadIntensity);
	~Drag();
	virtual void applyForce(vector<Particle>::iterator start, vector<Particle>::iterator end);
protected:
	float intensity;
	float quadIntensity;
};

class AxisForce : public Force{
public:
	AxisForce(){}
	~AxisForce(){}
	virtual void changeAxis(Vec3f axisStart, Vec3f axisEnd) = 0;
};

class Wind : public AxisForce{
public:
	Wind();
	Wind(ParticleType e, float intensity, Vec3f direction, float ran = 0.0, Vec3f ranDirection = Vec3f(0, 0, 0));
	~Wind();
	virtual void applyForce(vector<Particle>::iterator start, vector<Particle>::iterator end);
	virtual void changeAxis(Vec3f axisStart, Vec3f axisEnd);
protected:
	float intensity;
	float r;
	Vec3f direction;
	Vec3f ranDirection;
};

class Storm : public AxisForce{
public:
	Storm();
	Storm(ParticleType e, Vec3f axisStart, Vec3f axisEnd, float alongAxis, float radiusStart, float radiusEnd, float ran = 0.0);
	~Storm();
	virtual void applyForce(vector<Particle>::iterator start, vector<Particle>::iterator end);
	virtual void changeAxis(Vec3f axisStart, Vec3f axisEnd);
protected:
	Vec3f axis[2];
	float axisIntensity;
	float radius[2];
	float r;
	void forceParticle(Particle& p);
};



class ParticleSystem {

public:



	/** Constructor **/
	ParticleSystem();


	/** Destructor **/
	virtual ~ParticleSystem();

	/** Simulation fxns **/
	// This fxn should render all particles in the system,
	// at current time t.
	virtual void drawParticles(float t);

	// This fxn should save the configuration of all particles
	// at current time t.
	virtual void bakeParticles(float t);

	// This function should compute forces acting on all particles
	// and update their state (pos and vel) appropriately.
	virtual void computeForcesAndUpdateParticles(float t);

	// This function should reset the system to its initial state.
	// When you need to reset your simulation, PLEASE USE THIS FXN.
	// It sets some state variables that the UI requires to properly
	// update the display.  Ditto for the following two functions.
	virtual void resetSimulation(float t);

	// This function should start the simulation
	virtual void startSimulation(float t);

	// This function should stop the simulation
	virtual void stopSimulation(float t);

	// This function should clear out your data structure
	// of baked particles (without leaking memory).
	virtual void clearBaked();	

	void addForce(Force* f);
	void zeroAllParticleForces();
	void applyAllForces();
	void clearAllParticles();
	void clearAgedParticles();
	void addParticle(Vec3f POSITION, Vec3f VELOCITY, float MASS, float AGE_LIMIT, float SIZE, ParticleType t);
	void setBakeFps(int fps);

	// These accessor fxns are implemented for you
	float getBakeStartTime() { return bake_start_time; }
	float getBakeEndTime() { return bake_end_time; }
	float getBakeFps() { return bake_fps; }
	bool isSimulate() { return simulate; }
	bool isDirty() { return dirty; }
	void setDirty(bool d) { dirty = d; }

	static bool pred(Particle& p);

	vector<Force*> allForces;

protected:
	


	/** Some baking-related state **/
	float bake_fps;						// frame rate at which simulation was baked
	float bake_start_time;				// time at which baking started 
										// These 2 variables are used by the UI for
										// updating the grey indicator 
	float bake_end_time;				// time at which baking ended

	/** General state variables **/
	bool simulate;						// flag for simulation mode
	bool dirty;							// flag for updating ui (don't worry about this)

	map<ParticleType,vector<Particle>> particles;
	map<int, vector<Particle>> bakeBuffer;
};


#endif	// __PARTICLE_SYSTEM_H__
