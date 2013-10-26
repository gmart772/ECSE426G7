#define OUTX0 2
#define OUTX1 1
#define OUTX2 2
#define OUTX3 1

#define OUTY0 2
#define OUTY1 1
#define OUTY2 2
#define OUTY3 1

#define OFFX (OUTX0 + OUTX1 + OUTX2 + OUTX3)/4
#define OFFY (OUTY0 + OUTY1 + OUTY2 + OUTY3)/4

#define SENSX (OUTX2 - OUTX1)/2
#define SENSY (OUTY0 - OUTY3)/2

#define CXY (OUTX0 - OUTX3)/(2 * SENSX)
#define CYX (OUTY2 - OUTY1)/(2 * SENSY)
