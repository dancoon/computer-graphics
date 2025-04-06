// 3D Bézier Curve Implementation in OpenGL with Enhanced Coordinate Axes
#include <GL/glut.h>
#include <stdio.h>
#include <math.h>
#include <string.h>

// Control points for a cubic Bézier curve in 3D
GLfloat ctrlPoints[4][3] = {
    {-4.0, 0.0, -2.0},  // P0
    {-2.0, 4.0, 2.0},   // P1
    { 2.0, 4.0, 0.0},   // P2
    { 4.0, 0.0, 3.0}    // P3
};

// Rotation angles
static GLfloat xRot = 20.0f;
static GLfloat yRot = 30.0f;
static GLfloat zRot = 0.0f;

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

// Function to draw text in 3D space
void draw3DText(const char* text, float x, float y, float z) {
    glRasterPos3f(x, y, z);
    
    for (int i = 0; i < strlen(text); i++) {
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, text[i]);
    }
}

// Draw enhanced 3D coordinate axes with labels and grid
void drawAxes() {
    // Draw main coordinate axes
    glLineWidth(2.0);
    
    // X-axis (Red)
    glColor3f(1.0, 0.0, 0.0);
    glBegin(GL_LINES);
        glVertex3f(-5.0, 0.0, 0.0);
        glVertex3f(5.0, 0.0, 0.0);
    glEnd();
    
    // X-axis arrow
    glBegin(GL_TRIANGLES);
        glVertex3f(5.0, 0.0, 0.0);
        glVertex3f(4.8, 0.1, 0.1);
        glVertex3f(4.8, -0.1, -0.1);
    glEnd();
    
    draw3DText("X", 5.3, 0.0, 0.0);
    
    // Y-axis (Green)
    glColor3f(0.0, 1.0, 0.0);
    glBegin(GL_LINES);
        glVertex3f(0.0, -5.0, 0.0);
        glVertex3f(0.0, 5.0, 0.0);
    glEnd();
    
    // Y-axis arrow
    glBegin(GL_TRIANGLES);
        glVertex3f(0.0, 5.0, 0.0);
        glVertex3f(0.1, 4.8, 0.1);
        glVertex3f(-0.1, 4.8, -0.1);
    glEnd();
    
    draw3DText("Y", 0.0, 5.3, 0.0);
    
    // Z-axis (Blue)
    glColor3f(0.0, 0.0, 1.0);
    glBegin(GL_LINES);
        glVertex3f(0.0, 0.0, -5.0);
        glVertex3f(0.0, 0.0, 5.0);
    glEnd();
    
    // Z-axis arrow
    glBegin(GL_TRIANGLES);
        glVertex3f(0.0, 0.0, 5.0);
        glVertex3f(0.1, 0.1, 4.8);
        glVertex3f(-0.1, -0.1, 4.8);
    glEnd();
    
    draw3DText("Z", 0.0, 0.0, 5.3);
    
    // Origin label
    glColor3f(0.0, 0.0, 0.0);
    draw3DText("O", 0.2, 0.2, 0.2);
    
    // Draw grid on XZ plane for better spatial reference
    glLineWidth(0.5);
    glColor3f(0.7, 0.7, 0.7);
    
    // Draw grid on XZ plane
    for (int i = -5; i <= 5; i++) {
        if (i == 0) continue; // Skip axes
        
        // Lines parallel to X-axis
        glBegin(GL_LINES);
            glVertex3f(-5.0, 0.0, i);
            glVertex3f(5.0, 0.0, i);
        glEnd();
        
        // Lines parallel to Z-axis
        glBegin(GL_LINES);
            glVertex3f(i, 0.0, -5.0);
            glVertex3f(i, 0.0, 5.0);
        glEnd();
    }
    
    glLineWidth(1.0);
}

