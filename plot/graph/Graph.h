#pragma once
#include <SDL.h>
#include "../list/FastList.h"
#include "GraphContent.h"

struct Graph {
    struct GraphStyle {
        unsigned xtick = 1, ytick = 1;
        unsigned dpiMul = 1;
        bool displayXTicks = true, displayYTicks = true;
        SDL_Color background = {255, 255, 255, 255};
        SDL_Color tickColor = {150, 150, 150, 255};
        SDL_Color gridColor = {220, 220, 240, 100};
        SDL_Color axesColor = {50, 50, 50, 255};
    };
    struct GraphRange {
        double negative;
        double positive;
    };

    GraphRange xRanges, yRanges;
    unsigned pixelsWidth, pixelsHeight;

    GraphContent content;
    GraphContent interface;

    SDL_Surface* surface;
    SDL_Renderer* renderer;

    GraphStyle style;

    int pixelsX0, pixelsY0;
    double xRng, yRng;

    Graph(unsigned pxWidth, unsigned pxHeight, unsigned dpiMul, GraphRange xRanges, GraphRange yRanges,
          GraphStyle style);

    void dest();

    [[nodiscard]] GraphContent getContent() const;

    void setContent(const GraphContent &newcontent);

    void initInterface();

    void updateInterface();

    void updateRanges();

    SDL_Texture* getTexture(SDL_Renderer* rend) const;

    void matchYRange();

    void matchXRange();

    void render();
};
