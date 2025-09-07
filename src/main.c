#include <SDL.h>
#include <SDL_ttf.h>

#include "utils.h"

#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#define SCREEN_WIDTH_PX 400
#define SCREEN_HEIGHT_PX 600

#define RED 255, 0, 0
#define BLUE 0, 0, 255
#define GRAY 230, 230, 230
#define WHITE 255, 255, 255

// number of particles
#define N 10

#define PARTICLE_SIZE 10.f

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

enum WallCollision {
    Left,
    Right,
    Top,
    Bottom,
    None,
};

enum WallCollision to_collide(Particle* ptcl) {
    float nx = ptcl->velocity.x * TS + ptcl->position.x;
    float ny = ptcl->velocity.y * TS + ptcl->position.y;

    if (nx > SCREEN_WIDTH_PX - PARTICLE_SIZE / 2) return Right;
    else if (ny > SCREEN_HEIGHT_PX - PARTICLE_SIZE / 2) return Bottom;
    else if (nx < PARTICLE_SIZE / 2) return Left;
    else if (ny < PARTICLE_SIZE / 2) return Top;
    else return None;
}

void render_particle(SDL_Renderer* renderer, Particle* ptcl) {
    SDL_SetRenderDrawColor(renderer, RED, 255);

    // could just take in SDL_Rect instead of
    // taking up more memory...

    ptcl->position.x += ptcl->velocity.x * TS;
    ptcl->position.y += ptcl->velocity.y * TS;

    enum WallCollision c = to_collide(ptcl);

    switch (c) {
    case None:
        break;
    case Left:
    case Right: {
        ptcl->velocity.x *= -1;
        break;
    }
    case Top:
    case Bottom: {
        ptcl->velocity.y *= -1;
    }
    }

    SDL_Rect rect = {
        .x = (int)(ptcl->position.x - (PARTICLE_SIZE / 2.0)),
        .y = (int)(ptcl->position.y - (PARTICLE_SIZE / 2.0)),
        .h = (int)(PARTICLE_SIZE),
        .w = (int)(PARTICLE_SIZE),
    };

    SDL_RenderFillRect(renderer, &rect);
    // SDL_RenderDrawPoint(renderer, (int)particle->position.x, (int)particle->position.y);
}

int main() {
    rng_seed(1234);

    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        fprintf(stderr, "SDL_Init failed: %s\n", SDL_GetError());
        return 1;
    }

    SDL_Window* window;
    SDL_Renderer* renderer;
    window = SDL_CreateWindow("Particles", 0, 0, SCREEN_WIDTH_PX, SCREEN_HEIGHT_PX, SDL_WINDOW_HIDDEN);
    if (!window) {
        fprintf(stderr, "SDL_CreateWindow failed: %s\n", SDL_GetError());
        SDL_Quit();
        return 1;
    }
    SDL_ShowWindow(window);

    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (!renderer) {
        fprintf(stderr, "SDL_CreateRenderer failed: %s\n", SDL_GetError());
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 1;
    }

    SDL_Event event;
    int running = 1;
    int init = 1;
    int mouse_x, mouse_y;

    Particle list[N];

    for (int i = 0; i < N; ++i) {
        list[i].position.x = (float)(rand() % SCREEN_WIDTH_PX);
        list[i].position.y = (float)(rand() % SCREEN_HEIGHT_PX);
        list[i].velocity.x = randf_range(-30.f, 30.f);
        list[i].velocity.y = randf_range(-30.f, 30.f);
    }

    while (running) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                running = 0;
            } else if (event.type == SDL_MOUSEBUTTONDOWN) {
                SDL_GetMouseState(&mouse_x, &mouse_y);
            }
        }
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
