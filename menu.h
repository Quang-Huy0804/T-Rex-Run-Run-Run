// file menu.h se la file chua cac ham ve giao dien cho va ket thuc game

#ifndef _MENU__H
#define _MENU__H

#include <iostream>
#include <SDL.h>
#include <SDL_image.h>

// chinh kich thuoc va ten cua so
const int SCREEN_WIDTH = 1080;
const int SCREEN_HEIGHT = 400;
const char* WINDOW_TITLE = "Hi, I love you !!";

// tao man hinh cho
void showStartScreen(SDL_Renderer* renderer, SDL_Texture* manHinhCho)
{
    SDL_RenderClear(renderer);
    SDL_RenderCopy(renderer, manHinhCho, NULL, NULL);
    SDL_RenderPresent(renderer);

    SDL_Rect playButton = {365, 135, 190, 50};

    SDL_Event e;
    bool wait = true;
    while (wait)
    {
        while (SDL_PollEvent(&e))
        {
            if (e.type == SDL_QUIT)
            {
                exit(0);
            }
            if (e.type == SDL_MOUSEBUTTONDOWN && e.button.button == SDL_BUTTON_LEFT)
            {
                int mouseX = e.button.x;
                int mouseY = e.button.y;

                if (mouseX >= playButton.x && mouseX <= playButton.x + playButton.w &&
                    mouseY >= playButton.y && mouseY <= playButton.y + playButton.h)
                {
                    wait = false;
                }
            }
        }
        SDL_Delay(16);
    }
}


// tao hinh load lai game
void showGameOverScreen(SDL_Renderer* renderer, SDL_Texture* gameOverTexture)
{
    SDL_RenderClear(renderer);
    SDL_RenderCopy(renderer, gameOverTexture, NULL, NULL);
    SDL_RenderPresent(renderer);

    SDL_Event e;
    bool wait = true;
    while (wait)
    {
        while (SDL_PollEvent(&e))
        {
            if (e.type == SDL_QUIT)
                exit(0);
            if (e.type == SDL_MOUSEBUTTONDOWN && e.button.button == SDL_BUTTON_LEFT)
                wait = false;
        }
        SDL_Delay(16);
    }
}

#endif // _MENU__H
