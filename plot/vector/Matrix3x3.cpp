
#include "Matrix3x3.h"

Matrix3x3::Matrix3x3(const double (*contentNew)[3]) : content() {
    for (int i = 0; i < 3 * 3; i++) {
        content[i] = *((double *) contentNew + i);
    }
}

Matrix3x3::Matrix3x3(const double *contentNew) : content() {
    for (int i = 0; i < 3 * 3; i++) {
        content[i] = contentNew[i];
    }
}

const Matrix3x3 &Matrix3x3::operator=(double (*contentNew)[3]) {
    return *this = Matrix3x3(contentNew);
}

const Matrix3x3 &Matrix3x3::operator+=(const Matrix3x3 &other) {
    for (int i = 0; i < 3 * 3; i++)
        content[i] += other.content[i];
    return *this;
}

const Matrix3x3 &Matrix3x3::operator*=(const Matrix3x3 &other) {
    double results[3][3] = {{0, 0, 0},
                            {0, 0, 0},
                            {0, 0, 0}};

    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            for (int u = 0; u < 3; u++)
                results[i][j] += contentGrid[i][u] * other.contentGrid[u][j];
        }
    }
    *this = Matrix3x3(results);
    return *this;
}

const Matrix3x3 &Matrix3x3::operator-=(const Matrix3x3 &other) {
    for (int i = 0; i < 3 * 3; i++)
        content[i] -= other.content[i];
    return *this;
}

const Matrix3x3 &Matrix3x3::operator*=(double scaling) {
    for (int i = 0; i < 3 * 3; i++)
        content[i] *= scaling;
    return *this;
}

const Matrix3x3 &Matrix3x3::operator/=(double scaling) {
    for (int i = 0; i < 3 * 3; i++)
        content[i] /= scaling;
    return *this;
}

Matrix3x3 Matrix3x3::operator-(const Matrix3x3 &other) const {
    Matrix3x3 newMatrix(*this);
    newMatrix -= other;
    return newMatrix;
}

Matrix3x3 Matrix3x3::operator*(const Matrix3x3 &other) const {
    Matrix3x3 newMatrix(*this);
    newMatrix *= other;
    return newMatrix;
}

Matrix3x3 Matrix3x3::operator*(double scaling) const {
    Matrix3x3 newMatrix(*this);
    newMatrix *= scaling;
    return newMatrix;
}

Matrix3x3 Matrix3x3::operator/(double scaling) const {
    Matrix3x3 newMatrix(*this);
    newMatrix /= scaling;
    return newMatrix;
}

Matrix3x3 Matrix3x3::operator+() const {
    return Matrix3x3(*this);
}

Matrix3x3 Matrix3x3::operator-() const {
    Matrix3x3 newMatrix(*this);
    newMatrix *= -1.0;
    return newMatrix;
}

Matrix3x3 Matrix3x3::operator+(const Matrix3x3 &other) const {
    Matrix3x3 newMatrix(*this);
    newMatrix += other;
    return newMatrix;
}

GraphVector Matrix3x3::operator*(const GraphVector &other) const {
    return GraphVector(
            other.getX() * contentGrid[0][0] +
            other.getY() * contentGrid[0][1] +
            contentGrid[0][2],
            other.getX() * contentGrid[1][0] +
            other.getY() * contentGrid[1][1] +
            contentGrid[1][2]);
}
