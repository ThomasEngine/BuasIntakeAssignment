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
		return animations.size();
	}
	void Entity::updateAnimation()
	{
		SetSource(animations[curAnimation].w * animations[curAnimation].tick, 
			animations[curAnimation].h * animations[curAnimation].row, 
			animations[curAnimation].w, 
			animations[curAnimation].h);

		if (begin > animations[curAnimation].speed) { animations[curAnimation].tick++; begin = 0; }
		begin++;
		if (animations[curAnimation].tick >= animations[curAnimation].amount) { animations[curAnimation].tick = 0; }
			


	}
}
