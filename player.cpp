#include "player.h"

namespace Tmpl8
{

	Player::Player(float xpos, float ypos, SDL_Renderer* ren, Audio* audio)
	{
		// Player position
		m_Px = xpos;
		m_Py = ypos;

		// Kinematic vectors
		pos = { m_Px, m_Py };
		velocity = { 0, 0 };
		acceleration = { 0, VERTICAL_ACCALERATION }; // 0 is the beginning of the x accaleration, which at the start of the game is 0

		// Movement left, right, up, down, falling
		m_Left = false;
		m_Right = false;
		m_Up = false;
		m_Down = false;
		m_Fall = false;

		// Direction of the player
		m_XDirection = 1;

		// X and Y remainders
		float xRemainder = 0;
		float yRemainder = 0;


		//hitbox rect
		m_Rect = { static_cast<int>(m_Px) + 9, static_cast<int>(m_Py), (32 * 2) - 20, (32 * 2) - 10 };

		// Hitbox to detect player from falling
		fall_detect = { static_cast<int>(m_Px) + 9, static_cast<int>(m_Py), (32 * 2) - 25, (32 * 2) - 50 };

		m_Renderer = ren; // renderer from game
		m_Audio = audio; // Audio from game


		// Setting up all the sprite animations of the player
		SetImage("assets/frog.png", m_Renderer, 0);
		SetDest(m_Px, m_Py, 32 * 2, 32 * 2);
		m_IdolR = createCycle(0, 32, 32, 11, 3);
		m_IdolL = createCycle(7, 32, 32, 11, 3);

		m_WalkingR = createCycle(1, 32, 32, 12, 2);
		m_WalkingL = createCycle(2, 32, 32, 12, 2);

		m_JumpL = createCycle(3, 32, 32, 1, 0);
		m_JumpR = createCycle(10, 32, 32, 1, 0);

		m_FallingL = createCycle(9, 32, 32, 1, 0);
		m_FallingR = createCycle(8, 32, 32, 1, 0);

		// Extry animations if for when I continue developing
		m_DoubleJumpL = createCycle(12, 32, 32, 6, 4);
		m_DoubleJumpR = createCycle(4, 32, 32, 6, 4);

		m_HangingL = createCycle(6, 32, 32, 1, 0);
		m_HangingR = createCycle(11, 32, 32, 1, 0);

		setCurAnimation(m_IdolL); // starting animation

	}

	void Player::KeyDown(int key)  // When a key is pressed
	{
		switch (key)
		{
		case SDLK_LEFT:
		case SDLK_a:
			m_XDirection = 0;
			m_Left = true;
			m_Right = false;
			break;
		case SDLK_RIGHT:
		case SDLK_d:
			m_XDirection = 1;
			m_Right = true;
			m_Left = false;
			break;
		case SDLK_UP:
		case SDLK_SPACE:
		case SDLK_w:
			m_Up = true;
			m_Down = false;
			break;
		case SDLK_DOWN:
		case SDLK_s:
			m_Down = true;
			m_Up = false;
			break;
		case SDLK_0:
			resetPlayer();
			break;
		case SDLK_9:
			std::cout << m_Px << ", " << m_Py << std::endl;
			m_Px = 157;
			m_Py = 2090;
			pos = { m_Px, m_Py };
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
			m_Left = false;
			setCurAnimation(m_IdolL);
			break;
		case SDLK_RIGHT:
		case SDLK_d:
			m_Right = false;
			setCurAnimation(m_IdolR);
			break;
		case SDLK_UP:
		case SDLK_SPACE:
		case SDLK_w:
			m_Up = false;
			m_DoubleJumpAllowed = true; // Allow jump again
			break;
		case SDLK_DOWN:
		case SDLK_s:
			m_Down = false;
			break;
		default:
			break;
		}
	}

	void Player::Update(float deltaTime, World* world)
	{
		calculateKinematic(deltaTime);

		float tempY = m_Py; // Store the current Y position before moving
		// Move along X and Y separately + collision detection
		MoveX(velocity.x * deltaTime + acceleration.x * 0.5f, world);
		MoveY(velocity.y * deltaTime + acceleration.y * 0.5f, world);

		CheckCoins(world); // Check for coin collision
		
		m_YChange = m_Py - tempY;

		if (IsOnGround(world)) {
			m_JumpCount = 0;
		}

		// Update rect and destination for rendering
		SetDest(pos.x, pos.y, 32 * 2, 32 * 2);
		m_Rect = { static_cast<int>(pos.x) + 9, static_cast<int>(pos.y), (32 * 2) - 20, (32 * 2) - 10 };
		fall_detect = { static_cast<int>(pos.x) + 12, static_cast<int>(pos.y) + 40 , (32 * 2) - 26, (32 * 2) - 50 };

		updateAnimation();
	}

