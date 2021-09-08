#include <SDL.h>
#include <SDL2_gfxPrimitives.h>
#include <SDL_render.h>
#include "Graph.h"
#include "GraphPrimitive.h"
#include "../sdl/GfxExtensions.h"

#define VECTOR_ARITHMACY

int GraphPrimitive::render(Graph *graph) {
    switch (type) {
        case GRAPH_CIRCLE:
            return shapeGraphical.circle.render(graph->getSurface(), graph->getRenderer());
        case GRAPH_LINE:
            return shapeGraphical.line.render(graph->getSurface(), graph->getRenderer());
        case GRAPH_POINT:
            return shapeGraphical.point.render(graph->getSurface(), graph->getRenderer());
        case GRAPH_ARROW:
            return shapeGraphical.arrow.render(graph->getSurface(), graph->getRenderer());
        case GRAPH_FUNC:
            return shapeGraphical.func.render(graph, graph->getSurface(), graph->getRenderer());
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

GraphPrimitive GraphPrimitive::createPoint(GraphVector pos, unsigned width, SDL_Color color) {
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
    return {point.getX() * (float) graph->getPixelsWidth() / (float) (graph->getXRng()) + (float) graph->getPixelsX0(),
            -point.getY() * (float) graph->getPixelsHeight() / (float) (graph->getYRng()) +
            (float) graph->getPixelsY0()};
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

GraphPrimitive
GraphPrimitive::createFunc(double(*func)(double), unsigned pointsPerTick, unsigned int width, SDL_Color color) {
    GraphPrimitive primitive = {};
    primitive.type = GRAPH_FUNC;
    primitive.trueGraphical = false;
    primitive.shape.func.func = func;
    primitive.shape.func.pointsPerTick = pointsPerTick;
    primitive.shape.func.width = width;
    primitive.shape.func.color = color;
    return primitive;
}

int GraphPrimitiveLine::render(SDL_Surface *surface, SDL_Renderer *renderer) const {
    return thickLineRGBA(renderer, (Sint16) this->start.getX(), (Sint16) this->start.getY(),
                         (Sint16) this->end.getX(), (Sint16) this->end.getY(),
                         width,
                         this->color.r, this->color.g, this->color.b, this->color.a);
}

void GraphPrimitiveLine::computeGraphical(Graph *pGraph, GraphPrimitiveLine line) {
    GraphPrimitiveLine transformed = line;
    transformed.start = GraphPrimitive::transformPoint(transformed.start, pGraph);
    transformed.end = GraphPrimitive::transformPoint(transformed.end, pGraph);
    *this = transformed;
}

int GraphPrimitivePoint::render(SDL_Surface *surface, SDL_Renderer *renderer) const {
    return filledCircleRGBA(renderer, (Sint16) this->pos.getX(), (Sint16) this->pos.getY(), (Sint16) this->width,
                            this->color.r, this->color.g, this->color.b, this->color.a);
}

void GraphPrimitivePoint::computeGraphical(Graph *pGraph, GraphPrimitivePoint point) {
    GraphPrimitivePoint transformed = point;
    transformed.pos = GraphPrimitive::transformPoint(transformed.pos, pGraph);
    *this = transformed;
}

int GraphPrimitiveCircle::render(SDL_Surface *surface, SDL_Renderer *renderer) const {
    return thickCircleRGBA(renderer, (Sint16) this->pos.getX(), (Sint16) this->pos.getY(), (Sint16) this->width,
                           this->color.r, this->color.g, this->color.b, this->color.a, this->width);
}

void GraphPrimitiveCircle::computeGraphical(Graph *pGraph, GraphPrimitiveCircle circle) {
    GraphPrimitiveCircle transformed = circle;
    transformed.pos = GraphPrimitive::transformPoint(transformed.pos, pGraph);
    *this = transformed;
}

int GraphPrimitiveArrow::render(SDL_Surface *surface, SDL_Renderer *renderer) const {
#ifdef VECTOR_ARITHMACY
    GraphVector difference = start - end;
    const double arrowScaling = log(1 + difference.len()) * 3;
    GraphVector arrowGuideVector = (difference).normalized() * arrowScaling;
    GraphVector perpendicular = arrowGuideVector.perpendicular().normalized() * arrowScaling / 3;

    GraphVector firstArrow = end + arrowGuideVector + perpendicular,
            secondArrow = end + arrowGuideVector - perpendicular;

    GraphVector endDecreased = end + arrowGuideVector;

    int res = thickLineRGBA(renderer, (Sint16)start.getX(), (Sint16)start.getY(),
                            (Sint16)(endDecreased.getX()), (Sint16)(endDecreased.getY()),
                            width, color.r, color.g, color.b, color.a);

    res |= filledTrigonRGBA(renderer, (Sint16)firstArrow.getX(), (Sint16)firstArrow.getY(),
                            (Sint16)secondArrow.getX(), (Sint16)secondArrow.getY(),(Sint16)end.getX(),
                            (Sint16)end.getY(), color.r, color.g, color.b, color.a);
    return res;
#else
    const double cos = 0.95, sin = 0.31224989; // specified by arrow angle
    const double l2 = 20;
    const double dx = (this->start.getX() - this->end.getX());
    const double dy = (this->start.getY() - this->end.getY());
    const double l1 = sqrt(dx * dx + dy * dy);

    const double decreaseFactor = (l2 * cos) / (l1);

    GraphVector end1 = {
            (this->end.getX() + (dx * cos + dy * sin) * l2 / l1),
            (this->end.getY() + (-dx * sin + dy * cos) * l2 / l1)};
    GraphVector end2 = {
            (this->end.getX() + (dx * cos - dy * sin) * l2 / l1),
            (this->end.getY() + (dx * sin + dy * cos) * l2 / l1)};

    int res = thickLineRGBA(renderer, (Sint16)this->start.getX(), (Sint16)this->start.getY(),
                      (Sint16)(this->end.getX() + dx * decreaseFactor), (Sint16)(this->end.getY() + dy * decreaseFactor),
                      width,
                      this->color.r, this->color.g, this->color.b, this->color.a);


    res |= filledTrigonRGBA(renderer,
                        (Sint16)end1.getX(), (Sint16)end1.getY(),
                        (Sint16)end2.getX(), (Sint16)end2.getY(),
                        (Sint16)end.getX(), (Sint16)end.getY(),
                            this->color.r, this->color.g, this->color.b, this->color.a);
    return res;
#endif
}

void GraphPrimitiveArrow::computeGraphical(Graph *pGraph, GraphPrimitiveArrow arrow) {
    GraphPrimitiveArrow transformed = arrow;
    transformed.start = GraphPrimitive::transformPoint(transformed.start, pGraph);
    transformed.end = GraphPrimitive::transformPoint(transformed.end, pGraph);
    *this = transformed;
}

int GraphPrimitiveFunction::render(Graph *graph, SDL_Surface *surface, SDL_Renderer *renderer) const {
    double start = -graph->getXRanges().negative;
    double increment = (double) graph->getStyle().xtick / (double) pointsPerTick;
    GraphContent content {};
    content.penUp();
    content.moveTo(start, func(start));
    content.penDown();
    for (unsigned i = 0; i < graph->getXRng() * pointsPerTick; i++) {
        start += increment;
        content.moveTo(start, func(start), width, color);
    }
    content.render(graph);
    return 1;
}
