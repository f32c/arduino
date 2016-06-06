#ifndef COMPOSITING_H
#define COMPOSITING_H

extern "C"
{
  #include "compositing_line.h"
  #include "sprite.h"
  #include "shape.h"
}

#include <Arduino.h>
#include <inttypes.h>

#define VGA_X_MAX 640
#define VGA_Y_MAX 480

class Compositing
{
  private:

  public:
  // compositing line without pixel content
  // content needs to be malloc'd
    Compositing();
    void init();
    void alloc_sprites(int n);
    struct compositing_line **scanlines;
    volatile uint32_t *videobase_reg = (volatile uint32_t *)0xFFFFFB90;
    volatile uint8_t *cntrl_reg = (volatile uint8_t *)0xFFFFFB81;
    volatile uint8_t *vblank_reg = (volatile uint8_t *)0xFFFFFB87;
    
    struct sprite **Sprite; // global pointer array to sprites
    int n_sprites = 0, sprite_max = 0; // number of sprites currently created

    void sprite_refresh(void); // refresh compositing linked list after changing x/y positions
    int shape_to_sprite(struct shape *sh);
    int sprite_clone(int original); // new sprite with clone content from existing sprite
    int sprite_add(struct sprite *s); // new sprite with clone content from existing sprite
    int sprite_fill_rect(int w, int h, pixel_t color);
    void sprite_position(int sprite, int x, int y);
    void sprite_link_content(int original, int clone);

};

#endif
