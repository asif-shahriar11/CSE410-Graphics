#include <GL/glut.h>  // GLUT, include glu.h and gl.h
#include <cmath>
#include<vector>
#include<iostream>
#include<fstream>
#include "1805040_Header.h"
#include "bitmap_image.hpp"
using namespace std;

// variables
Point pos, l, r, u;

// const int stacks = 65, slices = 65;
// Point points[stacks][slices];

const int subdivision = 6;

double fov_y, aspect_ratio, near, far;
int n_rec, n_obj;
int pixels; // number of pixels along both axes
int image_width, image_height;

// for the checkerboard
double cell_width;
double ambient_coeff_board, diffuse_coeff_board, reflection_coeff_board;

vector<Object *> objects;
vector<PointLight> pointLights;
vector<SpotLight> spotLights;

bitmap_image image;
int image_count = 0;

const int window_height = 640, window_width = 640;
const int num_of_tiles = 500;

int texture = 0; // texture off by default

void loadData() {
    std::ifstream description;
    description.open("description.txt");

    Object *temp_object;

    description >> near >> far >> fov_y >> aspect_ratio;
    description >> n_rec;
    description >> pixels;
    image_width = pixels;
    image_height = pixels;
    // for the checkerboard
    description >> cell_width;
    description >> ambient_coeff_board >> diffuse_coeff_board >> reflection_coeff_board;

    // checkerboard
    temp_object = new CheckerBoard(pos, cell_width * num_of_tiles, cell_width);
    temp_object->setCoEfficients(ambient_coeff_board, diffuse_coeff_board, 0.0, reflection_coeff_board);
    objects.push_back(temp_object);

    description >> n_obj;

    string object_type;
    double ambient, diffuse, specular, reflection;
    int shininess;
    double tempX, tempY, tempZ, colorR, colorG, colorB;
    double radius, height, width, side;
    int normal_lights, point_lights;

    for(int i=0; i<n_obj; i++) {
        description >> object_type;

        if(object_type == "sphere") {
            description >> tempX >> tempY >> tempZ;
            description >> radius;
            description >> colorR >> colorG >> colorB;
            description >> ambient >> diffuse >> specular >> reflection;
            description >> shininess;

            Point center(tempX, tempY, tempZ);
            Color temp_color(colorR, colorG, colorB);
            temp_object = new Sphere(center, radius);
            temp_object->setColor(temp_color);
            temp_object->setCoEfficients(ambient, diffuse, specular, reflection);
            temp_object->setShine(shininess);

            objects.push_back(temp_object);

        }

        else if(object_type == "pyramid") {
            description >> tempX >> tempY >> tempZ;
            description >> width >> height;
            description >> colorR >> colorG >> colorB;
            description >> ambient >> diffuse >> specular >> reflection;
            description >> shininess;

            Color temp_color(colorR, colorG, colorB);

            Point a(tempX, tempY, tempZ); // lowest point
            Point b(tempX + width, tempY, tempZ);
            Point d(tempX, tempY + width, tempZ);
            Point c(tempX + width, tempY + width, tempZ);
            // center is the intersection of the line joining a and c and the line joining b and d
            Point center((a.getX() + c.getX())/2, (a.getY() + c.getY())/2, (a.getZ() + c.getZ())/2);
            Point top(center.x, center.y, center.z + height);
        


            // now we create the 6 triangles and push them to the objects' list
            // abd, bcd, top-a-b, top-b-c, top-c-d, top-d-a
            temp_object = new Triangle(a, b, d);
            temp_object->setColor(temp_color);
            temp_object->setCoEfficients(ambient, diffuse, specular, reflection);
            temp_object->setShine(shininess);
            objects.push_back(temp_object);

            temp_object = new Triangle(b, c, d);
            temp_object->setColor(temp_color);
            temp_object->setCoEfficients(ambient, diffuse, specular, reflection);
            temp_object->setShine(shininess);
            objects.push_back(temp_object);

            temp_object = new Triangle(top, a, b);
            temp_object->setColor(temp_color);
            temp_object->setCoEfficients(ambient, diffuse, specular, reflection);
            temp_object->setShine(shininess);
            objects.push_back(temp_object);

            temp_object = new Triangle(top, b, c);
            temp_object->setColor(temp_color);
            temp_object->setCoEfficients(ambient, diffuse, specular, reflection);
            temp_object->setShine(shininess);
            objects.push_back(temp_object);

            temp_object = new Triangle(top, c, d);
            temp_object->setColor(temp_color);
            temp_object->setCoEfficients(ambient, diffuse, specular, reflection);
            temp_object->setShine(shininess);
            objects.push_back(temp_object);

            temp_object = new Triangle(top, d, a);
            temp_object->setColor(temp_color);
            temp_object->setCoEfficients(ambient, diffuse, specular, reflection);
            temp_object->setShine(shininess);
            objects.push_back(temp_object);

        }

        else if(object_type == "cube") {
            description >> tempX >> tempY >> tempZ;
            description >> side;
            description >> colorR >> colorG >> colorB;
            description >> ambient >> diffuse >> specular >> reflection;
            description >> shininess;

            Color temp_color(colorR, colorG, colorB);

            // a cube is basically 6 squares - 12 triangles

            Point a(tempX, tempY, tempZ); // lowest point
            Point b(a.x + side, a.y, a.z);
            Point c(a.x + side, a.y, a.z + side);
            Point d(a.x, a.y, a.z + side);
            Point e(a.x, a.y + side, a.z);
            Point f(b.x, b.y + side, b.z);
            Point g(c.x, c.y + side, c.z);
            Point h(d.x, d.y + side, d.z);
            
            // triangles

            temp_object = new Triangle(a, b, d);
            temp_object->setColor(temp_color);
            temp_object->setCoEfficients(ambient, diffuse, specular, reflection);
            temp_object->setShine(shininess);
            objects.push_back(temp_object);

            temp_object = new Triangle(b, c, d);
            temp_object->setColor(temp_color);
            temp_object->setCoEfficients(ambient, diffuse, specular, reflection);
            temp_object->setShine(shininess);
            objects.push_back(temp_object);

            temp_object = new Triangle(e, h, a);
            temp_object->setColor(temp_color);
            temp_object->setCoEfficients(ambient, diffuse, specular, reflection);
            temp_object->setShine(shininess);
            objects.push_back(temp_object);

            temp_object = new Triangle(h, d, a);
            temp_object->setColor(temp_color);
            temp_object->setCoEfficients(ambient, diffuse, specular, reflection);
            temp_object->setShine(shininess);
            objects.push_back(temp_object);

            temp_object = new Triangle(f, g, b);
            temp_object->setColor(temp_color);
            temp_object->setCoEfficients(ambient, diffuse, specular, reflection);
            temp_object->setShine(shininess);
            objects.push_back(temp_object);

            temp_object = new Triangle(g, c, b);
            temp_object->setColor(temp_color);
            temp_object->setCoEfficients(ambient, diffuse, specular, reflection);
            temp_object->setShine(shininess);
            objects.push_back(temp_object);

            temp_object = new Triangle(e, f, h);
            temp_object->setColor(temp_color);
            temp_object->setCoEfficients(ambient, diffuse, specular, reflection);
            temp_object->setShine(shininess);
            objects.push_back(temp_object);

            temp_object = new Triangle(f, g, h);
            temp_object->setColor(temp_color);
            temp_object->setCoEfficients(ambient, diffuse, specular, reflection);
            temp_object->setShine(shininess);
            objects.push_back(temp_object);

            temp_object = new Triangle(h, g, d);
            temp_object->setColor(temp_color);
            temp_object->setCoEfficients(ambient, diffuse, specular, reflection);
            temp_object->setShine(shininess);
            objects.push_back(temp_object);

            temp_object = new Triangle(g, c, d);
            temp_object->setColor(temp_color);
            temp_object->setCoEfficients(ambient, diffuse, specular, reflection);
            temp_object->setShine(shininess);
            objects.push_back(temp_object);

            temp_object = new Triangle(e, f, a);
            temp_object->setColor(temp_color);
            temp_object->setCoEfficients(ambient, diffuse, specular, reflection);
            temp_object->setShine(shininess);
            objects.push_back(temp_object);

            temp_object = new Triangle(f, b, a);
            temp_object->setColor(temp_color);
            temp_object->setCoEfficients(ambient, diffuse, specular, reflection);
            temp_object->setShine(shininess);
            objects.push_back(temp_object);

        }
        
        else {

        }

    }


    double falloff, cutoff_angle;
    // light sources
    description >> normal_lights;

    for(int i=0; i<normal_lights; i++) {
        description >> tempX >> tempY >> tempZ;
        description >> falloff;
        PointLight temp_light = PointLight(Point(tempX, tempY, tempZ), falloff);
        pointLights.push_back(temp_light);
    }

    description >> point_lights;

    for(int i=0; i<point_lights; i++) {
        description >> tempX >> tempY >> tempZ;
        Point light_position(tempX, tempY, tempZ);
        description >> falloff;
        description >> tempX >> tempY >> tempZ;
        Point direction(tempX, tempY, tempZ);
        description >> cutoff_angle;

        SpotLight temp_light = SpotLight(light_position, direction, cutoff_angle, falloff);
        spotLights.push_back(temp_light);
    }
    


}


