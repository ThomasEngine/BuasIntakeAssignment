#pragma once
#include "object.h"
#include <SDL.h>
#include <SDL_image.h>
#include "sound.h"
#include <string>
#include "template.h"
#include <vector>



namespace Tmpl8
{
    struct MenuButton : public Object
    {
        MenuButton(const std::string& label) : label{ label }, isHovered{ false } {}
        std::string label;
        bool isHovered;
    };

    struct WorldLevel 
    {
        vec2 position;
        bool unlocked;
        std::string label;
    };

    struct MenuSelector 
    {
        int currentLevelIndex = 0;
        vec2 position;
    };

    enum class MenuType
    { 
        Main,
        GamePaused,
        Victory,
        WorldMap

    };
    enum class GameState
    {
        Playing,
        Paused
    };

	const int c_ButtonHeight = 128; // height of the buttons
	const int c_ButtonWidth = 514; // width of the buttons
    enum ButtonLayStd // standard button layout for 3 buttons in screen
    {
        ButtonX = SCREEN_WIDTH / 2 - c_ButtonWidth / 2, // x location for all of the buttons
        ButtonOneY = 84 + 52, // top
        ButtonTwoY = SCREEN_HEIGHT / 2 - c_ButtonHeight / 2, // middle
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
        void BuildWorldMap();
        void DrawButton(MenuButton& button);
        void DrawWorldMap();

        SDL_Texture* m_SpriteSheet;
        Object m_Background;
        SDL_Renderer* m_Renderer;
        MenuType m_CurrentMenu;
		GameState m_CurrentState = GameState::Paused;
        std::vector<MenuButton> m_Buttons;
        std::vector<WorldLevel> m_WorldLevels;
        MenuSelector m_Selector;
    };
}