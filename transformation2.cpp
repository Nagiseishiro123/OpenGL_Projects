#include <windows.h>
#include <math.h>
#include <gl\glut.h>
#include "tchar.h"
const int screenWidth = 700;
const int screenHeight = 700;

void init() {
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(-1.2, 1.2, -1.2, 1.2, 0.1, 100);
}
void setLight() {
    GLfloat	lightIntensity[] = { 0.7f, 0.7f, 0.7f, 1.0f };
    GLfloat light_position[] = { 10, 10, 20.0f, 0.0f };
    glLightfv(GL_LIGHT0, GL_POSITION, light_position);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, lightIntensity);

    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glShadeModel(GL_SMOOTH);
}
void setMaterial() {
    GLfloat	mat_ambient[] = { 0.7f, 0.7f, 0.7f, 1.0f };
    GLfloat	mat_diffuse[] = { 1.0f, 0.0f, 0.0f, 1.0f };
    GLfloat	mat_specular[] = { 1.0f, 1.0f, 1.0f, 1.0f };
    GLfloat mat_shininess[] = { 50.0f };

    glMaterialfv(GL_FRONT, GL_AMBIENT, mat_ambient);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);
}
void setCamera() {
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    gluLookAt(10, 6, 10, 0.0, 0.5, 0.0, 0.0, 1.0, 0.0);
}
void drawAxis() {
    float	xmax = 1.5, ymax = 1.5, zmax = 1.5;
    glColor3f(0.0, 0.0, 1.0);
    glBegin(GL_LINES);
    glVertex3f(-xmax, 0, 0);
    glVertex3f(xmax, 0, 0);
    glVertex3f(0, -ymax, 0);
    glVertex3f(0, ymax, 0);
    glVertex3f(0, 0, -zmax);
    glVertex3f(0, 0, zmax);
    glEnd();

    glColor3f(0.0, 0.0, 0.0);
    void* font = GLUT_BITMAP_TIMES_ROMAN_24;

    glRasterPos3f(1.5, 0, 0);
    glutBitmapCharacter(font, 'X');

    glRasterPos3f(0, 1.5, 0);
    glutBitmapCharacter(font, 'Y');

    glRasterPos3f(0, 0, 1.5);
    glutBitmapCharacter(font, 'Z');
}
void display() {
    glClearColor(1.0, 1.0, 1.0, 0.0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    setCamera();
    drawAxis();
    setLight();
    setMaterial();

    glPushMatrix();
    glutSolidCube(0.2);
    glPopMatrix();
    for (int i = 1; i < 5; i++) {
        float scale = 1;
        float radius = 0.1;
        float temp = 1;
        for (int j = 0; j < i; j++) {
            if (i == 1) {
                scale = 0.8;
                radius += 0.2 * 0.8 / 2;
                continue;
            }
            else if (j == i - 1) {
                scale = temp * 0.8;
                radius += 0.2 * scale / 2;
            }
            else {
                temp *= 0.8;
                radius += 0.2 * temp;
            }
        }
        glPushMatrix();
        glTranslatef(radius, 0, 0);
        if (i % 2 == 1) glRotatef(45, 1, 0, 0);
        glScalef(scale, scale, scale);
        glutSolidCube(0.2);
        glPopMatrix();

        glPushMatrix();
        glTranslatef(0, radius, 0);
        if (i % 2 == 1) glRotatef(45, 0, 1, 0);
        glScalef(scale, scale, scale);
        glutSolidCube(0.2);
        glPopMatrix();

        glPushMatrix();
        glTranslatef(0, 0, radius);
        if (i % 2 == 1) glRotatef(45, 0, 0, 1);
        glScalef(scale, scale, scale);
        glutSolidCube(0.2);
        glPopMatrix();

    }
    glFlush();
}
int main(int argc, char* argv[]) {
    glutInit(&argc, (char**)argv); //initialize the tool kit
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(screenWidth, screenHeight); //set window size
    glutInitWindowPosition(0, 0); // set window position on screen
    glutCreateWindow("Lab-Transformation"); // open the screen window

    glutDisplayFunc(display);

    init();
    glEnable(GL_DEPTH_TEST);

    glutMainLoop();
    return 0;
}
