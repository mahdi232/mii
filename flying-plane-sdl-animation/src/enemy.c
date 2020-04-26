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

#include "enemy.h"

bool Enemy_load(SDL_Renderer *renderer, Enemy *enemy, const char *image)
{
    // Load enemy image
    if(!Image_load(renderer, &enemy->image, image))
    {
        return false;
    }



    return true;
}

void Enemy_destroy(Enemy *enemy)
{
    Image_destroy(&enemy->image);
}

void Enemy_setX(Enemy *enemy, int x)
{
    // Enemy x coordinate
    enemy->image.rect.x = x;
}


void Enemy_setY(Enemy *enemy, int y )
{
    // Enemy y coordinate
    enemy->image.rect.y = y;
}




void Enemy_setCoordinates(Enemy *enemy, int x, int y)
{
    Enemy_setX(enemy, x);


    Enemy_setY(enemy, y);
}

void Enemy_moveX(Enemy *enemy, int x)
{
    Enemy_setX(enemy, enemy->image.rect.x + x);
}



void Enemy_moveY(Enemy *enemy, int y)
{
    Enemy_setY(enemy, enemy->image.rect.y + y);
}


void Enemy_setDirection(Enemy *enemy, SDL_Keycode keycode)
{
    switch (keycode)
    {
    case SDLK_UP:
        enemy->direction &= ~DIRECTION_DOWN;
        enemy->direction |= DIRECTION_UP;
        break;

    case SDLK_DOWN:
        enemy->direction &= ~SDLK_UP;
        enemy->direction |= DIRECTION_DOWN;
        break;

    case SDLK_RIGHT:
        enemy->direction &= ~SDLK_LEFT;
        enemy->direction |= DIRECTION_RIGHT;
        break;

    case SDLK_LEFT:
        enemy->direction &= ~DIRECTION_RIGHT;
        enemy->direction |= DIRECTION_LEFT;
        break;
    }
}

void Enemy_unsetDirection(Enemy *enemy, SDL_Keycode keycode)
{
    switch (keycode)
    {
    case SDLK_UP:
        enemy->direction &= ~DIRECTION_UP;
        break;

    case SDLK_DOWN:
        enemy->direction &= ~DIRECTION_DOWN;
        break;

    case SDLK_RIGHT:
        enemy->direction &= ~DIRECTION_RIGHT;
        break;

    case SDLK_LEFT:
        enemy->direction &= ~DIRECTION_LEFT;
        break;
    }
}

void Enemy_render(SDL_Renderer *renderer, Enemy *enemy)
{


    // Render enemy
  //  Image_render(renderer, &enemy->image);
}

void Enemy_move(Enemy *enemy, int screenWidth, int screenHeight, int framerate)
{
    int enemyStep = enemy->speed / framerate;

    if( (enemy->direction & DIRECTION_UP)
            && enemy->image.rect.y - enemyStep >= enemy->margin)
    {
        Enemy_moveY(enemy, -enemyStep);
    }
    else if( (enemy->direction & DIRECTION_DOWN)
             && enemy->image.rect.y + enemy->image.rect.h + enemyStep <= screenHeight - enemy->margin)
    {
        Enemy_moveY(enemy, enemyStep);
    }

    if( (enemy->direction & DIRECTION_RIGHT)
            && enemy->image.rect.x + enemy->image.rect.w + enemyStep <= screenWidth - enemy->margin)
    {
        Enemy_moveX(enemy, enemyStep);
    }
    else if( (enemy->direction & DIRECTION_LEFT)
             && enemy->image.rect.x - enemyStep >= enemy->margin)
    {
        Enemy_moveX(enemy, -enemyStep);
    }
}
