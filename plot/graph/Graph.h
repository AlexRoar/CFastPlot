#pragma once
#include <SDL.h>
#include "../list/FastList.h"
#include "GraphContent.h"

struct Graph {
    struct GraphStyle {
        unsigned xtick = 1, ytick = 1;
        unsigned dpiMul = 2;
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

    Graph(unsigned pxWidth, unsigned pxHeight, GraphRange xRanges, GraphRange yRanges, GraphStyle style);

    [[nodiscard]] int getPixelsX0() const;

    [[nodiscard]] int getPixelsY0() const;

    [[nodiscard]] bool isRenderNeeded() const;

    ~Graph();

    [[nodiscard]] GraphRange getXRanges() const;

    void setXRanges(const GraphRange &xRangesNew);

    [[nodiscard]] GraphRange getYRanges() const;

    void setYRanges(const GraphRange &yRangesNew);

    [[nodiscard]] unsigned getPixelsWidth() const;

    [[nodiscard]] unsigned getPixelsHeight() const;

    [[nodiscard]] SDL_Surface *getSurface() const;

    [[nodiscard]] SDL_Renderer *getRenderer() const;

    [[nodiscard]] GraphStyle getStyle() const;

    void setStyle(const GraphStyle &styleNew);

    [[nodiscard]] double getXRng() const;

    [[nodiscard]] double getYRng() const;

    [[nodiscard]] GraphContent getContent() const;

    void setContent(const GraphContent &newcontent);

    void initInterface();

    SDL_Texture* getTexture(SDL_Renderer* rend);

    void matchYRange();

    void matchXRange();

private:
    void render();

    void updateRanges();

    void updateInterface();

    GraphRange xRanges, yRanges;
    unsigned pixelsWidth, pixelsHeight;

    GraphContent content;
    GraphContent interface;

    SDL_Surface* surface;
    SDL_Renderer* renderer;

    GraphStyle style;

    int pixelsX0, pixelsY0;

    double xRng, yRng;

    bool renderNeeded;
};
