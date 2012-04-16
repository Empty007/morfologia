 /*
  * Khoros: $Id: vinclude.h,v 1.5 1992/03/25 17:29:22 dkhoros Exp $
  */

 /*
  * $Log: vinclude.h,v $
 * Revision 1.5  1992/03/25  17:29:22  dkhoros
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

#ifndef _vinclude_h_
#define _vinclude_h_

/* >>>>>>>>>>>>>>>>>>>>>>>>>>>>> <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
   >>>>                                                          <<<<
   >>>>      file:  vinclude.h					 <<<<
   >>>>                                                          <<<<
   >>>>      contains: #includes needed by all Khoros programs   <<<<
   >>>>                                                          <<<<
   >>>>	     written by:  Ron E. Neher				 <<<<
   >>>>                                                          <<<<
   >>>>      date:  3/29/88					 <<<<    
   >>>>                                                          <<<<
   >>>>      modifications: Rasure on 2/6/89                     <<<<
   >>>>                     Cyrus added venviron.h 5/90          <<<<
   >>>>                     Scott Wilson - Switch position of    <<<<
   >>>>                     viff.h and vdefines.h to eliminate   <<<<
   >>>>                     forward include reference that is    <<<<
   >>>>                     not used anymore.                    <<<<
   >>>>                                                          <<<<
   >>>>>>>>>>>>>>>>>>>>>>>>>>>>> <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<  */
   

#if defined(SYSV)
#    if defined(_IBMR2) || defined(CRAY)
#        include <fcntl.h>
#    else
#        include <sys/fcntl.h>
#    endif
#    include	<unistd.h>
#endif

#include <stdio.h>

#if defined(AUX)
#    include <stdlib.h>
#    include <stddef.h>
#    include <unistd.h>
#endif 

#include <signal.h>
#include <setjmp.h>
#include <ctype.h>
#include <pwd.h>
#include <errno.h>


#include <sys/types.h>
#include <sys/file.h>
#include <sys/time.h>
#include <sys/dir.h>

#if !defined(__convex__)
#   include <sys/param.h>
#endif

#include <sys/stat.h>

#if defined(apollo)
#   if !defined(SYSV)
#       include <strings.h>
#   else
#       include <string.h>
#   endif
#else
#   include <string.h>
#endif

/*
 *  Define directory entry structure to be either direct or dirent.  Depending
 *  if we are system POSIX compliant or on a CRAY running UNICOS.
 */
#ifdef DIRENT
#include        <dirent.h>
typedef struct dirent vdirect;

#else
typedef struct direct vdirect;
#endif

#include "khoros.h"
#include "viff.h"	  
#include "vmath.h"	
#include "keywords.h"
#include "toolboxes.h"
#include "fortran.h"
#include "vdefines.h"	
#include "kfile.h"	

#endif /* _vinclude_h_ */
/* Don't add after the endif */
