#ifndef _UPDATEGAME__H
#define _UPDATEGAME__H

#include <iostream>
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>

// Vẽ màn hình
void renderGame(SDL_Renderer* renderer, SDL_Texture* background, int backgroundX,
                SDL_Texture* khunglong, int dinoX, float dinoY, float dinoScale,
                SDL_Texture* xuongRong, int xuongRongX, SDL_Texture* xuongRong2, int xuongRongX2,
                int xuongRongY, float cactusScale,
                const std::string& scoreText, TTF_Font* font)
{
    SDL_RenderClear(renderer);

    // Vẽ background
    SDL_Rect bg1 = { backgroundX, 0, SCREEN_WIDTH, SCREEN_HEIGHT };
    SDL_Rect bg2 = { backgroundX + SCREEN_WIDTH, 0, SCREEN_WIDTH, SCREEN_HEIGHT };
    SDL_RenderCopy(renderer, background, NULL, &bg1);
    SDL_RenderCopy(renderer, background, NULL, &bg2);

    // Vẽ khủng long
    renderScaledTexture(khunglong, dinoX, static_cast<int>(dinoY), renderer, dinoScale);

    // Vẽ xương rồng
    renderScaledTexture(xuongRong, xuongRongX, xuongRongY, renderer, cactusScale);
    renderScaledTexture(xuongRong2, xuongRongX2, xuongRongY, renderer, cactusScale);

    // Vẽ điểm
    renderText(scoreText, 10, 10, renderer, font);

    SDL_RenderPresent(renderer);
}

// xử lý input
void handleEvents(SDL_Event& e, float& velocityY, int& jumpCount, const int MAX_JUMP, bool& isRunning)
{
    while (SDL_PollEvent(&e) != 0)
    {
        if (e.type == SDL_QUIT) isRunning = false;
        if (e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_SPACE && jumpCount < MAX_JUMP)
        {
            velocityY = -10.0f;
            jumpCount++;
        }
    }
}


// Cập nhật trạng thái game
void updateGame(
    float& dinoY, float& velocityY, int& jumpCount,
    const int GROUND_Y, const float GRAVITY, const float JUMP_FORCE, const int MAX_JUMP,
    int& xuongRongX, int& xuongRongX2, const int xuongRongY,
    int& backgroundX, const int backgroundSpeed,
    int& speed, const int maxSpeed,
    int& score, Uint32& speedTime, Uint32& scoreTime,
    SDL_Event& e
) {
    // Xử lý sự kiện nhảy
    if (e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_SPACE && jumpCount < MAX_JUMP) {
        velocityY = JUMP_FORCE;
        jumpCount++;
    }

    // Cập nhật vị trí khủng long
    velocityY += GRAVITY;
    dinoY += velocityY;
    if (dinoY >= GROUND_Y) {
        dinoY = GROUND_Y;
        velocityY = 0;
        jumpCount = 0;
    }

    // Cập nhật vị trí xương rồng
    xuongRongX -= speed;
    if (xuongRongX + 40 < 0)
        xuongRongX = 800 + rand() % 200;

    xuongRongX2 -= speed;
    if (xuongRongX2 + 40 < 0)
        xuongRongX2 = 800 + rand() % 200;

    if (abs(xuongRongX2 - xuongRongX) < 150)
        xuongRongX2 = xuongRongX + 300 + rand() % 200;

    // Cập nhật background
    backgroundX -= backgroundSpeed;
    if (backgroundX <= -SCREEN_WIDTH)
        backgroundX = 0;

    // Cập nhật tốc độ và điểm số theo thời gian
    Uint32 currentTime = SDL_GetTicks();
    if (currentTime - speedTime >= 10000) {
        if (speed < maxSpeed)
            speed += 1;
        speedTime = currentTime;
    }

    if (currentTime - scoreTime >= 1000) {
        score += 10;
        scoreTime = currentTime;
    }
}



// Nếu game over thì vẽ lại nhân vật,xương rồng và nút load game
bool checkGameOver(SDL_Renderer* renderer, SDL_Texture* background, int backgroundX,
                   SDL_Texture* khunglong, int dinoX, float dinoY, float dinoScale,
                   SDL_Texture* xuongRong, int xuongRongX, SDL_Texture* xuongRong2, int xuongRongX2,
                   int xuongRongY, float cactusScale,
                   SDL_Texture* loadLaiGame, SDL_Rect& loadRect,
                   SDL_Event& e, const SDL_Rect& dinoRect, const std::vector<SDL_Rect>& obstacles)
{
    for (const auto& obstacle : obstacles)
    {
        if (checkCollision(dinoRect, obstacle))
        {
            SDL_Log("GAME OVER!");

            SDL_RenderClear(renderer);

            SDL_Rect bg1 = { backgroundX, 0, SCREEN_WIDTH, SCREEN_HEIGHT };
            SDL_Rect bg2 = { backgroundX + SCREEN_WIDTH, 0, SCREEN_WIDTH, SCREEN_HEIGHT };
            SDL_RenderCopy(renderer, background, NULL, &bg1);
            SDL_RenderCopy(renderer, background, NULL, &bg2);

            renderScaledTexture(khunglong, dinoX, static_cast<int>(dinoY), renderer, dinoScale);
            renderScaledTexture(xuongRong, xuongRongX, xuongRongY, renderer, cactusScale);
            renderScaledTexture(xuongRong2, xuongRongX2, xuongRongY, renderer, cactusScale);

            SDL_RenderCopy(renderer, loadLaiGame, NULL, &loadRect);
            SDL_RenderPresent(renderer);

            bool waitingClick = true;
            while (waitingClick)
            {
                while (SDL_PollEvent(&e))
                {
                    if (e.type == SDL_QUIT) exit(0);
                    if (e.type == SDL_MOUSEBUTTONDOWN && e.button.button == SDL_BUTTON_LEFT)
                    {
                        int mouseX = e.button.x;
                        int mouseY = e.button.y;

                        if (mouseX >= loadRect.x && mouseX <= loadRect.x + loadRect.w &&
                            mouseY >= loadRect.y && mouseY <= loadRect.y + loadRect.h)
                        {
                            waitingClick = false;
                            return true; // yêu cầu restart
                        }
                    }
                }
                SDL_Delay(16);
            }

            return false; // nếu thoát ra ngoài
        }
    }
    return false;
}

#endif
