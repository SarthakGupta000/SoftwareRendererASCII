#include "util.h"

// helpers

struct Vec2 convertToArray(struct Vec2 point) {
    struct Vec2 newPoint;
    newPoint.x = (WIDTH / 2) + point.x;
    newPoint.y = (HEIGHT / 2) - point.y;
    return newPoint;
}

struct Vec2 convertToNomal(struct Vec2 point) {
    struct Vec2 newPoint;
    newPoint.x = point.x - (WIDTH / 2);
    newPoint.y = (HEIGHT / 2) - point.y;
    return newPoint;
}

int roundOff(double val) {
    return (int) round(val);
}

/*
Front view
D  C


A  B

Back view
F  E


G  H
*/

struct Object setupCuboid(struct Vec3 vertices[8], struct Vec3 origin, double brightness) { // in order
    struct Object cuboid;
    cuboid.numOfTriangles = 12;
    cuboid.origin = (struct Vec3) {origin.x, origin.y, origin.z};
    cuboid.triangles = (struct Vec3 **) malloc(sizeof(struct Vec3 *) * cuboid.numOfTriangles);
    for (int i = 0; i < cuboid.numOfTriangles; i++) {
        cuboid.triangles[i] = (struct Vec3 *) malloc(sizeof(struct Vec3) * 4);
    }
    // setting up the 12 triangles... I'll do it later
    return cuboid;
}

struct Camera *setupCamera() {
    struct Camera *camera = (struct Camera *) malloc(sizeof(struct Camera));
    for (int y = 0; y < HEIGHT; y++) {
        for (int x = 0; x < WIDTH; x++) {
            camera->renderingBuffer[y][x] = ' ';
            camera->depthBuffer[y][x] = INF;
        }
    }
    return camera;
}

struct World *setupWorld(int numOfObjects, struct Object objs[]) {
    struct World *world = (struct World *) malloc(sizeof(struct World));
    world->numOfObjects = numOfObjects;
    world->objs = (struct Object *) malloc(sizeof(struct Object) * numOfObjects);
    for (int i = 0; i < numOfObjects; i++) {
        world->objs[i] = objs[i];
    }
    return world;
}

int insideTriangle(struct Vec2 triangle[4], struct Vec2 normVec) {
    int sum = 0;
    for (int i = 0; i < 3; i++) {
        int arr[2];
        int next = 0;
        for (int j = 0; j < 3; j++) {
            if (i == j) {
                continue;
            }
            arr[next] = j;
            next++;
        }
        // suspicious use of (triangle[arr[0]].x - triangle[arr[1]].x) == 0 many times
        double m;
        if ((triangle[arr[0]].x - triangle[arr[1]].x) == 0) {
            m = INF;
            goto jmp;
        }
        m = (triangle[arr[0]].y - triangle[arr[1]].y) / (triangle[arr[0]].x - triangle[arr[1]].x);
        jmp:;
        double yIntercept = (triangle[arr[0]].x - triangle[arr[1]].x) == 0 ? triangle[arr[0]].y : triangle[arr[0]].y - (m * triangle[arr[0]].x);
        // general form (not really)
        double a = m;
        double b = -1;
        double c = yIntercept;
        double result1 = (triangle[arr[0]].x - triangle[arr[1]].x) == 0 ? triangle[i].x - triangle[arr[0]].x : (a * triangle[i].x) + (b * triangle[i].y) + c;
        double result2 = (triangle[arr[0]].x - triangle[arr[1]].x) == 0 ? normVec.x - triangle[arr[0]].x : (a * normVec.x) + (b * normVec.y) + c;
        if (result2 == 0) {
            sum += 1;
            break;
        }
        if ((result1 > 0 && result2 > 0) || (result1 < 0 && result2 < 0)) {
            sum += 1;
            break;
        }
    }
    if (sum == 3) {
        return 1;
    } else {
        return 0;
    }
}