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

#include <stdlib.h>
#include <stdio.h>
#include "raylib.h"
#include "screens.h"

//----------------------------------------------------------------------------------
// Module Variables Definition (local)
//----------------------------------------------------------------------------------

static const char *mapName = "one.tilemap";

static int framesCounter = 0;
static int finishScreen = 0;

static Color backgroundColor = {147, 211, 196, 255};

static Texture grassSprite;

static Texture playerSprite;
static Rectangle playerSource;
static Rectangle playerDestination;
static float playerSpeed;
static bool playerMoving;
static int playerDir;
static bool playerUp, playerDown, playerRight, playerLeft;
static int playerFramesCounter = 0;

static FILE *fp;
static Rectangle tileDestination;
static Rectangle tileSource;
static int *tileMap;
static char **sourceMap;
static int mapWidth;
static int mapHeight;
static int tileMapSize;

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

    tileDestination = (Rectangle){ 0, 0, 16, 16 };
    tileSource = (Rectangle){ 0, 0, 16, 16 };

    playerSprite = LoadTexture("resources/sprout_lands/Characters/BasicCharakterSpritesheet.png");
    playerSource = (Rectangle){ 0, 0, 48, 48 };
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
        2.f
    };

    mapWidth = -1;
    mapHeight = -1;

    fp = fopen(mapName, "r");
    if (fp != NULL) {
        int c;
        tileMapSize = 0;
        int fileSize = 0;

        while ((c = fgetc(fp)) != EOF) {
            fileSize++;

            if ((char)c != '\n' && (char)c != ' ') {
                tileMapSize++;
            }
        }

        tileMap = malloc(sizeof(int) * (tileMapSize - 2));
        if (fseek(fp, 0L, SEEK_SET) != 0) { /* Error */ }

        int counter = 0;
        for (int i = 0; i < fileSize; i++) {
            if ((c = fgetc(fp)) != EOF) {
                if ((char)c != '\n' && (char)c != ' ') {
                    if (mapWidth == -1) {
                        mapWidth = c - '0';
                    } else if (mapHeight == -1) {
                        mapHeight = c - '0';
                    } else {
                        tileMap[counter] = c - '0';
                        counter++;
                    }
                }
            }
        }
    }

    fclose(fp);
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
        playerMoving = true;
        playerDir = 1;
        playerUp = true;
    }
    if (IsKeyDown(KEY_S)) {
        playerMoving = true;
        playerDir = 0;
        playerDown = true;
    }
    if (IsKeyDown(KEY_A)) {
        playerMoving = true;
        playerDir = 2;
        playerLeft = true;
    }
    if (IsKeyDown(KEY_D)) {
        playerMoving = true;
        playerDir = 3;
        playerRight = true;
    }

    playerSource.x = playerSource.width * (float)playerFramesCounter;

    if (playerMoving) {
        if (playerUp) {
            playerDestination.y -= playerSpeed;
        }
        if (playerDown) {
            playerDestination.y += playerSpeed;
        }
        if (playerLeft) {
            playerDestination.x -= playerSpeed;
        }
        if (playerRight) {
            playerDestination.x += playerSpeed;
        }
        if (framesCounter % 8 == 1) {
            playerFramesCounter++;
        }
    } else if (framesCounter % 45 == 1) {
        playerFramesCounter++;
    }

    framesCounter++;
    if (playerFramesCounter > 3) {
        playerFramesCounter = 0;
    }

    if (!playerMoving && playerFramesCounter > 1) {
        playerFramesCounter = 0;
    }

    playerSource.x = playerSource.width * (float)playerFramesCounter;
    playerSource.y = playerSource.height * (float)playerDir;

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
        1.5f
    };

    playerMoving = false;
    playerUp = false;
    playerDown = false;
    playerRight = false;
    playerLeft = false;
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

    for (int i = 0; i < tileMapSize; i++) {
        if (tileMap[i] != 0) {
            tileDestination.x = tileDestination.width * (float)(i % mapWidth);
            tileDestination.y = tileDestination.height * (float)(i / mapWidth);
            tileSource.x = tileSource.width * (float)(tileMap[i]-1 % (int)(grassSprite.width/(int)tileSource.width));
            tileSource.y = tileSource.height * (float)(tileMap[i]-1 / (int)(grassSprite.width/(int)tileSource.width));

            DrawTexturePro(grassSprite, tileSource, tileDestination, (Vector2){ tileDestination.width, tileDestination.height }, 0.f, WHITE);
        }
    }

    DrawTexturePro(playerSprite, playerSource, playerDestination, (Vector2){ playerDestination.width, playerDestination.height }, 0.f, WHITE);

    EndMode2D();
}

// Gameplay Screen Unload logic
void UnloadGameplayScreen(void)
{
    // TODO: Unload GAMEPLAY screen variables here!
    free(tileMap);
}

// Gameplay Screen should finish?
int FinishGameplayScreen(void)
{
    return finishScreen;
}