#pragma once
#include "Graph.h"
#include "../sdl/GfxExtensions.h"

Graph::Graph(unsigned pxWidth, unsigned pxHeight, unsigned dpiMul, Graph::GraphRange xRanges, Graph::GraphRange yRanges,
             GraphStyle style) :
        xRanges(xRanges), yRanges(yRanges), pixelsWidth(pxWidth), pixelsHeight(pxHeight), style(style),
        surface(nullptr), renderer(nullptr),
        pixelsX0((int) ((float) (xRanges.negative) * (float) pxWidth /
                             (float) (xRanges.positive + xRanges.negative))),
        pixelsY0((int) ((float) yRanges.positive * (float) pxHeight /
                             (float) (yRanges.positive + yRanges.negative))),
        xRng(xRanges.positive + xRanges.negative), yRng(yRanges.positive + yRanges.negative) {
    surface = SDL_CreateRGBSurface(0, (int) (pxWidth), (int) (pxHeight), 32, rmask, gmask, bmask, amask);
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

void Graph::render(){
    updateRanges();
    SDL_FillRect(surface, nullptr, ColorToUint(style.background));
    this->interface.render(this);
    this->content.render(this);
}

void Graph::setContent(const GraphContent &newContent) {
    this->content.dest();
    this->content = newContent;
    this->content.content = this->content.content.copy();
    render();
    SDL_RenderPresent(renderer);
}

void Graph::initInterface() {
    if (style.displayXTicks) {
        double startPos = -xRanges.negative;
        startPos = floor(startPos);

        for (int i = 0; i < xRng / style.xtick + 1; i++) {
            // grid
            interface.addPrimitive(GraphPrimitive::createLine({(double) startPos, (double) yRanges.positive},
                                                               {(double) startPos, -(double) yRanges.negative}, 1,
                                                              style.gridColor));

            // ticks
            interface.addPrimitive(GraphPrimitive::createLine({(double) startPos, (double) style.xtick / 5},
                               {(double) startPos, -(double) style.xtick / 5}, 1,
                                                              style.tickColor));
            startPos += style.xtick;
        }
    }

    if (style.displayYTicks) {
        double startPos = -yRanges.negative;
        startPos = floor(startPos);

        for (int i = 0; i < yRng / style.ytick + 1; i++) {
            // grid
            interface.addPrimitive(GraphPrimitive::createLine({(double) xRanges.positive, (double) startPos },
                                                              {-(double) xRanges.negative,(double) startPos }, 1,
                                                              style.gridColor));

            // ticks
            interface.addPrimitive(GraphPrimitive::createLine({(double) style.ytick / 5, (double) startPos},
                                                              {-(double) style.ytick / 5, (double) startPos}, 1,
                                                              style.tickColor));
            startPos += style.ytick;
        }
    }
    interface.addPrimitive(GraphPrimitive::createArrow({(double) (-xRanges.negative), 0}, {(double) xRanges.positive, 0},
                        1, style.axesColor));
    interface.addPrimitive(GraphPrimitive::createArrow({0, (double) -yRanges.negative}, {0, (double) yRanges.positive},
                        1, style.axesColor));
}

void Graph::matchYRange() {
    updateRanges();
    double aspectRatio = (double)pixelsHeight / (double)pixelsWidth;
    double newYRange = xRng * aspectRatio;
    double diff = newYRange - yRng;
    yRanges.negative += (diff * ((double)yRanges.negative / yRng));
    yRanges.positive += (diff * ((double)yRanges.positive / yRng));
    yRng = yRanges.positive + yRanges.negative;
    interface.clear();
    initInterface();
    render();
}

void Graph::matchXRange() {
    updateRanges();
    double aspectRatio = (double)pixelsWidth / (double)pixelsHeight;
    double newXRange = yRng * aspectRatio;
    double diff = newXRange - xRng;
    xRanges.negative += (diff * ((double)xRanges.negative / xRng));
    xRanges.positive += (diff * ((double)xRanges.positive / xRng));
    xRng = xRanges.positive + xRanges.negative;

    interface.clear();
    initInterface();
    render();
}

void Graph::updateInterface() {
    updateRanges();
    interface.clear();
    initInterface();
}

void Graph::updateRanges() {
    const double eps = 1e-4;
    pixelsX0 = (int) ((float) (xRanges.negative) * (float) pixelsWidth /
                           (float) (xRanges.positive + xRanges.negative));
    pixelsY0 = (int) ((float) yRanges.positive * (float) pixelsHeight /
                           (float) (yRanges.positive + yRanges.negative));
    xRng = xRanges.positive + xRanges.negative;
    yRng = yRanges.positive + yRanges.negative;
    if (yRng < eps) {
        yRanges.positive = 0.1;
        yRanges.negative = 0.1;
        updateRanges();
    }

    if (xRng < eps) {
        xRanges.positive = 0.1;
        xRanges.negative = 0.1;
        updateRanges();
    }
}
