#pragma once
#include "Vector.h"

class Matrix3x3 {
    explicit Matrix3x3(const double contentNew[3][3]);

    explicit Matrix3x3(const double contentNew[9]);

    const Matrix3x3 &operator=(double contentNew[3][3]);

    Matrix3x3 operator+(const Matrix3x3 &other) const;

    Matrix3x3 operator-(const Matrix3x3 &other) const;

    Matrix3x3 operator*(double scaling) const;

    GraphVector operator*(const GraphVector& other) const;

    Matrix3x3 operator*(const Matrix3x3& other) const;

    Matrix3x3 operator/(double scaling) const;

    Matrix3x3 operator-() const;

    Matrix3x3 operator+() const;

    const Matrix3x3 &operator+=(const Matrix3x3 &other);

    const Matrix3x3 &operator-=(const Matrix3x3 &other);

    const Matrix3x3 &operator*=(double scaling);

    const Matrix3x3 &operator*=(const Matrix3x3& other);

    const Matrix3x3 &operator/=(double scaling);

    inline static Matrix3x3 zero() {
        static const double content[3][3] = {};
        return Matrix3x3(content);
    }

    inline static Matrix3x3 eye() {
        static const double content[3][3] = {
                {1.0, 0.0, 0.0},
                {0.0, 1.0, 0.0},
                {0.0, 0.0, 1.0},
        };
        return Matrix3x3(content);
    }

    inline static Matrix3x3 ones() {
        static const double content[3][3] = {
                {1.0, 1.0, 1.0},
                {1.0, 1.0, 1.0},
                {1.0, 1.0, 1.0},
        };
        return Matrix3x3(content);
    }

private:
    union {
        double content[9];
        double contentGrid[3][3];
    };
};