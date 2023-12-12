#pragma once

#define M_PI 3.14159265358979323846

#include <vector>
#include <GL/glut.h>
#include <stack>

#include "RGB.h"
#include "VirtualScreen.h"
#include "Matrix.h"

using namespace std;

class ShapeDrawer {
public:
    static int calcDistance(Point p1, Point p2) {
        return sqrt((p1.x - p2.x) * (p1.x - p2.x) + (p1.y - p2.y) * (p1.y - p2.y));
    }

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

    static void drawCircle(Canvas &canvas,  int layer, int radius, Point center, Matrix matrix, RGBColor color = RGBColor::BOUNDER) {
        bool isBounder = true;
        if (color != RGBColor::NONE) {
            glColor3ub(color.r(), color.g(), color.b());
        }

        int xT = center.x;
        int yT = center.y;

        // Point firstPoint = matrix.TransformPoint(Point(0 + xT, radius + yT));
        // radius = ShapeDrawer::calcDistance(center, firstPoint);
        
        if (radius <= 0) return;

        vector<Point> v[8];

        int x = 0;
        int y = radius;
        int p = (int)5 / 4 - radius;

        v[1].push_back(Point(x + xT, -y + yT));
        v[4].push_back(Point(-y + xT, x + yT));
        v[6].push_back(Point(x + xT, y + yT));
        v[7].push_back(Point(y + xT, x + yT));

        while (x <= y) {

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

            v[0].push_back(Point(y + xT, -x + yT));
            v[1].push_back(Point(x + xT, -y + yT));
            v[2].push_back(Point(-x + xT, -y + yT));
            v[3].push_back(Point(-y + xT, -x + yT));
            v[4].push_back(Point(-y + xT, x + yT));
            v[5].push_back(Point(-x + xT, y + yT));
            v[6].push_back(Point(x + xT, y + yT));
            v[7].push_back(Point(y + xT, x + yT));
        }

        reverse(v[1].begin(), v[1].end());
        reverse(v[3].begin(), v[3].end());
        reverse(v[5].begin(), v[5].end());
        reverse(v[7].begin(), v[7].end());

        Point prev = matrix.TransformPoint(v[0][0]);

        for (int i = 0; i < 8; i++) {
            int sz = v[i].size();
            for (int j = 0; j < sz; j++) {
                if (i == 0 && j == 0) continue;
                Point p = matrix.TransformPoint(v[i][j]);
                ShapeDrawer::drawLine(canvas, layer, prev, p);
                prev = p;
            }
        }

        ShapeDrawer::drawLine(canvas, layer, prev, matrix.TransformPoint(v[0][0]));
    }

