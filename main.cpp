#include <SDL.h>
#include <SDL_image.h>
#include <SDL_timer.h>
#include <SDL_render.h>
#include <SDL2_gfxPrimitives.h>
#include "plot/graph/Graph.h"

void SDLInit(SDL_Window *&win, int *w, int *h, SDL_Renderer *&rend);

void SDLEventLoop(SDL_Renderer *rend, SDL_Texture *texSmall, SDL_Rect *dest, SDL_Rect *destSmall,
                  Graph *graph, SDL_Texture *tex);

double func(double x) {
    return 10 * exp(-x / 10) *  sin(x);
}

int main(int argc, char *argv[]) {
    SDL_Window *win = nullptr;
    unsigned int dpiMul = 2;
    int w = 0, h = 0;
    SDL_Renderer *rend = nullptr;
    SDLInit(win, &w, &h, rend);

    unsigned mainGraphWidth = w / 2, mainGraphHeight = h / 2;
    unsigned smallGraphWidth = 400, smallGraphHeight = 400;

    Graph graph(mainGraphWidth, mainGraphHeight, dpiMul,
                {10, 20}, {10, 10}, Graph::GraphStyle());
    graph.matchXRange();
    graph.content.addVector({3, 5}, {4, 3});
    graph.content.addPrimitive(GraphPrimitive::createFunc(func, 5, 1, {32, 99, 155, 255}));
    graph.render();
    SDL_Texture* tex = graph.getTexture(rend);

    Graph graphSmall(smallGraphWidth * dpiMul, smallGraphHeight * dpiMul, dpiMul,
                     {1, 20}, {5, 10}, Graph::GraphStyle());
    graphSmall.matchXRange();
    graphSmall.setContent(graph.getContent());
    graphSmall.render();
    SDL_Texture* texSmall = graphSmall.getTexture(rend);

    SDL_Rect dest = {}, destSmall = {};
    SDL_QueryTexture(tex, NULL, NULL, &dest.w, &dest.h);
    SDL_QueryTexture(texSmall, NULL, NULL, &destSmall.w, &destSmall.h);

    destSmall.x = w - 100 - smallGraphWidth * dpiMul;
    destSmall.y = h - 100 - smallGraphHeight * dpiMul;

    dest.x = 20;
    dest.y = 20;

    SDLEventLoop(rend, texSmall, &dest, &destSmall, &graph, tex);

    SDL_DestroyTexture(tex);
    SDL_DestroyTexture(texSmall);
    SDL_DestroyRenderer(rend);
    SDL_DestroyWindow(win);
    SDL_Quit();

    graphSmall.dest();
    graph.dest();

    return 0;
}

void SDLEventLoop(SDL_Renderer *rend, SDL_Texture *texSmall, SDL_Rect *dest, SDL_Rect *destSmall,
                  Graph *graph, SDL_Texture *tex) {
    int close = 0, speed = 300;
    bool mousePressed = false;
    SDL_MouseMotionEvent pressPosition = {};
    while (!close) {
        SDL_Event event = {};
        while (SDL_PollEvent(&event)) {
            switch (event.type) {
                case SDL_MOUSEMOTION:{
                    if (mousePressed) {
                        SDL_MouseMotionEvent drag = event.motion;
                        double xDrag = (drag.x - (double)pressPosition.x) / graph->pixelsWidth * (double)graph->xRng * graph->style.dpiMul;
                        double yDrag = (drag.y - (double)pressPosition.y) / graph->pixelsHeight * (double)graph->yRng * graph->style.dpiMul;
                        graph->yRanges.positive += yDrag;
                        graph->yRanges.negative -= yDrag;
                        graph->xRanges.positive -= xDrag;
                        graph->xRanges.negative += xDrag;
                        graph->updateInterface();
                        graph->render();
                        tex = graph->getTexture(rend);
                        pressPosition = event.motion;
                    }
                    break;
                }
                case SDL_MOUSEBUTTONDOWN:{
                    mousePressed = true;
                    pressPosition = event.motion;
                    break;
                }
                case SDL_MOUSEBUTTONUP:{
                    mousePressed = false;
                    break;
                }
                case SDL_MOUSEWHEEL:{
                    graph->yRanges.positive += (double)event.wheel.y / 2;
                    graph->yRanges.negative += (double)event.wheel.y / 2;
                    graph->matchXRange();
                    graph->updateInterface();
                    tex = graph->getTexture(rend);
                    break;
                }
                case SDL_QUIT:
                    // handling of close button
                    close = 1;
                    break;
                case SDL_KEYDOWN:
                    // keyboard API for key pressed
                    switch (event.key.keysym.scancode) {
                        case SDL_SCANCODE_W:
                        case SDL_SCANCODE_UP:
                            dest->y -= speed / 30;
                            break;
                        case SDL_SCANCODE_A:
                        case SDL_SCANCODE_LEFT:
                            dest->x -= speed / 30;
                            break;
                        case SDL_SCANCODE_S:
                        case SDL_SCANCODE_DOWN:
                            dest->y += speed / 30;
                            break;
                        case SDL_SCANCODE_D:
                        case SDL_SCANCODE_RIGHT:
                            dest->x += speed / 30;
                            break;
                        default:
                            break;
                    }
            }
        }
        SDL_RenderClear(rend);
        SDL_RenderCopy(rend, tex, NULL, dest);
        SDL_RenderCopy(rend, texSmall, NULL, destSmall);
        SDL_RenderPresent(rend);

        // calculates to 60 fps
        SDL_Delay(1000 / 60);
    }
}

void SDLInit(SDL_Window *&win, int *w, int *h, SDL_Renderer *&rend) {
    win = SDL_CreateWindow("Graph", // creates a window
                          SDL_WINDOWPOS_CENTERED,
                          SDL_WINDOWPOS_CENTERED,
                          3000, 3000,
                          SDL_WINDOW_ALLOW_HIGHDPI | SDL_WINDOW_SHOWN | SDL_WINDOW_FULLSCREEN_DESKTOP);
    Uint32 render_flags = SDL_RENDERER_ACCELERATED;
    rend = SDL_CreateRenderer(win, -1, render_flags);
    SDL_GL_SetAttribute(SDL_GL_MULTISAMPLEBUFFERS, 2);
    if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
        printf("error initializing SDL: %s\n", SDL_GetError());
    }
    SDL_GL_GetDrawableSize(win, w, h);
    SDL_SetRenderDrawColor(rend, 100, 100, 100, 255);
    SDL_RenderFillRect(rend, nullptr);
}
