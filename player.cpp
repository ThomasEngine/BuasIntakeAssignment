#include "player.h"

namespace Tmpl8
{

	Player::Player(float xpos, float ypos, SDL_Renderer* ren)
	{
		// Player position
		px = xpos;
		py = ypos;

		// Kinematic vectors
		pos = { px, py };
		velocity = { 0, 0 };
		acceleration = { 0, VERTICAL_ACCALERATION }; // 0 is the beginning of the x accaleration, which at the start of the game is 0

		// Movement left, right, up, down, falling
		l = false;
		r = false;
		u = false;
		d = false;
		fall = false;

		// Direction of the player
		x_direction = 1;
		y_direction = 0;
		
		//hitbox rect
		rect = { static_cast<int>(px) + 9, static_cast<int>(py), (32 * 2) - 20, (32 * 2) - 10 };

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
		   x_direction = 0;
           l = true;  
           r = false;
           break;  
       case SDLK_RIGHT:  
       case SDLK_d:  
		   x_direction = 1;
           r = true;  
           l = false; 
           break;  
       case SDLK_UP:
	   case SDLK_SPACE:
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
		case SDLK_SPACE:
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
		acceleration = { 0, VERTICAL_ACCALERATION }; // reset the acceleration
		calculateKinematic(deltaTime); // x movement
		SetDest(px, py, 32 * 2, 32 * 2); // setting the destination of the player
		rect = { static_cast<int>(px) + 9, static_cast<int>(py), (32 * 2) - 20, (32 * 2) - 10 }; // setting the hitbox
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
			acceleration.x = -HORIZONTAL_ACCALERATION;
		}
		if (r)
		{
			if (getCurAnimation() != walkingr)
			{
				setCurAnimation(walkingr);
			}
			acceleration.x = HORIZONTAL_ACCALERATION;
		}
		if (fall)
		{
			if (getCurAnimation() != fallingr)
			{
				setCurAnimation(fallingr);
			}
		}
		if (!fall)
		{
			// These if statements is that when it is done falling it does not stay the falling animation
			if (x_direction == 0) // if direction is left
			{
				if (getCurAnimation() != idoll && getCurAnimation() != walkingl)
				{
					setCurAnimation(idoll);
				}
			}
			if (x_direction == 1) // if direction is right
			{
				if (getCurAnimation() != idolr && getCurAnimation() != walkingr)
				{
					setCurAnimation(idolr);
				}
			}
			acceleration.y = 0;
			velocity.y = 0;
		}
		if (u)
		{
			Jump();
		}
		// kinematic calculations
		acceleration.x -= velocity.x * HORIZONTAL_FRICTION;
		velocity += acceleration;
		pos += velocity + acceleration * 0.5;

		// setting now player position
		px = pos.x;
		py = pos.y;
	}
	void Player::Jump()
	{
		if (!fall)
		{
			velocity.y = -VERTICAL_JUMP_SPEED; // jump speed
		}
	}
}