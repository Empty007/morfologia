 /*
  * Khoros: $Id: toolboxes.h,v 1.3 1991/12/18 08:53:31 dkhoros Exp $
  */

 /*
  * $Log: toolboxes.h,v $
 * Revision 1.3  1991/12/18  08:53:31  dkhoros
 * HellPatch3
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

#ifndef _toolboxes_h_
#define _toolboxes_h_

/* >>>>>>>>>>>>>>>>>>>>>>>>>>>>> <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
   >>>>                                                          <<<<
   >>>>      file: toolboxes.h                                   <<<<
   >>>>                                                          <<<<
   >>>>      contains:  Different toolboxes currently installed  <<<<
   >>>>                 or the user wishes to use.  A toolbox is <<<<
   >>>>                 a set of contributed routines that are   <<<<
   >>>>                 not part of the core Khoros system.      <<<<
   >>>>                                                          <<<<
   >>>>      written by: Mark Young      			 <<<<
   >>>>                                                          <<<<
   >>>>>>>>>>>>>>>>>>>>>>>>>>>>> <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< */

#define DEFAULT_TOOLBOX_FILE "KHOROS_HOME/repos/Toolboxes"

typedef struct _khoros_toolboxes
{
	char	*variable;
	char	*path;
	char	*title;
	char	*description;
	char	*author;
	char	*info_file;
	struct _khoros_toolboxes *next;

} Khoros_toolboxes;

char		 *khoros_toolbox_file;
Khoros_toolboxes *khoros_toolbox_list;

char *vtoolbox_path();
char *vtoolbox_variable();
char **vtoolbox_list2strlist();
int vtoolbox_add();
int vtoolbox_delete();
int vtoolbox_write();
Khoros_toolboxes *vtoolbox_list();
Khoros_toolboxes *vtoolbox_entry();

#endif /* _toolboxes_h_ */
/* Don't add after this line */
