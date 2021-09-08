#include "Vector.h"
#include <cmath>

GraphVector::GraphVector(double x, double y) : x(x), y(y) {}


GraphVector GraphVector::operator-(const GraphVector &other) const {
    GraphVector tmp(*this);
    tmp -= other;
    return tmp;
}

GraphVector GraphVector::operator+(const GraphVector &other) const {
    GraphVector tmp(*this);
    tmp += other;
    return tmp;
}

const GraphVector &GraphVector::operator-=(const GraphVector &other) {
    x -= other.x;
    y -= other.y;
    return *this;
}

const GraphVector &GraphVector::operator+=(const GraphVector &other) {
    x += other.x;
    y += other.y;
    return *this;
}

GraphVector GraphVector::operator-() const {
    return GraphVector(-x, -y);
}

GraphVector GraphVector::operator+() const {
    return *this;
}

GraphVector GraphVector::perpendicular() const {
    return GraphVector(y, -x);
}

double GraphVector::lenSquared() const {
    return x * x + y * y;
}

double GraphVector::len() const {
    return sqrt(x * x + y * y);
}

void GraphVector::normalize() {
    double lenCached = len();

    x /= lenCached;
    y /= lenCached;
}

GraphVector GraphVector::normalized() const {
    double lenCached = len();
    return GraphVector(x / lenCached, y / lenCached);
}

const GraphVector &GraphVector::operator/=(double scaling) {
    x /= scaling;
    y /= scaling;
    return *this;
}

const GraphVector &GraphVector::operator*=(double scaling) {
    x *= scaling;
    y *= scaling;
    return *this;
}

GraphVector GraphVector::operator/(double scaling) const {
    GraphVector tmp(*this);
    tmp /= scaling;
    return tmp;
}

GraphVector GraphVector::operator*(double scaling) const {
    GraphVector tmp(*this);
    tmp *= scaling;
    return tmp;
}

double GraphVector::scalar(const GraphVector &other) const {
    return x * other.x + y * other.y;
}

GraphVector::GraphVector(): x(0), y(0) {
}
