#include<iostream>
#include<stack>
#include<cmath>
#include <fstream>
#include "point.h"
#include "matrix.h"
#include "triangle.h"
#include "utils.cpp"
#include "bitmap_image.hpp"
using namespace std;


int main() {

    // opening files
    std::ifstream scene;
    std::ofstream output;

    scene.open("scene.txt");
    output.open("stage1.txt");


    double eye_x, eye_y, eye_z;
    double look_x, look_y, look_z;
    double up_x, up_y, up_z;
    double fov_y, aspect_ratio, near, far;

    // first three lines - gluLookAt parameters
    scene >> eye_x >> eye_y >> eye_z;
    scene >> look_x >> look_y >> look_z;
    scene >> up_x >> up_y >> up_z;
    // corresponding vectors
    Point eye(eye_x, eye_y, eye_z);
    Point look(look_x, look_y, look_z);
    Point up(up_x, up_y, up_z);

    // fourth line - gluPerspective parameters
    scene >> fov_y >> aspect_ratio >> near >> far;

    // display code

    double tempX, tempY, tempZ, tempAngle;
    int num_of_triangles = 0; // for stage 2 and 3
    string line, valueLine;
     
    stack<Matrix> S; // a stack of transformation matrices
    Matrix M = get_identity_matrix(); // identity matrix


    // stage-1

    while (true) {
        
        scene >> line;

        if(line == "triangle") {
            // reading the points
            scene >> tempX >> tempY >> tempZ;
            Point p1(tempX, tempY, tempZ);
            scene >> tempX >> tempY >> tempZ;
            Point p2(tempX, tempY, tempZ);  
            scene >> tempX >> tempY >> tempZ;
            Point p3(tempX, tempY, tempZ);

            // multiplying each point with the transformation matrix
            p1 = matrix_point_multiply(M, p1);
            p2 = matrix_point_multiply(M, p2);
            p3 = matrix_point_multiply(M, p3);

            // scaling
            p1.scale();
            p2.scale();
            p3.scale();

            // printing the points in stage1.txt
            p1.print(output);
            p2.print(output);
            p3.print(output);

            output << endl;

            num_of_triangles++;

        }

        else if(line == "translate") {
            scene >> tempX >> tempY >> tempZ; // translation amounts
            Matrix T = get_translation_matrix(tempX, tempY, tempZ); // translation matrix
            M = matrix_multiply(M, T); // multiplying the transformation matrix with the translation matrix
        }

        else if(line == "scale") {
            scene >> tempX >> tempY >> tempZ; // scaling amounts
            Matrix T = get_scaling_matrix(tempX, tempY, tempZ); // scaling matrix
            M = matrix_multiply(M, T); // multiplying the transformation matrix with the scaling matrix
        }

        else if(line == "rotate") {
            scene >> tempAngle >> tempX >> tempY >> tempZ; // rotation angle and axis
            Matrix T = get_rotation_matrix(tempAngle, tempX, tempY, tempZ); // rotation matrix
            M = matrix_multiply(M, T); // multiplying the transformation matrix with the rotation matrix
        }

        else if(line == "push") {
            S.push(M); // pushing the transformation matrix to the stack
        }

        else if(line == "pop") {
            M = S.top(); // popping the top transformation matrix from the stack
            S.pop();
        }

        else if(line == "end") {
            break;
        }
    }

    // closing files
    scene.close();
    output.close();


    // stage-2

    Point l = point_subtraction(look, eye); // l = look - eye
    l.normalize();
    Point r = get_cross_product(l, up); // r = l x up
    r.normalize();
    Point u = get_cross_product(r, l); // u = r x l

    Matrix T1 = get_translation_matrix(-1.0 * eye.getX(), -1.0 * eye.getY(), -1.0 * eye.getZ()); // translation matrix
    // rotation matrix
    Matrix R1;
    R1.m[0][0] = r.getX();
    R1.m[0][1] = r.getY();
    R1.m[0][2] = r.getZ();

    R1.m[1][0] = u.getX();
    R1.m[1][1] = u.getY();
    R1.m[1][2] = u.getZ();

    R1.m[2][0] = -1.0 * l.getX();
    R1.m[2][1] = -1.0 * l.getY();
    R1.m[2][2] = -1.0 * l.getZ();
    R1.m[3][3] = 1.0;

    Matrix V = matrix_multiply(R1, T1); // view transformation matrix, V = RT


    scene.open("stage1.txt");
    output.open("stage2.txt");

    for(int i=0; i< num_of_triangles; i++) {
        scene >> tempX >> tempY >> tempZ;
        Point p1(tempX, tempY, tempZ);
        scene >> tempX >> tempY >> tempZ;
        Point p2(tempX, tempY, tempZ);  
        scene >> tempX >> tempY >> tempZ;
        Point p3(tempX, tempY, tempZ);

        p1 = matrix_point_multiply(V, p1);
        p2 = matrix_point_multiply(V, p2);
        p3 = matrix_point_multiply(V, p3);

        p1.scale();
        p2.scale();
        p3.scale();

        p1.print(output);
        p2.print(output);
        p3.print(output);

        output << endl;
    }

    scene.close();
    output.close();

    // stage-3
    double fov_x = fov_y * aspect_ratio;
    double t = near * tan(fov_y * M_PI / 360.0);
    double r1 = near * tan(fov_x * M_PI / 360.0);

    Matrix P;
    P.m[0][0] = near / r1;
    P.m[1][1] = near / t;
    P.m[2][2] = -1.0 * (far + near) / (far - near);
    P.m[2][3] = -1.0 * (2.0 * far * near) / (far - near);
    P.m[3][2] = -1.0;

    scene.open("stage2.txt");
    output.open("stage3.txt");

    for(int i=0; i< num_of_triangles; i++) {
        scene >> tempX >> tempY >> tempZ;
        Point p1(tempX, tempY, tempZ);
        scene >> tempX >> tempY >> tempZ;
        Point p2(tempX, tempY, tempZ);  
        scene >> tempX >> tempY >> tempZ;
        Point p3(tempX, tempY, tempZ);

        p1 = matrix_point_multiply(P, p1);
        p2 = matrix_point_multiply(P, p2);
        p3 = matrix_point_multiply(P, p3);

        p1.scale();
        p2.scale();
        p3.scale();

        p1.print(output);
        p2.print(output);
        p3.print(output);

        output << endl;
    }

    scene.close();
    output.close();

    // stage-4 : Clipping and Scan Conversion using Z-Buffer Algorithm
    
    // limits of the view plane
    double right_limit = 1.0;
    double left_limit = -1.0;
    double top_limit = 1.0;
    double bottom_limit = -1.0;
    double z_max = 1.0;
    double z_min = -1.0;
    
    // reading data

    double screen_width, screen_height;
    scene.open("config.txt");
    scene >> screen_width >> screen_height;
    scene.close();

    vector<Triangle> triangles;

    scene.open("stage3.txt");

    for(int i=0; i<num_of_triangles; i++) {
        scene >> tempX >> tempY >> tempZ;
        Point p1(tempX, tempY, tempZ);
        scene >> tempX >> tempY >> tempZ;
        Point p2(tempX, tempY, tempZ);  
        scene >> tempX >> tempY >> tempZ;
        Point p3(tempX, tempY, tempZ);
        Triangle temp(p1, p2, p3);
        double R = random_util();
        double G = random_util();
        double B = random_util();
        temp.setColor(Color(R, G, B));
        triangles.push_back(temp);
    }

    scene.close();

    // initializing z-buffer & frame-buffer

    double dx = (right_limit - left_limit) / screen_width;
    double dy = (top_limit - bottom_limit) / screen_height;
    double top_y = top_limit - dy / 2.0;
    double bottom_y = bottom_limit + dy / 2.0;
    double left_x = left_limit + dx / 2.0;
    double right_x = right_limit - dx / 2.0;

    bitmap_image image(screen_width, screen_height);

    // frame buffer
    Color** frame_buffer;
    int size = (int)screen_width;
    frame_buffer = new Color*[size];

    // z-buffer
    vector<vector<double>> z_buffer;
    
    // initializing frame-buffer and z-buffer
    for(int i=0; i<screen_width; i++) {
        vector<double> row;
        frame_buffer[i] = new Color[size];
        for(int j=0; j<screen_height; j++) {
            row.push_back(z_max);
            frame_buffer[i][j] = Color(0, 0, 0);
        }
        z_buffer.push_back(row);
    }
    

    // applying procedure
    for(int i=0; i<triangles.size(); i++) {
        
        Triangle temp = triangles[i];

        // finding top_scanline and bottom_scanline and clipping if necessary
        double max_y = max(temp.points[0].getY(), max(temp.points[1].getY(), temp.points[2].getY()));
        double min_y = min(temp.points[0].getY(), max(temp.points[1].getY(), temp.points[2].getY()));
        int top_scanline, bottom_scanline;
        if(max_y >= top_limit) top_scanline = 0; // clipping
        else top_scanline = int(ceil((top_y - max_y) / dy));  
        if(min_y <= bottom_limit) min_y = screen_height - 1; // clipping
        else bottom_scanline = screen_height - 1 - int(round((min_y - bottom_y) / dy));


        // debugging
        // cout << "i: " << i << endl;
        // cout << "p1: " << temp.points[0].getX() << " " << temp.points[0].getY() << " " << temp.points[0].getZ() << endl;
        // cout << "p2: " << temp.points[1].getX() << " " << temp.points[1].getY() << " " << temp.points[1].getZ() << endl;
        // cout << "p3: " << temp.points[2].getX() << " " << temp.points[2].getY() << " " << temp.points[2].getZ() << endl;
        // cout << "max_y: " << max_y << endl;
        // cout << "min_y: " << min_y << endl;
        // cout << "top_y: " << top_y << endl;
        // cout << "bottom_y: " << bottom_y << endl;
        // cout << "top_scanline: " << top_scanline << endl;
        // cout << "bottom_scanline: " << bottom_scanline << endl;


        for(int row_no = top_scanline; row_no <= bottom_scanline; row_no++) {
            
            // finding leftmost and rightmost intersecting column

            double y_mid = top_y - row_no * dy;

            // finding the intersection point between y_mid and the lines of the triangle
            double x_intersects[2]; // x-coordinates of the intersection points
            int count = 0;
            for(int i=0; i<2; i++) {
                for(int j=i+1; j<3; j++) {
                    // lines: (P0, P1), (P0, P2), (P1, P2)
                    Point p1 = temp.points[i];
                    Point p2 = temp.points[j];
                    if(p1.getY() != p2.getY()) {
                        double x = p1.getX() + (y_mid - p1.getY()) * (p2.getX() - p1.getX()) / (p2.getY() - p1.getY());
                        // handling an erroreneous case
                        if(x > max(p1.getX(), p2.getX()) || x < min(p1.getX(), p2.getX())) continue;
                        x_intersects[count++] = x;
                    }
                }
            }

            double leftmost_x = min(x_intersects[0], x_intersects[1]);
            double rightmost_x = max(x_intersects[0], x_intersects[1]);

            int left_intersecting_col, right_intersecting_col;
            if(leftmost_x <= left_limit) left_intersecting_col = 0; // clipping
            else left_intersecting_col = int(round((leftmost_x - left_x) / dx));
            if(rightmost_x >= right_limit) right_intersecting_col = screen_width - 1; // clipping
            else right_intersecting_col = screen_width - 1 - int(round((right_x - rightmost_x) / dx));

            // debugging
            // cout << "leftmost_x: " << leftmost_x << endl;
            // cout << "rightmost_x: " << rightmost_x << endl;
            // cout << "left_intersecting_col: " << left_intersecting_col << endl;
            // cout << "right_intersecting_col: " << right_intersecting_col << endl;
            

            for(int col_no = left_intersecting_col; col_no <= right_intersecting_col; col_no++) {
                double x_mid = left_x + col_no * dx;
                // calculating the z-coordinate of the point (x_mid, y_mid)
                // first we find the normal of the triangle plane
                Point normal = get_normal(temp.points[0], temp.points[1], temp.points[2]);
                // then we get the z-coordinate by solving the equation of the plane
                double z_value = (normal.getX() * (temp.points[0].getX() - x_mid) + normal.getY() * (temp.points[0].getY() - y_mid) + normal.getZ() * temp.points[0].getZ()) / normal.getZ();
                // cout << "z_value: " << z_value << endl;
                if(z_value > z_min && z_value < z_max) {
                    if(z_value < z_buffer[row_no][col_no]) {
                        z_buffer[row_no][col_no] = z_value;
                        frame_buffer[row_no][col_no] = temp.getColor();
                    }
                }
            }          
        }

    }

    // printing z-buffer to z_buffer.txt
    output.open("z_buffer.txt");

    for(int i=0; i<screen_height; i++) {
        for(int j=0; j<screen_width; j++) {
            if(z_buffer[i][j] < z_max) {
                output << fixed << setprecision(6) << z_buffer[i][j] << '\t';
            }
            // setting the pixels of the image accordingly
            image.set_pixel(j, i, frame_buffer[i][j].getR(), frame_buffer[i][j].getG(), frame_buffer[i][j].getB());
        }
        output << endl;
    }

    output.close();

    image.save_image("out.bmp");

    // freeing z-buffer
    for(int i=0; i<screen_height; i++) {
        z_buffer[i].clear();
    }
    z_buffer.clear();
    z_buffer.shrink_to_fit();

    // freeing frame-buffer
    for(int i=0; i<screen_height; i++) {
        delete [] frame_buffer[i];
    }
    delete [] frame_buffer;

    image.clear();


    return 0;
    

}