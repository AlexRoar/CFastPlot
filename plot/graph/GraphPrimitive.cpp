#include <SDL.h>
#include <SDL2_gfxPrimitives.h>
#include <SDL_render.h>
#include "Graph.h"
#include "GraphPrimitive.h"
#include "../sdl/GfxExtensions.h"

int GraphPrimitive::render(SDL_Surface *surface, SDL_Renderer* renderer) {
    switch (type) {
        case GRAPH_CIRCLE:
            return circleGraphical.render(surface, renderer);
        case GRAPH_LINE:
            return lineGraphical.render(surface, renderer);
        case GRAPH_POINT:
            return pointGraphical.render(surface, renderer);
        case GRAPH_ARROW:
            return arrowGraphical.render(surface, renderer);
        default: {
            printf("Unreachable switch branch");
            exit(EXIT_FAILURE);
        }
    }
}

void GraphPrimitive::computeGraphical(Graph *graph) {
    switch (type) {
        case GRAPH_CIRCLE:
            return circleGraphical.computeGraphical(graph, circle);
        case GRAPH_LINE:
            return lineGraphical.computeGraphical(graph, line);
        case GRAPH_POINT:
            return pointGraphical.computeGraphical(graph, point);
        case GRAPH_ARROW:
            return arrowGraphical.computeGraphical(graph, arrow);
        default: {
            printf("Unreachable switch branch");
            exit(EXIT_FAILURE);
        }
    }
}

GraphPrimitive GraphPrimitive::createPoint(GraphPosition pos, unsigned width, SDL_Color color)  {
    GraphPrimitive primitive = {};
    primitive.type = GRAPH_POINT;
    primitive.point.pos = pos;
    primitive.point.width = width;
    primitive.point.color = color;
    return primitive;
}

GraphPrimitive GraphPrimitive::createLine(GraphPosition start, GraphPosition end, unsigned width, SDL_Color color) {
    GraphPrimitive primitive = {};
    primitive.type = GRAPH_LINE;
    primitive.line.start = start;
    primitive.line.end = end;
    primitive.line.width = width;
    primitive.line.color = color;
    return primitive;
}

GraphPrimitive
GraphPrimitive::createCircle(GraphPosition pos, unsigned radius, unsigned width, SDL_Color color) {
    GraphPrimitive primitive = {};
    primitive.type = GRAPH_CIRCLE;
    primitive.circle.pos = pos;
    primitive.circle.radius = radius;
    primitive.circle.width = width;
    primitive.circle.color = color;
    return primitive;
}

GraphPosition GraphPrimitive::transformPoint(GraphPosition point, Graph *graph) {
    return {point.x * (float)graph->pixelsWidth / (float)(graph->xRng) + (float)graph->pixelsX0,
            -point.y * (float)graph->pixelsHeight  / (float)(graph->yRng) + (float)graph->pixelsY0};
}

GraphPrimitive
GraphPrimitive::createArrow(GraphPosition start, GraphPosition end, unsigned width, SDL_Color color) {
    GraphPrimitive primitive = {};
    primitive.type = GRAPH_ARROW;
    primitive.arrow.start = start;
    primitive.arrow.end = end;
    primitive.arrow.width = width;
    primitive.arrow.color = color;
    return primitive;
}

int GraphPrimitiveLine::render(SDL_Surface *surface, SDL_Renderer* renderer) const {
    return thickLineRGBA(renderer, (Sint16)this->start.x, (Sint16)this->start.y,
                         (Sint16)this->end.x, (Sint16)this->end.y,
                         width,
                         this->color.r, this->color.g, this->color.b, this->color.a);
}

void GraphPrimitiveLine::computeGraphical(Graph *pGraph, GraphPrimitiveLine line) {
    GraphPrimitiveLine transformed = line;
    transformed.start = GraphPrimitive::transformPoint(transformed.start, pGraph);
    transformed.end = GraphPrimitive::transformPoint(transformed.end, pGraph);
    *this = transformed;
}

int GraphPrimitivePoint::render(SDL_Surface *surface, SDL_Renderer* renderer) const {
    return filledCircleRGBA(renderer, (Sint16)this->pos.x, (Sint16)this->pos.y, (Sint16)this->width,
                            this->color.r, this->color.g, this->color.b, this->color.a);
}

void GraphPrimitivePoint::computeGraphical(Graph *pGraph, GraphPrimitivePoint point) {
    GraphPrimitivePoint transformed = point;
    transformed.pos = GraphPrimitive::transformPoint(transformed.pos, pGraph);
    *this = transformed;
}

int GraphPrimitiveCircle::render(SDL_Surface *surface, SDL_Renderer* renderer) const {
    return thickCircleRGBA(renderer, (Sint16)this->pos.x, (Sint16)this->pos.y, (Sint16)this->width,
                           this->color.r, this->color.g, this->color.b, this->color.a, this->width);
}

void GraphPrimitiveCircle::computeGraphical(Graph *pGraph, GraphPrimitiveCircle circle) {
    GraphPrimitiveCircle transformed = circle;
    transformed.pos = GraphPrimitive::transformPoint(transformed.pos, pGraph);
    *this = transformed;
}

int GraphPrimitiveArrow::render(SDL_Surface *surface, SDL_Renderer *renderer) const {
    auto res = aalineRGBA(renderer, (Sint16)this->start.x, (Sint16)this->start.y,
                      (Sint16)this->end.x, (Sint16)this->end.y,
                      this->color.r, this->color.g, this->color.b, this->color.a);
    const double cos = 0.95, sin = 0.31224989; // specified by arrow angle
    const double dx = (this->start.x - this->end.x);
    const double dy = (this->start.y - this->end.y);
    const double l1 = sqrt(dx * dx + dy * dy);
    const double l2 = 20;
    GraphPosition end1 = {
            (this->end.x + (dx * cos + dy * sin) * l2 / l1),
            (this->end.y + (-dx * sin + dy * cos) * l2 / l1)};
    GraphPosition end2 = {
            (this->end.x + (dx * cos - dy * sin) * l2 / l1),
            (this->end.y + (dx * sin + dy * cos) * l2 / l1)};

    res |= aatrigonRGBA(renderer,
                        (Sint16)end1.x, (Sint16)end1.y,
                        (Sint16)end2.x, (Sint16)end2.y,
                        (Sint16)end.x, (Sint16)end.y,
                        this->color.r, this->color.g, this->color.b, this->color.a);
    return res;
}

void GraphPrimitiveArrow::computeGraphical(Graph *pGraph, GraphPrimitiveArrow arrow) {
    GraphPrimitiveArrow transformed = arrow;
    transformed.start = GraphPrimitive::transformPoint(transformed.start, pGraph);
    transformed.end = GraphPrimitive::transformPoint(transformed.end, pGraph);
    *this = transformed;
}
