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
		void calculateKinematic(float deltaTime);

		void resetPlayer();

		bool CheckCollision(SDL_Rect* rect, const World* world);
		bool IsOnGround(const World* world);
		

		void SetYPos(float y) { m_Py = y; pos.y = y; m_Rect.y = static_cast<int>(y); }
		void SetXPos(float x) { m_Px = x; pos.x = x; m_Rect.x = static_cast<int>(x) + 9; }

		SDL_Rect getRect() const { return m_Rect; }
		int GetAmountCoins() const { return m_CollectedCoins; }

		void Jump();

	private:
		SDL_Renderer* m_Renderer;
		Audio* m_Audio;
		float m_Px, m_Py;
		float m_XRemainder;
		float m_YRemainder;
		int m_IdolR, m_IdolL, m_WalkingR, m_WalkingL, m_JumpL, m_JumpR, m_FallingL, m_FallingR, m_Hit, m_DoubleJumpL, m_DoubleJumpR, m_HangingL, m_HangingR; // all the different animations
		int m_XDirection; // direction of the player 0 = left 1 = right
		bool m_Left, m_Right, m_Up, m_Down, m_Fall; // Player move directions
		bool m_DoubleJumpAllowed;
		int m_JumpCount = 0;
		int m_MaxJumps = 2;
		int m_CollectedCoins = 0;
		SDL_Rect m_Rect;
		vec2 pos;
		vec2 velocity;
		vec2 acceleration;
	};
}