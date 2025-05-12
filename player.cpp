#include "player.h"

namespace Tmpl8
{

	Player::Player(float xpos, float ypos, SDL_Renderer* ren)
		: Entity()
		
	{
		// Player position
		px = xpos;
		py = ypos;

		// Kinematic vectors
		pos = { px, py };
		velocity = { 0, 0 };
		acceleration = { 0, 0 };

		// Movement left, right, up, down, falling
		l = false;
		r = false;
		u = false;
		d = false;
		fall = false;


		m_renderer = ren; // renderer from game


		// Setting up all the sprite animations of the player
		SetImage("assets/frog.png", m_renderer, 0);
		SetDest(px, py, 32 * 2, 32 * 2);
		idolr = createCycle(0, 32, 32, 11, 3);
		idoll = createCycle(7, 32, 32, 11, 3);

		walkingr = createCycle(1, 32, 32, 12, 2);
		walkingl = createCycle(2, 32, 32, 12, 2);

		jumpl = createCycle(3, 32, 32, 1, 0);
		jumpr = createCycle(3, 32, 32, 1, 0);

		fallingl = createCycle(5, 32, 32, 7, 6);
		fallingr = createCycle(3, 32, 32, 1, 0);

		setCurAnimation(idoll); // starting animation

		// Settings player stats
		SetHealth(100);
		SetMaxHealth(100);
	}
    void Player::KeyDown(int key)  // When a key is pressed
    {  
       switch (key)  
       {  
       case SDLK_LEFT:  
       case SDLK_a:  
           l = true;  
           r = false;
           break;  
       case SDLK_RIGHT:  
       case SDLK_d:  
           r = true;  
           l = false; 
           break;  
       case SDLK_UP:  
       case SDLK_w:  
           u = true;  
           d = false;  
           break;  
       case SDLK_DOWN:  
       case SDLK_s:  
           d = true;  
           u = false;  
           break;  
       default:  
           break;  
       }  
    }
	void Player::KeyUp(int key) // When a key is released
	{
		switch (key)
		{
		case SDLK_LEFT:
		case SDLK_a:
			l = false;
			setCurAnimation(idoll);
			break;
		case SDLK_RIGHT:
		case SDLK_d:
			r = false;
			setCurAnimation(idolr);
			break;
		case SDLK_UP:
		case SDLK_w:
			u = false;
			break;
		case SDLK_DOWN:
		case SDLK_s:
			d = false;
			break;
		default:
			break;
		}
	}
	void Player::Update(float deltaTime)
	{
		acceleration = { 0, 0 }; // reset the acceleration
		calculateKinematic(deltaTime); // x movement
		SetDest(px, py, 32 * 2, 32 * 2); // setting the destination of the player
		updateAnimation();
	}

	void Player::calculateKinematic(float deltaTime)
	{
		if (l)
		{
			if (getCurAnimation() != walkingl)
			{
				setCurAnimation(walkingl);
			}
			acceleration.x = -1 * HORIZONTAL_ACCALERATION;
		}
		if (r)
		{
			if (getCurAnimation() != walkingr)
			{
				setCurAnimation(walkingr);
			}
			acceleration.x = 1 * HORIZONTAL_ACCALERATION;
		}
		// kinematic calculations
		acceleration.x -= velocity.x * HORIZONTAL_FRICTION;
		velocity += acceleration;
		pos += velocity + acceleration * 0.5;

		// setting now player position
		px = pos.x;
		py = pos.y;
	}
	void Player::drawRect() // draws hitbox to the player. For debuging
	{
		rect = { static_cast<int>(px) + 9, static_cast<int>(py), (32 * 2)-20, (32 * 2)-10};
		SDL_SetRenderDrawColor(m_renderer, 255, 0, 0, 255); // Red
		SDL_RenderDrawRect(m_renderer, &rect);
	}
}