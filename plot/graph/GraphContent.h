#pragma once
#include "GraphPrimitive.h"
#include "../list/FastList.h"

struct GraphContent {
    GraphContent();

    size_t moveTo(double x, double y, unsigned width = 1, SDL_Color color = {0, 0, 255, 255});

    size_t moveTo(GraphVector pos, unsigned width = 1, SDL_Color color = {237, 85, 59, 255});

    size_t addPrimitive(GraphPrimitive primitive);

    size_t addVector(GraphVector vector, GraphVector position, unsigned width= 3, SDL_Color color= {60, 174, 163, 255});

    void penDown();

    void penUp();

    ~GraphContent();

    void render(Graph* graph, bool calcGraphics=true);

    void clear();

    GraphContent& operator=(const GraphContent& other);

    GraphPrimitive getById(size_t id);

    void setById(size_t id, GraphPrimitive primitive);

private:
    GraphVector lastPenPosition;
    bool isPenDown;
    FastList<GraphPrimitive> content;
};
