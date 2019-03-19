/* Compositing2 sprite example
 * with some moving vertical color stripes.
 */
#include <Compositing.h>
#include "shapes.h"

#define SPRITE_MAX 200
#define N_SHAPES (sizeof(Shape)/sizeof(Shape[0]))

extern Compositing c2;

struct sprite_speed 
{
  int x,y;
};
struct sprite_speed *Sprite_speed; // [SPRITE_MAX];

int sprite_start, sprite_end;

void stop_sprites()
{
  for(int i = sprite_start; i < sprite_end; i++)
    c2.Sprite[i]->y = 2000;
}

void setup_sprites()
{
  int i;
  Sprite_speed = (struct sprite_speed *) malloc(SPRITE_MAX * sizeof(struct sprite_speed));
  sprite_start = c2.n_sprites;
  for(i = 0; i < N_SHAPES && i < SPRITE_MAX; i++)
    c2.shape_to_sprite(&Shape[i]);
  for(i = sprite_start+N_SHAPES; i < sprite_start+SPRITE_MAX; i++)
    c2.sprite_clone(sprite_start + (i%N_SHAPES) );
  sprite_end = c2.n_sprites;
  stop_sprites(); // all off screen
#if 0
  Serial.println("*** setup sprite ***");
  Serial.println((uint32_t)Sprite_speed, HEX);
  Serial.println(sprite_start);
  Serial.println(sprite_end);
  Serial.println(c2.sprite_max);
#endif
}

void prepare_sprites()
{
  int i;
  for(i = sprite_start; i < sprite_end; i++)
  {
    c2.Sprite[i]->x = 20 + (rand() % 600);
    c2.Sprite[i]->y = 20 + (rand() % 400);
    Sprite_speed[i-sprite_start].x = (rand() % 3)-1;
    Sprite_speed[i-sprite_start].y = (rand() % 3)-1;
  }
  #if 0
  Serial.print("prepare ");
  Serial.print(sprite_start);
  Serial.print(" ");
  Serial.println(i);
  #endif
}

void loop_sprites()
{
  int i;

  for(i = sprite_start; i < sprite_end; i++)
  {
    c2.Sprite[i]->x += Sprite_speed[i-sprite_start].x;
    c2.Sprite[i]->y += Sprite_speed[i-sprite_start].y;
    if(c2.Sprite[i]->x < -40)
    {
      Sprite_speed[i-sprite_start].x = 1;
      if( (rand()&7) == 0 )
        Sprite_speed[i-sprite_start].y = (rand()%3)-1;
    }
    if(c2.Sprite[i]->x > VGA_X_MAX)
    {
      Sprite_speed[i-sprite_start].x = -1;
    }

    if(c2.Sprite[i]->y < -40)
    {
      Sprite_speed[i-sprite_start].y = 1;
      if( (rand()&7) == 0 )
        Sprite_speed[i-sprite_start].x = (rand()%3)-1;
    }
    if(c2.Sprite[i]->y > VGA_Y_MAX+40)
      Sprite_speed[i-sprite_start].y = -1;
  }
  while((*c2.vblank_reg & 0x80) == 0);
  c2.sprite_refresh();
  while((*c2.vblank_reg & 0x80) != 0);
  //delay(15);
}
