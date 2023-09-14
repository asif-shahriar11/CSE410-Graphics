#ifndef _1805040_HEADER_H
#define _1805040_HEADER_H

#include<iostream>
#include<cmath>
#include<fstream>
#include <iomanip> // required for std::setprecision
#include<vector>
#include <GL/glut.h>
#include "bitmap_image.hpp"
using namespace std;


class Point;
class Matrix;
class Color;
class Ray;
class PointLight;
class SpotLight;
class Object;
class Triangle;
class Sphere;
class CheckerBoard;

extern int n_rec;

extern vector<Object *> objects;
extern vector<PointLight> pointLights;
extern vector<SpotLight> spotLights;

extern int texture;

#define N_STACKS 	100
#define N_SLICES 	100

class Point {
    // also serves as a vector
    
public:
    double x;
    double y;
    double z;
    double w;

    Point() {
        x = 0.0;
        y = 0.0;
        z = 0.0;
        w = 1.0;
    }

    Point(double x, double y, double z, double w = 1.0) {
        this->x = x;
        this->y = y;
        this->z = z;
        this->w = w;
    }

    void setX(double x) { this->x = x; }

    void setY(double y) { this->y = y; }

    void setZ(double z) { this->z = z; }

    void setW(double w) { this->w = w; }

    double getX() { return x; }

    double getY() { return y; }

    double getZ() { return z; }

    double getW() { return w; }

    void print() {
        cout << x << " " << y << " " << z << endl;
    }

    double get_magnitude() {
        return sqrt(x * x + y * y + z * z);
    }

    // print in a file
    void print(ofstream &file) {
        //file << x << " " << y << " " << z << endl;
        // print x, y, z in the file with a precision of 6 digits after the end of decimal
        file << std::fixed << std::setprecision(7) << x << " " << y << " " << z << endl;
    }

    void scale() {
        x = x / w;
        y = y / w;
        z = z / w;
        w = 1.0;
    }

    void normalize() {
        double mag = sqrt(x * x + y * y + z * z);
        x = x / mag;
        y = y / mag;
        z = z / mag;
    }

    // operator overloading
    Point operator+(const Point &p) {
        Point temp;
        temp.x = x + p.x;
        temp.y = y + p.y;
        temp.z = z + p.z;
        return temp;
    }

    Point operator-(const Point &p) {
        Point temp;
        temp.x = x - p.x;
        temp.y = y - p.y;
        temp.z = z - p.z;
        return temp;
    }

    Point operator*(const double d) {
        Point temp;
        temp.x = x * d;
        temp.y = y * d;
        temp.z = z * d;
        return temp;
    }


};

static Point get_cross_product(Point a, Point x) {
    Point tempPoint;
    tempPoint.setX(a.getY() * x.getZ() - a.getZ() * x.getY());
    tempPoint.setY(a.getZ() * x.getX() - a.getX() * x.getZ());
    tempPoint.setZ(a.getX() * x.getY() - a.getY() * x.getX());
    return tempPoint;
}

static double get_dot_product(Point a, Point x) {
    return a.getX() * x.getX() + a.getY() * x.getY() + a.getZ() * x.getZ();
}

class Color {
public:
    double R, G, B;
    Color() {
        R = 0;
        G = 0;
        B = 0;
    }
    Color(double R, double G, double B) {
        this->R = R;
        this->G = G;
        this->B = B;
    }
    double getR() { return R; }
    double getG() { return G; }
    double getB() { return B; }

    void normalize() {
        if(R > 1) R = 1;
        if(G > 1) G = 1;
        if(B > 1) B = 1;
        if(R < 0) R = 0;
        if(G < 0) G = 0;
        if(B < 0) B = 0;
    }

    // overload multiplication operator
    Color operator*(double d) {
        return Color(R * d, G * d, B * d);
    }
};



class Ray {
public:
    Point start;
    Point dir;
    Ray() {}
    Ray(Point start, Point dir) {
        this->start = start;
        this->dir = dir;
        this->dir.normalize();
    }

    Ray(const Ray &ray) {
        this->start = ray.start;
        this->dir = ray.dir;
    }

};



