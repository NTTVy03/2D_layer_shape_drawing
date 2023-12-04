#pragma once

#include "Shape.h"

class Layer {
private:
    int id;
    vector<Shape*> shapes;

public:
    Layer(int layerId) : id(layerId) {}

    void addShape(Shape* shape) {
        shapes.push_back(shape);
    }

    int getId() const {
        return id;
    }

    void drawShapes() const {
        for (const auto& shape : shapes) {
            shape->identifyVertices();
            shape->draw();
        }
    }

    void removeShape(int shapeIndex) {
        if (shapeIndex >= 0 && shapeIndex < shapes.size()) {
            delete shapes[shapeIndex];
            shapes.erase(shapes.begin() + shapeIndex);
        }
    }
    // Add any additional methods you need for the Layer class
};