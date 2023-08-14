#include <GL/glut.h>  // GLUT, include glu.h and gl.h
#include <cmath>
#include <time.h>
#include <sys/timeb.h>

// Global variables
GLfloat angleHour = 0.0f;   
GLfloat angleMin = 0.0f;
GLfloat angleSec = 0.0f;
GLfloat cx = 0.0f, cy = 0.0f, rInner = 0.3f, rInner2 = 0.32f, rOuter = 0.4f, rOuter2 = 0.38f; // for circles
GLfloat rCenter = 0.015f;
GLfloat rBorder1 = 0.5f, rBorder2 = 1.0f;
GLfloat handLenSec = 0.25f;
GLfloat handLenMin = 0.20f;
GLfloat handLenHour = 0.16f;
GLfloat anglePendulum = 0.0f;
GLfloat anglePendulumMax = 45.0f;
GLfloat freq = M_PI;
GLfloat timeP = 0.0f;




/* Initialize OpenGL Graphics */
void initGL() {
    // Set "clearing" or background color
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f); // Black and opaque
}

void drawBigMark() {
    glBegin(GL_LINES);  
        // glColor3f(1.0f,1.0f,1.0f);  // White
        glColor3f(0.0f,1.0f,1.0f);  // cyan
        glVertex2d(0.0f, rInner - rInner * 1.0 / 3);
        glVertex2d( 0.0f, rInner);
    glEnd();
}

void drawSmallMark() {
    glBegin(GL_LINES);  
        // glColor3f(1.0f,1.0f,1.0f);  // White
        glColor3f(0.0f,1.0f,1.0f);  // cyan
        glVertex2d(0.0f, rInner - rInner * 1.0 / 6);
        glVertex2d( 0.0f, rInner);
    glEnd();
}

void drawSmallestMark() {
    glBegin(GL_LINES);  
        glColor3f(0.0f,1.0f,1.0f);  // cyan
        glVertex2d(0.0f, rInner - rInner * 1.0 / 10);
        glVertex2d( 0.0f, rInner);
    glEnd();
}

void drawBigMarks() {
    glColor3f(0.0f,1.0f,1.0f);  // Cyan
    drawBigMark();
    // for (int i=0; i<4; i++) {
    //     glPushMatrix(); // Create a new scope
    //         glRotatef(90, 0,0,1);
    //         //glTranslatef(0.0,0.2,0.0);
    //         drawBigMark();
    //     glPopMatrix();
    // }
    glPushMatrix(); // Create a new scope
        for(int i=1; i<4; i++) {
            glRotatef(90, 0,0,1);
            //glTranslatef(0.0,0.2,0.0);
            drawBigMark();
        }
    glPopMatrix();
}

void drawSmallMarks() {
    glColor3f(0.0f,1.0f,1.0f);  // Cyan
    drawSmallMark();
    glPushMatrix(); // Create a new scope
        for(int i=1; i<12; i++) {
            glRotatef(30, 0,0,1);
            //glTranslatef(0.0,0.2,0.0);
            drawSmallMark();
        }
    glPopMatrix();
}

void drawSmallestMarks() {
    glColor3f(0.0f,1.0f,1.0f);  // Cyan
    drawSmallestMark();
    glPushMatrix(); // Create a new scope
        for(int i=1; i<60; i++) {
            glRotatef(6, 0,0,1);
            //glTranslatef(0.0,0.2,0.0);
            drawSmallestMark();
        }
    glPopMatrix();
}