    static void drawEllipse(Canvas& canvas, int layer, int radius_x, int radius_y, Point center, Matrix matrix, RGBColor color = RGBColor::BOUNDER) {
        bool isBounder = true;
        if (color != RGBColor::NONE) {
            glColor3ub(color.r(), color.g(), color.b());
        }

        int xT = center.x;
        int yT = center.y;

        int a = radius_x;
        int b = radius_y;

        if (a <= 0 || b <= 0) return;

        vector<Point> v[4];

        // PT: (x/a)^2 + (y/b)^2 = 1
        //f(x,y) = (xa)^2 + (yb)^2 - (ab)^2
            // < 0: trong
            // = 0: thuoc
            // > 0: ngoai
        // dx = 2.a^2.x
        // dy = 2.b^2.y
            // phan cach tai: dx = dy hay x = (a/b)^2.y

        // P1: x < (a/b)^2.y; bat dau: (0,b) -> theo x
        // p_i = f(x + 1, y - 1/2)

        int p = b * b + (b - 0.5) * (b - 0.5) * a * a - a * a * b * b;
        int st = (a * a) / (b * b);

        int x = 0;
        int y = b;

        v[0].push_back(Point(x + xT, y + yT));
        v[1].push_back(Point(-x + xT, y + yT));
        v[2].push_back(Point(-x + xT, -y + yT));
        v[3].push_back(Point(x + xT, -y + yT));

        while (x <= st * y) {
            if (p < 0) {
                p = p + (2 * x + 3) * b * b;
            }
            else {
                p = p + (2 * x + 3) * b * b - a * a * (2 * y - 2);
                y--;
            }
            x++;

            v[0].push_back(Point(x + xT, y + yT));
            v[1].push_back(Point(-x + xT, y + yT));
            v[2].push_back(Point(-x + xT, -y + yT));
            v[3].push_back(Point(x + xT, -y + yT));
        }

        // P2: x < a -> theo y
        // p_i = f(x + 1/2, y - 1)

        while (y >= 0) {
            if (p < 0) {
                p = p + b * b * (2 * x + 2) - a * a * (2 * y - 3);
                x++;
            }
            else {
                p = p - a * a * (2 * y - 3);
            }
            y--;

            v[0].push_back(Point(x + xT, y + yT));
            v[1].push_back(Point(-x + xT, y + yT));
            v[2].push_back(Point(-x + xT, -y + yT));
            v[3].push_back(Point(x + xT, -y + yT));
        }

        reverse(v[0].begin(), v[0].end());
        reverse(v[2].begin(), v[2].end());

        Point prev = matrix.TransformPoint(v[0][0]);

        for (int i = 0; i < 4; i++) {
            int sz = v[i].size();
            for (int j = 0; j < sz; j++) {
                if (i == 0 && j == 0) continue;
                // if (i != 1) continue;
                Point p = matrix.TransformPoint(v[i][j]);
                ShapeDrawer::drawLine(canvas, layer, prev, p);
                prev = p;
            }
        }

        ShapeDrawer::drawLine(canvas, layer, prev, matrix.TransformPoint(v[0][0]));
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

            if (!cell) continue;
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

    // Lab 3
    Matrix matrix;
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

    virtual void render(Canvas& canvas) {
        draw(canvas);
        if (fillColor != RGBColor::NONE) {
            ShapeDrawer::fillColor(canvas, layer, getStartFillPoint(), fillColor);
        }
    }

    // LAB 3
    void Rotate(double angle) {
        Point center = getStartFillPoint();

        matrix.Translate(-center.x, -center.y);
        matrix.Rotate(angle);
        matrix.Translate(center.x, center.y);

        identifyVertices();
    }

    void Translate(double offsetX, double offsetY) {
        matrix.Translate(offsetX, offsetY);
        identifyVertices();
    }

    void Scale(double scaleX, double scaleY) {
        Point center = getStartFillPoint();
        Point newCenter = Point(center.x * scaleX, center.y * scaleY);

        matrix.Scale(scaleX, scaleY);
        matrix.Translate(center.x - newCenter.x, center.y - newCenter.y);
        identifyVertices();
    }

    void Shear(double shearX, double shearY) {
        matrix.Shear(shearX, shearY);
        identifyVertices();
    }
};

// ================= SHAPE =============================
// LINE - done
class Line : public Shape {
    vector<Point> vertices;
public:
    Line() {};

    void identifyVertices() override {
        vertices.clear();

        vertices.push_back(startPoint);
        vertices.push_back(endPoint);

        vertices = matrix.TransformPoints(vertices);
    }

    Point getStartFillPoint() override {
        return Point((vertices[0].x + vertices[1].x) / 2, (vertices[0].y + vertices[1].y) / 2);
    }

    void draw(Canvas& canvas) override {
        RGBColor color = (fillColor == RGBColor::NONE ? RGBColor::BOUNDER : fillColor);
        ShapeDrawer::drawLine(canvas, this->getLayer(), vertices[0], vertices[1], color);
    }

    void render(Canvas& canvas) override {
        draw(canvas);
    }
};

// TRIANGLE
// isosceles right triangle - vuong can - done
class IR_Triangle : public Shape {
protected:
    vector<Point> vertices;

public:
    IR_Triangle() {}

    IR_Triangle(const Point& startPoint, const Point& endPoint, int layer)
        : Shape(startPoint, endPoint, layer) {}

