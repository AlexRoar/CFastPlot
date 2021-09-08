#include "Graph.h"
#include "GraphContent.h"

GraphContent::GraphContent() :
        lastPenPosition({0, 0}), isPenDown(true), content() {
    content.init();
}

GraphContent::~GraphContent() {
    content.dest();
}

size_t GraphContent::moveTo(GraphVector pos, unsigned width, SDL_Color color) {
    size_t res = 0;
    if (isPenDown)
        res = content.pushBack(GraphPrimitive::createLine(lastPenPosition, pos, width, color));
    lastPenPosition = pos;
    return res;
}

size_t GraphContent::moveTo(double x, double y, unsigned width, SDL_Color color) {
    return moveTo({x, y}, width, color);
}

void GraphContent::penDown() {
    isPenDown = true;
}

void GraphContent::penUp() {
    isPenDown = false;
}


void GraphContent::render(Graph *graph, bool calcGraphics) {
    for (size_t pos = content.begin(); pos != content.end(); content.nextIterator(&pos)) {
        GraphPrimitive elem = {};
        content.get(pos, &elem);
        if (calcGraphics)
            elem.computeGraphical(graph);
        elem.render(graph);
    }
}

void GraphContent::clear() {
    content.clear();
}

size_t GraphContent::addVector(GraphVector vector, GraphVector position, unsigned width, SDL_Color color) {
    size_t result = 0;
    content.pushBack(GraphPrimitive::createArrow(position,
                                                 {vector.getX() + position.getX(), vector.getY() + position.getY()},
                                                 width, color), &result);
    return result;
}

size_t GraphContent::addPrimitive(GraphPrimitive primitive) {
    return content.pushBack(primitive);
}

GraphContent &GraphContent::operator=(const GraphContent &other) {
    if (this != &other){
        this->content.dest();
        this->content = other.content.copy();

        this->isPenDown = other.isPenDown;
        this->lastPenPosition = other.lastPenPosition;
    }
    return *this;
}

GraphPrimitive GraphContent::getById(size_t id) {
    GraphPrimitive* element = nullptr;
    content.get(id, &element);
    return *element;
}

void GraphContent::setById(size_t id, GraphPrimitive primitive) {
    content.set(id, primitive);
}