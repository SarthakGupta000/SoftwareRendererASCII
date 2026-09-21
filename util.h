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
    struct Vec3 (*triangles)[4]; // fourth for brightness as (N, 0, 0)
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

int roundOff(double val);

struct Camera *setupCamera(); // set depths to INF and rendering buffer to spaces

struct World *setupWorld(int numOfObjects, struct Object objs[]);

void fill(struct Camera *camera, struct Vec2 triangle[3]); // updates the depth buffer and fills
 
void rasterize(struct Vec3 triangle[3]); // everything like clipping, projection, calling fill

void render(struct Camera *camera);

void clearScreen(struct Camera *camera); // both the buffer and the screen

void deleteCamera(struct Camera *camera);

void deleteWorld(struct World *world);