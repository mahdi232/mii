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

#include "background.h"

bool Background_load(SDL_Renderer *renderer, Background *background, const char *file)
{
    return Image_load(renderer, &background->image, file);
}

void Background_destroy(Background *background)
{
    Image_destroy(&background->image);
}

void Background_move(Background *background, int framerate)
{
    background->image.rect.x -= background->speed / framerate;
    if(background->image.rect.x <= - background->image.rect.w)
        background->image.rect.x = 0;
}
void Background_render(SDL_Renderer *renderer, Background *background, int width , int height)
{
    // Unused height
    (void) width;

    SDL_Rect crop = background->image.rect;

    if(background->direction == DIRECTION_RIGHT )
    {
        int remaining = background->image.rect.w + background->image.rect.x;

        if(background->image.rect.x > - background->image.rect.w )
        {
            crop.x = - background->image.rect.x;
            crop.w = (remaining > width) ? width : remaining;


            SDL_Point pos = {0, 0 };
            Image_renderCrop(renderer, &background->image, &crop, &pos);
        }

        if(remaining < width)
        {
            crop.x = 0;
            crop.w = width - remaining;

            SDL_Point pos = { remaining, 0 };
            Image_renderCrop(renderer, &background->image, &crop, &pos);
        }
    }
}
