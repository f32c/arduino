/***************************************************************************/
/*      RT1.C    Ray tracing program
        Copyright 1987 Eric Graham
        Permission is granted to copy and modify this file, provided
        that this notice is retained.
        
        Modified for ANSI C by Emu
*/
/***************************************************************************/
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>
#include <math.h>

#include "raytrace.h"
/***************************************************************************/
/***************************************************************************/
/*-------------------------------------------------------------------------*/
/*				Routine	:	main
				Input	:	none
				Output	:	hoffentlich
*/
/***************************************************************************/
int oldcolor;

/*-------------------------------------------------------------------------*/
void main_raytrace(void)
{
    double line[6],brite[3];
    struct observer o;  struct world w;
    int i,j,ii,jj,skip; short int si,sj;
	
	  // setup_screen();
 
    setup_world(&o,&w,&skip); 			/*  Provide this function to set up the
                       				observer and the world */
    si=1+(o.nx-1)/skip;
    sj=1+(o.ny-1)/skip;

    for (jj=j=0; j<o.ny; j+=skip,jj++) {
		// printf("zeile %4d \n",jj);
        for (ii=i=0; i<o.nx; i+=skip,ii++){
            pixline(line,&o,i,j); 
            raytrace(brite,line,&w);
			emu_set_pix(ii,jj, brite[0] *255.0 +0.5, brite[1] *255.0 + 0.5, brite[2] * 255.0 +0.5); 
		//	printf ("%4d %4d %3d %3d %3d\n", ii, jj, (int) brite[0], (int) brite[1], (int) brite[2]);
        }                         								/* a pixel */
    }
	//write_bitmap();
	//puts("written");
  //cleanup("exit");         				/* Free up resources allocated */
}                       				/* by initsc() */
 
/***************************************************************************/
/*-------------------------------------------------------------------------*/
/*				Routine	:	raytrace		rekursiv
				Input	:	brite			double
							line			double
							w				struct world
				Output	:	hoffentlich
				Comment	:	Do the raytracing 
*/
/***************************************************************************/
/*-------------------------------------------------------------------------*/
void raytrace(double brite[3], double *line, struct world *w)
{
	double t,tmin,pos[3];  
	int k;
    struct patch ptch;  struct sphere *spnear;
    struct lamp *lmpnear;

    tmin=BIG;  
    spnear=0;									/* can we see some spheres */
    for (k=0; k<w->numsp; ++k)
        if (intsplin_sphere(&t,line,w->sp+k)) {
            if (t<tmin) {
            	tmin=t; 
            	spnear=w->sp+k;
            }
        }
    lmpnear=0;									/* are we looking at a lamp */
    for (k=0; k<w->numlmp; ++k)
        if (intsplin_lamp(&t,line,w->lmp+k)) {
            if (t < tmin) {
            	tmin=t; 
            	lmpnear=w->lmp+k;
            }
        }
    if (lmpnear) {								/* we see a lamp! */
        for (k=0; k<3; ++k)
            brite[k]=lmpnear->color[k]/(lmpnear->radius*
                     lmpnear->radius);
         return;
        }
    if (inthor(&t,line))						/* do we see the ground? */
        if (t<tmin) {
            point(pos,t,line);  
            k=gingham(pos); 					/* cheap vinyl */
            veccopy(w->horizon[k].pos,pos);
            pixbrite(brite,&(w->horizon[k]),w,0);
            return;
        }
    if (spnear) {								/* we see a sphere */
        point(ptch.pos,tmin,line);  
        setnorm(&ptch,spnear);
        colorcpy(ptch.color,spnear->color);
        switch(spnear->type) {					/* treat the surface type */
            case BRIGHT:						/* is it a highlight? */
                if (glint(brite,&ptch,w,spnear,line)) 
                return;
            case DULL:
                pixbrite(brite,&ptch,w,spnear); 
                return;
            case MIRROR:
                mirror(brite,&ptch,w,line); 
                return;
        }
         return;
    }
    skybrite(brite,line,w);					/* nothing else, must be sky */
}
 
/***************************************************************************/
/*-------------------------------------------------------------------------*/
/*				Routine	:	skybrite
				Comment	:	calculate sky color
	   						Blend a sky color from the zenith 
	   						to the horizon 
*/
/***************************************************************************/
/*-------------------------------------------------------------------------*/
void skybrite(double brite[3], double *line, struct world *w)
{ 
	double sin2,cos2;  
      
    sin2=line[5]*line[5];
    sin2/=(line[1]*line[1]+line[3]*line[3]+sin2);
    cos2=1.0-sin2;
    brite[0]=cos2*w->skyhor[0]+sin2*w->skyzen[0];
    brite[1]=cos2*w->skyhor[1]+sin2*w->skyzen[1];
    brite[2]=cos2*w->skyhor[2]+sin2*w->skyzen[2];
}
 
