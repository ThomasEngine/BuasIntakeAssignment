#pragma once  
#include "object.h"  
#include <vector>

namespace Tmpl8  
{  
class Entity : public Object  
{  
public:  
	void SetHealth(int h) { health = h; }  
	void SetMaxHealth(int mh) { maxHealth = mh; } 
	void SetSpeed(float s) { eSpeed = s; }
	float GetSpeed() const { return eSpeed; }
	int GetHealth() const { return health; }  
	int GetMaxHealth() const { return maxHealth; }  

	int createCycle(int row, int w, int h, int amount, int speed);  
	void setCurAnimation(int c) { begin = 0, curAnimation = c; }  
	int getCurAnimation() { return curAnimation; }
	void updateAnimation();  

private:  
	int health, maxHealth;  
	struct cycle  
	{  
		int row;  
		int w;  
		int h;  
		int amount;  
		int speed;  
		int tick;  
	};  
	std::vector<cycle> animations; // Use std::vector explicitly  
	int curAnimation;  
	int begin;  
	float eSpeed;
};  
}