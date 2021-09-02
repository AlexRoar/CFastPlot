#pragma once
#include <SDL.h>
#include <SDL2_gfxPrimitives.h>
#include <SDL_render.h>
struct Graph;

struct GraphPosition {
    double x, y;
};

struct GraphPrimitiveLine {
    GraphPosition start;
    GraphPosition end;
    unsigned width;
    SDL_Color color;

    int render(SDL_Surface* surface, SDL_Renderer* renderer) const;

    void computeGraphical(Graph *pGraph, GraphPrimitiveLine point);
};

struct GraphPrimitiveCircle {
    GraphPosition pos;
    unsigned radius;
    unsigned width;
    SDL_Color color;

    int render(SDL_Surface* surface, SDL_Renderer* renderer) const;

    void computeGraphical(Graph *pGraph, GraphPrimitiveCircle point);
};

struct GraphPrimitivePoint {
    GraphPosition pos;
    unsigned width;
    SDL_Color color;

    int render(SDL_Surface* surface, SDL_Renderer* renderer) const;

    void computeGraphical(Graph *pGraph, GraphPrimitivePoint point);
};

struct GraphPrimitiveArrow {
    GraphPosition start;
    GraphPosition end;
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

    static GraphPrimitive createCircle(GraphPosition pos, unsigned radius, unsigned width, SDL_Color color);

    static GraphPrimitive createLine(GraphPosition start, GraphPosition end, unsigned width, SDL_Color color);

    static GraphPrimitive createArrow(GraphPosition start, GraphPosition end, unsigned width, SDL_Color color);

    static GraphPrimitive createPoint(GraphPosition pos, unsigned width, SDL_Color color);

    static GraphPosition transformPoint(GraphPosition point, Graph* graph);

    void computeGraphical(Graph* graph);

    int render(SDL_Surface* surface, SDL_Renderer* renderer);
};