/***************************************************************************/
/*-------------------------------------------------------------------------*/
/*				Routine	:	pixline
				Comment	:	
							calculate ray for pixel i,j
*/
/***************************************************************************/
/*-------------------------------------------------------------------------*/
void pixline(double *line, struct observer *o, int i, int j) 
{
    double x,y,tp[3];  
    
    y=(0.5*o->ny-j)*o->py;
    x=(i-0.5*o->nx)*o->px;
    tp[0]=o->viewdir[0]*o->fl+y*o->vhat[0]+x*o->uhat[0]+o->obspos[0];
    tp[1]=o->viewdir[1]*o->fl+y*o->vhat[1]+x*o->uhat[1]+o->obspos[1];
    tp[2]=o->viewdir[2]*o->fl+y*o->vhat[2]+x*o->uhat[2]+o->obspos[2];
    genline(line,o->obspos,tp); 				/* generate equation of line */
}
 
/***************************************************************************/
/*-------------------------------------------------------------------------*/
/*				Routine	:	vecsub
				Comment	:	
							a = b-c for vectors
*/
/***************************************************************************/
/*-------------------------------------------------------------------------*/
void vecsub(double *a,double *b, double *c) 
{
    a[0]=b[0]-c[0];
    a[1]=b[1]-c[1];
    a[2]=b[2]-c[2];
}
 
/***************************************************************************/
/*-------------------------------------------------------------------------*/
/*				Routine	:	intsplin_sphere
				Comment	:	
							intersection of sphere and line
*/
/***************************************************************************/
/*-------------------------------------------------------------------------*/
double intsplin_sphere(double *t, double *line, struct sphere *sp)
{/* 						t returns the parameter for where on the line */
    int k; 								/* the sphere is hit */
    double a,b,c,d,p,q,tt,sqrt();
 
    a=b=0.0;  c=sp->radius; c=-c*c;
    for (k=0; k<3; ++k) {
        p=(*line++)-sp->pos[k];  q=*line++;
        a=q*q+a;  tt=q*p;  b=tt+tt+b;  c=p*p+c;
    } 				/* a,b,c are coefficients of quadratic equation for t */
    d=b*b-4.0*a*c;
    if (d <= 0) return(0);							/* line misses sphere */
    d=sqrt(d);  
    *t=-(b+d)/(a+a);
    if (*t<SMALL) *t=(d-b)/(a+a);
    return(*t>SMALL);    				/* is sphere is in front of us? */
}
 /*-------------------------------------------------------------------------*/
/*				Routine	:	intsplin_lamp
				Comment	:	
							intersection of lamp and line
*/
/***************************************************************************/
/*-------------------------------------------------------------------------*/
double intsplin_lamp(double *t, double *line, struct lamp *lp)
{/* 						t returns the parameter for where on the line */
    int k; 								/* the sphere is hit */
    double a,b,c,d,p,q,tt,sqrt();
 
    a=b=0.0;  c=lp->radius; c=-c*c;
    for (k=0; k<3; ++k) {
        p=(*line++)-lp->pos[k];  q=*line++;
        a=q*q+a;  tt=q*p;  b=tt+tt+b;  c=p*p+c;
    } 				/* a,b,c are coefficients of quadratic equation for t */
    d=b*b-4.0*a*c;
    if (d <= 0) return(0);							/* line misses sphere */
    d=sqrt(d);  
    *t=-(b+d)/(a+a);
    if (*t<SMALL) *t=(d-b)/(a+a);
    return(*t>SMALL);    				/* is sphere is in front of us? */
}
/***************************************************************************/
/*-------------------------------------------------------------------------*/
/*				Routine	:	qintsplin
				Comment	:	
							same as intsplin, but we don't need t
*/
/***************************************************************************/
/*-------------------------------------------------------------------------*/
double qintsplin(double *line, struct sphere *sp)
{
 	double a,b,c,d,p,q;  
    int k;
 
    a=b=0.0;  
    c=sp->radius; 
    c=-c*c;
    for (k=0; k<3; ++k) {
        p=(*line++)-sp->pos[k];  q=*line++;
        a+=q*q;  b+=2.0*p*q;  c+=p*p;
    }
    d=b*b-4.0*a*c;  
    return(d > 0.0);
}
 
/***************************************************************************/
/*-------------------------------------------------------------------------*/
/*				Routine	:	inthor
				Comment	:	
							intersection of line with ground
*/
/***************************************************************************/
/*-------------------------------------------------------------------------*/
double inthor(double *t, double *line)
{
    if (line[5] == 0.0) return(0);
    *t=-line[4]/line[5];  
    return(*t > SMALL);
}
 
