#pragma once
#include "Graph.h"

#if SDL_BYTEORDER == SDL_BIG_ENDIAN
static const Uint32 rmask = 0xff000000;
static const Uint32 gmask = 0x00ff0000;
static const Uint32 bmask = 0x0000ff00;
static const Uint32 amask = 0x000000ff;
#else
static const Uint32 rmask = 0x000000ff;
static const Uint32 gmask = 0x0000ff00;
static const Uint32 bmask = 0x00ff0000;
static const Uint32 amask = 0xff000000;
#endif

Graph::Graph(unsigned pxWidth, unsigned pxHeight, unsigned dpiMul, Graph::GraphRange xRanges, Graph::GraphRange yRanges,
             unsigned xtick, unsigned ytick, bool displayXTicks, bool displayYTicks) :
        xRanges(xRanges), yRanges(yRanges), pixelsWidth(pxWidth), pixelsHeight(pxHeight),
        xtick(xtick), ytick(ytick), displayXTicks(displayXTicks), displayYTicks(displayYTicks),
        surface(nullptr), renderer(nullptr),
        pixelsX0((unsigned) ((float) (xRanges.negative) * (float) pxWidth /
                             (float) (xRanges.positive + xRanges.negative))),
        pixelsY0((unsigned) ((float) yRanges.positive * (float) pxHeight /
                             (float) (yRanges.positive + yRanges.negative))),
        xRng(xRanges.positive + xRanges.negative), yRng(yRanges.positive + yRanges.negative),
        dpiMul(dpiMul){
    surface = SDL_CreateRGBSurface(0, (int) (pxWidth), (int) (pxHeight), 32, rmask, gmask, bmask, amask);
    SDL_FillRect(surface, nullptr, -1);
    renderer = SDL_CreateSoftwareRenderer(surface);

    initInterface();

    interface.render(this);
    SDL_RenderPresent(renderer);
}

void Graph::dest() {
    SDL_DestroyRenderer(renderer);
    SDL_FreeSurface(surface);
    renderer = nullptr;
    surface = nullptr;
}

SDL_Texture *Graph::getTexture(SDL_Renderer *rend) const {
    return SDL_CreateTextureFromSurface(rend, surface);;
}

GraphContent Graph::getContent() const {
    GraphContent newContent = content;
    newContent.content = newContent.content.copy();
    return newContent;
}

void Graph::setContent(const GraphContent &newContent) {
    SDL_FillRect(surface, nullptr, -1);
    this->content.dest();
    this->content = newContent;
    this->content.content = this->content.content.copy();
    this->interface.render(this);
    this->content.render(this);
    SDL_RenderPresent(renderer);
}

void Graph::initInterface() {
    const SDL_Color tickColor = {150, 150, 150, 255};
    const SDL_Color gridColor = {220, 220, 240, 100};

    if (displayXTicks) {
        for (int i = 0; i < xRng / xtick + 1; i++) {
            // grid
            interface.addLine({{(double) xtick * i, (double) yRanges.positive},
                               {(double) xtick * i, -(double) yRanges.negative}, 1,
                               gridColor});
            interface.addLine({{-(double) xtick * i, (double) yRanges.positive},
                               {-(double) xtick * i, -(double) yRanges.negative}, 1,
                               gridColor});
            // ticks
            interface.addLine({{(double) xtick * i, (double) xtick / 5},
                               {(double) xtick * i, -(double) xtick / 5}, 1,
                               tickColor});
            interface.addLine({{-(double) xtick * i, (double) xtick / 5},
                               {-(double) xtick * i, -(double) xtick / 5}, 1,
                               tickColor});
        }
    }

    if (displayYTicks) {
        for (int i = 0; i < yRng / ytick + 1; i++) {
            // grid
            interface.addLine({{-(double) xRanges.negative, (double) ytick * i},
                               {(double) xRanges.positive, (double) ytick * i}, 1,
                               gridColor});
            interface.addLine({{-(double) xRanges.negative, -(double) ytick * i},
                               {(double) xRanges.positive, -(double) ytick * i}, 1,
                               gridColor});

            // ticks
            interface.addLine({{(double) ytick / 5, (double) ytick * i},
                               {-(double) ytick / 5, (double) ytick * i}, 1,
                               tickColor});
            interface.addLine({{(double) ytick / 5, -(double) ytick * i},
                               {-(double) ytick / 5, -(double) ytick * i}, 1,
                               tickColor});
        }
    }
    interface.addArrow({{(double) (-xRanges.negative), 0}, {(double) xRanges.positive, 0},
                        1, {50, 50, 50, 255}});
    interface.addArrow({{0, (double) -yRanges.negative}, {0, (double) yRanges.positive},
                        1, {50, 50, 50, 255}});
}

void Graph::plot(double (*func)(double), unsigned pointsPerTick, unsigned width, SDL_Color color) {
    double start = -xRanges.negative;
    double increment = (double)xtick / (double)pointsPerTick;
    content.penUp();
    content.moveTo(start, func(start));
    content.penDown();
    for (unsigned i = 0; i < xRng * pointsPerTick; i++) {
        start += increment;
        content.moveTo(start, func(start), width, color);
    }
    content.render(this);
}

void Graph::matchYRange() {
    double aspectRatio = (double)pixelsHeight / (double)pixelsWidth;
    double newYRange = xRng * aspectRatio;
    double diff = newYRange - yRng;
    yRanges.negative += (diff * ((double)yRanges.negative / yRng));
    yRanges.positive += (diff * ((double)yRanges.positive / yRng));
    yRng = yRanges.positive + yRanges.negative;

    SDL_FillRect(surface, nullptr, -1);
    interface.clear();
    initInterface();
    interface.render(this);
    content.render(this);
}

void Graph::matchXRange() {
    double aspectRatio = (double)pixelsWidth / (double)pixelsHeight;
    double newXRange = yRng * aspectRatio;
    double diff = newXRange - xRng;
    xRanges.negative += (diff * ((double)xRanges.negative / xRng));
    xRanges.positive += (diff * ((double)xRanges.positive / xRng));
    xRng = xRanges.positive + xRanges.negative;

    SDL_FillRect(surface, nullptr, -1);
    interface.clear();
    initInterface();
    interface.render(this);
    content.render(this);
}
