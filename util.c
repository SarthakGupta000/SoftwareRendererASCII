#include "util.h"

// helpers

int approxEqual(double a, double b) {
    return fabs(a - b) < EPSILON;
}

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
3  2


0  1

Back view
5  4


6  7
*/

struct Object setupCuboid(struct Vec3 vertices[8], struct Vec3 origin, double brightness) { // in order
    struct Object cuboid;
    cuboid.numOfTriangles = 12;
    cuboid.origin = (struct Vec3) {origin.x, origin.y, origin.z};
    cuboid.triangles = (struct Vec3 **) malloc(sizeof(struct Vec3 *) * cuboid.numOfTriangles);
    for (int i = 0; i < cuboid.numOfTriangles; i++) {
        cuboid.triangles[i] = (struct Vec3 *) malloc(sizeof(struct Vec3) * 4);
    }

    // Front
    cuboid.triangles[0][0] = vertices[3];
    cuboid.triangles[0][1] = vertices[1];
    cuboid.triangles[0][2] = vertices[2];
    cuboid.triangles[0][3] = (struct Vec3) {brightness, 0, 0};

    cuboid.triangles[1][0] = vertices[0];
    cuboid.triangles[1][1] = vertices[1];
    cuboid.triangles[1][2] = vertices[3];
    cuboid.triangles[1][3] = (struct Vec3) {brightness, 0, 0};

    // Back
    cuboid.triangles[2][0] = vertices[4];
    cuboid.triangles[2][1] = vertices[5];
    cuboid.triangles[2][2] = vertices[6];
    cuboid.triangles[2][3] = (struct Vec3) {brightness, 0, 0};

    cuboid.triangles[3][0] = vertices[6];
    cuboid.triangles[3][1] = vertices[4];
    cuboid.triangles[3][2] = vertices[7];
    cuboid.triangles[3][3] = (struct Vec3) {brightness, 0, 0};

    // Left
    cuboid.triangles[4][0] = vertices[0];
    cuboid.triangles[4][1] = vertices[3];
    cuboid.triangles[4][2] = vertices[4];
    cuboid.triangles[4][3] = (struct Vec3) {brightness, 0, 0};

    cuboid.triangles[5][0] = vertices[0];
    cuboid.triangles[5][1] = vertices[7];
    cuboid.triangles[5][2] = vertices[4];
    cuboid.triangles[5][3] = (struct Vec3) {brightness, 0, 0};

    // Right
    cuboid.triangles[6][0] = vertices[1];
    cuboid.triangles[6][1] = vertices[5];
    cuboid.triangles[6][2] = vertices[6];
    cuboid.triangles[6][3] = (struct Vec3) {brightness, 0, 0};

    cuboid.triangles[7][0] = vertices[1];
    cuboid.triangles[7][1] = vertices[2];
    cuboid.triangles[7][2] = vertices[5];
    cuboid.triangles[7][3] = (struct Vec3) {brightness, 0, 0};

    // Top
    cuboid.triangles[8][0] = vertices[2];
    cuboid.triangles[8][1] = vertices[3];
    cuboid.triangles[8][2] = vertices[4];
    cuboid.triangles[8][3] = (struct Vec3) {brightness, 0, 0};

    cuboid.triangles[9][0] = vertices[2];
    cuboid.triangles[9][1] = vertices[4];
    cuboid.triangles[9][2] = vertices[5];
    cuboid.triangles[9][3] = (struct Vec3) {brightness, 0, 0};

    // Bottom
    cuboid.triangles[10][0] = vertices[1];
    cuboid.triangles[10][1] = vertices[7];
    cuboid.triangles[10][2] = vertices[6];
    cuboid.triangles[10][3] = (struct Vec3) {brightness, 0, 0};

    cuboid.triangles[11][0] = vertices[0];
    cuboid.triangles[11][1] = vertices[7];
    cuboid.triangles[11][2] = vertices[1];
    cuboid.triangles[11][3] = (struct Vec3) {brightness, 0, 0};

    return cuboid;
}

