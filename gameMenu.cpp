#include "gameMenu.h"
#include "template.h"

namespace Tmpl8
{
    GameMenu::GameMenu(SDL_Renderer* renderer)
        : m_renderer{ renderer }, m_currentMenu{ MenuType::Main }
    {
		// Load sprite sheet
        SDL_Surface* surface = IMG_Load("assets/menu(1024x1024).png");
		m_spriteSheet = SDL_CreateTextureFromSurface(m_renderer, surface);
		SDL_FreeSurface(surface);

        // Load background image
        Background.SetImage("assets/background/Background_2.png", m_renderer, 0);
        Background.SetSource(0, 0, 1280, 720);
        Background.SetDest(0, 0, 1280, 720);

        // Build the menu
        BuildMenu(m_currentMenu);
    }

    void GameMenu::SetMenu(MenuType type)
    {
        m_currentMenu = type;
        BuildMenu(type);
    }

    void GameMenu::BuildMenu(MenuType type)
    {
        m_buttons.clear();
        switch (type)
        {
        case MenuType::Main:
            m_buttons.emplace_back("Levels");
            m_buttons.back().SetDest(ScreenWidth/2 - 514 / 2, 84 + 52, 514, 128);
            m_buttons.back().SetTexture(m_spriteSheet); 
            m_buttons.back().SetSource(0, 0, 514, 128); 

            m_buttons.emplace_back("Settings");
            m_buttons.back().SetDest(ScreenWidth/2 - 514 / 2, 296 , 514, 128);
            m_buttons.back().SetTexture(m_spriteSheet);
            m_buttons.back().SetSource(0, 128, 514, 128);

            m_buttons.emplace_back("Exit");
            m_buttons.back().SetDest(ScreenWidth/2 - 514 / 2, 508 - 52, 514, 128);
            m_buttons.back().SetTexture(m_spriteSheet);
            m_buttons.back().SetSource(0, 256, 514 , 128); 
            break;
		case MenuType::GamePaused:
			m_buttons.emplace_back("Resume");
            m_buttons.back().SetDest(ScreenWidth / 2 - 514 / 2, 84 + 52, 514, 128);
            m_buttons.back().SetTexture(m_spriteSheet);
            m_buttons.back().SetSource(0, 384, 514, 128);

			m_buttons.emplace_back("Restart");
            m_buttons.back().SetDest(ScreenWidth / 2 - 514 / 2, 296, 514, 128);
            m_buttons.back().SetTexture(m_spriteSheet);
            m_buttons.back().SetSource(0, 512, 514, 128);

            m_buttons.emplace_back("Exit");
            m_buttons.back().SetDest(ScreenWidth / 2 - 514 / 2, 508 - 52, 514, 128);
            m_buttons.back().SetTexture(m_spriteSheet);
            m_buttons.back().SetSource(0, 256, 514, 128);
			break;
        case MenuType::Victory:

            m_buttons.emplace_back("Restart");
            m_buttons.back().SetDest(ScreenWidth / 2 - 514 / 2, 296, 514, 128);
            m_buttons.back().SetTexture(m_spriteSheet);
            m_buttons.back().SetSource(0, 512, 514, 128);

            m_buttons.emplace_back("Exit");
            m_buttons.back().SetDest(ScreenWidth / 2 - 514 / 2, 508 - 52, 514, 128);
            m_buttons.back().SetTexture(m_spriteSheet);
            m_buttons.back().SetSource(0, 256, 514, 128);
        }
    }

    void GameMenu::Render()
    {
        SDL_RenderClear(m_renderer);
        
        // draw background
        SDL_Rect dest = Background.GetDest();
        SDL_Rect src = Background.GetSource();
	    SDL_RenderCopy(m_renderer, Background.GetTex(), &src, &dest);

        for (MenuButton& button : m_buttons)
        {
            DrawButton(button);
        }

        SDL_RenderPresent(m_renderer);
    }

    void GameMenu::DrawButton(MenuButton& button)
    {
        // Drawing bigger if hovered else normal
        SDL_Rect dest = button.GetDest();
        if (button.isHovered)
        {
            dest.x -= 5; dest.y -= 5; dest.w += 10; dest.h += 10;
        }
        SDL_RenderCopy(m_renderer, button.GetTex(), &button.GetSource(), &dest);
    }
    void GameMenu::HandleEvent(int MouseX, int MouseY, bool MousePressed, GameState& outGameStateType, bool& backToMenu, bool& outShouldStartGame, bool& outShouldExit, bool& outShouldRestart, Audio* audio, MenuType& outMenuType)
    {  
        for (MenuButton& button : m_buttons)
        {
            SDL_Rect rect = button.GetDest();
            if (MouseX >= rect.x && MouseX <= rect.x + rect.w &&
                MouseY >= rect.y && MouseY <= rect.y + rect.h)
            {
                if (!button.isHovered)
                {
					audio->PlayHoverSound(); // Play hover sfx
					button.isHovered = true;
                }
                if (MousePressed)
                {
                    audio->PlayClickSound(); // Play click sfx

                    // Main menu 
                    if (m_currentMenu == MenuType::Main)
                    {
                        m_currentState = GameState::Paused;
                        if (button.label == "Levels")
                        {
							m_currentState = GameState::Playing;
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
					else if (m_currentMenu == MenuType::GamePaused)
					{
                        m_currentState = GameState::Paused;
                        if (button.label == "Resume")
                        {
                            m_currentState = GameState::Playing;
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
                    else if (m_currentMenu == MenuType::Victory)
                    {
                        m_currentState = GameState::Paused;
                        if (button.label == "Restart")
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