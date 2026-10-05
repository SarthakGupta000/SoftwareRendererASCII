#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#define FOCAL_LENGTH 10
#define SPEED 0.5
#define EPSILON 1e-6
#define HEIGHT 30
#define WIDTH 75

struct Vec3 {
    double x;
    double y;
    double z;
};

struct Vec2 {
    double x;
    double y;
};

struct Object {
    struct Vec3 origin; // in world space
    struct Vec3 **triangles; // fourth for brightness as (N, 0, 0) where N belongs to [0, 256)
    int numOfTriangles;
};


struct World {
    struct Object *objs; // in world space
    int numOfObjects;
};

struct Camera {
    char renderingBuffer[HEIGHT][WIDTH];
    double depthBuffer[HEIGHT][WIDTH];
};

int approxEqual(double a, double b);

struct Vec2 convertToArray(struct Vec2 point);

struct Vec2 convertToNomal(struct Vec2 point);

int roundOff(double val);

struct Object setupCuboid(struct Vec3 vertices[8], struct Vec3 origin, double brightness);

struct Camera *setupCamera(); // set depths to INFINITY and rendering buffer to spaces

struct World *setupWorld(int numOfObjects, struct Object objs[]);

int insideScreen(struct Vec2 point);

int insideTriangle(struct Vec2 triangle[4], struct Vec2 point);

char getPixel(double brightness);

// array is positive Z
void fill(struct Camera *camera, struct Vec3 triangle[4]); // updates the depth buffer, fills, 2D clipping

struct Vec3 clippedLine(struct Vec3 point1, struct Vec3 point2);

void rasterize(struct Camera *camera, struct Vec3 triangle[4]); // everything like negative clipping, calling fill
 
// print
void render(struct Camera *camera, struct World *world);

void clearScreen(struct Camera *camera); // both the buffer and the screen

void deleteCamera(struct Camera *camera);

void deleteWorld(struct World *world);