// Draw the 3D Bézier curve
void drawBezierCurve() {
    float t, point[3];
    
    // Draw coordinate axes first
    drawAxes();
    
    // Draw control points
    glColor3f(0.0, 0.5, 1.0);  // Blue points
    glPointSize(6.0);
    glBegin(GL_POINTS);
    
    for (int i = 0; i < 4; i++) {
        glVertex3fv(ctrlPoints[i]);
    }
    
    glEnd();
    
    // Draw control polygon
    glColor3f(0.5, 0.5, 0.5);  // Gray lines
    glLineWidth(1.0);
    glBegin(GL_LINE_STRIP);
    
    for (int i = 0; i < 4; i++) {
        glVertex3fv(ctrlPoints[i]);
    }
    
    glEnd();
    
    // Draw vertical lines from control points to XZ plane for better spatial understanding
    glColor3f(0.7, 0.7, 0.7);  // Light gray
    glLineStipple(1, 0xAAAA);  // Dashed line pattern
    glEnable(GL_LINE_STIPPLE);
    
    glBegin(GL_LINES);
    for (int i = 0; i < 4; i++) {
        glVertex3f(ctrlPoints[i][0], ctrlPoints[i][1], ctrlPoints[i][2]);
        glVertex3f(ctrlPoints[i][0], 0.0, ctrlPoints[i][2]);
    }
    glEnd();
    
    glDisable(GL_LINE_STIPPLE);
    
    // Draw the Bézier curve
    glColor3f(1.0, 0.0, 0.0);  // Red curve
    glLineWidth(2.5);
    glBegin(GL_LINE_STRIP);
    
    for (t = 0.0; t <= 1.0; t += 0.01) {
        calculateBezierPoint(t, point, 4);
        glVertex3fv(point);
    }
    
    glEnd();
    glLineWidth(1.0);
    
    // Label the curve
    glColor3f(0.0, 0.0, 0.0);
    float midPoint[3];
    calculateBezierPoint(0.5, midPoint, 4);
    draw3DText("Bezier Curve", midPoint[0], midPoint[1] + 0.5, midPoint[2]);
}

// Display function
void display() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();
    
    // Set viewing position and angle
    glTranslatef(0.0f, 0.0f, -20.0f);
    glRotatef(xRot, 1.0f, 0.0f, 0.0f);
    glRotatef(yRot, 0.0f, 1.0f, 0.0f);
    glRotatef(zRot, 0.0f, 0.0f, 1.0f);
    
    drawBezierCurve();
    
    // Display rotation information
    glLoadIdentity();
    glColor3f(0.0, 0.0, 0.0);
    glRasterPos2f(-4.7, -3.7);
    char rotInfo[50];
    sprintf(rotInfo, "Rotation: X=%.1f, Y=%.1f, Z=%.1f", xRot, yRot, zRot);
    for (int i = 0; i < strlen(rotInfo); i++) {
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, rotInfo[i]);
    }
    
    glutSwapBuffers();
}

// Reshape function
void reshape(int w, int h) {
    GLfloat fAspect;
    
    if (h == 0)
        h = 1;
        
    glViewport(0, 0, w, h);
    
    fAspect = (GLfloat)w / (GLfloat)h;
    
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    
    gluPerspective(45.0f, fAspect, 1.0f, 100.0f);
    
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

// Special keys for rotation
void specialKeys(int key, int x, int y) {
    if (key == GLUT_KEY_UP)
        xRot -= 5.0f;
    if (key == GLUT_KEY_DOWN)
        xRot += 5.0f;
    if (key == GLUT_KEY_LEFT)
        yRot -= 5.0f;
    if (key == GLUT_KEY_RIGHT)
        yRot += 5.0f;
    if (key == GLUT_KEY_PAGE_UP)
        zRot += 5.0f;
    if (key == GLUT_KEY_PAGE_DOWN)
        zRot -= 5.0f;
        
    // Normalize angles
    if (xRot > 360.0f) xRot -= 360.0f;
    if (xRot < 0.0f) xRot += 360.0f;
    if (yRot > 360.0f) yRot -= 360.0f;
    if (yRot < 0.0f) yRot += 360.0f;
    if (zRot > 360.0f) zRot -= 360.0f;
    if (zRot < 0.0f) zRot += 360.0f;
        
    glutPostRedisplay();
}

// Main function
int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(800, 600);
    glutCreateWindow("3D Bézier Curve with Coordinate Axes");
    
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
    glEnable(GL_DEPTH_TEST);
    
    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutSpecialFunc(specialKeys);
    
    glutMainLoop();
    return 0;
}