struct Camera *setupCamera() {
    struct Camera *camera = (struct Camera *) malloc(sizeof(struct Camera));
    for (int y = 0; y < HEIGHT; y++) {
        for (int x = 0; x < WIDTH; x++) {
            camera->renderingBuffer[y][x] = ' ';
            camera->depthBuffer[y][x] = INFINITY;
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

int insideScreen(struct Vec2 point) {
    struct Vec2 arr = convertToArray(point);
    if (arr.x >= 0 && arr.x < WIDTH && arr.y >= 0 && arr.y < HEIGHT) {
        return 1;
    }
    return 0;
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
        if (approxEqual(triangle[arr[0]].x, triangle[arr[1]].x)) {
            if ((triangle[i].x > triangle[arr[0]].x && normVec.x > triangle[arr[0]].x) || (triangle[i].x < triangle[arr[0]].x && normVec.x < triangle[arr[0]].x)) {
                sum++;
                continue;
            }
        }
        double m = (triangle[arr[0]].y - triangle[arr[1]].y) / (triangle[arr[0]].x - triangle[arr[1]].x);
        double yInt = triangle[arr[0]].y - (m * triangle[arr[0]].x);
        double a = m;
        double b = -1;
        double c = yInt;
        double val1 = (a * triangle[i].x) + (b * triangle[i].y) + c;
        double val2 = (a * normVec.x) + (b * normVec.y) + c;
        if ((val1 > 0 && val2 > 0) || (val1 < 0 && val2 < 0)) {
            sum++;
        }
    }
    if (sum == 3 && insideScreen(normVec)) {
        return 1;
    } else {
        return 0;
    }
}

char getPixel(double brightness) {
    const char *pixels = " .:-=+*#%@";
    int index = (int)((brightness * 10) / 256.0);

    return pixels[index];
}

// takes the 3D triangle with positive or zero z values
void fill(struct Camera *camera, struct Vec3 triangle[4]) {
    struct Vec2 triangle2D[3];
    for (int i = 0; i < 3; i++) {
        triangle2D[i].x = approxEqual(triangle[i].z , 0.0) ? triangle[i].x : (FOCAL_LENGTH * triangle[i].x) / triangle[i].z;
        triangle2D[i].y = approxEqual(triangle[i].z , 0.0) ? triangle[i].y : (FOCAL_LENGTH * triangle[i].y) / triangle[i].z;
    }
    // plane
    double unidepth = 0.0;
    double a, b, c;
    if (approxEqual((((triangle[0].x - triangle[2].x) * (triangle[1].y - triangle[2].y)) - ((triangle[1].x - triangle[2].x) * (triangle[0].y - triangle[2].y))), 0.0) || approxEqual((((triangle[0].y - triangle[2].y) * (triangle[1].x - triangle[2].x)) - ((triangle[1].y - triangle[2].y) * (triangle[0].x - triangle[2].x))), 0.0)) {
        return;
    }
    if (triangle[0].z == triangle[1].z && triangle[1].z == triangle[2].z) {
        unidepth = triangle[0].z;
        goto jmp;
    }
    a = (((triangle[0].z - triangle[2].z) * (triangle[1].y - triangle[2].y)) - ((triangle[1].z - triangle[2].z) * (triangle[0].y - triangle[2].y))) / (((triangle[0].x - triangle[2].x) * (triangle[1].y - triangle[2].y)) - ((triangle[1].x - triangle[2].x) * (triangle[0].y - triangle[2].y)));
    b = (((triangle[0].z - triangle[2].z) * (triangle[1].x - triangle[2].x)) - ((triangle[1].z - triangle[2].z) * (triangle[0].x - triangle[2].x))) / (((triangle[0].y - triangle[2].y) * (triangle[1].x - triangle[2].x)) - ((triangle[1].y - triangle[2].y) * (triangle[0].x - triangle[2].x)));
    c = triangle[0].z - (a * triangle[0].x) - (b * triangle[0].y);
    jmp:;
    for (int y = 0; y < HEIGHT; y++) {
        for (int x = 0; x < WIDTH; x++) {
            struct Vec2 arr = {(double) x, (double) y};
            struct Vec2 norm = convertToNomal(arr);
            if (insideTriangle(triangle2D, norm)) {
                double currentDepth;
                if (approxEqual(unidepth , 0.0)) {
                    currentDepth = (c * FOCAL_LENGTH) / (FOCAL_LENGTH - (a * norm.x) - (b * norm.y));
                } else {
                    currentDepth = unidepth;
                }
                if (camera->depthBuffer[y][x] > currentDepth) {
                    camera->depthBuffer[y][x] = currentDepth;
                    camera->renderingBuffer[y][x] = getPixel(triangle[3].x);
                }
            }
        }
    }
}

struct Vec3 clippedLine(struct Vec3 point1, struct Vec3 point2) {
    double dx = point1.x - point2.x;
    double dy = point1.y - point2.y;
    double dz = point1.z - point2.z;
    struct Vec3 ratio = {dx, dy, dz};
    ratio.x *= point1.z / dz;
    ratio.y *= point1.z / dz;
    struct Vec3 newPoint = {point1.x - ratio.x, point1.y - ratio.y, 0.0};
    return newPoint;
}

void rasterize(struct Camera *camera, struct Vec3 triangle[4]) {
    int sum = 0;
    for (int i = 0; i < 3; i++) {
        if (approxEqual(triangle[i].z, 0.0)) {
            sum++;
        }
    }
    if (sum == 0) {
        sum = 0;
        for (int i = 0; i < 3; i++) {
            if (triangle[i].z > 0) {
                sum++;
            }
        }
        if (sum == 3) {
            fill(camera, triangle);
            return;
        }
        if (sum == 0) {
            return;
        }
        if (sum == 1) {
            int val = 0;
            for (int i = 0; i < 3; i++) {
                if (triangle[i].z > 0) {
                    val = i;
                }
            }
            int vals[2];
            int next = 0;
            for (int i = 0; i < 3; i++) {
                if (i == val) {
                    continue;
                }
                vals[next] = i;
                next++;
            }
            struct Vec3 newTriangle[4];
            newTriangle[0] = triangle[val];
            newTriangle[1] = clippedLine(triangle[val], triangle[vals[0]]);
            newTriangle[2] = clippedLine(triangle[val], triangle[vals[1]]);
            newTriangle[3] = triangle[3];
            fill(camera, newTriangle);
            return;
        }
        if (sum == 2) {
            int val = 0;
            for (int i = 0; i < 3; i++) {
                if (triangle[i].z < 0) {
                    val = i;
                }
            }
            int vals[2];
            int next = 0;
            for (int i = 0; i < 3; i++) {
                if (i == val) {
                    continue;
                }
                vals[next] = i;
                next++;
            }
            struct Vec3 newTriangle1[4];
            newTriangle1[0] = triangle[vals[0]];
            newTriangle1[1] = triangle[vals[1]];
            newTriangle1[2] = clippedLine(triangle[vals[0]], triangle[val]);
            newTriangle1[3] = triangle[3];
            fill(camera, newTriangle1);
            struct Vec3 newTriangle2[4];
            newTriangle2[0] = triangle[vals[1]];
            newTriangle2[1] = clippedLine(triangle[val], triangle[vals[0]]);
            newTriangle2[2] = clippedLine(triangle[val], triangle[vals[1]]);
            newTriangle2[3] = triangle[3];
            fill(camera, newTriangle2);
            return;
        }
    }
    if (sum == 3) {
        fill(camera, triangle);
        return;
    }
    if (sum == 2) {
        double val = 0;
        for (int i = 0; i < 3; i++) {
            if (!approxEqual(triangle[i].z, 0.0)) {
                val = triangle[i].z;
                break;
            }
        }
        if (val > 0) {
            fill(camera, triangle);
            return;
        }
        if (val < 0) {
            return;
        }
    }
    if (sum == 1) {
        int vals[2];
        int next = 0;
        int zeroVal = 0;
        for (int i = 0; i < 3; i++) {
            if (!approxEqual(triangle[i].z, 0.0)) {
                vals[next] = i;
                next++;
            } else {
                zeroVal = i;
            }
        }
        if (triangle[vals[0]].z > 0 && triangle[vals[1]].z > 0) {
            fill(camera, triangle);
            return;
        }
        if (triangle[vals[0]].z < 0 && triangle[vals[1]].z < 0) {
            return;
        }
        struct Vec3 newTriangle[4];
        newTriangle[0] = triangle[zeroVal];
        newTriangle[1] = triangle[vals[0]].z > 0 ? triangle[vals[0]] : triangle[vals[1]];
        newTriangle[2] = clippedLine(triangle[vals[0]], triangle[vals[1]]);
        newTriangle[3] = triangle[3];
        fill(camera, newTriangle);
        return;
    }
}

void render(struct Camera *camera, struct World *world) {
    for (int i = 0; i < world->numOfObjects; i++) {
        for (int j = 0; j < world->objs[i].numOfTriangles; j++) {
            struct Vec3 worldTriangle[4];
            for (int k = 0; k < 3; k++) {
                worldTriangle[k].x = world->objs[i].triangles[j][k].x + world->objs[i].origin.x;
                worldTriangle[k].y = world->objs[i].triangles[j][k].y + world->objs[i].origin.y;
                worldTriangle[k].z = world->objs[i].triangles[j][k].z + world->objs[i].origin.z;
            }
            worldTriangle[3] = world->objs[i].triangles[j][3];
            rasterize(camera, worldTriangle);
        }
    }
    for (int y = 0; y < HEIGHT; y++) {
        for (int x = 0; x < WIDTH; x++) {
            printf("%c ", camera->renderingBuffer[y][x]);
        }
        printf("\n");
    }
}

void clearScreen(struct Camera *camera) {
    printf("\033[2J\033[1;1H");
    for (int y = 0; y < HEIGHT; y++) {
        for (int x = 0; x < WIDTH; x++) {
            camera->renderingBuffer[y][x] = ' ';
            camera->depthBuffer[y][x] = INFINITY;
        }
    }
}

void deleteCamera(struct Camera *camera) {
    free(camera);
}

void deleteWorld(struct World *world) {
    for (int i = 0; i < world->numOfObjects; i++) {
        for (int j = 0; j < world->objs[i].numOfTriangles; j++) {
            free(world->objs[i].triangles[j]);
        }
        free(world->objs[i].triangles);
    }
    free(world->objs);
    free(world);
}