#include "gameMenu.h"
#include "template.h"

namespace Tmpl8
{
    GameMenu::GameMenu(SDL_Renderer* renderer)
        : m_renderer{ renderer }, m_currentMenu{ MenuType::Main }
    {
		// Load sprite sheet
        SDL_Surface* surface = IMG_Load("assets/menu(514x384).png");
		m_spriteSheet = SDL_CreateTextureFromSurface(m_renderer, surface);
		SDL_FreeSurface(surface);


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
		case MenuType::Levels:
			m_buttons.emplace_back("Level 1");
			m_buttons.back().SetDest(100, 100, 200, 50);
			m_buttons.back().SetTexture(m_spriteSheet);
			m_buttons.back().SetSource(0, 0, 600, 200);

			m_buttons.emplace_back("Level 2");
			m_buttons.back().SetDest(100, 200, 200, 50);
			m_buttons.back().SetTexture(m_spriteSheet);
			m_buttons.back().SetSource(609, 417, 200, 50);

			m_buttons.emplace_back("Back");
			m_buttons.back().SetDest(100, 300, 200, 50);
			m_buttons.back().SetTexture(m_spriteSheet);
			m_buttons.back().SetSource(0, 100, 200, 50);
			break;
		case MenuType::Pause:
			m_buttons.emplace_back("Resume");
			m_buttons.back().SetDest(100, 100, 200, 50);
			m_buttons.back().SetTexture(m_spriteSheet);
			m_buttons.back().SetSource(0, 0, 600, 200);

			m_buttons.emplace_back("Restart");
			m_buttons.back().SetDest(100, 200, 200, 50);
			m_buttons.back().SetTexture(m_spriteSheet);
			m_buttons.back().SetSource(609, 417, 200, 50);

			m_buttons.emplace_back("Exit");
			m_buttons.back().SetDest(100, 300, 200, 50);
			m_buttons.back().SetTexture(m_spriteSheet);
			m_buttons.back().SetSource(1216, 625, 200, 50);
			break;
        }
    }

    void GameMenu::Render()
    {
        //SDL_SetRenderDrawColor(m_renderer, 66, 224, 245, 255);
        SDL_RenderClear(m_renderer);
        
        // draw background
        SDL_Rect dest = Background.GetDest();
        SDL_Rect src = Background.GetSource();
        SDL_RenderCopyEx(m_renderer, Background.GetTex(), &src, &dest, 0, NULL, SDL_FLIP_NONE);

        for (size_t i = 0; i < m_buttons.size(); ++i)
        {
            DrawButton(m_buttons[i], i == m_hoveredButton);
        }

        SDL_RenderPresent(m_renderer);
    }

    void GameMenu::DrawButton(MenuButton& button, bool hovered)
    {
        // Change appearance when hovored
        SDL_Rect dest = button.GetDest();
        if (hovered)
        {
            // Slightly enlarge
            dest.x -= 5; dest.y -= 5; dest.w += 10; dest.h += 10;
        }
        SDL_RenderCopy(m_renderer, button.GetTex(), &button.GetSource(), &dest);
    }

    void GameMenu::HandleEvent(int MouseX, int MouseY, bool MousePressed, GameState& outGameStateType, bool& outShouldStartGame, bool& outShouldExit, bool& outShouldRestart)
    {
        m_hoveredButton = -1;
        for (size_t i = 0; i < m_buttons.size(); ++i)
        {
            SDL_Rect rect = m_buttons[i].GetDest();
            if (MouseX >= rect.x && MouseX <= rect.x + rect.w &&
                MouseY >= rect.y && MouseY <= rect.y + rect.h)
            {
                m_hoveredButton = static_cast<int>(i);

                if (MousePressed)
                {
                    // Main menu example
                    if (m_currentMenu == MenuType::Main)
                    {
                        if (m_buttons[i].label == "Levels")
                        {
                            outGameStateType = GameState::Playing;
                            outShouldStartGame = true;
                        }
                        else if (m_buttons[i].label == "Settings")
                            outGameStateType = GameState::Settings;
                        else if (m_buttons[i].label == "Exit")
                            outShouldExit = true;
                    }
                    // Levels menu example
                    else if (m_currentMenu == MenuType::Levels)
                    {
                        if (m_buttons[i].label == "Level 1" || m_buttons[i].label == "Level 2")
                            outShouldStartGame = true;
                        else if (m_buttons[i].label == "Back")
                            outGameStateType = GameState::MainMenu;
                    }
                    // Add similar logic for Pause, Settings, etc.
                }
            }
        }
    }
}