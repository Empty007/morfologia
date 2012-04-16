 /*
  * Khoros: $Id: vmath.h,v 1.3 1992/03/20 22:35:06 dkhoros Exp $
  */

 /*
  * $Log: vmath.h,v $
 * Revision 1.3  1992/03/20  22:35:06  dkhoros
 * VirtualPatch5
 *
  */ 

/*
 *----------------------------------------------------------------------
 *
 * Copyright 1990, University of New Mexico.  All rights reserved.
 * 
 * Permission to copy and modify this software and its documen-
 * tation only for internal use in your organization is hereby
 * granted, provided that this notice is retained thereon and
 * on all copies.  UNM makes no representations as too the sui-
 * tability and operability of this software for any purpose.
 * It is provided "as is" without express or implied warranty.
 * 
 * UNM DISCLAIMS ALL WARRANTIES WITH REGARD TO THIS SOFTWARE,
 * INCLUDING ALL IMPLIED WARRANTIES OF MERCHANTABILITY AND FIT-
 * NESS.  IN NO EVENT SHALL UNM BE LIABLE FOR ANY SPECIAL,
 * INDIRECT OR CONSEQUENTIAL DAMAGES OR ANY OTHER DAMAGES WHAT-
 * SOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER
 * IN AN ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS
 * ACTION, ARISING OUT OF OR IN CONNECTION WITH THE USE OR PER-
 * FORMANCE OF THIS SOFTWARE.
 * 
 * No other rights, including for example, the right to redis-
 * tribute this software and its documentation or the right to
 * prepare derivative works, are granted unless specifically
 * provided in a separate license agreement.
 *----------------------------------------------------------------------
 */

#include "unmcopyright.h"	 /* Copyright 1990 by UNM */

#ifndef _vmath_h
#define _vmath_h

/* >>>>>>>>>>>>>>>>>>>>>>>>>>>>> <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
   >>>>                                                          <<<<
   >>>>      file: vmath.h					 <<<<
   >>>>                                                          <<<<
   >>>>      contains:  #defines for khoros math constants       <<<<
   >>>>                                                          <<<<
   >>>>      date:                                               <<<<
   >>>>                                                          <<<<
   >>>>      written by: 					 <<<<
   >>>>                                                          <<<<
   >>>>      modifications:  Rasure for Khoros, 2/6/89           <<<<
   >>>>                                                          <<<<
   >>>>>>>>>>>>>>>>>>>>>>>>>>>>> <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<  */


#include <math.h>

/*
 *  Definitions for some possibly machine dependent constants, more dependable
 *  than math.h since math.h is often missing (Way to go SYS V!).  
 *  Some installations may want to put values in here that more 
 *  closely match the machine.
 */


#define XV_MAXFLOAT	1.0e+38       /* Good enough for most applications */
#define XV_MAXLINT	2147483647    /* Assuming 32 bit 2's complement */
#define XV_MAXSINT	32767         /* Assuming 16 bit 2's complement */

#define XV_GAMMA	0.57721566490153286060
#define XV_DEG	       57.29577951308232087680
#define XV_PHI		1.61803398874989484820
#define XV_E		2.71828182845904523536
#define XV_LOG2E	1.4426950408889634074
#define XV_LOG10E	0.43429448190325182765
#define XV_LN2		0.69314718055994530942
#define XV_LN10		2.30258509299404568402
#define XV_PI		3.14159265358979323846
#define XV_2PI		(XV_PI * 2)
#define XV_PI_2		1.57079632679489661923
#define XV_PI_4		0.78539816339744830962
#define XV_1_PI		0.31830988618379067154
#define XV_2_PI		0.63661977236758134308
#define XV_SQRTPI	1.77245385090551602730
#define XV_SQRT2	1.41421356237309504880
#define XV_SQRT1_2	0.70710678118654752440

#undef  MAX
#define MAX(x,y) (((x) > (y)) ? (x) : (y))

#undef  MIN
#define MIN(x,y) (((x) < (y)) ? (x) : (y))

#undef  ABS
#define ABS(x) (((x) >= 0)  ? (x) : (-(x)))

/*
 * declair the math functions in the vmath library
 */
double kfmod();
double absolute();
double bess();
int powtwo();
double dpower();
double fact();
double vfact();
double vstep();
double vgnoise();
double vunoise();
float urng();
long vrandom();
double vcabs();
double dvcabs();
double cang();
double dcang();

#endif /* _vmath_h */
/* Don't add after the endif */
