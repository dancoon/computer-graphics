// 2D Bézier Curve Implementation in OpenGL with Coordinate Axes
#include <GL/glut.h>
#include <stdio.h>
#include <math.h>

// Control points for a cubic Bézier curve
GLfloat ctrlPoints[4][3] = {
    {-4.0, 0.0, 0.0},  // P0
    {-2.0, 4.0, 0.0},  // P1
    { 2.0, 4.0, 0.0},  // P2
    { 4.0, 0.0, 0.0}   // P3
};

// Function to calculate factorial
int factorial(int n) {
    if (n <= 1)
        return 1;
    return n * factorial(n - 1);
}

// Function to calculate combination nCr
int combination(int n, int r) {
    return factorial(n) / (factorial(r) * factorial(n - r));
}

// Calculate a point on Bézier curve using Bernstein basis
void calculateBezierPoint(float t, float* point, int numControlPoints) {
    int i, j;
    float bernstein;
    point[0] = 0.0;
    point[1] = 0.0;
    point[2] = 0.0;

    for (i = 0; i < numControlPoints; i++) {
        bernstein = combination(numControlPoints - 1, i) *
                    pow(t, i) *
                    pow(1 - t, numControlPoints - 1 - i);

        for (j = 0; j < 3; j++) {
            point[j] += ctrlPoints[i][j] * bernstein;
        }
    }
}

// Draw coordinate axes with labels
void drawAxes() {
    // Draw X and Y axes
    glLineWidth(1.5);

    // X-axis (red)
    glColor3f(1.0, 0.0, 0.0);
    glBegin(GL_LINES);
        glVertex3f(-5.0, 0.0, 0.0);
        glVertex3f(5.0, 0.0, 0.0);
    glEnd();

    // X-axis arrow
    glBegin(GL_TRIANGLES);
        glVertex3f(5.0, 0.0, 0.0);
        glVertex3f(4.8, 0.1, 0.0);
        glVertex3f(4.8, -0.1, 0.0);
    glEnd();

    // Y-axis (green)
    glColor3f(0.0, 1.0, 0.0);
    glBegin(GL_LINES);
        glVertex3f(0.0, -5.0, 0.0);
        glVertex3f(0.0, 5.0, 0.0);
    glEnd();

    // Y-axis arrow
    glBegin(GL_TRIANGLES);
        glVertex3f(0.0, 5.0, 0.0);
        glVertex3f(0.1, 4.8, 0.0);
        glVertex3f(-0.1, 4.8, 0.0);
    glEnd();

    // Draw tick marks on axes
    glColor3f(0.5, 0.5, 0.5);

    // X-axis ticks
    for (int i = -4; i <= 4; i++) {
        if (i == 0) continue; // Skip origin
        glBegin(GL_LINES);
            glVertex3f(i, -0.1, 0.0);
            glVertex3f(i, 0.1, 0.0);
        glEnd();
    }

    // Y-axis ticks
    for (int i = -4; i <= 4; i++) {
        if (i == 0) continue; // Skip origin
        glBegin(GL_LINES);
            glVertex3f(-0.1, i, 0.0);
            glVertex3f(0.1, i, 0.0);
        glEnd();
    }

    // Draw labels
    glColor3f(0.0, 0.0, 0.0);
    glRasterPos3f(5.2, 0.0, 0.0);
    glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, 'X');

    glRasterPos3f(0.0, 5.2, 0.0);
    glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, 'Y');

    glRasterPos3f(0.2, 0.2, 0.0);
    glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, 'O');
}

// Draw the Bézier curve
void drawBezierCurve() {
    float t, point[3];

    // First draw the axes
    drawAxes();

    // Draw control points
    glColor3f(0.0, 0.0, 1.0);  // Blue points
    glPointSize(5.0);
    glBegin(GL_POINTS);

    for (int i = 0; i < 4; i++) {
        glVertex3fv(ctrlPoints[i]);
    }

    glEnd();

    // Draw control polygon
    glColor3f(0.5, 0.5, 0.5);  // Gray lines
    glBegin(GL_LINE_STRIP);

    for (int i = 0; i < 4; i++) {
        glVertex3fv(ctrlPoints[i]);
    }

    glEnd();

    // Draw the Bézier curve
    glColor3f(1.0, 0.0, 0.0);  // Red curve
    glLineWidth(2.0);
    glBegin(GL_LINE_STRIP);

    for (t = 0.0; t <= 1.0; t += 0.01) {
        calculateBezierPoint(t, point, 4);
        glVertex3fv(point);
    }

    glEnd();
    glLineWidth(1.0);
}

// Display function
void display() {
    glClear(GL_COLOR_BUFFER_BIT);
    drawBezierCurve();
    glutSwapBuffers();
}

// Reshape function to maintain aspect ratio
void reshape(int w, int h) {
    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    if (w <= h)
        glOrtho(-5.0, 5.0, -5.0 * (GLfloat) h / (GLfloat) w,
                5.0 * (GLfloat) h / (GLfloat) w, -5.0, 5.0);
    else
        glOrtho(-5.0 * (GLfloat) w / (GLfloat) h,
                5.0 * (GLfloat) w / (GLfloat) h, -5.0, 5.0, -5.0, 5.0);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

// Main function
int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    glutInitWindowSize(500, 500);
    glutInitWindowPosition(100, 100);
    glutCreateWindow("2D Bézier Curve with Axes");

    glClearColor(1.0, 1.0, 1.0, 1.0);
    glutDisplayFunc(display);
    glutReshapeFunc(reshape);

    glutMainLoop();
    return 0;
}