void capture() {
    // capture the image

    if(texture == 1) {
        cout << "Capturing image (with texture)" << endl;
    }
    else {
        cout << "Capturing image (without texture)" << endl;
    }
    

    image = bitmap_image(image_width, image_height);

    for(int i=0; i<image_width; i++) {
        for(int j=0; j<image_height; j++) {
            image.set_pixel(i, j, 0, 0, 0);
        }
    }

    double plane_distance = (window_height / 2) / tan(fov_y * M_PI / 360);

    Point top_left = pos + l * plane_distance - r * (window_width / 2) + u * (window_height / 2);

    double du = window_width * 1.0 / image_width;
    double dv = window_height * 1.0 / image_height;

    top_left = top_left + r * (du / 2.0) - u * (dv / 2.0); // middle of the grid

    int nearest_idx;
    double t, t_min;

    for(int i = 0; i<image_width; i++) {
        for(int j = 0; j<image_height; j++) {
            // current pixel
            Point current_pixel = top_left + r * (i * du) - u * (j * dv);

            // casting ray from EYE to (curPixel-eye) direction ; eye is the position of the camera
            Ray ray(pos, current_pixel - pos);
            Color color;

            // finding nearest object
            nearest_idx = -1;
            t_min = INFINITY;
            for (int k = 0; k < objects.size(); k++) {
				t = objects[k]->intersect(ray, color, 0);
				if (t > 0 && t < t_min)
				{
					t_min = t;
					nearest_idx = k;
				}
			}

            if(nearest_idx != -1) {
                t_min = objects[nearest_idx]->intersect(ray, color, 1);
                color.normalize();
                image.set_pixel(i, j, color.getR() * 255, color.getG() * 255, color.getB() * 255);
            }
            // color.normalize();
            // cout << color.getR() << " " << color.getG() << " " << color.getB() << endl;
            // image.set_pixel(i, j, color.getR() * 255, color.getG() * 255, color.getB() * 255);

            
        }
    }

    string image_Name = "Output_" + to_string(image_count) + ".bmp";
    image.save_image(image_Name);
    cout << "Image saved" << endl;
    image_count++;
    
}





