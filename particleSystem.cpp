#pragma warning(disable : 4786)

#include "particleSystem.h"

#include "modelerapp.h"

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <math.h>
#include <limits.h>

/***************
* Particle
***************/
Particle::Particle(){
	position = Vec3f(0, 0, 0);
	velocity = Vec3f(0, 0, 0);
	force = Vec3f(0, 0, 0);
	mass = 1;
	age = 0;
	type = NOTHING;
}
Particle::Particle(Vec3f pos, Vec3f v, Vec3f f, float m, float al, float sz, ParticleType t) :
	position(pos), velocity(v), force(f), mass(m), age(0), ageLimit(al),size(sz),type(t){
	}
Particle::~Particle(){

}
Vec3f Particle::getAccel(){
	return force / mass;
}
void Particle::clearForce(){
	force = Vec3f(0, 0, 0);
}


/***************
* Gravity
***************/
Gravity::Gravity(){
	g = 0;
	r = 0;
	effectType = NOTHING;
}
Gravity::Gravity(ParticleType e, float grav, float ran){
	g = grav;
	r = ran;
	effectType = e;
}
Gravity::~Gravity(){}
void Gravity::applyForce(vector<Particle>::iterator start, vector<Particle>::iterator end){
	vector<Particle>::iterator p;
	for (p = start; p != end; p++){
		float tg;
		int k = rand() % 100 - 50;
		tg = (float)k / 100.0 * r + g;
		if (p->type == effectType)p->force += Vec3f(0, -p->mass*tg, 0);
	}
}
/***************
* Wind
***************/
Wind::Wind(){
	effectType = NOTHING;
	intensity = 0.0;
	r = 0.0;
	direction = Vec3f(1, 0, 0);
}
Wind::Wind(ParticleType e, float inten, Vec3f direct, float ran, Vec3f ranDirect){
	effectType = e;
	intensity = inten;
	r = ran;
	direction = direct;
	direction.normalize();
	ranDirection = ranDirect;
	ranDirection.normalize();
}
Wind::~Wind(){

}
void Wind::applyForce(vector<Particle>::iterator start, vector<Particle>::iterator end){
	vector<Particle>::iterator p;
	for (p = start; p != end; p++){
		float tg;
		int k = rand() % 100 - 50;
		tg = (float)k / 100.0 * r;
		if(p->type == effectType)p->force += direction * intensity + ranDirection * tg;
	}
}



/***************
 * Constructors
 ***************/

ParticleSystem::ParticleSystem() 
{
	// TODO
	bake_fps = 10;
}





/*************
 * Destructor
 *************/

ParticleSystem::~ParticleSystem() 
{
	// TODO
	vector<Force*>::iterator it;
	for (it = allForces.begin(); it != allForces.end(); it++){
		if ((*it) != NULL)delete (*it);
	}
	allForces.clear();
}


/******************
 * Simulation fxns
 ******************/

/** Start the simulation */
void ParticleSystem::startSimulation(float t)
{
    
	// TODO
	clearBaked();
	bake_start_time = t;
	bake_fps = ModelerApplication::Instance()->GetFps() / 3;
	// These values are used by the UI ...
	// -ve bake_end_time indicates that simulation
	// is still progressing, and allows the
	// indicator window above the time slider
	// to correctly show the "baked" region
	// in grey.
	bake_end_time = -1;
	simulate = true;
	dirty = true;

}

/** Stop the simulation */
void ParticleSystem::stopSimulation(float t)
{
    
	// TODO
	clearAllParticles();
	bake_end_time = t;
	// These values are used by the UI
	simulate = false;
	dirty = true;

}

/** Reset the simulation */
void ParticleSystem::resetSimulation(float t)
{
    
	// TODO
	clearBaked();
	clearAllParticles();
	bake_start_time = t;
	bake_end_time = -1;
	bake_fps = ModelerApplication::Instance()->GetFps() / 3;
	// These values are used by the UI
	simulate = false;
	dirty = true;

}

