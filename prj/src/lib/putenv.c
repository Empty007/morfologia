 /*
  * Khoros: $Id: putenv.c,v 1.2 1992/03/20 23:39:30 dkhoros Exp $
  */

#if !defined(lint) && !defined(SABER)
static char rcsid[] = "Khoros: $Id: putenv.c,v 1.2 1992/03/20 23:39:30 dkhoros Exp $";
#endif

 /*
  * $Log: putenv.c,v $
 * Revision 1.2  1992/03/20  23:39:30  dkhoros
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
#include "vgparm.h"	


/* >>>>>>>>>>>>>>>>>>>>>>>>>>>>> <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
   >>>>                                                       <<<<
   >>>>	    file name: putenv.c                               <<<<
   >>>>                                                       <<<<
   >>>>   description: File Utility                           <<<<
   >>>>                                                       <<<<
   >>>>      routines: vputenv()			      <<<<
   >>>>		       vremenv()			      <<<<
   >>>>                                                       <<<<
   >>>> modifications:					      <<<<
   >>>>                                                       <<<<
   >>>>>>>>>>>>>>>>>>>>>>>>>>>>> <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< */



/**************************************************************
*
* MODULE NAME: vputenv
*
*     PURPOSE: This module is used to put an environment variable
*	       in the environ list.  If "PUTENV" is defined then
*	       we call putenv() otherwise we do our own putenv.
*
*       INPUT: 	name - the string to be added into putenv
*
*      OUTPUT:  none
*
* CALLED FROM: application program
*
* WRITTEN BY:  Mark Young
*
**************************************************************/


int vputenv(name)

char *name;
{
#ifndef PUTENV

	int    size, num = 1;
	char   **env, *temp;
	extern char **environ;
	static int init = FALSE;

	if (name == NULL)
	   return(1);

	if ((temp = strchr(name, '=')) != NULL)
	   size = temp - name;
	else
	   size = VStrlen(name);

	env = environ;
	while (*env != NULL)
	{
	   if (strncmp(*env, name, size) == 0)
	   {
	      if (init) free(*env);
	      *env = name;
	      return(0);
	   }
	   env++; num++;
	}

	/*
	 *  If we are changing the environ for the first time then we need
	 *  copy the current environ into an array of strings allocated by
	 *  malloc.
	 */
	if (init == FALSE)
	{
	   if ((env = (char **) malloc(sizeof(char *) * (num+1))) == NULL)
	      return(1);

	   memcpy((char *) env, (char *) environ, sizeof(char *) * (num-1));
	   init = TRUE;
	}
	else
	{
	   env = environ;
	   if ((env = (char **) realloc(env, sizeof(char *) * (num+1))) == NULL)
	      return(1);
	}
	env[num -1] = name;
	env[num]    = NULL;
	environ	    = env;
	return(0);
#else
	return(putenv(name));
#endif
}



/**************************************************************
*
* MODULE NAME: vremenv
*
*     PURPOSE: This module is used to remove an environment variable
*	       from the environ list.  Since there is no consensus
*	       on whether this routines exists on many machines, we
*	       simply write our own.
*
*	       "this routine was added purely so that experimental
*		progress with the transport library could continue.
*		I do not expect this routine to survive the transisition
*		as Khoros 1.0 goes to Khoros 2.0  <mark - feb, 1992>"
*
*       INPUT: 	name - the string to be removed from the environment list
*
*      OUTPUT:  none
*
* CALLED FROM: application program
*
* WRITTEN BY:  Mark Young
*
**************************************************************/


int vremenv(name)

char *name;
{
	int    size;
	char   **env;
	extern char **environ;

	if (name == NULL)
	   return(1);

	size = VStrlen(name);
	env = environ;
	while (*env != NULL)
	{
	   if (strncmp(*env, name, size) == 0)
	   {
	      do
	      {
		 *env++ = *(env+1);
	      } while (*env != NULL);
	      return(0);
	   }
	   env++;
	}
	return(1);
}