/* Initialize OpenGL Graphics */
void initGL() {
    // Set "clearing" or background color
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);   // Black and opaque
    glEnable(GL_DEPTH_TEST);   // Enable depth testing for z-culling
}


/************** Draw axes: X in Red, Y in Green and Z in Blue *****************/
void drawAxes() {
    glLineWidth(3);
    glBegin(GL_LINES);
        glColor3f(1,0,0);   // Red
        // X axis
        glVertex3f(-10,0,0);
        glVertex3f(10,0,0);

        glColor3f(0,1,0);   // Green
        // Y axis
        glVertex3f(0,-10,0);
        glVertex3f(0,10,0);

        glColor3f(0,0,1);   // Blue
        // Z axis
        glVertex3f(0,0,-10);
        glVertex3f(0,0,10);
    glEnd();
}





void keyboardListener(unsigned char key, int xx,int yy){
    double rate = 0.05;

	switch(key){
        
        case '0':
            capture();
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

        case '9':
            texture = 1 - texture;
            if(texture == 1) {
                cout << "Texture on" << endl;
            }
            else {
                cout << "Texture off" << endl;
            }
            break;

		default:
			break;
	}
	glutPostRedisplay();
}


void specialKeyListener(int key, int x,int y)
{
	switch(key){
        
        case GLUT_KEY_UP:		//down arrow key
			pos.x+=l.x;
			pos.y+=l.y;
			pos.z+=l.z;
			break;
		case GLUT_KEY_DOWN:		// up arrow key
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


void display() {
    // clear the display
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClearColor(0, 0, 0, 0); // color black
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	/********************
	/ set-up camera here
	********************/
	// load the correct matrix -- MODEL-VIEW matrix
	glMatrixMode(GL_MODELVIEW);

	// initialize the matrix
	glLoadIdentity();



	gluLookAt(pos.x, pos.y, pos.z,
			  pos.x + l.x, pos.y + l.y, pos.z + l.z,
			  u.x, u.y, u.z);

	// again select MODEL-VIEW
	glMatrixMode(GL_MODELVIEW);

	//drawAxes();

	//for (Object *each : objects)    each->draw();
	

    for(int i=0; i<objects.size(); i++) {
        objects[i]->draw();
    }

	// //for (PointLight light : pointLights) light.draw();

    for(int i=0; i<pointLights.size(); i++) {
        pointLights[i].draw();
    }
	

	// //for (SpotLight light : spotLights) light.draw();

    for(int i=0; i<spotLights.size(); i++) {
        spotLights[i].draw();
    }
	

	glutSwapBuffers();
}

void animate()
{
	// codes for any changes in Models, Camera
	glutPostRedisplay();
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



/* Main function: GLUT runs as a console application starting at main()  */
int main(int argc, char** argv) {

    // camera setup
    pos.x = 0; 
    pos.y= -200; 
    pos.z = 50;

    l.x = 0; 
    l.y = 1; 
    l.z = 0;
    
    u.x = 0; 
    u.y = 0;
    u.z = 1;
    
    r.x = 1; 
    r.y = 0; 
    r.z = 0;




    loadData();


    glutInit(&argc, argv);                  // Initialize GLUT
    glutInitWindowSize(window_width, window_height);           // Set the window's initial width & height
    glutInitWindowPosition(50, 50);         // Position the window's initial top-left corner
	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGB);	//Depth, Double buffer, RGB color
    glutCreateWindow("1805040_ray_tracing");          // Create a window with the given title


    // clearing the screen
	glClearColor(0,0,0,0);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(fov_y, aspect_ratio, near, far);

    glEnable(GL_DEPTH_TEST);	//enable Depth Testing

    glutDisplayFunc(display);               // Register display callback handler for window re-paint
    //glutReshapeFunc(reshape);               // Register callback handler for window re-shape
    glutIdleFunc(animate);	

	glutKeyboardFunc(keyboardListener);
	glutSpecialFunc(specialKeyListener);

    //initGL();                               // Our own OpenGL initialization
    glutMainLoop();                         // Enter the event-processing loop

    // cleanup
    for (int i=0; i < objects.size(); i++) {
		delete objects[i];
	}
	vector<Object*>().swap(objects);
	vector<PointLight>().swap(pointLights);
	vector<SpotLight>().swap(spotLights);

    return 0;
}
