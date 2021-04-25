#include <stdio.h>
#include <gb/font.h>
#include <gb/console.h>
#include <gb/drawing.h>

typedef struct Character
{
    // When this is set to 1, the update_toad() function will be called.
    UBYTE updated;

    // Position
    UINT8 x;
    UINT8 y;

    // States
    UBYTE facing_right; // 0=LEFT  1=RIGHT
    UBYTE body_animate; // Body animation state: 0=OFF  1=ON

    // Body metasprite
    UINT8 body_tile_index;
    UINT8 body_frame_index;
    UINT8 body_frame_delay;

    // Smoke metasprite
    UINT8 smoke_tile_index;
    UINT8 smoke_frame_index;
    UINT8 smoke_frame_delay;

} Character;