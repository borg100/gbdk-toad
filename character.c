#include <stdio.h>
#include <gb/font.h>
#include <gb/console.h>
#include <gb/drawing.h>

typedef struct Character
{
    UBYTE updated;

    // Position
    UINT8 x;
    UINT8 y;

    // States
    UBYTE facing_right;

    // Body metasprite
    UINT8 body_tile_index;
    UINT8 body_frame_index;
    UINT8 body_frame_delay;
    UBYTE body_animate;

    // Smoke metasprite
    UINT8 smoke_tile_index;
    UINT8 smoke_frame_index;
    UINT8 smoke_frame_delay;

} Character;