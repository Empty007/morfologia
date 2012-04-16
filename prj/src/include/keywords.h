 /*
  * Khoros: $Id: keywords.h,v 1.2 1991/10/01 23:51:27 khoros Exp $
  */

 /*
  * $Log: keywords.h,v $
 * Revision 1.2  1991/10/01  23:51:27  khoros
 * HellPatch2
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
#include "unmcopyright.h"        /* Copyright 1990 by UNM */

#ifndef _keywords_h_
#define _keywords_h_

/* >>>>>>>>>>>>>>>>>>>>>>>>>>>>> <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
   >>>>                                                          <<<<
   >>>>      file: keywords.h                                    <<<<
   >>>>                                                          <<<<
   >>>>      contains:  Keywords structure definition 		 <<<<
   >>>>			for the use of keywords 	 	 <<<<
   >>>>			shorthand in specifying input files for  <<<<
   >>>>			Khoros programs				 <<<<
   >>>>                                                          <<<<
   >>>>      written by: Danielle Argiro 			 <<<<
   >>>>                                                          <<<<
   >>>>>>>>>>>>>>>>>>>>>>>>>>>>> <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< */


#define DEFAULT_KEYWORD_FILE "KHOROS_HOME/repos/Keywords"

typedef struct _khoros_keywords {
	char	*keyword;
	char	*fullpath;
	struct _khoros_keywords *next;
	} Khoros_keywords;

Khoros_keywords *khoros_keyword_list;
char *khoros_keyword_file;


#endif /* _keywords_h_ */
/* Don't add after this line */

