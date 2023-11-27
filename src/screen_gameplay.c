/**********************************************************************************************
*
*   raylib - Advance Game template
*
*   Gameplay Screen Functions Definitions (Init, Update, Draw, Unload)
*
*   Copyright (c) 2014-2022 Ramon Santamaria (@raysan5)
*
*   This software is provided "as-is", without any express or implied warranty. In no event
*   will the authors be held liable for any damages arising from the use of this software.
*
*   Permission is granted to anyone to use this software for any purpose, including commercial
*   applications, and to alter it and redistribute it freely, subject to the following restrictions:
*
*     1. The origin of this software must not be misrepresented; you must not claim that you
*     wrote the original software. If you use this software in a product, an acknowledgment
*     in the product documentation would be appreciated but is not required.
*
*     2. Altered source versions must be plainly marked as such, and must not be misrepresented
*     as being the original software.
*
*     3. This notice may not be removed or altered from any source distribution.
*
**********************************************************************************************/

#include "raylib.h"
#include "screens.h"

//----------------------------------------------------------------------------------
// Module Variables Definition (local)
//----------------------------------------------------------------------------------
static int framesCounter = 0;
static int finishScreen = 0;

static Color backgroundColor = {147, 211, 196, 255};

static Texture grassSprite;

static Texture playerSprite;
static Rectangle playerOrigin;
static Rectangle playerDestination;
static float playerSpeed;
static Camera2D camera;

//----------------------------------------------------------------------------------
// Gameplay Screen Functions Definition
//----------------------------------------------------------------------------------

// Gameplay Screen Initialization logic
void InitGameplayScreen(void)
{
    // TODO: Initialize GAMEPLAY screen variables here!
    framesCounter = 0;
    finishScreen = 0;

    SetExitKey(0);

    grassSprite = LoadTexture("resources/sprout_lands/Tilesets/Grass.png");

    playerSprite = LoadTexture("resources/sprout_lands/Characters/BasicCharakterSpritesheet.png");
    playerOrigin = (Rectangle){ 0, 0, 48, 48 };
    playerDestination = (Rectangle){ 200, 200, 100, 100 };
    playerSpeed = 3.f;

    camera = (Camera2D) {
        (Vector2) {
            GetScreenWidth()/2,
            GetScreenHeight()/2
        },
        (Vector2) {
            playerDestination.x - playerDestination.width/2,
            playerDestination.y - playerDestination.height/2
        },
        0.f,
        1.f
    };
}

// Gameplay Screen Update logic
void UpdateGameplayScreen(void)
{
    // TODO: Update GAMEPLAY screen variables here!

    // Press enter or tap to change to ENDING screen
    // if (IsKeyPressed(KEY_ENTER) || IsGestureDetected(GESTURE_TAP))
    // {
    //     finishScreen = 1;
    //     PlaySound(fxCoin);
    // }

    if (IsKeyDown(KEY_W)) {
        playerDestination.y -= playerSpeed;
    }
    if (IsKeyDown(KEY_S)) {
        playerDestination.y += playerSpeed;
    }
    if (IsKeyDown(KEY_A)) {
        playerDestination.x -= playerSpeed;
    }
    if (IsKeyDown(KEY_D)) {
        playerDestination.x += playerSpeed;
    }

    camera = (Camera2D) {
        (Vector2) {
            GetScreenWidth()/2,
            GetScreenHeight()/2
        },
        (Vector2) {
            playerDestination.x - playerDestination.width/2,
            playerDestination.y - playerDestination.height/2
        },
        0.f,
        1.f
    };
}

// Gameplay Screen Draw logic
void DrawGameplayScreen(void)
{
    // TODO: Draw GAMEPLAY screen here!
    // DrawRectangle(0, 0, GetScreenWidth(), GetScreenHeight(), PURPLE);
    // Vector2 pos = { 20, 10 };
    // DrawTextEx(font, "GAMEPLAY SCREEN", pos, font.baseSize*3.0f, 4, MAROON);
    // DrawText("PRESS ENTER or TAP to JUMP to ENDING SCREEN", 130, 220, 20, MAROON);
    ClearBackground(backgroundColor);
    BeginMode2D(camera);
    DrawTexture(grassSprite, 100, 50, WHITE);
    DrawTexturePro(playerSprite, playerOrigin, playerDestination, (Vector2){ playerDestination.width, playerDestination.height }, 0.f, WHITE);
    EndMode2D();
}

// Gameplay Screen Unload logic
void UnloadGameplayScreen(void)
{
    // TODO: Unload GAMEPLAY screen variables here!
}

// Gameplay Screen should finish?
int FinishGameplayScreen(void)
{
    return finishScreen;
}