#include "SDL.h"
#include "SDL_gpu.h"
#include "compat.h"
#include "common.h"


int main(int argc, char* argv[])
{
	GPU_Target* screen;

	printRenderers();

	screen = GPU_Init(800, 600, GPU_DEFAULT_INIT_FLAGS);
	if(screen == NULL)
		return -1;

	printCurrentRenderer();

	{
		Uint32 startTime;
		long frameCount;
		Uint8 done;
		SDL_Event event;

        float dt;
        const Uint8* keystates;

        #define MAX_SPRITES 50
        int numSprites = 1;

        float x[MAX_SPRITES];
        float y[MAX_SPRITES];
        float velx[MAX_SPRITES];
        float vely[MAX_SPRITES];
        int i;

        GPU_Rect buffer_viewport = GPU_MakeRect(400, 20, 400, 580);
        GPU_Rect buffer_screen_viewport = GPU_MakeRect(20, 20, 100, 100);
        GPU_Rect small_viewport = GPU_MakeRect(600, 20, 100, 100);
        GPU_Rect viewport = GPU_MakeRect(100, 100, 600, 400);

		GPU_Image* buffer;

        GPU_Image* image = GPU_LoadImage("data/test.bmp");
        if(image == NULL)
            return -1;

        buffer = GPU_CreateImage(800, 600, GPU_FORMAT_RGB);
        GPU_LoadTarget(buffer);

        startTime = SDL_GetTicks();
        frameCount = 0;

        for(i = 0; i < MAX_SPRITES; i++)
        {
            x[i] = rand()%screen->w;
            y[i] = rand()%screen->h;
            velx[i] = 10 + rand()%screen->w/10;
            vely[i] = 10 + rand()%screen->h/10;
        }


        keystates = SDL_GetKeyState(NULL);

        dt = 0.010f;
        done = 0;
        while(!done)
        {
            while(SDL_PollEvent(&event))
            {
                if(event.type == SDL_QUIT)
                    done = 1;
                else if(event.type == SDL_KEYDOWN)
                {
                    if(event.key.key == SDLK_ESCAPE)
                        done = 1;
                    else if(event.key.key == SDLK_r)
                    {
                        viewport = GPU_MakeRect(0.0f, 0.0f, screen->w, screen->h);
                    }
                    else if(event.key.key == SDLK_EQUALS || event.key.key == SDLK_PLUS)
                    {
                        if(numSprites < MAX_SPRITES)
                            numSprites++;
                    }
                    else if(event.key.key == SDLK_MINUS)
                    {
                        if(numSprites > 0)
                            numSprites--;
                    }
                }
            }

            if(keystates[KEY_UP])
                viewport.y -= 100*dt;
            else if(keystates[KEY_DOWN])
                viewport.y += 100*dt;

            if(keystates[KEY_LEFT])
                viewport.x -= 100*dt;
            else if(keystates[KEY_RIGHT])
                viewport.x += 100*dt;

            if(keystates[KEY_w])
                viewport.h -= 100*dt;
            else if(keystates[KEY_s])
                viewport.h += 100*dt;

            if(keystates[KEY_a])
                viewport.w -= 100*dt;
            else if(keystates[KEY_d])
                viewport.w += 100*dt;

            for(i = 0; i < numSprites; i++)
            {
                x[i] += velx[i]*dt;
                y[i] += vely[i]*dt;
                if(x[i] < 0)
                {
                    x[i] = 0;
                    velx[i] = -velx[i];
                }
                else if(x[i]> screen->w)
                {
                    x[i] = screen->w;
                    velx[i] = -velx[i];
                }

                if(y[i] < 0)
                {
                    y[i] = 0;
                    vely[i] = -vely[i];
                }
                else if(y[i]> screen->h)
                {
                    y[i] = screen->h;
                    vely[i] = -vely[i];
                }
            }


            GPU_UnsetClip(screen);
            GPU_Clear(screen);

            // Draw on buffer
            GPU_ClearRGBA(buffer->target, 100, 0, 0, 0);

            GPU_SetViewport(buffer->target, buffer_viewport);
            for(i = 0; i < numSprites; i++)
            {
                GPU_Blit(image, NULL, buffer->target, x[i], y[i]);
            }

            // Draw buffer to screen
            GPU_SetViewport(screen, buffer_screen_viewport);
            GPU_Blit(buffer, NULL, screen, screen->w/2, screen->h/2);



            GPU_SetClipRect(screen, small_viewport);
            GPU_ClearRGBA(screen, 0, 100, 0, 0);

            GPU_SetViewport(screen, small_viewport);
            for(i = 0; i < numSprites; i++)
            {
                GPU_Blit(image, NULL, screen, x[i], y[i]);
            }


            GPU_SetClipRect(screen, viewport);
            GPU_ClearRGBA(screen, 0, 0, 100, 0);

            GPU_SetViewport(screen, viewport);
            for(i = 0; i < numSprites; i++)
            {
                GPU_Blit(image, NULL, screen, x[i], y[i]);
            }

            GPU_Flip(screen);

            frameCount++;
            if(frameCount%500 == 0)
                printf("Average FPS: %.2f\n", 1000.0f*frameCount/(SDL_GetTicks() - startTime));
        }

        printf("Average FPS: %.2f\n", 1000.0f*frameCount/(SDL_GetTicks() - startTime));

        GPU_FreeImage(image);
	}

	GPU_Quit();

	return 0;
}


