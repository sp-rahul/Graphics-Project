#include <iostream>
#include<GL/gl.h>
#include <GL/glut.h>
#include <math.h>
#include <stdlib.h>
#include<ctime>
#include<cstring>
#define ScreenWidth getmaxx()
#define ScreenHeight getmaxy()
#define GroundY ScreenHeight*0.75
#define DEG2RAD 3.14159/180.0

using namespace std;


float _moveA = 0.0f;
float _moveB = 0.0f;
float _moveC = 0.0f;
float _moveS = 0.0f;
float _angle1 = 0.0f;
float speed = 0.02f;

void fish5();
void fish4();
void fish3();
void rain();
void drawHair(float r, float x, float y);
void drawCircle(float r, float x, float y);
void drawEye(float r, float x, float y);
void drawMouth();
void drawBody();
void drawBody2();
void drawHumanMouth();
void drawHumanBody();
void drawHumanBody2();
void drawHumanBody3();
void drawHumanLeftLag();
void drawHumanLeftLag1();
void drawHumanRightLag();
void drawHumanRightLag1();
void drawHumanLeftHand();
void drawHumanRightHand();
void humanBody();
void DrawEllipse(float radiusX, float radiusY,int posx,int posy);
void drawBag();
enum DIRECTION {LEFT=0,RIGHT,TOP,BOTTOM};

float cloudSpeed[4]= {-12,38,14,3};

void createSky()
{
    glBegin(GL_POLYGON);// Draw a Red 1x1 Square centered at origin
    glColor3ub(135,206,250);
    glVertex2f(38.0f,3.0f);
    glVertex2f(38.0f,14.0f);
    glVertex2f(-12.0f,14.0f);
    glVertex2f(-12.0f,3.0f);

    glEnd();
}

void createSun()
{
    glPushMatrix();
    glColor3d(255,255,0);
    glTranslatef(23.0,_moveS, 0);
    glutSolidSphere(1.0, 250, 250);
    glPopMatrix();
}

void updateSunVerticalPosition(int value)
{

    if(_moveS > 15)
    {
        _moveS = 0;
    }
    //Notify GLUT that the display has changed

    _moveS += 0.150;

    glutTimerFunc(20, updateSunVerticalPosition, 0); //Notify GLUT to call update again in 25 milliseconds
    glutPostRedisplay();
}

void createMoon()
{
    glPushMatrix();
    glColor3ub(217,217,217);
    glTranslatef(-8.0,_moveS, 0);
    glutSolidSphere(1, 250, 250);
    glPopMatrix();

    //2nd circle for moon

    glPushMatrix();
    glColor3ub(0,0,0);
    glTranslatef(-8,_moveS+0.25, 0);
    glutSolidSphere(1, 250, 250);
    glPopMatrix();
}


int numberOfCloud;

typedef struct
{
    float translateX[10],translateY[10];
    int _size;
    int direction;
    float shapeSize;
} CloudShape;

CloudShape cloudShape[5];

void initCloud()
{
    srand(time(0));
    numberOfCloud = (rand()%3) + 3;
    float x1[]= {1.0,1.0,2.0,2.0,0.0,3.0};
    float y1[]= {6.0,7.0,7.0,6.0,6.5,6.5};

    memcpy(cloudShape[0].translateX, x1,sizeof(x1));
    memcpy(cloudShape[0].translateY, y1,sizeof(y1));
    cloudShape[0]._size = 6;
    cloudShape[0].direction = rand()%3;
    cloudShape[0].shapeSize = ((rand()%4)+7)/10.0;

    float x2[]= {1.0,1.0,2.0,2.0,0.0,3.0};
    float y2[]= {9.0,10.0,10.0,9.0,9.5,9.5};
    memcpy(cloudShape[1].translateX, x2,sizeof(x2));
    memcpy(cloudShape[1].translateY, y2,sizeof(y2));
    cloudShape[1]._size = 6;
    cloudShape[1].direction = rand()%3;
    cloudShape[1].shapeSize = ((rand()%4)+7)/10.0;

    float x3[]= {1.0,1.0,2.0,2.0,0.0,3.0};
    float y3[]= {11.0,12.0,12.0,11.0,11.5,11.5};
    memcpy(cloudShape[2].translateX, x3,sizeof(x3));
    memcpy(cloudShape[2].translateY, y3,sizeof(y3));
    cloudShape[2]._size = 6;
    cloudShape[2].direction = rand()%3;
    cloudShape[2].shapeSize = ((rand()%4)+7)/10.0;

    float x4[]= {7.0,7.0,8.0,8.0,6.0,9.0};
    float y4[]= {6.0,7.0,7.0,6.0,6.5,6.5};

    memcpy(cloudShape[3].translateX, x4,sizeof(x4));
    memcpy(cloudShape[3].translateY, y4,sizeof(y4));
    cloudShape[3]._size = 6;
    cloudShape[3].direction = rand()%3;
    cloudShape[3].shapeSize = ((rand()%4)+7)/10.0;

    float x5[]= {10.0,10.0,11.0,11.0,9.0,12.0};
    float y5[]= {6.0,7.0,7.0,6.0,6.5,6.5};
    memcpy(cloudShape[4].translateX, x5,sizeof(x5));
    memcpy(cloudShape[4].translateY, y5,sizeof(y5));
    cloudShape[4]._size = 6;
    cloudShape[4].direction = rand()%3;
    cloudShape[4].shapeSize = ((rand()%4)+7)/10.0;


}
void createCloud(CloudShape cs)
{
    glPushMatrix();
    if(cs.direction==LEFT || cs.direction==RIGHT)
    {
        glTranslatef(cloudSpeed[cs.direction],0.0f,0.0f);
    }
    else
    {
        glTranslatef(0.0f,cloudSpeed[cs.direction],0.0f);
    }

    for(int i=0; i<cs._size; i++)
    {
        glPushMatrix();
        glColor3d(255,255,255);
        glTranslatef(cs.translateX[i],cs.translateY[i], 0);
        glutSolidSphere(cs.shapeSize, 250, 250);
        glPopMatrix();
    }

    glPopMatrix();
}

void updateCloudPosition(int value)
{
    if(cloudSpeed[LEFT] > +36)
    {
        cloudSpeed[LEFT] = -20;
    }
    cloudSpeed[LEFT] += 0.13;

    if(cloudSpeed[RIGHT] < -20)
    {
        cloudSpeed[RIGHT] = +36;
    }
    cloudSpeed[RIGHT] -= 0.13;

    if(cloudSpeed[TOP] <-5)
    {
        cloudSpeed[TOP] = +20;
    }
    cloudSpeed[TOP] -= 0.13;

    glutTimerFunc(20, updateCloudPosition, 0); //Notify GLUT to call update again in 25 milliseconds
    glutPostRedisplay();
}

