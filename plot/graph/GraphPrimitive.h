#pragma once
#include <SDL.h>
#include <SDL2_gfxPrimitives.h>
#include <SDL_render.h>
struct Graph;

struct GraphVector {
    double x, y;
};

struct GraphPrimitiveLine {
    GraphVector start;
    GraphVector end;
    unsigned width;
    SDL_Color color;

    int render(SDL_Surface* surface, SDL_Renderer* renderer) const;

    void computeGraphical(Graph *pGraph, GraphPrimitiveLine point);
};

struct GraphPrimitiveCircle {
    GraphVector pos;
    unsigned radius;
    unsigned width;
    SDL_Color color;

    int render(SDL_Surface* surface, SDL_Renderer* renderer) const;

    void computeGraphical(Graph *pGraph, GraphPrimitiveCircle point);
};

struct GraphPrimitivePoint {
    GraphVector pos;
    unsigned width;
    SDL_Color color;

    int render(SDL_Surface* surface, SDL_Renderer* renderer) const;

    void computeGraphical(Graph *pGraph, GraphPrimitivePoint point);
};

struct GraphPrimitiveArrow {
    GraphVector start;
    GraphVector end;
    unsigned width;
    SDL_Color color;

    int render(SDL_Surface* surface, SDL_Renderer* renderer) const;

    void computeGraphical(Graph *pGraph, GraphPrimitiveArrow point);
};

struct GraphPrimitive {
    enum GraphPrimitiveType {
        GRAPH_LINE,
        GRAPH_POINT,
        GRAPH_CIRCLE,
        GRAPH_ARROW
    };
    GraphPrimitiveType type;

    union {
        GraphPrimitiveLine line;
        GraphPrimitiveCircle circle;
        GraphPrimitivePoint point;
        GraphPrimitiveArrow arrow;
    };

    union {
        GraphPrimitiveLine lineGraphical;
        GraphPrimitiveCircle circleGraphical;
        GraphPrimitivePoint pointGraphical;
        GraphPrimitiveArrow arrowGraphical;
    };

    static GraphPrimitive createCircle(GraphVector pos, unsigned radius, unsigned width, SDL_Color color);

    static GraphPrimitive createLine(GraphVector start, GraphVector end, unsigned width, SDL_Color color);

    static GraphPrimitive createArrow(GraphVector start, GraphVector end, unsigned width, SDL_Color color);

    static GraphPrimitive createPoint(GraphVector pos, unsigned width, SDL_Color color);

    static GraphVector transformPoint(GraphVector point, Graph* graph);

    void computeGraphical(Graph* graph);

    int render(SDL_Surface* surface, SDL_Renderer* renderer);
};