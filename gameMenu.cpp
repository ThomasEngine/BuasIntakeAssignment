#include "gameMenu.h"

namespace Tmpl8
{
    GameMenu::GameMenu(SDL_Renderer* renderer)
		: m_Renderer{ renderer }, m_CurrentMenu{ MenuType::Main }
    {
		// Load sprite sheet
        SDL_Surface* surface = IMG_Load("assets/Menu/menu(1024x1024).png");
		m_SpriteSheet = SDL_CreateTextureFromSurface(m_Renderer, surface);
		SDL_FreeSurface(surface);

        // Load background image
        m_Background.SetImage("assets/background/Background_2.png", m_Renderer, 0);
        m_Background.SetSource(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
        m_Background.SetDest(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);

        // Build the menu
        BuildMenu(m_CurrentMenu);
    }

    void GameMenu::SetMenu(MenuType type)
    {
        m_CurrentMenu = type;
        BuildMenu(type);
    }

    void GameMenu::BuildMenu(MenuType type)
    {
		// Clear previous buttons
        m_Buttons.clear();

        // Creating new buttons
        switch (type)
        {
        case MenuType::Main:
            m_Buttons.emplace_back("Levels");
            m_Buttons.back().SetDest(ButtonX, ButtonOneY, c_ButtonWidth, c_ButtonHeight);
            m_Buttons.back().SetTexture(m_SpriteSheet); 
            m_Buttons.back().SetSource(0, 0, c_ButtonWidth, c_ButtonHeight); 

            m_Buttons.emplace_back("Settings");
            m_Buttons.back().SetDest(ButtonX, ButtonTwoY, c_ButtonWidth, c_ButtonHeight);
            m_Buttons.back().SetTexture(m_SpriteSheet);
            m_Buttons.back().SetSource(0, c_ButtonHeight, c_ButtonWidth, c_ButtonHeight);

            m_Buttons.emplace_back("Exit");
            m_Buttons.back().SetDest(ButtonX, ButtonThreeY, c_ButtonWidth, c_ButtonHeight);
            m_Buttons.back().SetTexture(m_SpriteSheet);
            m_Buttons.back().SetSource(0, c_ButtonHeight * 2, c_ButtonWidth, c_ButtonHeight); 
            break;
		case MenuType::GamePaused:
			m_Buttons.emplace_back("Resume");
            m_Buttons.back().SetDest(ButtonX, ButtonOneY, c_ButtonWidth, c_ButtonHeight);
            m_Buttons.back().SetTexture(m_SpriteSheet);
            m_Buttons.back().SetSource(0, c_ButtonHeight * 3, c_ButtonWidth, c_ButtonHeight);

			m_Buttons.emplace_back("Restart");
            m_Buttons.back().SetDest(ButtonX, ButtonTwoY, c_ButtonWidth, c_ButtonHeight);
            m_Buttons.back().SetTexture(m_SpriteSheet);
            m_Buttons.back().SetSource(0, c_ButtonHeight * 4, c_ButtonWidth, c_ButtonHeight);

            m_Buttons.emplace_back("Exit");
            m_Buttons.back().SetDest(ButtonX, ButtonThreeY, c_ButtonWidth, c_ButtonHeight);
            m_Buttons.back().SetTexture(m_SpriteSheet);
            m_Buttons.back().SetSource(0, c_ButtonHeight * 2, c_ButtonWidth, c_ButtonHeight);
			break;
        case MenuType::Victory:
            m_Buttons.emplace_back("Victory");
            m_Buttons.back().SetDest(ButtonX, ButtonOneY, c_ButtonWidth, c_ButtonHeight);
            m_Buttons.back().SetTexture(m_SpriteSheet);
            m_Buttons.back().SetSource(0, c_ButtonHeight * 6, c_ButtonWidth, c_ButtonHeight);

            m_Buttons.emplace_back("Restart");
            m_Buttons.back().SetDest(ButtonX, ButtonTwoY, c_ButtonWidth, c_ButtonHeight);
            m_Buttons.back().SetTexture(m_SpriteSheet);
            m_Buttons.back().SetSource(0, c_ButtonHeight * 4, c_ButtonWidth, c_ButtonHeight);

            m_Buttons.emplace_back("Exit");
            m_Buttons.back().SetDest(ButtonX, ButtonThreeY, c_ButtonWidth, c_ButtonHeight);
            m_Buttons.back().SetTexture(m_SpriteSheet);
            m_Buttons.back().SetSource(0, c_ButtonHeight * 2, c_ButtonWidth, c_ButtonHeight);
        }
    }

    void GameMenu::BuildWorldMap()  
    {  
       m_WorldLevels.clear();  
       // Add levels to world levels  
       m_WorldLevels.emplace_back(WorldLevel{ {200, 400}, true, "Level 1" });
       m_WorldLevels.emplace_back(WorldLevel{ {200, 400}, false, "Level 2" });
       m_WorldLevels.emplace_back(WorldLevel{ {200, 400}, false, "Level 2" });
       m_Selector.currentLevelIndex = 0;
       m_Selector.position = m_WorldLevels[0].position;
    }

    void GameMenu::Render()
    {
        SDL_RenderClear(m_Renderer);
        
        // draw background
        SDL_Rect dest = m_Background.GetDest();
        SDL_Rect src = m_Background.GetSource();
	    SDL_RenderCopy(m_Renderer, m_Background.GetTex(), &src, &dest);

        for (MenuButton& button : m_Buttons)
        {
            DrawButton(button);
        }

        SDL_RenderPresent(m_Renderer);
    }

    void GameMenu::DrawButton(MenuButton& button)
    {
        // Drawing bigger if hovered else normal
        SDL_Rect dest = button.GetDest();
        if (button.isHovered)
        {
            dest.x -= 5; dest.y -= 5; dest.w += 10; dest.h += 10;
        }
        SDL_RenderCopy(m_Renderer, button.GetTex(), &button.GetSource(), &dest);
    }
    void GameMenu::DrawWorldMap()
    {
        // Draw levels
        for (const WorldLevel& level : m_WorldLevels)
        {
            SDL_Rect levelRect = { level.position.x - 16, level.position.y - 16, 32 ,32 };
            SDL_SetRenderDrawColor(m_Renderer, level.unlocked ? 0 : 128, 200, 0, 255);  // different color if unlocked
            SDL_RenderFillRect(m_Renderer, &levelRect);
        }
		// Draw selector
		SDL_Rect selRect = { m_Selector.position.x - 20, m_Selector.position.y - 32, 40, 10 };
		SDL_SetRenderDrawColor(m_Renderer, 255, 0, 0, 255);  // red rectangle for selector
		SDL_RenderFillRect(m_Renderer, &selRect);
    }

    void GameMenu::HandleEvent(int MouseX, int MouseY, bool MousePressed, GameState& outGameStateType, bool& backToMenu, bool& outShouldStartGame, bool& outShouldExit, bool& outShouldRestart, Audio* audio, MenuType& outMenuType)
    {  
        for (MenuButton& button : m_Buttons)
        {
            SDL_Rect rect = button.GetDest();
            if (MouseX >= rect.x && MouseX <= rect.x + rect.w &&
                MouseY >= rect.y && MouseY <= rect.y + rect.h)
            {
                if (!button.isHovered) // tried && m_CurrentState == Paused but fix the full problem, will debug this later
                {
					audio->PlayHoverSound(); // Play hover sfx
					button.isHovered = true;
                }
                if (MousePressed)
                {
                    audio->PlayClickSound(); // Play click sfx

                    // Main menu 
                    if (m_CurrentMenu == MenuType::Main)
                    {
                        m_CurrentState = GameState::Paused;
                        if (button.label == "Levels")
                        {
							m_CurrentState = GameState::Playing;
                            outGameStateType = GameState::Playing;
                            outShouldStartGame = true;
                        }
                        else if (button.label == "Settings")
                        {
                            outGameStateType = GameState::Paused;
                        }
                        else if (button.label == "Exit")
                            outShouldExit = true;
                    }
                    //else if (m_CurrentMenu == MenuType::WorldMap)
                    //{
                    //    // Example for keyboard input (add to your event handling)
                    //    if (event.type == SDL_KEYDOWN) {
                    //        if (event.key.keysym.sym == SDLK_RIGHT && m_Selector.currentLevelIndex < m_WorldLevels.size() - 1) {
                    //            m_Selector.currentLevelIndex++;
                    //            m_Selector.position = m_WorldLevels[m_Selector.currentLevelIndex].position;
                    //        }
                    //        else if (event.key.keysym.sym == SDLK_LEFT && m_Selector.currentLevelIndex > 0) {
                    //            m_Selector.currentLevelIndex--;
                    //            m_Selector.position = m_WorldLevels[m_Selector.currentLevelIndex].position;
                    //        }
                    //        else if (event.key.keysym.sym == SDLK_RETURN || event.key.keysym.sym == SDLK_SPACE) {
                    //            if (m_WorldLevels[m_Selector.currentLevelIndex].unlocked) {
                    //                // Start the selected level
                    //                outShouldStartGame = true;
                    //                // Set game state, etc.
                    //            }
                    //        }
                    //    }
                    //}
					// Pause menu
					else if (m_CurrentMenu == MenuType::GamePaused)
					{
                        m_CurrentState = GameState::Paused;
                        if (button.label == "Resume")
                        {
                            m_CurrentState = GameState::Playing;
                            outShouldStartGame = true;
                        }
                        else if (button.label == "Restart")
                        {
                            outShouldRestart = true;
                            outGameStateType = GameState::Playing;
                        }
                        else if (button.label == "Exit")
                        {  
                            outShouldExit = true;
                        }
					}
                    // Victory Menu
                    else if (m_CurrentMenu == MenuType::Victory)
                    {
                        m_CurrentState = GameState::Paused;
                        if (button.label == "Restart" || button.label == "Victory")
                        {
                            outShouldRestart = true;
                            outGameStateType = GameState::Playing;
                        }
                        else if (button.label == "Exit")
                        {
                            outShouldExit = true;
                        }
                    }
                }
            }
			else button.isHovered = false;
        }
    }
}