// light sources
class PointLight {
public:
    Point light_position;
    Color color;
    double falloff;
    PointLight() {}
    // with color
    PointLight(Point light_position, Color color, double falloff = 0.0) {
        this->light_position = light_position;
        this->color = color;
        this->falloff = falloff;
    }
    // uses default color
    PointLight(Point light_position, double falloff = 0.0) {
        this->light_position = light_position;
        this->falloff = falloff;
        this->color = Color(1, 1, 0); 
    }
    Point get_light_position() { return light_position; }
    Color getColor() { return color; }

    void draw() {
        glColor3f(color.R, color.G, color.B);
        glPushMatrix();
        glTranslatef(light_position.getX(), light_position.getY(), light_position.getZ());
        glutSolidSphere(1.0, 50.0, 50.0);
        glPopMatrix();
    }
};


class SpotLight {
public:
    PointLight pointLight;
    Point light_direction;
    double cutoff_angle;
    //double falloff;

    SpotLight(Point light_position, Point light_direction, double cutoff_angle, double falloff = 0.0) {
        this->pointLight = PointLight(light_position, falloff);
        this->light_direction = light_direction;
        this->cutoff_angle = cutoff_angle;
        //this->falloff = falloff;
    }
    // with color
    SpotLight(Point light_position, Point light_direction, Color color, double cutoff_angle, double falloff = 0.0) {
        this->pointLight = PointLight(light_position, color, falloff);
        this->light_direction = light_direction;
        this->cutoff_angle = cutoff_angle;
        //this->falloff = falloff;
    }

    void draw() {

        Color color = pointLight.getColor();
        Point light_position = pointLight.get_light_position();
        glColor3f(color.R, color.G, color.B);
        glPushMatrix();
        glTranslatef(light_position.getX(), light_position.getY(), light_position.getZ());
        glutSolidSphere(5.0, 100.0, 100.0);
        glPopMatrix();

    }
};


// a base class for all objects
class Object {
public:
    Point reference_point;
    Color color;
    double height, width, length;
    int shine; // exponent term of specular component
    vector <double> coefficients; // ambient, diffuse, specular, reflection coEfficients

    Object() {
        coefficients = vector<double>(4,0);
    }

    void setColor(Color color) { this->color = color; }

    void setShine(int shine) { this->shine = shine; }

    void setCoEfficients(double ambient, double diffuse, double specular, double reflection) {
        coefficients[0] = ambient;
        coefficients[1] = diffuse;
        coefficients[2] = specular;
        coefficients[3] = reflection;
    }

    virtual void draw() = 0;

    virtual double getIntersectingT(Ray ray) { return -1.0; }

    virtual Ray getNormal(Point intersection_point, Ray incident_ray) { return Ray(); }

    virtual Point getNormal(Point intersection_point) { return Point(); }

    virtual Color getColorAt(Point intersection_point) {
        return Color(this->color.getR(), this->color.getG(), this->color.getB());
    }
    

