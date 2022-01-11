#include <BitShooter.h>

#define STROKE 4.0

extern void mapBind();

array_t *map2D, *mapIndices;

void map2D_put_quad(float x, float y)
{
    static int marker = 0;
    unsigned int indices[6] = {
        0 + marker,  1 + marker,  3 + marker,
        1 + marker,  2 + marker,  3 + marker
    };

    vec2 p = screen_to_world(vec2_new(x, y));
    Quad2D q = rect_to_quad(rect_new(p.x, p.y, STROKE * 2.0 / iResolution.x, STROKE * 2.0 / iResolution.y));
    array_push(map2D, &q);

    for (int i = 0; i < 6; i++) {
        array_push(mapIndices, &indices[i]);
    }
    
    marker += 4;
}

void map2D_init(int width, int height)
{
    int size = 2 * (width + height);
    map2D = array_new(size, sizeof(Quad2D));
    mapIndices = array_new(size * 6, sizeof(unsigned int));
    for (int x = 0; x < width; x++) {
        for (int y = 0; y < height; y++) {
            if (x == 0 || x == width - 1 || y == 0 || y == height - 1) map2D_put_quad(x, y);
        }
    }

    mapBind();
}