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
        MenuButton(const std::string& label) : label{ label }, isHovered{ false } {}
    };

    enum class MenuType
    { 
        Main,
        GamePaused,
        Victory,

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
		void SetGameState(GameState state) { m_currentState = state; }
        void Render();
        void HandleEvent(int MouseX, int MouseY, bool MousePressed, GameState& outGameStateType, bool& backToMenu, bool& outShouldStartGame, bool& outShouldExit, bool& outShouldRestart, Audio* audio, MenuType& outMenuType);

    private:
        void BuildMenu(MenuType type);
        void DrawButton(MenuButton& button);

        SDL_Texture* m_spriteSheet;
        Object Background;
        SDL_Renderer* m_renderer;
        MenuType m_currentMenu;
		GameState m_currentState;
        std::vector<MenuButton> m_buttons;
    };
}