    virtual double intersect(Ray ray, Color &color, int level) { 

        double t = getIntersectingT(ray);
        //cout << "t = " << t << endl;
        if(t < 0) return -1.0;
        // finding intersection point and it's color
        Point intersection_point = ray.start + ray.dir * t;
        Color color_at_intersection = getColorAt(intersection_point);
        if(level == 0) {
            color = Color(0, 0, 0);
            color.normalize();
            return t;
        }


        double ambient = coefficients[0];
        double diffuse = coefficients[1];
        double specular = coefficients[2];
        double reflection = coefficients[3];

        color = color_at_intersection * ambient;
        color.normalize();

        Point normal = getNormal(intersection_point);
        normal.normalize(); // N : normal at intersection point

        double scaling_factor;
        double phong = 0.0;
        double lambert = 0.0;

        for (PointLight light : pointLights) {
            Point light_dir = light.light_position - intersection_point; // PS = toSource
            double dis = light_dir.get_magnitude(); // distance between intersection point P and light source S
            light_dir.normalize();
            // scaling factor for intensity
            scaling_factor = exp(-1.0 * dis * dis * light.falloff);

            Point light_position = intersection_point + light_dir * 1e-5;
            
            // casting incident ray from light position to intersection point
            Ray light_ray = Ray(light_position, light_dir);

            Color temp_color;
            double t2, t_min = INFINITY;
            bool is_shadow = false;
            for(int j = 0; j < objects.size(); j++) {
                t2 = objects[j]->intersect(light_ray, temp_color, 0);
                if(t2 > 0 && t2 < t_min) {
                    t_min = t2;
                }
            }

            // if not obstructed 
            if(t < t_min) {
                // lambert value
                lambert += max(get_dot_product(normal, light_dir), 0.0) * scaling_factor; // toSource.N
                
                Point reflection_dir = normal * (2 * get_dot_product(light_dir, normal)) - light_dir; // R' : reflected ray
                reflection_dir.normalize();
                // phong value
                phong += max(pow(get_dot_product(reflection_dir, ray.dir), shine), 0.0) * scaling_factor; 

                // updating diffuse components
                color.R += light.color.getR() * (diffuse * lambert) * color_at_intersection.getR();
                color.G += light.color.getG() * (diffuse * lambert) * color_at_intersection.getG();
                color.B += light.color.getB() * (diffuse * lambert) * color_at_intersection.getB();
                color.normalize();

                // updating specular components
                color.R += light.color.getR() * specular * phong;
                color.G += light.color.getG() * specular * phong;
                color.B += light.color.getB() * specular * phong;
                color.normalize();
            }
        }

        for (SpotLight light : spotLights) {
            
            Point light_position = light.pointLight.get_light_position();
            Point light_dir = intersection_point - light_position;
            light_dir.normalize();

            double dot = get_dot_product(light_dir, light.light_direction);
            double dot_len = light_dir.get_magnitude() * light.light_direction.get_magnitude();
            double angle = acos(dot / dot_len * 1.0) * 180 / M_PI;
            if(fabs(angle) > light.cutoff_angle) continue;

            light_dir = light_dir * (-1);
            light_position = intersection_point + light_dir * 1e-5;
            double dis = light_dir.get_magnitude(); // distance between intersection point and light source
            scaling_factor = exp(-1.0 * dis * dis * light.pointLight.falloff);

            Ray light_ray = Ray(light_position, light_dir);

            Color temp_color;
            double t2, t_min = INFINITY;
            bool is_shadow = false;
            for(int j = 0; j < objects.size(); j++) {
                t2 = objects[j]->intersect(light_ray, temp_color, 0);
                if(t2 > 0 && t2 < t_min) {
                    t_min = t2;
                }
            }

            if(t < t_min) {
                // lambert value
                lambert += max(get_dot_product(normal, light_dir), 0.0) * scaling_factor; // toSource.N
                
                Point reflection_dir = normal * (2 * get_dot_product(light_dir, normal)) - light_dir; // R'
                reflection_dir.normalize();
                // phong value
                phong += max(pow(get_dot_product(reflection_dir, light_dir), shine), 0.0) * scaling_factor; 

                // updating diffuse components
                color.R += light.pointLight.color.getR() * (diffuse * lambert) * color_at_intersection.getR();
                color.G += light.pointLight.color.getG() * (diffuse * lambert) * color_at_intersection.getG();
                color.B += light.pointLight.color.getB() * (diffuse * lambert) * color_at_intersection.getB();
                color.normalize();

                // updating specular components
                color.R += light.pointLight.color.getR() * specular * phong;
                color.G += light.pointLight.color.getG() * specular * phong;
                color.B += light.pointLight.color.getB() * specular * phong;
                color.normalize();
            }
        }

        if(level < n_rec) {
            // reflected ray
            Point reflection_dir = ray.dir - normal * (2 * get_dot_product(ray.dir, normal));
            reflection_dir.normalize();

            Point reflection_start = intersection_point + reflection_dir * 1e-5;
            Ray reflection_ray = Ray(reflection_start, reflection_dir);

            Color temp_color;
            double t2 = -1.0;
            double min_t = INFINITY;
            int min_index = -1;
            for(int i = 0; i < objects.size(); i++) {
                t2 = objects[i]->intersect(reflection_ray, temp_color, 0);
                if(t2 > 0 && t2 < min_t) {
                    min_t = t2;
                    min_index = i;
                }
            }
            if(min_index != -1) {
                min_t = objects[min_index]->intersect(reflection_ray, temp_color, level + 1);
                color.R += temp_color.R * reflection;
                color.G += temp_color.G * reflection;
                color.B += temp_color.B * reflection;
                color.normalize();
            }
        }
        return t;
    }    

    // destructor
    virtual ~Object() {
        coefficients.clear();
        coefficients.shrink_to_fit();
    }
    
};

