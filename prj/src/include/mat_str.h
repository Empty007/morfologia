/* Struct. el. type */

#define  MM_IMG     0
#define  MM_MAT     1
#define  MM_GRAPH   2


/* Node of a graph */

typedef struct {
   int n;                   /* number of node neighbors */
   int val;                 /* node value */
   int x;                   /* node coordinates */
   int y;
   int *neig;               /* pointer to the first element
                               of an array of node indices */
} mm_node;


/* Graph based structuring function */

typedef struct {
   int n;                    /* graph number of nodes */
   mm_node *nodes;           /* pointer to the first element of an
                                array of nodes */    
} mm_graph;


/* Generic structuring element: */

typedef struct _gen_str {
  int tag;
  int m, n, p;     /* dimensions */
  int x[3][3];     /* elementary matrix */
  short *dat;      /* general matrix */
  mm_graph g;      /* graph based str. el. */
} mat_str;

