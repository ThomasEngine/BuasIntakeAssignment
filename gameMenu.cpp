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

        hover_active = false;

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
		case MenuType::Settings:
			// Music slider bar
			volumeSliderBar.SetDest(sliderMinX, sliderMaxX, sliderMaxX - sliderMinX, 10);
			volumeSliderBar.SetTexture(m_spriteSheet);
			volumeSliderBar.SetSource(0, 0, 600, 200);

			// Music slider handle
			volumeSliderHandle.SetDest(sliderMinX + (volume * 4), sliderMaxX - 10, 20, 20);
			volumeSliderHandle.SetTexture(m_spriteSheet);
			volumeSliderHandle.SetSource(0, 0, 600, 200);

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
            DrawButton(m_buttons[i]);
        }

        if (m_currentMenu == MenuType::Settings)
            DrawSlider();

        SDL_RenderPresent(m_renderer);
    }

    void GameMenu::DrawButton(MenuButton& button)
    {
        // Change appearance when hovored
        SDL_Rect dest = button.GetDest();
        if (button.isHovered)
        {
            // Slightly enlarge
            dest.x -= 5; dest.y -= 5; dest.w += 10; dest.h += 10;
        }
        SDL_RenderCopy(m_renderer, button.GetTex(), &button.GetSource(), &dest);
    }

    void GameMenu::DrawSlider()
    {
        // Draw slider
		SDL_Rect sliderBarDest = volumeSliderBar.GetDest();
		SDL_Rect sliderBarSrc = volumeSliderBar.GetSource();
		SDL_RenderCopy(m_renderer, volumeSliderBar.GetTex(), &sliderBarSrc, &sliderBarDest);

        // Draw knob
		SDL_Rect sliderHandleDest = volumeSliderHandle.GetDest();
		SDL_Rect sliderHandleSrc = volumeSliderHandle.GetSource();
		SDL_RenderCopy(m_renderer, volumeSliderHandle.GetTex(), &sliderHandleSrc, &sliderHandleDest);
    }

    void GameMenu::HandleEvent(int MouseX, int MouseY, bool MousePressed, GameState& outGameStateType, bool& outShouldStartGame, bool& outShouldExit, bool& outShouldRestart, Audio* audio, MenuType& outMenuType)
    {  
        for (size_t i = 0; i < m_buttons.size(); ++i)
        {
            SDL_Rect rect = m_buttons[i].GetDest();
            if (MouseX >= rect.x && MouseX <= rect.x + rect.w &&
                MouseY >= rect.y && MouseY <= rect.y + rect.h)
            {
                if (!m_buttons[i].isHovered)
                {
					audio->PlayHoverSound(); // Play hover sfx
					m_buttons[i].isHovered = true;
                }
                if (MousePressed)
                {
                    audio->PlayClickSound(); // Play click sfx

                    // Main menu 
                    if (m_currentMenu == MenuType::Main)
                    {
                        if (m_buttons[i].label == "Levels")
                        {
                            outGameStateType = GameState::Playing;
                            outShouldStartGame = true;
                        }
                        else if (m_buttons[i].label == "Settings")
                        {
                            outGameStateType = GameState::Paused;
                            outMenuType = MenuType::Settings;
                        }
                        else if (m_buttons[i].label == "Exit")
                            outShouldExit = true;
                    }
                    // Levels menu 
                    else if (m_currentMenu == MenuType::Levels)
                    {
                        if (m_buttons[i].label == "Level 1" || m_buttons[i].label == "Level 2")
                            outShouldStartGame = true;
                        else if (m_buttons[i].label == "Back")
                        {
                            outGameStateType = GameState::Paused;
                            outMenuType = MenuType::Main;
                        }
                    }
                    else if (m_currentMenu == MenuType::Quit)
                        outShouldExit = true;
                    // Settings menu
                    else if (m_currentMenu == MenuType::Settings)
                    {
                        if (m_buttons[i].label == "Back")
                        {
                            outGameStateType = GameState::Paused;
                            outMenuType = MenuType::Main;
                        }
                    }
                }

            }
			else m_buttons[i].isHovered = false;
        }
        SDL_Rect knobRect = volumeSliderHandle.GetDest();
        if (MousePressed)
        {
            if (MouseX >= knobRect.x && MouseX <= knobRect.x + knobRect.w &&
                MouseY >= knobRect.y && MouseY <= knobRect.y + knobRect.h)
            {
                sliderDragging = true;
                m_sliderHandleOffset = MouseX - knobRect.x;
            }
        }
        if (sliderDragging)
        {
            int newHandleX = MouseX - m_sliderHandleOffset;
            if (newHandleX >= sliderMaxX) newHandleX = sliderMaxX;
            if (newHandleX <= sliderMinX) newHandleX = sliderMinX; // making sure slider does not go beyond the slider
            volume = (newHandleX - sliderMinX) / 4;
            volumeSliderHandle.SetDest(newHandleX + 10, sliderY - 10);
        }
        if (!MousePressed) sliderDragging = false;
    }
}