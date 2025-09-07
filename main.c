#include <SDL.h>
#include <SDL_ttf.h>

#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define SCREEN_WIDTH_PX 400
#define SCREEN_HEIGHT_PX 600
#define SCREEN_WIDTH 3
#define SCREEN_HEIGHT 3

#define RED 255, 0, 0
#define BLUE 0, 0, 255
#define GRAY 230, 230, 230
#define WHITE 255, 255, 255

// number of particles
#define N 100

// time step
#define TS 0.5f

typedef struct {
    float x, y;
} Position;
typedef struct {
    float x, y;
} Velocity;

typedef struct {
    Position position;
    Velocity velocity;
} Particle;

void rerender_background(SDL_Renderer* renderer, uint8_t r, uint8_t g, uint8_t b) {
    SDL_SetRenderDrawColor(renderer, r, g, b, 255);
    SDL_RenderClear(renderer);
}

void render_particle(SDL_Renderer* renderer, Particle* particle) {
    SDL_SetRenderDrawColor(renderer, RED, 255);

    // could just take in SDL_Rect instead of
    // taking up more memory...

    particle->position.x += particle->velocity.x * TS;
    particle->position.y += particle->velocity.y * TS;

    SDL_RenderDrawPoint(renderer, (int)particle->position.x, (int)particle->position.y);
}

int main() {
    srand((unsigned)time(NULL));

    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        fprintf(stderr, "SDL_Init failed: %s\n", SDL_GetError());
        return 1;
    }

    SDL_Window* window;
    SDL_Renderer* renderer;
    window = SDL_CreateWindow("Tic Tac Toe", 0, 0, SCREEN_WIDTH_PX, SCREEN_HEIGHT_PX, SDL_WINDOW_HIDDEN);
    if (!window) {
        fprintf(stderr, "SDL_CreateWindow failed: %s\n", SDL_GetError());
        SDL_Quit();
        return 1;
    }
    SDL_ShowWindow(window);

    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (!renderer) {
        fprintf(stderr, "SDL_CreateRenderer failed: %s\n", SDL_GetError());
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 1;
    }

    int running = 1;
    int init = 1;
    int mouse_x, mouse_y;

    Particle list[N];

    for (int i = 0; i < N; ++i) {
        list[i].position.x = (float)(rand() % SCREEN_WIDTH_PX);
        list[i].position.y = (float)(rand() % SCREEN_HEIGHT_PX);
        list[i].velocity.x = (float)((rand() % 7) - 3); // range -3..3
        list[i].velocity.y = (float)((rand() % 7) - 3);
    }

    while (running) {
        // redraw background for next frame
        SDL_SetRenderDrawColor(renderer, WHITE, 255);
        SDL_RenderClear(renderer);

        // redraw gameboard
        for (int i = 0; i < N; ++i) render_particle(renderer, &list[i]);

        // present the rendered frame
        SDL_RenderPresent(renderer);

        SDL_Delay(16);
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}
