
#include "render.h"

#include <math.h>
#include <string.h>
#include <math.h>

#include "util.h"

// useful for converting degrees to radians
#define PI 3.14159265358979323846

// useful for mapping f into the range [0, k)
float float_mod(float f, int k) {
    // map into (-k, k)
    float res = fmodf(f, k);

    // fold into [0, k]
    if (res < 0) {
        res += k;
    }

    // accommodate for floating point rounding errors
    if (res >= k) {
        res = 0.0;
    }
    return res;
}

//struct containing all endpoints
typedef struct {
    float Lx, Ly, Rx, Ry;
} endpoints_t;

float boundaries(float f, float k, float fSmaller0, float fBiggerK) {
    float result = f;
    
    if(f < 0)
        result = fSmaller0;
    else if(f > k)
        result = fBiggerK;
    
    return result;
}

float getHeightAtCoords(float x, float y, const ctx_t* c) {
    return c->height_map[(int) x + (int) y * c->map_size];
}

uint32_t getColorAtCoords(float x, float y, const ctx_t* c) {
    return c->color_map[(int) x + (int) y * c->map_size];
}

//calculates the endpoints with the available data
endpoints_t getEndpoints(const player_t* p, int distance) {
    int angle = p->angle;
    float x = p->x,
          y = p->y,
          a = cos(angle) * distance,
          b = sin(angle) * distance;
    endpoints_t e = {x + a - b, y - b - a, x + a - b, y - b + a};
    
    return e;
}

//calculates the displayed height at a certain point
float getDisplayedHeight(float x, float y, const player_t* p, const ctx_t* c, int distance) {
    return (c->scr_width/2) * ((getHeightAtCoords(x, y, c) - p->height)/distance) + (c->scr_height/2);
}

int min(int a, int b) {
    return a < b ? a : b;
}

int max(int a, int b) {
    return a > b ? a : b;
}

/** Move the player according to its velocities.
 */
void update_player(player_t* p, ctx_t const* ctx) {
    float x = p->x, y = p->y;
    int angle = p->angle,
        velocity = p->v,
        height = p->height,
        map_size = ctx->map_size;
        
    //A1.1 + angle in [0,360[
    angle += p->v_angular;
    
    angle = boundaries(angle, 359, angle + 360, angle - 360);
    
    p->angle = angle;
    
    //A1.2
    if(velocity != 0) {
        x += cos(angle * PI/180) * velocity;
        y -= sin(angle * PI/180) * velocity;
        
        x = float_mod(x, map_size);
        y = float_mod(y, map_size);
        
        p->x = x;
        p->y = y;
    }
    
    //A1.3
    //uint8_t *height_map = ctx->height_map;
    //int roundedX = (int) x, roundedY = (int) y;
    //int mapHeightAtCoords = *(mapBaseAddress + (int) (roundedX * roundedY < 0 ? 0 : roundedX * roundedY - 1));
    
    int mapHeightAtCoords = getHeightAtCoords(x, y, ctx);

    height += p->v_height;
    
    if(height < mapHeightAtCoords + 20)
        height = mapHeightAtCoords + 20;
    
    p->height = height;
}

/** Draw a vertical line into the context's out buffer in the screen column u
 *  from screen row v_from (exclusively) to screen row v_to (inclusively).
 *
 *  For every call to this function, v_from <= v_to has to hold.
 *  If v_from == v_to holds, nothing is drawn.
 *
 *  Coordinates have their origin in the bottom left corner of the screen and
 *  are 1-based.
 */
void draw_line(ctx_t* c, int u, int v_from, int v_to, uint32_t color) {
    //A.2
    int height = c->scr_height,
        width = c->scr_width;
    uint32_t* buffer = c->out;
    
    for(int i = height - v_to; i <= height - v_from - 1; i++) {
        buffer[i * width + u - 1] = color;
    }
}

/** Render the scene from player's perspective into the context's out buffer.
 */
void render(const player_t* p, ctx_t* c) {
    //A.3
    int height = c->scr_height,
        width = c->scr_width,
        distance = c->distance;
    
    //initializing screen in sky color
    for(int i = 1; i <= width; i++) {
        draw_line(c, i, 0, height, c->sky_color);
    }
    
    for(int i = distance; i > 0; i--) {
        endpoints_t e = getEndpoints(p, i);
        
        UNUSED(e);
        
        /*for(int j = min(e.Lx, e.Rx) + 1; j <= max(e.Lx, e.Rx) + 1; j++) {
            //TODO: Fix error in scr coords -> map coords (height/color)
            
            float Qu =  -1 * ((e.Ry - e.Ly)/(e.Lx - e.Rx)) * (j - e.Lx) + e.Ly;
            
            draw_line(c, j, 0, getDisplayedHeight(j, Qu, p, c, i), getColorAtCoords(j, Qu, c));
        }*/
    }
}

int bonus_implemented(void) {
    // TODO change to 1 if the bonus exercise is implemented.
    return 0;
}
