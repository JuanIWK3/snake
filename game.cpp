#include <SDL2/SDL.h>
#include <SDL2/SDL_events.h>
#include <SDL2/SDL_render.h>
#include <SDL2/SDL_timer.h>
#include <iostream>
#include <string>
#include <valarray>
#include <vector>
#include <cstdlib>
#include <unistd.h>
#include <chrono>
#include <thread>

using namespace std;

const int SQUARES = 16;
const int SQUARE_SIZE = 50;
const int GAP = 1;
const int SCREEN_WIDTH = (SQUARE_SIZE + GAP) * SQUARES;
const int SCREEN_HEIGHT = (SQUARE_SIZE + GAP) * SQUARES;

struct GameState
{
  bool running = true;
};

struct Snake
{
  vector<valarray<int>> body_squares;
  int direction = 1;

  Snake()
  {
    body_squares.push_back({4, 7});
  }

  void change_direction(int direction)
  {
    this->direction = direction;
  }

  void remove_tail()
  {
    this->body_squares.pop_back();
  }

  bool check_wall_collision()
  {
    auto head = this->body_squares[0];
    if (head[0] < 0 || head[0] >= SQUARES || head[1] < 0 || head[1] >= SQUARES)
    {
      return true;
    }

    return false;
  }

  bool check_body_collision()
  {
    auto head = this->body_squares[0];
    for (int i = 1; i < this->body_squares.size(); i++)
    {
      if (head[0] == this->body_squares[i][0] && head[1] == this->body_squares[i][1])
      {
        return true;
      }
    }

    return false;
  }

  void move()
  {
    switch (direction)
    {
    case 0:
      this->body_squares.insert(this->body_squares.begin(), {this->body_squares[0][0], this->body_squares[0][1] - 1});
      this->remove_tail();
      break;
    case 1:
      this->body_squares.insert(this->body_squares.begin(), {this->body_squares[0][0] + 1, this->body_squares[0][1]});
      this->remove_tail();
      break;
    case 2:
      this->body_squares.insert(this->body_squares.begin(), {this->body_squares[0][0], this->body_squares[0][1] + 1});
      this->remove_tail();
      break;
    case 3:
      this->body_squares.insert(this->body_squares.begin(), {this->body_squares[0][0] - 1, this->body_squares[0][1]});
      this->remove_tail();
      break;
    }
  }
};

struct Apple
{
  valarray<int> position = {0, 0};

  Apple()
  {
    this->position = {rand() % SQUARES, rand() % SQUARES};
  }

  void move()
  {
    this->position = {rand() % SQUARES, rand() % SQUARES};
  }
};

void eat_apple(Snake &snake, Apple &apple)
{
}

int main()
{
  SDL_Window *window = nullptr;
  SDL_Renderer *renderer = nullptr;
  SDL_Event e;
  SDL_Init(SDL_INIT_EVERYTHING);

  bool running = true;

  GameState state = GameState();
  Snake snake = Snake();
  Apple apple = Apple();

  SDL_Rect rects[SQUARES][SQUARES];

  for (int i = 0; i < SQUARES; i++)
  {
    for (int j = 0; j < SQUARES; j++)
    {
      SDL_Rect r{(SQUARE_SIZE + GAP) * i, (SQUARE_SIZE + GAP) * j, SQUARE_SIZE, SQUARE_SIZE};
      rects[i][j] = r;
    }
  }

  SDL_CreateWindowAndRenderer(SCREEN_WIDTH, SCREEN_HEIGHT, 0, &window, &renderer);

  int count = 0;

  while (state.running)
  {
    while (SDL_PollEvent(&e))
    {
      if (e.type == SDL_QUIT)
        state.running = false;

      else if (e.type == SDL_KEYDOWN)
      {

        if (e.key.keysym.sym == 27)
        {
          state.running = false;
        }

        switch (e.key.keysym.sym)
        {
        case SDLK_UP:
          snake.change_direction(0);
          break;
        case SDLK_DOWN:
          snake.change_direction(2);
          break;
        case SDLK_LEFT:
          snake.change_direction(3);
          break;
        case SDLK_RIGHT:
          snake.change_direction(1);
          break;
        }
      }
    }

    snake.move();

    // checks for collisions

    if (snake.check_wall_collision())
    {
      state.running = false;
    }

    if (snake.check_body_collision())
    {
      state.running = false;
    }

    if (snake.body_squares[0][0] == apple.position[0] && snake.body_squares[0][1] == apple.position[1])
    {
      snake.body_squares.push_back({apple.position[0], apple.position[1]});
      apple.move();
    }

    SDL_SetRenderDrawColor(renderer, 40, 40, 40, 255);
    SDL_RenderClear(renderer);

    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);

    for (int i = 0; i < SQUARES; i++)
    {
      for (int j = 0; j < SQUARES; j++)
      {
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);

        for (auto snake_square : snake.body_squares)
        {
          if (i == snake_square[0] && j == snake_square[1])
          {
            SDL_SetRenderDrawColor(renderer, 0, 200, 0, 255);
          }
        }

        if (i == apple.position[0] && j == apple.position[1])
        {
          SDL_SetRenderDrawColor(renderer, 200, 0, 0, 255);
        }

        SDL_RenderFillRect(renderer, &rects[i][j]);
      }
    }

    SDL_RenderPresent(renderer);

    SDL_Delay(100);
  }

  return 0;
}