    void identifyVertices() override {
        // Clear the existing vertices
        vertices.clear();

        int width = (endPoint.x - startPoint.x);
        int height = (endPoint.y - startPoint.y);
        int sz = min(abs(width), abs(height));

        int signW = (width < 0 ? -1 : 1);
        int signH = (height < 0 ? -1 : 1);

        width = sz * signW;
        height = sz * signH;

        // Add the vertices to the vector
        vertices.push_back(startPoint);
        vertices.push_back(Point(startPoint.x + width, startPoint.y));
        vertices.push_back(Point(startPoint.x, startPoint.y + height));
    
        vertices = matrix.TransformPoints(vertices);
    }

    Point getStartFillPoint() override {
        return ShapeDrawer::getCentroid(vertices);
    }

    void draw(Canvas& canvas) override {
        ShapeDrawer::drawShape(canvas, this->getLayer(), vertices);
    }
};

// equilateral triangle - deu - done
class Equi_Triangle : public Shape {
protected:
    vector<Point> vertices;

public:
    Equi_Triangle() {}

    Equi_Triangle(const Point& startPoint, const Point& endPoint, int layer)
        : Shape(startPoint, endPoint, layer) {}

    void identifyVertices() override {
        // Clear the existing vertices
        vertices.clear();

        // Calculate the width and height of the bounding box
        double width = abs(endPoint.x - startPoint.x);
        double height = abs(endPoint.y - startPoint.y);

        // Find the side length of the equilateral triangle (minimum of width and height)
        double sideLength = min(width, height);

        // Calculate the center of the bounding box
        double centerX = (startPoint.x + endPoint.x) / 2.0;
        double centerY = (startPoint.y + endPoint.y) / 2.0;

        // Calculate the height of the equilateral triangle (sqrt(3)/2 * sideLength)
        double triangleHeight = sqrt(3.0) / 2.0 * sideLength;

        // Calculate the vertices of the equilateral triangle
        Point vertex1(centerX, centerY - triangleHeight / 2.0);                      // Top vertex
        Point vertex2(centerX - sideLength / 2.0, centerY + triangleHeight / 2.0);  // Bottom-left vertex
        Point vertex3(centerX + sideLength / 2.0, centerY + triangleHeight / 2.0);  // Bottom-right vertex

        vertices.push_back(vertex1);
        vertices.push_back(vertex2);
        vertices.push_back(vertex3);

        vertices = matrix.TransformPoints(vertices);
    }

    Point getStartFillPoint() override {
        return ShapeDrawer::getCentroid(vertices);
    }

    void draw(Canvas& canvas) override {
        ShapeDrawer::drawShape(canvas, this->getLayer(), vertices);
    }
};

// RECTANGE - done
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

        vertices = matrix.TransformPoints(vertices);
    }

    Point getStartFillPoint() override {
        return ShapeDrawer::getCentroid(vertices);
    }

    void draw(Canvas& canvas) override {
        ShapeDrawer::drawShape(canvas, this->getLayer(), vertices);
    }
};

// SQUARE - done
class Square : public Shape {
protected:
    vector<Point> vertices;

public:
    Square() {}

    Square(const Point& startPoint, const Point& endPoint, int layer)
        : Shape(startPoint, endPoint, layer) {}

    void identifyVertices() override {
        // Clear the existing vertices
        vertices.clear();

        int width = (endPoint.x - startPoint.x);
        int height = (endPoint.y - startPoint.y);
        int sz = min(abs(width), abs(height));

        int signW = (width < 0 ? -1 : 1);
        int signH = (height < 0 ? -1 : 1);

        width = sz * signW;
        height = sz * signH;

        vertices.push_back(startPoint);
        vertices.push_back(Point(startPoint.x + width, startPoint.y));
        vertices.push_back(Point(startPoint.x + width, startPoint.y + height));
        vertices.push_back(Point(startPoint.x, startPoint.y +  height));

        vertices = matrix.TransformPoints(vertices);
    }

