#pragma once

#include "Shape.h"
#include "Util.h"

class DrawingApp {
private:
    vector<Shape*> shapes;
public:
    void addShape(Shape* newShape) {
        shapes.push_back(newShape);
    }

    void drawShapes() const {
        for (const auto& shape : shapes) {
            shape->draw();
        }
    }

    void removeShapebyLayer(int layer) {
        int sz = shapes.size();

        for (int shapeIndex = 0; shapeIndex < sz; shapeIndex++) {
            if (shapes[shapeIndex]->getLayer() == layer) {
                shapes.erase(shapes.begin() + shapeIndex);
            }
        }
    }    

    Shape* findShapeContains(int x, int y) {
        for (const auto& shape : shapes) {
            int minX = min(shape->getStartPoint().x, shape->getEndPoint().x);
            int maxX = max(shape->getStartPoint().x, shape->getEndPoint().x);

            int minY = min(shape->getStartPoint().y, shape->getEndPoint().y);
            int maxY = max(shape->getStartPoint().y, shape->getEndPoint().y);

            if (isIn(x, minX, maxX) && isIn(y, minY, maxY)) {
                return shape;
            }
        }
    }
    // Add any additional methods you need for the DrawingApp class
};