void drawScene() {
    glClearColor(1.0f,1.0f,1.0f,1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    glColor3d(1,0,0);
    glLoadIdentity(); //Reset the drawing perspective
    gluOrtho2D(-12,38,-19,14);//range
    glMatrixMode(GL_MODELVIEW);


    createSky();

    ///sun///

    createSun();
    ///cloud 1///

    for(int i=0; i<numberOfCloud; i++)
    {
        createCloud(cloudShape[i]);
    }


  ///sea portion///
    glBegin(GL_POLYGON);// Draw a Red 1x1 Square centered at origin
    glColor3ub(65,105,225);
    glVertex2f(38.0f,-19.0f);
    glVertex2f(38.0f,3.0f);
    glVertex2f(-12.0f,3.0f);
    glVertex2f(-12.0f,-19.0f);

    glEnd();

///fish

    fish5();
    fish4();
    fish3();

///left soil portion///
//polygon 1
    glBegin(GL_POLYGON);// Draw a Red 1x1 Square centered at origin
    glColor3ub(0,128,0);
    glVertex2f(4.0f,0.0f);
    glVertex2f(6.0f,1.0f);
    glVertex2f(3.0f,3.0f);
    glVertex2f(-12.0f,3.0f);
    glVertex2f(-12.0f,0.0f);

    glEnd();

//polygon 2
    glBegin(GL_POLYGON);// Draw a Red 1x1 Square centered at origin
    glColor3ub(0,128,0);
    glVertex2f(5.0f,-3.0f);
    glVertex2f(7.0f,-1.5f);
    glVertex2f(4.0f,0.0f);
    glVertex2f(-12.0f,0.0f);
    glVertex2f(-12.0f,-3.0f);

    glEnd();

//polygon 3
    glBegin(GL_POLYGON);// Draw a Red 1x1 Square centered at origin
    glColor3ub(0,128,0);
    glVertex2f(6.0f,-4.5f);
    glVertex2f(8.0f,-4.0f);
    glVertex2f(5.0f,-3.0f);
    glVertex2f(-12.0f,-3.0f);
    glVertex2f(-12.0f,-4.5f);

    glEnd();

//polygon 4
    glBegin(GL_POLYGON);// Draw a Red 1x1 Square centered at origin
    glColor3ub(0,128,0);
    glVertex2f(2.0f,-9.0f);
    glVertex2f(9.5f,-7.0f);
    glVertex2f(6.0f,-4.5f);
    glVertex2f(-12.0f,-4.5f);
    glVertex2f(-12.0f,-9.5f);

    glEnd();

//polygon 5
    glBegin(GL_POLYGON);// Draw a Red 1x1 Square centered at origin
    glColor3ub(0,128,0);
    glVertex2f(2.0f,-11.5f);
    glVertex2f(5.0f,-10.0f);
    glVertex2f(2.0f,-9.0f);
    glVertex2f(-12.0f,-9.0f);
    glVertex2f(-12.0f,-11.5f);

    glEnd();

///left sidewise border partition///
 //polygon 1
    glBegin(GL_POLYGON);// Draw a Red 1x1 Square centered at origin
    glColor3ub(0,0,0);
    glVertex2f(6.0f,0.5f);
    glVertex2f(6.0f,1.0f);
    glVertex2f(4.0f,0.0f);
    glVertex2f(4.5f,-0.25f);

    glEnd();

 //polygon 2
    glBegin(GL_POLYGON);// Draw a Red 1x1 Square centered at origin
    glColor3ub(0,0,0);
    glVertex2f(7.0f,-2.0f);
    glVertex2f(7.0f,-1.5f);
    glVertex2f(5.0f,-3.0f);
    glVertex2f(5.5f,-3.15f);

    glEnd();

//polygon 3
    glBegin(GL_POLYGON);// Draw a Red 1x1 Square centered at origin
    glColor3ub(0,0,0);
    glVertex2f(8.0f,-4.4f);
    glVertex2f(8.0f,-4.0f);
    glVertex2f(6.0f,-4.5f);
    glVertex2f(6.3f,-4.8f);

    glEnd();

//polygon 4
    glBegin(GL_POLYGON);// Draw a Red 1x1 Square centered at origin
    glColor3ub(0,0,0);
    glVertex2f(9.5f,-7.5f);
    glVertex2f(9.5f,-7.0f);
    glVertex2f(2.0f,-9.0f);
    glVertex2f(3.0f,-9.3f);

    glEnd();

//polygon 5
    glBegin(GL_POLYGON);// Draw a Red 1x1 Square centered at origin
    glColor3ub(0,0,0);
    glVertex2f(5.0f,-10.5f);
    glVertex2f(5.0f,-10.0f);
    glVertex2f(2.0f,-11.5f);
    glVertex2f(2.0f,-12.0f);

    glEnd();

//polygon 6
    glBegin(GL_POLYGON);// Draw a Red 1x1 Square centered at origin
    glColor3ub(0,0,0);
    glVertex2f(2.0f,-12.0f);
    glVertex2f(2.0f,-11.5f);
    glVertex2f(-12.0f,-11.5f);
    glVertex2f(-12.0f,-12.0f);

    glEnd();





///straw///
//polygon 1
    glBegin(GL_POLYGON);// Draw a Red 1x1 Square centered at origin
    glColor3ub(189,183,107);
    glVertex2f(2.5f,-1.8f);
    glVertex2f(2.0f,0.2f);
    glVertex2f(-3.0f,0.2f);
    glVertex2f(-3.5f,-1.8f);

    glEnd();

 //polygon 2
    glBegin(GL_POLYGON);// Draw a Red 1x1 Square centered at origin
    glColor3ub(189,183,107);
    glVertex2f(2.0f,0.2f);
    glVertex2f(1.0f,1.7f);
    glVertex2f(-2.0f,1.7f);
    glVertex2f(-3.0f,0.2f);

    glEnd();
//polygon 3
    glBegin(GL_POLYGON);// Draw a Red 1x1 Square centered at origin
    glColor3ub(189,183,107);
    glVertex2f(1.0f,1.7f);
    glVertex2f(0.0f,2.2f);
    glVertex2f(-1.0f,2.2f);
    glVertex2f(-2.0f,1.7f);

    glEnd();

///triangle///

    glBegin(GL_POLYGON);// Draw a Red 1x1 Square centered at origin
    glColor3ub(189,183,107);
    glVertex2f(0.0f,2.2f);
    glVertex2f(-0.5f,2.4f);
    glVertex2f(-1.0f,2.2f);

    glEnd();

//polygon

    glBegin(GL_POLYGON);// Draw a Red 1x1 Square centered at origin
    glColor3ub(189,183,107);
    glVertex2f(-0.4f,2.3f);
    glVertex2f(-0.4f,2.7f);
    glVertex2f(-0.6f,2.7f);
    glVertex2f(-0.6f,2.3f);

    glEnd();


///house on left side///
//polygon 1
    glBegin(GL_POLYGON);// Draw a Red 1x1 Square centered at origin
    glColor3ub(139,69,19);
    glVertex2f(-2.7f,2.2f);
    glVertex2f(-6.7f,2.2f);
    glVertex2f(-5.0f,0.0f);
    glVertex2f(-1.0f,0.0f);

    glEnd();


//polygon 2
    glBegin(GL_POLYGON);// Draw a Red 1x1 Square centered at origin
    glColor3ub(139,69,19);
    glVertex2f(-6.5f,2.0f);
    glVertex2f(-6.7f,2.2f);
    glVertex2f(-8.5f,0.0f);
    glVertex2f(-8.0f,0.0f);

    glEnd();

//polygon 3
    glBegin(GL_POLYGON);// Draw a Red 1x1 Square centered at origin
    glColor3ub(205,133,63);
    glVertex2f(-6.5f,2.0f);
    glVertex2f(-8.0f,0.0f);
    glVertex2f(-8.0f,-1.5f);
    glVertex2f(-5.0f,-2.0f);
    glVertex2f(-5.0f,0.0f);

    glEnd();

//polygon 4
    glBegin(GL_POLYGON);// Draw a Red 1x1 Square centered at origin
    glColor3ub(128,135,32);
    glVertex2f(-6.0f,-1.0f);
    glVertex2f(-6.0f,0.0f);
    glVertex2f(-7.0f,0.0f);
    glVertex2f(-7.0f,-1.0f);

    glEnd();

//polygon 5
    glBegin(GL_POLYGON);// Draw a Red 1x1 Square centered at origin
    glColor3ub(139,69,19);
    glVertex2f(-5.0f,-2.0f);
    glVertex2f(-8.0f,-1.5f);
    glVertex2f(-8.3f,-1.9f);
    glVertex2f(-5.0f,-2.5f);

    glEnd();

//polygon 6
    glBegin(GL_POLYGON);// Draw a Red 1x1 Square centered at origin
    glColor3ub(184,134,11);
    glVertex2f(-1.5f,-2.0f);
    glVertex2f(-1.5f,0.0f);
    glVertex2f(-5.0f,0.0f);
    glVertex2f(-5.0f,-2.0f);

    glEnd();

//polygon 7
    glBegin(GL_POLYGON);// Draw a Red 1x1 Square centered at origin
    glColor3ub(139,69,19);
    glVertex2f(-5.0f,-2.0f);
    glVertex2f(-5.0f,-2.5f);
    glVertex2f(-1.0f,-2.5f);
    glVertex2f(-1.5f,-2.0f);

    glEnd();


///door:left
//polygon a
    glBegin(GL_POLYGON);// Draw a Red 1x1 Square centered at origin
    glColor3ub(0,0,0);
    glVertex2f(-2.5f,-2.0f);
    glVertex2f(-2.5f,-0.2f);
    glVertex2f(-4.0f,-0.2f);
    glVertex2f(-4.0f,-2.0f);

    glEnd();

//polygon b
    glBegin(GL_POLYGON);// Draw a Red 1x1 Square centered at origin
    glColor3ub(128,135,32);
    glVertex2f(-3.3f,-2.0f);
    glVertex2f(-3.3f,-0.4f);
    glVertex2f(-4.0f,-0.2f);
    glVertex2f(-4.0f,-2.0f);

    glEnd();

//polygon c
    glBegin(GL_POLYGON);// Draw a Red 1x1 Square centered at origin
    glColor3ub(128,135,32);
    glVertex2f(-2.5f,-2.0f);
    glVertex2f(-2.5f,-0.2f);
    glVertex2f(-3.2f,-0.4f);
    glVertex2f(-3.2f,-2.0f);

    glEnd();



///tree left side
//polygon 1
    glBegin(GL_POLYGON);
    glColor3ub(139,69,19);
    glVertex2f(-9.5,1);
    glVertex2f(-9,1.5);
    glVertex2f(-9.5,2.5);
    glVertex2f(-10,2);
    glEnd();

//polygon 2
    glBegin(GL_POLYGON);
    glColor3ub(139,69,19);
    glVertex2f(-9,1.5);
    glVertex2f(-8.5,1);
    glVertex2f(-8,2);
    glVertex2f(-8,2.5);
    glEnd();

//polygon 3
    glBegin(GL_POLYGON);
    glColor3ub(139,69,19);
    glVertex2f(-9.5,1);
    glVertex2f(-9.5,-3.5);
    glVertex2f(-9,-4);
    glVertex2f(-8.5,-3.5);
    glVertex2f(-8.5,1);
    glEnd();

//polygon 4
    glBegin(GL_POLYGON);
    glColor3ub(139,69,19);
    glVertex2f(-8.5,-3);
    glVertex2f(-9.5,-3);
    glVertex2f(-10.5,-3.5);
    glVertex2f(-7.5,-3.5);
    glEnd();



//triangle 1
    glBegin(GL_TRIANGLES);
    glColor3ub(139,69,19);
    glVertex2f(-10,-0.5);
    glVertex2f(-9.5,-1);
    glVertex2f(-9.5,-0.6);
    glEnd();

//triangle 2
    glBegin(GL_TRIANGLES);
    glColor3ub(139,69,19);
    glVertex2f(-9.5,1);
    glVertex2f(-9,1.5);
    glVertex2f(-8.5,1);
    glEnd();

///shop

///Polygon 1
    glBegin(GL_POLYGON);// Draw a Red 1x1 Square centered at origin
    glColor3ub(0,0,139);
    glVertex2f(-2.7f,-3.2f);
    glVertex2f(-6.7f,-3.2f);
    glVertex2f(-5.3f,-5.0f);
    glVertex2f(-1.1f,-5.0f);

    glEnd();

///Polygon 2
    glBegin(GL_POLYGON);// Draw a Red 1x1 Square centered at origin
    glColor3ub(205,133,63);
    glVertex2f(-6.7f,-3.2f);
    glVertex2f(-6.6f,-7.0f);
    glVertex2f(-5.0f,-8.0f);
    glVertex2f(-5.0f,-5.0f);

    glEnd();


///Polygon 3
    glBegin(GL_POLYGON);// Draw a Red 1x1 Square centered at origin
    glColor3ub(184,134,11);
    glVertex2f(-1.5f,-8.0f);
    glVertex2f(-1.5f,-5.0f);
    glVertex2f(-5.0f,-5.0f);
    glVertex2f(-5.0f,-8.0f);

    glEnd();


//polygon 4
    glBegin(GL_POLYGON);// Draw a Red 1x1 Square centered at origin
    glColor3ub(128,135,32);
    glVertex2f(-2.5f,-7.5f);
    glVertex2f(-2.5f,-5.5f);
    glVertex2f(-4.5f,-5.5f);
    glVertex2f(-4.5f,-7.5f);

    glEnd();

///Store keeper
    drawHair(.6,-3.5,-6.1);
    drawCircle(.5,-3.5,-6.2);
    drawEye(.15,-3.7,-6.1);
    drawEye(.15,-3.25,-6.1);
    drawMouth();
    drawBody();
    drawBody2();

///Human
    drawHair(1,2.5,-1.5);
    drawCircle(.9,2.5,-1.7);
    drawEye(.25,2.05,-1.45);
    drawEye(.25,2.85,-1.45);
    drawHumanMouth();
    drawHumanBody();
    drawHumanBody2();
    drawHumanBody3();
    drawHumanLeftLag();
    drawHumanLeftLag1();
    drawHumanRightLag();
    drawHumanRightLag1();
    drawHumanLeftHand();
    drawHumanRightHand();
    drawBag();

///right soil portion///
//polygon1//
    glBegin(GL_POLYGON);

    glColor3ub(0,128,0);
    glVertex2f(13.0f,3.0f);
    glVertex2f(16.0f,1.0f);
    glVertex2f(38.0f,1.0f);
    glVertex2f(38.0f,3.0f);

    glEnd();

//polygon2//
    glBegin(GL_POLYGON);

    glColor3ub(0,128,0);
    glVertex2f(16,1);
    glVertex2f(14,0);
    glVertex2f(17,-1.5);
    glVertex2f(38,-1.5);
    glVertex2f(38,1);

    glEnd();

//polygon3//
    glBegin(GL_POLYGON);

    glColor3ub(0,128,0);
    glVertex2f(17,-1.5);
    glVertex2f(15,-3);
    glVertex2f(18,-4);
    glVertex2f(38,-4);
    glVertex2f(38,-1.5);

    glEnd();

//polygon4//
    glBegin(GL_POLYGON);

    glColor3ub(0,128,0);
    glVertex2f(18,-4);
    glVertex2f(16,-4.5);
    glVertex2f(21,-7);
    glVertex2f(38,-7);
    glVertex2f(38,-4);

    glEnd();

//polygon5//
    glBegin(GL_POLYGON);

    glColor3ub(0,128,0);
    glVertex2f(21,-7);
    glVertex2f(19,-8);
    glVertex2f(22,-9);
    glVertex2f(38,-9);
    glVertex2f(38,-7);

    glEnd();

///right sidewise border partition///

//polygon1//
    glBegin(GL_POLYGON);

    glColor3ub(0,0,0);
    glVertex2f(12.5,3);
    glVertex2f(15.6,0.8);
    glVertex2f(16,1);
    glVertex2f(13,3);

    glEnd();

//polygon2//
    glBegin(GL_POLYGON);

    glColor3ub(0,0,0);
    glVertex2f(14,-0.5);
    glVertex2f(16.4,-1.9);
    glVertex2f(17,-1.5);
    glVertex2f(14,0);

    glEnd();

//polygon3//
    glBegin(GL_POLYGON);

    glColor3ub(0,0,0);
    glVertex2f(15,-3.5);
    glVertex2f(17,-4.2);
    glVertex2f(18,-4);
    glVertex2f(15,-3);

    glEnd();

//polygon4//
    glBegin(GL_POLYGON);

    glColor3ub(0,0,0);
    glVertex2f(16,-5);
    glVertex2f(20.5,-7.2);
    glVertex2f(21,-7);
    glVertex2f(16,-4.5);

    glEnd();

//polygon5//
    glBegin(GL_POLYGON);

    glColor3ub(0,0,0);
    glVertex2f(19,-8.5);
    glVertex2f(22,-9.5);
    glVertex2f(22,-9);
    glVertex2f(19,-8);

    glEnd();

//polygon6//
    glBegin(GL_POLYGON);

    glColor3ub(0,0,0);
    glVertex2f(38,-9);
    glVertex2f(22,-9);
    glVertex2f(22,-9.5);
    glVertex2f(38,-9.5);

    glEnd();

///house on right side///

//polygon1//
    glBegin(GL_POLYGON);

    //glColor3ub(25,25,112);
    glColor3ub(47,79,79);
    glVertex2f(25.3,1.2);
    glVertex2f(21.3,1.2);
    glVertex2f(23,-1);
    glVertex2f(27,-1);

    glEnd();

//polygon2//
    glBegin(GL_POLYGON);
      glColor3ub(47,79,79);
    //glColor3ub(139,69,19);
    glVertex2f(21.5,1);
    glVertex2f(21.3,1.2);
    glVertex2f(19.5,-1);
    glVertex2f(20,-1);

    glEnd();

//polygon3//
    glBegin(GL_POLYGON);

    glColor3ub(184,134,11);
    glVertex2f(21.5,1);
    glVertex2f(20,-1);
    glVertex2f(20,-2.5);
    glVertex2f(23,-3);
    glVertex2f(23,-1);

    glEnd();

//polygon4//
   glBegin(GL_POLYGON);

    glColor3ub(160,82,45);
    glVertex2f(22,-2);
    glVertex2f(22,-1);
   glVertex2f(21,-1);
    glVertex2f(21,-2);

    glEnd();

//polygon5//
    glBegin(GL_POLYGON);
     glColor3ub(47,79,79);
    //glColor3ub(139,69,19);
    glVertex2f(23,-3);
    glVertex2f(20,-2.5);
    glVertex2f(19.7,-2.9);
    glVertex2f(23,-3.5);

    glEnd();

//polygon6//
   glBegin(GL_POLYGON);

   glColor3ub(205,133,63);
    glVertex2f(26.5,-3);
    glVertex2f(26.5,-1);
    glVertex2f(23,-1);
    glVertex2f(23,-3);

    glEnd();

//polygon7//
    glBegin(GL_POLYGON);
     glColor3ub(47,79,79);
    //glColor3ub(139,69,19);
    glVertex2f(23,-3);
    glVertex2f(23,-3.5);
    glVertex2f(27,-3.5);
    glVertex2f(26.5,-3);

    glEnd();

///door:right///
//polygon a//
    glBegin(GL_POLYGON);

    glColor3ub(0,0,0);
    glVertex2f(25.5,-3);
    glVertex2f(25.5,-1.2);
    glVertex2f(24,-1.2);
    glVertex2f(24,-3);

    glEnd();

//polygon b//
    glBegin(GL_POLYGON);

    glColor3ub(160,82,45);
    glVertex2f(24.7,-3);
    glVertex2f(24.7,-1.4);
    glVertex2f(24,-1.2);
    glVertex2f(24,-3);

    glEnd();

//polygon c//
    glBegin(GL_POLYGON);

    glColor3ub(160,82,45);
    glVertex2f(25.5,-3);
    glVertex2f(25.5,-1.2);
    glVertex2f(24.8,-1.4);
    glVertex2f(24.8,-3);

    glEnd();

///hillview///
///hill-1///
//polygon1//
    glBegin(GL_POLYGON);
    glColor3ub(0,100,0);
    glVertex2f(-12,3);
    glVertex2f(-11.5,4.5);
    glVertex2f(-7.5,4.5);
    glVertex2f(-7,3);
    //glVertex2f();
    glEnd();

//polygon2//
    glBegin(GL_POLYGON);
    glColor3ub(0,100,0);
    glVertex2f(-11.5,4.5);
    glVertex2f(-11,5.5);
    glVertex2f(-8,5.5);
    glVertex2f(-7.5,4.5);
    //glVertex2f();
    glEnd();

//polygon3//
    glBegin(GL_POLYGON);
    glColor3ub(0,100,0);
    glVertex2f(-11,5.5);
    glVertex2f(-10.5,6);
    glVertex2f(-8.5,6);
    glVertex2f(-8,5.5);
    //glVertex2f();
    glEnd();

//polygon4//
    glBegin(GL_POLYGON);
    glColor3ub(0,100,0);
    glVertex2f(-10.5,6);
    glVertex2f(-10.3,6.2);
    glVertex2f(-8.7,6.2);
    glVertex2f(-8.5,6);
    //glVertex2f();
    glEnd();

//polygon5//
    glBegin(GL_POLYGON);
    glColor3ub(0,100,0);
    glVertex2f(-10.3,6.2);
    glVertex2f(-10.2,6.3);
    glVertex2f(-8.8,6.3);
    glVertex2f(-8.7,6);
    //glVertex2f();
    glEnd();

///Hill 2///
//polygon1//
    glBegin(GL_POLYGON);
    glColor3ub(0,100,0);
    glVertex2f(-7,3);
    glVertex2f(-6.5,4);
    glVertex2f(-4.5,4);
    glVertex2f(-4,3);
    //glVertex2f();
    glEnd();

//polygon2//
    glBegin(GL_POLYGON);
    glColor3ub(0,100,0);
    glVertex2f(-6.5,4);
    glVertex2f(-6,4.5);
    glVertex2f(-5,4.5);
    glVertex2f(-4.5,4);
    //glVertex2f();
    glEnd();

///Hill 3///
//polygon1//
    glBegin(GL_POLYGON);
    glColor3ub(0,100,0);
    glVertex2f(-4,3);
    glVertex2f(-3.5,4.5);
    glVertex2f(-1.5,4.5);
    glVertex2f(-1,3);
    //glVertex2f();
    glEnd();

//polygon2//
    glBegin(GL_POLYGON);
    glColor3ub(0,100,0);
    glVertex2f(-3.5,4.5);
    glVertex2f(-3,5);
    glVertex2f(-2,5);
    glVertex2f(-1.5,4.5);
    //glVertex2f();
    glEnd();

///Hill 4///
//polygon1//
    glBegin(GL_POLYGON);
    glColor3ub(0,100,0);
    glVertex2f(-1,3);
    glVertex2f(-0.5,6);
    glVertex2f(4.5,6);
    glVertex2f(5,3);
    //glVertex2f();
    glEnd();

//polygon2//
    glBegin(GL_POLYGON);
    glColor3ub(0,100,0);
    glVertex2f(-0.5,6);
    glVertex2f(0,7);
    glVertex2f(4,7);
    glVertex2f(4.5,6);
    //glVertex2f();
    glEnd();

//polygon3//
    glBegin(GL_POLYGON);
    glColor3ub(0,100,0);
    glVertex2f(0,7);
    glVertex2f(1,8);
    glVertex2f(3,8);
    glVertex2f(4,7);
    //glVertex2f();
    glEnd();
//triangle//
    glBegin(GL_TRIANGLES);
    glColor3ub(0,100,0);
    glVertex2f(1,8);
    glVertex2f(3,8);
    glVertex2f(2,8.3);
    glEnd();

///hill 5///
//polygon1//
    glBegin(GL_POLYGON);
    glColor3ub(0,100,0);
    glVertex2f(5,3);
    glVertex2f(5.5,4);
    glVertex2f(6.5,4);
    glVertex2f(7,3);
    //glVertex2f();
    glEnd();

//triangle//
    glBegin(GL_TRIANGLES);
    glColor3ub(0,100,0);
    glVertex2f(5.5,4);
    glVertex2f(6.5,4);
    glVertex2f(6,4.5);
    glEnd();

///hill 6///
//polygon1//
    glBegin(GL_POLYGON);
    glColor3ub(0,100,0);
    glVertex2f(7,3);
    glVertex2f(8,5);
    glVertex2f(13,5);
    glVertex2f(14,3);
    //glVertex2f();
    glEnd();

//polygon2//
    glBegin(GL_POLYGON);
    glColor3ub(0,100,0);
    glVertex2f(8,5);
    glVertex2f(9,6);
    glVertex2f(12,6);
    glVertex2f(13,5);
    //glVertex2f();
    glEnd();
//triangle//
    glBegin(GL_TRIANGLES);
    glColor3ub(0,100,0);
    glVertex2f(9,6);
    glVertex2f(12,6);
    glVertex2f(10.5,6.5);
    glEnd();

///hill 7///
//polygon1//
    glBegin(GL_POLYGON);
    glColor3ub(0,100,0);
    glVertex2f(14,3);
    glVertex2f(15,5);
    glVertex2f(20,5);
    glVertex2f(21,3);
    //glVertex2f();
    glEnd();

//polygon2//
    glBegin(GL_POLYGON);
    glColor3ub(0,100,0);
    glVertex2f(15,5);
    glVertex2f(16,6);
    glVertex2f(19,6);
    glVertex2f(20,5);
    //glVertex2f();
    glEnd();

//triangle//
    glBegin(GL_TRIANGLES);
    glColor3ub(0,100,0);
    glVertex2f(16,6);
    glVertex2f(19,6);
    glVertex2f(17.5,6.5);
    glEnd();

///hill 8///
//polygon1//
    glBegin(GL_POLYGON);
    glColor3ub(0,100,0);
    glVertex2f(21,3);
    glVertex2f(22,5);
    glVertex2f(24,5);
    glVertex2f(25,3);
    //glVertex2f();
    glEnd();

//polygon2//
    glBegin(GL_POLYGON);
    glColor3ub(0,100,0);
    glVertex2f(22,5);
    glVertex2f(22.5,5.5);
    glVertex2f(23.5,5.5);
    glVertex2f(24,5);
    //glVertex2f();
    glEnd();

///hill 9///
//polygon1//
    glBegin(GL_POLYGON);
    glColor3ub(0,100,0);
    glVertex2f(25,3);
    glVertex2f(25.5,4.5);
    glVertex2f(28.5,4.5);
    glVertex2f(29,3);
    //glVertex2f();
    glEnd();

//polygon2//
    glBegin(GL_POLYGON);
    glColor3ub(0,100,0);
    glVertex2f(25.5,4.5);
    glVertex2f(26,5);
    glVertex2f(28,5);
    glVertex2f(28.5,4.5);
    //glVertex2f();
    glEnd();

///hill 10///
//polygon4//
    glBegin(GL_POLYGON);
    glColor3ub(0,100,0);
    glVertex2f(29,3);
    glVertex2f(29.5,4);
    glVertex2f(30.5,4);
    glVertex2f(31,3);
    //glVertex2f();
    glEnd();

//triangle//
    glBegin(GL_TRIANGLES);
    glColor3ub(0,100,0);
    glVertex2f(29.5,4);
    glVertex2f(30,4.5);
    glVertex2f(30.5,4);
    glEnd();

///hill 11///
//polygon1//
    glBegin(GL_POLYGON);
    glColor3ub(0,100,0);
    glVertex2f(31,3);
    glVertex2f(31.5,4.5);
    glVertex2f(32.5,4.5);
    glVertex2f(33,3);
    //glVertex2f();
    glEnd();

//triangle//
    glBegin(GL_TRIANGLES);
    glColor3ub(0,100,0);
    glVertex2f(31.5,4.5);
    glVertex2f(32.5,4.5);
    glVertex2f(32,5);
    glEnd();

///hill 12///
//polygon1//
    glBegin(GL_POLYGON);
    glColor3ub(0,100,0);
    glVertex2f(33,3);
    glVertex2f(33.5,4.5);
    glVertex2f(37.5,4.5);
    glVertex2f(38,3);
    //glVertex2f();
    glEnd();

//polygon2//
    glBegin(GL_POLYGON);
    glColor3ub(0,100,0);
    glVertex2f(33.5,4.5);
    glVertex2f(34,5);
    glVertex2f(37,5);
    glVertex2f(37.5,4.5);
    //glVertex2f();
    glEnd();

//triangle//
    glBegin(GL_TRIANGLES);
    glColor3ub(0,100,0);
    glVertex2f(34,5);
    glVertex2f(37,5);
    glVertex2f(35.5,5.5);
    glEnd();

///tree upper side///

    glPushMatrix();
    glColor3ub(50,205,50);
    glTranslatef(-10.5, 2.5, 0);
    glutSolidSphere(1.0, 150, 150);
    glPopMatrix();

    glPushMatrix();
    glColor3ub(50,205,50);
    glTranslatef(-9.5, 3.0, 0);
    glutSolidSphere(1.0, 150, 150);
    glPopMatrix();

    glPushMatrix();
    glColor3ub(50,205,50);
    glTranslatef(-7.5, 1.5, 0);
    glutSolidSphere(1.0, 150, 150);
    glPopMatrix();

    glPushMatrix();
    glColor3ub(50,205,50);
    glTranslatef(-7.5, 2.5, 0);
    glutSolidSphere(1.0, 150, 150);
    glPopMatrix();

    glPushMatrix();
    glColor3ub(50,205,50);
    glTranslatef(-8.5, 3.5, 0);
    glutSolidSphere(1.0, 150, 150);
    glPopMatrix();

    glPushMatrix();
    glColor3ub(50,205,50);
    glTranslatef(-10.0, 4.0, 0);
    glutSolidSphere(1.0, 150, 150);
    glPopMatrix();

    glPushMatrix();
    glColor3ub(50,205,50);
    glTranslatef(-8.0, 4.0, 0);
    glutSolidSphere(1.0, 150, 150);
    glPopMatrix();

    glPushMatrix();
    glColor3ub(50,205,50);
    glTranslatef(-9.0, 5.0, 0);
    glutSolidSphere(1.0, 150, 150);
    glPopMatrix();







///boat-1 motion left to right
    glPushMatrix();
    //glColor3d(1,0,0);
    glTranslatef(_moveA, 0.0f, 0.0f);
    glBegin(GL_QUADS);
    glColor3ub(139,69,19);
        glVertex2f(1.0f, -14.0f);
        glVertex2f(1.50f, -13.0f);
        glVertex2f(-2.0f, -13.0f);
        glVertex2f(-3.0f, -14.0f);
    glEnd();

    glBegin(GL_QUADS);
    glColor3ub(0,0,0);
        glVertex2f(2.0f, -14.5f);
        glVertex2f(2.0f, -14.0f);
        glVertex2f(-3.0f, -14.0f);
        glVertex2f(-3.0f, -14.5f);
    glEnd();

    glBegin(GL_TRIANGLES);
    glColor3ub(0,0,0);
        glVertex2f(-3.0f, -14.5f);
        glVertex2f(-3.0f, -14.0f);
        glVertex2f(-4.5f, -13.7f);

    glEnd();

    glBegin(GL_TRIANGLES);
    glColor3ub(0,0,0);
        glVertex2f(2.0f, -14.5f);
        glVertex2f(3.5f, -13.7f);
        glVertex2f(2.0f, -14.0f);

    glEnd();

    glBegin(GL_TRIANGLES);
    glColor3ub(0,0,0);
        glVertex2f(2.0f, -14.0f);
        glVertex2f(1.5f, -13.0f);
        glVertex2f(1.0f, -14.0f);

    glEnd();

    glPopMatrix();







    ///////////////
    ///boat-2 motion right to left
    glPushMatrix();
    //glColor3d(1,0,0);
    glTranslatef(_moveB, 0.0f, 0.0f);
    glBegin(GL_QUADS);
    glColor3ub(139,69,19);
        glVertex2f(22.0f, -17.5f);
        glVertex2f(21.0f, -16.5f);
        glVertex2f(17.50f, -16.5f);
        glVertex2f(18.0f, -17.5f);
    glEnd();

    glBegin(GL_QUADS);
    glColor3ub(0,0,0);
        glVertex2f(22.0f, -18.0f);
        glVertex2f(22.0f, -17.5f);
        glVertex2f(17.0f, -17.5f);
        glVertex2f(17.0f, -18.0f);
    glEnd();

    glBegin(GL_QUADS);
    glColor3ub(255,99,71);
        glVertex2f(20.5f, -16.5f);
        glVertex2f(21.0f, -14.5f);
        glVertex2f(18.5f, -14.5f);
        glVertex2f(18.0f, -16.5f);
    glEnd();

    glBegin(GL_QUADS);
    glColor3ub(139,69,19);
        glVertex2f(19.8f, -14.5f);
        glVertex2f(19.8f, -14.0f);
        glVertex2f(19.7f, -14.0f);
        glVertex2f(19.7f, -14.5f);
    glEnd();

    glBegin(GL_TRIANGLES);
    glColor3ub(0,0,0);
        glVertex2f(17.0f, -18.0f);
        glVertex2f(17.0f, -17.5f);
        glVertex2f(15.5f, -17.2f);

    glEnd();

    glBegin(GL_TRIANGLES);
    glColor3ub(0,0,0);
        glVertex2f(22.0f, -17.5f);
        glVertex2f(22.0f, -18.0f);
        glVertex2f(23.5f, -17.2f);

    glEnd();

    glBegin(GL_TRIANGLES);
    glColor3ub(0,0,0);
        glVertex2f(17.0f, -17.5f);
        glVertex2f(18.0f, -17.5f);
        glVertex2f(17.5f, -16.5f);

    glEnd();

    glPopMatrix();

///boat-3 without motion
//polygon-1//
    glBegin(GL_POLYGON);
    glColor3f(0,0,0);
    glVertex2f(8,-2.5);
    glVertex2f(10.5,-2.5);
    glVertex2f(10.5,-2.2);
    glVertex2f(8,-2.2);
    glEnd();

//triangle-1//
    glBegin(GL_TRIANGLES);
    glColor3f(0,0,0);
    glVertex2f(8,-2.2);
    glVertex2f(7,-2);
    glVertex2f(8,-2.5);
    glEnd();

//triangle-2//
    glBegin(GL_TRIANGLES);
    glColor3f(0,0,0);
    glVertex2f(11.5,-2);
    glVertex2f(10.5,-2.2);
    glVertex2f(10.5,-2.5);
    glEnd();


///windmill structure///
    glBegin(GL_POLYGON);// Draw a Red 1x1 Square centered at origin
    glColor3ub(192,192,192);
    //glColor3ub(0,206,209);

    glVertex2f(31.0f,2.0f);
    glVertex2f(30.0f,-6.0f);
    glVertex2f(34.0f,-6.0f);
    glVertex2f(33.0f,2.0f);
    glVertex2f(32.5f,3.0f);
    glVertex2f(31.5f,3.0f);



    glEnd();


     //circle//
    glPushMatrix();
    glColor3ub(0,0,0);
    glTranslatef(32.0f,3.0f,0.0f);
    glutSolidSphere(0.5,150,150);
	glRotatef(_angle1, 0.0f, 0.0f,1.0f);


    glBegin(GL_QUADS);// first stand to hold the blade
    glColor3ub(255,255,0);
    glVertex2f(0.0f,0.0f);
    glVertex2f(5.0f,0.0f);
    glVertex2f(5.0f,0.25f);
    glVertex2f(0.0f,0.25f);
    glEnd();

    glBegin(GL_QUADS);// second stand to hold the blade
    glColor3ub(255,255,0);
    glVertex2f(0.0f,0.0f);
    glVertex2f(0.0f,5.0f);
    glVertex2f(-0.25f,5.0f);
    glVertex2f(-0.25f,0.0f);
    glEnd();

    glBegin(GL_QUADS);// third stand to hold the blade
    glColor3ub(255,255,0);
    glVertex2f(0.0f,-0.25f);
    glVertex2f(0.0f,0.0f);
    glVertex2f(-5.0f,0.0f);
    glVertex2f(-5.0f,-0.25f);
    glEnd();

    glBegin(GL_QUADS);// fourth stand to hold the blade
    glColor3ub(255,255,0);
    glVertex2f(0.25f,0.0f);
    glVertex2f(0.0f,0.0f);
    glVertex2f(0.0f,-5.0f);
    glVertex2f(0.25f,-5.0f);
    glEnd();

    glBegin(GL_TRIANGLES);// first triangular blade to hold the blade
    glColor3ub(128,0,0);
    glVertex2f(0.25f,0.0f);
    glVertex2f(5.0f,-2.5f);
    glVertex2f(5.0f,0.0f);
    glEnd();

    glBegin(GL_TRIANGLES);// second triangular blade to hold the blade
    glColor3ub(128,0,0);
    glVertex2f(0.0f,0.025f);
    glVertex2f(2.5f,5.0f);
    glVertex2f(0.0f,5.0f);
    glEnd();

    glBegin(GL_TRIANGLES);// third triangular blade to hold the blade
    glColor3ub(128,0,0);
    glVertex2f(-0.25f,0.0f);
    glVertex2f(-5.0f,2.5f);
    glVertex2f(-5.0f,0.0f);
    glEnd();

    glBegin(GL_TRIANGLES);// fourth triangular blade to hold the blade
    glColor3ub(128,0,0);
    glVertex2f(0.0f,-0.025f);
    glVertex2f(-2.5f,-5.0f);
    glVertex2f(0.0f,-5.0f);
    glEnd();







    glPopMatrix();





    rain();

    glutSwapBuffers();
}


void init() {
   glClearColor(0.0f, 0.0f, 0.0f, 1.0f);


}

void drawScene2() {
    glClearColor(1.0f,1.0f,1.0f,1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    glColor3d(1,0,0);
    glLoadIdentity(); //Reset the drawing perspective
    gluOrtho2D(-12,38,-19,14);//range
    glMatrixMode(GL_MODELVIEW);


    ///sky///
    glBegin(GL_POLYGON);// Draw a Red 1x1 Square centered at origin
    glColor3ub(0,0,0);
    glVertex2f(38.0f,3.0f);
    glVertex2f(38.0f,14.0f);
    glVertex2f(-12.0f,14.0f);
    glVertex2f(-12.0f,3.0f);

    glEnd();

     ///Stars///

    glPushMatrix();


                    glPointSize(2.0);
                   glColor3ub(255,255,255);

                   glBegin(GL_POINTS);

                    glVertex2f(1,4);
                    glVertex2f(2,7.5);
                    glVertex2f(4,5);
                    glVertex2f(-2,8);
                    glVertex2f(30,5);
                    glVertex2f(-1,9);
                    glVertex2f(35,7);
                    glVertex2f(-3,7);
                    glVertex2f(-7,5);
                    glVertex2f(10,7.5);
                    glVertex2f(12,8.5);
                    glVertex2f(-11,6);
                    glVertex2f(37,7.5);
                    glVertex2f(-10,4.5);
                    glVertex2f(13,7);
                    glVertex2f(14,7.5);
                    glVertex2f(18,10);
                   // glVertex2f(-10,4.5);

       glEnd();



    glPopMatrix();


///moon///
    createMoon();


    ///cloud ///

    for(int i=0; i<numberOfCloud; i++)
    {
        createCloud(cloudShape[i]);
    }


  ///sea portion///
    glBegin(GL_POLYGON);// Draw a Red 1x1 Square centered at origin
    glColor3ub(65,105,225);
    glVertex2f(38.0f,-19.0f);
    glVertex2f(38.0f,3.0f);
    glVertex2f(-12.0f,3.0f);
    glVertex2f(-12.0f,-19.0f);

    glEnd();

///Fish

    fish5();
    fish4();
    //fish3();

///left soil portion///
//polygon 1
    glBegin(GL_POLYGON);// Draw a Red 1x1 Square centered at origin
    glColor3ub(0,128,0);
    glVertex2f(4.0f,0.0f);
    glVertex2f(6.0f,1.0f);
    glVertex2f(3.0f,3.0f);
    glVertex2f(-12.0f,3.0f);
    glVertex2f(-12.0f,0.0f);

    glEnd();

//polygon 2
    glBegin(GL_POLYGON);// Draw a Red 1x1 Square centered at origin
    glColor3ub(0,128,0);
    glVertex2f(5.0f,-3.0f);
    glVertex2f(7.0f,-1.5f);
    glVertex2f(4.0f,0.0f);
    glVertex2f(-12.0f,0.0f);
    glVertex2f(-12.0f,-3.0f);

    glEnd();

//polygon 3
    glBegin(GL_POLYGON);// Draw a Red 1x1 Square centered at origin
    glColor3ub(0,128,0);
    glVertex2f(6.0f,-4.5f);
    glVertex2f(8.0f,-4.0f);
    glVertex2f(5.0f,-3.0f);
    glVertex2f(-12.0f,-3.0f);
    glVertex2f(-12.0f,-4.5f);

    glEnd();

//polygon 4
    glBegin(GL_POLYGON);// Draw a Red 1x1 Square centered at origin
    glColor3ub(0,128,0);
    glVertex2f(2.0f,-9.0f);
    glVertex2f(9.5f,-7.0f);
    glVertex2f(6.0f,-4.5f);
    glVertex2f(-12.0f,-4.5f);
    glVertex2f(-12.0f,-9.5f);

    glEnd();

//polygon 5
    glBegin(GL_POLYGON);// Draw a Red 1x1 Square centered at origin
    glColor3ub(0,128,0);
    glVertex2f(2.0f,-11.5f);
    glVertex2f(5.0f,-10.0f);
    glVertex2f(2.0f,-9.0f);
    glVertex2f(-12.0f,-9.0f);
    glVertex2f(-12.0f,-11.5f);

    glEnd();

///left sidewise border partition///
 //polygon 1
    glBegin(GL_POLYGON);// Draw a Red 1x1 Square centered at origin
    glColor3ub(0,0,0);
    glVertex2f(6.0f,0.5f);
    glVertex2f(6.0f,1.0f);
    glVertex2f(4.0f,0.0f);
    glVertex2f(4.5f,-0.25f);

    glEnd();

 //polygon 2
    glBegin(GL_POLYGON);// Draw a Red 1x1 Square centered at origin
    glColor3ub(0,0,0);
    glVertex2f(7.0f,-2.0f);
    glVertex2f(7.0f,-1.5f);
    glVertex2f(5.0f,-3.0f);
    glVertex2f(5.5f,-3.15f);

    glEnd();

//polygon 3
    glBegin(GL_POLYGON);// Draw a Red 1x1 Square centered at origin
    glColor3ub(0,0,0);
    glVertex2f(8.0f,-4.4f);
    glVertex2f(8.0f,-4.0f);
    glVertex2f(6.0f,-4.5f);
    glVertex2f(6.3f,-4.8f);

    glEnd();

//polygon 4
    glBegin(GL_POLYGON);// Draw a Red 1x1 Square centered at origin
    glColor3ub(0,0,0);
    glVertex2f(9.5f,-7.5f);
    glVertex2f(9.5f,-7.0f);
    glVertex2f(2.0f,-9.0f);
    glVertex2f(3.0f,-9.3f);

    glEnd();

//polygon 5
    glBegin(GL_POLYGON);// Draw a Red 1x1 Square centered at origin
    glColor3ub(0,0,0);
    glVertex2f(5.0f,-10.5f);
    glVertex2f(5.0f,-10.0f);
    glVertex2f(2.0f,-11.5f);
    glVertex2f(2.0f,-12.0f);

    glEnd();

//polygon 6
    glBegin(GL_POLYGON);// Draw a Red 1x1 Square centered at origin
    glColor3ub(0,0,0);
    glVertex2f(2.0f,-12.0f);
    glVertex2f(2.0f,-11.5f);
    glVertex2f(-12.0f,-11.5f);
    glVertex2f(-12.0f,-12.0f);

    glEnd();





///straw///
//polygon 1
    glBegin(GL_POLYGON);// Draw a Red 1x1 Square centered at origin
    glColor3ub(189,183,107);
    glVertex2f(2.5f,-1.8f);
    glVertex2f(2.0f,0.2f);
    glVertex2f(-3.0f,0.2f);
    glVertex2f(-3.5f,-1.8f);

    glEnd();

 //polygon 2
    glBegin(GL_POLYGON);// Draw a Red 1x1 Square centered at origin
    glColor3ub(189,183,107);
    glVertex2f(2.0f,0.2f);
    glVertex2f(1.0f,1.7f);
    glVertex2f(-2.0f,1.7f);
    glVertex2f(-3.0f,0.2f);

    glEnd();
//polygon 3
    glBegin(GL_POLYGON);// Draw a Red 1x1 Square centered at origin
    glColor3ub(189,183,107);
    glVertex2f(1.0f,1.7f);
    glVertex2f(0.0f,2.2f);
    glVertex2f(-1.0f,2.2f);
    glVertex2f(-2.0f,1.7f);

    glEnd();

///triangle///

    glBegin(GL_POLYGON);// Draw a Red 1x1 Square centered at origin
    glColor3ub(189,183,107);
    glVertex2f(0.0f,2.2f);
    glVertex2f(-0.5f,2.4f);
    glVertex2f(-1.0f,2.2f);

    glEnd();

//polygon

    glBegin(GL_POLYGON);// Draw a Red 1x1 Square centered at origin
    glColor3ub(189,183,107);
    glVertex2f(-0.4f,2.3f);
    glVertex2f(-0.4f,2.7f);
    glVertex2f(-0.6f,2.7f);
    glVertex2f(-0.6f,2.3f);

    glEnd();


///house on left side///
//polygon 1
    glBegin(GL_POLYGON);// Draw a Red 1x1 Square centered at origin
    glColor3ub(139,69,19);
    glVertex2f(-2.7f,2.2f);
    glVertex2f(-6.7f,2.2f);
    glVertex2f(-5.0f,0.0f);
    glVertex2f(-1.0f,0.0f);

    glEnd();


//polygon 2
    glBegin(GL_POLYGON);// Draw a Red 1x1 Square centered at origin
    glColor3ub(139,69,19);
    glVertex2f(-6.5f,2.0f);
    glVertex2f(-6.7f,2.2f);
    glVertex2f(-8.5f,0.0f);
    glVertex2f(-8.0f,0.0f);

    glEnd();

//polygon 3
    glBegin(GL_POLYGON);// Draw a Red 1x1 Square centered at origin
    glColor3ub(205,133,63);
    glVertex2f(-6.5f,2.0f);
    glVertex2f(-8.0f,0.0f);
    glVertex2f(-8.0f,-1.5f);
    glVertex2f(-5.0f,-2.0f);
    glVertex2f(-5.0f,0.0f);

    glEnd();

//polygon 4
    glBegin(GL_POLYGON);// Draw a Red 1x1 Square centered at origin
    glColor3ub(128,135,32);
    glVertex2f(-6.0f,-1.0f);
    glVertex2f(-6.0f,0.0f);
    glVertex2f(-7.0f,0.0f);
    glVertex2f(-7.0f,-1.0f);

    glEnd();

//polygon 5
    glBegin(GL_POLYGON);// Draw a Red 1x1 Square centered at origin
    glColor3ub(139,69,19);
    glVertex2f(-5.0f,-2.0f);
    glVertex2f(-8.0f,-1.5f);
    glVertex2f(-8.3f,-1.9f);
    glVertex2f(-5.0f,-2.5f);

    glEnd();

//polygon 6
    glBegin(GL_POLYGON);// Draw a Red 1x1 Square centered at origin
    glColor3ub(184,134,11);
    glVertex2f(-1.5f,-2.0f);
    glVertex2f(-1.5f,0.0f);
    glVertex2f(-5.0f,0.0f);
    glVertex2f(-5.0f,-2.0f);

    glEnd();

//polygon 7
    glBegin(GL_POLYGON);// Draw a Red 1x1 Square centered at origin
    glColor3ub(139,69,19);
    glVertex2f(-5.0f,-2.0f);
    glVertex2f(-5.0f,-2.5f);
    glVertex2f(-1.0f,-2.5f);
    glVertex2f(-1.5f,-2.0f);

    glEnd();


///door:left
//polygon a
    glBegin(GL_POLYGON);// Draw a Red 1x1 Square centered at origin
    glColor3ub(0,0,0);
    glVertex2f(-2.5f,-2.0f);
    glVertex2f(-2.5f,-0.2f);
    glVertex2f(-4.0f,-0.2f);
    glVertex2f(-4.0f,-2.0f);

    glEnd();

//polygon b
    glBegin(GL_POLYGON);// Draw a Red 1x1 Square centered at origin
    glColor3ub(128,135,32);
    glVertex2f(-3.3f,-2.0f);
    glVertex2f(-3.3f,-0.4f);
    glVertex2f(-4.0f,-0.2f);
    glVertex2f(-4.0f,-2.0f);

    glEnd();

//polygon c
    glBegin(GL_POLYGON);// Draw a Red 1x1 Square centered at origin
    glColor3ub(128,135,32);
    glVertex2f(-2.5f,-2.0f);
    glVertex2f(-2.5f,-0.2f);
    glVertex2f(-3.2f,-0.4f);
    glVertex2f(-3.2f,-2.0f);

    glEnd();



///tree left side
//polygon 1
    glBegin(GL_POLYGON);
    glColor3ub(139,69,19);
    glVertex2f(-9.5,1);
    glVertex2f(-9,1.5);
    glVertex2f(-9.5,2.5);
    glVertex2f(-10,2);
    glEnd();

//polygon 2
    glBegin(GL_POLYGON);
    glColor3ub(139,69,19);
    glVertex2f(-9,1.5);
    glVertex2f(-8.5,1);
    glVertex2f(-8,2);
    glVertex2f(-8,2.5);
    glEnd();

//polygon 3
    glBegin(GL_POLYGON);
    glColor3ub(139,69,19);
    glVertex2f(-9.5,1);
    glVertex2f(-9.5,-3.5);
    glVertex2f(-9,-4);
    glVertex2f(-8.5,-3.5);
    glVertex2f(-8.5,1);
    glEnd();

//polygon 4
    glBegin(GL_POLYGON);
    glColor3ub(139,69,19);
    glVertex2f(-8.5,-3);
    glVertex2f(-9.5,-3);
    glVertex2f(-10.5,-3.5);
    glVertex2f(-7.5,-3.5);
    glEnd();



//triangle 1
    glBegin(GL_TRIANGLES);
    glColor3ub(139,69,19);
    glVertex2f(-10,-0.5);
    glVertex2f(-9.5,-1);
    glVertex2f(-9.5,-0.6);
    glEnd();

//triangle 2
    glBegin(GL_TRIANGLES);
    glColor3ub(139,69,19);
    glVertex2f(-9.5,1);
    glVertex2f(-9,1.5);
    glVertex2f(-8.5,1);
    glEnd();

///shop

///Polygon 1
    glBegin(GL_POLYGON);// Draw a Red 1x1 Square centered at origin
    glColor3ub(0,0,139);
    glVertex2f(-2.7f,-3.2f);
    glVertex2f(-6.7f,-3.2f);
    glVertex2f(-5.3f,-5.0f);
    glVertex2f(-1.1f,-5.0f);

    glEnd();

///Polygon 2
    glBegin(GL_POLYGON);// Draw a Red 1x1 Square centered at origin
    glColor3ub(205,133,63);
    glVertex2f(-6.7f,-3.2f);
    glVertex2f(-6.6f,-7.0f);
    glVertex2f(-5.0f,-8.0f);
    glVertex2f(-5.0f,-5.0f);

    glEnd();


///Polygon 3
    glBegin(GL_POLYGON);// Draw a Red 1x1 Square centered at origin
    glColor3ub(184,134,11);
    glVertex2f(-1.5f,-8.0f);
    glVertex2f(-1.5f,-5.0f);
    glVertex2f(-5.0f,-5.0f);
    glVertex2f(-5.0f,-8.0f);

    glEnd();


//polygon 4
    glBegin(GL_POLYGON);// Draw a Red 1x1 Square centered at origin
    glColor3ub(128,135,32);
    glVertex2f(-2.5f,-7.5f);
    glVertex2f(-2.5f,-5.5f);
    glVertex2f(-4.5f,-5.5f);
    glVertex2f(-4.5f,-7.5f);

    glEnd();

///Store keeper
    drawHair(.6,-3.5,-6.1);
    drawCircle(.5,-3.5,-6.2);
    drawEye(.15,-3.7,-6.1);
    drawEye(.15,-3.25,-6.1);
    drawMouth();
    drawBody();
    drawBody2();

///Human
    drawHair(1,2.5,-1.5);
    drawCircle(.9,2.5,-1.7);
    drawEye(.25,2.05,-1.45);
    drawEye(.25,2.85,-1.45);
    drawHumanMouth();
    drawHumanBody();
    drawHumanBody2();
    drawHumanBody3();
    drawHumanLeftLag();
    drawHumanLeftLag1();
    drawHumanRightLag();
    drawHumanRightLag1();
    drawHumanLeftHand();
    drawHumanRightHand();
    drawBag();

///right soil portion///
//polygon1//
    glBegin(GL_POLYGON);

    glColor3ub(0,128,0);
    glVertex2f(13.0f,3.0f);
    glVertex2f(16.0f,1.0f);
    glVertex2f(38.0f,1.0f);
    glVertex2f(38.0f,3.0f);

    glEnd();

//polygon2//
    glBegin(GL_POLYGON);

    glColor3ub(0,128,0);
    glVertex2f(16,1);
    glVertex2f(14,0);
    glVertex2f(17,-1.5);
    glVertex2f(38,-1.5);
    glVertex2f(38,1);

    glEnd();

//polygon3//
    glBegin(GL_POLYGON);

    glColor3ub(0,128,0);
    glVertex2f(17,-1.5);
    glVertex2f(15,-3);
    glVertex2f(18,-4);
    glVertex2f(38,-4);
    glVertex2f(38,-1.5);

    glEnd();

//polygon4//
    glBegin(GL_POLYGON);

    glColor3ub(0,128,0);
    glVertex2f(18,-4);
    glVertex2f(16,-4.5);
    glVertex2f(21,-7);
    glVertex2f(38,-7);
    glVertex2f(38,-4);

    glEnd();

//polygon5//
    glBegin(GL_POLYGON);

    glColor3ub(0,128,0);
    glVertex2f(21,-7);
    glVertex2f(19,-8);
    glVertex2f(22,-9);
    glVertex2f(38,-9);
    glVertex2f(38,-7);

    glEnd();

///right sidewise border partition///

//polygon1//
    glBegin(GL_POLYGON);

    glColor3ub(0,0,0);
    glVertex2f(12.5,3);
    glVertex2f(15.6,0.8);
    glVertex2f(16,1);
    glVertex2f(13,3);

    glEnd();

//polygon2//
    glBegin(GL_POLYGON);

    glColor3ub(0,0,0);
    glVertex2f(14,-0.5);
    glVertex2f(16.4,-1.9);
    glVertex2f(17,-1.5);
    glVertex2f(14,0);

    glEnd();

//polygon3//
    glBegin(GL_POLYGON);

    glColor3ub(0,0,0);
    glVertex2f(15,-3.5);
    glVertex2f(17,-4.2);
    glVertex2f(18,-4);
    glVertex2f(15,-3);

    glEnd();

//polygon4//
    glBegin(GL_POLYGON);

    glColor3ub(0,0,0);
    glVertex2f(16,-5);
    glVertex2f(20.5,-7.2);
    glVertex2f(21,-7);
    glVertex2f(16,-4.5);

    glEnd();

//polygon5//
    glBegin(GL_POLYGON);

    glColor3ub(0,0,0);
    glVertex2f(19,-8.5);
    glVertex2f(22,-9.5);
    glVertex2f(22,-9);
    glVertex2f(19,-8);

    glEnd();

//polygon6//
    glBegin(GL_POLYGON);

    glColor3ub(0,0,0);
    glVertex2f(38,-9);
    glVertex2f(22,-9);
    glVertex2f(22,-9.5);
    glVertex2f(38,-9.5);

    glEnd();

///house on right side///

//polygon1//
    glBegin(GL_POLYGON);

    //glColor3ub(25,25,112);
    glColor3ub(47,79,79);
    glVertex2f(25.3,1.2);
    glVertex2f(21.3,1.2);
    glVertex2f(23,-1);
    glVertex2f(27,-1);

    glEnd();

//polygon2//
    glBegin(GL_POLYGON);
      glColor3ub(47,79,79);
    //glColor3ub(139,69,19);
    glVertex2f(21.5,1);
    glVertex2f(21.3,1.2);
    glVertex2f(19.5,-1);
    glVertex2f(20,-1);

    glEnd();

//polygon3//
    glBegin(GL_POLYGON);

    glColor3ub(184,134,11);
    glVertex2f(21.5,1);
    glVertex2f(20,-1);
    glVertex2f(20,-2.5);
    glVertex2f(23,-3);
    glVertex2f(23,-1);

    glEnd();

//polygon4//
   glBegin(GL_POLYGON);

    glColor3ub(160,82,45);
    glVertex2f(22,-2);
    glVertex2f(22,-1);
   glVertex2f(21,-1);
    glVertex2f(21,-2);

    glEnd();

//polygon5//
    glBegin(GL_POLYGON);
     glColor3ub(47,79,79);
    //glColor3ub(139,69,19);
    glVertex2f(23,-3);
    glVertex2f(20,-2.5);
    glVertex2f(19.7,-2.9);
    glVertex2f(23,-3.5);

    glEnd();

//polygon6//
   glBegin(GL_POLYGON);

   glColor3ub(205,133,63);
    glVertex2f(26.5,-3);
    glVertex2f(26.5,-1);
    glVertex2f(23,-1);
    glVertex2f(23,-3);

    glEnd();

//polygon7//
    glBegin(GL_POLYGON);
     glColor3ub(47,79,79);
    //glColor3ub(139,69,19);
    glVertex2f(23,-3);
    glVertex2f(23,-3.5);
    glVertex2f(27,-3.5);
    glVertex2f(26.5,-3);

    glEnd();

///door:right///
//polygon a//
    glBegin(GL_POLYGON);

    glColor3ub(0,0,0);
    glVertex2f(25.5,-3);
    glVertex2f(25.5,-1.2);
    glVertex2f(24,-1.2);
    glVertex2f(24,-3);

    glEnd();

//polygon b//
    glBegin(GL_POLYGON);

    glColor3ub(160,82,45);
    glVertex2f(24.7,-3);
    glVertex2f(24.7,-1.4);
    glVertex2f(24,-1.2);
    glVertex2f(24,-3);

    glEnd();

//polygon c//
    glBegin(GL_POLYGON);

    glColor3ub(160,82,45);
    glVertex2f(25.5,-3);
    glVertex2f(25.5,-1.2);
    glVertex2f(24.8,-1.4);
    glVertex2f(24.8,-3);

    glEnd();

///hillview///
///hill-1///
//polygon1//
    glBegin(GL_POLYGON);
    glColor3ub(0,100,0);
    glVertex2f(-12,3);
    glVertex2f(-11.5,4.5);
    glVertex2f(-7.5,4.5);
    glVertex2f(-7,3);
    //glVertex2f();
    glEnd();

//polygon2//
    glBegin(GL_POLYGON);
    glColor3ub(0,100,0);
    glVertex2f(-11.5,4.5);
    glVertex2f(-11,5.5);
    glVertex2f(-8,5.5);
    glVertex2f(-7.5,4.5);
    //glVertex2f();
    glEnd();

//polygon3//
    glBegin(GL_POLYGON);
    glColor3ub(0,100,0);
    glVertex2f(-11,5.5);
    glVertex2f(-10.5,6);
    glVertex2f(-8.5,6);
    glVertex2f(-8,5.5);
    //glVertex2f();
    glEnd();

//polygon4//
    glBegin(GL_POLYGON);
    glColor3ub(0,100,0);
    glVertex2f(-10.5,6);
    glVertex2f(-10.3,6.2);
    glVertex2f(-8.7,6.2);
    glVertex2f(-8.5,6);
    //glVertex2f();
    glEnd();

//polygon5//
    glBegin(GL_POLYGON);
    glColor3ub(0,100,0);
    glVertex2f(-10.3,6.2);
    glVertex2f(-10.2,6.3);
    glVertex2f(-8.8,6.3);
    glVertex2f(-8.7,6);
    //glVertex2f();
    glEnd();

///Hill 2///
//polygon1//
    glBegin(GL_POLYGON);
    glColor3ub(0,100,0);
    glVertex2f(-7,3);
    glVertex2f(-6.5,4);
    glVertex2f(-4.5,4);
    glVertex2f(-4,3);
    //glVertex2f();
    glEnd();

//polygon2//
    glBegin(GL_POLYGON);
    glColor3ub(0,100,0);
    glVertex2f(-6.5,4);
    glVertex2f(-6,4.5);
    glVertex2f(-5,4.5);
    glVertex2f(-4.5,4);
    //glVertex2f();
    glEnd();

///Hill 3///
//polygon1//
    glBegin(GL_POLYGON);
    glColor3ub(0,100,0);
    glVertex2f(-4,3);
    glVertex2f(-3.5,4.5);
    glVertex2f(-1.5,4.5);
    glVertex2f(-1,3);
    //glVertex2f();
    glEnd();

//polygon2//
    glBegin(GL_POLYGON);
    glColor3ub(0,100,0);
    glVertex2f(-3.5,4.5);
    glVertex2f(-3,5);
    glVertex2f(-2,5);
    glVertex2f(-1.5,4.5);
    //glVertex2f();
    glEnd();

///Hill 4///
//polygon1//
    glBegin(GL_POLYGON);
    glColor3ub(0,100,0);
    glVertex2f(-1,3);
    glVertex2f(-0.5,6);
    glVertex2f(4.5,6);
    glVertex2f(5,3);
    //glVertex2f();
    glEnd();

//polygon2//
    glBegin(GL_POLYGON);
    glColor3ub(0,100,0);
    glVertex2f(-0.5,6);
    glVertex2f(0,7);
    glVertex2f(4,7);
    glVertex2f(4.5,6);
    //glVertex2f();
    glEnd();

//polygon3//
    glBegin(GL_POLYGON);
    glColor3ub(0,100,0);
    glVertex2f(0,7);
    glVertex2f(1,8);
    glVertex2f(3,8);
    glVertex2f(4,7);
    //glVertex2f();
    glEnd();
//triangle//
    glBegin(GL_TRIANGLES);
    glColor3ub(0,100,0);
    glVertex2f(1,8);
    glVertex2f(3,8);
    glVertex2f(2,8.3);
    glEnd();

///hill 5///
//polygon1//
    glBegin(GL_POLYGON);
    glColor3ub(0,100,0);
    glVertex2f(5,3);
    glVertex2f(5.5,4);
    glVertex2f(6.5,4);
    glVertex2f(7,3);
    //glVertex2f();
    glEnd();

//triangle//
    glBegin(GL_TRIANGLES);
    glColor3ub(0,100,0);
    glVertex2f(5.5,4);
    glVertex2f(6.5,4);
    glVertex2f(6,4.5);
    glEnd();

///hill 6///
//polygon1//
    glBegin(GL_POLYGON);
    glColor3ub(0,100,0);
    glVertex2f(7,3);
    glVertex2f(8,5);
    glVertex2f(13,5);
    glVertex2f(14,3);
    //glVertex2f();
    glEnd();

//polygon2//
    glBegin(GL_POLYGON);
    glColor3ub(0,100,0);
    glVertex2f(8,5);
    glVertex2f(9,6);
    glVertex2f(12,6);
    glVertex2f(13,5);
    //glVertex2f();
    glEnd();
//triangle//
    glBegin(GL_TRIANGLES);
    glColor3ub(0,100,0);
    glVertex2f(9,6);
    glVertex2f(12,6);
    glVertex2f(10.5,6.5);
    glEnd();

///hill 7///
//polygon1//
    glBegin(GL_POLYGON);
    glColor3ub(0,100,0);
    glVertex2f(14,3);
    glVertex2f(15,5);
    glVertex2f(20,5);
    glVertex2f(21,3);
    //glVertex2f();
    glEnd();

//polygon2//
    glBegin(GL_POLYGON);
    glColor3ub(0,100,0);
    glVertex2f(15,5);
    glVertex2f(16,6);
    glVertex2f(19,6);
    glVertex2f(20,5);
    //glVertex2f();
    glEnd();

//triangle//
    glBegin(GL_TRIANGLES);
    glColor3ub(0,100,0);
    glVertex2f(16,6);
    glVertex2f(19,6);
    glVertex2f(17.5,6.5);
    glEnd();

///hill 8///
//polygon1//
    glBegin(GL_POLYGON);
    glColor3ub(0,100,0);
    glVertex2f(21,3);
    glVertex2f(22,5);
    glVertex2f(24,5);
    glVertex2f(25,3);
    //glVertex2f();
    glEnd();

//polygon2//
    glBegin(GL_POLYGON);
    glColor3ub(0,100,0);
    glVertex2f(22,5);
    glVertex2f(22.5,5.5);
    glVertex2f(23.5,5.5);
    glVertex2f(24,5);
    //glVertex2f();
    glEnd();

///hill 9///
//polygon1//
    glBegin(GL_POLYGON);
    glColor3ub(0,100,0);
    glVertex2f(25,3);
    glVertex2f(25.5,4.5);
    glVertex2f(28.5,4.5);
    glVertex2f(29,3);
    //glVertex2f();
    glEnd();

//polygon2//
    glBegin(GL_POLYGON);
    glColor3ub(0,100,0);
    glVertex2f(25.5,4.5);
    glVertex2f(26,5);
    glVertex2f(28,5);
    glVertex2f(28.5,4.5);
    //glVertex2f();
    glEnd();

///hill 10///
//polygon4//
    glBegin(GL_POLYGON);
    glColor3ub(0,100,0);
    glVertex2f(29,3);
    glVertex2f(29.5,4);
    glVertex2f(30.5,4);
    glVertex2f(31,3);
    //glVertex2f();
    glEnd();

//triangle//
    glBegin(GL_TRIANGLES);
    glColor3ub(0,100,0);
    glVertex2f(29.5,4);
    glVertex2f(30,4.5);
    glVertex2f(30.5,4);
    glEnd();

///hill 11///
//polygon1//
    glBegin(GL_POLYGON);
    glColor3ub(0,100,0);
    glVertex2f(31,3);
    glVertex2f(31.5,4.5);
    glVertex2f(32.5,4.5);
    glVertex2f(33,3);
    //glVertex2f();
    glEnd();

//triangle//
    glBegin(GL_TRIANGLES);
    glColor3ub(0,100,0);
    glVertex2f(31.5,4.5);
    glVertex2f(32.5,4.5);
    glVertex2f(32,5);
    glEnd();

///hill 12///
//polygon1//
    glBegin(GL_POLYGON);
    glColor3ub(0,100,0);
    glVertex2f(33,3);
    glVertex2f(33.5,4.5);
    glVertex2f(37.5,4.5);
    glVertex2f(38,3);
    //glVertex2f();
    glEnd();

//polygon2//
    glBegin(GL_POLYGON);
    glColor3ub(0,100,0);
    glVertex2f(33.5,4.5);
    glVertex2f(34,5);
    glVertex2f(37,5);
    glVertex2f(37.5,4.5);
    //glVertex2f();
    glEnd();

//triangle//
    glBegin(GL_TRIANGLES);
    glColor3ub(0,100,0);
    glVertex2f(34,5);
    glVertex2f(37,5);
    glVertex2f(35.5,5.5);
    glEnd();

///tree upper side///

    glPushMatrix();
    glColor3ub(50,205,50);
    glTranslatef(-10.5, 2.5, 0);
    glutSolidSphere(1.0, 150, 150);
    glPopMatrix();

    glPushMatrix();
    glColor3ub(50,205,50);
    glTranslatef(-9.5, 3.0, 0);
    glutSolidSphere(1.0, 150, 150);
    glPopMatrix();

    glPushMatrix();
    glColor3ub(50,205,50);
    glTranslatef(-7.5, 1.5, 0);
    glutSolidSphere(1.0, 150, 150);
    glPopMatrix();

    glPushMatrix();
    glColor3ub(50,205,50);
    glTranslatef(-7.5, 2.5, 0);
    glutSolidSphere(1.0, 150, 150);
    glPopMatrix();

    glPushMatrix();
    glColor3ub(50,205,50);
    glTranslatef(-8.5, 3.5, 0);
    glutSolidSphere(1.0, 150, 150);
    glPopMatrix();

    glPushMatrix();
    glColor3ub(50,205,50);
    glTranslatef(-10.0, 4.0, 0);
    glutSolidSphere(1.0, 150, 150);
    glPopMatrix();

    glPushMatrix();
    glColor3ub(50,205,50);
    glTranslatef(-8.0, 4.0, 0);
    glutSolidSphere(1.0, 150, 150);
    glPopMatrix();

    glPushMatrix();
    glColor3ub(50,205,50);
    glTranslatef(-9.0, 5.0, 0);
    glutSolidSphere(1.0, 150, 150);
    glPopMatrix();







///boat-1 motion left to right
    glPushMatrix();
    //glColor3d(1,0,0);
    glTranslatef(_moveA, 0.0f, 0.0f);
    glBegin(GL_QUADS);
    glColor3ub(139,69,19);
        glVertex2f(1.0f, -14.0f);
        glVertex2f(1.50f, -13.0f);
        glVertex2f(-2.0f, -13.0f);
        glVertex2f(-3.0f, -14.0f);
    glEnd();

    glBegin(GL_QUADS);
    glColor3ub(0,0,0);
        glVertex2f(2.0f, -14.5f);
        glVertex2f(2.0f, -14.0f);
        glVertex2f(-3.0f, -14.0f);
        glVertex2f(-3.0f, -14.5f);
    glEnd();

    glBegin(GL_TRIANGLES);
    glColor3ub(0,0,0);
        glVertex2f(-3.0f, -14.5f);
        glVertex2f(-3.0f, -14.0f);
        glVertex2f(-4.5f, -13.7f);

    glEnd();

    glBegin(GL_TRIANGLES);
    glColor3ub(0,0,0);
        glVertex2f(2.0f, -14.5f);
        glVertex2f(3.5f, -13.7f);
        glVertex2f(2.0f, -14.0f);

    glEnd();

    glBegin(GL_TRIANGLES);
    glColor3ub(0,0,0);
        glVertex2f(2.0f, -14.0f);
        glVertex2f(1.5f, -13.0f);
        glVertex2f(1.0f, -14.0f);

    glEnd();

    glPopMatrix();







    ///////////////
    ///boat-2 motion right to left
    glPushMatrix();
    //glColor3d(1,0,0);
    glTranslatef(_moveB, 0.0f, 0.0f);
    glBegin(GL_QUADS);
    glColor3ub(139,69,19);
        glVertex2f(22.0f, -17.5f);
        glVertex2f(21.0f, -16.5f);
        glVertex2f(17.50f, -16.5f);
        glVertex2f(18.0f, -17.5f);
    glEnd();

    glBegin(GL_QUADS);
    glColor3ub(0,0,0);
        glVertex2f(22.0f, -18.0f);
        glVertex2f(22.0f, -17.5f);
        glVertex2f(17.0f, -17.5f);
        glVertex2f(17.0f, -18.0f);
    glEnd();

    glBegin(GL_QUADS);
    glColor3ub(255,99,71);
        glVertex2f(20.5f, -16.5f);
        glVertex2f(21.0f, -14.5f);
        glVertex2f(18.5f, -14.5f);
        glVertex2f(18.0f, -16.5f);
    glEnd();

    glBegin(GL_QUADS);
    glColor3ub(139,69,19);
        glVertex2f(19.8f, -14.5f);
        glVertex2f(19.8f, -14.0f);
        glVertex2f(19.7f, -14.0f);
        glVertex2f(19.7f, -14.5f);
    glEnd();

    glBegin(GL_TRIANGLES);
    glColor3ub(0,0,0);
        glVertex2f(17.0f, -18.0f);
        glVertex2f(17.0f, -17.5f);
        glVertex2f(15.5f, -17.2f);

    glEnd();

    glBegin(GL_TRIANGLES);
    glColor3ub(0,0,0);
        glVertex2f(22.0f, -17.5f);
        glVertex2f(22.0f, -18.0f);
        glVertex2f(23.5f, -17.2f);

    glEnd();

    glBegin(GL_TRIANGLES);
    glColor3ub(0,0,0);
        glVertex2f(17.0f, -17.5f);
        glVertex2f(18.0f, -17.5f);
        glVertex2f(17.5f, -16.5f);

    glEnd();

    glPopMatrix();

///boat-3 without motion
//polygon-1//
    glBegin(GL_POLYGON);
    glColor3f(0,0,0);
    glVertex2f(8,-2.5);
    glVertex2f(10.5,-2.5);
    glVertex2f(10.5,-2.2);
    glVertex2f(8,-2.2);
    glEnd();

//triangle-1//
    glBegin(GL_TRIANGLES);
    glColor3f(0,0,0);
    glVertex2f(8,-2.2);
    glVertex2f(7,-2);
    glVertex2f(8,-2.5);
    glEnd();

//triangle-2//
    glBegin(GL_TRIANGLES);
    glColor3f(0,0,0);
    glVertex2f(11.5,-2);
    glVertex2f(10.5,-2.2);
    glVertex2f(10.5,-2.5);
    glEnd();


///windmill structure///
    glBegin(GL_POLYGON);// Draw a Red 1x1 Square centered at origin
    glColor3ub(192,192,192);
    //glColor3ub(0,206,209);

    glVertex2f(31.0f,2.0f);
    glVertex2f(30.0f,-6.0f);
    glVertex2f(34.0f,-6.0f);
    glVertex2f(33.0f,2.0f);
    glVertex2f(32.5f,3.0f);
    glVertex2f(31.5f,3.0f);



    glEnd();


     //circle//
    glPushMatrix();
    glColor3ub(0,0,0);
    glTranslatef(32.0f,3.0f,0.0f);
    glutSolidSphere(0.5,150,150);
	glRotatef(_angle1, 0.0f, 0.0f,1.0f);


    glBegin(GL_QUADS);// first stand to hold the blade
    glColor3ub(255,255,0);
    glVertex2f(0.0f,0.0f);
    glVertex2f(5.0f,0.0f);
    glVertex2f(5.0f,0.25f);
    glVertex2f(0.0f,0.25f);
    glEnd();

    glBegin(GL_QUADS);// second stand to hold the blade
    glColor3ub(255,255,0);
    glVertex2f(0.0f,0.0f);
    glVertex2f(0.0f,5.0f);
    glVertex2f(-0.25f,5.0f);
    glVertex2f(-0.25f,0.0f);
    glEnd();

    glBegin(GL_QUADS);// third stand to hold the blade
    glColor3ub(255,255,0);
    glVertex2f(0.0f,-0.25f);
    glVertex2f(0.0f,0.0f);
    glVertex2f(-5.0f,0.0f);
    glVertex2f(-5.0f,-0.25f);
    glEnd();

    glBegin(GL_QUADS);// fourth stand to hold the blade
    glColor3ub(255,255,0);
    glVertex2f(0.25f,0.0f);
    glVertex2f(0.0f,0.0f);
    glVertex2f(0.0f,-5.0f);
    glVertex2f(0.25f,-5.0f);
    glEnd();

    glBegin(GL_TRIANGLES);// first triangular blade to hold the blade
    glColor3ub(128,0,0);
    glVertex2f(0.25f,0.0f);
    glVertex2f(5.0f,-2.5f);
    glVertex2f(5.0f,0.0f);
    glEnd();

    glBegin(GL_TRIANGLES);// second triangular blade to hold the blade
    glColor3ub(128,0,0);
    glVertex2f(0.0f,0.025f);
    glVertex2f(2.5f,5.0f);
    glVertex2f(0.0f,5.0f);
    glEnd();

    glBegin(GL_TRIANGLES);// third triangular blade to hold the blade
    glColor3ub(128,0,0);
    glVertex2f(-0.25f,0.0f);
    glVertex2f(-5.0f,2.5f);
    glVertex2f(-5.0f,0.0f);
    glEnd();

    glBegin(GL_TRIANGLES);// fourth triangular blade to hold the blade
    glColor3ub(128,0,0);
    glVertex2f(0.0f,-0.025f);
    glVertex2f(-2.5f,-5.0f);
    glVertex2f(0.0f,-5.0f);
    glEnd();



    glPopMatrix();






    glutSwapBuffers();
}






void SpecialInput(int key, int x, int y)
{
switch(key)
{
case GLUT_KEY_UP:

 glutDisplayFunc(drawScene);
break;
case GLUT_KEY_DOWN:

 glutDisplayFunc(drawScene2);
break;

}
glutPostRedisplay();
}



void update1(int value) {

    _moveA += speed;
    if(_moveA > 38)
    {
        _moveA = -38;
    }
     //Notify GLUT that the display has changed

//_moveA += 0.11;

    glutTimerFunc(20, update1, 0); //Notify GLUT to call update again in 25 milliseconds
    glutPostRedisplay();
}

void handleMouse(int button, int state, int x, int y) {

if (button == GLUT_LEFT_BUTTON)
	{
	    speed += 0.05f;
    }


else if (button == GLUT_RIGHT_BUTTON)
	{
	    speed -= 0.05f;
	}
glutPostRedisplay();
}
void handleKeypress(unsigned char key, int x, int y) {
	switch (key) {
case 's'://stops
    speed = 0.0f;
    break;
case 'r'://runs
    speed = 0.02f;
    break;
glutPostRedisplay();
	}}

void update2(int value) {


    if(_moveB < -36)
    {
        _moveB = +38;
    }
     //Notify GLUT that the display has changed

_moveB -= 0.13;

    glutTimerFunc(20, update2, 0); //Notify GLUT to call update again in 25 milliseconds
    glutPostRedisplay();
}
//function for windlmill
void update3(int value) {

    _angle1-=2.0f;
    if(_angle1 > 360.0)
    {
        _angle1-=360;
    }
	glutPostRedisplay(); //Notify GLUT that the display has changed

	glutTimerFunc(10, update3, 0); //Notify GLUT to call update again in 25 milliseconds
}
void update4(int value) {


    if(_moveC > +36)
    {
        _moveC = -38;
    }
     //Notify GLUT that the display has changed

_moveC += 0.13;

    glutTimerFunc(20, update4, 0); //Notify GLUT to call update again in 25 milliseconds
    glutPostRedisplay();
}

void update5(int value) {


    if(_moveS > +36)
    {
        _moveS = -38;
    }
     //Notify GLUT that the display has changed

_moveS += 0.13;

    glutTimerFunc(20, update5, 0); //Notify GLUT to call update again in 25 milliseconds
    glutPostRedisplay();
}









int main(int argc, char** argv) {
    initCloud();
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);







    glutInitWindowSize(1500, 1000);
    glutCreateWindow("Transformation");


    init();
    glutSpecialFunc(SpecialInput);


    glutDisplayFunc(drawScene);
    glutTimerFunc(20, update1, 0); //Add a timer
    glutTimerFunc(20, update2, 0); //Add a timer

    glutTimerFunc(10, update3, 0);
    glutTimerFunc(20, updateCloudPosition, 0);
    glutTimerFunc(20, updateSunVerticalPosition, 0);

    glutKeyboardFunc(handleKeypress);

    glutMouseFunc(handleMouse);
    glutMainLoop();
    return 0;
}
void drawHair(float r, float x, float y) {
        float i = 0.0f;

        glBegin(GL_TRIANGLE_FAN);
        glColor3ub(0,0,0);

        glVertex2f(x, y); // Center
        for(i = 0.0f; i <= 360; i++)
                glVertex2f(r*cos(M_PI * i / 180.0) + x, r*sin(M_PI * i / 180.0) + y);

        glEnd();
}
void drawCircle(float r, float x, float y) {
        float i = 0.0f;

        glBegin(GL_TRIANGLE_FAN);
        glColor3ub(255,250,205);

        glVertex2f(x, y); // Center
        for(i = 0.0f; i <= 360; i++)
                glVertex2f(r*cos(M_PI * i / 180.0) + x, r*sin(M_PI * i / 180.0) + y);

        glEnd();
}
void drawEye(float r, float x, float y) {
        float i = 0.0f;

        glBegin(GL_TRIANGLE_FAN);
        glColor3ub(105,105,105);

        glVertex2f(x, y); // Center
        for(i = 0.0f; i <= 360; i++)
                glVertex2f(r*cos(M_PI * i / 180.0) + x, r*sin(M_PI * i / 180.0) + y);

        glEnd();
}
void drawMouth(){

        glBegin(GL_POLYGON);// Draw a Red 1x1 Square centered at origin
        glColor3ub(0,0,0);
        glVertex2f(-3.25f,-6.45f);
        glVertex2f(-3.25f,-6.35f);
        glVertex2f(-3.7f,-6.35f);
        glVertex2f(-3.7f,-6.45f);

        glEnd();
}
void drawBody(){

        glBegin(GL_POLYGON);// Draw a Red 1x1 Square centered at origin
        glColor3ub(255,255,255);
        glVertex2f(-3.25f,-6.7f);
        glVertex2f(-3.25f,-6.6f);
        glVertex2f(-3.7f,-6.7f);
        glVertex2f(-3.7f,-6.5f);

        glEnd();
}
void drawBody2(){

        glBegin(GL_POLYGON);// Draw a Red 1x1 Square centered at origin
        glColor3ub(255,20,147);
        glVertex2f(-2.8f,-6.8f);
        glVertex2f(-2.8f,-7.5f);
        glVertex2f(-4.2f,-7.5f);
        glVertex2f(-4.2f,-6.8f);

        glEnd();
}
void drawHumanMouth(){

        glBegin(GL_POLYGON);// Draw a Red 1x1 Square centered at origin
        glColor3ub(0,0,0);
        glVertex2f(2.8f,-2.2f);
        glVertex2f(2.8f,-2.0f);
        glVertex2f(2.1f,-2.0f);
        glVertex2f(2.1f,-2.2f);

        glEnd();
}
void drawHumanBody(){

        glBegin(GL_POLYGON);// Draw a Red 1x1 Square centered at origin
        glColor3ub(255,255,255);
        glVertex2f(2.8f,-2.8f);
        glVertex2f(2.8f,-2.5f);
        glVertex2f(2.1f,-2.5f);
        glVertex2f(2.1f,-2.8f);

        glEnd();
}
void drawHumanBody2(){

        glBegin(GL_POLYGON);// Draw a Red 1x1 Square centered at origin
        glColor3ub(205,92,92);
        glVertex2f(3.5f,-4.8f);
        glVertex2f(3.5f,-2.8f);
        glVertex2f(1.4f,-2.8f);
        glVertex2f(1.4f,-4.8f);

        glEnd();
}
void drawHumanBody3(){

        glBegin(GL_POLYGON);// Draw a Red 1x1 Square centered at origin
        glColor3ub(30,144,255);
        glVertex2f(3.4f,-3.5f);
        glVertex2f(3.4f,-2.8f);
        glVertex2f(1.5f,-2.8f);
        glVertex2f(1.5f,-3.5f);

        glEnd();
}
void drawHumanLeftLag(){

        glBegin(GL_POLYGON);// Draw a Red 1x1 Square centered at origin
        glColor3ub(255,255,255);
        glVertex2f(2.0f,-6.0f);
        glVertex2f(2.0f,-4.8f);
        glVertex2f(1.5f,-4.8f);
        glVertex2f(1.5f,-6.0f);

        glEnd();
}
void drawHumanLeftLag1(){

        glBegin(GL_POLYGON);// Draw a Red 1x1 Square centered at origin
        glColor3ub(0,0,0);
        glVertex2f(2.2f,-6.4f);
        glVertex2f(2.2f,-5.9f);
        glVertex2f(1.3f,-5.9f);
        glVertex2f(1.3f,-6.4f);

        glEnd();
}
void drawHumanRightLag(){

        glBegin(GL_POLYGON);// Draw a Red 1x1 Square centered at origin
        glColor3ub(255,255,255);
        glVertex2f(3.3f,-6.0f);
        glVertex2f(3.3f,-4.8f);
        glVertex2f(2.8f,-4.8f);
        glVertex2f(2.8f,-6.0f);

        glEnd();
}
void drawHumanRightLag1(){

        glBegin(GL_POLYGON);// Draw a Red 1x1 Square centered at origin
        glColor3ub(0,0,0);
        glVertex2f(3.5f,-6.4f);
        glVertex2f(3.5f,-5.9f);
        glVertex2f(2.6f,-5.9f);
        glVertex2f(2.6f,-6.4f);

        glEnd();
}
void drawHumanLeftHand(){

        glBegin(GL_POLYGON);// Draw a Red 1x1 Square centered at origin
        glColor3ub(205,92,92);
        glVertex2f(1.1f,-5.0f);
        glVertex2f(1.5f,-2.8f);
        glVertex2f(1.0f,-3.0f);
        glVertex2f(0.8f,-5.0f);

        glEnd();
}
void drawHumanRightHand(){

        glBegin(GL_POLYGON);// Draw a Red 1x1 Square centered at origin
        glColor3ub(205,92,92);
        glVertex2f(3.8f,-4.9f);
        glVertex2f(3.4f,-2.8f);
        glVertex2f(3.8f,-3.0f);
        glVertex2f(4.1f,-4.7f);

        glEnd();
}
void drawBag(){

        glBegin(GL_POLYGON);// Draw a Red 1x1 Square centered at origin
        glColor3ub(0,0,0);
        glVertex2f(1.3f,-5.7f);
        glVertex2f(1.3f,-5.0f);
        glVertex2f(0.3f,-5.0f);
        glVertex2f(-0.3f,-5.7f);

        glEnd();
}
void rain(){

     ///gluOrtho2D(-12,38,-19,14);//range
    int i,rx,ry;
    for(i=0;i<1400;i++)
    {
        rx=rand() % 50 - 12;
        ry=rand() % 33 - 19;
       // if(ry<GroundY-4)
        //{
        ///if(ry<GroundY-120 || (ry>GroundY-120 && (rx<x-20 || rx>x+60)))
        //line(rx,ry,rx+0.5,ry+4);
           // {
                glBegin(GL_LINES);
                glColor3ub(255,255,255);
                glVertex2f(rx,ry);
                glVertex2f(rx+.05,ry+.4);
                glEnd();
            //}
        //}
    }


}
void fish5()
{
 glPushMatrix();
    //glColor3d(1,0,0);
    //glTranslatef(_moveA, 0.0f, 0.0f);


    glBegin(GL_QUADS);
    glColor3ub(0,0,100);
        glVertex2f(16.0f, -15.0f);
        glVertex2f(14.0f, -14.0f);
        glVertex2f(12.0f, -15.0f);
        glVertex2f(14.0f, -16.0f);

    glEnd();

    glBegin(GL_TRIANGLES);
    glColor3ub(0,0,100);
        glVertex2f(11.0f, -14.5f);
        glVertex2f(13.0f, -15.0f);
        glVertex2f(11.0f, -15.7f);

    glEnd();

     glPushMatrix();
    glColor3d(255,255,255);
    glTranslatef(15.0, -15.0, 0);
    glutSolidSphere(0.16, 250, 250);
    glPopMatrix();



    glPopMatrix();

}
void fish4()
{
 glPushMatrix();
    //glColor3d(1,0,0);
    glTranslatef(_moveA, 0.0f, 0.0f);


    glBegin(GL_QUADS);
    glColor3ub(0,50,100);
        glVertex2f(-4.0f, -16.0f);
        glVertex2f(-6.0f, -15.0f);
        glVertex2f(-8.0f, -16.0f);
        glVertex2f(-6.0f, -17.0f);

    glEnd();

    glBegin(GL_TRIANGLES);
    glColor3ub(0,50,100);
        glVertex2f(-9.0f, -15.5f);
        glVertex2f(-7.0f, -16.0f);
        glVertex2f(-9.0f, -16.7f);

    glEnd();

    glPushMatrix();
    glColor3d(255,255,200);
    glTranslatef(-5.0, -16.0, 0);
    glutSolidSphere(0.16, 250, 250);
    glPopMatrix();




    glPopMatrix();

}

void fish3()
{
 glPushMatrix();
    //glColor3d(1,0,0);
    glTranslatef(_moveB, 0.0f, 0.0f);


    glBegin(GL_QUADS);
    glColor3ub(0,50,100);
        glVertex2f(31.0f, -17.0f);
        glVertex2f(29.0f, -16.0f);
        glVertex2f(27.0f, -17.0f);
        glVertex2f(29.0f, -18.0f);

    glEnd();

    glBegin(GL_TRIANGLES);
    glColor3ub(0,50,100);
        glVertex2f(32.0f, -16.5f);
        glVertex2f(30.0f, -17.0f);
        glVertex2f(32.0f, -17.7f);

    glEnd();

    glPushMatrix();
    glColor3d(255,255,200);
    glTranslatef(28.0, -17.0, 0);
    glutSolidSphere(0.16, 250, 250);
    glPopMatrix();




    glPopMatrix();

}
/*
    glBegin(GL_POLYGON);// Draw a Red 1x1 Square centered at origin
    glColor3ub(128,135,32);
    glVertex2f(-2.5f,-7.5f);
    glVertex2f(-2.5f,-5.5f);
    glVertex2f(-4.5f,-5.5f);
    glVertex2f(-4.5f,-7.5f);
*/

