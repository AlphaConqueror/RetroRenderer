
#include "render.h"

#include <math.h>
#include <string.h>

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

/** Move the player according to its velocities.
 */
void update_player(player_t* p, ctx_t const* ctx) {
    NOT_IMPLEMENTED;
    UNUSED(p);
    UNUSED(ctx);
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
    NOT_IMPLEMENTED;
    UNUSED(c);
    UNUSED(u);
    UNUSED(v_from);
    UNUSED(v_to);
    UNUSED(color);
}

/** Render the scene from player's perspective into the context's out buffer.
 */
void render(const player_t* p, ctx_t* c) {
    NOT_IMPLEMENTED;
    UNUSED(p);
    UNUSED(c);
}

int bonus_implemented(void) {
    // TODO change to 1 if the bonus exercise is implemented.
    return 0;
}
