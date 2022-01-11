#include <BitShooter.h>

static unsigned int shaderBack, shaderPolygon;
static unsigned int backgroundID;
static unsigned int triangleVAO, triangleEBO, triangleID;
static unsigned int mapID, mapVAO, mapEBO;

static void triBind(Tri2D* tri)
{
    unsigned int triangleIndices[] = {0, 1, 2};

    triangleID = glee_buffer_id();
    glBindVertexArray(triangleID);
    glGenBuffers(1, &triangleVAO);
    glGenBuffers(1, &triangleEBO);
    glBindBuffer(GL_ARRAY_BUFFER, triangleVAO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(Tri2D), tri, GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, triangleEBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(triangleIndices), &triangleIndices[0], GL_STATIC_DRAW);
    glee_buffer_attribute_set(0, 2, 0, 0);
}

static void triRebind(Tri2D* tri)
{
    glBindVertexArray(triangleID);
    glBindBuffer(GL_ARRAY_BUFFER, triangleVAO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(Tri2D), tri, GL_STATIC_DRAW);
}

void graphics2D_init()
{
    backgroundID = glee_buffer_quad_create();
    shaderBack = glee_shader_load("shaders/vert.glsl", "shaders/frag.glsl");
    glee_shader_uniform_set(shaderBack, 2, "u_resolution", &iResolution.x);

    Tri2D triangle = player_to_triangle(player2D);
    triBind(&triangle);
    map2D_init(iResolution.x, iResolution.y);

    shaderPolygon = glee_shader_load("shaders/vert.glsl", "shaders/polygon_basic.glsl");
    glee_shader_uniform_set(shaderPolygon, 2, "u_resolution", &iResolution.x);
}

void triDraw(Tri2D* tri)
{
    static vec4 red = {1.0, 0.0, 0.0, 1.0};

    glUseProgram(shaderPolygon);
    triRebind(tri);
    glee_shader_uniform_set(shaderPolygon, 4, "u_color", &red.x);
    glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, 0);
}

void mapBind()
{
    mapID = glee_buffer_id();
    glBindVertexArray(mapID);
    glGenBuffers(1, &mapVAO);
    glGenBuffers(1, &mapEBO);
    glBindBuffer(GL_ARRAY_BUFFER, mapVAO);
    glBufferData(GL_ARRAY_BUFFER, map2D->used * map2D->bytes, map2D->data, GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mapEBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, mapIndices->used * mapIndices->bytes, mapIndices->data, GL_STATIC_DRAW);
    glee_buffer_attribute_set(0, 2, 0, 0);
}

void mapDraw()
{
    static vec4 black = {0.0, 0.0, 0.0, 1.0};

    glUseProgram(shaderPolygon);
    glBindVertexArray(mapID);
    glee_shader_uniform_set(shaderPolygon, 4, "u_color", &black.x);
    glDrawElements(GL_TRIANGLES, map2D->used * 6, GL_UNSIGNED_INT, 0);
}

void backgroundDraw(float T)
{
    glUseProgram(shaderBack);
    glBindVertexArray(backgroundID);
    glee_shader_uniform_set(shaderBack, 1, "u_time", &T);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
}