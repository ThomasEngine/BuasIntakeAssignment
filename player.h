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
		void Jump();
		void SetPos(float x, float y) { px = x; py = y; pos.x = x; pos.y = y; rect = {static_cast<int>(px) + 9, static_cast<int>(py), (32 * 2) - 20, (32 * 2) - 10 };
		}

		SDL_Rect getRect() const { return rect; }

		void SetFall(bool f) { fall = f; }

	private:
		SDL_Renderer* m_renderer;
		float px, py;
		int idolr, idoll, walkingr, walkingl, jumpl, jumpr, fallingl, fallingr, hit; // all the different animations
		bool l, r, u, d, fall; // left right up down
		int x_direction; // direction of the player 0 = left 1 = right
		int y_direction; // direction of the player 0 = down 1 = up
		SDL_Rect rect;
		vec2 pos;
		vec2 velocity;
		vec2 acceleration;
	};
}