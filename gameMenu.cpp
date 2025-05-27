#include "gameMenu.h"

namespace Tmpl8
{
    GameMenu::GameMenu(SDL_Renderer* renderer)
		: m_Renderer{ renderer }, m_CurrentMenu{ MenuType::Main }, c_ButtonWidth{ 514 }, c_ButtonHeight{ 128 }
    {
		// Load sprite sheet
        SDL_Surface* surface = IMG_Load("assets/menu(1024x1024).png");
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
            m_Buttons.back().SetDest(ButtonLayStd::ButtonX, ButtonLayStd::ButtonOneY, c_ButtonWidth, c_ButtonHeight);
            m_Buttons.back().SetTexture(m_SpriteSheet); 
            m_Buttons.back().SetSource(0, 0, c_ButtonWidth, c_ButtonHeight); 

            m_Buttons.emplace_back("Settings");
            m_Buttons.back().SetDest(ButtonLayStd::ButtonX, ButtonLayStd::ButtonTwoY, c_ButtonWidth, c_ButtonHeight);
            m_Buttons.back().SetTexture(m_SpriteSheet);
            m_Buttons.back().SetSource(0, c_ButtonHeight, c_ButtonWidth, c_ButtonHeight);

            m_Buttons.emplace_back("Exit");
            m_Buttons.back().SetDest(ButtonLayStd::ButtonX, ButtonLayStd::ButtonThreeY, c_ButtonWidth, c_ButtonHeight);
            m_Buttons.back().SetTexture(m_SpriteSheet);
            m_Buttons.back().SetSource(0, c_ButtonHeight * 2, c_ButtonWidth, c_ButtonHeight); 
            break;
		case MenuType::GamePaused:
			m_Buttons.emplace_back("Resume");
            m_Buttons.back().SetDest(ButtonLayStd::ButtonX, ButtonLayStd::ButtonOneY, c_ButtonWidth, c_ButtonHeight);
            m_Buttons.back().SetTexture(m_SpriteSheet);
            m_Buttons.back().SetSource(0, c_ButtonHeight * 3, c_ButtonWidth, c_ButtonHeight);

			m_Buttons.emplace_back("Restart");
            m_Buttons.back().SetDest(ButtonLayStd::ButtonX, ButtonLayStd::ButtonTwoY, c_ButtonWidth, c_ButtonHeight);
            m_Buttons.back().SetTexture(m_SpriteSheet);
            m_Buttons.back().SetSource(0, c_ButtonHeight * 4, c_ButtonWidth, c_ButtonHeight);

            m_Buttons.emplace_back("Exit");
            m_Buttons.back().SetDest(ButtonLayStd::ButtonX, ButtonLayStd::ButtonThreeY, c_ButtonWidth, c_ButtonHeight);
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