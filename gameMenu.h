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
		~GameMenu() { SDL_DestroyTexture(m_SpriteSheet); }

        // Setters
        void SetMenu(MenuType type);
		void SetGameState(GameState state) { m_CurrentState = state; }

        void Render();
        void HandleEvent(int MouseX, int MouseY, bool MousePressed, GameState& outGameStateType, bool& backToMenu, bool& outShouldStartGame, bool& outShouldExit, bool& outShouldRestart, Audio* audio, MenuType& outMenuType);

    private:
        void BuildMenu(MenuType type);
        void DrawButton(MenuButton& button);

        SDL_Texture* m_SpriteSheet;
        Object m_Background;
        SDL_Renderer* m_Renderer;
        MenuType m_CurrentMenu;
		GameState m_CurrentState = GameState::Paused;
        std::vector<MenuButton> m_Buttons;
    };
}