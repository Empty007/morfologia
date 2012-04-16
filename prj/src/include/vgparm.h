 /*
  * Khoros: $Id: vgparm.h,v 1.2 1992/03/20 23:40:01 dkhoros Exp $
  */

 /*
  * $Log: vgparm.h,v $
 * Revision 1.2  1992/03/20  23:40:01  dkhoros
 * VirtualPatch5
 *
  */ 


/*
 *----------------------------------------------------------------------
 *
 * Copyright 1990, University of New Mexico.  All rights reserved.

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
 *---------------------------------------------------------------------
 */

#include "unmcopyright.h"	 /* Copyright 1990 by UNM */


/* >>>>>>>>>>>>>>>>>>>>>>>>>>>>> <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
   >>>>                                                       <<<<
   >>>>	    file name: vgparm.h                               <<<<
   >>>>                                                       <<<<
   >>>>   description: indescribable                          <<<<
   >>>>                                                       <<<<
   >>>> modifications:					      <<<<
   >>>>                                                       <<<<
   >>>>>>>>>>>>>>>>>>>>>>>>>>>>> <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< */


#ifndef _VGPARMS_H_
#define _VGPARMS_H_

#include "vinclude.h"	

/*
 * Varargs parameter list handling...YECH!!!!
 */
#if !defined(__STDC__)
#include <varargs.h>
#define	VA_START(arg, param)	va_start(arg)
#else
#include <stdarg.h>
#define	VA_START(arg, param)	va_start(arg, param)
#endif

#define VA_END(arg) va_end(arg)

#define MAX_ARGS 200
#define MAX_ARG_LEN 512
#define MAX_LINE_LEN 512
#define MAX_KEY_LEN 40
#define BUFFERSIZE 512
#define MODE 00644

typedef struct arg_entry {
		char key[MAX_KEY_LEN];
		char sarg[MAX_ARG_LEN];
		struct arg_entry *next;
		} ARG_ENTRY;

char *_cleanup_string();
char *_expand_tilda();
char *_expand_variable();

#endif /* _VGPARMS_H_ */
/* don`t add after the endif */
