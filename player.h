#pragma once
#include "template.h"
#include "entity.h"

namespace Tmpl8
{
	class Player : public Entity
	{
	public:
		Player(float xpos, float ypos, SDL_Renderer* ren);
		~Player() { SDL_DestroyTexture(GetTex()); }

		void KeyDown(int key);
		void KeyUp(int key);

		void Update(float deltaTime);
		void calculateKinematic(float deltaTime);

		SDL_Rect getRect() const { return rect; }
		void drawRect();

		void SetFall(bool f) { fall = f; }

	private:
		SDL_Renderer* m_renderer;
		float px, py;
		int idolr, idoll, walkingr, walkingl, jumpl, jumpr, fallingl, fallingr, hit; // all the different animations
		bool l, r, u, d, fall; // left right up down
		SDL_Rect rect;
		vec2 pos;
		vec2 velocity;
		vec2 acceleration;
	};
}