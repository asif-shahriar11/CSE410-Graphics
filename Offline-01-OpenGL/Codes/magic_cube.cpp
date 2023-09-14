#include <GL/glut.h>  // GLUT, include glu.h and gl.h
#include <cmath>
#include<vector>
#include<iostream>

struct point {
    GLfloat x, y, z;
};

// Global variables

bool isAxes = false;

struct point pos;   // position of the eye
struct point l;     // look/forward direction
struct point r;     // right direction
struct point u;     // up direction

// scales
GLfloat octTranslateFactor = 0.0f;
GLfloat octScaleFactor = 1.0f;

// sphere radius
GLfloat maxSphereRadius = 1/sqrt(3); // 0.57735 / 10 = 0.057735
GLfloat sphereRadius = 0.0f; // initial

GLfloat dihedralAngle = 109.47;
GLfloat cylinderAngle = 180.0 - dihedralAngle;

const int stacks = 65, slices = 65;
struct point points[stacks][slices];

const int subdivision = 6;

// for rotation
GLfloat rotationAngle = 0.0f;


/* Initialize OpenGL Graphics */
void initGL() {
    // Set "clearing" or background color
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);   // Black and opaque
    glEnable(GL_DEPTH_TEST);   // Enable depth testing for z-culling
}


void drawTriangle() {
    glBegin(GL_TRIANGLES);  // Each set of 3 vertices form a triangle
        glVertex3f(1.0f, 0.0f, 0.0f); // x
        glVertex3f(0.0f, 1.0f, 0.0f); // y
        glVertex3f(0.0f, 0.0f, 1.0f); // z
    glEnd();

}

void drawQuad(point p1, point p2, point p3, point p4) {
    glBegin(GL_QUADS);
        glVertex3f(p1.x, p1.y, p1.z);
        glVertex3f(p2.x, p2.y, p2.z);
        glVertex3f(p3.x, p3.y, p3.z);
        glVertex3f(p4.x, p4.y, p4.z);
    glEnd();
}


void drawOctFace() {
    glPushMatrix();
        octTranslateFactor = (1 - octScaleFactor) / 3.0;
        glTranslatef(octTranslateFactor, octTranslateFactor, octTranslateFactor);
        glScalef(octScaleFactor, octScaleFactor, octScaleFactor);
        drawTriangle();
    glPopMatrix();
}


void drawOctahedron() {
    // upper
    glColor3f(0.0f,1.0f,1.0f);  // Cyan
    drawOctFace();
    
    glPushMatrix(); // 

        glColor3f(1.0f, 0.0f, 1.0f);     // Magenta
        glRotatef(90, 0,1,0); 
        drawOctFace();


        glColor3f(0.0f,1.0f,1.0f);  // Cyan
        glRotatef(90, 0,1,0); 
        drawOctFace();

        glColor3f(1.0f, 0.0f, 1.0f);     // Magenta
        glRotatef(90, 0,1,0); 
        drawOctFace();

        // lower

        glColor3f(1.0f,0.0f,1.0f);  // Magenta
        glRotatef(180, 1,0,0); 
        drawOctFace();

        glColor3f(0.0f,1.0f,1.0f);  // Cyan
        glRotatef(90, 0,1,0); 
        drawOctFace();

        glColor3f(1.0f, 0.0f, 1.0f);     // Magenta
        glRotatef(90,0,1,0); 
        drawOctFace();

        glColor3f(0.0f,1.0f,1.0f);  // Cyan
        glRotatef(90, 0,1,0); 
        drawOctFace();

    glPopMatrix();
}



/*********************** sphere ************************/


