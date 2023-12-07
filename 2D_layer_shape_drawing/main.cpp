#include <GL/glut.h>
#include <vector>

#include "Define.h"
#include "Global.h"

// Define color
const RGBColor RGBColor::WHITE = RGBColor(255, 255, 255);
const RGBColor RGBColor::BLACK = RGBColor(0,0,0);
const RGBColor RGBColor::RED = RGBColor(255, 0, 0);
const RGBColor RGBColor::GREEN = RGBColor(0,255,0);
const RGBColor RGBColor::BLUE = RGBColor(0, 0, 255);
const RGBColor RGBColor::NONE = RGBColor(-1, -1, -1);
RGBColor RGBColor::BACKROUND = RGBColor::WHITE;
RGBColor RGBColor::BOUNDER = RGBColor::BLACK;

// Value for Global variable
int Global::height = 600;
int Global::width = 1000;
Canvas Global::canvas = Canvas(Global::height, Global::width);

int Global::maxLayer = 0;
int Global::selectedShapeType = LINE_CODE;  // 0: No shape selected, 1: Rectangle, 2: Circle, 3: Triangle
Shape* Global::newShape = FactoryShape::getShape(LINE_CODE);
Shape* Global::selectedShape = nullptr;
bool Global::isSelectingMode = false; // is in selecting mode
RGBColor Global::curFillColor = RGBColor::NONE;
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
    case NONE_CODE:
        Global::curFillColor = RGBColor::NONE;
        break;
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
        glutAddMenuEntry("Line", LINE_CODE);
        glutAddMenuEntry("IR Triangle", IR_TRIANGLE_CODE);
        glutAddMenuEntry("Equi Triangle", EQUI_TRIANGLE_CODE);
        glutAddMenuEntry("Rectangle", RECTANGLE_CODE);
        glutAddMenuEntry("Square", SQUARE_CODE);
        glutAddMenuEntry("Circle", CIRCLE_CODE);
        glutAddMenuEntry("Ellipse", ELLIPSE_CODE);
        glutAddMenuEntry("Hexagon", EQUI_HEXAGON_CODE);
        glutAddMenuEntry("Pentagon", EQUI_PENTAGON_CODE);
        glutAddMenuEntry("Arrow", ARROW_CODE);
        glutAddMenuEntry("Star", STAR_CODE);
        glutAddMenuEntry("Plus", PLUS_CODE);
        glutAddMenuEntry("Minus", MINUS_CODE);
        glutAddMenuEntry("Divide", DIVIDE_CODE);
        glutAddMenuEntry("Times", TIMES_CODE);

        int colorSubMenuId = glutCreateMenu(colorMenu);
        glutAddMenuEntry("None", NONE_CODE);
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
                // unselect old shape
                if (Global::selectedShape) {
                    Global::selectedShape->setUnselected(Global::canvas);
                }

                // select new shape
                Global::selectedShape = Global::drawingApp.findShapebyLayer(Global::canvas.getCellAt(x, y)->layer());

                if (Global::selectedShape) {
                    printf("Selected layer: %d\n", Global::selectedShape->getLayer());
                    Global::selectedShape->setSelected(Global::canvas);
                }
            }
        }
        else if (Global::newShape) {
            //printf("Drawing mode\n");

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
                if (Global::selectedShapeType != LINE_CODE && Global::newShape->getStartPoint() == Point(x, y)) {
                    // remove shape
                    Global::drawingApp.removeShapebyLayer(Global::newShape->getLayer());
                }
                else Global::newShape->setEndPoint(Point(x, y));

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
         // printf("Moving point: (%d,%d)\n", x, y);
        if (!(Global::selectedShapeType != LINE_CODE 
                && Global::newShape->getStartPoint() == Point(x, y))) {
            Global::newShape->setEndPoint(Point(x, y));  
            // redraw to add new shape
            glutPostRedisplay();
        }
    }
}

void display()
{
    // Convert integer values to normalized floating-point values
    float r = RGBColor::BACKROUND.r() / 255.0;
    float g = RGBColor::BACKROUND.g() / 255.0;
    float b = RGBColor::BACKROUND.b() / 255.0;

    glClearColor(r, g, b, 1.0);  // Set clear color using the converted values
    glClear(GL_COLOR_BUFFER_BIT);

    // Global::canvas.rebuild(Global::height, Global::width);
    Global::canvas.clear();

    Global::drawingApp.drawShapes(Global::canvas);

    glFlush();
    
    // swap buffers
    glutSwapBuffers();
}

void reshape(int width, int height)
{
    // Set the viewport and the projection matrix
 
    // glMatrixMode(GL_PROJECTION);
    // glLoadIdentity();
    glViewport(0, 0, width, height);
    glOrtho(0, width, height, 0, -1, 1);  // Make the GLUT and setPixel same coordinate
    // glMatrixMode(GL_MODELVIEW);
    // glLoadIdentity();

    Global::height = height;
    Global::width = width;
    Global::canvas.rebuild(Global::height, Global::width);
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