/***************************************************************************/
/*-------------------------------------------------------------------------*/
/*				Routine	:	genline
				Comment	:	
							generate the equation of a line
							through the two points a and b
*/
/***************************************************************************/
/*-------------------------------------------------------------------------*/
void genline( double *l, double *a, double *b)
{
/*    for (k=0; k<3; ++k) {*l++=a[k]; *l++=b[k]-a[k];} */
	*l++=a[0]; *l++=b[0]-a[0];
	*l++=a[1]; *l++=b[1]-a[1];
	*l++=a[2]; *l++=b[2]-a[2];
}
 
/***************************************************************************/
/*-------------------------------------------------------------------------*/
/*				Routine	:	dot
				Comment	:	
							dot produkt of 2 vectors
*/
/***************************************************************************/
/*-------------------------------------------------------------------------*/
double dot(double *a, double *b)
{
	return(a[0]*b[0]+a[1]*b[1]+a[2]*b[2]);
}
 
/***************************************************************************/
/*-------------------------------------------------------------------------*/
/*				Routine	:	point
				Comment	:	
	   						calculate position of a point on the
	   						line with parameter t
*/
/***************************************************************************/
/*-------------------------------------------------------------------------*/
void point(double *pos, double t, double *line)
{
    double a;
    
    a=*line++;  pos[0]=a+(*line++)*t;
    a=*line++;  pos[1]=a+(*line++)*t;
    a=*line++;  pos[2]=a+(*line++)*t;
}
 
/***************************************************************************/
/*-------------------------------------------------------------------------*/
/*				Routine	:	glint
				Comment	:	
	   						are we looking at a highlight ?
*/
/***************************************************************************/
/*-------------------------------------------------------------------------*/
int glint(double brite[3], struct patch *p, struct world *w, struct sphere *spc, double *incident)
{
    int k,l,firstlite;
    int emu1;  
    static double minglint=0.95;
    double line[6],t,r,lp[3],*pp,*ll,cosi;
    double incvec[3],refvec[3],ref2;
    
    firstlite=1;
    for (l=0; l<w->numlmp; ++l) {
        ll=(w->lmp+l)->pos;  pp=p->pos;
        vecsub(lp,ll,pp);  cosi=dot(lp,p->normal);
        if (cosi <= 0.0) continue; 					/* not with this lamp! */
        genline(line,pp,ll);
        for (k=0; k<w->numsp; ++k) {
            if (w->sp+k == spc) continue;
            if (intsplin_sphere(&t,line,w->sp+k)) goto cont;
        }
        if (firstlite) {
            incvec[0]=incident[1];  
			incvec[1]=incident[3];
            incvec[2]=incident[5];
            reflect(refvec,p->normal,incvec);
            ref2=dot(refvec,refvec);  firstlite=0;
        }
        r=dot(lp,lp);  t=dot(lp,refvec);
        t*=t/(dot(lp,lp)*ref2);
        if (t > minglint) { 							/* it's a highlight */
            for (k=0; k<3; ++k) brite[k]=1.0;
            return(1);
        }
    cont:
    	emu1 = emu1;
    }
    return(0);
}
 
/***************************************************************************/
/*-------------------------------------------------------------------------*/
/*				Routine	:	mirror
				Comment	:	
							bounce ray off mirror
*/
/***************************************************************************/
/*-------------------------------------------------------------------------*/
int mirror(double brite[3], struct patch *p, struct world *w, double *incident)
{
    double line[6],incvec[3],refvec[3],t;
    
    incvec[0]=incident[1];  
    incvec[1]=incident[3];
    incvec[2]=incident[5];  
    t=dot(p->normal,incvec);
    if (t >= 0) { 						/* we're inside a sphere, it's dark */
        brite[0] = brite[1] = brite[2] = 0.0;
        return(0);
    }
    reflect(refvec,p->normal,incvec);  line[0]=p->pos[0];
    line[2]=p->pos[1];  line[4]=p->pos[2];  line[1]=refvec[0];
    line[3]=refvec[1];  line[5]=refvec[2];
    raytrace(brite,line,w);  					/* recursion saves the day */
    brite[0]=brite[0]*p->color[0];
    brite[1]=brite[1]*p->color[1];
    brite[2]=brite[2]*p->color[2];
    return(1);
}
 
