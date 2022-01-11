#include <BitShooter.h>

#define TRI_DIF_SCALE 0.02
#define TRI_DIF_X 1.0 * TRI_DIF_SCALE
#define TRI_DIF_Y 2.0 * TRI_DIF_SCALE
#define TRI_DIF_ROT -90.0

Player2D player2D = {{0.0, 0.0}, 0.0};

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