#include "entity.h"

namespace Tmpl8
{
	int Entity::createCycle(int row, int w, int h, int amount, int speed)
	{
		cycle tmp;
		tmp.row = row;
		tmp.w = w;
		tmp.h = h;
		tmp.amount = amount;
		tmp.speed = speed;
		tmp.tick = 0;
		animations.push_back(tmp);
		return animations.size() - 1;
	}
	void Entity::updateAnimation()
	{
		SetSource(animations[m_CurAnimation].w * animations[m_CurAnimation].tick, 
			animations[m_CurAnimation].h * animations[m_CurAnimation].row, 
			animations[m_CurAnimation].w, 
			animations[m_CurAnimation].h);

		if (m_Begin > animations[m_CurAnimation].speed) { animations[m_CurAnimation].tick++; m_Begin = 0; }
		m_Begin++;
		if (animations[m_CurAnimation].tick >= animations[m_CurAnimation].amount) { animations[m_CurAnimation].tick = 0; }
			


	}
}
