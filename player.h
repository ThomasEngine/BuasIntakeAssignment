#pragma once
#include "template.h"
#include "entity.h"
#include "world.h"
#include <vector>

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
		void Render();
		void calculateKinematic(float deltaTime);
		void Move(const World* world);
		std::vector<Object*> GetCollisions(const World* world);
		

		void Jump();
		void StopJump();
		void StopMoveX();
		void SetPos(float x, float y) { px = x; py = y; pos.x = x; pos.y = y; rect = {static_cast<int>(x) + 9, static_cast<int>(y), (32 * 2) - 20, (32 * 2) - 10 };
		}
		void SetYPos(float y) { py = y; pos.y = y; rect.y = static_cast<int>(y); }

		SDL_Rect getRect() const { return rect; }
		bool GoingUp();
		bool GoingLeft();


		void SetFall(bool f) { fall = f; }
		void SetVelocityX(int vx) { velocity.x = vx; }

	private:
		SDL_Renderer* m_renderer;
		float px, py;
		int idolr, idoll, walkingr, walkingl, jumpl, jumpr, fallingl, fallingr, hit; // all the different animations
		int x_direction; // direction of the player 0 = left 1 = right
		int y_direction; // direction of the player 0 = down 1 = up
		bool l, r, u, d, fall; // left right up down
		bool col_top, col_down, col_left, col_right;
		SDL_Rect rect;
		vec2 pos;
		vec2 velocity;
		vec2 acceleration;
	};
}