/*******************************************************************************************
*
*   animal crossing clone with raylib
*
*   animals!
*   do they even be crossing?
*
*   This game has been created using raylib (www.raylib.com)
*   raylib is licensed under an unmodified zlib/libpng license (View raylib.h for details)
*
*   Copyright (c) 2021 Ramon Santamaria (@raysan5)
*
********************************************************************************************/

#include "raylib.h"
#include "screens.h"    // NOTE: Declares global (extern) variables and screens functions

#if defined(PLATFORM_WEB)
    #include <emscripten/emscripten.h>
#endif

//----------------------------------------------------------------------------------
// Shared Variables Definition (global)
// NOTE: Those variables are shared between modules through screens.h
//----------------------------------------------------------------------------------
GameScreen currentScreen = LOGO;
Font font = { 0 };
Music music = { 0 };
Sound fxCoin = { 0 };

//----------------------------------------------------------------------------------
// Local Variables Definition (local to this module)
//----------------------------------------------------------------------------------
static const int screenWidth = 800;
static const int screenHeight = 450;
static bool running = true;
static Color backgroundColor = {147, 211, 196, 255};

static float transAlpha = 0.0f;
static bool onTransition = false;
static bool transFadeOut = false;
static int transFromScreen = -1;
static GameScreen transToScreen = UNKNOWN;

//----------------------------------------------------------------------------------
// Local Functions Declaration
//----------------------------------------------------------------------------------
static void ChangeToScreen(int screen);     // Change to screen, no transition effect

static void TransitionToScreen(int screen); // Request transition to next screen
static void UpdateTransition(void);         // Update transition effect
static void DrawTransition(void);           // Draw transition effect (full-screen rectangle)

static void UpdateDrawFrame(void);          // Update and draw one frame

int main(void)
{
    InitWindow(screenWidth, screenHeight, "Animals!");
    SetExitKey(0);

//     InitAudioDevice();

//     font = LoadFont("resources/mecha.png");
//     music = LoadMusicStream("resources/ambient.ogg");
//     fxCoin = LoadSound("resources/coin.wav");

//     SetMusicVolume(music, 1.0f);
//     PlayMusicStream(music);

//     // Setup and init first screen
//     currentScreen = LOGO;
//     InitLogoScreen();

#if defined(PLATFORM_WEB)
    emscripten_set_main_loop(UpdateDrawFrame, 60, 1);
#else
    SetTargetFPS(60);

    // Main game loop
    while (!WindowShouldClose())
    {
        UpdateDrawFrame();
    }
#endif

//     switch (currentScreen)
//     {
//         case LOGO: UnloadLogoScreen(); break;
//         case TITLE: UnloadTitleScreen(); break;
//         case GAMEPLAY: UnloadGameplayScreen(); break;
//         case ENDING: UnloadEndingScreen(); break;
//         default: break;
//     }

//     // Unload global data loaded
//     UnloadFont(font);
//     UnloadMusicStream(music);
//     UnloadSound(fxCoin);

//     CloseAudioDevice();

    CloseWindow();

    return 0;
}

//----------------------------------------------------------------------------------
// Module specific Functions Definition
//----------------------------------------------------------------------------------
static void ChangeToScreen(GameScreen screen)
{
    switch (currentScreen)
    {
        case LOGO: UnloadLogoScreen(); break;
        case TITLE: UnloadTitleScreen(); break;
        case GAMEPLAY: UnloadGameplayScreen(); break;
        case ENDING: UnloadEndingScreen(); break;
        default: break;
    }

    switch (screen)
    {
        case LOGO: InitLogoScreen(); break;
        case TITLE: InitTitleScreen(); break;
        case GAMEPLAY: InitGameplayScreen(); break;
        case ENDING: InitEndingScreen(); break;
        default: break;
    }

    currentScreen = screen;
}

static void TransitionToScreen(GameScreen screen)
{
    onTransition = true;
    transFadeOut = false;
    transFromScreen = currentScreen;
    transToScreen = screen;
    transAlpha = 0.0f;
}

static void UpdateTransition(void)
{
    if (!transFadeOut)
    {
        transAlpha += 0.05f;

        // NOTE: Due to float internal representation, condition jumps on 1.0f instead of 1.05f
        // For that reason we compare against 1.01f, to avoid last frame loading stop
        if (transAlpha > 1.01f)
        {
            transAlpha = 1.0f;

            switch (transFromScreen)
            {
                case LOGO: UnloadLogoScreen(); break;
                case TITLE: UnloadTitleScreen(); break;
                case OPTIONS: UnloadOptionsScreen(); break;
                case GAMEPLAY: UnloadGameplayScreen(); break;
                case ENDING: UnloadEndingScreen(); break;
                default: break;
            }

            switch (transToScreen)
            {
                case LOGO: InitLogoScreen(); break;
                case TITLE: InitTitleScreen(); break;
                case GAMEPLAY: InitGameplayScreen(); break;
                case ENDING: InitEndingScreen(); break;
                default: break;
            }

            currentScreen = transToScreen;

            transFadeOut = true;
        }
    }
    else
    {
        transAlpha -= 0.02f;

        if (transAlpha < -0.01f)
        {
            transAlpha = 0.0f;
            transFadeOut = false;
            onTransition = false;
            transFromScreen = -1;
            transToScreen = UNKNOWN;
        }
    }
}

static void DrawTransition(void)
{
    DrawRectangle(0, 0, GetScreenWidth(), GetScreenHeight(), Fade(BLACK, transAlpha));
}

static void UpdateDrawFrame(void)
{
    UpdateMusicStream(music);

    // if (!onTransition)
    // {
    //     switch(currentScreen)
    //     {
    //         case LOGO:
    //         {
    //             UpdateLogoScreen();

    //             if (FinishLogoScreen()) TransitionToScreen(TITLE);

    //         } break;
    //         case TITLE:
    //         {
    //             UpdateTitleScreen();

    //             if (FinishTitleScreen() == 1) TransitionToScreen(OPTIONS);
    //             else if (FinishTitleScreen() == 2) TransitionToScreen(GAMEPLAY);

    //         } break;
    //         case OPTIONS:
    //         {
    //             UpdateOptionsScreen();

    //             if (FinishOptionsScreen()) TransitionToScreen(TITLE);

    //         } break;
    //         case GAMEPLAY:
    //         {
    //             UpdateGameplayScreen();

    //             if (FinishGameplayScreen() == 1) TransitionToScreen(ENDING);
    //             //else if (FinishGameplayScreen() == 2) TransitionToScreen(TITLE);

    //         } break;
    //         case ENDING:
    //         {
    //             UpdateEndingScreen();

    //             if (FinishEndingScreen() == 1) TransitionToScreen(TITLE);

    //         } break;
    //         default: break;
    //     }
    // }
    // else UpdateTransition();    // Update transition (fade-in, fade-out)

    BeginDrawing();

        // ClearBackground(RAYWHITE);
        ClearBackground(backgroundColor);

        switch(currentScreen)
        {
            case LOGO: DrawLogoScreen(); break;
            case TITLE: DrawTitleScreen(); break;
            case OPTIONS: DrawOptionsScreen(); break;
            case GAMEPLAY: DrawGameplayScreen(); break;
            case ENDING: DrawEndingScreen(); break;
            default: break;
        }

        // Draw full screen rectangle in front of everything
        if (onTransition) DrawTransition();

        //DrawFPS(10, 10);

    EndDrawing();
}
