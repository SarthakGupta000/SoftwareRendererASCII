#include <unistd.h>
#include "util.h"

// rotates along each axis
void rotate(struct Vec3 *vector, double theta) {
    // x
    (*vector).y = (*vector).y * cos(theta) - (*vector).z * sin(theta);
    (*vector).z = (*vector).y * sin(theta) + (*vector).z * cos(theta);
    // y
    (*vector).x = (*vector).x * cos(theta) + (*vector).z * sin(theta);
    (*vector).z = (*vector).z * cos(theta) - (*vector).x * sin(theta);
    // z
    (*vector).x = (*vector).x * cos(theta) - (*vector).y * sin(theta);
    (*vector).y = (*vector).x * sin(theta) + (*vector).y * cos(theta);
}

int main() {
    struct Camera *camera = setupCamera();
    double numObj = 1;
    struct Object objs[1];
    struct Vec3 vertices[8] = {
    // Front face (Z = -1.5)
        {-1.5, -1.5, -1.5}, // 0: Front-Bottom-Left
        { 1.5, -1.5, -1.5}, // 1: Front-Bottom-Right
        { 1.5,  1.5, -1.5}, // 2: Front-Top-Right
        {-1.5,  1.5, -1.5}, // 3: Front-Top-Left

    // Back face (Z = 1.5)
        {-1.5,  1.5,  1.5}, // 4: Back-Top-Right
        { 1.5,  1.5,  1.5}, // 5: Back-Top-Left
        { 1.5, -1.5,  1.5}, // 6: Back-Bottom-Left
        {-1.5, -1.5,  1.5}  // 7: Back-Bottom-Right
    
    };
    struct Vec3 origin = {0.0, 0.0, 4};
    double bright = 255;
    double theta = 0.01;
    objs[0] = setupCuboid(vertices, origin, bright);
    struct World *world = setupWorld(numObj, objs);
    int current = 2;
    for (int i = 0; i < 6; i++) {
        objs[0].triangles[(2 * i)][3].x = current;
        objs[0].triangles[(2 * i) + 1][3].x = current;
        current += 42;
    }
    while (1) {
        render(camera, world);
        for (int i = 0; i < objs[0].numOfTriangles; i++) {
            for (int j = 0; j < 3; j++) {
                rotate(&(objs[0].triangles[i][j]), theta);
            }
        }
        usleep(5000);
        clearScreen(camera);
    }
    jmp:;
    deleteWorld(world);
    deleteCamera(camera);
    return 0;
}