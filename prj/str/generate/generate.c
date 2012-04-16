/* write data to multiple files whose names are built at runtime */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define NUM_FILES 200
//#define DATA_ELEMENTS 500
#define FILE_IDX_LEN(F) (#F)

int main(void)
{
//	const char *f_prefix = "";
	const char *f_suffix = ".str";
	char *fname;
	FILE *fp;
	unsigned ctr;
	
	/* allocate maximum necessary space for the filenames */
	fname = malloc(13);
	if (fname == NULL) {
//		free(rndnums);
		return EXIT_FAILURE;
	}
	char car[11] = "000000000\0";
	int j;
	for (ctr = 0; ctr < (1 << 9); ctr++) {
		
		char escrive[100] = "MM_STRUCT\n1 3 3 1\n";
		char numb[26] = "0 0 0 0 0 0 0 0 0\0";
		
		for (j = 0; j < 9; j++) {
			if ((ctr & (1<<j)) != 0) {
				car[j] = '1';
				numb[2 * j] = '1';
			}
			else {
				car[j] = '0';
				numb[2 * j] = '0';
			}
		}
		
		strcat (escrive, numb);

		/* build the current file name */
		sprintf(fname, "%s%s\0", car, f_suffix);
		
		/* attempt to open the file */
		if ((fp = fopen(fname, "w")) == NULL) {
			fprintf(stderr, "failed to open %s\n", fname);
			free(fname);
//			free(rndnums);
			return EXIT_FAILURE;
		}
		else {
			/* write data to it */
			fwrite(escrive, sizeof *escrive, sizeof(escrive), fp);
			fclose(fp); /* close file */
		}
	}
	
	free(fname);
//	free(rndnums);
	return EXIT_SUCCESS;
}