    Point getStartFillPoint() override {
        return ShapeDrawer::getCentroid(vertices);
    }

    void draw(Canvas& canvas) override {
        ShapeDrawer::drawShape(canvas, this->getLayer(), vertices);
    }
};

// OVAL
// circle - done
class Circle : public Shape {
protected:
    int radius;
    Point center;
public:
    Circle() {}

    Circle(const Point& startPoint, int radius, int layer)
        : Shape(startPoint, startPoint, layer), radius(radius) {}

    void identifyVertices() override {
        int dx = abs(startPoint.x - endPoint.x);
        int dy = abs(startPoint.y - endPoint.y);
        radius = min(dx / 2, dy / 2);

        center = Point((startPoint.x + endPoint.x) / 2, (startPoint.y + endPoint.y) / 2);

        // center = matrix.TransformPoint(center);
    }

    Point getStartFillPoint() override {
        return matrix.TransformPoint(center);
    }

    void draw(Canvas& canvas) override {
        ShapeDrawer::drawCircle(canvas, this->getLayer(), radius, center, matrix);
    }
};

// ellipse - done
class Ellipse : public Shape {
protected:
    int radius_x, radius_y;
    Point center;
    double scale = 1;
public:
    Ellipse() {}

    void identifyVertices() override {
        radius_x = abs(startPoint.x - endPoint.x) / 2 * scale;
        radius_y = abs(startPoint.y - endPoint.y) / 2 * scale;

        radius_y = min(int(radius_x * 0.7), radius_y);

        center = Point((startPoint.x + endPoint.x) / 2, (startPoint.y + endPoint.y) / 2);

        // center = matrix.TransformPoint(center);
    }

    Point getStartFillPoint() override {
        return matrix.TransformPoint(center);
    }

    void draw(Canvas& canvas) override {
        ShapeDrawer::drawEllipse(canvas, this->getLayer(), radius_x, radius_y, center, matrix);
    }
};

// POLYGON
// equilateral hexagon - luc giac deu - done 
class Hexagon : public Shape {
protected:
    vector<Point> vertices;

public:
    Hexagon() {}

    Hexagon(const Point& startPoint, const Point& endPoint, int layer)
        : Shape(startPoint, endPoint, layer) {}

    void identifyVertices() override {
        // Clear the existing vertices
        vertices.clear();

        int width = abs(endPoint.x - startPoint.x);
        int height = abs(endPoint.y - startPoint.y);
        int hexagonSize = min(width, height);

        Point center;
        center.x = (startPoint.x + endPoint.x) / 2;
        center.y = (startPoint.y + endPoint.y) / 2;

        double angle = M_PI / 6.0;  // Starting angle of 30 degrees (in radians)

        for (int i = 0; i < 6; ++i) {
            Point vertex;
            vertex.x = center.x + static_cast<int>(hexagonSize * cos(angle));
            vertex.y = center.y + static_cast<int>(hexagonSize * sin(angle));

            // Add the vertex to the vector
            vertices.push_back(vertex);

            angle += M_PI / 3.0;  // Increment angle by 60 degrees (in radians)
        }

        vertices = matrix.TransformPoints(vertices);
    }

    Point getStartFillPoint() override {
        return ShapeDrawer::getCentroid(vertices);
    }

    void draw(Canvas& canvas) override {
        ShapeDrawer::drawShape(canvas, this->getLayer(), vertices);
    }
};

// equilateral pentagon - ngu giac deu - done
class Pentagon : public Shape {
protected:
    vector<Point> vertices;

public:
    Pentagon() {}

    Pentagon(const Point& startPoint, const Point& endPoint, int layer)
        : Shape(startPoint, endPoint, layer) {}

