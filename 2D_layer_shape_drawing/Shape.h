#pragma once

#include <vector>
#include <GL/glut.h>

#include "RGB.h"

using namespace std;

class Point {
public:
    float x;
    float y;

    Point(): x(0), y(0) {}
    Point(float x, float y) : x(x), y(y) {}
};

class Shape {
protected:
    Point startPoint;
    Point endPoint;
    int layer;
    RGBColor fillColor;
public:
    Shape() {}

    Shape(const Point& startPoint, const Point& endPoint, int layer)
        : startPoint(startPoint), endPoint(endPoint), layer(layer) {}

    void setFillColor(RGBColor color) {
        fillColor = color;
    }

    void setStartPoint(Point point) {
        startPoint = point;
    }

    void setEndPoint(Point point) {
        endPoint = point;
    }

    void setLayer(int l) {
        layer = l;
    }

    virtual void draw() = 0;
    virtual void identifyVertices() = 0;
};

class ShapeDrawer {
public:
    static void drawShape(const vector<Point>& vertices) {
        glBegin(GL_LINE_LOOP);
        for (const auto& vertex : vertices) {
            glVertex2f(vertex.x, vertex.y);
        }
        glEnd();
    }
};

class Rectangle : public Shape {
protected:
    vector<Point> vertices;

public:
    Rectangle() {}

    Rectangle(const Point& startPoint, const Point& endPoint, int layer)
        : Shape(startPoint, endPoint, layer) {}

    void identifyVertices() override {
        // Clear the existing vertices
        vertices.clear();

        // Identify the vertices of the rectangle
        Point topLeft(startPoint.x, startPoint.y);
        Point topRight(endPoint.x, startPoint.y);
        Point bottomRight(endPoint.x, endPoint.y);
        Point bottomLeft(startPoint.x, endPoint.y);

        // Add the vertices to the vector
        vertices.push_back(topLeft);
        vertices.push_back(topRight);
        vertices.push_back(bottomRight);
        vertices.push_back(bottomLeft);
    }

    void draw() override {
        ShapeDrawer::drawShape(vertices);
    }
};

class Circle : public Shape {
protected:
    float radius;

public:
    Circle(): radius(0) {}

    Circle(const Point& startPoint, float radius, int layer)
        : Shape(startPoint, startPoint, layer), radius(radius) {}

    void identifyVertices() override {
        // Circles do not have distinct vertices, so this method may not be relevant for circles
    }

    void draw() override {
        const int numSegments = 100;
        const float angleIncrement = 2.0 * 3.14159265358979323846 / numSegments;

        vector<Point> vertices;
        for (int i = 0; i < numSegments; ++i) {
            float angle = i * angleIncrement;
            float x = startPoint.x + cos(angle) * radius;
            float y = startPoint.y + sin(angle) * radius;
            vertices.emplace_back(x, y);
        }

        ShapeDrawer::drawShape(vertices);
    }
};

class EquilateralTriangle : public Shape {
protected:
    vector<Point> vertices;

public:
    EquilateralTriangle() {}

    EquilateralTriangle(const Point& startPoint, const Point& endPoint, int layer)
        : Shape(startPoint, endPoint, layer) {}

    void identifyVertices() override {
        // Clear the existing vertices
        vertices.clear();

        // Calculate the height of the equilateral triangle
        float height = (endPoint.y - startPoint.y);

        // Calculate the length of each side of the equilateral triangle
        float sideLength = height / sqrt(3);

        // Identify the vertices of the equilateral triangle
        Point top((startPoint.x + endPoint.x) / 2, startPoint.y);
        Point left(startPoint.x, endPoint.y);
        Point right(endPoint.x, endPoint.y);

        // Add the vertices to the vector
        vertices.push_back(top);
        vertices.push_back(left);
        vertices.push_back(right);
    }

    void draw() override {
        ShapeDrawer::drawShape(vertices);
    }
};

#include "Define.h"

class FactoryShape {
public:
    static Shape* getShape(int shapeType) {
        switch (shapeType)
        {
        case CIRCLE_CODE:
            return new Circle();
        case RECTANGLE_CODE:
            return new Rectangle();
        case TRIANGLE_CODE:
            return new EquilateralTriangle();
        default:
            return new Rectangle();
        }
    }
};