#include<windows.h>
#include <GL/glut.h>
#include <stdlib.h>
#include <math.h>
#include<stdio.h>
#include<stdlib.h>
#include <iostream>
using namespace std;
/* Establece el tamaño inicial de la ventana de visualizacion. */
GLsizei winWidth=600, winHeight =600;

int x1=-1,y1=-1;
int coor=0;

class wcPt3D {
    public:
    GLfloat x, y, z;
};

GLint n_CtrlPts = 100, nBezCurvePts = 1000;
wcPt3D ctrlPts [100];

void init (void)
{
    /* Establece el color de la ventana de visualizacion en blanco. */
    glClearColor (1.0, 1.0, 1.0, 1.0);
    glMatrixMode(GL_PROJECTION);
    gluOrtho2D(0.0, 200.0, 0.0, 150.0);
}

void displayFcn (void)
{
    glClear (GL_COLOR_BUFFER_BIT); // Borra la ventana de visualización.
    glColor3f (1.0, 0.0, 0.0); // Establece el color de los puntos en rojo.
    glPointSize (3.0);
}

void winReshapeFcn (GLint newWidth, GLint newHeight)
{
    /* Mantiene una relación de aspeto de valor 1.0. */
    glViewport (0, 0, newHeight, newHeight);
    glMatrixMode (GL_PROJECTION);
    glLoadIdentity ( );
    gluOrtho2D (0.0 , GLdouble(newWidth),0.0 , GLdouble( newHeight));
}

void plotPoint (wcPt3D bezCurvePt)
{
    glBegin (GL_POINTS);
    glColor3f(1,0,0);
    glVertex2f (bezCurvePt.x, bezCurvePt.y);
    glEnd ( );
}
void plotPt2(GLint x, GLint y)
{
    glBegin(GL_POINTS);
    glColor3f(0,0,1);
    glVertex2i(x,y);
    glEnd();
}

/* Calcula los coeficientes binomiales C para un valor dado de n. */
void binomialCoeffs (GLint n, GLint * C)
{
    GLint k, j;
    for (k = 0; k <= n; k++) {
        /* Compute n!/(k!(n - k)!). */
        C [k] = 1;
        for (j = n; j >= k + 1; j--)
            C [k] *= j;
            for (j = n - k; j >= 2; j--)
                C [k] /= j;
    }
}

void computeBezPt (GLfloat u, wcPt3D * bezPt, GLint nCtrlPts,
wcPt3D * ctrlPts, GLint * C)
{
    GLint k, n = nCtrlPts - 1;
    GLfloat bezBlendFcn;
    bezPt->x = bezPt->y = bezPt->z = 0.0;
    /* Calcula las funciones de combinación y los puntos de control de
    combinación. */
    for (k = 0; k < nCtrlPts; k++) {
        bezBlendFcn = C [k] * pow (u, k) * pow (1 - u, n - k);
        bezPt->x += ctrlPts [k].x * bezBlendFcn;
        bezPt->y += ctrlPts [k].y * bezBlendFcn;
        bezPt->z += ctrlPts [k].z * bezBlendFcn;
        }
}

void bezier (wcPt3D * ctrlPts, GLint nCtrlPts, GLint nBezCurvePts)
{
    wcPt3D bezCurvePt;
    GLfloat u;
    GLint *C, k;
    /* Reserva espacio para los coeficientes binomiales */
    C = new GLint [nCtrlPts];
    binomialCoeffs (nCtrlPts - 1, C);
    for (k = 0; k <= nBezCurvePts; k++) {
        u = GLfloat (k) / GLfloat (nBezCurvePts);
        computeBezPt (u, &bezCurvePt, nCtrlPts, ctrlPts, C);
        plotPoint (bezCurvePt);
    }
    delete [ ] C;
}

void mousePtPlot(GLint button , GLint action , GLint xMouse, GLint yMouse)
{
    if(button==GLUT_LEFT_BUTTON && action==GLUT_DOWN) //GLUT_RIGHT_BUTTON
    {

        printf("obtiene coordenada: ");
        x1=xMouse;
        y1=winHeight-yMouse;
        ctrlPts[coor].x=x1;
        ctrlPts[coor].y=y1;
        ctrlPts[coor].z=0.0;
        plotPt2(x1,y1);
        cout<<ctrlPts[coor].x<<" - "<<ctrlPts[coor].y<<endl;
        coor++;
    }
    if(button==GLUT_RIGHT_BUTTON && action==GLUT_DOWN)
    {
        printf("ultima coordenada: ");
        x1=xMouse;
        y1=winHeight-yMouse;
        ctrlPts[coor].x=x1;
        ctrlPts[coor].y=y1;
        ctrlPts[coor].z=0;
        plotPt2(x1,y1);
        cout<<ctrlPts[coor].x<<" - "<<ctrlPts[coor].y<<endl;
        coor++;
        bezier (ctrlPts, coor, nBezCurvePts);
    }
    glFlush();

}


int main(int argc, char** argv)
{

    glutInit (&argc, argv);
    glutInitDisplayMode (GLUT_SINGLE | GLUT_RGB);
    glutInitWindowPosition (50, 50);
    glutInitWindowSize (winWidth, winHeight);
    glutCreateWindow ("Curva de Bezier");
    init ( );
    glutDisplayFunc (displayFcn);
    glutReshapeFunc (winReshapeFcn);
    glutMouseFunc(mousePtPlot);
    glutMainLoop ( );
    return 0;
}
