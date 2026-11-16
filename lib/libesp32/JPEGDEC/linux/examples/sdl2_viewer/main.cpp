#include <SDL2/SDL.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <JPEGDEC.h>
JPEGDEC jpg;
uint8_t *pFrameBuf; // 16-bit combined frame buffer
int w, h;

int JPEGDraw(JPEGDRAW *pDraw)
{
    uint16_t *d, *s;
    
    d = (uint16_t *)pFrameBuf;
    d += pDraw->x;
    d += (pDraw->y * w);
    s = pDraw->pPixels;
    for (int y=0; y<pDraw->iHeight; y++) {
        memcpy(d, s, pDraw->iWidth * sizeof(uint16_t));
        d += w;
        s += pDraw->iWidth;
    }
    return 1;
} /* JPEGDraw() */

int main(int argc, char *argv[])
{
    SDL_Window *win;
    SDL_Surface *canvas, *winSurface;
    void *pOld;
    
    if (argc != 2) {
        printf("sdl2 jpeg viewer\nUsage: sdl2_viewer <filename>\n");
        return -1;
    }
    if (!jpg.open(argv[1], JPEGDraw)) {
    	printf("Error opening %s = %d\n", argv[1], jpg.getLastError());
    	return -1;
    }
    jpg.setPixelType(RGB565_LITTLE_ENDIAN);
    w = jpg.getWidth(); h = jpg.getHeight();
    if (jpg.getJPEGType() == JPEG_MODE_PROGRESSIVE) {
        // N.B. Progressive mode only supports decoding the first 'scan'
        // The resulting image is a 1/8th thumbnail
        w >>= 3; h >>= 3;
    }
    if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
        printf("SDL_Init Error: %s\n", SDL_GetError());
        return EXIT_FAILURE;
    }

    pFrameBuf = (uint8_t *)malloc(w * h * 2); // 16-bit frame
    //jpg.setFramebuffer(pFrameBuf);

    win = SDL_CreateWindow("JPEGDEC viewer", 100, 100, w, h, SDL_WINDOW_SHOWN);
    if (win == nullptr) {
        printf("SDL_CreateWindow Error: %s\n", SDL_GetError());
        return EXIT_FAILURE;
    }
    // Create a surface to hold the JPEG image
    canvas = SDL_CreateRGBSurfaceWithFormat(0, w, h, 16, SDL_PIXELFORMAT_RGB565);
    if (canvas == nullptr) {
        printf("SDL_CreateSurface error %s\n", SDL_GetError());
        SDL_DestroyWindow(win);
	SDL_Quit();
        free(pFrameBuf);
	return EXIT_FAILURE;
    }
    pOld = canvas->pixels; // keep old pixel pointer; we will substitute our own
    canvas->pixels = pFrameBuf; // point to the pixels the library will generate
    winSurface = SDL_GetWindowSurface(win);
    jpg.decode(0,0,0);
    SDL_BlitSurface(canvas, NULL, winSurface, NULL);
    SDL_UpdateWindowSurface(win);

    bool bQuit = false;
    while (!bQuit) {
        SDL_Event e;
        while (SDL_PollEvent(&e)) { // take care of queued events
            if (e.type == SDL_QUIT || e.type == SDL_KEYDOWN) {
                bQuit = true;
            }
        }
    }

    // Clean up
    canvas->pixels = pOld; // restore original pointer
    SDL_FreeSurface(canvas);
    SDL_FreeSurface(winSurface);
    SDL_DestroyWindow(win);
    SDL_Quit();
    free(pFrameBuf);

    return EXIT_SUCCESS;
} /* main() */