// clock interface
void drawSkeleton() {
    
    glLineWidth(1.0);
    
    // center circle
    glBegin(GL_POLYGON);    // All vertices form a single polygon
        glColor3f(1.0f,0.0f,1.0f);  // Orange
        cx = 0.0;
        cy = 0.0;
        for (float theta = 0; theta < 360; theta += 10) {
            float x = cx + rCenter * cos(theta/180*M_PI);
            float y = cy + rCenter * sin(theta/180*M_PI);
            glVertex2f(x,y);
        }
    glEnd();

    // inner circle
    glBegin(GL_LINE_LOOP);  // All vertices form a single loop of single pixel width
        glColor3f(1.0f,0.0f,0.0f); 
        for (float theta = 0; theta < 360; theta += 10) {
            float x = cx + rInner * cos(theta/180*M_PI);
            float y = cy + rInner * sin(theta/180*M_PI);
            glVertex2f(x,y);
        }

    glEnd();

    glBegin(GL_LINE_LOOP);  // All vertices form a single loop of single pixel width
        glColor3f(1.0f,0.0f,0.0f); 
        for (float theta = 0; theta < 360; theta += 10) {
            float x = cx + rInner2 * cos(theta/180*M_PI);
            float y = cy + rInner2 * sin(theta/180*M_PI);
            glVertex2f(x,y);
        }

    glEnd();

    // outer circle
    glBegin(GL_LINE_LOOP);  // All vertices form a single loop of single pixel width
        glColor3f(0.0f,1.0f,1.0f); 
        for (float theta = 0; theta < 360; theta += 10) {
            float x = cx + rOuter2 * cos(theta/180*M_PI);
            float y = cy + rOuter2 * sin(theta/180*M_PI);
            glVertex2f(x,y);
        }

    glEnd();

    glBegin(GL_LINE_LOOP);  // All vertices form a single loop of single pixel width
        glColor3f(0.0f,1.0f,1.0f); 
        for (float theta = 0; theta < 360; theta += 10) {
            float x = cx + rOuter * cos(theta/180*M_PI);
            float y = cy + rOuter * sin(theta/180*M_PI);
            glVertex2f(x,y);
        }

    glEnd();

    // border
    glBegin(GL_LINE_LOOP);  // All vertices form a single loop of single pixel width

        glColor3f(1.0f,0.0f,1.0f); 
        glVertex2d(rBorder1, rBorder1);
        glVertex2d(rBorder1, -1.0f*rBorder1);
        glVertex2d(0.0f, -1.0f*rBorder2);
        glVertex2d(-1.0f*rBorder1, -1.0f*rBorder1);
        glVertex2d(-1.0f*rBorder1, rBorder1);
    glEnd();
}

// hands
void drawHands() {
    GLfloat angleTemp;
    // second
    glLineWidth(1.0);
    angleTemp = -1.0 * angleSec + M_PI / 2;
	glBegin(GL_LINES);
	    glColor3f(0.0f, 1.0f, 1.0f);
        glVertex2f(0.0f, 0.0f);
        glColor3f(1.0f, 0.0f, 1.0f);
        glVertex2f(handLenSec * cos(angleTemp) , handLenSec * sin(angleTemp));
	glEnd();

    // minute
    glLineWidth(2.0);
    angleTemp = -1.0 * angleMin + M_PI / 2;
	glBegin(GL_LINES);
	    glColor3f(0.0f, 0.0f, 1.0f);
        glVertex2f(0.0f, 0.0f);
        glColor3f(1.0f, 0.0f, 0.0f);
        glVertex2f(handLenMin * cos(angleTemp) , handLenMin * sin(angleTemp));
	glEnd();

    // hour
    glLineWidth(3.0);
    angleTemp = -1.0 * angleHour + M_PI / 2;
	glBegin(GL_LINES);
	    glColor3f(1.0f, 0.0f, 0.0f);
        glVertex2f(0.0f, 0.0f);
        glColor3f(0.0f, 0.0f, 1.0f);
        glVertex2f(handLenHour * cos(angleTemp) , handLenHour * sin(angleTemp));
	glEnd();

}

void drawSecondHand() {
    GLfloat angleTemp = -1.0 * angleSec + M_PI / 2;
    // second
    glLineWidth(1.0);
    glPushMatrix();
        glRotatef(-1.0 * angleTemp, 1,0,0);
        glBegin(GL_LINES);
            glColor3f(0.0f, 1.0f, 1.0f);
            glVertex2f(0.0f, 0.0f);
            glColor3f(1.0f, 0.0f, 1.0f);
            glVertex2f(0.0f, handLenSec);
	    glEnd();
    glPopMatrix();
}

void drawMinuteHand() {
    GLfloat angleTemp = -1.0 * angleMin + M_PI / 2;
    // second
    glLineWidth(3.0);
    glPushMatrix();
        glRotatef(-1.0 * angleTemp, 0,0,1);
        glBegin(GL_LINES);
            glColor3f(0.0f, 0.0f, 1.0f);
            glVertex2f(0.0f, 0.0f);
            glColor3f(1.0f, 0.0f, 0.0f);
            glVertex2f(0.0f, handLenMin);
	    glEnd();
    glPopMatrix();
}

void drawHourHand() {
    GLfloat angleTemp = -1.0 * angleHour + M_PI / 2;
    // second
    glLineWidth(5.0);
    glPushMatrix();
        glRotatef(-1.0 * angleTemp, 0,0,1);
        glBegin(GL_LINES);
            glColor3f(1.0f, 0.0f, 0.0f);
            glVertex2f(0.0f, 0.0f);
            glColor3f(0.0f, 0.0f, 1.0f);
            glVertex2f(0.0f, handLenHour);
	    glEnd();
    glPopMatrix();
}

void drawAllHands() {
    drawSecondHand();
    drawMinuteHand();
    drawHourHand();
}



