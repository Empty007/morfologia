 /*
  * Khoros: $Id: strings.c,v 1.4 1992/03/20 23:39:36 dkhoros Exp $
  */

#if !defined(lint) && !defined(SABER)
static char rcsid[] = "Khoros: $Id: strings.c,v 1.4 1992/03/20 23:39:36 dkhoros Exp $";
#endif

 /*
  * $Log: strings.c,v $
 * Revision 1.4  1992/03/20  23:39:36  dkhoros
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
   >>>>	    file name: string.c                               <<<<
   >>>>                                                       <<<<
   >>>>   description: File utility                           <<<<
   >>>>                                                       <<<<
   >>>>      routines: vlower_string()			      <<<<
   >>>>		       vupper_string()			      <<<<
   >>>>		       vreplace_string()		      <<<<
   >>>>		       vreplace_char()			      <<<<
   >>>>		       vcleanup_string()		      <<<<
   >>>>                                                       <<<<
   >>>>		       vstrstr()			      <<<<
   >>>>		       vstrpbrk()			      <<<<
   >>>>		       vstrtok()			      <<<<
   >>>>                                                       <<<<
   >>>> modifications:					      <<<<
   >>>>                                                       <<<<
   >>>>>>>>>>>>>>>>>>>>>>>>>>>>> <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< */

/**************************************************************
*
* MODULE NAME: vlower_string
*
*     PURPOSE: This function returns the lower case string for
*	       supplied input string.
*
*
*       INPUT: input_string    - the file to be expanded
*
*	       output_string - if the output string is not NULL then
*			     this will be where we place the lowered
*			     string.  Otherwise we malloc enough space
*			     to lower the string.
*	       
*        
*      OUTPUT: the lower case string
*
* CALLED FROM: 
*
*  WRITTEN BY:  Mark Young & Tom Sauer
*
**************************************************************/


char *vlower_string(input_string, output_string)

char	*input_string, *output_string;
{
	char    *string;


	if (input_string == NULL)
	{
	   (void) fprintf(stderr,"\nvlower_string:\n");
	   (void) fprintf(stderr,"   Error!  NULL input string encountered.\n");
	   return(NULL);
	}

	/*
	 *  If the output_string is NULL then VStrcpy the input string
	 */
	if (output_string == NULL)
	   output_string = VStrcpy(input_string);
	else if (input_string != output_string)
	   strcpy(output_string, input_string);

	string = output_string;
	while (*string != '\0')
	{
	   if (isupper(*string))
	      *string = tolower(*string);

	   string++;
	}
	return(output_string);
}



/**************************************************************
*
* MODULE NAME: vupper_string
*
*     PURPOSE: This function returns the upper case string for
*	       supplied input string.
*
*
*       INPUT: input_string    - the file to be expanded
*
*	       output_string - if the output string is not NULL then
*			     this will be where we place the uppered
*			     string.  Otherwise we malloc enough space
*			     to upper the string.
*	       
*        
*      OUTPUT: the upper case string
*
* CALLED FROM: 
*
*  WRITTEN BY:  Mark Young & Tom Sauer
*
**************************************************************/


char *vupper_string(input_string, output_string)

char	*input_string, *output_string;
{
	char    *string;


	if (input_string == NULL)
	{
	   (void) fprintf(stderr,"\nvupper_string:\n");
	   (void) fprintf(stderr,"   Error!  NULL input string encountered.\n");
	   return(NULL);
	}

	/*
	 *  If the output_string is NULL then VStrcpy the input string
	 */
	if (output_string == NULL)
	   output_string = VStrcpy(input_string);
	else if (input_string != output_string)
	   strcpy(output_string, input_string);

	string = output_string;
	while (*string != '\0')
	{
	   if (islower(*string))
	      *string = toupper(*string);

	   string++;
	}
	return(output_string);
}



/**************************************************************
*
* MODULE NAME: vreplace_string
*
*     PURPOSE:  This function performs a global change of text on
*		the input string.  It returns a string where every
*		occurrence of the scan string is replaced with the 
*		replacement string.  If output_string is sent in as NULL,
*		the result will be malloc'ed for you.  If output_string
*		is provided as non-null, the result will be stored in
*		output_string.  Note that if the latter use is chosen,
*		output_string must be previously allocated with a sufficient
*		amount of storage space before this routine is called.
*
*		For example, if we call:
*		new_string = vreplace_string("Welcome to Khoros", "Khoros", 
*					     "the Twilight Zone", NULL);
*		the result will be 
*		new_string = "Welcome to the Twilight Zone".
*
*       INPUT:  input_string - the string to be changed.
*		scan_str - the string indicating the text to be replaced
*		replace_str - the string indicating the text replace with
*		output_string - NULL if you want the routine to malloc for you,
*			        non-NULL if you want it to use previously
*				allocated space.
*
*      OUTPUT:  the original string with the indicated change made
*
*  WRITTEN BY:  Mark Young
*
**************************************************************/


