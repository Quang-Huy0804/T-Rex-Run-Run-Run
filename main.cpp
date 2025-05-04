#include <iostream>
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <vector>
#include "graphics.h"
#include "menu.h"
#include "game.h"
#include "updateGame.h"

using namespace std;

int main(int argc, char* argv[])
{

    SDL_Window* window = nullptr;
    SDL_Renderer* renderer = nullptr;
    SDL_Texture* background = nullptr, *khunglong = nullptr, *xuongRong = nullptr, *xuongRong2 = nullptr, *manHinhCho = nullptr, *loadLaiGame = nullptr;
    TTF_Font* font = nullptr;

    initGame(window, renderer, background, khunglong, xuongRong, xuongRong2, manHinhCho, loadLaiGame, font);

restart_game:
    int score = 0;
    Uint32 speedTime = SDL_GetTicks();   // Mốc thời gian tăng tốc độ
    Uint32 scoreTime = SDL_GetTicks();   // Mốc thời gian tăng điểm

    float dinoScale, cactusScale;
    SDL_Rect loadRect;
    int dinoW, dinoH, cactusW, cactusH;
    setupGameObjects(khunglong, xuongRong, loadLaiGame, dinoScale, cactusScale, loadRect,dinoW, dinoH, cactusW, cactusH);

    const int GROUND_Y = 272;
    float dinoY = GROUND_Y;
    float velocityY = 0;
    const float GRAVITY = 0.5f;
    const float JUMP_FORCE = -10.0f;
    int jumpCount = 0;
    const int MAX_JUMP = 2;

    int xuongRongX = 600;
    int xuongRongX2 = 1000;
    const int xuongRongY = 313;

    const int dinoX = 40;
    int backgroundX = 0;
    const int backgroundSpeed = 3;

    int speed = 5;
    const int maxSpeed = 12;

    SDL_Event e;
    bool isRunning = true;

    // FPS và frameDelay
    const int FPS = 60;
    const int frameDelay = 1000 / FPS;

    Uint32 frameStart;
    int frameTime;
    int fpsCounter = 0;
    int lastFpsUpdate = SDL_GetTicks();

    while (isRunning)
    {
        handleEvents(e, velocityY, jumpCount, MAX_JUMP, isRunning);
        updateGame( dinoY, velocityY, jumpCount,GROUND_Y, GRAVITY, JUMP_FORCE, MAX_JUMP,xuongRongX, xuongRongX2,
            xuongRongY,backgroundX, backgroundSpeed,speed, maxSpeed,score, speedTime, scoreTime,e);

        frameStart = SDL_GetTicks();

        SDL_Rect dinoRect = {
            dinoX + static_cast<int>(dinoW * dinoScale * 0.1),
            static_cast<int>(dinoY) + static_cast<int>(dinoH * dinoScale * 0.1),
            static_cast<int>(dinoW * dinoScale * 0.7),
            static_cast<int>(dinoH * dinoScale * 0.7)
        };

        std::vector<SDL_Rect> obstacles = {
            { xuongRongX + static_cast<int>(cactusW * cactusScale * 0.15), xuongRongY + static_cast<int>(cactusH * cactusScale * 0.05),
              static_cast<int>(cactusW * cactusScale * 0.6), static_cast<int>(cactusH * cactusScale * 0.75) },
            { xuongRongX2 + static_cast<int>(cactusW * cactusScale * 0.15), xuongRongY + static_cast<int>(cactusH * cactusScale * 0.05),
              static_cast<int>(cactusW * cactusScale * 0.6), static_cast<int>(cactusH * cactusScale * 0.75) }
        };

        if (checkGameOver(renderer, background, backgroundX, khunglong, dinoX, dinoY, dinoScale, xuongRong, xuongRongX, xuongRong2, xuongRongX2,
                          xuongRongY, cactusScale, loadLaiGame, loadRect, e,dinoRect, obstacles))
        {
            goto restart_game;
        }

        std::string scoreText = "Score: " + std::to_string(score);

        renderGame(renderer, background, backgroundX, khunglong, dinoX, dinoY, dinoScale, xuongRong,
                   xuongRongX, xuongRong2, xuongRongX2, xuongRongY, cactusScale, scoreText, font);

        frameTime = SDL_GetTicks() - frameStart;
        if (frameDelay > frameTime)
        {
            SDL_Delay(frameDelay - frameTime);
        }

        fpsCounter++;
        if (SDL_GetTicks() - lastFpsUpdate >= 1000)
        {
            SDL_Log("FPS: %d", fpsCounter);
            fpsCounter = 0;
            lastFpsUpdate = SDL_GetTicks();
        }
    }

    SDL_DestroyTexture(khunglong);
    SDL_DestroyTexture(xuongRong);
    SDL_DestroyTexture(xuongRong2);
    SDL_DestroyTexture(background);
    SDL_DestroyTexture(manHinhCho);
    SDL_DestroyTexture(loadLaiGame);
    quitSDL(window, renderer);

    return 0;
}

