#pragma once
#include <SDL.h>
#include "../list/FastList.h"
#include "GraphContent.h"

struct Graph {
    struct GraphRange {
        double negative;
        double positive;
    };

    GraphRange xRanges, yRanges;
    unsigned pixelsWidth, pixelsHeight;
    unsigned xtick, ytick;
    unsigned dpiMul;

    bool displayXTicks, displayYTicks;

    GraphContent content;
    GraphContent interface;

    SDL_Surface* surface;
    SDL_Renderer* renderer;

    unsigned pixelsX0, pixelsY0;
    double xRng, yRng;

    Graph(unsigned pxWidth, unsigned pxHeight, unsigned dpiMul, GraphRange xRanges, GraphRange yRanges,
          unsigned xtick = 1, unsigned ytick = 1, bool displayXTicks = true, bool displayYTicks = true);

    void dest();

    [[nodiscard]] GraphContent getContent() const;

    void setContent(const GraphContent &newcontent);

    void initInterface();

    SDL_Texture* getTexture(SDL_Renderer* rend) const;

    void plot(double(*func)(double), unsigned pointsPerTick=10, unsigned width=1, SDL_Color color={32, 99, 155, 255});

    void matchYRange();

    void matchXRange();

    void render();
};