char *vreplace_string(input_string, scan_str, replace_str, output_string)

char	*input_string, *output_string;
char	*scan_str, *replace_str;
{
	char    *string, *temp = NULL;
	int	ilen, rlen, size, length, numchar;


	if (input_string == NULL)
	{
	   (void) fprintf(stderr,"\nvreplace_string:\n");
	   (void) fprintf(stderr,"   Error!  NULL input string encountered.\n");
	   return(NULL);
	}

	if (scan_str == NULL)
	{
	   (void) fprintf(stderr,"\nvreplace_string:\n");
	   (void) fprintf(stderr,"   Error!  NULL scan string encountered.\n");
	   return(NULL);
	}

	ilen = VStrlen(scan_str);
	rlen = VStrlen(replace_str);
	length = MAX(rlen, LENGTH);

	/*
	 *  If the output_string is NULL then we need to allocate temporary
	 *  space for the string.  Another problem case is when the input
	 *  and output strings are the same but the replacement token is
	 *  larger than the input token.  In this case we use a temporary
	 *  array and then strcpy the result into the output array, hoping
	 *  that user was smart enough to malloc enough space for us.
	 */
	if ((input_string == output_string && rlen > ilen) ||
	    (output_string == NULL))
	{
	   size = length;
	   if ((temp = (char *) malloc((unsigned) sizeof(char) * size)) == NULL)
	   {
	      (void) fprintf(stderr,"\nvreplace_string:\n");
	      (void) fprintf(stderr,"  Error! Out of memory for temporary \
string array.  Tried to malloc '%d' bytes.\n", size);
	      return(NULL);
	   }
	   string = temp;
	}
	else
	   string = output_string;

	while (*input_string != '\0')
	{
	   if (temp != NULL)
	   {
	      numchar = string - temp;
	      if (numchar >= (size-rlen-1))
	      {
	         size += length;
	         if ((temp = (char *) realloc(temp, size)) == NULL)
	         {
		    (void) fprintf(stderr,"\nvreplace_string:\n");
		    (void) fprintf(stderr,"  Error! Out of memory for temporary\
 string array.  Tried to re-alloc '%d' bytes.\n", size);
		    return(NULL);
	         }
	         string = temp + numchar;
	      }
	   }

	   if (strncmp(input_string, scan_str, ilen) == 0)
	   {
	      strncpy(string, replace_str, rlen);
	      string += rlen;
	      input_string += ilen;
	   }
	   else
	      *string++ = *input_string++;
	}
	*string = '\0';

	/*
	 *  Check to see if the output string should be malloc'ed or if
	 *  the input and output strings are the same but we used a temporary
	 *  since the input token was smaller than the replacement token.
	 */
	if (output_string == NULL)
	{
	   output_string = VStrcpy(temp);
	   free(temp);
	}
	else if (input_string == output_string && rlen > ilen)
	{
	   strcpy(output_string, temp);
	   free(temp);
	}
	return(output_string);
}



/**************************************************************
*
* MODULE NAME: vreplace_char
*
*     PURPOSE:  This function performs a global change of character on
*		the input string.  It returns a string where every
*		occurrence of the scan character is replaced with the 
*		replacement character.  If output_string is sent in as NULL,
*		the result will be malloc'ed for you.  If output_string
*		is provided as non-null, the result will be stored in
*		output_string.  Note that if the latter use is chosen,
*		output_string must be previously allocated with a sufficient
*		amount of storage space before this routine is called.
*
*		For example, if we call:
*		new_string = vreplace_char("Many fishes", "f", "w", NULL);
*		the result will be 
*		new_string = "Many wishes".
*
*       INPUT:  input_string - the string to be changed.
*		scan_char - the character to be replaced
*		replace_char - the character to replace with
*		output_string - NULL if you want the routine to malloc for you,
*			        non-NULL if you want it to use previously
*				allocated space.
*
*      OUTPUT:  the original string with the indicated change made
*
*  WRITTEN BY:  Mark Young
*
**************************************************************/


char *vreplace_char(input_string, scan_char, replace_char, output_string)

char	*input_string, *output_string;
char	scan_char, replace_char;
{
	char    *string;


	if (input_string == NULL)
	{
	   (void) fprintf(stderr,"\nvreplace_char:\n");
	   (void) fprintf(stderr,"   Error!  NULL input string encountered.\n");
	   return(NULL);
	}

	/*
	 *  If the output_string is NULL then VStrcpy the input string
	 */
	if (output_string == NULL)
	   output_string = VStrcpy(input_string);
	else if (input_string != output_string)
	   strcpy(output_string, input_string);

	string = output_string;
	while (string != NULL)
	{
	   if ((string = strchr(string, scan_char)) != NULL)
	   {
	      *string++ = replace_char;
	      if (*string == '\0') break;
	   }
	}
	return(output_string);
}



