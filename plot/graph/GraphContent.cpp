#include "Graph.h"
#include "GraphContent.h"

GraphContent::GraphContent() :
        lastPenPosition({0, 0}), isPenDown(true), content() {
    content.init();
}

void GraphContent::dest() {
    content.dest();
}

void GraphContent::moveTo(GraphPosition pos, unsigned width, SDL_Color color) {
    if (isPenDown)
        content.pushBack(GraphPrimitive::createLine(lastPenPosition, pos, width, color));
    lastPenPosition = pos;
}

void GraphContent::moveTo(double x, double y, unsigned width, SDL_Color color) {
    moveTo({x, y}, width, color);
}

void GraphContent::penDown() {
    isPenDown = true;
}

void GraphContent::penUp() {
    isPenDown = false;
}

void GraphContent::addLine(GraphPrimitiveLine line) {
    GraphPrimitive primitive = {GraphPrimitive::GraphPrimitiveType::GRAPH_LINE};
    primitive.line = line;
    content.pushBack(primitive);
}

void GraphContent::addArrow(GraphPrimitiveArrow line) {
    GraphPrimitive primitive = {GraphPrimitive::GraphPrimitiveType::GRAPH_ARROW};
    primitive.arrow = line;
    content.pushBack(primitive);
}

void GraphContent::render(Graph* graph, bool calcGraphics) {
    for(size_t pos = content.begin(); pos != content.end(); content.nextIterator(&pos)) {
        GraphPrimitive elem = {};
        content.get(pos, &elem);
        if(calcGraphics)
            elem.computeGraphical(graph);
        elem.render(graph->surface, graph->renderer);
    }
}

void GraphContent::clear() {
    content.clear();
}