    void identifyVertices() override {
        // Clear the existing vertices
        vertices.clear();

        int width = abs(endPoint.x - startPoint.x);
        int height = abs(endPoint.y - startPoint.y);
        int pentagonSize = min(width, height);

        Point center;
        center.x = (startPoint.x + endPoint.x) / 2;
        center.y = (startPoint.y + endPoint.y) / 2;

        double angle = M_PI / 10.0;  // Starting angle of 36 degrees (in radians)

        for (int i = 0; i < 5; ++i) {
            Point vertex;
            vertex.x = center.x + static_cast<int>(pentagonSize * cos(angle));
            vertex.y = center.y + static_cast<int>(pentagonSize * sin(angle));

            // Add the vertex to the vector
            vertices.push_back(vertex);

            angle += 2 * M_PI / 5.0;  // Increment angle by 72 degrees (in radians)
        }

        vertices = matrix.TransformPoints(vertices);
    }

    Point getStartFillPoint() override {
        return ShapeDrawer::getCentroid(vertices);
    }

    void draw(Canvas& canvas) override {
        ShapeDrawer::drawShape(canvas, this->getLayer(), vertices);
    }
};

// ARROW - done
class Arrow : public Shape {
protected:
    vector<Point> vertices;

public:
    Arrow() {}

    Arrow(const Point& startPoint, const Point& endPoint, int layer)
        : Shape(startPoint, endPoint, layer) {}

    void identifyVertices() override {
        // Clear the existing vertices
        vertices.clear();

        // tam
        int x = (startPoint.x + endPoint.x) / 2;
        int y = (startPoint.y + endPoint.y) / 2;

        int a = abs(startPoint.x - endPoint.x); // bounding box - dai
        int b = abs(startPoint.y - endPoint.y); // bounding box - rong

        int tg_ngang = a * 0.5;
        int tg_doc = tg_ngang;

        int hcn_ngang = a - tg_ngang;
        int hcn_doc = tg_doc * 0.6;

        vertices.push_back(Point(x, y - tg_doc / 2)); // dinh tg tren
        vertices.push_back(Point(x + tg_ngang, y)); // dinh tg phai
        vertices.push_back(Point(x, y + tg_doc / 2)); // dinh tg duoi

        vertices.push_back(Point(x, y + hcn_doc / 2)); // dinh hcn phai duoi
        vertices.push_back(Point(x - hcn_ngang, y + hcn_doc / 2)); // dinh hcn trai duoi
        vertices.push_back(Point(x - hcn_ngang, y - hcn_doc / 2)); // dinh hcn trai tren
        vertices.push_back(Point(x, y - hcn_doc / 2)); // dinh hcn phai tren

        vertices = matrix.TransformPoints(vertices);
    }

    Point getStartFillPoint() override {
        return ShapeDrawer::getCentroid(vertices);
    }

    void draw(Canvas& canvas) override {
        ShapeDrawer::drawShape(canvas, this->getLayer(), vertices);
    }
};

// STAR - done
class Star: public Shape {
protected:
    vector<Point> vertices;

public:
    Star() {}

    Star(const Point& startPoint, const Point& endPoint, int layer)
        : Shape(startPoint, endPoint, layer) {}

    void identifyVertices() override {
        // Clear the existing vertices
        vertices.clear();

        int numPoints = 5;
        double angleBetweenPoints = 2 * M_PI / numPoints;  // Angle between each point
        double startAngle = M_PI / 4;

        Point centerPoint = Point((startPoint.x + endPoint.x) / 2, (startPoint.y + endPoint.y) / 2);
        int radius_big = min(abs(startPoint.x - endPoint.x), abs(startPoint.y + endPoint.y));
        int radius_small = radius_big * 0.4;

        for (int i = 0; i < numPoints; ++i) {
            double angle = i * angleBetweenPoints + startAngle;  // Angle for the current point

            vertices.push_back(Point(centerPoint.x + (int)(radius_big * cos(angle)), centerPoint.y + (int)(radius_big * sin(angle))));

            angle += angleBetweenPoints * 0.5;  // Angle for the current point

            vertices.push_back(Point(centerPoint.x + (int)(radius_small * cos(angle)), centerPoint.y + (int)(radius_small * sin(angle))));
        }

        vertices = matrix.TransformPoints(vertices);
    }

