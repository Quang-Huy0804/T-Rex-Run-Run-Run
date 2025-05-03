// file game.h se la nhung file ve logic game

#ifndef _GAME__H
#define _GAME__H

#include <iostream>
#include <SDL.h>
#include <SDL_image.h>

// khởi tạo SDL, cửa sổ, renderer, texture, font.
void initGame(SDL_Window*& window, SDL_Renderer*& renderer,
              SDL_Texture*& background, SDL_Texture*& khunglong,
              SDL_Texture*& xuongRong, SDL_Texture*& xuongRong2,
              SDL_Texture*& manHinhCho, SDL_Texture*& loadLaiGame,
              TTF_Font*& font)
{
    window = initSDL(SCREEN_WIDTH, SCREEN_HEIGHT, WINDOW_TITLE);
    renderer = createRenderer(window);

    background = loadTexture("ảnh/phongCanh/background.png", renderer);
    khunglong = loadTexture("C:\\LTNC\\T-Rex Run Run\\ảnh\\T-rex\\khunglong.png", renderer);
    xuongRong = loadTexture("C:\\LTNC\\T-Rex Run Run\\ảnh\\cactus\\cactus_4.bmp", renderer);
    xuongRong2 = loadTexture("C:\\LTNC\\T-Rex Run Run\\ảnh\\cactus\\cactus_2.bmp", renderer);
    manHinhCho = loadTexture("C:\\LTNC\\T-Rex Run Run\\ảnh\\phongCanh\\manHinhCho.png", renderer);
    loadLaiGame = loadTexture("C:\\LTNC\\T-Rex Run Run\\ảnh\\anhLoadGame\\loadLaiGame.png", renderer);

    showStartScreen(renderer, manHinhCho);

    if (TTF_Init() == -1)
        logErrorAndExit("TTF_Init", TTF_GetError());

    font = TTF_OpenFont("C:\\LTNC\\T-Rex Run Run\\Purisa-BoldOblique.ttf", 20);
    if (!font)
        logErrorAndExit("TTF_OpenFont", TTF_GetError());
}

// vẽ ảnh với kích thước nhỏ hơn hoặc lớn hơn ảnh gốc
void renderScaledTexture(SDL_Texture* texture, int x, int y, SDL_Renderer* renderer, float scale)
{
    int w, h;
    SDL_QueryTexture(texture, NULL, NULL, &w, &h);
    SDL_Rect dest = { x, y, static_cast<int>(w * scale), static_cast<int>(h * scale) };
    SDL_RenderCopy(renderer, texture, NULL, &dest);
}

// Điểm số trong game
void renderText(const std::string& text, int x, int y, SDL_Renderer* renderer, TTF_Font* font)
{
    SDL_Color color = {255, 255, 255}; // Trắng
    SDL_Surface* surface = TTF_RenderText_Solid(font, text.c_str(), color);
    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_Rect dstRect = {x, y, surface->w, surface->h};

    SDL_FreeSurface(surface);
    SDL_RenderCopy(renderer, texture, NULL, &dstRect);
    SDL_DestroyTexture(texture);
}

// dặt lại trạng thái ban đầu cho game
void resetGame(float& dinoY, float& velocityY, int& jumpCount,
               int& xuongRongX, int& xuongRongX2, int& speed, Uint32& startTime)
{
    dinoY = 272;
    velocityY = 0;
    jumpCount = 0;
    xuongRongX = 600;
    xuongRongX2 = 1000;
    speed = 5;
    startTime = SDL_GetTicks();
}

// kiểm tra va chạm của khung long và xương rồng
bool checkCollision(const SDL_Rect& a, const SDL_Rect& b)
{
    return SDL_HasIntersection(&a, &b); // hàm này có sẵn trong SDL, trả về true nếu 2 DL_Rect giao nhau
}

void setupGameObjects(SDL_Texture* khunglong, SDL_Texture* xuongRong, SDL_Texture* loadLaiGame,
                      float& dinoScale, float& cactusScale, SDL_Rect& loadRect,
                      int& dinoW, int& dinoH, int& cactusW, int& cactusH)
{
    SDL_QueryTexture(khunglong, NULL, NULL, &dinoW, &dinoH);
    dinoScale = 1.0f / 8;

    SDL_QueryTexture(xuongRong, NULL, NULL, &cactusW, &cactusH);
    cactusScale = 2.0f / 3;

    int loadW, loadH;
    SDL_QueryTexture(loadLaiGame, NULL, NULL, &loadW, &loadH);
    float loadScale = 0.03f;
    loadRect = {
        (SCREEN_WIDTH - static_cast<int>(loadW * loadScale)) / 2,
        (SCREEN_HEIGHT - static_cast<int>(loadH * loadScale)) / 2,
        static_cast<int>(loadW * loadScale),
        static_cast<int>(loadH * loadScale)
    };
}

#endif // _GAME__H
