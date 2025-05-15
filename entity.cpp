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
		m_Animations.push_back(tmp);
		return m_Animations.size() - 1;
	}
	void Entity::updateAnimation() // got this setup from a tutorial, I understand how it works but would not be able to recreate it from scratch
	{
		SetSource(m_Animations[m_CurAnimation].w * m_Animations[m_CurAnimation].tick, 
			m_Animations[m_CurAnimation].h * m_Animations[m_CurAnimation].row, 
			m_Animations[m_CurAnimation].w, 
			m_Animations[m_CurAnimation].h);

		if (m_Begin > m_Animations[m_CurAnimation].speed) { m_Animations[m_CurAnimation].tick++; m_Begin = 0; }
		m_Begin++;
		if (m_Animations[m_CurAnimation].tick >= m_Animations[m_CurAnimation].amount) { m_Animations[m_CurAnimation].tick = 0; }
			


	}
}