void drawPendulum() {

    glPushMatrix(); // Create a new scope
        glRotatef(-1.0 * anglePendulum, 0,0,1);
        glTranslatef(0.0,0.4,0.0);

        // small circle
        GLfloat cx, cy, r;
        glBegin(GL_POLYGON);    // All vertices form a single polygon
            glColor3f(1.0f,0.0f,0.0f);  // Orange
            cx = 0.0;
            cy = -0.4;
            r = 0.025;
            for (float theta = 0; theta < 360; theta += 10) {
                float x = cx + r * cos(theta/180*M_PI);
                float y = cy + r * sin(theta/180*M_PI);
                glVertex2f(x,y);
            }
        glEnd();
        // hand
        glLineWidth(10.0);
        glBegin(GL_LINES);
            glColor3f(1.0f, 0.0f, 0.0f);
            glVertex2f(0.0f, -0.4f);
            glColor3f(0.0f, 0.0f, 1.0f);
            glVertex2f(0.0f, -0.765f);
        glEnd();
        // big circle
        glBegin(GL_POLYGON);    // All vertices form a single polygon
            glColor3f(0.0f,0.0f,1.0f);  // Orange
            cx = 0.0;
            cy = -0.765;
            r = 0.05;
            for (float theta = 0; theta < 360; theta += 10) {
                float x = cx + r * cos(theta/180*M_PI);
                float y = cy + r * sin(theta/180*M_PI);
                glVertex2f(x,y);
            }
        glEnd();

    glPopMatrix();
    
    
}


// timer
void redraw(int value) {
	struct timeb tb;
	time_t tim = time(0);
	struct tm* t;
	t = localtime(&tim); 
	ftime(&tb);
    

	angleSec = (double)(t->tm_sec + (double)tb.millitm / 1000.0) * M_PI / 30.0;
	angleMin = (double)t->tm_min * M_PI / 30.0 + angleSec / 60.0;
	angleHour = (double)(t->tm_hour > 12 ? t->tm_hour-12 : t->tm_hour)/6.0 * M_PI+ angleMin/12.0;

    if(t->tm_hour > 12) angleHour = (double) (t->tm_hour-12) * M_PI / 6.0 + angleMin / 12.0;
    else angleHour = (double) (t->tm_hour) * M_PI / 6.0 + angleMin / 12.0;

    // pendulum
    anglePendulum = anglePendulumMax * cos(freq*timeP); 
    timeP = timeP + 1.0 / 100;

	glutPostRedisplay();
	glutTimerFunc(10 , redraw , 1);
}



void display() {
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f); // Set background color to black and opaque
    glClear(GL_COLOR_BUFFER_BIT);  // Clear the color buffer

    drawSkeleton();

    drawBigMarks();
    drawSmallMarks();
    drawSmallestMarks();

    drawHands();

    glPushMatrix();
        glTranslatef(0.0,-0.4,0.0);
        drawPendulum();
    glPopMatrix();

    glFlush();  // Render now
}


/* Handler for window re-size event. Called back when the window first appears and
   whenever the window is re-sized with its new width and height */
void reshape(GLsizei width, GLsizei height) {  // GLsizei for non-negative integer
    // Compute aspect ratio of the new window
    if (height == 0) height = 1;                // To prevent divide by 0
    GLfloat aspect = (GLfloat)width / (GLfloat)height;

    // Set the viewport to cover the new window
    glViewport(0, 0, width, height);

    // Set the aspect ratio of the clipping area to match the viewport
    glMatrixMode(GL_PROJECTION);  // To operate on the Projection matrix
    glLoadIdentity();             // Reset the projection matrix
    if (width >= height) {
        // aspect >= 1, set the height from -1 to 1, with larger width
        gluOrtho2D(-1.0 * aspect, 1.0 * aspect, -1.0, 1.0);
    } else {
        // aspect < 1, set the width to -1 to 1, with larger height
        gluOrtho2D(-1.0, 1.0, -1.0 / aspect, 1.0 / aspect);
    }
}



/* Main function: GLUT runs as a console application starting at main() */
int main(int argc, char** argv) {
    glutInit(&argc, argv);                  // Initialize GLUT
    glutInitWindowSize(640, 480);           // Set the window's initial width & height - non-square
    glutInitWindowPosition(50, 50);         // Position the window's initial top-left corner
    glutCreateWindow("1805040_clock"); // Create window with the given title
    glutDisplayFunc(display);               // Register callback handler for window re-paint event
    glutReshapeFunc(reshape);               // Register callback handler for window re-size event
    initGL(); 
    glutTimerFunc(10 , redraw , 1);                         
    glutMainLoop();                         // Enter the infinite event-processing loop
    return 0;
}
