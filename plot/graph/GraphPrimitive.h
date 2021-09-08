#pragma once
#include <SDL.h>
#include <SDL2_gfxPrimitives.h>
#include <SDL_render.h>
#include "../vector/Vector.h"
struct Graph;

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

struct GraphPrimitiveFunction {
    double(*func)(double) = nullptr;
    unsigned width;
    unsigned pointsPerTick;
    SDL_Color color;

    int render(Graph* graph, SDL_Surface* surface, SDL_Renderer* renderer) const;
};

struct GraphPrimitive {
    typedef union {
        GraphPrimitiveLine line;
        GraphPrimitiveCircle circle;
        GraphPrimitivePoint point;
        GraphPrimitiveArrow arrow;
        GraphPrimitiveFunction func;
    } GraphPrimitives;

    enum GraphPrimitiveType {
        GRAPH_LINE,
        GRAPH_POINT,
        GRAPH_CIRCLE,
        GRAPH_ARROW,
        GRAPH_FUNC
    };
    GraphPrimitiveType type;
    bool trueGraphical;

    GraphPrimitives shape;
    GraphPrimitives shapeGraphical;

    static GraphPrimitive createCircle(GraphVector pos, unsigned radius, unsigned width, SDL_Color color);

    static GraphPrimitive createLine(GraphVector start, GraphVector end, unsigned width, SDL_Color color);

    static GraphPrimitive createArrow(GraphVector start, GraphVector end, unsigned width, SDL_Color color);

    static GraphPrimitive createPoint(GraphVector pos, unsigned width, SDL_Color color);

    static GraphPrimitive createFunc(double(*func)(double), unsigned pointsPerTick, unsigned width, SDL_Color color);

    static GraphVector transformPoint(GraphVector point, Graph* graph);

    void computeGraphical(Graph* graph);

    [[nodiscard]] bool isTrueGraphical() const;

    void setTrueGraphical(bool trueGraphical);

    int render(Graph* graph);
};