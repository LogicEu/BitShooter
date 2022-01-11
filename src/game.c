#include <BitShooter.h>

#include <stdlib.h>
#include <string.h>
#include <time.h>

#define PLAYER_SPEED 1.0
#define PLAYER_ROT 5.0

vec2 iResolution = {800.0, 600.0};
float iTime, iScale = 1.0;

void gameInit(int argc, char** argv)
{
    rand_seed(time(NULL));

    int fullscreen = 0;
    if (argc > 1) {
        if (!strcmp(argv[1], "-f") || !strcmp(argv[1], "-fullscreen")) {
            fullscreen++;
        }
    }

    glee_init();
    glee_window_create("BitShooter", iResolution.x, iResolution.y, fullscreen, 0);
    graphics2D_init();
}

void gameUpdate()
{
    iTime = glee_time_get();
    while(glee_window_is_open()) {
        glee_screen_clear();
        float deltaTime = glee_time_delta(&iTime);
        
        if (glee_key_pressed(GLFW_KEY_ESCAPE)) break;
        
        if (glee_key_down(GLFW_KEY_W)) {
            player2D.pos = vec2_add(player2D.pos, vec2_mult(rad_to_vec2(player2D.rot), deltaTime * PLAYER_SPEED));
        }
        if (glee_key_down(GLFW_KEY_S)) {
            player2D.pos = vec2_sub(player2D.pos, vec2_mult(rad_to_vec2(player2D.rot), deltaTime * PLAYER_SPEED));
        }
        if (glee_key_down(GLFW_KEY_D)) {
            player2D.rot -= deltaTime * PLAYER_ROT;
        }
        if (glee_key_down(GLFW_KEY_A)) {
            player2D.rot += deltaTime * PLAYER_ROT;
        }

        Tri2D triPlayer2D = player_to_triangle(player2D);

        backgroundDraw(iTime);
        triDraw(&triPlayer2D);
        mapDraw();

        glee_screen_refresh();
    }
}

void gameExit()
{
    glee_deinit();
    exit(EXIT_SUCCESS);
}