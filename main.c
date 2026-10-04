#include <unistd.h>
#include "util.h"

int main() {
    struct Camera *camera = setupCamera();
    double numObj = 1;
    struct Object objs[1];
    struct Vec3 vertices[8] = {
    // Front face (Z = -1.0)
        {-1.0, -1.0, -1.0}, // 0: Front-Bottom-Left
        { 1.0, -1.0, -1.0}, // 1: Front-Bottom-Right
        { 1.0,  1.0, -1.0}, // 2: Front-Top-Right
        {-1.0,  1.0, -1.0}, // 3: Front-Top-Left

    // Back face (Z = 1.0)
        { 1.0,  1.0,  1.0}, // 4: Back-Top-Right
        {-1.0,  1.0,  1.0}, // 5: Back-Top-Left
        {-1.0, -1.0,  1.0}, // 6: Back-Bottom-Left
        { 1.0, -1.0,  1.0}  // 7: Back-Bottom-Right
    
    };
    struct Vec3 origin = {0.0, 0.0, 12.0};
    double bright = 255;
    objs[0] = setupCuboid(vertices, origin, bright);
    struct World *world = setupWorld(1, objs);
    while (1) {
        render(camera, world);
        char move;
        scanf("%c", &move);
        if (move == 'd') {
            for (int i = 0; i < numObj; i++) {
                world->objs[i].origin.x -= SPEED;
            }
        } else if (move == 'a') {
            for (int i = 0; i < numObj; i++) {
                world->objs[i].origin.x += SPEED;
            }
        } else if (move == 'u') {
            for (int i = 0; i < numObj; i++) {
                world->objs[i].origin.y -= SPEED;
            }
        } else if (move == 'b') {
            for (int i = 0; i < numObj; i++) {
                world->objs[i].origin.y += SPEED;
            }
        } else if (move == 'w') {
            for (int i = 0; i < numObj; i++) {
                world->objs[i].origin.z -= SPEED;
            }
        } else if (move == 's') {
            for (int i = 0; i < numObj; i++) {
                world->objs[i].origin.z += SPEED;
            }
        } else if (move == 'q') {
            goto jmp;
        }
        usleep(100);
        clearScreen(camera);
    }
    jmp:;
    deleteWorld(world);
    deleteCamera(camera);
    return 0;
}