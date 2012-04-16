 /*
  * Khoros: $Id: machtype.c,v 1.4 1992/03/20 23:40:43 dkhoros Exp $
  */

#if !defined(lint) && !defined(SABER)
static char rcsid[] = "Khoros: $Id: machtype.c,v 1.4 1992/03/20 23:40:43 dkhoros Exp $";
#endif

 /*
  * $Log: machtype.c,v $
 * Revision 1.4  1992/03/20  23:40:43  dkhoros
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

/*
    Written: Scott Wilson, Tom Sauer, Mark Young
    Date:    13-Jan-1988
    Modified: 5-Apr-89 SRW Added support for VME Delta 68030 Series
              Thu Jun 28 16:42:57 MDT 1990 changed call and now uses
		                           the include file machdefs.h
              2-Aug-90 SRW Fixed DEC/Mips support, added MIPS support
              2-Aug-90 SRW Added Silicon Graphics support
              1991 Completely rewritten. Jeremey Worley and Mark Young
*/

#include "unmcopyright.h"	 /* Copyright 1990 by UNM */
#include "vinclude.h"	
#include "machdefs.h"	

long
machtype(name)
char *name;
{
	if (name != NULL)
	   (void) strcpy(name, LocalDef.hosttype);

	return(LocalDef.order);
}