    Point getStartFillPoint() override {
        return ShapeDrawer::getCentroid(vertices);
    }

    void draw(Canvas& canvas) override {
        ShapeDrawer::drawShape(canvas, this->getLayer(), vertices);
    }
};

// PLUS - done
class Plus: public Shape {
protected:
    vector<Point> vertices;

public:
    Plus() {}

    Plus(const Point& startPoint, const Point& endPoint, int layer)
        : Shape(startPoint, endPoint, layer) {}

    void identifyVertices() override {
        // Clear the existing vertices
        vertices.clear();

        Point centerPoint((startPoint.x + endPoint.x) / 2, (startPoint.y + endPoint.y) / 2);
        int x = centerPoint.x;
        int y = centerPoint.y;

        int a = abs(endPoint.x - startPoint.x); // dai
        int b = abs(endPoint.y - startPoint.y); // rong

        a = max(a, b);
        b = a * 0.3;

        vertices.push_back(Point(x - b / 2, y - b / 2)); // giao trai tren
        vertices.push_back(Point(x - b / 2, y - a / 2)); // tren trai
        vertices.push_back(Point(x + b / 2, y - a / 2)); // tren phai

        vertices.push_back(Point(x + b / 2, y - b / 2)); // giao phai tren
        vertices.push_back(Point(x + a / 2, y - b / 2)); // phai tren
        vertices.push_back(Point(x + a / 2, y + b / 2)); // phai duoi

        vertices.push_back(Point(x + b / 2, y + b / 2)); // giao phai duoi
        vertices.push_back(Point(x + b / 2, y + a / 2)); // duoi phai
        vertices.push_back(Point(x - b / 2, y + a / 2)); // duoi trai

        vertices.push_back(Point(x - b / 2, y + b / 2)); // giao trai duoi
        vertices.push_back(Point(x - a / 2, y + b / 2)); // trai duoi
        vertices.push_back(Point(x - a / 2, y - b / 2)); // trai tren

        vertices = matrix.TransformPoints(vertices);
    }

    Point getStartFillPoint() override {
        return ShapeDrawer::getCentroid(vertices);
    }

    void draw(Canvas& canvas) override {
        ShapeDrawer::drawShape(canvas, this->getLayer(), vertices);
    }
};

// MINUS - done
class Minus : public Shape {
protected:
    vector<Point> vertices;

public:
    Minus() {}

    Minus(const Point& startPoint, const Point& endPoint, int layer)
        : Shape(startPoint, endPoint, layer) {}

    void identifyVertices() override {
        // Clear the existing vertices
        vertices.clear();

        // Add the vertices to the vector
        int width = abs(endPoint.x - startPoint.x);
        int height = 0.3 * width; // Height is 0.3 times the width

        int centerX = (startPoint.x + endPoint.x) / 2;
        int centerY = (startPoint.y + endPoint.y) / 2;

        // Calculate the four vertices of the minus shape
        Point topLeft(min(startPoint.x, endPoint.x), centerY - height / 2);
        Point topRight(max(startPoint.x, endPoint.x), centerY - height / 2);
        Point bottomLeft(min(startPoint.x, endPoint.x), centerY + height / 2);
        Point bottomRight(max(startPoint.x, endPoint.x), centerY + height / 2);

        vertices.push_back(topLeft);
        vertices.push_back(topRight);
        vertices.push_back(bottomRight);
        vertices.push_back(bottomLeft);

        vertices = matrix.TransformPoints(vertices);
    }

    Point getStartFillPoint() override {
        return ShapeDrawer::getCentroid(vertices);
    }

    void draw(Canvas& canvas) override {
        ShapeDrawer::drawShape(canvas, this->getLayer(), vertices);
    }
};

// TIMES - done
class Times : public Shape {
protected:
    vector<Point> vertices;

public:
    Times() {}

    Times(const Point& startPoint, const Point& endPoint, int layer)
        : Shape(startPoint, endPoint, layer) {}

