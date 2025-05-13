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
		
		// Collision bools
		col_top = false;
		col_down = false;
		col_left = false;
		col_right = false;

		// Direction of the player
		x_direction = 1;
		y_direction = 0;
		
		// X and Y remainders
		float xRemainder = 0;
		float yRemainder = 0;

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
	void Player::Update(float deltaTime, const World* world)
	{
		calculateKinematic(deltaTime);

		// Move along X and Y separately for proper collision handling
		MoveX(velocity.x + acceleration.x * 0.5f, world);
		MoveY(velocity.y + acceleration.y * 0.5f, world);

		// Update rect and destination for rendering
		SetDest(pos.x, pos.y, 32 * 2, 32 * 2);
		rect = { static_cast<int>(pos.x) + 9, static_cast<int>(pos.y), (32 * 2) - 20, (32 * 2) - 10 };
		updateAnimation();
	}

	void Player::MoveX(float amount, const World* world)
	{
		xRemainder += amount;
		int move = static_cast<int>(round(xRemainder));
		if (move != 0)
		{
			xRemainder -= move;
			int sign = (move > 0) ? 1 : -1;
			while (move != 0)
			{
				// Update rect for collision check
				rect.x = (pos.x + sign) + 9;
				std::vector<Object*> collisions = GetCollisions(world);
				if (collisions.empty())
				{
					pos.x += sign;
					rect.x = static_cast<int>(pos.x) + 9;
					move -= sign;
				}
				else
				{
					// Stop at collision
					velocity.x = 0;
					break;
				}
			}
		}
	}

	void Player::MoveY(float amount, const World* world)
	{
		yRemainder += amount;
		int move = static_cast<int>(round(yRemainder));
		if (move != 0)
		{
			yRemainder -= move;
			int sign = (move > 0) ? 1 : -1;
			while (move != 0)
			{
				// Update rect for collision check
				rect.y = pos.y + sign;
				std::vector<Object*> collisions = GetCollisions(world);
				if (collisions.empty())
				{
					pos.y += sign;
					move -= sign;
					fall = true;
				}
				else
				{
					// Stop at collision
					velocity.y = 0;
					fall = false;
					break;
				}
			}
		}
	}



	void Player::calculateKinematic(float deltaTime)
	{
		// reset the acceleration
		acceleration = { 0, VERTICAL_ACCALERATION };

		// left movment
		if (l)
		{
			if (getCurAnimation() != walkingl)
			{
				setCurAnimation(walkingl);
			}
			acceleration.x = -HORIZONTAL_ACCALERATION;
		}

		// right movement
		if (r)
		{
			if (getCurAnimation() != walkingr)
			{
				setCurAnimation(walkingr);
			}
			acceleration.x = HORIZONTAL_ACCALERATION;
		}

		// falling animation
		if (fall)
		{
			if (getCurAnimation() != fallingr)
			{
				setCurAnimation(fallingr);
			}
		}

		// idle animations when not falling or moving
		if (!fall)
		{
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
			//acceleration.y = 0;
			velocity.y = 0;
		}

		// Jumping u = up
		if (u)
		{
			Jump();
		}

		// kinematic calculations
		acceleration.x -= velocity.x * HORIZONTAL_FRICTION;
		velocity += acceleration;

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



    std::vector<Object*> Player::GetCollisions(const World* world)  
    {  
       std::vector<Object*> collisions;  
       const auto& map = world->GetMap(); // Get the tile map from the world  
       for (const auto& tile : map)  
       {  
           if (SDL_HasIntersection(&this->rect, &tile.GetDest()))  
           {  
               if (tile.GetSolid())  
               {  
                   collisions.push_back(const_cast<Object*>(&tile));  
               }  
           }  
       }  
       return collisions;  
    }



	//void Player::Jump()
	//{
	//	if (velocity.y == 0 && fall == 0)
	//	{
	//		velocity.y = -VERTICAL_JUMP_SPEED; // jump speed
	//	}
	//}

	void Player::StopJump()
	{
		if (velocity.y >= -7)
		{
			float newVelocityY = velocity.y / 2;
			velocity.y = -newVelocityY;
		}
		else
		{
			velocity.y *= -1;
		}
	}

	void Player::StopMoveX()
	{
		velocity.x = 0;
	}

	bool Player::GoingUp()
	{
		if (velocity.y > 0)
		{
			velocity.y = 0;
			return false;
		}
		else return true;
	}
	bool Player::GoingLeft()
	{
		if (velocity.x < 0)
		{
			return true;
		}
		else return false;
	}
}