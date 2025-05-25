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
    enum ButtonLayStd // standard button layout for 3 buttons in screen
    {
        ButtonX = 1280 / 2 - 514 / 2, // x location for all of the buttons
        ButtonOneY = 84 + 52, // top
        ButtonTwoY = 720 / 2 - 128 / 2, // middle
        ButtonThreeY = 508 - 52 // under
    };
    // 264

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

        int m_ButtonWidth;
        int m_ButtonHeight;
    };
}