/***************************************************************************/
/*-------------------------------------------------------------------------*/
/*				Routine	:	pixbrite
				Comment	:	
	   						how bright is the patch
*/
/***************************************************************************/
/*-------------------------------------------------------------------------*/
void pixbrite( double brite[3], struct patch *p, struct world *w, struct sphere *spc)
{
      int k,l,emu1;
    double line[6],t,r,lp[3],*pp,*ll,cosi,diffuse;
    double sqrt();
    static double zenith[3]={0.0,0.0,1.0},f1=1.5,f2=0.4;
    
    diffuse=(dot(zenith,p->normal)+f1)*f2;
    for (k=0; k<3; ++k) brite[k]=diffuse*w->illum[k]*p->color[k];
    if (p && w) {
        for (l=0; l<w->numlmp; ++l) {
            ll=(w->lmp+l)->pos;  pp=p->pos;  vecsub(lp,ll,pp);
            cosi=dot(lp,p->normal);  if (cosi <= 0.0) goto cont;
            genline(line,pp,ll);
            for (k=0; k<w->numsp; ++k) {
                if (w->sp+k == spc) continue;
                if (intsplin_sphere(&t,line,w->sp+k)) goto cont;
            }
            r=sqrt(dot(lp,lp));  cosi=cosi/(r*r*r);
            for (k=0; k<3; ++k)
                brite[k]=brite[k]+cosi*p->color[k]
                         *w->lmp[l].color[k];
            cont:
            emu1 = emu1;
        }
    }
}
 
/***************************************************************************/
/*-------------------------------------------------------------------------*/
/*				Routine	:	setnorm
				Comment	:	
	   						normal (radial) direction of sphere
*/
/***************************************************************************/
/*-------------------------------------------------------------------------*/
void setnorm(struct patch *p, struct sphere *s)
{
    double *t,a;  
    
    vecsub(t=p->normal,p->pos,s->pos);  
    a=1.0/s->radius;
    *t=(*t)*a; ++t;
    *t=(*t)*a; ++t;
    *t=(*t)*a; ++t;
}
 
/***************************************************************************/
/*-------------------------------------------------------------------------*/
/*				Routine	:	colorcpy
				Comment	:	
	   						a = b for colors
*/
/***************************************************************************/
/*-------------------------------------------------------------------------*/
void colorcpy(double *a, double *b)
{
	a[0] = b[0];
	a[1] = b[1];
	a[2] = b[2];
}
 
/***************************************************************************/
/*-------------------------------------------------------------------------*/
/*				Routine	:	veccopy
				Comment	:	
	   						a = b for vectors
*/
/***************************************************************************/
/*-------------------------------------------------------------------------*/
void veccopy(double *a, double *b)
{
	a[0] = b[0];
	a[1] = b[1];
	a[2] = b[2];
}
 
/***************************************************************************/
/*-------------------------------------------------------------------------*/
/*				Routine	:	gingham
				Comment	:	
	   						are we on black or white tile ?
*/
/***************************************************************************/
/*-------------------------------------------------------------------------*/
double gingham(double *pos)
{       										/* tiles are 3 units wide */
    double x,y;  int kx,ky;
    
    kx=ky=0;  x=pos[0]; y=pos[1];
    if (x < 0) {x=-x; ++kx;}
    if (y < 0.0) {y=-y; ++ky;}
    return(((((int)x)+kx)/3+(((int)y)+ky)/3)%2);
}
 
/***************************************************************************/
/*-------------------------------------------------------------------------*/
/*				Routine	:	reflect
				Comment	:	
							law of reflection, n is unit normal
							x is incoming ray
							y is outgoing ray
*/
/***************************************************************************/
/*-------------------------------------------------------------------------*/
void reflect(double *y, double *n, double *x)
{
    double u[3],v[3],vv,xn,xv;  
    
    vecprod(u,x,n);      /* normal to the plane of n and y */
    if (veczero(u)) {    /* bounce right back */
        y[0]=-x[0];  y[1]=-x[1];  y[2]=-x[2];  return;
    }
    vecprod(v,u,n);          /* u,v and n are orthogonal */
    vv=dot(v,v);  xv=dot(x,v)/vv;  xn=dot(x,n);
/*  for (k=0; k<3; ++k) y[k]=xv*v[k]/(xn*n[k]);					    wrong!  */
    y[0]=xv*v[0]-(xn*n[0]); 									/* right   */
    y[1]=xv*v[1]-(xn*n[1]); 									/* right   */
    y[2]=xv*v[2]-(xn*n[2]); 									/* right   */
}
 
/***************************************************************************/
/*-------------------------------------------------------------------------*/
/*				Routine	:	vecprod
				Comment	:	
							vector product a = b^c
*/
/***************************************************************************/
/*-------------------------------------------------------------------------*/
void vecprod(double *a, double *b, double *c)
{

    a[0]=b[1]*c[2]-b[2]*c[1];
    a[1]=b[2]*c[0]-b[0]*c[2];
    a[2]=b[0]*c[1]-b[1]*c[0];
}
 
/***************************************************************************/
/*-------------------------------------------------------------------------*/
/*				Routine	:	veczero
				Comment	:	
	   						is vector null
*/
/***************************************************************************/
/*-------------------------------------------------------------------------*/
int veczero(double *v)
{
    if (fabs(v[0]) > SMALL)	return(0);  
    if (fabs(v[1]) > SMALL)	return(0);
    if (fabs(v[2]) > SMALL)	return(0); 
	return(1);
}



