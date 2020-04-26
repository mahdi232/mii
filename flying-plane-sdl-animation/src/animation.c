/*
 * Copyright (c) 2018 Amine Ben Hassouna <amine.benhassouna@gmail.com>
 * All rights reserved.
 *
 * Permission is hereby granted, free of charge, to any
 * person obtaining a copy of this software and associated
 * documentation files (the "Software"), to deal in the
 * Software without restriction, including without
 * limitation the rights to use, copy, modify, merge,
 * publish, distribute, sublicense, and/or sell copies of
 * the Software, and to permit persons to whom the Software
 * is furnished to do so, subject to the following
 * conditions:
 *
 * The above copyright notice and this permission notice
 * shall be included in all copies or substantial portions
 * of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF
 * ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED
 * TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A
 * PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT
 * SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY
 * CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION
 * OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR
 * IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
 * DEALINGS IN THE SOFTWARE.
 *
 */

#include "animation.h"
#include <SDL_mixer.h>
bool Animation_start(SDL_Renderer *renderer, int width, int height)
{
    Animation animation = {0};
    animation.renderer  = renderer;
    animation.width     = width;
    animation.height    = height;

    // Loat animation assets
    if(!Animation_load(&animation))
        return false;
    // Initialize framerate manager : 30 FPS
    FPSmanager fpsmanager;
    SDL_initFramerate(&fpsmanager);
    SDL_setFramerate(&fpsmanager, 60);

    // Initialize start frame
    int frame = SDL_getFramecount(&fpsmanager);


    // Event loop exit flag
    bool quit = false;

    // Event loop
    while(!quit)
    {
        SDL_Event e;

        // Get available event
        while(SDL_PollEvent(&e))
        {
            // Handle animation events
            quit = Animation_handleEvent(&animation, &e);
        }

        // Each new frame
        int currentFrame = SDL_getFramecount(&fpsmanager);
        if(currentFrame != frame)
        {
            // Update animation frame
            Animation_update(&animation, SDL_getFramerate(&fpsmanager));

            currentFrame = frame;
        }

        // Render animation
        Animation_render(&animation);

        // Update screen
        SDL_RenderPresent(renderer);

        // Delay
        SDL_framerateDelay(&fpsmanager);
    }

    Animation_destroy(&animation);

    return true;
}


bool Animation_load(Animation *animation)
{
    // Load sound effects
    animation->plane.jump=Mix_LoadWAV("assets/jump.wav");
        if(!animation->plane.jump)
        {
            printf("Mix_LoadWAV: %s\n", Mix_GetError());
        }

    // Load the ocean image
    if(!Background_load(animation->renderer, &animation->background, BACKGROUND_IMG))
        return false; 

    // Background move direction
    animation->background.direction = DIRECTION_RIGHT ;

    // Background speed in pixel/second
    animation->background.speed = PLANE_SPEED;

    if(!Plane_load(animation->renderer, &animation->plane, PLANE_IMG))
    {
        Background_destroy(&animation->background);
        return false;

    }

    // Background speed in pixel/second
    animation->background.speed = ENEMY_SPEED;

    if(!Enemy_load(animation->renderer, &animation->enemy, ENEMY_IMG))
    {
        Background_destroy(&animation->background);
        return false;

    }



    animation->enemy.margin = ENEMY_MARGIN;

    animation->plane.margin = PLANE_MARGIN;


    // Set plane initial position
    Plane_setX(&animation->plane, (animation->width - animation->plane.image.rect.w) /3.5 ); // Horiz. center
    Plane_setY(&animation->plane, (animation->height - animation->plane.image.rect.h - animation ->plane.margin)*0.85);
    // Set enemy initial position

    Enemy_setX(&animation->enemy, (animation->width - animation->enemy.image.rect.w) /1.4 ); // Horiz. center
    Enemy_setY(&animation->enemy, (animation->height - animation->enemy.image.rect.h - animation ->enemy.margin)*0.85);

    // Plane move direction
    animation->plane.direction = DIRECTION_STOP;

    // Enemy move direction
    animation->enemy.direction = DIRECTION_RIGHT;

    // Plane move speed in pixel/second
    animation->plane.speed = PLANE_SPEED;
// Enemy move speed in pixel/second
    animation->enemy.speed = ENEMY_SPEED;
    return true;
}

void Animation_destroy(Animation *animation)
{
    Plane_destroy(&animation->plane);
    Background_destroy(&animation->background);
    Enemy_destroy(&animation->enemy);
}

bool Animation_handleEvent(Animation *animation, SDL_Event *e)
{
    bool quit = false;

    // User requests quit
    if(e->type == SDL_QUIT)
    {
        quit = true;
    }
    else if(e->type == SDL_KEYDOWN)
    {
        switch (e->key.keysym.sym)
        {
        case SDLK_ESCAPE:
            quit = true;
            break;

        case SDLK_UP:
        case SDLK_DOWN:
        case SDLK_RIGHT:
        case SDLK_LEFT:
            Plane_setDirection(&animation->plane, e->key.keysym.sym);
            break;
        }


    }
    else if(e->type == SDL_KEYUP)
    {
        switch (e->key.keysym.sym)
        {
        case SDLK_UP:
        case SDLK_DOWN:
        case SDLK_RIGHT:
        case SDLK_LEFT:
            Plane_unsetDirection(&animation->plane, e->key.keysym.sym);
            break;
        }
    }

    return quit;
}

void Animation_update(Animation *animation, int framerate)
{
    // Move ocean
    Background_move(&animation->background, framerate);

    // Move plane
    Plane_move(&animation->plane, animation->width, animation->height, framerate);
}

void Animation_render(Animation *animation)
{
    // Render ocean
    Background_render(animation->renderer, &animation->background, animation->width, animation->height);

    // Render plane
    Plane_render(animation->renderer, &animation->plane);
    // Render enemy
    Enemy_render(animation->renderer, &animation->enemy);
}
