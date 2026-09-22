#include <math.h>
#define FOCAL_LENGTH 1
#define INF 100000
#define HEIGHT 50
#define WIDTH 50

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

struct Vec2 convertToArray(struct Vec2 point);

struct Vec2 convertToNomal(struct Vec2 point);

int roundOff(double val);

struct Object setupCuboid(struct Vec3 vertices[8], struct Vec3 origin, double brightness);

struct Camera *setupCamera(); // set depths to INF and rendering buffer to spaces

struct World *setupWorld(int numOfObjects, struct Object objs[]);

int insideTriangle(struct Vec2 triangle[4], struct Vec2 point);

char getPixel(double brightness);

// array is positive Z
void fill(struct Camera *camera, struct Vec3 triangle[4]); // updates the depth buffer, fills, 2D clipping
 
void rasterize(struct Vec3 triangle[4]); // everything like negative clipping, calling fill

void render(struct Camera *camera);

void clearScreen(struct Camera *camera); // both the buffer and the screen

void deleteCamera(struct Camera *camera);

void deleteWorld(struct World *world);