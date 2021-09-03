#pragma once
#include "GraphPrimitive.h"
#include "../list/FastList.h"

struct GraphContent {
    GraphVector lastPenPosition;
    bool isPenDown;
    FastList<GraphPrimitive> content;

    GraphContent();

    void moveTo(double x, double y, unsigned width = 1, SDL_Color color = {0, 0, 255, 255});

    void moveTo(GraphVector pos, unsigned width = 1, SDL_Color color = {237, 85, 59, 255});

    void addPrimitive(GraphPrimitive primitive);

    void addVector(GraphVector vector, GraphVector position, unsigned width=3, SDL_Color color={60, 174, 163, 255});

    void penDown();

    void penUp();

    void dest();

    void render(Graph* graph, bool calcGraphics=true);

    void clear();
};
