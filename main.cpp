#include <iostream>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <cstdlib>
#include <ctime>

using namespace std;

int const SCREEN_W = 800;
int const SCREEN_H = 600;

SDL_Window *gWindow = NULL;
SDL_Renderer *gRenderer = NULL;
SDL_Texture *gTexture = NULL;

// kiểm tra khởi tạo
bool init()
{
    bool success = true;
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        cout << "SDL could not initalized %s\n" << SDL_GetError() << endl;
        success = false;
    } else {
        gWindow = SDL_CreateWindow("AimGame", SDL_WINDOWPOS_UNDEFINED,SDL_WINDOWPOS_UNDEFINED, SCREEN_W,SCREEN_H,SDL_WINDOW_SHOWN);
        if (gWindow == NULL) {
            cout << "Window could not initalized %s\n" << SDL_GetError() << endl;
            success = false;
        } else {
            // tạo renderer cho cửa sổ
            gRenderer = SDL_CreateRenderer(gWindow, - 1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
            if (gRenderer == NULL) {
                cout << "Renderer could not created! SDL_ERROR:" << SDL_GetError() << endl;
                success = false;
            }
        }
    }

    return success;
}  

// định dạng cho hình vuông
void renderSquare(int x, int y, int size)
{
    SDL_Rect square;
    square.x = x;
    square.y = y;
    square.w = size;
    square.h = size;

    // đặt màu cho renderer (chọn màu bút)
    SDL_SetRenderDrawColor(gRenderer, 255, 0, 0, 255);
    // vẽ hình từ renderer lên square (to màu)
    SDL_RenderFillRect(gRenderer, &square);

    // định nghĩa màu nền trước khi clear để tránh bị nhầm nền
    SDL_SetRenderDrawColor(gRenderer, 0, 0, 0, 255);

    // cập nhật màn hình
    SDL_RenderPresent(gRenderer);
}

bool isMouseInSquare(int x_mouse, int y_mouse, int x_square, int y_square, int size) {
    if ((x_mouse <= x_square+size and x_mouse >= x_square) and (y_mouse <= y_square+size and y_mouse >= y_square)) {
        return true;
    }
    return false;
}

// đóng cửa sổ, giải phóng bộ nhớ
void close() 
{
    SDL_DestroyRenderer(gRenderer);
    gRenderer = NULL;

    SDL_DestroyWindow(gWindow);

    SDL_Quit();
}

int main (int argv, char *argc[]) {
    if (init() == false) {
        cout << "Cannot initialized window" << endl;
    }  else {
        // khởi động cửa sổ
        bool quit = false;
        SDL_Event e;

        // sau khi khởi động cửa sổ thì vẽ luôn 1 cái ô vuông có sẵn
        int square_size = 50;
        int x_pos = rand() % (SCREEN_W - square_size);
        int y_pos = rand () % (SCREEN_H - square_size);
        renderSquare(x_pos, y_pos, square_size);
        
        while (quit == false) {
            while (SDL_PollEvent(&e) != 0) {
                if (e.type == SDL_QUIT) {
                    quit = true;
                } else if (e.type == SDL_MOUSEBUTTONDOWN) {
                    if (e.button.button == SDL_BUTTON_LEFT) {
                        int x_mouse, y_mouse;
                        SDL_GetMouseState(&x_mouse, &y_mouse); // lấy vị trí của con chuột vào x_mouse, y_mouse

                        // nếu click trong hình vuông thì thực hiện xóa cái cũ random cái mới
                        if (isMouseInSquare(x_mouse, y_mouse, x_pos, y_pos, square_size)) {
                            x_pos = rand() % (SCREEN_W - square_size);
                            y_pos = rand () % (SCREEN_H - square_size);
                            SDL_RenderClear(gRenderer);
                            renderSquare(x_pos, y_pos, square_size);
                        }
                    }
                }
            }
            // cài đặt màu nền 
            SDL_SetRenderDrawColor(gRenderer, 0, 0, 0, 255);

            // cập nhật màn hình
            SDL_RenderPresent(gRenderer);

        }
    }
    
    close();
    return 0;
}