#define BIG 1.0e10
#define SMALL 1.0e-3

#define DULL    0
#define BRIGHT  1
#define MIRROR  2

#define SCREEN_WIDTH 640
#define SCREEN_HEIGHT 480

/***************************************************************************/
struct lamp {
    double pos[3];					/* position of lamp */
    double color[3];					/* color of lamp */
    double radius;					/* size of lamp */
};
 
struct sphere {
    double pos[3];					/* position of sphere */
    double color[3];				/* color of sphere */
    double radius;					/* size of sphere */
    int type;						/* type of surface, DULL, BRIGHT or MIRROR */
};
 
struct patch {						/* a small bit of something visible */
    double pos[3];					/* position */
    double normal[3];				/* direction 90 degrees to surface */
    double color[3];				/* color of patch */
};
 
struct world {  					/* everything in the universe, except observer */
    int numsp;						/* number of spheres */
    struct sphere *sp;				/* array of spheres */
    int numlmp;						/* number of lamps */
    struct lamp *lmp;				/* array of lamps */
    struct patch horizon[2];		/* alternate squares on the ground */
    double illum[3];				/* background diffuse illumination */
    double skyhor[3];				/* sky color at horizon */
    double skyzen[3];				/* sky color overhead */
};
 
struct observer {					/* now the observer */
    double obspos[3];				/* his position */
    double viewdir[3];				/* direction he is looking */
    double uhat[3];					/* left to right in view plane */
    double vhat[3];					/* down to up in view plane */
    double fl,px,py;				/* focal length and pixel sizes */
    int nx,ny;						/* number of pixels */
};

void main_raytrace(void);
void raytrace(double brite[3], double *line, struct world *w);
void skybrite(double brite[3], double *line, struct world *w);
void pixline(double *line, struct observer *o, int i, int j);
void vecsub(double *a,double *b, double *c);
double intsplin_sphere(double *t, double *line, struct sphere *sp);
double intsplin_lamp(double *t, double *line, struct lamp *lp);
double qintsplin(double *line, struct sphere *sp);
double inthor(double *t, double *line);
void genline( double *l, double *a, double *b);
double dot(double *a, double *b);
void point(double *pos, double t, double *line);
int glint(double brite[3], struct patch *p, struct world *w, struct sphere *spc, double *incident);
int mirror(double brite[3], struct patch *p, struct world *w, double *incident);
void pixbrite( double brite[3], struct patch *p, struct world *w, struct sphere *spc);
void setnorm(struct patch *p, struct sphere *s);
void colorcpy(double *a, double *b);
void veccopy(double *a, double *b);
double gingham(double *pos);
void reflect(double *y, double *n, double *x);
void vecprod(double *a, double *b, double *c);
int veczero(double *v);

void setup_world(struct observer *o, struct world *w, int *skip);

// rtc3.c
void setup_screen(void);
void write_bitmap(void);
void emu_set_pix(int x, int y, int r, int g, int b);
void loop (void);
void cleanup (char *s);
void write_bitmap(void);

