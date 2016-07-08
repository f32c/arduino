/*      RT2.C
        Copyright 1987 Eric Graham
        All rights reserved.
        This file may not be copied, modified or uploaded to a bulletin
        board system, except as provided below.
        Permission is granted to make a reasonable number of backup copies,
        in order that it may be used to generate executable code for use
        on a single computer system.
        Permission is granted to modify this code and use the modified code
        for non commercial use by the original purchaser of this software,
        and provided that this notice is included in the modified version.
 
        An executable version of this code, with extensive optimization,
        a generalized input interface and sample data files is available
        for $15.00 (U.S. Funds) from
 
                        Eric Graham
                        P.O. Box 579
                        Sandia Park
                        New Mexico 87047
                        U.S.A.
*/
#include <stdint.h>
#include <unistd.h>
#include <stdlib.h>
#include <math.h>
#include <stdio.h>

#include "raytrace.h"
 
/* This function is really a stub, you should */
void setup_world(struct observer *o, struct world *w, int *skip)
{
 double alt,az,degtorad;
 int i,j,k;
 double t,r,tp[3],lampfac,pmin[3],pmax[3];
 double sqrt(),sin(),cos();
 char str[128];
 int emuskip;
 
 degtorad=0.0174533;
 //printf("Bitte den skip - faktor eingeben (1 .. 50)\n");
 //gets(str);
 //sscanf(str,"%d",&emuskip);
 emuskip = 1;
 if (emuskip <= 0) emuskip = 10;
 if (emuskip >= 50) emuskip = 10;
  printf("skip-faktor = %d\n",emuskip);
 *skip=emuskip;
 for (k=0; k<3; ++k) {pmin[k]=BIG; pmax[k]=-BIG;}
 
 o->obspos[0]=-35.;
 o->obspos[1]=0.;
 o->obspos[2]=8.;
 
 o->nx=SCREEN_WIDTH; o->ny=SCREEN_HEIGHT;
 o->px=1.0/o->nx; o->py=0.75/o->ny;
 
 alt=-10.0; az=0.0;
 
 (o->fl)=0.028*30.0;
 
 alt*=degtorad;
 az*=degtorad;
 
 o->viewdir[0]=cos(az)*cos(alt);
 o->viewdir[1]=sin(az)*cos(alt);
 o->viewdir[2]=sin(alt);
 
 o->uhat[0]=sin(az);
 o->uhat[1]=-cos(az);
 o->uhat[2]=0.0;
 
 o->vhat[0]=-cos(az)*sin(alt);
 o->vhat[1]=-sin(az)*sin(alt);
 o->vhat[2]=cos(alt);
 
 w->numsp=12;
 
 w->sp=(struct sphere *)malloc(sizeof(struct sphere)*w->numsp);
 if (!w->sp)
        {printf("\nUnable to allocate memory"); cleanup("0");}
 
 w->sp[0].pos[0]=8.0;
 w->sp[0].pos[1]=0.0;
 w->sp[0].pos[2]=3.0;
 w->sp[0].radius=3.0;
 w->sp[0].type=MIRROR;
 w->sp[0].color[0]=1.0;
 w->sp[0].color[1]=0.7;
 w->sp[0].color[2]=0.7;

 w->sp[1].pos[0]=-8.0;
 w->sp[1].pos[1]=8.0;
 w->sp[1].pos[2]=3.0;
 w->sp[1].radius=3.0;
 w->sp[1].type=MIRROR;
 w->sp[1].color[0]=1.0;
 w->sp[1].color[1]=0.7;
 w->sp[1].color[2]=0.7;
 
 w->sp[2].pos[0]=-8.0;
 w->sp[2].pos[1]=-8.0;
 w->sp[2].pos[2]=3.0;
 w->sp[2].radius=3.0;
 w->sp[2].type=MIRROR;
 w->sp[2].color[0]=1.0;
 w->sp[2].color[1]=0.7;
 w->sp[2].color[2]=0.7;

 w->sp[3].pos[0]=8.0;
 w->sp[3].pos[1]=8.0;
 w->sp[3].pos[2]=3.0;
 w->sp[3].radius=3.0;
 w->sp[3].type=MIRROR;
 w->sp[3].color[0]=1.0;
 w->sp[3].color[1]=0.7;
 w->sp[3].color[2]=0.7;
 
 w->sp[4].pos[0]=8.0;
 w->sp[4].pos[1]=-8.0;
 w->sp[4].pos[2]=3.0;
 w->sp[4].radius=3.0;
 w->sp[4].type=MIRROR;
 w->sp[4].color[0]=1.0;
 w->sp[4].color[1]=0.7;
 w->sp[4].color[2]=0.7;

 w->sp[5].pos[0]=16.0;
 w->sp[5].pos[1]=8.0;
 w->sp[5].pos[2]=3.0;
 w->sp[5].radius=3.0;
 w->sp[5].type=MIRROR;
 w->sp[5].color[0]=1.0;
 w->sp[5].color[1]=0.7;
 w->sp[5].color[2]=0.7;
 
 w->sp[6].pos[0]=16.0;
 w->sp[6].pos[1]=-8.0;
 w->sp[6].pos[2]=3.0;
 w->sp[6].radius=3.0;
 w->sp[6].type=MIRROR;
 w->sp[6].color[0]=1.0;
 w->sp[6].color[1]=0.7;
 w->sp[6].color[2]=0.7;

 w->sp[7].pos[0]=-16.0;
 w->sp[7].pos[1]=8.0;
 w->sp[7].pos[2]=3.0;
 w->sp[7].radius=3.0;
 w->sp[7].type=MIRROR;
 w->sp[7].color[0]=1.0;
 w->sp[7].color[1]=0.7;
 w->sp[7].color[2]=0.7;
 
 w->sp[8].pos[0]=-16.0;
 w->sp[8].pos[1]=-8.0;
 w->sp[8].pos[2]=3.0;
 w->sp[8].radius=3.0;
 w->sp[8].type=MIRROR;
 w->sp[8].color[0]=1.0;
 w->sp[8].color[1]=0.7;
 w->sp[8].color[2]=0.7;

 w->sp[9].pos[0]=0.0;
 w->sp[9].pos[1]=-8.0;
 w->sp[9].pos[2]=3.0;
 w->sp[9].radius=3.0;
 w->sp[9].type=MIRROR;
 w->sp[9].color[0]=1.0;
 w->sp[9].color[1]=0.7;
 w->sp[9].color[2]=0.7;

 w->sp[10].pos[0]=0.0;
 w->sp[10].pos[1]=8.0;
 w->sp[10].pos[2]=3.0;
 w->sp[10].radius=3.0;
 w->sp[10].type=MIRROR;
 w->sp[10].color[0]=1.0;
 w->sp[10].color[1]=0.7;
 w->sp[10].color[2]=0.7;
 
 w->sp[11].pos[0]=8.0;
 w->sp[11].pos[1]=0.0;
 w->sp[11].pos[2]=30.0;
 w->sp[11].radius=20.0;
 w->sp[11].type=MIRROR;
 w->sp[11].color[0]=1.0;
 w->sp[11].color[1]=0.7;
 w->sp[11].color[2]=0.7;

 w->numlmp=1;
 
 w->lmp=(struct lamp *)malloc(sizeof(struct lamp)*w->numlmp);
 if (!w->lmp)
        {printf("\nUnable to allocate lamp memory"); cleanup("lamp alloc");}
 
 w->lmp[0].pos[0]=-100;
 w->lmp[0].pos[1]=-100;
 w->lmp[0].pos[2]=100;
 w->lmp[0].radius=10.0;
 
 w->lmp[0].color[0]=1.0;
 w->lmp[0].color[1]=1.0;
 w->lmp[0].color[2]=1.0;
 
 w->horizon[0].normal[0]=0.0;
 w->horizon[0].normal[1]=0.0;
 w->horizon[0].normal[2]=1.0;
 w->horizon[1].normal[0]=0.0;
 w->horizon[1].normal[1]=0.0;
 w->horizon[1].normal[2]=1.0;
 
 w->horizon[0].color[0]=1.0;
 w->horizon[0].color[1]=0.1;
 w->horizon[0].color[2]=0.2;
 
 w->horizon[1].color[0]=0.1;
 w->horizon[1].color[1]=1.;
 w->horizon[1].color[2]=0.2;
 
 w->illum[0]=0.3;
 w->illum[1]=0.3;
 w->illum[2]=0.3;
 
 w->skyzen[0]=0.1;
 w->skyzen[1]=0.1;
 w->skyzen[2]=1.0;
 
 w->skyhor[0]=0.2;
 w->skyhor[1]=0.2;
 w->skyhor[2]=0.4;
 
 
 lampfac=BIG;                   /* modify the lamp brightness so as to */
 for (i=0; i<w->numsp; ++i)     /* get the right exposure              */
        for (j=0; j<w->numlmp; ++j)
                {vecsub(tp,w->sp[i].pos,w->lmp[j].pos);
                 r=sqrt(dot(tp,tp));
                 r-=w->sp[i].radius;
                 for (k=0; k<3; ++k)
                        {t=w->sp[i].color[k]*w->lmp[j].color[k]/(r*r);
                         if (t == 0.0) continue;
                         t=(1.0-w->sp[i].color[k]*w->illum[k])/t;
                         if (t<lampfac) lampfac=t;
                        }
                }
 
 for (j=0; j<w->numlmp; ++j)
        for (k=0; k<3; ++k)
                w->lmp[j].color[k]*=lampfac;
 printf("\nlampfac=%f",lampfac);
}
