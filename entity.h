#pragma once  
#include "object.h"  
#include <vector>

namespace Tmpl8  
{  
class Entity : public Object  
{  
public:  
	int createCycle(int row, int w, int h, int amount, int speed);  
	void setCurAnimation(int c) { m_Begin = 0, m_CurAnimation = c; }  
	int getCurAnimation() { return m_CurAnimation; }
	void updateAnimation();  

private:  
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
	int m_CurAnimation;  
	int m_Begin;  
};  
}