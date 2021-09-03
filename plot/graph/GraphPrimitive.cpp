#include <SDL.h>
#include <SDL2_gfxPrimitives.h>
#include <SDL_render.h>
#include "Graph.h"
#include "GraphPrimitive.h"
#include "../sdl/GfxExtensions.h"

int GraphPrimitive::render(Graph* graph, SDL_Surface *surface, SDL_Renderer* renderer) {
    switch (type) {
        case GRAPH_CIRCLE:
            return shapeGraphical.circle.render(surface, renderer);
        case GRAPH_LINE:
            return shapeGraphical.line.render(surface, renderer);
        case GRAPH_POINT:
            return shapeGraphical.point.render(surface, renderer);
        case GRAPH_ARROW:
            return shapeGraphical.arrow.render(surface, renderer);
        case GRAPH_FUNC:
            return shapeGraphical.func.render(graph, surface, renderer);
        default: {
            printf("Unreachable switch branch");
            exit(EXIT_FAILURE);
        }
    }
}

void GraphPrimitive::computeGraphical(Graph *graph) {
    if (trueGraphical) {
        shapeGraphical = shape;
        return;
    }
    switch (type) {
        case GRAPH_CIRCLE:
            return shapeGraphical.circle.computeGraphical(graph, shape.circle);
        case GRAPH_LINE:
            return shapeGraphical.line.computeGraphical(graph, shape.line);
        case GRAPH_POINT:
            return shapeGraphical.point.computeGraphical(graph, shape.point);
        case GRAPH_ARROW:
            return shapeGraphical.arrow.computeGraphical(graph, shape.arrow);
        case GRAPH_FUNC:
            shapeGraphical = shape;
            return;
        default: {
            printf("Unreachable switch branch");
            exit(EXIT_FAILURE);
        }
    }
}

GraphPrimitive GraphPrimitive::createPoint(GraphVector pos, unsigned width, SDL_Color color)  {
    GraphPrimitive primitive = {};
    primitive.type = GRAPH_POINT;
    primitive.trueGraphical = false;
    primitive.shape.point.pos = pos;
    primitive.shape.point.width = width;
    primitive.shape.point.color = color;
    return primitive;
}

GraphPrimitive GraphPrimitive::createLine(GraphVector start, GraphVector end, unsigned width, SDL_Color color) {
    GraphPrimitive primitive = {};
    primitive.type = GRAPH_LINE;
    primitive.trueGraphical = false;
    primitive.shape.line.start = start;
    primitive.shape.line.end = end;
    primitive.shape.line.width = width;
    primitive.shape.line.color = color;
    return primitive;
}

GraphPrimitive
GraphPrimitive::createCircle(GraphVector pos, unsigned radius, unsigned width, SDL_Color color) {
    GraphPrimitive primitive = {};
    primitive.type = GRAPH_CIRCLE;
    primitive.trueGraphical = false;
    primitive.shape.circle.pos = pos;
    primitive.shape.circle.radius = radius;
    primitive.shape.circle.width = width;
    primitive.shape.circle.color = color;
    return primitive;
}

GraphVector GraphPrimitive::transformPoint(GraphVector point, Graph *graph) {
    return {point.x * (float)graph->pixelsWidth / (float)(graph->xRng) + (float)graph->pixelsX0,
            -point.y * (float)graph->pixelsHeight  / (float)(graph->yRng) + (float)graph->pixelsY0};
}

GraphPrimitive
GraphPrimitive::createArrow(GraphVector start, GraphVector end, unsigned width, SDL_Color color) {
    GraphPrimitive primitive = {};
    primitive.type = GRAPH_ARROW;
    primitive.trueGraphical = false;
    primitive.shape.arrow.start = start;
    primitive.shape.arrow.end = end;
    primitive.shape.arrow.width = width;
    primitive.shape.arrow.color = color;
    return primitive;
}

bool GraphPrimitive::isTrueGraphical() const {
    return trueGraphical;
}

void GraphPrimitive::setTrueGraphical(bool trueGraph) {
    GraphPrimitive::trueGraphical = trueGraph;
}

GraphPrimitive GraphPrimitive::createFunc(double(*func)(double), unsigned pointsPerTick, unsigned int width, SDL_Color color) {
    GraphPrimitive primitive = {};
    primitive.type = GRAPH_FUNC;
    primitive.trueGraphical = false;
    primitive.shape.func.func = func;
    primitive.shape.func.pointsPerTick = pointsPerTick;
    primitive.shape.func.width = width;
    primitive.shape.func.color = color;
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
    const double cos = 0.95, sin = 0.31224989; // specified by arrow angle
    const double l2 = 20;
    const double dx = (this->start.x - this->end.x);
    const double dy = (this->start.y - this->end.y);
    const double l1 = sqrt(dx * dx + dy * dy);

    const double decreaseFactor = (l2 * cos) / (l1);

    GraphVector end1 = {
            (this->end.x + (dx * cos + dy * sin) * l2 / l1),
            (this->end.y + (-dx * sin + dy * cos) * l2 / l1)};
    GraphVector end2 = {
            (this->end.x + (dx * cos - dy * sin) * l2 / l1),
            (this->end.y + (dx * sin + dy * cos) * l2 / l1)};

    int res = thickLineRGBA(renderer, (Sint16)this->start.x, (Sint16)this->start.y,
                      (Sint16)(this->end.x + dx * decreaseFactor), (Sint16)(this->end.y + dy * decreaseFactor),
                      width,
                      this->color.r, this->color.g, this->color.b, this->color.a);


    res |= filledTrigonRGBA(renderer,
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

int GraphPrimitiveFunction::render(Graph* graph, SDL_Surface *surface, SDL_Renderer *renderer) const {
    double start = -graph->xRanges.negative;
    double increment = (double)graph->style.xtick / (double)pointsPerTick;
    GraphContent content;
    content.penUp();
    content.moveTo(start, func(start));
    content.penDown();
    for (unsigned i = 0; i < graph->xRng * pointsPerTick; i++) {
        start += increment;
        content.moveTo(start, func(start), width, color);
    }
    content.render(graph);
    content.dest();
    return 1;
}
