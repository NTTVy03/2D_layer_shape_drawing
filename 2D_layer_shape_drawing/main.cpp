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
int Global::height = 600;
int Global::width = 800;
Canvas Global::canvas = Canvas(height, width);

int Global::maxLayer = 0;
int Global::selectedShapeType = LINE_CODE;  // 0: No shape selected, 1: Rectangle, 2: Circle, 3: Triangle
Shape* Global::newShape = FactoryShape::getShape(LINE_CODE);
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
        Global::switchMode(true);
        printf("Action: Select a shape\n");
        break;
    }
        // Add additional cases for other menu options
    }
}

void shapeTypeMenu(int option)
{
    // swich to Drawing mode
    Global::switchMode(false);

    Global::selectedShapeType = option;
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
    y = Global::height - y;
    if (button == GLUT_RIGHT_BUTTON && state == GLUT_DOWN)
    {
        // show MENU
        int shapeTypeSubMenuId = glutCreateMenu(shapeTypeMenu);
        glutAddMenuEntry("Line", LINE_CODE);
        glutAddMenuEntry("Rectangle", RECTANGLE_CODE);
        glutAddMenuEntry("Circle", CIRCLE_CODE);
        glutAddMenuEntry("Triangle", TRIANGLE_CODE);

        int colorSubMenuId = glutCreateMenu(colorMenu);
        glutAddMenuEntry("Red", RED_CODE);
        glutAddMenuEntry("Green", GREEN_CODE);
        glutAddMenuEntry("Blue", BLUE_CODE);

        int mainMenuId = glutCreateMenu(menu);
        glutAddSubMenu("Choose Shape Type", shapeTypeSubMenuId);
        glutAddMenuEntry("Select Shape", SELECT_MODE);
        glutAddSubMenu("Select Color", colorSubMenuId);
        // Add additional menu entries for other options

        glutAttachMenu(GLUT_RIGHT_BUTTON);
    }
    else if (button == GLUT_LEFT_BUTTON) {
        if (Global::isSelectingMode) {
            // selecting mode
            if (state == GLUT_UP) {
                // Find layer that (x,y) in rectangle (Start Point) - (End Point)
                Global::selectedShape = Global::drawingApp.findShapeContains(x, y);
                Global::selectedShape->setSelected();
            }
        }
        else if (Global::newShape) {
            printf("Drawing mode\n");

            // drawing mode
            if (state == GLUT_DOWN) {
                // add newShape to DrawApp
                Global::drawingApp.addShape(Global::newShape);
                
                Global::newShape->setLayer(++Global::maxLayer);
                Global::newShape->setFillColor(Global::curFillColor);

                // printf("Start point: (%d,%d)\n", x, y);
                Global::newShape->setStartPoint(Point(x, y));
            }
            else if (state == GLUT_UP) {
                // printf("End point: (%d,%d)\n", x, y);
                Global::newShape->setEndPoint(Point(x, y));

                Global::newShape = FactoryShape::getShape(Global::selectedShapeType);

                // redraw to add new shape
                glutPostRedisplay();
            }
        }
    }
}

void motion(int x, int y) {
    // draw shape while moving 
    if (Global::newShape) {
         y = Global::height - y;
       
         // printf("Moving point: (%d,%d)\n", x, y);
         Global::newShape->setEndPoint(Point(x, y));

        // redraw to add new shape
        glutPostRedisplay();
    }
}

void display()
{
    // Render here    
    glClear(GL_COLOR_BUFFER_BIT);

    Global::canvas.rebuild(Global::height, Global:: width);

    Global::drawingApp.drawShapes();

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

    // match Canvas with new size of screen
    Global::height = height;
    Global::width = width;
    Global::canvas.rebuild(height, width);
}

int main(int argc, char** argv)
{
    // Initialize GLUT
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);

    // Create a window
    glutInitWindowSize(Global::width, Global::height);
    glutCreateWindow("Drawing App");

    // Set the display callback
    glutDisplayFunc(display);

    // Set the reshape callback
    glutReshapeFunc(reshape);

    // Register the mouse callback
    glutMouseFunc(mouse);

    // Register the motion callback - drawing
    glutMotionFunc(motion);

    // Enter the main loop
    glutMainLoop();

    return 0;
}