/** Compute forces and update particles **/
void ParticleSystem::computeForcesAndUpdateParticles(float t)
{
	clearAgedParticles();
	zeroAllParticleForces();
	applyAllForces();
	map<ParticleType, vector<Particle>>::iterator mit;
	vector<Particle>::iterator vit;
	float time_delta = 0.025;
	for (int i = 1; i < int(PARTICLE_TYPES); i++){
		mit = particles.find(ParticleType(i));
		if (mit != particles.end()){
			for (vit = mit->second.begin(); vit != mit->second.end(); vit++){
				vit->position += vit->velocity * time_delta;
				vit->velocity += vit->getAccel() * time_delta;
				vit->age += time_delta;
			}
		}
	}
	// TODO
}

void ParticleSystem::setBakeFps(int fps){
	bake_fps = fps;
}

/** Render particles */
void ParticleSystem::drawParticles(float t)
{
	map<ParticleType, vector<Particle>>::iterator mit;
	vector<Particle>::iterator vit;
	map<int, vector<Particle>>::iterator fit;
	fit = bakeBuffer.find(int(t*bake_fps));
	if (fit != bakeBuffer.end()){
		for (vit = fit->second.begin(); vit != fit->second.end(); vit++){
			vit->render();
		}
	}
	else{
		bakeParticles(t);
		for (int i = 1; i < int(PARTICLE_TYPES); i++){
			mit = particles.find(ParticleType(i));
			if (mit != particles.end()){
				for (vit = mit->second.begin(); vit != mit->second.end(); vit++){
					vit->render();
				}
			}
		}
	}
	// TODO
}





/** Adds the current configuration of particles to
  * your data structure for storing baked particles **/
void ParticleSystem::bakeParticles(float t) 
{
	map<ParticleType, vector<Particle>>::iterator mit;
	vector<Particle>::iterator vit;
	for (int i = 1; i < int(PARTICLE_TYPES); i++){
		mit = particles.find(ParticleType(i));
		if (mit != particles.end()){
			for (vit = mit->second.begin(); vit != mit->second.end(); vit++){
				bakeBuffer[int(t * bake_fps)].push_back(*vit);
			}
		}
	}
	// TODO
}

/** Clears out your data structure of baked particles */
void ParticleSystem::clearBaked()
{
	bakeBuffer.clear();
	// TODO
}


void ParticleSystem::addForce(Force* f){
	allForces.push_back(f);
}
void ParticleSystem::zeroAllParticleForces(){
	map<ParticleType, vector<Particle>>::iterator mit;
	vector<Particle>::iterator vit;
	for (int p = 1; p < int(PARTICLE_TYPES); p++){
		mit = particles.find(ParticleType(p));
		if (mit != particles.end()){
			for (vit = mit->second.begin(); vit != mit->second.end(); vit++){
				vit->clearForce();
			}
		}
	}
}

void ParticleSystem::applyAllForces(){
	map<ParticleType, vector<Particle>>::iterator mit;
	vector<Force*>::iterator fit;
	for (fit = allForces.begin(); fit != allForces.end(); fit++){
		if ((*fit) != NULL){
			mit = particles.find((*fit)->getType());
			if (mit != particles.end()){
				(*fit)->applyForce(mit->second.begin(), mit->second.end());
			}
		}
	}
}

void ParticleSystem::clearAllParticles(){
	particles.clear();
}

bool ParticleSystem::pred(Particle& p){
	return p.age > p.ageLimit + 1e-6;
}

void ParticleSystem::clearAgedParticles(){
	map<ParticleType, vector<Particle>>::iterator mit;
	for (int p = 1; p < int(PARTICLE_TYPES); p++){
		mit = particles.find(ParticleType(p));
		if (mit != particles.end()){
			mit->second.erase(std::remove_if(mit->second.begin(), mit->second.end(), pred), mit->second.end());
		}
	}

}

void ParticleSystem::addParticle(Vec3f pos, Vec3f v, float m, float al, float sz,ParticleType t){
	Particle p(pos,v,Vec3f(0,0,0),m,al,sz,t);
	particles[t].push_back(p);
}