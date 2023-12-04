#include <GL/glut.h>
#include <vector>

#include "Define.h"
#include "Drawing.h"
#include "Global.h"

// Define color
const RGBColor RGBColor::WHITE = RGBColor(255, 255, 255);
const RGBColor RGBColor::BLACK = RGBColor(0,0,0);
const RGBColor RGBColor::RED = RGBColor(255, 0, 0);
const RGBColor RGBColor::GREEN = RGBColor(0,255,0);
const RGBColor RGBColor::BLUE = RGBColor(0,0,255);

// Value for Global variable
int Global::maxLayer = 0;
int Global::selectedShapeType = 0;  // 0: No shape selected, 1: Rectangle, 2: Circle, 3: Triangle
Shape* Global::newShape = nullptr;
Shape* Global::selectedShape = nullptr;
bool Global::isSelectingMode = false; // is in selecting mode
RGBColor Global::curFillColor = RGBColor::WHITE;
DrawingApp Global::drawingApp = DrawingApp();

void menu(int option)
{
    switch (option)
    {
    case SELECT_MODE: // Switch to action select a shape
    {
        // unable drawing mode
        if (Global::newShape) {
            delete Global::newShape;
        }
        Global::newShape = nullptr;

        Global::isSelectingMode = true;
        Global::selectedShape = nullptr;

        printf("Action: Select a shape\n");
        break;
    }
        // Add additional cases for other menu options
    }
}

void shapeTypeMenu(int option)
{
    // swich to Drawing mode
    Global::isSelectingMode = false;
    Global::selectedShape = nullptr;

    Global::selectedShapeType = option;

    if (Global::newShape) {
        delete Global::newShape;
    }

    Global::newShape = FactoryShape::getShape(Global::selectedShapeType);

    printf("Action: Select shape type %d\n", option);
}

void colorMenu(int option)
{
    printf("Action: Select color %d\n", option);

    // Set the selected color for the next shape to be drawn
    switch (option)
    {
    case RED_CODE:
        Global::curFillColor = RGBColor::RED;
        break;
    case GREEN_CODE:
        Global::curFillColor = RGBColor::GREEN;
        break;
    case BLUE_CODE:
        Global::curFillColor = RGBColor::BLUE;
        break;
    }

    if (Global::isSelectingMode && Global::selectedShape)
    {
        // Refill the selected shape with the selected color
        Global::selectedShape->setFillColor(Global::curFillColor);
        glutPostRedisplay();
    }
}

void mouse(int button, int state, int x, int y)
{
    if (button == GLUT_RIGHT_BUTTON && state == GLUT_DOWN)
    {
        // show MENU
        int shapeTypeSubMenuId = glutCreateMenu(shapeTypeMenu);
        glutAddMenuEntry("Rectangle", RECTANGLE_CODE);
        glutAddMenuEntry("Circle", CIRCLE_CODE);
        glutAddMenuEntry("Triangle", TRIANGLE_CODE);

        int colorSubMenuId = glutCreateMenu(colorMenu);
        glutAddMenuEntry("Red", RED_CODE);
        glutAddMenuEntry("Green", GREEN_CODE);
        glutAddMenuEntry("Blue", BLUE_CODE);

        int mainMenuId = glutCreateMenu(menu);
        glutAddSubMenu("Choose Shape Type", shapeTypeSubMenuId);
        glutAddMenuEntry("Switch to Action Select Shape", SELECT_MODE);
        glutAddSubMenu("Select Color", colorSubMenuId);
        // Add additional menu entries for other options

        glutAttachMenu(GLUT_RIGHT_BUTTON);
    }
    else if (button == GLUT_LEFT_BUTTON) {
        if (Global::isSelectingMode) {
            // selecting mode
            if (state == GLUT_UP) {
                // Find layer that (x,y) in rectangle (Start Point) - (End Point)
            }
        }
        else if (Global::newShape) {
            printf("Drawing mode\n");

            // drawing mode
            if (state == GLUT_DOWN) {
                // Start Point
                printf("Start point: (%d,%d)\n", x, y);
                Global::newShape->setStartPoint(Point(x,y));
            }
            else if (state == GLUT_UP) {
                // End Point
                printf("End point: (%d,%d)\n", x, y);
                Global::newShape->setEndPoint(Point(x, y));

                // add newShape to DrawApp
                Global::newShape->setLayer(++Global::maxLayer);
                Global::newShape->setFillColor(Global::curFillColor);
                
                Global::drawingApp.addLayer(Global::maxLayer);
                Global::drawingApp.addShapeToLayer(Global::maxLayer, Global::newShape);

                Global::newShape = nullptr;

                // redraw to add new shape
                glutPostRedisplay();
            }
        }
    }
}

void display()
{
    // Render here
    glClear(GL_COLOR_BUFFER_BIT);

    Global::drawingApp.drawLayers();

    glFlush();
    
    // Swap buffers
    glutSwapBuffers();
}

void reshape(int width, int height)
{
    // Set the viewport and the projection matrix
    glViewport(0, 0, width, height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0, width, 0, height);
    glMatrixMode(GL_MODELVIEW);
}

int main(int argc, char** argv)
{
    // Initialize GLUT
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);

    // Create a window
    glutInitWindowSize(800, 600);
    glutCreateWindow("Drawing App");

    // Set the display callback
    glutDisplayFunc(display);

    // Set the reshape callback
    glutReshapeFunc(reshape);

    // Register the mouse callback - STEP 4
    glutMouseFunc(mouse);

    // Enter the main loop
    glutMainLoop();

    return 0;
}