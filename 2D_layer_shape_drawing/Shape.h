#pragma once

#include <vector>
#include <GL/glut.h>
#include <stack>

#include "RGB.h"
#include "VirtualScreen.h"

using namespace std;

class Point {
public:
    int x;
    int y;

    Point(): x(0), y(0) {}
    Point(int x, int y) : x(x), y(y) {}

    bool operator==(const Point& other) const {
        return (x == other.x) && (y == other.y);
    }
};

class ShapeDrawer {
public:
    static void setPixel(Canvas& canvas, int x, int y, int layer, RGBColor color, bool isBounder) {
        if (canvas.setCell(x,y, layer, color, isBounder) && color != RGBColor::NONE) {
            glBegin(GL_POINTS);
            glVertex2i(x, y);
            glEnd();
        }
    }

    static void drawShape(Canvas& canvas, int layer, const vector<Point>& vertices) {
        // Draw bounder
        int len = vertices.size();
        for (int i = 0; i < len; i++) {
            Point v1 = vertices[i];
            Point v2 = vertices[(i + 1) % len];

            ShapeDrawer::drawLine(canvas, layer, v1, v2);
        }
    }

    static void drawCircle(Canvas &canvas,  int layer, int radius, Point center, RGBColor color = RGBColor::BOUNDER) {
        bool isBounder = true;
        if (color != RGBColor::NONE) {
            glColor3ub(color.r(), color.g(), color.b());
        }

        if (radius <= 0) return;

        int xT = center.x;
        int yT = center.y;

        ShapeDrawer::setPixel(canvas, 0 + xT, radius + yT, layer, color, isBounder);
        ShapeDrawer::setPixel(canvas, 0 + xT, -radius + yT, layer, color, isBounder);
        ShapeDrawer::setPixel(canvas, radius + xT, 0 + yT, layer, color, isBounder);
        ShapeDrawer::setPixel(canvas, -radius + xT, 0 + yT, layer, color, isBounder);

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
            ShapeDrawer::setPixel(canvas, x + xT, y + yT, layer, color, isBounder); 	ShapeDrawer::setPixel(canvas, -x + xT, y + yT, layer, color, isBounder);
            ShapeDrawer::setPixel(canvas, x + xT, -y + yT, layer, color, isBounder);	ShapeDrawer::setPixel(canvas, -x + xT, -y + yT, layer, color, isBounder);
            ShapeDrawer::setPixel(canvas, y + xT, x + yT, layer, color, isBounder); 	ShapeDrawer::setPixel(canvas, -y + xT, x + yT, layer, color, isBounder);
            ShapeDrawer::setPixel(canvas, y + xT, -x + yT, layer, color, isBounder);	ShapeDrawer::setPixel(canvas, -y + xT, -x + yT, layer, color, isBounder);
        }
    }

    static void drawLine(Canvas& canvas, int layer, Point start, Point end, RGBColor color = RGBColor::BOUNDER) {
        // line is always a bounder
        bool isBounder = true;

        if (color != RGBColor::NONE) {
            glColor3ub(color.r(), color.g(), color.b());
        }

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
                ShapeDrawer::setPixel(canvas, x, y, layer, color, isBounder);
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
                ShapeDrawer::setPixel(canvas, x, y, layer, color, isBounder);
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

    static void fillColor(Canvas& canvas, int layer, Point startFillPoint, RGBColor fillColor) {
        bool isBounder = false;
        if (fillColor != RGBColor::NONE) {
            glColor3ub(fillColor.r(), fillColor.g(), fillColor.b());
        }

        // Non-recursive implementation
        stack<Point> s;

        int x = startFillPoint.x;
        int y = startFillPoint.y;

        s.push(startFillPoint);

        while (!s.empty()) {
            Point p = s.top();
            s.pop();

            x = p.x;
            y = p.y;
            
            Cell* cell = canvas.getCellAt(x,y);

            if (cell->layer() == layer && cell->isBounder()) continue; // bounder of layer
            if (cell->layer() == layer && cell->color() == fillColor) continue; // already color

            ShapeDrawer::setPixel(canvas, x, y, layer, fillColor, isBounder);

            if (canvas.isValidCell(x + 1, y))
                s.push(Point(x + 1, y));
            if (canvas.isValidCell(x - 1, y))
                s.push(Point(x - 1, y));
            if (canvas.isValidCell(x, y + 1))
                s.push(Point(x, y + 1));
            if (canvas.isValidCell(x, y - 1))
                s.push(Point(x, y - 1));
        }
    }

    static Point getCentroid(vector<Point> vertices) {
        int len = vertices.size();
        int sumX = 0;
        int sumY = 0;

        for (int i = 0; i < len; i++) {
            sumX += vertices[i].x;
            sumY += vertices[i].y;
        }

        return Point(sumX / len, sumY / len);
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

    void setSelected(Canvas& canvas) {}
    void setUnselected(Canvas& canvas) {}

    virtual void draw(Canvas &canvas) = 0;
    virtual void identifyVertices() = 0;
    virtual Point getStartFillPoint() = 0;

    void render(Canvas& canvas) {
        draw(canvas);
        if (fillColor != RGBColor::NONE) {
            ShapeDrawer::fillColor(canvas, layer, getStartFillPoint(), fillColor);
        }
    }
};

class Line : public Shape {
public:
    Line() {};

    void identifyVertices() override {}

    Point getStartFillPoint() override {
        return startPoint;
    }

    void draw(Canvas& canvas) override {
        RGBColor color = (fillColor == RGBColor::NONE ? RGBColor::BOUNDER : fillColor);
        ShapeDrawer::drawLine(canvas, this->getLayer(), startPoint, endPoint, color);
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

    Point getStartFillPoint() override {
        return ShapeDrawer::getCentroid(vertices);
    }

    void draw(Canvas& canvas) override {
        ShapeDrawer::drawShape(canvas, this->getLayer(), vertices);
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
        int dx = abs(startPoint.x - endPoint.x);
        int dy = abs(startPoint.y - endPoint.y);
        radius = min(dx/2, dy/2);

        center = Point((startPoint.x + endPoint.x) / 2, (startPoint.y + endPoint.y) / 2);
    }

    Point getStartFillPoint() override {
        return center;
    }

    void draw(Canvas& canvas) override {
        ShapeDrawer::drawCircle(canvas, this->getLayer(), radius, center);
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

    Point getStartFillPoint() override {
        return ShapeDrawer::getCentroid(vertices);
    }

    void draw(Canvas& canvas) override {
        ShapeDrawer::drawShape(canvas, this->getLayer(), vertices);
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