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

		// X and Y remainders
		float xRemainder = 0;
		float yRemainder = 0;


		//hitbox rect
		rect = { static_cast<int>(px) + 9, static_cast<int>(py), (32 * 2) - 20, (32 * 2) - 10 };

		// Hitbox to detect player from falling
		fall_detect = { static_cast<int>(px) + 9, static_cast<int>(py), (32 * 2) - 25, (32 * 2) - 50 };

		m_renderer = ren; // renderer from game


		// Setting up all the sprite animations of the player
		SetImage("assets/frog.png", m_renderer, 0);
		SetDest(px, py, 32 * 2, 32 * 2);
		idolr = createCycle(0, 32, 32, 11, 3);
		idoll = createCycle(7, 32, 32, 11, 3);

		walkingr = createCycle(1, 32, 32, 12, 2);
		walkingl = createCycle(2, 32, 32, 12, 2);

		jumpl = createCycle(3, 32, 32, 1, 0);
		jumpr = createCycle(10, 32, 32, 1, 0);

		fallingl = createCycle(9, 32, 32, 1, 0);
		fallingr = createCycle(8, 32, 32, 1, 0);

		double_jumpl = createCycle(12, 32, 32, 6, 4);
		doublejumpr = createCycle(4, 32, 32, 6, 4);

		hangingl = createCycle(6, 32, 32, 1, 0);
		hangingr = createCycle(11, 32, 32, 1, 0);

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

		float tempY = pos.y; // Store the current Y position before moving
		// Move along X and Y separately + collision detection
		MoveX(velocity.x + acceleration.x * 0.5f, world);
		MoveY(velocity.y + acceleration.y * 0.5f, world);

		change_y = pos.y - tempY;


		// Update rect and destination for rendering
		SetDest(pos.x, pos.y, 32 * 2, 32 * 2);
		rect = { static_cast<int>(pos.x) + 9, static_cast<int>(pos.y), (32 * 2) - 20, (32 * 2) - 10 };
		fall_detect = { static_cast<int>(pos.x) + 12, static_cast<int>(pos.y) + 40 , (32 * 2) - 26, (32 * 2) - 50 };

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
				//Update rect for collision check
				rect.x = (pos.x + sign) + 9;
				fall_detect.x = rect.x + 12;

				if (!CheckCollision(&rect, world))
				{
					pos.x += sign;
					rect.x = static_cast<int>(pos.x) + 9;
					fall_detect.x = rect.x + 12;
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
				fall_detect.y = rect.y;
				if (!CheckCollision(&rect, world))
				{
					pos.y += sign;
					rect.y = static_cast<int>(pos.y);
					fall_detect.y = rect.y;
					move -= sign;
					fall = true;
				}
				else
				{
					// Only set velocity.y = 0 if moving down (hitting ground)
					if (sign > 0)
					{
						velocity.y = 0;
						fall = false;
					}
					// If moving up (hitting ceiling), also stop upward movement
					else if (sign < 0)
					{
						velocity.y = 0;
						fall = true;
					}
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
		
		if (velocity.y < 0) // if the player is moving up
		{
			//fall = false; // set fall to false
			if (x_direction == 0) // if direction is left
			{
				if (getCurAnimation() != jumpl)
				{
					setCurAnimation(jumpl);
				}
			}
			if (x_direction == 1) // if direction is right
			{
				if (getCurAnimation() != jumpr)
				{
					setCurAnimation(jumpr);
				}
			}
		}

		// falling animation
		if (fall)
		{
			if (x_direction == 0) // if direction is left
			{
				if (getCurAnimation() != fallingl)
				{
					setCurAnimation(fallingl);
				}
			}
			if (x_direction == 1) // if direction is right
			{
				if (getCurAnimation() != fallingr)
				{
					setCurAnimation(fallingr);
				}
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
		}

		// Jumping u = up
		if (u)
		{
			Jump();
		}

		// kinematic calculations
		acceleration.x -= velocity.x * HORIZONTAL_FRICTION;
		velocity += acceleration;

		// setting new player position
		px = pos.x;
		py = pos.y;
	}

	void Player::Jump() // If player is allowed to jump. Jump.
	{
		if (!fall)
		{
			velocity.y = -VERTICAL_JUMP_SPEED; // jump speed
		}
	}

	std::vector<Object*> Player::GetCollisions(SDL_Rect* rect, const World* world)
	{
		std::vector<Object*> collisions;
		const auto& map = world->GetMap(); // Get the tile map from the world  
		for (const auto& tile : map)
		{
			if (SDL_HasIntersection(rect, &tile.GetDest()))
			{
				if (tile.GetSolid())
				{
					collisions.push_back(const_cast<Object*>(&tile));
				}
			}
		}
		return collisions;
	}
	bool Player::CheckCollision(SDL_Rect* rect, const World* world)
	{
		const auto& map = world->GetMap(); // Get the tile map from the world
		for (const auto& tile : map)
		{
			if (SDL_HasIntersection(rect, &tile.GetDest()))
			{
				if (tile.GetSolid())
				{
					return true;
				}
			}
		}
		return false;
	}

	bool Player::IsOnGround(const World* world)
	{
		// Create a small rectangle just below the player's feet
		SDL_Rect groundCheck = rect;
		groundCheck.w -= 6;
		groundCheck.x += 3;
		groundCheck.y += rect.h; // Move to just below the player
		groundCheck.h = 1;       // Only 1 pixel tall

		return CheckCollision(&groundCheck, world);
	}
};