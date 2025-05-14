#pragma once

#include <vector>
#include <SDL.h>
#include <SDL_image.h>
#include "object.h"
#include <string>


namespace Tmpl8
{
    struct MenuButton : public Object
    {
        std::string label;
        MenuButton(const std::string& label) : label(label) {}
    };

    enum class MenuType
    { 
        Main, 
        Levels, 
        Pause, 
        Settings 
    };
    enum class GameState
    {
        MainMenu,
        Playing,
        Paused,
        Levels,
        Settings
    };

    class GameMenu
    {
    public:
        GameMenu(SDL_Renderer* renderer);
		~GameMenu() { SDL_DestroyTexture(m_spriteSheet); }
        void SetMenu(MenuType type);
        void Render();
        void HandleEvent(int MouseX, int MouseY, bool MousePressed, GameState& outGameStateType, bool& outShouldStartGame, bool& outShouldExit, bool& outShouldRestart);

    private:
        void BuildMenu(MenuType type);
        void DrawButton(MenuButton& button, bool hovered);

        SDL_Texture* m_spriteSheet = nullptr;
        Object Background;
        SDL_Renderer* m_renderer;
        MenuType m_currentMenu;
        std::vector<MenuButton> m_buttons;
        int m_hoveredButton = -1;
    };
}