// generate vertices for +X face only by intersecting 2 circular planes
// (longitudinal and latitudinal) at the given longitude/latitude angles
std::vector<point> buildUnitPositiveX(int subdivision)
{
    const float DEG2RAD = acos(-1) / 180.0f;

    std::vector<point> vertices;
    float n1[3];        // normal of longitudinal plane rotating along Y-axis
    float n2[3];        // normal of latitudinal plane rotating along Z-axis
    float v[3];         // direction vector intersecting 2 planes, n1 x n2
    point p;
    float a1;           // longitudinal angle along Y-axis
    float a2;           // latitudinal angle along Z-axis

    // compute the number of vertices per row, 2^n + 1
    int pointsPerRow = (int)pow(2, subdivision) + 1;

    // rotate latitudinal plane from 45 to -45 degrees along Z-axis (top-to-bottom)
    for(unsigned int i = 0; i < pointsPerRow; ++i)
    {
        // normal for latitudinal plane
        // if latitude angle is 0, then normal vector of latitude plane is n2=(0,1,0)
        // therefore, it is rotating (0,1,0) vector by latitude angle a2
        a2 = DEG2RAD * (45.0f - 90.0f * i / (pointsPerRow - 1));
        n2[0] = -sin(a2);
        n2[1] = cos(a2);
        n2[2] = 0;

        // rotate longitudinal plane from -45 to 45 along Y-axis (left-to-right)
        for(unsigned int j = 0; j < pointsPerRow; ++j)
        {
            // normal for longitudinal plane
            // if longitude angle is 0, then normal vector of longitude is n1=(0,0,-1)
            // therefore, it is rotating (0,0,-1) vector by longitude angle a1
            a1 = DEG2RAD * (-45.0f + 90.0f * j / (pointsPerRow - 1));
            n1[0] = -sin(a1);
            n1[1] = 0;
            n1[2] = -cos(a1);

            // find direction vector of intersected line, n1 x n2
            v[0] = n1[1] * n2[2] - n1[2] * n2[1];
            v[1] = n1[2] * n2[0] - n1[0] * n2[2];
            v[2] = n1[0] * n2[1] - n1[1] * n2[0];

            p.x = n1[1] * n2[2] - n1[2] * n2[1];
            p.y = n1[2] * n2[0] - n1[0] * n2[2];
            p.z = n1[0] * n2[1] - n1[1] * n2[0];

            // normalize direction vector
            float scale;
            scale = sphereRadius / sqrt(v[0]*v[0] + v[1]*v[1] + v[2]*v[2]);
            v[0] *= scale;
            v[1] *= scale;
            v[2] *= scale;
            scale = sphereRadius / sqrt(p.x * p.x + p.y * p.y + p.z * p.z);
            p.x *= scale;
            p.y *= scale;
            p.z *= scale;

            // add a vertex into array
            // vertices.push_back(v[0]);
            // vertices.push_back(v[1]);
            // vertices.push_back(v[2]);
            vertices.push_back(p);
            points[i][j].x = v[0];
            points[i][j].y = v[1];
            points[i][j].z = v[2];
        }
    }

    return vertices;
}


void drawSphereFace() {
    
    std::vector<point> points = buildUnitPositiveX(subdivision);

    int numOfPoints = pow(2, subdivision) + 1; // 65

    for(int i=0; i < numOfPoints - 1; i++) {
        for(int j=0; j < numOfPoints - 1; j++) {
            int idx = i * (numOfPoints - 1) + j + i;
            point p1 = points[idx];
            point p2 = points[idx + 1];
            point p3 = points[idx + numOfPoints + 1];
            point p4 = points[numOfPoints];
            
            drawQuad(p1, p2, p3, p4);
        
        }
    }

}


void drawAdjustedSphereFace() {

    glPushMatrix();
        glTranslatef(octScaleFactor, 0.0, 0.0);
        sphereRadius = maxSphereRadius * (1 - octScaleFactor);
        drawSphereFace();
    glPopMatrix(); 
}


void drawSphere() {

    glColor3f(0.0f, 0.0f, 1.0f); // blue +x
    drawAdjustedSphereFace();

    glPushMatrix(); 

        glColor3f(0.0f, 1.0f, 0.0f);     // green -z
        glRotatef(90, 0,1,0); 
        drawAdjustedSphereFace();

        glColor3f(0.0f, 0.0f, 1.0f);   // blue -x
        glRotatef(90, 0,1,0);
        drawAdjustedSphereFace();

        
        glRotatef(90, 0,1,0); 
        glColor3f(0.0f, 1.0f, 0.0f);     // green +z
        drawAdjustedSphereFace();

        // lower

        glColor3f(1.0f,0.0f,0.0f);  // red +y
        glRotatef(90, 0,0,1); 
        drawAdjustedSphereFace();

        glColor3f(1.0f,0.0f,0.0f);  // red -y
        glRotatef(180, 0,0,1); 
        drawAdjustedSphereFace();



    glPopMatrix();


}



/****************** cylinder *********************/


void drawCylinderSurface(double height, double radius, int segments) {
    double tempx = radius, tempy = 0;
    double currx, curry;
    glBegin(GL_QUADS);
        for (int i = 1; i <= segments; i++) {
            double theta = i * cylinderAngle * M_PI / segments;
            currx = radius * cos(theta);
            curry = radius * sin(theta);

            // GLfloat c = (2+cos(theta))/3;
            // glColor3f(c,c,c);
            glVertex3f(currx, curry, height/2);
            glVertex3f(currx, curry, -height/2);

            glVertex3f(tempx, tempy, -height/2);
            glVertex3f(tempx, tempy, height/2);

            tempx = currx;
            tempy = curry;
        }
    glEnd();
}


