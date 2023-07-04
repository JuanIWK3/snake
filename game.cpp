#include <SDL2/SDL.h>
#include <SDL2/SDL_events.h>
#include <SDL2/SDL_render.h>
#include <SDL2/SDL_timer.h>
#include <iostream>
#include <valarray>
#include <vector>
#include <cstdlib>

using namespace std;

struct Snake {
  int pos[2] = {0,0};

  void move_up() {
    if (this->pos[1] % 8 == 0) {
      this->pos[1] = 7;
    } else {
      this->pos[1] -= 1;
    }
  }

  void move_down() {
    if (this->pos[1] % 8 == 7) {
      this->pos[1] = 0;
    } else {
      this->pos[1] += 1;
    }
  }

  void move_left() {
    if (this->pos[0] % 8 == 0) {
      this->pos[0] = 7;
    } else {
      this->pos[0] -= 1;
    }
  }

  void move_right() {
    if (this->pos[0] % 8 == 7) {
      this->pos[0] = 0;
    } else {
      this->pos[0] += 1;
    }
  }

  void eat() {
    cout << "eat" << endl;
  }
};

struct Apple {
  int pos[2] = {4,4};

  void get_another_pos() {
    this->pos[0] = rand() % 8;
    this->pos[1] = rand() % 8;
    
    cout << "changed pos: " << this->pos[0] << ", " << this->pos[1] << endl;
  }
};

void eat_apple(Snake& snake, Apple& apple) {
  snake.eat();
  apple.get_another_pos();
}

int main() {
  SDL_Window* window = nullptr;
  SDL_Renderer* renderer = nullptr;
  SDL_Event e;
  SDL_Init(SDL_INIT_EVERYTHING);

  bool running = true;

  const int SPEED = 50;
  const int SQUARE_SIZE = 50;
  const int GAP = 1;
  const int SCREEN_WIDTH = (SQUARE_SIZE + GAP) * 8;
  const int SCREEN_HEIGHT = (SQUARE_SIZE + GAP) * 8;

  Snake snake = Snake();
  Apple apple = Apple();

  SDL_Rect rects[8][8];

  for (int i = 0; i < 8; i++) {
    for (int j = 0; j < 8; j++) {
      SDL_Rect r{(SQUARE_SIZE + GAP) * i, (SQUARE_SIZE + GAP) * j, SQUARE_SIZE, SQUARE_SIZE};
      rects[i][j] = r;
    }
  }


  SDL_CreateWindowAndRenderer(SCREEN_WIDTH, SCREEN_HEIGHT, 0, &window, &renderer);


  int count = 0;

  while (running) {
    while (SDL_PollEvent(&e)) {
      if (e.type == SDL_QUIT) running = false;

      else if (e.type == SDL_KEYDOWN) {

        if (e.key.keysym.sym == 27) {
          running = false;
        }

        switch(e.key.keysym.sym) {
          case SDLK_UP:
            snake.move_up();
              break;
          case SDLK_DOWN:
            snake.move_down();
              break;
          case SDLK_LEFT:
            snake.move_left();
              break;
          case SDLK_RIGHT:
            snake.move_right();
            break;
        }

        if (snake.pos[0] == apple.pos[0] && snake.pos[1] == apple.pos[1]) {
          eat_apple(snake, apple);
        }
      }
    }

    SDL_SetRenderDrawColor(renderer,40,40,40,255); 
    SDL_RenderClear(renderer); 

    SDL_SetRenderDrawColor(renderer,0,0,0,255); 
    
    for (int i = 0; i < 8; i++) {
      for (int j = 0; j < 8; j++) {
        SDL_SetRenderDrawColor(renderer,0,0,0,255); 
        if (i == snake.pos[0] && j == snake.pos[1]) {
          SDL_SetRenderDrawColor(renderer,50,50,50,255);
        } 
        if (i == apple.pos[0] && j == apple.pos[1]) {
          SDL_SetRenderDrawColor(renderer,255,50,50,255);
        }
        SDL_RenderFillRect(renderer, &rects[i][j]);
      }
    }

    SDL_RenderPresent(renderer); 
  }

  return 0;
}
