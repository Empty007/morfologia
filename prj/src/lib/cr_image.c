 /*
  * Khoros: $Id: cr_image.c,v 1.2 1991/12/18 09:56:22 dkhoros Exp $
  */

#if !defined(lint) && !defined(SABER)
static char rcsid[] = "Khoros: $Id: cr_image.c,v 1.2 1991/12/18 09:56:22 dkhoros Exp $";
#endif

 /*
  * $Log: cr_image.c,v $
 * Revision 1.2  1991/12/18  09:56:22  dkhoros
 * HellPatch3
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
#
# FILE NAME:	cr_image.c
# 
# AUTHORS:	Ron E. Neher
# 
# DATE:		3/23/89
# 
# DESCRIPTION:	Create a generic image 
# 
# MODIFICATIONS:  Added "map_scheme" to the parameter list.  The image's
#		  map_scheme was hardwired to VFF_MS_NONE which meant that
#		  no map space was being allocated.
#			(Mark Young & John Rasure 7/23/89)
# 
*/

/*INCLUDE*/
#include "unmcopyright.h"	 /* Copyright 1990 by UNM */
#include "vinclude.h"	 	

#define LENGTH 512

/**************************************************************
*
* MODULE NAME: createimage
*
*     PURPOSE: Create a generic image
*
*       INPUT: 	col_size -- the size of a column
*		row_size -- the size of a row
*		data_storage_type -- the VFF_TYP_* define of image
*		num_of_images -- # of images pointed to by imagedata
*		num_data_bands -- # of bands/pixel, /image or dim vec data
*		comment -- description of image
*		map_row_size -- # of columns in map array
*		map_col_size -- # of rows in map array
*		map_storage_type -- Storage type of cells in the maps
*		location_type -- implied or explicit location data
*		location_dim -- explicit locations can be of any dimension
*
*      OUTPUT: 	1.  returns a pointer to an xvimage with image defined
*
* CALLED FROM: 
*
* ROUTINES CALLED: 
*
**************************************************************/
struct xvimage *
createimage(col_size, row_size, data_storage_type, num_of_images,
            num_data_bands, comment, map_row_size, map_col_size,
	    map_scheme, map_storage_type, location_type, location_dim)
unsigned
long	col_size,
	row_size,
	data_storage_type,
	num_of_images,
	num_data_bands,
	map_row_size,
	map_col_size,
	map_scheme,
	map_storage_type,
	location_type,
	location_dim;
char	*comment;
{
struct
xvimage *image;
char	*maps, 
	*imagedata, 
	tmp_comment[LENGTH];
long 	machtype();
float	*location;
int	cstrlen,
	image_data_size_bytes,		/* # data bytes */
	image_data_count_pixels,	/* # data pixels */
	map_size_bytes,			/* # map bytes */
	map_count_cells,		/* # map cells */
	location_size_bytes,		/* # location bytes */
	location_count_objects;		/* # location objs */

/* malloc room for the xvimage structure */

    if ((image=(struct xvimage *)kcalloc(1,sizeof(struct xvimage)))==NULL)
    {
        (void) fprintf(stderr,"createimage: No space for image \
- malloc failed!\n");
        return(0);
    }

/* setup the comment (can only be 511 chars) */

    cstrlen = VStrlen(comment);
    if (cstrlen > 0)
    {
       if (cstrlen < 512)
          (void) strcpy(tmp_comment, comment);
       else
       {
          (void) strncpy(tmp_comment, comment, LENGTH - 1);
          (void) strcat(tmp_comment, "");
       }
    }
    else
       (void) strcpy(tmp_comment, "");

/* Load the image header with the values. These can be over-ridden by
   giving them a different value after returning to the calling routine.
 */
    image->identifier = XV_FILE_MAGIC_NUM;
    image->file_type = XV_FILE_TYPE_XVIFF;
    image->release = XV_IMAGE_REL_NUM;
    image->version = XV_IMAGE_VER_NUM;
    image->machine_dep = machtype(NULL);
    (void) strcpy(image->comment, tmp_comment);
    image->row_size = row_size;
    image->col_size = col_size;
    image->startx = VFF_NOTSUB;
    image->starty = VFF_NOTSUB;
    image->pixsizx = 1.0;
    image->pixsizy = 1.0;
    image->location_type = location_type;
    image->location_dim = location_dim;
    image->num_of_images = num_of_images;
    image->num_data_bands = num_data_bands;
    image->data_storage_type = data_storage_type;
    image->data_encode_scheme = VFF_DES_RAW;
    image->map_scheme = map_scheme;
    image->map_storage_type = map_storage_type;
    image->map_row_size = map_row_size;
    image->map_col_size = map_col_size;
    image->map_subrow_size = 0;
    image->map_enable = VFF_MAP_OPTIONAL;
    image->maps_per_cycle = 0;      /* Don't care */
    image->color_space_model = VFF_CM_NONE;
    image->ispare1 = 0;
    image->ispare2 = 0;
    image->fspare1 = 0;
    image->fspare2 = 0;

/* get the sizes for the image data, map data, and location data */

    if (! imagesize(image, 			/* xvimage */
		    &image_data_size_bytes,	/* # data bytes */
		    &image_data_count_pixels,	/* # data pixels */
		    &map_size_bytes,		/* # map bytes */
		    &map_count_cells,		/* # map cells */
		    &location_size_bytes,	/* # location bytes */
		    &location_count_objects	/* # location objs */
		   ))
    {
	fprintf(stderr, "createimage: Uninterpretable image \
specificationa\n");
	return(0);
    }

/* malloc room for the image data */

    if (image_data_size_bytes > 0)
    {
       if ((imagedata = (char *)
	  malloc((unsigned int)image_data_size_bytes)) == NULL)
       {
	  (void) fprintf(stderr,"createimage: Not enough memory for image\
 data!\n");
	  return(0);
       }
    }
    else
    {
       imagedata = NULL;
    }

/* malloc room for the color map data */

    if (map_size_bytes > 0)
    {
       if ((maps = (char *)malloc((unsigned int)map_size_bytes)) == NULL)
       {
	   (void) fprintf(stderr,"createimage: Not enough memory for maps\
 data!\n");
	   return(0);
       }
    }
    else
    {
       maps = NULL;
    }


/* malloc room for the location */

    if (location_size_bytes)
    {
       if ((location = (float *)
	   malloc((unsigned int)location_size_bytes))==NULL)
       {
           (void) fprintf(stderr,"createimage: Not enough memory \
 for location data!\n");
           return(0);
       }
    }
    else
    {
       location = NULL;
    }


/* Load the image data, color map data, and location data */

    image->maps = maps;
    image->location = location;
    image->imagedata = imagedata;

    return(image);
}