void drawSingleCylinder() {

    glPushMatrix();
        GLfloat cylinderRadius = maxSphereRadius * (1 - octScaleFactor); // radius = sphere radius
        GLfloat cylinderHeight = sqrt(2) * (octScaleFactor); // height = scaled triangle arm
        GLfloat displace = 1/sqrt(2) * octScaleFactor;
        //std::cout << sphereRadius << std::endl;
        glRotatef(-45, 0, 1, 0);
        glTranslatef(displace, 0, 0);
        drawCylinderSurface(cylinderHeight, cylinderRadius, 10000);
    glPopMatrix();
    
}

void drawFourCylinders() {
    
    drawSingleCylinder();

    glPushMatrix();
        
        glRotatef(90, 0, 1, 0);
        drawSingleCylinder();

        glRotatef(90, 0, 1, 0);
        drawSingleCylinder();

        glRotatef(90, 0, 1, 0);
        drawSingleCylinder();

    glPopMatrix();
}

void drawCylinder() {

    glColor3f(1.0f, 1.0f, 0.0f); 
    drawFourCylinders();
    
    glPushMatrix();

        glRotatef(-90, 1, 0, 0);
        drawFourCylinders();

        glRotatef(90, 0, 0, 1);
        drawFourCylinders();

    glPopMatrix();

}


/************** Draw axes: X in Red, Y in Green and Z in Blue *****************/
void drawAxes() {
    glLineWidth(3);
    glBegin(GL_LINES);
        glColor3f(1,0,0);   // Red
        // X axis
        glVertex3f(-2,0,0);
        glVertex3f(2,0,0);

        glColor3f(0,1,0);   // Green
        // Y axis
        glVertex3f(0,-2,0);
        glVertex3f(0,2,0);

        glColor3f(0,0,1);   // Blue
        // Z axis
        glVertex3f(0,0,-2);
        glVertex3f(0,0,2);
    glEnd();
}



void display() {
    // glClear(GL_COLOR_BUFFER_BIT);            // Clear the color buffer (background)
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);             // To operate on Model-View matrix
    glLoadIdentity();                       // Reset the model-view matrix

    // default arguments of gluLookAt
    // gluLookAt(0,0,0, 0,0,-100, 0,1,0);

    // control viewing (or camera)
    gluLookAt(pos.x,pos.y,pos.z,
              pos.x+l.x,pos.y+l.y,pos.z+l.z,
              u.x,u.y,u.z);

    // draw

    if (isAxes) drawAxes();

    //glPushMatrix();
        glRotatef(rotationAngle, 0, 1, 0);
        drawOctahedron();
        drawSphere();
        drawCylinder();
    //glPopMatrix();
    
    



    glutSwapBuffers();  // Render now
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
    /*if (width >= height) {
        // aspect >= 1, set the height from -1 to 1, with larger width
        gluOrtho2D(-1.0 * aspect, 1.0 * aspect, -1.0, 1.0);
    } else {
        // aspect < 1, set the width to -1 to 1, with larger height
        gluOrtho2D(-1.0, 1.0, -1.0 / aspect, 1.0 / aspect);
    }*/
    // Enable perspective projection with fovy, aspect, zNear and zFar
    gluPerspective(45.0f, aspect, 0.1f, 100.0f);
}

