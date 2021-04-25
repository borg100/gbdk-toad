#include <stdio.h>
#include <gb/font.h>
#include <gb/console.h>
#include <gb/drawing.h>
#include <gb/metasprites.h>
#include "character.c"
#include "tiles/toadbody.h"
#include "tiles/toadsmoke.h"

UBYTE running = 1;
joypads_t joypads;

// Blank tile.
const unsigned char blank_data[] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};

// Sprite delays (60 = 1 second)
#define FRAME_DELAY 7
#define TOAD_SMOKE_FRAME_DELAY 45

// Sprite indexes
// Toad smoke
#define TOAD_SMOKE_SPRITE_INDEX 0 // Metasprite size: 1 x 2 = 2 sprites
#define TOAD_SMOKE_SPRITE_COUNT 2
// Toad body
#define TOAD_BODY_SPRITE_INDEX 2 // Metasprite size: 2 x 3 = 6 sprites
#define TOAD_BODY_SPRITE_COUNT 6

// Sprite frames
// Toad body stand
#define TOAD_BODY_STAND_FRAME 0
// Toad body walk
#define TOAD_BODY_WALK_FRAME_START 1
#define TOAD_BODY_WALK_FRAME_END 3

void load_toad_data(Character *toad, UINT8 first_tile)
{
    // Body tiles
    toad->body_tile_index = first_tile;
    // Number of tiles = (sizeof(toadbody_data) >> 4)
    set_sprite_data(toad->body_tile_index, (sizeof(toadbody_data) >> 4), toadbody_data);

    // Smoke tiles
    toad->smoke_tile_index = toad->body_tile_index + (sizeof(toadbody_data) >> 4);
    // Number of tiles = (sizeof(toadsmoke_data) >> 4)
    set_sprite_data(toad->smoke_tile_index, (sizeof(toadsmoke_data) >> 4), toadsmoke_data);
}

UBYTE can_toad_move(Character *toad, UINT8 x, UINT8 y)
{
    return x >= 16 && x <= 160;
}

void move_toad(Character *toad, UINT8 x, UINT8 y)
{
    UINT8 i;

    if (toad->facing_right)
    {
        // Facing right (Default)
        move_metasprite(toadbody_metasprites[toad->body_frame_index], toad->body_tile_index, TOAD_BODY_SPRITE_INDEX, x, y);
    }
    else
    {
        // Facing left (Flip)
        move_metasprite_vflip(toadbody_metasprites[toad->body_frame_index], toad->body_tile_index, TOAD_BODY_SPRITE_INDEX, x, y);
    }

    if (toad->body_animate)
    {
        // Hide the smoke
        for (i = 0; i < TOAD_SMOKE_SPRITE_COUNT; i++)
        {
            set_sprite_tile(TOAD_SMOKE_SPRITE_INDEX + i, 0);
        }
    }
    else
    {
        // Show the smoke
        move_metasprite(toadsmoke_metasprites[toad->smoke_frame_index], toad->smoke_tile_index, TOAD_SMOKE_SPRITE_INDEX, x - 4, y - 4);
    }
}

void setup_toad(Character *toad)
{
    toad->updated = 1;

    // Initial Position
    toad->x = 64;
    toad->y = 64;

    toad->facing_right = 1;

    // Toad: 2x3
    toad->body_animate = 0;
    toad->body_frame_index = TOAD_BODY_STAND_FRAME;
    toad->body_frame_delay = 0;

    // Toad Smoke: 2x1
    toad->smoke_frame_index = 0;
    toad->smoke_frame_delay = TOAD_SMOKE_FRAME_DELAY;
}

void main(void)
{
    SHOW_BKG;
    SHOW_SPRITES;

    // Set palettes
    BGP_REG = 0b01010101;  // Set BG palette to GRAY
    OBP1_REG = 0b00000000; // Set OBJ palette to WHITE

    Character toad;

    // Load blank sprite data.
    set_sprite_data(0, 1, blank_data);

    // Load toad sprite data.
    load_toad_data(&toad, 1);

    // Set initial toad values.
    setup_toad(&toad);

    while (running)
    {
        joypad_ex(&joypads);

        if (toad.body_animate && !toad.body_frame_delay)
        {
            // Animate the body when Toad is moving.
            toad.updated = 1;
            toad.body_frame_delay = FRAME_DELAY;
            toad.body_frame_index++;

            if (toad.body_frame_index > TOAD_BODY_WALK_FRAME_END)
                // Reached the last frame. Reset to FRAME_START.
                toad.body_frame_index = TOAD_BODY_WALK_FRAME_START;
        }
        if (!toad.body_animate && !toad.smoke_frame_delay)
        {
            // Animate the smoke when Toad stops moving.
            toad.updated = 1;
            toad.smoke_frame_delay = TOAD_SMOKE_FRAME_DELAY;
            toad.smoke_frame_index++;
            // Number of frames = (sizeof(toadsmoke_metasprites) >> 1)
            if (toad.smoke_frame_index >= (sizeof(toadsmoke_metasprites) >> 1))
                // Reached the last frame. Reset to first frame.
                toad.smoke_frame_index = 0;
        }

        if (joypads.joy0 & J_LEFT)
        {
            // Move left
            toad.facing_right = 0;
            if (can_toad_move(&toad, toad.x - 1, toad.y))
            {
                toad.updated = 1;
                toad.x -= 1;
                toad.body_animate = 1; // Start body animation
            }
        }
        else if (joypads.joy0 & J_RIGHT)
        {
            // Move right
            toad.facing_right = 1;
            if (can_toad_move(&toad, toad.x + 1, toad.y))
            {
                toad.updated = 1;
                toad.x += 1;
                toad.body_animate = 1; // Start body animation
            }
        }
        else // Not moving
        {
            if (toad.body_animate || toad.body_frame_index != TOAD_BODY_STAND_FRAME)
            {
                // If body is animated OR body frame is not STAND_FRAME.
                toad.updated = 1;
                // Stop body animation
                toad.body_animate = 0;
                toad.body_frame_index = TOAD_BODY_STAND_FRAME;
                // Start smoke animation
                toad.smoke_frame_index = 0;
                toad.smoke_frame_delay = TOAD_SMOKE_FRAME_DELAY;
            }
        }

        if (toad.updated)
        {
            // If there's been any changes, update the metasprite.
            toad.updated = 0;
            move_toad(&toad, toad.x, toad.y);
        }

        // Decrement animation delays
        if (toad.body_frame_delay > 0)
            toad.body_frame_delay--;
        if (toad.smoke_frame_delay > 0)
            toad.smoke_frame_delay--;

        // Wait for vblank
        wait_vbl_done();
    }
}