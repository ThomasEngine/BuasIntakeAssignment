#pragma once

#include <vector>
#include <string>
#include <SDL.h>
#include <SDL_image.h>
#include "sound.h"
#include "object.h"


namespace Tmpl8
{
    struct MenuButton : public Object
    {
        std::string label;
        bool isHovered;
        MenuButton(const std::string& label) : label(label) {}
    };

    enum class MenuType
    { 
        Main, 
        Levels, 
        Pause, 
        Settings,
        Quit
    };
    enum class GameState
    {
        Playing,
        Paused
    };

    class GameMenu
    {
    public:
        GameMenu(SDL_Renderer* renderer);
		~GameMenu() { SDL_DestroyTexture(m_spriteSheet); }
        void SetMenu(MenuType type);
        void Render();
        void HandleEvent(int MouseX, int MouseY, bool MousePressed, GameState& outGameStateType, bool& outShouldStartGame, bool& outShouldExit, bool& outShouldRestart, Audio* audio, MenuType& outMenuType);

    private:
        void BuildMenu(MenuType type);
        void DrawButton(MenuButton& button);
        void DrawSlider();

        SDL_Texture* m_spriteSheet = nullptr;
        Object Background;
        SDL_Renderer* m_renderer;
        MenuType m_currentMenu;
        std::vector<MenuButton> m_buttons;

        Object volumeSliderBar;
		Object volumeSliderHandle;
        int sliderMinX = 300;
        int sliderMaxX = 700;
        int sliderY = 500;
        int m_sliderHandleOffset;
		int volume = 100; // Volume percentage
		bool sliderDragging = false;

		bool hover_active;
    };
}