void keyboardListener(unsigned char key, int xx,int yy){
    double rate = 0.05;

	switch(key){
        
        case ',': // sphere ++, octahadron --
        if(octScaleFactor > 0.0) {
            octScaleFactor -= 0.1;
            //sphereScaleFactor += 0.1;
        }
        break;

        case '.': // sphere --, octahadron ++
            if(octScaleFactor <= 0.9) {
                octScaleFactor += 0.1;
                //sphereScaleFactor -= 0.1;
            }            
            break;

        case 'a': // clockwise rotation
            rotationAngle -= 15.0;          
            break;

        case 'd': // counter clockwise rotation
            rotationAngle += 15.0;             
            break;
        
        case '1':
			r.x = r.x*cos(rate)+l.x*sin(rate);
			r.y = r.y*cos(rate)+l.y*sin(rate);
			r.z = r.z*cos(rate)+l.z*sin(rate);

			l.x = l.x*cos(rate)-r.x*sin(rate);
			l.y = l.y*cos(rate)-r.y*sin(rate);
			l.z = l.z*cos(rate)-r.z*sin(rate);
			break;

        case '2':
			r.x = r.x*cos(-rate)+l.x*sin(-rate);
			r.y = r.y*cos(-rate)+l.y*sin(-rate);
			r.z = r.z*cos(-rate)+l.z*sin(-rate);

			l.x = l.x*cos(-rate)-r.x*sin(-rate);
			l.y = l.y*cos(-rate)-r.y*sin(-rate);
			l.z = l.z*cos(-rate)-r.z*sin(-rate);
			break;

        case '3':
			l.x = l.x*cos(rate)+u.x*sin(rate);
			l.y = l.y*cos(rate)+u.y*sin(rate);
			l.z = l.z*cos(rate)+u.z*sin(rate);

			u.x = u.x*cos(rate)-l.x*sin(rate);
			u.y = u.y*cos(rate)-l.y*sin(rate);
			u.z = u.z*cos(rate)-l.z*sin(rate);
			break;

        case '4':
			l.x = l.x*cos(-rate)+u.x*sin(-rate);
			l.y = l.y*cos(-rate)+u.y*sin(-rate);
			l.z = l.z*cos(-rate)+u.z*sin(-rate);

			u.x = u.x*cos(-rate)-l.x*sin(-rate);
			u.y = u.y*cos(-rate)-l.y*sin(-rate);
			u.z = u.z*cos(-rate)-l.z*sin(-rate);
			break;

        case '5':
			u.x = u.x*cos(rate)+r.x*sin(rate);
			u.y = u.y*cos(rate)+r.y*sin(rate);
			u.z = u.z*cos(rate)+r.z*sin(rate);

			r.x = r.x*cos(rate)-u.x*sin(rate);
			r.y = r.y*cos(rate)-u.y*sin(rate);
			r.z = r.z*cos(rate)-u.z*sin(rate);
			break;

        case '6':
			u.x = u.x*cos(-rate)+r.x*sin(-rate);
			u.y = u.y*cos(-rate)+r.y*sin(-rate);
			u.z = u.z*cos(-rate)+r.z*sin(-rate);

			r.x = r.x*cos(-rate)-u.x*sin(-rate);
			r.y = r.y*cos(-rate)-u.y*sin(-rate);
			r.z = r.z*cos(-rate)-u.z*sin(-rate);
			break;

		default:
			break;
	}
	glutPostRedisplay();
}


void specialKeyListener(int key, int x,int y)
{
	switch(key){
        
        case GLUT_KEY_UP:
			pos.x+=l.x;
			pos.y+=l.y;
			pos.z+=l.z;
			break;
		case GLUT_KEY_DOWN:	
			pos.x-=l.x;
			pos.y-=l.y;
			pos.z-=l.z;
			break;

		case GLUT_KEY_RIGHT:
			pos.x+=r.x;
			pos.y+=r.y;
			pos.z+=r.z;
			break;
		case GLUT_KEY_LEFT :
			pos.x-=r.x;
			pos.y-=r.y;
			pos.z-=r.z;
			break;

		case GLUT_KEY_PAGE_UP:
		    pos.x+=u.x;
			pos.y+=u.y;
			pos.z+=u.z;
			break;
		case GLUT_KEY_PAGE_DOWN:
            pos.x-=u.x;
			pos.y-=u.y;
			pos.z-=u.z;
			break;

		default:
			break;
	}
	glutPostRedisplay();
}





/* Main function: GLUT runs as a console application starting at main()  */
int main(int argc, char** argv) {

    // camera position
    pos.x = 0.5; 
    pos.y = 0.5; 
    pos.z = -5;

    l.x = 0;
    l.y = 0;
    l.z = 1;
    
    u.x = 0;
    u.y = 1;
    u.z = 0;
    
    r.x = 1;
    r.y = 0;
    r.z = 0;

    glutInit(&argc, argv);                  // Initialize GLUT
    glutInitWindowSize(640, 640);           // Set the window's initial width & height
    glutInitWindowPosition(50, 50);         // Position the window's initial top-left corner
	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGB);	//Depth, Double buffer, RGB color
    glutCreateWindow("1805040_Magic_Cube");          // Create a window with the given title
    glutDisplayFunc(display);               // Register display callback handler for window re-paint
    glutReshapeFunc(reshape);               // Register callback handler for window re-shape

	glutKeyboardFunc(keyboardListener);
	glutSpecialFunc(specialKeyListener);

    initGL();                               // Our own OpenGL initialization
    glutMainLoop();                         // Enter the event-processing loop
    return 0;
}

