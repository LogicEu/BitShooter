#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include <glee.h>
#include <photon.h>

#define TRI_DIF_SCALE 0.02
#define TRI_DIF_X 1.0 * TRI_DIF_SCALE
#define TRI_DIF_Y 2.0 * TRI_DIF_SCALE
#define TRI_DIF_ROT -90.0

#define PLAYER_SPEED 1.0
#define PLAYER_ROT 5.0

typedef struct Player2D {
    vec2 pos;
    float rot;
} Player2D;

Player2D player;
vec2 resolution = {800.0, 600.0};
float scale = 1.0;

static unsigned int shaderBack, shaderTriangle;
static unsigned int quadID;
static unsigned int triangleVAO, triangleEBO, triangleID;
static unsigned int indices[3] = {0, 1, 2};

vec2 world_to_screen(vec2 p)
{
    vec2 v;
    v.x = (p.x + 0.5) * resolution.x * scale;
    v.y = (p.y + 0.5) * resolution.y * scale;
    return v;
}

vec2 screen_to_world(vec2 p)
{
    vec2 v;
    v.x = p.x / resolution.x * scale - 0.5;
    v.y = p.y / resolution.y * scale - 0.5;
    return v;
}

Tri2D player_to_triangle(Player2D p)
{
    Tri2D t;
    t.a = vec2_add(p.pos, vec2_new(TRI_DIF_X, -TRI_DIF_Y));
    t.b = vec2_add(p.pos, vec2_new(0.0, TRI_DIF_Y));
    t.c = vec2_add(p.pos, vec2_new(-TRI_DIF_X, -TRI_DIF_Y));
    
    t.a = vec2_rotate_around(t.a, p.pos, p.rot + deg_to_rad(TRI_DIF_ROT));
    t.b = vec2_rotate_around(t.b, p.pos, p.rot + deg_to_rad(TRI_DIF_ROT));
    t.c = vec2_rotate_around(t.c, p.pos, p.rot + deg_to_rad(TRI_DIF_ROT));
    
    return t;
}

void tribind(Tri2D* tri)
{
    triangleID = glee_buffer_id();
    glBindVertexArray(triangleID);
    glGenBuffers(1, &triangleVAO);
    glGenBuffers(1, &triangleEBO);
    glBindBuffer(GL_ARRAY_BUFFER, triangleVAO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(Tri2D), tri, GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, triangleEBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), &indices[0], GL_STATIC_DRAW);
    glee_buffer_attribute_set(0, 2, 0, 0);
}

void trirebind(Tri2D* tri)
{
    glBindVertexArray(triangleID);
    glBindBuffer(GL_ARRAY_BUFFER, triangleVAO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(Tri2D), tri, GL_STATIC_DRAW);
}

void triDraw(Tri2D* tri)
{
    glUseProgram(shaderTriangle);
    trirebind(tri);
    glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, 0);
}

void backgroundDraw(float T)
{
    glUseProgram(shaderBack);
    glBindVertexArray(quadID);
    glee_shader_uniform_set(shaderBack, 1, "u_time", &T);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
}

int main(int argc, char** argv)
{
    rand_seed(time(NULL));
    int fullscreen = 0;
    if (argc > 1) {
        if (!strcmp(argv[1], "-f") || !strcmp(argv[1], "-fullscreen")) {
            fullscreen++;
        }
    }

    //Init scene
    Player2D player2D = {{0.0, 0.0}, 0.0};
    Tri2D triPlayer2D = player_to_triangle(player2D);
    vec4 red = {1.0, 0.0, 0.0, 1.0};

    //Init OpenGL

    glee_init();
    glee_window_create("BitShooter", resolution.x, resolution.y, fullscreen, 0);

    quadID = glee_buffer_quad_create();
    shaderBack = glee_shader_load("shaders/vert.glsl", "shaders/frag.glsl");
    glee_shader_uniform_set(shaderBack, 2, "u_resolution", &resolution.x);

    tribind(&triPlayer2D);
    shaderTriangle = glee_shader_load("shaders/vert.glsl", "shaders/triangle.glsl");
    glee_shader_uniform_set(shaderTriangle, 2, "u_resolution", &resolution.x);
    glee_shader_uniform_set(shaderTriangle, 4, "u_color", &red.x);

    float deltaTime, T = glee_time_get();
    while(glee_window_is_open()) {
        glee_screen_clear();
        deltaTime = glee_time_delta(&T);
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

        triPlayer2D = player_to_triangle(player2D);

        backgroundDraw(T);
        triDraw(&triPlayer2D);

        glee_screen_refresh();
    }
    glee_deinit();
    return EXIT_SUCCESS;
}