// object classes
class Triangle : public Object {
public:
    Point p1, p2, p3;

    Triangle(Point p1, Point p2, Point p3) {
        this->p1 = p1;
        this->p2 = p2;
        this->p3 = p3;
    }

    void draw() {
        glColor3f(color.getR(), color.getG(), color.getB());
        glBegin(GL_TRIANGLES);
            glVertex3f(p1.getX(), p1.getY(), p1.getZ());
            glVertex3f(p2.getX(), p2.getY(), p2.getZ());
            glVertex3f(p3.getX(), p3.getY(), p3.getZ());
        glEnd();
    }

    virtual Ray getNormal(Point intersection_point, Ray incident_ray) {
        Point normal = get_cross_product(p2 - p1, p3 - p1);
        normal.normalize();
        if(get_dot_product(normal, incident_ray.dir) < 0) {
            normal = normal * (-1);
        }
        return Ray(intersection_point, normal);
    }

    virtual Point getNormal(Point intersection_point) {
        Point normal = get_cross_product(p2 - p1, p3 - p1);
        normal.normalize();
        return normal;
    }

    virtual double getIntersectingT(Ray ray) {
        
        Point normal = get_cross_product(p2 - p1, p3 - p1);
        normal.normalize();
        if(get_dot_product(normal, ray.dir) > 0) {
            normal = normal * (-1);
        }
        double t = get_dot_product(normal, p1 - ray.start) / get_dot_product(normal, ray.dir);
        if(t < 0) return -1.0;
        Point intersection_point = ray.start + ray.dir * t;
        Point v1 = p2 - p1;
        Point v2 = p3 - p1;
        Point v3 = intersection_point - p1;
        double dot11 = get_dot_product(v1, v1);
        double dot12 = get_dot_product(v1, v2);
        double dot13 = get_dot_product(v1, v3);
        double dot22 = get_dot_product(v2, v2);
        double dot23 = get_dot_product(v2, v3);
        double invDenom = 1 / (dot11 * dot22 - dot12 * dot12);
        double u = (dot22 * dot13 - dot12 * dot23) * invDenom;
        double v = (dot11 * dot23 - dot12 * dot13) * invDenom;
        if(u >= 0 && v >= 0 && u + v <= 1) {
            color = this->color;
            return t;
        }
        return -1.0;
    }
};


class Sphere : public Object {
public:
    Sphere() {}
    Sphere(Point center, double radius) {
        reference_point = center;
        length = radius;
        width = radius;
        height = radius;
    }

    void draw() {
        glColor3f(color.getR(), color.getG(), color.getB());
        glPushMatrix();
            glTranslatef(reference_point.getX(), reference_point.getY(), reference_point.getZ());
            glutSolidSphere(length, N_SLICES, N_STACKS);
        glPopMatrix();
    }

    virtual Ray getNormal(Point intersection_point, Ray incident_ray) {
        Point normal = intersection_point - reference_point;
        normal.normalize();
        return Ray(intersection_point, normal);
    }

    virtual Point getNormal(Point intersection_point) {
        Point normal = intersection_point - reference_point;
        normal.normalize();
        return normal;
    }

    virtual double getIntersectingT(Ray ray) {
        double a = 1;
        double b = 2.0 * get_dot_product(ray.dir, ray.start - reference_point);
        double c = get_dot_product(ray.start - reference_point, ray.start - reference_point) - length * length;
        double d = b * b - 4 * a * c;
        if(d < 0) return -1.0;
        double t1 = (-b - sqrt(d)) / (2 * a);
        double t2 = (-b + sqrt(d)) / (2 * a);
        if(t1 < 0 && t2 < 0) return -1.0;
        if(t1 > 0) return t1;
        if(t2 > 0) return t2;
        return -1.0;
    }
};

class Pyramid : public Object {
public:
    Pyramid(Point lowest_point, double width, double height) {
        reference_point = lowest_point;
        this->length = 0.0;
        this->width = width;
        this->height = height;
    }

    void draw() {
        glColor3f(color.getR(), color.getG(), color.getB());
        glPushMatrix();
        glTranslatef(reference_point.getX(), reference_point.getY(), reference_point.getZ());
        glScalef(length, height, width);
        glutSolidCube(1);
        glPopMatrix();
    }
};

