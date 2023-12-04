#pragma once

#include <vector>
#include <GL/glut.h>

#include "RGB.h"
#include <stack>

using namespace std;

class Point {
public:
    int x;
    int y;

    Point(): x(0), y(0) {}
    Point(int x, int y) : x(x), y(y) {}
};

class ShapeDrawer {
public:
    static void setPixel(int x, int y) {
        glBegin(GL_POINTS);
        glVertex2i(x, y);
        glEnd();
    }

    static void drawShape(const vector<Point>& vertices) {
        // Draw bounder
        int len = vertices.size();
        for (int i = 0; i < len; i++) {
            Point v1 = vertices[i];
            Point v2 = vertices[(i + 1) % len];

            ShapeDrawer::drawLine(v1, v2);
        }
    }

    static void drawCircle(int radius, Point center, RGBColor color = RGBColor::WHITE) {
        glColor3ub(color.r(), color.g(), color.b());

        if (radius <= 0) return;

        int xT = center.x;
        int yT = center.y;

        ShapeDrawer::setPixel(0 + xT, radius + yT);
        ShapeDrawer::setPixel(0 + xT, -radius + yT);
        ShapeDrawer::setPixel(radius + xT, 0 + yT);
        ShapeDrawer::setPixel(-radius + xT, 0 + yT);

        int x = 0;
        int y = radius;
        int p = (int)5 / 4 - radius;

        while (x < y) {
            int xM = x + 1;
            int yM = y - (int)1 / 2;

            if (p < 0) {
                p = p + 2 * x + 3;
            }
            else {
                p = p + 2 * x - 2 * y + 5;
                y--;
            }

            x++;
            ShapeDrawer::setPixel(x + xT, y + yT);	ShapeDrawer::setPixel(-x + xT, y + yT);
            ShapeDrawer::setPixel(x + xT, -y + yT);	ShapeDrawer::setPixel(-x + xT, -y + yT);
            ShapeDrawer::setPixel(y + xT, x + yT);	ShapeDrawer::setPixel(-y + xT, x + yT);
            ShapeDrawer::setPixel(y + xT, -x + yT);	ShapeDrawer::setPixel(-y + xT, -x + yT);
        }
    }

    static void drawLine(Point start, Point end, RGBColor color = RGBColor::WHITE) {
        glColor3ub(color.r(), color.g(), color.b());

        int x1 = start.x;
        int y1 = start.y;

        int x2 = end.x;
        int y2 = end.y;

        if (abs(x2 - x1) > abs(y2 - y1)) {
            if (x1 > x2) {
                swap(x1, x2);
                swap(y1, y2);
            }

            int dx = x2 - x1;
            int dy = y2 - y1;

            int sign = (y2 - y1 > 0) ? 1 : -1;
            int p = 2 * dy - sign * dx;
            int x = x1, y = y1;
            while (x <= x2) {
                ShapeDrawer::setPixel(x, y);
                if (p >= 0) {
                    p = p + 2 * dy - (sign > 0) * sign * 2 * dx;
                    y = y + (sign > 0) * sign;
                }
                else {
                    p = p + 2 * dy - (sign < 0) * sign * 2 * dx;
                    y = y + (sign < 0) * sign;
                }
                x = x + 1;
            }
        }
        else {
            if (y1 > y2) {
                swap(x1, x2);
                swap(y1, y2);
            }

            int dx = x2 - x1;
            int dy = y2 - y1;

            int sign = (x2 - x1 > 0) ? 1 : -1;
            int p = -2 * dx + sign * dy;
            int x = x1, y = y1;
            while (y <= y2) {
                ShapeDrawer::setPixel(x, y);
                if (p >= 0) {
                    p = p - 2 * dx + (sign < 0) * sign * 2 * dy;
                    x = x + (sign < 0) * sign;
                }
                else {
                    p = p - 2 * dx + (sign > 0) * sign * 2 * dy;;
                    x = x + (sign > 0) * sign;
                }
                y = y + 1;
            }
        }
    }

    static void fillColor(Point startFillPoint, RGBColor fillColor) {
        glColor3ub(fillColor.r(), fillColor.g(), fillColor.b());

        // Non-recursive implementation
        stack<pair<int, int>> s;

        int x = startFillPoint.x;
        int y = startFillPoint.y;

        s.push(make_pair(x, y));

        while (!s.empty()) {
            pair<int, int> p = s.top();
            s.pop();

            x = p.first;
            y = p.second;
            
            /*
            get pixel from virtual screen: (x,y), color, layer, isBounder

            if (x,y) is Bounder of this layer -> continue;
            else:
                color for (x,y) -> set it to bounder of layer
                add 4 neighbour pixels
            */
            
            /* 
            Cell* cell = canvas->getCellAt(y, x);
            if (cell->layer() == _layer && (cell->color() == _color || cell->isBounder())) {
                continue;
            }

            setPixel(x, y, _color, _layer, false, canvas);

            if (isValidPos(x + 1, y, canvas))
                s.push(make_pair(x + 1, y));
            if (isValidPos(x - 1, y, canvas))
                s.push(make_pair(x - 1, y));
            if (isValidPos(x, y + 1, canvas))
                s.push(make_pair(x, y + 1));
            if (isValidPos(x, y - 1, canvas))
                s.push(make_pair(x, y - 1));
            */
        }
    }
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

    int getLayer() { return layer; }
    Point getStartPoint() { return startPoint; }
    Point getEndPoint() { return endPoint; }

    void setFillColor(RGBColor color) {
        fillColor = color;
    }

    void setStartPoint(Point point) {
        startPoint = point;
    }

    void setEndPoint(Point point) {
        endPoint = point;
        identifyVertices();
    }

    void setLayer(int l) {
        layer = l;
    }

    void setSelected() {}
    void setUnselected() {}

    virtual void draw() = 0;
    virtual void identifyVertices() = 0;

};

class Line : public Shape {
public:
    Line() {};

    void identifyVertices() override {}

    void draw() override {
        ShapeDrawer::drawLine(startPoint, endPoint, fillColor);
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
    Point center;
public:
    Circle() {}

    Circle(const Point& startPoint, float radius, int layer)
        : Shape(startPoint, startPoint, layer), radius(radius) {}

    void identifyVertices() override {
        int dx = (startPoint.x - endPoint.x);
        int dy = (startPoint.y - endPoint.y);
        radius = min(abs(dx)/2, abs(dy)/2);

        center = Point((startPoint.x + endPoint.x) / 2, (startPoint.y + endPoint.y) / 2);
    }

    void draw() override {
        ShapeDrawer::drawCircle(radius, center);
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
        case LINE_CODE:
            return new Line();
        case CIRCLE_CODE:
            return new Circle();
        case RECTANGLE_CODE:
            return new Rectangle();
        case TRIANGLE_CODE:
            return new EquilateralTriangle();
        default:
            return new Line();
        }
    }
};