    void identifyVertices() override {
        // Clear the existing vertices
        vertices.clear();

        int x = (startPoint.x + endPoint.x) / 2;
        int y = (startPoint.y + endPoint.y) / 2;

        int a = abs(startPoint.x - endPoint.x); // dai
        int b = a * 0.4; // rong

        vertices.push_back(Point(x, y - b / 2)); // giao tren
        vertices.push_back(Point(x + a / 2 - b / 2, y - a / 2)); // tren trai
        vertices.push_back(Point(x + a / 2, y - a / 2)); // tren phai

        vertices.push_back(Point(x + b / 2, y)); // giao phai
        vertices.push_back(Point(x + a / 2, y + a / 2)); // phai phai
        vertices.push_back(Point(x + a / 2 - b / 2, y + a / 2)); // phai trai

        vertices.push_back(Point(x, y + b / 2)); // giao duoi
        vertices.push_back(Point(x - a / 2 + b / 2, y + a / 2)); // duoi phai
        vertices.push_back(Point(x - a / 2, y + a / 2)); // duoi trai

        vertices.push_back(Point(x - b / 2, y)); // giao trai
        vertices.push_back(Point(x - a / 2, y - a / 2)); // trai trai
        vertices.push_back(Point(x - a / 2 + b / 2, y - a / 2)); // trai phai

        vertices = matrix.TransformPoints(vertices);
    }

    Point getStartFillPoint() override {
        return ShapeDrawer::getCentroid(vertices);
    }

    void draw(Canvas& canvas) override {
        ShapeDrawer::drawShape(canvas, this->getLayer(), vertices);
    }
};

// DIVIDE - done
class Divide : public Shape {
protected:
    vector<Point> vertices;

public:
    Divide() {}

    Divide(const Point& startPoint, const Point& endPoint, int layer)
        : Shape(startPoint, endPoint, layer) {}

    void identifyVertices() override {
        // Clear the existing vertices
        vertices.clear();

        int x = (startPoint.x + endPoint.x) / 2;
        int y = (startPoint.y + endPoint.y) / 2;

        int a = abs(startPoint.x - endPoint.x); // dai
        int b = 1.3 * a; // rong
        int c = 0.3 * a; // do day

        vertices.push_back(Point(x + b / 2 - c, y - a / 2)); // trai tren
        vertices.push_back(Point(x + b / 2, y - a / 2)); // phai tren
        vertices.push_back(Point(x - b / 2 + c, y + a / 2)); // phai duoi
        vertices.push_back(Point(x - b / 2, y + a / 2)); // trai duoi

        vertices = matrix.TransformPoints(vertices);
    }

    Point getStartFillPoint() override {
        return ShapeDrawer::getCentroid(vertices);
    }

    void draw(Canvas& canvas) override {
        ShapeDrawer::drawShape(canvas, this->getLayer(), vertices);
    }
};

// =============== END SHAPE ===========================

#include "Define.h"

class FactoryShape {
public:
    static Shape* getShape(int shapeType) {
        switch (shapeType)
        {
        case LINE_CODE:
            return new Line();
        case IR_TRIANGLE_CODE:
            return new IR_Triangle();
        case EQUI_TRIANGLE_CODE:
            return new Equi_Triangle();
        case RECTANGLE_CODE:
            return new Rectangle();
        case SQUARE_CODE:
            return new Square();
        case CIRCLE_CODE:
            return new Circle();
        case ELLIPSE_CODE:
            return new Ellipse();
        case EQUI_HEXAGON_CODE:
            return new Hexagon();
        case EQUI_PENTAGON_CODE:
            return new Pentagon();
        case ARROW_CODE:
            return new Arrow();
        case STAR_CODE:
            return new Star();
        case PLUS_CODE:
            return new Plus();
        case MINUS_CODE:
            return new Minus();
        case DIVIDE_CODE:
            return new Divide();
        case TIMES_CODE:
            return new Times();
        default:
            return new Line();
        }
    }
};