class Cube : public Object {
public:
    Cube(Point bottom_lower, double length) {
        reference_point = bottom_lower;
        this->length = length;
        width = length;
        height = length;
    }

    void draw() {
        glColor3f(color.getR(), color.getG(), color.getB());
        glPushMatrix();
        glTranslatef(reference_point.getX(), reference_point.getY(), reference_point.getZ());
        glScalef(length, height, width);
        glutSolidCube(1);
        glPopMatrix();
    }
};

class CheckerBoard : public Object {
public:
    int num_of_tiles = 50;
    bitmap_image texture_white;
    bitmap_image texture_black;

    CheckerBoard(Point camera_pos, double floor_width, double tile_width) {
        //reference_point = camera_pos;
        reference_point = Point(-floor_width / 2, -floor_width / 2, 0);
        this->width = tile_width;
        this->length = tile_width;
        this->shine = 0;
        num_of_tiles = (int) (floor_width / tile_width);

        texture_white = bitmap_image("texture_w.bmp");
        texture_black = bitmap_image("texture_b.bmp");
        //texture_white = bitmap_image("texture_b.bmp");
        //texture_black = bitmap_image("texture_w.bmp");
    }

    virtual void draw() {
        for(int i=0; i<num_of_tiles; i++) {
            for(int j=0; j<num_of_tiles; j++) {
                if((i+j)%2 == 0) {
                    glColor3f(0, 0, 0);
                }
                else {
                    glColor3f(1, 1, 1);
                }
                glBegin(GL_QUADS);
                    glVertex3f(reference_point.getX() + i * width, reference_point.getY() + j * width, 0);
                    glVertex3f(reference_point.getX() + (i+1) * width, reference_point.getY() + j * width, 0);
                    glVertex3f(reference_point.getX() + (i+1) * width, reference_point.getY() + (j + 1) * width, 0);
                    glVertex3f(reference_point.getX() + i * width, reference_point.getY() + (j + 1) * width, 0);
                glEnd();
            }
        }
    }

    virtual double getIntersectingT(Ray ray) {
        Point normal = Point(0, 0, 1);
        double t = get_dot_product(normal, ray.start) * (-1.0) / get_dot_product(normal, ray.dir);
        //if(t < 0) return -1.0;
        Point intersection_point = ray.start + ray.dir * t;

        if(intersection_point.x < reference_point.x || intersection_point.x > -reference_point.x || intersection_point.y < reference_point.y || intersection_point.y > -reference_point.y) {
            return -1.0;
        }
        return t;
    }

    virtual Ray getNormal(Point intersection_point, Ray incident_ray) {
        Point normal = Point(0, 0, 1);
        if(incident_ray.dir.getZ() <= 0) {
            normal = normal * (-1);
        }
        return Ray(intersection_point, normal);
    }

    virtual Point getNormal(Point intersection_point) {
        Point normal = Point(0, 0, 1);
        return normal;
    }


    virtual Color getColorAt(Point intersection_point) {

        double x = intersection_point.x - reference_point.x;
        double y = intersection_point.y - reference_point.y;

        int cell = ((int)(x / width) + (int)(y / width)) % 2;

        if(texture == 0) {
            // no texture
            if(cell  == 0) {
                return Color(0, 0, 0);
            }
            else {
                return Color(1, 1, 1);
            }   
        }

        else {
            // texture on
            x = fmod(x, width);
            y = fmod(y, width);

            int tex_x_b = (int) (texture_black.height() * x) / width;
            int tex_y_b = (int) (texture_black.width() * y) / width;
            int tex_x_w = (int) (texture_white.height() * x) / width;
            int tex_y_w = (int) (texture_white.width() * y) / width;

            unsigned char r, g, b;
            if(cell == 0) {
                texture_black.get_pixel(tex_x_b, tex_y_b, r, g, b);
            }
            else {
                texture_white.get_pixel(tex_x_w, tex_y_w, r, g, b);
            }

            double R = 0.5 * (cell + (double) r / 255);
            double G = 0.5 * (cell + (double) g / 255);
            double B = 0.5 * (cell + (double) b / 255);

            return Color(R, G, B);
        }

        
    }

    virtual ~CheckerBoard() {
        texture_black.~bitmap_image();
        texture_white.~bitmap_image();
    }

};

#endif
