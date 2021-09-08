#pragma once

class GraphVector {
public:
    GraphVector();

    GraphVector(double x, double y);

    inline static GraphVector zero() {
        return GraphVector(0, 0);
    }

    GraphVector operator+(const GraphVector &other) const;

    GraphVector operator-(const GraphVector &other) const;

    GraphVector operator*(double scaling) const;

    GraphVector operator/(double scaling) const;

    GraphVector operator-() const;

    GraphVector operator+() const;

    [[nodiscard]] GraphVector perpendicular() const;

    [[nodiscard]] GraphVector normalized() const;

    [[nodiscard]] double len() const;

    [[nodiscard]] double lenSquared() const;

    [[nodiscard]] double scalar(const GraphVector& other) const;

    void normalize();

    const GraphVector &operator+=(const GraphVector &other);

    const GraphVector &operator-=(const GraphVector &other);

    const GraphVector &operator*=(double scaling);

    const GraphVector &operator/=(double scaling);

    [[nodiscard]] inline double getX() const {
        return x;
    }

    inline void setX(double xnew) {
        x = xnew;
    }

    [[nodiscard]] inline double getY() const {
        return y;
    }

    inline void setY(double ynew) {
        y = ynew;
    }
private:
    double x, y;
};