	void Player::MoveX(float amount, const World* world)
	{
		m_XRemainder += amount;
		int move = static_cast<int>(round(m_XRemainder));
		if (move != 0)
		{
			m_XRemainder -= move;
			int sign = (move > 0) ? 1 : -1;
			while (move != 0)
			{
				//Update rect for collision check
				m_Rect.x = (pos.x + sign) + 9;
				fall_detect.x = m_Rect.x + 12;

				if (!CheckCollision(&m_Rect, world))
				{
					pos.x += sign;
					m_Rect.x = static_cast<int>(pos.x) + 9;
					fall_detect.x = m_Rect.x + 12;
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
		m_YRemainder += amount;
		int move = static_cast<int>(round(m_YRemainder));
		if (move != 0)
		{
			m_YRemainder -= move;
			int sign = (move > 0) ? 1 : -1;
			while (move != 0)
			{
				// Update rect for collision check
				m_Rect.y = pos.y + sign;
				fall_detect.y = m_Rect.y;
				if (!CheckCollision(&m_Rect, world))
				{
					pos.y += sign;
					m_Rect.y = static_cast<int>(pos.y);
					fall_detect.y = m_Rect.y;
					move -= sign;
					m_Fall = true;
				}
				else
				{
					// Only set velocity.y = 0 if moving down (hitting ground)
					if (sign > 0)
					{
						velocity.y = 0;
						m_Fall = false;
					}
					// If moving up (hitting ceiling), also stop upward movement
					else if (sign < 0)
					{
						velocity.y = 0;
						m_Fall = true;
					}
					break;
				}
			}
		}
	}

    void Player::CheckCoins(World* world)
    {  
       SDL_Rect playerRect = getRect();  
       auto& coins = world->GetCoins(); // Get the coins from the world  
       for (auto coin_it = coins.begin(); coin_it != coins.end(); )  // Using an it so I can erase coin from vector
       {  
           if (SDL_HasIntersection(&playerRect, &coin_it->GetDest()))  
           {  
               // Coin collected
               m_CollectedCoins++; // Add coin to total coins
               coin_it = coins.erase(coin_it); // Remove the coin from the vector  
			   m_Audio->PlayCoinSound();
               std::cout << "Collected coins: " << m_CollectedCoins << std::endl; // Print the number of collected coins
           }  
           else  
           {  
               ++coin_it; // next coin
           }  
       }  
    }

	void Player::calculateKinematic(float deltaTime)
	{
		// reset the acceleration
		acceleration = { 0, VERTICAL_ACCALERATION };

		// left movment
		if (m_Left)
		{
			if (getCurAnimation() != m_WalkingL)
			{
				setCurAnimation(m_WalkingL);
			}
			acceleration.x = -HORIZONTAL_ACCALERATION;
		}

		// right movement
		if (m_Right)
		{
			if (getCurAnimation() != m_WalkingR)
			{
				setCurAnimation(m_WalkingR);
			}
			acceleration.x = HORIZONTAL_ACCALERATION;
		}
		
		if (velocity.y < 0) // if the player is moving up
		{
			m_Fall = false;
			if (m_XDirection == 0) // if direction is left
			{
				if (getCurAnimation() != m_JumpL)
				{
					setCurAnimation(m_JumpL);
				}
			}	
			if (m_XDirection == 1) // if direction is right
			{
				if (getCurAnimation() != m_JumpR)
				{
					setCurAnimation(m_JumpR);
				}
			}
		}

		// falling animation
		if (m_Fall)
		{
			if (m_XDirection == 0) // if direction is left
			{
				if (getCurAnimation() != m_FallingL)
				{
					setCurAnimation(m_FallingL);
				}
			}
			if (m_XDirection == 1) // if direction is right
			{
				if (getCurAnimation() != m_FallingR)
				{
					setCurAnimation(m_FallingR);
				}
			}
		}

		// idle animations when not falling or moving
		if (!m_Fall)
		{

			if (m_XDirection == 0) // if direction is left
			{
				if (getCurAnimation() != m_IdolL && getCurAnimation() != m_WalkingL)
				{
					setCurAnimation(m_IdolL);
				}
			}
			if (m_XDirection == 1) // if direction is right
			{
				if (getCurAnimation() != m_IdolR && getCurAnimation() != m_WalkingR)
				{
					setCurAnimation(m_IdolR);
				}
			}
		}

		// Jumping u = up
		if (m_Up)
		{
			Jump();
			m_DoubleJumpAllowed = false;
		}

		// kinematic calculations
		acceleration.x -= velocity.x * HORIZONTAL_FRICTION;
		velocity += acceleration;

		// Clamp vertical velocity (fall speed)
		if (velocity.y > MAX_FALL_SPEED)
			velocity.y = MAX_FALL_SPEED;


		// setting new player position
		m_Px = pos.x;
		m_Py = pos.y;
	}

	void Player::resetPlayer()
	{
		// Reset player position
		m_Px = 64;
		m_Py = 95 * 31;
		// Reset kinematic vectors
		pos = { m_Px, m_Py };
		velocity = { 0, 0 };
		acceleration = { 0, VERTICAL_ACCALERATION };
		m_Left = false;
		m_Right = false;
		m_Up = false;
		m_Down = false;
		m_Fall = false;
	}

	void Player::Jump()
	{
		if (m_Fall && m_JumpCount == 0)
		{
			m_JumpCount++; // only allow 1 jump
		}
		
		if (m_JumpCount < m_MaxJumps && m_DoubleJumpAllowed)
		{
			velocity.y = -VERTICAL_JUMP_SPEED;
			m_JumpCount++;
			std::cout << m_JumpCount << std::endl;
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
		SDL_Rect groundCheck = m_Rect;
		groundCheck.w -= 6;
		groundCheck.x += 3;
		groundCheck.y += m_Rect.h; // Move to just below the player
		groundCheck.h = 1;       // Only 1 pixel tall

		return CheckCollision(&groundCheck, world);
	}
};