#pragma once
#include "template.h"
#include "entity.h"
#include "world.h"
#include "sound.h"
#include <vector>

namespace Tmpl8
{
	class Player : public Entity
	{
	public:
		Player(float xpos, float ypos, SDL_Renderer* ren, Audio* audio);
		~Player() { SDL_DestroyTexture(GetTex()); }

		void KeyDown(int key);
		void KeyUp(int key);

		void Update(float deltaTime, World* world);
		void MoveX(float amount, const World* world);
		void MoveY(float amount, const World* world);
		void CheckCoins(World* world);
		float GetChangeY() { return change_y; }
		void calculateKinematic(float deltaTime);

		void resetPlayer();

		std::vector<Object*> GetCollisions(SDL_Rect* rect, const World* world);
		bool CheckCollision(SDL_Rect* rect, const World* world);
		bool IsOnGround(const World* world);
		

		void SetYPos(float y) { py = y; pos.y = y; rect.y = static_cast<int>(y); }
		void SetXPos(float x) { px = x; pos.x = x; rect.x = static_cast<int>(x) + 9; }

		SDL_Rect getRect() const { return rect; }
		SDL_Rect getFallRect() const { return fall_detect; }

		void Jump();

	private:
		SDL_Renderer* m_renderer;
		Audio* m_audio;
		float px, py;
		float xRemainder; // x remainder for the player
		float yRemainder; // y remainder for the player
		float change_y; // change in y
		int idolr, idoll, walkingr, walkingl, jumpl, jumpr, fallingl, fallingr, hit, double_jumpl, doublejumpr, hangingl, hangingr; // all the different animations
		int x_direction; // direction of the player 0 = left 1 = right
		bool l, r, u, d, fall; // left right up down
		bool doubleJumpAllowed;
		int jumpCount = 0;
		int maxJumps = 2;
		int collectedCoins = 0;
		SDL_Rect rect;
		SDL_Rect fall_detect;
		vec2 pos;
		vec2 velocity;
		vec2 acceleration;
	};
}