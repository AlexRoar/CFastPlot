#pragma once
#include "GraphPrimitive.h"
#include "../list/FastList.h"

struct GraphContent {
    GraphPosition lastPenPosition;
    bool isPenDown;
    FastList<GraphPrimitive> content;

    GraphContent();

    void moveTo(double x, double y, unsigned width = 1, SDL_Color color = {0, 0, 255, 255});

    void moveTo(GraphPosition pos, unsigned width = 1, SDL_Color color = {0, 0, 255, 255});

    void addLine(GraphPrimitiveLine line);

    void addArrow(GraphPrimitiveArrow line);

    void penDown();

    void penUp();

    void dest();

    void render(Graph* graph, bool calcGraphics=true);

    void clear();
};