/**************************************************************
*
* MODULE NAME: vcleanup_string
*
*     PURPOSE: 	This function is used to strip the white space
*		off the beginning and end of a string.
*
*       INPUT: string - the string to be cleaned up
*
*	       output_string - if the output string is not NULL then
*			     this will be where we place the cleaned
*			     string.  Otherwise we malloc enough space
*			     to place the cleaned up string.
*
*
*      OUTPUT:  returns the cleaned up string
*
* CALLED FROM:
*
*  WRITTEN BY:  Mark Young
*
**************************************************************/


char *vcleanup_string(input_string, output_string)

char	*input_string, *output_string;
{
	char *string, *temp, buffer[LENGTH];


	if (input_string == NULL)
	{
	   (void) fprintf(stderr,"\nvcleanup_string:\n");
	   (void) fprintf(stderr,"   Error!  NULL input string encountered.\n");
	   return(NULL);
	}

	if (VStrlen(input_string) < LENGTH)
	{
	   string = buffer;
	   strcpy(string, input_string);
	}
	else
	{
	   if ((string = VStrcpy(input_string)) == NULL)
	      return(NULL);
	}

	if ((temp = _cleanup_string(string)) == NULL)
	{
	   if (output_string != NULL) output_string[0] = '\0';
	   output_string = NULL;
	}
	else if (output_string == NULL)
	   output_string = VStrcpy(temp);
	else
	   strcpy(output_string, temp);

	if (string != buffer) free(string);
	return(output_string);
}


/**************************************************************
*
* MODULE NAME: vstrstr
*
*     PURPOSE: 	This function performs the strstr().
*
*       INPUT: input_string - the string to search
*
*	       sub_string -  the sub_string to search the input_string for.
*
*      OUTPUT:  returns a pointer to the first occurence of the sub-string in
*	        the input_string, otherwise it returns NULL.
*
* CALLED FROM:
*
*  WRITTEN BY:  Hideo Noro (noro@ohara.info.waseda.ac.jp), Mark Young,
*		Tom Sauer
*
**************************************************************/


char *vstrstr(input_string, sub_string)

char	*input_string, *sub_string;
{
	char *string;
	int  len;

	if (input_string == NULL)
	{
	   (void) fprintf(stderr,"\nvstrstr:\n");
	   (void) fprintf(stderr,"   Error!  NULL input string encountered.\n");
	   return(NULL);
	}
	if (sub_string == NULL)
	{
	   (void) fprintf(stderr,"\nvstrstr:\n");
	   (void) fprintf(stderr,"   Error!  NULL sub-string encountered.\n");
	   return(NULL);
	}

	len = VStrlen(sub_string);
	string = input_string;
	while (*string != '\0')
	{
	   if (VStrncmp(string, sub_string, len) == 0)
	     return(string);
	   else
	     string++;
	}
        return(NULL);

}



/**************************************************************
*
* MODULE NAME: vstrpbrk
*
*     PURPOSE: 	This function performs the strpbrk().
*
*       INPUT: input_string - the string to search
*
*	       sub_string -  the sub_string to search the input_string for.
*
*      OUTPUT:  returns a pointer to the first occurence of the sub-string in
*	        the input_string, otherwise it returns NULL.
*
* CALLED FROM:
*
*  WRITTEN BY:  Mark Young & Steve Jorgensen
*
**************************************************************/


char *vstrpbrk(input_string, sub_string)

char	*input_string, *sub_string;
{
	char    *string;


	if (input_string == NULL)
	{
	   (void) fprintf(stderr,"\nvstrpbrk:\n");
	   (void) fprintf(stderr,"   Error!  NULL input string encountered.\n");
	   return(NULL);
	}

	if (sub_string == NULL)
	{
	   (void) fprintf(stderr,"\nvstrpbrk:\n");
	   (void) fprintf(stderr,"   Error!  NULL token string encountered.\n");
	   return(NULL);
	}

	string = input_string;
	while (*string != '\0')
	{
	   if (strchr(sub_string, *string))
	      return(string);

	   string++;
	}
	return(NULL);
}



/**************************************************************
*
* MODULE NAME: vstrtok
*
*     PURPOSE: 	This function performs the strtok().
*
*       INPUT: input_string - the string to search
*
*	       sub_string -  the sub_string to search the input_string for.
*
*      OUTPUT:  returns a pointer to the first occurence of the sub-string in
*	        the input_string, otherwise it returns NULL.
*
* CALLED FROM:
*
*  WRITTEN BY:  Mark Young & Steve Jorgensen
*
**************************************************************/


char *vstrtok(input_string, sub_string)

char	*input_string, *sub_string;
{
	static char *string = NULL;


	if (input_string == NULL && string == NULL)
	   return(NULL);
	else if (input_string == NULL)
	   input_string = string;
	else
	   string = input_string;

	if ((string = vstrpbrk(string, sub_string)) != NULL)
	{
	   *string++ = '\0';
	   while (*string != '\0' && strchr(sub_string, *string))
	       string++;
	}
	return(input_string);
}
