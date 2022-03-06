#include <iostream>
#include <SDL.h>
#include "SDL_utils.h"
#include <vector>
#include <cmath>

using namespace std;

//**************************************************************
const int SCREEN_WIDTH = 800;
const int SCREEN_HEIGHT = 600;
const char WINDOW_TITLE[] = "Moving Wheel in SDL";

int main(int argc, char* argv[])
{
    SDL_Window* window;
    SDL_Renderer* renderer;
    initSDL(window, renderer, SCREEN_WIDTH, SCREEN_HEIGHT, WINDOW_TITLE);

    //Nạp các ảnh bánh xe vào một danh sách, sử dụng vector
    vector<SDL_Texture *> wheels;
    /*
    //Ảnh nền trắng
    wheels.push_back(loadTexture("images/wheel01.bmp", renderer));
    wheels.push_back(loadTexture("images/wheel02.bmp", renderer));
    wheels.push_back(loadTexture("images/wheel03.bmp", renderer));
    wheels.push_back(loadTexture("images/wheel04.bmp", renderer));
    wheels.push_back(loadTexture("images/wheel05.bmp", renderer));
    wheels.push_back(loadTexture("images/wheel06.bmp", renderer));
    wheels.push_back(loadTexture("images/wheel07.bmp", renderer));
    wheels.push_back(loadTexture("images/wheel08.bmp", renderer));
    */

    //Ảnh không có nền (transparent background), chuyển đổi (convert) từ ảnh trên
    //Dùng Photoshop, hay webapp: http://www140.lunapic.com/editor/?action=transparent
    wheels.push_back(loadTexture("images/w01.bmp", renderer));
    wheels.push_back(loadTexture("images/w02.bmp", renderer));
    wheels.push_back(loadTexture("images/w03.bmp", renderer));
    wheels.push_back(loadTexture("images/w04.bmp", renderer));
    wheels.push_back(loadTexture("images/w05.bmp", renderer));
    wheels.push_back(loadTexture("images/w06.bmp", renderer));
    wheels.push_back(loadTexture("images/w07.bmp", renderer));
    wheels.push_back(loadTexture("images/w08.bmp", renderer));
    int imageNum = wheels.size();
    for (int i = 0; i < imageNum; ++i) {
        if (wheels[i] == nullptr) {
            for (int j = 0; j < imageNum; ++j) SDL_DestroyTexture(wheels[j]);
            quitSDL(window, renderer);
            return 1;
        }
    }

    //Vẽ ảnh đầu vào giữa màn hình
    int iSize = 60; //Kích cỡ ảnh (và là đường kính của bánh xe)
    int x = SCREEN_WIDTH / 2 - iSize / 2;
    int y = SCREEN_HEIGHT / 2 - iSize / 2;


    // Bước nhảy mỗi khi dịch chuyển bằng chu vi chia 8 (bởi có 8 hình vẽ bánh xe)
    int step = M_PI * iSize / imageNum;
    int curImage = 0;
    bool rightDirection = true;
    SDL_Event e;
    bool running = true;
    while (running) {
        // Đợi 200 mili giây
        SDL_Delay(200);

        //Xoá màn hình về màu trắng (cùng màu nền của các ảnh bánh xe)
        //(Nếu dùng ảnh transparent background thì mầu nền gì cũng được
        //SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);   // white
        SDL_RenderClear(renderer);
        //Vẽ bánh xe hiện thời vào khung hình chữ nhật
        renderTexture(wheels[curImage], renderer, x, y, iSize, iSize);
        // Dùng lệnh hiển thị (đưa) hình đã vẽ ra mành hình
       //Khi thông thường chạy với môi trường bình thường ở nhà
        //SDL_RenderPresent(renderer);
       //Khi chạy ở máy thực hành WinXP ở trường (máy ảo)
       SDL_UpdateWindowSurface(window);

        while (SDL_PollEvent(&e)) {
            // Nếu sự kiện là kết thúc (như đóng cửa sổ) thì thoát khỏi vòng lặp
            if (e.type == SDL_QUIT) {
                running = false;
                break;
            }
            // Nếu có một phím được nhấn, thì xét phím đó là gì để xử lý tiếp
            if (e.type == SDL_KEYDOWN) {
                // Nếu nhấn phìm ESC thì thoát khỏi vòng lặp
                if (e.key.keysym.sym == SDLK_ESCAPE) running = false;
                else if (e.key.keysym.sym == SDLK_LEFT) rightDirection = false;
                else if (e.key.keysym.sym == SDLK_RIGHT) rightDirection = true;
                break;
            }
        }

        if (rightDirection) {
            x = (x + step) % SCREEN_WIDTH;
            curImage = (curImage + 1) % imageNum;
        } else {
            x = (x + SCREEN_WIDTH - step) % SCREEN_WIDTH;
            curImage = (curImage + imageNum - 1) % imageNum;
        }

    }

    //Giải phóng các biến texture đã dùng
    for (int j = 0; j < imageNum; ++j) SDL_DestroyTexture(wheels[j]);
    //Kết thúc SDL (giải phóng các biển window và renderer)
    quitSDL(window, renderer);
    return 0;
}

