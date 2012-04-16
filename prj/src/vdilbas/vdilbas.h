 /*
  * Khoros: $Id$
  */

 /*
  * $Log$
  */

/*
 *----------------------------------------------------------------------
 *
 * Copyright 1992, University of New Mexico.  All rights reserved.
 * Permission to copy and modify this software and its documen-
 * tation only for internal use in your organization is hereby
 * granted, provided that this notice is retained thereon and
 * on all copies.  UNM makes no representations as to the sui-
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
 * No other rights, including, for example, the right to redis-
 * tribute this software and its documentation or the right to
 * prepare derivative works, are granted unless specifically
 * provided in a separate license agreement.
 *---------------------------------------------------------------------
 */

#include "unmcopyright.h"        /* Copyright 1992 by UNM */


/*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>  <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
 >>>>
 >>>>         File Name: vdilbas.h
 >>>>            Author: Guillermo Pablo Salas
 >>>> Date Last Updated: Wed Jul 19 09:49:21 1995
 >>>> 
 >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>   <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

#ifndef _vdilbas_h_
#define _vdilbas_h_

#include "vinclude.h"


/* -include_includes */
/* -include_includes_end */

char	**av,		/* argv and argc used by gparams */
	*program;	/* a pointer to the program name argv[0] */
int	ac;

/****************************************************************
*
*                    vdilbas Argument Structure
*
****************************************************************/

typedef struct _vdilbas  {

/*
 *  input - basis  (Required infile)
 */
char	*i_file;	/* input - basis  FILENAME */
int	 i_flag;	/* input - basis  FLAG */

/*
 *  Structuring Element (Required infile)
 */
char	*str_file;	/* Structuring Element FILENAME */
int	 str_flag;	/* Structuring Element FLAG */

/*
 *  basis dilate (Required outfile)
 */
char	*o_file;	/* basis dilate FILENAME */
int	 o_flag;	/* basis dilate FLAG */

}  vdilbas_struct;

vdilbas_struct *vdilbas;

/* -include_additions */
/* -include_additions_end */

/* -include_macros */
/* -include_macros_end */

#endif
