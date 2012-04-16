//start modified by Edu 19/06/2011
# include <stdio.h>
# include <malloc/malloc.h>
# include <stdlib.h>
# include "pac_win.h"
# include "pac_bas.h"
# include "pac_def.h"
# include "mat_str.h"

#define oo (1<<30)

extern int mask[NB];
int imprime, geral, detalhe, beatriz;
int menor_f = oo;
int last_f = 0;
int upper_bound_program = oo;
int num_sol = 0;

typedef struct {
	int total;
	int *A;
	int *B;
	int *ind;
} int_struc;

typedef struct parte {
	int_struc parte;
	struct parte * prox;
} int_list_struc;

typedef struct {
	int total;
	int nv;
	int nb;
	win_struc win;
	int_struc * intervs;
} aha_struc;

typedef struct {
	win_struc S_ero;
	win_struc S_dil;
	int upper_bound_ero;
	int f_ero;
	int g_ero;
	int h_ero;
	int upper_bound_dil;
	int f_dil;
	int g_dil;
	int h_dil;
} str_A_star;

typedef struct A_star_list{
	str_A_star A_star;
	struct A_star_list * prox;
} str_A_star_list;

typedef struct base_list {
	trv_struc base;
	struct base_list * prox;
} trv_struc_list;

typedef struct win_list {
	win_struc win;
	struct win_list * prox;
} win_struc_list;

typedef struct saida_lista_search {
	trv_struc_list * list_base;
	str_A_star_list * list_A_star;
	win_struc C;
	struct saida_lista_search * prox;
} output_search;

void * xmalloc (size)
int size;
{
	void * aux = malloc (size);
	if (aux == NULL) {
		fprintf (stderr, "My problem is alocation memory\n");
		exit (1);
	}
	return aux;
}

/* esta funcao junta bases em um mesmo arquivo */
/*=*/ int  append_basis_by_ron (char *fname, trv_struc * itrv) {
	FILE  *fd;
	int   n4bytes, section, i, j;
	
	fd = fopen (fname, "a");
	if (fd == NULL) {
		fprintf(stderr, "write_base : fopen failed\n");
		return 0;
	}
	
	fprintf (fd, "BASE %d %d\n", itrv->nv, itrv->total);
	
	write_win2 (fd, &itrv->win);
	
	n4bytes = (itrv->nv+31)/32;
	
	for (i=0; i<itrv->total; i++) {
		for (j=0; j<n4bytes; j++) {
			fprintf (fd, "%x ", itrv->A[itrv->ind[i]+j]); 
		}
		fprintf (fd, "  ");
		for (j=0; j<n4bytes; j++) {
			fprintf (fd, "%x ", itrv->B[itrv->ind[i]+j]); 
		}
		fprintf (fd, "\n");
	}
	
	fclose (fd);
	
	return(1) ;
}

/*=*/ int zera_win_by_ron (win_struc * win) {
	if (win == NULL) {
		fprintf (stderr, "init_win_by_ron () pointer to NULL\n");
		return 0;
	}
	win->wsize = win->hsize = win->size = 0;
	win->data = NULL;  
	return 1;
}

/*=*/ int free_win_by_ron (win_struc * win) {
	if (win == NULL) {
		fprintf (stderr, "free_win_by_ron () pointer to NULL\n");
		return 0;
	}
	win->wsize = win->hsize = win->size = 0;
	if (win->data) free (win->data);
	win->data = NULL;  
	return 1;
}

/*=*/ int zera_basis_by_ron (trv_struc * base) {
	if (base == NULL) {
		fprintf (stderr, "init_basis_by_ron () pointer to NULL\n");
		return 0;
	}
	base->total = base->size = 0;
	base->nv = 0;
	base->nb = 0;
	base->A = base->B = base->ind = NULL;
	zera_win_by_ron (&base->win);
	return 1;
}

/*=*/ int free_basis_by_ron (trv_struc * base) {
	if (base == NULL) {
		fprintf (stderr, "free_basis_by_ron () pointer to NULL\n");
		return 0;
	}
	base->total = base->size = 0;
	base->nv = 0;
	base->nb = 0;
	if (base->A) free (base->A);
	if (base->B) free (base->B);
	if (base->ind) free (base->ind);
	base->A = base->B = base->ind = NULL;
	free_win_by_ron (&base->win);
	return 1;
}

/*=*/ int disp_win_by_ron ( win_struc * win) {
	int  nshifts;
	int  mask;
	int  i, j, section;
	int  data, i_ori, j_ori, imprimiu=0;
	
	if (win == NULL) {
		fprintf (stderr, "disp_win_by_ron () pointer to NULL\n");
		return 0;
	}
	
	i_ori = (win->hsize-1)/2;
	j_ori = (win->wsize-1)/2;
	
	for(i=0; i<win->hsize; i++) {
		for(j=0; j<win->wsize; j++) {
			nshifts = ((i*win->wsize)+j)%NB ;
			mask = 1<<nshifts ;
			section = ((i*win->wsize)+j)/NB ;
			data = win->data[section] ;
			if (i==i_ori && j==j_ori) {
				printf ("[");
				imprimiu = 1;
			}
			else {
				if (imprimiu) {
					imprimiu=0;
				}
				else {
					printf (" ");
				}
			}
			if((data & mask) == mask) printf ("#");
			else printf ("-");
			if (imprimiu)  printf ("]");
		}
		printf ("\n");
	}
	return 1;
}

/*=*/ int function_size_win (win_struc * win) {
	int  nshifts;
	int  mask;
	int  i, j, section;
	int  data, cont=0;
	
	if (win == NULL) {
		fprintf (stderr, "fuction_size_win () pointers to NULL\n");
		return 0;
	}
	
	for(i=0; i<win->hsize; i++) {
		for(j=0; j<win->wsize; j++) {
			nshifts = ((i*win->wsize)+j)%NB ;
			mask = 1<<nshifts ;
			section = ((i*win->wsize)+j)/NB ;
			data = win->data[section] ;
			if((data & mask) == mask) cont++;
		}
	}
	return cont;
}

/*=*/ int quebra_base_intervalos (trv_struc * sai, trv_struc * base) {
	int i, ind, j;
	
	if (base == NULL || sai == NULL) {
		fprintf (stderr, "quebra_base_intervalso () pointers to NULL\n");
		return 0;
	}
	
	for (i=0; i<base->total; i++) {
		sai[i].A = (int *) xmalloc (sizeof (int) * base->nb);
		sai[i].B = (int *) xmalloc (sizeof (int) * base->nb);
		sai[i].ind = (int *) xmalloc (sizeof (int));
		sai[i].ind[0] = 0;
		sai[i].size = sai[i].total = 1; /* 1 intervalo */
		sai[i].nv = base->nv;
		sai[i].nb = base->nb;
		copy_win (&(base->win), &(sai[i].win));
	}
	for (i=0; i<base->total; i++) {
		ind = base->ind[i];
		for (j=0; j<base->nb; j++) {
			sai[i].A[j]=base->A[ind+j];
			sai[i].B[j]=base->B[ind+j];
		}
	}
	return 1;
}

/*=*/ int pontos_elemento_estruturante (int * X_ker, int * Y_ker, 
                                        mat_str * ker, int * num_points) {
	int size, m, n, points, i, j;
	
	if (X_ker == NULL || Y_ker == NULL || 
		ker == NULL || num_points == NULL) {
		fprintf (stderr, "pontos_elemento_estruturante () pointers to NULL\n");
		return 0;
	}
	
	size = ker->m * ker->n;
	m = ker->m; n = ker->n;
	
	// Alocacao em antes da chamada
	// X_ker = (int *) xmalloc (sizeof (int) * size);
	// Y_ker = (int *) xmalloc (sizeof (int) * size);
	
	points = 0;
	switch (ker->tag) {
		case MM_IMG:
			for (i=0; i<size; i++)
				if (ker->dat[i] == 1) {
					X_ker[points] = i%m - (m-1)/2;
					Y_ker[points++] = (n-1)/2 - i/n;
				}
			break;
		case MM_MAT:
			for (i=0; i<3; i++)
				for (j=0; j<3; j++)
					if (ker->x[i][j] == 1) {
						X_ker[points] = i-1;
						Y_ker[points++] = 1-j;
					}
			break;
	}
	
	// if (X_ker) free (X_ker);
	// if (Y_ker) free (Y_ker);
	
	*num_points = points;
	
	return 1;
}

/*=*/ int translada_basis (trv_struc *basis_out, trv_struc *basis_in, 
                           int x, int y) {
	mat_str str;
	int n_lin, n_col, l_ori, c_ori, lin, col, i, j;
	int maior, k_1, k_2;
	
	if (basis_out == NULL || basis_in == NULL)  {
		fprintf (stderr, "translada_basis () pointers to NULL\n");
		return 0;
	}
	
	n_lin = 2*(y>0?y:-y)+1;
	n_col = 2*(x>0?x:-x)+1;
	
	lin = (y>=0?0:(n_lin-1));
	col = (x<=0?0:(n_col-1));
	
	maior = n_lin;
	if (maior<n_col) maior = n_col;
	if (maior%2==0) maior++;
	k_1 = (maior-1)/2 - (n_lin-1)/2;
	k_2 = (maior-1)/2 - (n_col-1)/2;
	
	lin += k_1;
	col += k_2; 
	
	/* construindo o elemento estruturante */
	str.tag = MM_IMG;
	str.m = maior; str.n = maior; str.p = 1;
	str.dat = (short *) xmalloc (sizeof (short) * maior * maior);
	for (i=0; i<maior*maior; i++) str.dat[i]=0;
	str.dat[lin*maior+col]=1;
	
	free_basis_by_ron (basis_out);
	erosion_basis (basis_in, &str, basis_out);
	
	if (str.dat != NULL) free (str.dat);
	return 1;
}

/* Erosao de uma janela: baseado na funcao dilate_win
 * By Ronaldo Fumio Hashimoto
 * 21.01.2000
 */
/*=*/ int erode_win (win_struc *win, win_struc *win_ero, mat_str *str) {
	int size, m, n, points, i, j;
	int *X_ker, *Y_ker;
	int *X_win, *Y_win, w, h, winsize, k, k_1, k_2, tam_X_win;
	int  m_dil, n_dil, size_dil, x_dil, y_dil;
	mat_str str_rot, * ker;
	win_struc * win_vector, win_aux;
	int maior, X_col, Y_lin, primeira_vez;
	
	if (str == NULL) {
		fprintf (stderr, "There is no Structuring Element\n");
		return 0;
	}
	
	if (win == NULL || win_ero == NULL) {
		fprintf (stderr, "erode_win () pointers to NULL\n");
		return 0;
	}
	
	if (str->tag == MM_GRAPH) {
		fprintf (stderr, "Structuring Element Type MM_GRAPH is not supported\n");
		return 0;
	}
	
	/* transpondo o elemento struturante */
	lvrotstr (str, 4, 0, &str_rot);
	ker = &str_rot;
	// ker = str;
	
	size = ker->m * ker->n;
	m = ker->m; n = ker->n;
	
	X_ker = (int *) xmalloc (sizeof (int) * size);
	Y_ker = (int *) xmalloc (sizeof (int) * size);
	// printf ("ker->m = %d ker->n = %d\n", ker->m, ker->n);
	// printf ("Elemento Estruturante:\n");
	points = 0;
	switch (ker->tag) {
		case MM_IMG:
			for (i=0; i<size; i++)
				if (ker->dat[i] == 1) {
					X_ker[points] = i%n - (n-1)/2;
					Y_ker[points++] = (m-1)/2 - i/n;
					// printf (" %d (%d, %d)\n", i, Y_ker[points-1], X_ker[points-1]);
				}
			break;
		case MM_MAT:
			for (i=0; i<3; i++)
				for (j=0; j<3; j++)
					if (ker->x[i][j] == 1) {
						X_ker[points] = i-1;
						Y_ker[points++] = 1-j;
					}
			break;
	}
	
	winsize = win->wsize * win->hsize;
	w = win->wsize; h = win->hsize;
	X_win = (int *) xmalloc (sizeof (int) * winsize);
	Y_win = (int *) xmalloc (sizeof (int) * winsize);
	
	// printf ("------------------------------------------------\n");
	// printf ("Janela a ser erodida:\n");
	for (i=0, k=0; i<winsize; i++) {
		k_1 = i/32;
		k_2 = i%32; 
		
		if (((win->data[k_1]) & (mask[k_2]))!= 0) {
			X_win[k] = i % w - (w-1)/2; 
			Y_win[k] = (h-1)/2 - i / h;
			// printf (" %d (%d, %d)\n", i, Y_win[k], X_win[k]);
			k++;
		} 
	}
	
	tam_X_win = k;
	// win_ero->wsize = win->wsize - 2 * (ker->m/2);
	// win_ero->hsize = win->hsize - 2 * (ker->n/2);
	
	maior = 0;
	for (i=0; i<points; i++) {
		for (j=0; j<win->size; j++) {
			X_col = 2*(X_win[j]+X_ker[i]);
			Y_lin = 2*(Y_win[j]-Y_ker[i]);
			if (X_col<0) X_col = - X_col;
			if (Y_lin<0) Y_lin = - Y_lin;
			if (maior < X_col) maior = X_col;
			if (maior < Y_lin) maior = Y_lin;
		}
	}
	if (maior%2==0) maior += 1; else maior +=3;
	copy_win (win, &win_aux);
	
	if (maior > win->hsize) {
		transform_to_big_window (&win_aux, maior);
	}
	else maior = win->hsize;
	
	// printf ("Tamanho da janela atual: %d\n", maior);
	
	win_ero->wsize = maior;
	win_ero->hsize = maior;
	
	m_dil = win_ero->wsize;
	n_dil = win_ero->hsize;
	size_dil = m_dil * n_dil;
	
	// printf ("maior = %d m_dil = %d n_dil = %d\n", maior, m_dil, n_dil);
	
	k_1 = (size_dil+31)/32;
	win_ero->data = (int *) xmalloc (sizeof(int)*k_1);
	for (j=0; j<k_1; j++) win_ero->data[j] = 0;
	
	// printf ("Window a ser Erodido\n");
	// disp_win_by_ron (&win_aux);
	// printf ("\n");
	
	win_vector = (win_struc *) xmalloc (sizeof(win_struc)*points);
	
	for (i=0; i<points; i++) {
		win_vector[i].data = (int *) xmalloc (sizeof(int)*k_1);
		for (j=0; j<k_1; j++) win_vector[i].data[j] = 0;
		win_vector[i].wsize = m_dil;
		win_vector[i].hsize = n_dil;
		win_vector[i].size  = win->size;
	}
	
	x_dil = (m_dil-1)/2;
	y_dil = (n_dil-1)/2;
	
	for (i=0; i<points; i++) {
		for (j=0; j<tam_X_win; j++) {
			k = (x_dil+X_win[j]+X_ker[i])+(y_dil-Y_win[j]-Y_ker[i])*m_dil;
			k_1 = k/32;
			k_2 = k%32;
			win_vector[i].data[k_1] |= mask[k_2];
		}
		// printf ("Window Vector: %d\n", i+1);
		// disp_win_by_ron (&win_vector[i]);
		// printf ("\n");
	}
	
	// printf ("comecando a fazer a erosao\n");
	k_1 = (size_dil+31)/32;
	primeira_vez = 1;
	for (i=0; i<points; i++) {
		if (primeira_vez) {
			for (j=0; j<k_1; j++) 
				win_ero->data[j] = win_vector[i].data[j];
			primeira_vez = 0;
		}
		else {
			for (j=0; j<k_1; j++) {
				win_ero->data[j] &= win_vector[i].data[j];
			}
		}
		// printf ("Erosion %d/%d\n", i+1, points);
		// disp_win_by_ron (win_ero);
		// printf ("\n");
	}
	
	win_ero->size  = function_size_win (win_ero);
	
	// printf ("Erosao Final:\n");
	// disp_win_by_ron (win_ero);
	// printf ("\n");
	
	for (i=0; i<points; i++) {
		if (win_vector[i].data != NULL) free (win_vector[i].data);
	}
	if (win_vector != NULL) free (win_vector);
	if (X_win) free (X_win); if (Y_win) free (Y_win);
	if (X_ker) free (X_ker); if (Y_ker) free (Y_ker);
	if (str_rot.tag == MM_IMG && str_rot.dat) free (str_rot.dat);
	free_win_by_ron (&win_aux);
	return 1;
}

/*=*/ int transform_to_big_window (win_struc * win, int lado) {
	int i, j, nshifts, section, cont=0;
	int l_ori, c_ori, l_new_ori, c_new_ori;
	int * new_data, new_tam, mask_antes;
	int new_i, new_j, new_nshifts, new_section;
	
	if (win == NULL) {
		fprintf (stderr, "transform_to_big_window () pointer to NULL\n");
		return 0;
	}
	
	if (win->hsize != win->wsize) {
		fprintf (stderr, "hsize and wsize of the window must be equal\n");
		return 0;
	}
	
	if (win->hsize > lado) {
		fprintf (stderr, "new dimension must be greater than hsize of the window\n");
		return 0;
	}
	
	if (win->hsize == lado) {
		/* nada a fazer */
		return 1;
	}
	
	/* In this case, win->hsize < lado */
	new_tam = (lado * lado + NB1) / NB;
	new_data = (int *) xmalloc (sizeof(int)*new_tam);
	for (i=0; i<new_tam; i++) new_data[i]=0;
	l_new_ori = c_new_ori = (lado-1)/2;
	l_ori = c_ori = (win->hsize-1)/2;
	for (i=0; i<win->hsize; i++) {
		for (j=0; j<win->wsize; j++) {
			nshifts = (i*win->wsize+j)%NB;
			section = (i*win->wsize+j)/NB;
			if ((win->data[section] & mask[nshifts]) != 0) {
				new_i = i + (l_new_ori - l_ori);
				new_j = j + (c_new_ori - c_ori);
				new_nshifts = (new_i*lado+new_j)%NB;
				new_section = (new_i*lado+new_j)/NB;
				new_data[new_section] |= mask[new_nshifts];
				cont++;
			}
		}
	}
	
	win->hsize = win->wsize = lado;
	if (win->data) free (win->data);
	win->data = new_data;
	return 1;
}

/*=*/ int A_contido_em_Bc (int * A, int * B, int tam_nv) {
	int contido, k;
	
	if (A == NULL || B == NULL) {
		fprintf (stderr, "A_contido_em_Bc () pointers to NULL\n");
		return 0;
	}
	
	for (k=0, contido=1; k<tam_nv && contido; k++) {
		if ( ((A[k/NB] & mask[k%NB]) & 
			  ((~B[k/NB]) & mask[k%NB])) != (A[k/NB] & mask[k%NB]) ) {
			contido=0;
		}
	}
	
	// printf ("contido = %d tam_nv = %d\n", contido, tam_nv);
	// contido=1;
	// for (i=0; i<tam_nb; i++) {
	//   if ((A[i] & (~B[i])) != A[i]) {
	//     contido=0;
	//     break;
	//   }
	// }
	
	return contido;
}

/*=*/ int intersection_B_win (trv_struc * base, win_struc * win) {
	/* colocar win no formato de base->B se win esta contido */
	/* em base->win */
	
	int * A, * B, tam_B, i, cont=0, contido;
	int * winidx, * base_A, * base_B;
	int nshifts, block, win_nshifts, section, ind;
	int j, tem, * winidx_ron;
	
	if (base == NULL || win == NULL) {
		fprintf (stderr, "intersection_B_win () pointers to NULL\n");
		return 0;
	}
	
	
	if (base->size != 1) {
		fprintf (stderr, "The basis must represent just one interval\n");
		return 0;
	}
	
	/* colocar win e base->win em um mesmo tamanho de retangulo */
	if (base->win.hsize < win->hsize) 
		transform_to_big_window (&base->win, win->hsize);
	else
		transform_to_big_window (win, base->win.hsize);
	
	/* monta o lado direito  do intervalo no tamanho do retangulo da janela */
	/* monta o lado esquerdo do intervalo no tamanho do retangulo da janela */
	tam_B = (base->win.hsize*base->win.wsize+NB1)/NB;
	B = (int *) xmalloc (sizeof (int)*tam_B);
	A = (int *) xmalloc (sizeof (int)*tam_B);
	for (i=0; i<tam_B; i++) A[i] = B[i] = 0;
	win_index (&base->win, &winidx);
	ind = base->ind[0]; /* ind must be equal to zero */
	base_A = &base->A[ind];
	base_B = &base->B[ind];
	
	for (i=0; i<base->nv; i++) {
		nshifts = i%NB;
		block   = i/NB;
		if ((base_B[block] & mask[nshifts]) != 0) {
			win_nshifts = winidx[i]%NB;
			section     = winidx[i]/NB;
			B[section] |= mask[win_nshifts];
		}
		if ((base_A[block] & mask[nshifts]) != 0) {
			win_nshifts = winidx[i]%NB;
			section     = winidx[i]/NB;
			A[section] |= mask[win_nshifts];
		}
	}
	
	/* invertendo B */
	for (i=0; i<tam_B; i++) B[i] = ~B[i];
	
	{
		// int * aux;
		// aux = win->data;
		// win->data = B;
		// printf ("B:\n");
		// disp_win_by_ron (win);
		// win->data = aux;
		// printf ("Janela:\n");
		// disp_win_by_ron (win);
	}
	
	if (winidx) free (winidx);
	win_index (win, &winidx);
	
	// for (j=0; j<win->size; j++) printf ("%d ", winidx[j]); printf ("\n");
	contido = 1; /* se terminar com 1, continua, senao cai fora */
	/* Problema de Implementacao */
	/* verificando consistencia  */
	/* se W[i] == 0 entao B[i] == 1 */
	for (i=0; i<win->hsize*win->wsize; i++) {
		for (j=0, tem=0; j<win->size && !tem; j++) {
			if (i==winidx[j]) tem = 1;
		}
		/* neste caso, o valor de W no ponto e' zero */
		if (!tem) {
			win_nshifts = i%NB;
			section     = i/NB;
			if ( (B[section] & mask[win_nshifts]) == 0 ) {
				contido = 0;
				// printf ("Nao bateu %d\n", i);
				break;
			}
		}
	}
	
	if (contido) {
		/* fazendo a interseccao */
		for (i=0; i<tam_B; i++) B[i] = (B[i] & win->data[i]);
	}
	
	/* verifica se o lado esquerdo esta' contido no lado direito */
	// contido=1;
	for (i=0; i<tam_B && contido; i++) {
		if ((A[i]&B[i])!=A[i]) {
			contido=0;
			break;
		}
	}
	
	if (!contido) {
		free_basis_by_ron (base);
	}
	else {
		/* faz a negacao do lado direito */
		for (i=0; i<tam_B; i++) B[i] = ~B[i];
		
		/* liberando a base antiga */
		free_basis_by_ron (base);
		
		/* atualizando os novos parametros da base */
		base->nv = win->size;
		base->nb = (win->size+NB1)/NB;
		base->total = base->size = 1;
		base->A = (int *) xmalloc (sizeof(int)*base->total*base->nb);
		base->B = (int *) xmalloc (sizeof(int)*base->total*base->nb);
		base->ind = (int *) xmalloc (sizeof(int)*base->total);
		base->ind[0] = 0;
		copy_win (win, &base->win);
		base_A = &base->A[0];
		base_B = &base->B[0];
		
		/* novos indices da janela win */
		if (winidx) free (winidx);
		win_index (win, &winidx);
		
		/* montando o lado esquerdo e direito do intervalo da base */
		for (i=0; i<base->nb; i++) base_A[i] = base_B[i] = 0;
		
		for (i=0; i<base->nv; i++) {
			win_nshifts = winidx[i]%NB;
			section     = winidx[i]/NB;
			if ((B[section] & mask[win_nshifts]) != 0) {
				nshifts = i%NB;
				block   = i/NB;
				base_B[block] |= mask[nshifts];
			}
			if ((A[section] & mask[win_nshifts]) != 0) {
				nshifts = i%NB;
				block   = i/NB;
				base_A[block] |= mask[nshifts];
			}
		}
	}
	
	if (A) free (A);
	if (B) free (B);
	if (winidx) free (winidx);
	return 1;
}

/* Nota: intervalo deve conter somente um intervalo */
/* verifica se o unico intervalo que esta' em intervalo ja' aparece em base */
/*=*/ int verifica_se_a_base_ja_tem (trv_struc * base, trv_struc * intervalo) {
	int ind, i, j, tem=0;
	
	if (base == NULL || intervalo == NULL) {
		fprintf (stderr, "verifica_se_a_base_ja_tem () pointers to NULL\n");
		return 0;
	}
	
	for (i=0, tem=0; i<base->total && !tem; i++) {
		ind = base->ind[i];
		for (j=0, tem=1; j<base->nv && tem; j++) {
			if ( ((base->A[ind+j/NB] & mask[j%NB])  != 
				  (intervalo->A[j/NB] & mask[j%NB])) || 
				((base->B[ind+j/NB] & mask[j%NB])  != 
				 (intervalo->B[j/NB] & mask[j%NB]))) {
					tem=0;
				}
		}
	}
	
	return tem;
}

/*=*/ int copy_basis_by_ron (trv_struc * base_in, trv_struc * base_out) {
	int i, j, ind;
	
	if (base_out == NULL || base_in == NULL) {
		fprintf (stderr, "copy_basis_by_ron () pointers to NULL\n");
		return 0;
	}
	
	zera_basis_by_ron (base_out);
	
	base_out->size  = base_in->size;
	base_out->total = base_in->total;
	base_out->nv    = base_in->nv;
	base_out->nb    = base_in->nb;
	
	if (base_in->total) {
		base_out->A   = (int *) xmalloc (sizeof(int)*base_in->total*base_in->nb);
		base_out->B   = (int *) xmalloc (sizeof(int)*base_in->total*base_in->nb);
		base_out->ind = (int *) xmalloc (sizeof(int)*base_in->total);
	}
	
	for (i=0; i<base_in->total; i++) 
		base_out->ind[i] = base_in->ind[i];
	for (i=0; i<base_in->total; i++) {
		ind = base_in->ind[i];
		for (j=0; j<base_in->nb; j++) {
			base_out->A[ind+j] = base_in->A[ind+j];
			base_out->B[ind+j] = base_in->B[ind+j];
		}
	}
	
	if (base_in->total) copy_win (&base_in->win, &base_out->win);
	// copy_win (&base_in->win, &base_out->win);
	
	return 1;
}

/* Nota: intervalo deve conter somente um intervalo */
/* acrescenta o unico intervalo que esta' em intervalo na base */
/*=*/ int coloca_na_base (trv_struc * base, trv_struc * intervalo) {
	int ind, i, j, *A, *B, * IND;
	
	if (base == NULL || intervalo == NULL) {
		fprintf (stderr, "coloca_na_base () pointers to NULL\n");
		return 0;
	}
	
	
	if (base->total == 0) {
		copy_basis_by_ron (intervalo, base);
		return 1;
	}
	
	if (base->win.hsize < intervalo->win.hsize) 
		transform_to_big_window (&base->win, intervalo->win.hsize);
	else
		transform_to_big_window (&intervalo->win, base->win.hsize);
	
	A   = (int *) xmalloc (sizeof (int) * (base->total+1) * base->nb);
	B   = (int *) xmalloc (sizeof (int) * (base->total+1) * base->nb);
	IND = (int *) xmalloc (sizeof (int) * (base->total+1));
	
	for (i=0; i<base->total; i++) IND[i] = base->ind[i];
	/* neste ponto i==base->total */
	IND[i] = IND[i-1] + base->nb;
	
	for (i=0; i<base->total; i++) {
		ind = base->ind[i];
		for (j=0; j<base->nb; j++) {
			A[ind+j] = base->A[ind+j];
			B[ind+j] = base->B[ind+j];
		}
	}
	
	/* neste ponto i==base->total */
	ind = IND[i];
	for (j=0; j<base->nb; j++) {
		A[ind+j] = intervalo->A[j];
		B[ind+j] = intervalo->B[j];
	}
	
	(base->total)++; (base->size)++;
	if (base->A) free (base->A); 
	if (base->B) free (base->B); 
	if (base->ind) free (base->ind);
	base->A = A; base->B = B; base->ind = IND;
	return 1;
}

/*=*/ int base_ele_erre_W_by_edu (trv_struc * sai, trv_struc * base, mat_str * str, trv_struc * upper) {
	/* trv_struc * base represents only one interval */
	win_struc * win = &(base->win);
	mat_str str_rot, * ker;
	win_struc * win_ero, win_in;
	int i, * X_ker, * Y_ker, size, m, n, points;
	trv_struc base_aux;
	int retorno, retorno2;
	
	if (sai == NULL || base == NULL || str == NULL) {
		fprintf (stderr, "base_ele_erre_W () pointers to NULL\n");
		return 0;
	}
	
	if (base->size != 1) {
		fprintf (stderr, "The basis must represent just one interval\n");
		return 0;
	}
	
	
	/* transpondo o elemento struturante */
	str_rot.dat = NULL;
	lvrotstr (str, 4, 0, &str_rot);
	
	copy_win (&(base->win), &win_in);
	
	win_ero = &(sai->win);
	win_ero->size = win_ero->hsize = win_ero->wsize = 0;
	if (sai->win.data) free (sai->win.data);
	sai->win.data = NULL;
	
	/* construindo a janela $W=W'\ominus C^t$ W =W' ⊖ Ct.*/
	erode_win (&win_in, &(sai->win), &str_rot);
	win_ero = &(sai->win);
	
	sai->total = sai->size = 0;
	sai->nv = win_ero->size;
	sai->nb = (sai->nv+NB1)/NB;
	sai->A = sai->B = sai->ind = NULL;
	
	ker = str;
	size = ker->m * ker->n;
	m = ker->m; n = ker->n;
	
	X_ker = (int *) xmalloc (sizeof (int) * size);
	Y_ker = (int *) xmalloc (sizeof (int) * size);
	
	pontos_elemento_estruturante (X_ker, Y_ker, ker, &points);
	
	zera_basis_by_ron (&base_aux);
	
	// printf("Input Window\n");
	// disp_win_by_ron (&win_in);
	// printf("Eroded Window\n");
	// disp_win_by_ron (win_ero);
	// printf("\n");
	
	
	for (i=0; i<points; i++) {
		// printf ("Base a ser transladada\n");
		// disp_basis_by_ron (base);
		translada_basis (&base_aux, base, X_ker[i], Y_ker[i]);
		// printf ("Translacao %d por (%d,%d)\n", i+1, X_ker[i], Y_ker[i]);
		// disp_basis_by_ron (&base_aux);
		intersection_B_win (&base_aux, win_ero);
		if (base_aux.total == 1) {
			// printf ("Deu Interseccao\n");
			retorno = verifica_se_a_base_ja_tem (sai, &base_aux);
//			disp_basis_by_ron (upper);
//			disp_basis_by_ron (&base_aux);
			if (!retorno) {
				retorno2 = verifica_bases_menor (&base_aux, upper);
				if (retorno2) {
//					printf("fuck yeah\n");
					coloca_na_base (sai, &base_aux);
				}
			}
		}
		// else printf ("Nao deu Interseccao\n");
		free_basis_by_ron (&base_aux);
	}
	
	if (sai->total == 0) zera_basis_by_ron (sai);
	if (X_ker) free (X_ker); 
	if (Y_ker) free (Y_ker);
	if (win_in.data) free (win_in.data);
	if (str_rot.tag == MM_IMG && str_rot.dat) free (str_rot.dat);
	return 1;
}


/* Nota: base deve conter somente um unico intervalo */
/* Nota: base esta' definida em W' */
/* Nota: sai deve estar definida em W=\erosion(W')_{str} */
/* gera somente as translacoes do intervalo por pontos de str */
/*=*/ int base_ele_erre_W (trv_struc * sai, trv_struc * base, mat_str * str) {
	/* trv_struc * base represents only one interval */
	win_struc * win = &(base->win);
	mat_str str_rot, * ker;
	win_struc * win_ero, win_in;
	int i, * X_ker, * Y_ker, size, m, n, points;
	trv_struc base_aux;
	int retorno;
	
	if (sai == NULL || base == NULL || str == NULL) {
		fprintf (stderr, "base_ele_erre_W () pointers to NULL\n");
		return 0;
	}
	
	if (base->size != 1) {
		fprintf (stderr, "The basis must represent just one interval\n");
		return 0;
	}
	
	
	/* transpondo o elemento struturante */
	str_rot.dat = NULL;
	lvrotstr (str, 4, 0, &str_rot);
	
	copy_win (&(base->win), &win_in);
	
	win_ero = &(sai->win);
	win_ero->size = win_ero->hsize = win_ero->wsize = 0;
	if (sai->win.data) free (sai->win.data);
	sai->win.data = NULL;
	
	/* construindo a janela $W=W'\ominus C^t$ W =W' ⊖ Ct.*/
	erode_win (&win_in, &(sai->win), &str_rot);
	win_ero = &(sai->win);
	
	sai->total = sai->size = 0;
	sai->nv = win_ero->size;
	sai->nb = (sai->nv+NB1)/NB;
	sai->A = sai->B = sai->ind = NULL;
	
	ker = str;
	size = ker->m * ker->n;
	m = ker->m; n = ker->n;
	
	X_ker = (int *) xmalloc (sizeof (int) * size);
	Y_ker = (int *) xmalloc (sizeof (int) * size);
	
	pontos_elemento_estruturante (X_ker, Y_ker, ker, &points);
	
	zera_basis_by_ron (&base_aux);
	
	// printf("Input Window\n");
	// disp_win_by_ron (&win_in);
	// printf("Eroded Window\n");
	// disp_win_by_ron (win_ero);
	// printf("\n");
	
	for (i=0; i<points; i++) {
		// printf ("Base a ser transladada\n");
		// disp_basis_by_ron (base);
		translada_basis (&base_aux, base, X_ker[i], Y_ker[i]);
		// printf ("Translacao %d por (%d,%d)\n", i+1, X_ker[i], Y_ker[i]);
		// disp_basis_by_ron (&base_aux);
		intersection_B_win (&base_aux, win_ero);
		if (base_aux.total == 1) {
			// printf ("Deu Interseccao\n");
			retorno = verifica_se_a_base_ja_tem (sai, &base_aux);
			if (!retorno) {
				coloca_na_base (sai, &base_aux);
			}
		}
		// else printf ("Nao deu Interseccao\n");
		free_basis_by_ron (&base_aux);
	}
	
	if (sai->total == 0) zera_basis_by_ron (sai);
	if (X_ker) free (X_ker); 
	if (Y_ker) free (Y_ker);
	if (win_in.data) free (win_in.data);
	if (str_rot.tag == MM_IMG && str_rot.dat) free (str_rot.dat);
	return 1;
}

/*=*/ int Bc_menos_A (int * D, int * A, int * Bc, int tam) {
	int i;
	
	if (A == NULL || Bc == NULL || D == NULL) {
		fprintf (stderr, "Bc_menos_A () pointers to NULL\n");
		return 0;
	}
	
	for (i=0; i<tam; i++) D[i] = ((~A[i])&(~Bc[i]));
	
	return 1;
}

/*=*/ int numero_de_combinacoes (int m, int n) { 
	if (n==0) return 1;
	if (m==n) return 1;
	return numero_de_combinacoes (m-1,n-1) + numero_de_combinacoes (m-1,n);
}

/*=*/ int _combinacoes_ (int m, int n, int * * v) {
	static cont = 0;
	int k, i, j;
	
	if (*v==NULL) {
		*v = (int *) xmalloc (sizeof(int)*n);
		for (i=0; i<n; i++) (*v)[i] = i;
		return ++cont;
	}
	
	if ((*v)[n-1] < m-1) {
		(*v)[n-1]++;
		return ++cont;
	}
	
	/* neste caso v[n-1] == m-1 */
	
	/* verifica se acabou */
	if ((*v)[0] == m-n) {
		free (*v);
		*v = NULL;
		return 0;
	}
	
	/* neste caso v[n-1] == m-1 e ainda da para continuar */
	
	for (i=n-2; i>=0; i--) {
		if ((*v)[i]<m-1 && (*v)[i]!=m-n+i) break;
	}
	(*v)[i]++; k=(*v)[i]+1;
	for (i++; i<n; i++) (*v)[i]=k++;
	return ++cont;
}

/*=*/ int free_aha (aha_struc * apt) {
	int i;
	
	if (apt == NULL) {
		fprintf (stderr, "free_aha_W () pointer to NULL\n");
		return 0;
	}
	
	if (apt->win.data) free (apt->win.data);
	apt->win.data = NULL;
	
	for (i=0; i<apt->total; i++) {
		if (apt->intervs[i].A) free (apt->intervs[i].A);
		if (apt->intervs[i].B) free (apt->intervs[i].B);
		if (apt->intervs[i].ind) free (apt->intervs[i].ind);
		apt->intervs[i].A = apt->intervs[i].B = apt->intervs[i].ind = NULL;
	}
	
	if (apt->intervs ) free (apt->intervs); 
	apt->intervs = NULL;
	apt->total = 0;
	return 1;
}

/*=*/ int zera_aha (aha_struc * apt) {
	int i;
	
	if (apt == NULL) {
		fprintf (stderr, "zera_aha_W () pointer to NULL\n");
		return 0;
	}
	
	apt->win.data = NULL;
	apt->total = 0;
	
	// for (i=0; i<apt->total; i++) {
	//  apt->intervs[i].A = apt->intervs[i].B = apt->intervs[i].ind = NULL;
	// }
	
	apt->intervs = NULL;
	return 1;
}

/*=*/ int union_basis_by_ron (trv_struc *basis1, trv_struc *basis2, 
                              trv_struc *basis3) {
	int k,i;
	int n4bytes;
	int total;
	int st;
	int nv;
	int ind;
	trv_struc basis4;
	
	/* supondo que basis3 foi convenientemente zerado */
	free_basis_by_ron (basis3);
	
	/* supondo que basis1->win == basis2->win */
	copy_win (&basis1->win, &basis3->win);
	
	n4bytes = (basis3->win.size + 31)/32;
	total = basis1->total + basis2->total;
	
	basis3->A = (int *) xmalloc (sizeof(int) * total * n4bytes);
	basis3->B = (int *) xmalloc (sizeof(int) * total * n4bytes);
	basis3->ind = (int *) xmalloc (sizeof(int) * total);
	
	k = basis1->total * basis1->nb;
	for (i = 0; i < k; i++) {
		basis3->A[i] = basis1->A[i];
		basis3->B[i] = basis1->B[i];
	}
	ind = k;
	k = basis2->total * basis2->nb;
	for (i = 0; i < k; i++){
		basis3->A[i + ind] = basis2->A[i];
		basis3->B[i + ind] = basis2->B[i];
	}         
	for (i = 0; i < total; i++){
		basis3->ind[i] =  i * n4bytes;
	}
	
	basis3->nb = n4bytes;
	basis3->nv = basis3->win.size;
	basis3->total = total;
	basis3->size = total;
	
	// if (geral) {
	//   printf ("TEstando Union\n");
	//   disp_basis_by_ron (basis3);
	// }
	
	zera_basis_by_ron (&basis4);
	st = copy_win (&(basis3->win), &(basis4.win));
	st = negation_basis (basis3, &basis4);
	// if (geral) {
	//   printf ("TEstando Negation I\n");
	//   disp_basis_by_ron (&basis4);
	// }
	free_basis_by_ron (basis3);
	st = copy_win (&(basis4.win), &(basis3->win));
	st = negation_basis (&basis4, basis3);
	// if (geral) {
	//   printf ("TEstando Negation II\n");
	//   disp_basis_by_ron (basis3);
	// }
	free_basis_by_ron (&basis4);
	return 1;
}   

/*=*/ int igual_basis_by_ron (trv_struc * base1, trv_struc * base2) {
	win_struc * win1, * win2, window1, window2;
	int i, j, k, tem, ind1, ind2, igual, tam;
	
	if (base1 == NULL || base2 == NULL) {
		fprintf (stderr, "igual_basis_by_ron () pointers to NULL\n");
		return 0;
	}
	
	win1 = &base1->win;  win2 = &base2->win;
	
	if (  (base1->size  != base2->size)  ||
        (base1->total != base2->total) ||
        (base1->nv    != base2->nv) ||
        (base1->nb    != base2->nb) ||
        (win1->size != win2->size) ) {
		// printf ("saiu por causa de dados fundamentais\n");
		return 0;
	}
	
	window1.data = window2.data = NULL;
	copy_win (win1, &window1);
	copy_win (win2, &window2);
	
	if (window1.hsize < window2.hsize) 
		transform_to_big_window (&window1, window2.hsize);
	else
		transform_to_big_window (&window2, window1.hsize);
	
	tam = (window1.hsize * window1.wsize+NB1)/NB;
	igual = 1;
	for (i=0; i<tam; i++) {
		if (window1.data[i] != window2.data[i]) {
			igual = 0;
			break;
		}
	}
	
	// if (!igual) printf ("saiu por causa da janela\n");
	
	for (i=0; i<base1->total && igual; i++) {
		ind1 = base1->ind[i];
		for (j=0, tem=0; j<base2->total && !tem; j++) {
			ind2 = base2->ind[j];
			for (k=0, tem=1; k<base1->nv && tem; k++) {
				if ( ((base1->A[ind1+k/NB] & mask[k%NB])  != 
					  (base2->A[ind2+k/NB] & mask[k%NB])) || 
					((base1->B[ind1+k/NB] & mask[k%NB])  != 
					 (base2->B[ind2+k/NB] & mask[k%NB])) ) {
						tem=0;
					}
			}
		}
		igual = tem;
	}
	
	if (window1.data) free (window1.data);
	if (window2.data) free (window2.data);
	
	// if (!igual) printf ("nao sei porque saiu\n");
	
	return igual;
}

/*=*/ int gera_um_Z (trv_struc * Z, int * vet_apt, aha_struc * aha, 
					 int tam_aha, trv_struc * esse) {
	trv_struc base1, base2;
	int j, k, jj, ind;
	
	zera_basis_by_ron (&base1);
	zera_basis_by_ron (&base2);
	
	/* gerando os conjuntos esse[j], isto e', S_j */
	for (j=0; j<tam_aha; j++) { /* para cada conjunto H */
		if (aha[j].total) {
			free_basis_by_ron (&esse[j]);
			copy_win (&aha[j].win, &esse[j].win);
			esse[j].nv = aha[j].nv;
			esse[j].nb = aha[j].nb;
			esse[j].total = esse[j].size = aha[j].intervs[vet_apt[j]].total;
			esse[j].ind = (int *) xmalloc (sizeof(int)*esse[j].total);
			esse[j].A = (int *) xmalloc (sizeof(int)*esse[j].total*esse[j].nb);
			esse[j].B = (int *) xmalloc (sizeof(int)*esse[j].total*esse[j].nb);
			for (k=0; k<aha[j].intervs[vet_apt[j]].total; k++) {
				esse[j].ind[k] = ind = aha[j].intervs[vet_apt[j]].ind[k];
				for (jj=0; jj<aha[j].nb; jj++) {
					esse[j].A[ind+jj] = aha[j].intervs[vet_apt[j]].A[ind+jj];
					esse[j].B[ind+jj] = aha[j].intervs[vet_apt[j]].B[ind+jj];
				}
			}
			// printf ("Esse %d\n", j);
			// disp_basis_by_ron (&esse[j]);
		}
	}
	
	/* Agora faz a uniao e coloca em base1 */
	zera_basis_by_ron (&base1);
	
	for (j=0; j<tam_aha; j++) { /* para cada conjunto H */
		if (aha[j].total) break;  /* primeiro que tem diferente de zero */
	}
	
	if (j<tam_aha) {
		// printf ("j = %d\n", j);
		copy_basis_by_ron (&(esse[j]), &base1);
		// disp_basis_by_ron (&base1);
	}
	
	for (j++; j<tam_aha; j++) { /* para cada conjunto H */
		if (aha[j].total) {
			union_basis_by_ron (&base1, &esse[j], &base2);
			free_basis_by_ron  (&base1);
			copy_basis_by_ron  (&base2, &base1);
			// printf ("j = %d\n", j);
			// disp_basis_by_ron (&base2);
			free_basis_by_ron  (&base2);
		}
	}
	
	/* para a uniao para a base de saida */
	copy_basis_by_ron (&base1, Z);
	free_basis_by_ron (&base1);
	
	/* prepara o proximo vetor para gerar o proximo Z */
	for (j=tam_aha-1; j>=0; j--) { /* para cada conjunto H */
		if (vet_apt[j] < aha[j].total-1) {vet_apt[j]++; break;}
		vet_apt[j] = 0;
	}
	
	return 1;
}

/*=*/ int upper_bound_by_ron (trv_struc * base_out, trv_struc * base_in, 
                              mat_str * str) {
	mat_str str_rot;
	
	if (base_out == NULL || base_in == NULL || str == NULL) {
		fprintf (stderr, "upper_bound_by_ron () pointers to NULL\n");
		return 0;
	}
	
	/* transpondo o elemento struturante */
	str_rot.dat = NULL;
	lvrotstr (str, 4, 0, &str_rot);
	
	free_basis_by_ron (base_out);
	erosion_basis (base_in, str, base_out);
	
	if (str_rot.tag == MM_IMG && str_rot.dat) free (str_rot.dat);
	return 1;
}

/*=*/ int diff_basis_by_ron (trv_struc * base_out, trv_struc * base1, 
                             trv_struc * base2) {
	/* calcula base2 - base1 */
	trv_struc base3;
	
	if (base_out == NULL || base1 == NULL || base2 == NULL) {
		fprintf (stderr, "diff_basis_by_ron () pointers to NULL\n");
		return 0;
	}
	
	zera_basis_by_ron (&base3);
	free_basis_by_ron (base_out);
	
	/* condicao: janelas base1->win e base2->win devem ser iguais */
	copy_win (&(base1->win), &(base3.win));
	negation_basis (base1, &base3);
	
	supremum_win (&(base3.win), &(base2->win), &(base_out->win));
	if (base3.win.size != base_out->win.size) {
		transfer_basis_according_window (&base3, &(base_out->win));
	}
	if (base2->win.size != base_out->win.size) {
		transfer_basis_according_window (base2, &(base_out->win));
	}
	intersection_basis (&base3, base2, base_out);
	
	free_basis_by_ron (&base3);
	return 1;
}

/* Nota: na verdade base nao e' uma base, pois so' contem o lado esquerdo */
/* Logo X e' colocado em base->A */
/*=*/ int coloca_X_na_base (trv_struc * base, int * X) {
	int ind, i, j, *A, *B, * IND;
	
	if (base == NULL || X == NULL) {
		fprintf (stderr, "coloca_X_na_base () pointers to NULL\n");
		return 0;
	}
	
	if (base->total == 0) {
		/* base->win ja' deve estar montado */
		base->total = base->size = 1;
		base->nv = base->win.size;
		base->nb = (base->nv + NB1) / NB;
		base->A  = (int *) xmalloc (sizeof (int) * base->total * base->nb);
		base->B  = (int *) xmalloc (sizeof (int) * base->total * base->nb);
		base->ind = (int *) xmalloc (sizeof (int) * base->total);
		base->ind[0] = 0;
		for (i=0; i<base->nb; i++) {
			base->A[i] = X[i];
			base->B[i] = ~(X[i]);
		}
		return 1;
	}
	
	A   = (int *) xmalloc (sizeof (int) * (base->total+1) * base->nb);
	B   = (int *) xmalloc (sizeof (int) * (base->total+1) * base->nb);
	IND = (int *) xmalloc (sizeof (int) * (base->total+1));
	
	for (i=0; i<base->total; i++) IND[i] = base->ind[i];
	/* neste ponto i==base->total */
	IND[i] = IND[i-1] + base->nb;
	
	for (i=0; i<base->total; i++) {
		ind = base->ind[i];
		for (j=0; j<base->nb; j++) {
			A[ind+j] = base->A[ind+j];
			B[ind+j] = base->B[ind+j];
		}
	}
	
	/* neste ponto i==base->total */
	ind = IND[i];
	for (j=0; j<base->nb; j++) {
		A[ind+j] = X[j];
		B[ind+j] = ~(X[j]);
	}
	
	(base->total)++; (base->size)++;
	if (base->A) free (base->A); 
	if (base->B) free (base->B); 
	if (base->ind) free (base->ind);
	base->A = A; base->B = B; base->ind = IND;
	return 1;
}

/* verifica se X ja' aparece em base->A. Nota: base->B == NULL */
/*=*/ int verifica_base_X (trv_struc * base, int * X) {
	int ind, i, j, tem=0;
	
	if (base == NULL || X == NULL) {
		fprintf (stderr, "verifica_base_X () pointers to NULL\n");
		return 0;
	}
	
	for (i=0, tem=0; i<base->total && !tem; i++) {
		ind = base->ind[i];
		for (j=0, tem=1; j<base->nv && tem; j++) {
			if ( ((base->A[ind+j/NB] & mask[j%NB])  != 
				  (X[j/NB] & mask[j%NB])) ) {
				tem=0;
			}
		}
	}
	
	return tem;
}

/*=*/ int intervalo_contido (int * A1, int * B1, int * A2, int * B2, 
                             int tam_nv) {
	int contido, tam_nb, i;
	
	if (A1 == NULL || B1 == NULL || A2 == NULL || B2 == NULL) {
		fprintf (stderr, "intervalo_contido () pointers to NULL\n");
		return 0;
	}
	
	tam_nb = (tam_nv + NB1) / NB;
	for (i=0; i<tam_nb; i++) {
		A1[i] = ~A1[i];
		B1[i] = ~B1[i];
	}
	
	contido = A_contido_em_Bc (A2, A1, tam_nv) &&
	A_contido_em_Bc (B1, B2, tam_nv);
	
	// printf ("contido 2 = %d\n", contido);
	for (i=0; i<tam_nb; i++) {
		A1[i] = ~A1[i];
		B1[i] = ~B1[i];
	}
	
	return contido;
}

/*=*/ int verifica_bases_menor (trv_struc *base1, trv_struc *base2) {
	/* verifica se base1 <= base2 */
	int i, ind1, ind2, tem, j;
	
	if (base1 == NULL || base2 == NULL) {
		fprintf (stderr, "verifica_bases_menor () pointers to NULL\n");
		return 0;
	}
	
	for (i=0, tem=1; i<base1->total && tem; i++) { 
		/* para cada intervalo de base1 */
		ind1 = base1->ind[i];
		for (j=0, tem=0; j<base2->total && !tem; j++) { 
			/* procura um intervalo da base2 que o contenha */
			ind2 = base2->ind[j];
			tem = intervalo_contido (&base1->A[ind1], &base1->B[ind1], 
									 &base2->A[ind2], &base2->B[ind2], base1->nv);
			// printf ("tem = %d\n", tem);
		}
	}
	return tem;
}

/*=*/ int diminui_desmonta (trv_struc * desmonta, win_struc * win) {
	int i, j, tam, ind;
	trv_struc base1, base_um;
	trv_struc_list * base, * apt_base;
	
	base = NULL; tam = 0;
	zera_basis_by_ron (&base_um);
	zera_basis_by_ron (&base1);
	
	for (i=0; i<desmonta->total; i++) {
		free_basis_by_ron (&base_um);
		base_um.total = base_um.size = 1;
		base_um.nv = desmonta->win.size;
		base_um.nb = (base_um.nv + NB1) / NB;
		copy_win (&desmonta->win, &base_um.win);
		base_um.A = (int *) xmalloc (sizeof (int) * base_um.nb);
		base_um.B = (int *) xmalloc (sizeof (int) * base_um.nb);
		base_um.ind = (int *) xmalloc (sizeof (int) * base_um.total);
		base_um.ind[0] = 0;
		ind = desmonta->ind[i];
		for (j=0; j<desmonta->nb; j++) {
			base_um.A[j] = desmonta->A[ind+j];
			base_um.B[j] = desmonta->B[ind+j];
		}
		intersection_B_win (&base_um, win);
		if (base_um.total == 1) {
			apt_base = (trv_struc_list *) xmalloc (sizeof(trv_struc_list));
			zera_basis_by_ron (&apt_base->base);
			copy_basis_by_ron (&base_um, &apt_base->base);
			apt_base->prox = base;
			base = apt_base;
			tam++;
		}
	}
	
	if (tam) {
		zera_basis_by_ron (&base1);
		base1.total = base1.size = tam;
		base1.nv = win->size;
		base1.nb = (base1.nv + NB1) / NB;
		copy_win (win, &base1.win);
		base1.A = (int *) xmalloc (sizeof (int) * base1.total * base1.nb);
		base1.B = (int *) xmalloc (sizeof (int) * base1.total * base1.nb);
		base1.ind = (int *) xmalloc (sizeof (int) * base1.total);
		for (i=0; i<base1.total; i++) base1.ind[i] = i * base1.nb;
		for (i=0; i<tam; i++) {
			ind = base1.ind[i];
			for (j=0; j<base1.nb; j++) {
				base1.A[ind+j] = base->base.A[j];
				base1.B[ind+j] = base->base.B[j];
			}
			apt_base = base;
			base = base->prox;
			free_basis_by_ron (&apt_base->base);
			free (apt_base);
		}
		free_basis_by_ron (desmonta);
		copy_basis_by_ron (&base1, desmonta);
		free_basis_by_ron (&base1);
		
	}
	else {free_basis_by_ron (desmonta);}
	
	free_basis_by_ron (&base_um);
	free_basis_by_ron (&base1);
	
	return 1;
}

/*=*/ int desmonta_uma_base (trv_struc * abre, trv_struc * base) {
	int i, retorno, ind, j, * D, * idx, tam_idx, * X, volta;
	int * comb, ii, jj, num_comb, indice, k;
	
	if (abre == NULL || base == NULL) {
		fprintf (stderr, " desmonta_uma_base () pointers to NULL\n");
		return 0;
	}
	
	free_basis_by_ron (abre);
	copy_win (&(base->win), &(abre->win));
	
	
	idx = (int *) xmalloc (sizeof(int) * base->nv);
	/* idx[i] */
	D = (int *) xmalloc (sizeof(int) * base->nb);
	X = (int *) xmalloc (sizeof(int) * base->nb);
	
	for (i=0; i<base->total; i++) { 
		
		/* para cada intervalo [A,B] da base base */
		ind = base->ind[i];
		Bc_menos_A (D, &base->A[ind], &base->B[ind], base->nb);
		
		/* gera os indices do conjunto */
		for (j=0, tam_idx=0; j<base->nv; j++) {
			//printf ("Chegou j=%d %d %d\n", j, tam_idx, base->nv);
			if ((D[j/NB] & mask[j%NB]) != 0) {
				idx[tam_idx++] = j;
			}
		}
		
		/* Primeiro Conjunto X = A*/
		for (jj=0; jj<base->nb; jj++) 
			X[jj] = base->A[ind+jj];
		
		retorno = verifica_base_X (abre, X);
		if (!retorno) coloca_X_na_base (abre, X);
		
		for (j=1; j<=tam_idx; j++) {
			comb = NULL; num_comb = numero_de_combinacoes (tam_idx, j);
			// printf ("Gerando comb (%d, %d)\r", tam_idx, j);
			for (k=0; k<num_comb; k++) {
				_combinacoes_ (tam_idx, j, &comb);
				/* gerando um conjunt X */
				for (jj=0; jj<base->nb; jj++) 
					X[jj] = base->A[ind+jj];
				for (jj=0; jj<j; jj++) {
					indice = idx[comb[jj]];
					X[indice/NB] |= mask[indice%NB];
				}
				/* verifica se X ja' esta' em abre */
				retorno = verifica_base_X (abre, X);
				if (!retorno)
					coloca_X_na_base (abre, X);
			}
			if (comb) {free (comb); comb=NULL;}
		}
	}
	
	if (idx) free (idx);
	if (D) free (D);
	if (X) free (X);
	return 1;
}

/*=*/ int read_uma_base_by_ron (FILE * fd, trv_struc * interv) {
	int nvars, ninterv;
	
	char header[8];
	int  n4bytes, data, i, j, b, st;
	
	/* check file header --------------------------------------------------- */
	fscanf (fd, "%s", header);
	if (strncmp (header, "BASE", 6) != 0) {
		fprintf(stderr, "read_itrv() : unrecognized file format\n") ;
		return 0;
	}
	
	/* input: number of variables and number of examples ------------------- */
	fscanf(fd, "%d %d\n", &nvars, &ninterv);
	
	n4bytes = (nvars+31)/32 ;
	
	st = alloc_itrv (interv, nvars, ninterv);
	if (st == 0) {
		fprintf(stderr, "read_itrv : alloc_interv() failed\n");
		return(0) ;
	}
	
	st = read_win2 (fd, &interv->win);
	if (st == 0){
		fprintf(stderr, "read_win2() failed\n");
		return 0;
	}
	
	i = 0;
	b = 0;
	
	/* reading process ---------------------------------------------------- */
	
	while(i<ninterv) {
		
		interv->ind[i] = b;
		
		for (j=0; j<n4bytes; j++) {     /* reads inferior extremity data */
			fscanf(fd, "%x ", &data);
			interv->A[b+j] = data;
		} 
		
		for (j=0; j<n4bytes; j++) {     /* reads superior extremity data */
			fscanf(fd, "%x ", &data);
			interv->B[b+j] = data;
		} 
		
		i++;
		b = b+n4bytes;
		
	}
	interv->total = ninterv ;
	return 1;
}


/*=*/ int transform_to_small_window (win_struc * win, int lado) {
	int i, j, nshifts, section, cont=0;
	int l_ori, c_ori, l_new_ori, c_new_ori;
	int * new_data, new_tam, mask_antes;
	int new_i, new_j, new_nshifts, new_section, * winidx;
	int linha, coluna, nao_da;
	int X, Y, ori_i, ori_j;
	
	if (win == NULL) {
		fprintf (stderr, "transform_to_big_window () pointer to NULL\n");
		return 0;
	}
	
	if (win->hsize != win->wsize) {
		fprintf (stderr, "hsize and wsize of the window must be equal\n");
		return 0;
	}
	
	if (win->hsize == lado) {
		/* nada a fazer */
		return 1;
	}
	
	win_index (win, &winidx);
	nao_da = 0;
	ori_i = (win->hsize - 1) / 2;
	ori_j = (win->wsize - 1) / 2;
	for (i=0; i<win->size && !nao_da; i++) {
		linha  = winidx[i]/win->wsize;
		coluna = winidx[i]%win->wsize;
		X = coluna - ori_j;
		if (X<0) X=-X;
		Y = ori_i - linha;
		if (Y<0) Y=-Y;
		if (2*X+1 > lado) nao_da = 1;
		if (2*Y+1 > lado) nao_da = 1;
	}
	
	if (winidx) free (winidx);
	
	if (nao_da) {
		fprintf (stderr, "invalid new dimension\n");
		return 0;
	}
	
	
	/* In this case, win->hsize < lado */
	new_tam = (lado * lado + NB1) / NB;
	new_data = (int *) xmalloc (sizeof(int)*new_tam);
	for (i=0; i<new_tam; i++) new_data[i]=0;
	l_new_ori = c_new_ori = (lado-1)/2;
	l_ori = c_ori = (win->hsize-1)/2;
	for (i=0; i<win->hsize; i++) {
		for (j=0; j<win->wsize; j++) {
			nshifts = (i*win->wsize+j)%NB;
			section = (i*win->wsize+j)/NB;
			if ((win->data[section] & mask[nshifts]) != 0) {
				new_i = i + (l_new_ori - l_ori);
				new_j = j + (c_new_ori - c_ori);
				new_nshifts = (new_i*lado+new_j)%NB;
				new_section = (new_i*lado+new_j)/NB;
				new_data[new_section] |= mask[new_nshifts];
				cont++;
			}
		}
	}
	
	win->hsize = win->wsize = lado;
	if (win->data) free (win->data);
	win->data = new_data;
	
	return 1;
}

/*=*/ int transfer_basis_according_window_menor (trv_struc *base, 
                                                 win_struc *win) {
	trv_struc base_aux, base_sai;
	int i, j, ind, erro;
	
	if (base == NULL || win == NULL) {
		fprintf (stderr, "transfer_basis_according_window_menor () ");
		fprintf (stderr, " pointers to NULL\n");
		return 0;
	}
	
	zera_basis_by_ron (&base_sai);
	base_sai.total = base_sai.size = base->total;
	copy_win (win, &base_sai.win);
	base_sai.nv = win->size;
	base_sai.nb = (win->size + NB1)/NB;
	base_sai.ind = (int *) xmalloc (sizeof(int) * base->total);
	base_sai.A = (int *) xmalloc (sizeof(int) * base->total * 
										  base_sai.nb);
	base_sai.B = (int *) xmalloc (sizeof(int) * base->total * 
										  base_sai.nb);
	for (i=0; i<base->total; i++) 
		base_sai.ind[i] = i*base_sai.nb;
	
	zera_basis_by_ron (&base_aux);
	base_aux.total = base_aux.size = 1; /* somente 1 intervalo */
	copy_win (&base->win, &base_aux.win);
	base_aux.nv = base->nv;
	base_aux.nb = base->nb;
	base_aux.ind = (int *) xmalloc (sizeof(int));
	base_aux.A = (int *) xmalloc (sizeof(int) * base_aux.nb);
	base_aux.B = (int *) xmalloc (sizeof(int) * base_aux.nb);
	base_aux.ind[0] = 0;
	
	erro = 0;
	
	for (i=0; i<base->total; i++) {
		ind = base->ind[i];
		for (j=0; j<base->nb; j++) {
			base_aux.A[j] = base->A[ind+j];
			base_aux.B[j] = base->B[ind+j];
		}
		intersection_B_win (&base_aux, win);
		if (base_aux.total == 0) {
			fprintf (stderr, "transfer_basis_according_window_menor failed\n");
			erro = 1;
			break;
		}
		else {
			ind = base_sai.ind[i];
			for (j=0; j<base_sai.nb; j++) {
				base_sai.A[ind+j] = base_aux.A[j];
				base_sai.B[ind+j] = base_aux.B[j];
			}
		}
	}
	
	if (!erro) {
		free_basis_by_ron (base);
		copy_basis_by_ron (&base_sai, base);
	}
	
	free_basis_by_ron (&base_aux);
	free_basis_by_ron (&base_sai);
	return 1;
}

/*=*/ int menor_lado_possivel_da_janela (win_struc * win) {
	int i, *winidx, linha, coluna, lado;
	int ori_i, ori_j, X, Y, maior;
	
	ori_i = (win->hsize - 1) / 2;
	ori_j = (win->wsize - 1) / 2;
	
	win_index (win, &winidx);
	maior = lado=0;
	for (i=0; i<win->size; i++) {
		linha  = winidx[i]/win->wsize;
		coluna = winidx[i]%win->wsize;
		X = coluna - ori_j;
		Y = ori_i - linha;
		// printf ("linha = %d coluna = %d\n", linha, coluna);
		// printf ("X = %d Y = %d\n", X, Y);
		if (X<0) X=-X;
		if (Y<0) Y=-Y;
		if (lado < linha) lado = linha;
		if (lado < coluna) lado = coluna;
		if (maior < X) maior = X;
		if (maior < Y) maior = Y;
	}
	
	// printf ("maior = %d\n", 2*maior+1);
	maior = (2*maior+1)+2;
	if (winidx) free (winidx);
	if (lado%2==0) lado += 3;
	else lado += 2;
	return maior+2;
}

/*=*/ testa_desmonta (trv_struc * desmonta, trv_struc * diff) {
	trv_struc base1, base2;
	int retorno;
	
	zera_basis_by_ron (&base1);
	zera_basis_by_ron (&base2);
	
	copy_win (&desmonta->win, &base1.win);
	negation_basis (desmonta, &base1);
	
	copy_win (&desmonta->win, &base2.win);
	negation_basis (&base1, &base2);
	
	retorno = igual_basis_by_ron (&base2, diff);
	
	free_basis_by_ron (&base1);
	free_basis_by_ron (&base2);
	
	return retorno;
}

int iterative_deepening(str_A_star * A_star) {
//	printf("fdil = %d fero = %d updil = %d upero = %d\n", A_star->f_dil, A_star->f_ero, A_star->upper_bound_dil, A_star->upper_bound_ero);
	if (A_star->S_dil.size > 1) {
		if (A_star->upper_bound_dil < upper_bound_program) {
			upper_bound_program = A_star->upper_bound_dil;
		}
		if (A_star->f_dil < menor_f && A_star->f_dil > last_f) {
			menor_f = A_star->f_dil;
		}
	}
	
	if (A_star->S_ero.size > 1) {
		if (A_star->upper_bound_ero < upper_bound_program) {
			upper_bound_program = A_star->upper_bound_ero;
		}
		if (A_star->f_ero < menor_f && A_star->f_ero > last_f) {
			menor_f = A_star->f_ero;
		}
	}
}


/*=*/ int lvsolbas (char * fname1, char * fname2, char * fname3) {
	trv_struc basis1, basis2;
	mat_str   str;
	trv_struc_list * lower;
	output_search * sai, * apt_sai;
	trv_struc_list * apt_trv, * aux_trv;
	win_struc S;
	str_A_star A_star;
	int lado;
	
	if (fname1 == NULL || fname2 == NULL) {
		fprintf (stderr, "lvsolbas () pointers to NULL\n");
		return 0;
	}
	
	zera_basis_by_ron (&basis1);
	read_itrv (fname1, &basis1);
	read_str (fname2, &str);
	
	detalhe = 0;
	
	lado = menor_lado_possivel_da_janela (&basis1.win);
	transform_to_small_window (&basis1.win, lado);
	
//	printf("f's %d %d %d %d\n", lado_maiorR, A_star.h_dil, A_star.g_dil, A_star.f_dil);
	
	// disp_basis_by_ron (&basis1);
	// disp_win_by_ron (&S);
	
	//disp_win_by_ron (&A_star.S_dil);
	//disp_win_by_ron (&A_star.S_ero);
	
//	printf("f's %d %d %d %d\n", lado_maiorR, A_star.h_ero, A_star.g_ero, A_star.f_ero);
	
	minimo_S(&basis1, &S);
	
	A_star.S_dil = S;
	int lado_maiorR = R_win(&S);
	A_star.h_dil = (lado_maiorR / 2) * 9;
	if (A_star.h_dil < 9) A_star.h_dil = 9;
	A_star.g_dil = 0;
	A_star.f_dil = A_star.h_dil + A_star.g_dil;

	A_star.upper_bound_dil = upper_bound(&basis1);

	zera_basis_by_ron (&basis2);
	free_basis_by_ron (&basis2);
	basis_dual_operator (&basis1, &basis2);
	
//	disp_basis_by_ron(&basis2);
	
	minimo_S(&basis2, &S);
	
	A_star.S_ero = S;
	lado_maiorR = R_win(&S);
	A_star.h_ero = (lado_maiorR / 2) * 9;
	if (A_star.h_ero < 9) A_star.h_ero = 9;
	A_star.g_ero = 0;
	A_star.f_ero = A_star.h_ero + A_star.g_ero;

	A_star.upper_bound_ero = upper_bound(&basis2);
	
//	printf("f's dil %d %d %d %d %d\n", lado_maiorR, A_star.h_dil, A_star.g_dil, A_star.f_dil, A_star.upper_bound_dil);
//	printf("f's ero %d %d %d %d %d\n", lado_maiorR, A_star.h_ero, A_star.g_ero, A_star.f_ero, A_star.upper_bound_ero);
	
	menor_f = 0;
	while (num_sol == 0) {
		last_f = menor_f;
		if (last_f == oo) menor_f = oo - 1;
		else menor_f = oo;
		upper_bound_program = oo;		
		iterative_deepening(&A_star);
		
		printf("Menores menor_f %d y last_f %d\n", menor_f, last_f);
		
		paralelo_para_sequencial (&basis1, &A_star);
	}
	
	free_basis_by_ron (&basis1);
	if (str.tag == MM_IMG && str.dat) free (str.dat);
	
	return 1;
}

/*=*/ int main (int argc, char * * argv) {
	int detalhe = 1;
	beatriz = 0;
	
	if (detalhe) {
		if (!lvsolbas(argv[1], argv[2], argv[3])) {
			fprintf(stderr, "vlsol : lvsolbas() failed\n");
			return 1;
		}
	}

	return 0;
}
// ************************************************************************

/*=*/ int aha_crescente_W_by_edu (aha_struc * sai, trv_struc * base, mat_str * str, trv_struc * upper) {
	trv_struc elle;
	int i, ii, tam, ind1;
	
	if (sai == NULL || base == NULL || str == NULL) {
		fprintf (stderr, "aha_W () pointers to NULL\n");
		return 0;
	}
	
	if (base->size != 1) {
		fprintf (stderr, "aha(): The basis must represent just one interval\n");
		return 0;
	}
//	printf("asdf\n");
//	disp_basis_by_ron(upper);
	
	zera_basis_by_ron (&elle);
	
//	base_ele_erre_W (&elle, base, str);
	base_ele_erre_W_by_edu (&elle, base, str, upper);
	
	if (elle.total==0) {
		zera_aha (sai);
		free_basis_by_ron (&elle);
		return 1;
	}
	
	tam = sai->total = elle.total;
	sai->nv = elle.nv;
	sai->nb = (sai->nv+NB1)/NB;
	copy_win (&elle.win, &sai->win);
	sai->intervs = (int_struc *) xmalloc (sizeof(int_struc)*tam);
	
	for (i=0; i<elle.total; i++) {
		ind1 = elle.ind[i];
		sai->intervs[i].total = 1;
		sai->intervs[i].A = (int *) xmalloc (sizeof(int)*sai->nb);
		sai->intervs[i].B = (int *) xmalloc (sizeof(int)*sai->nb);
		sai->intervs[i].ind = (int *) xmalloc (sizeof(int));
		sai->intervs[i].ind[0] = 0;
		for (ii=0; ii<elle.nb; ii++) {
			sai->intervs[i].A[ii] = elle.A[ind1+ii];
			sai->intervs[i].B[ii] = elle.B[ind1+ii];
		}
	}
	
	free_basis_by_ron (&elle);
	
	return 1;
}

/*=*/ int aha_crescente_W (aha_struc * sai, trv_struc * base, mat_str * str) {
	trv_struc elle;
	int i, ii, tam, ind1;
	
	if (sai == NULL || base == NULL || str == NULL) {
		fprintf (stderr, "aha_W () pointers to NULL\n");
		return 0;
	}
	
	if (base->size != 1) {
		fprintf (stderr, "aha(): The basis must represent just one interval\n");
		return 0;
	}
	
	zera_basis_by_ron (&elle);
	
	base_ele_erre_W (&elle, base, str);
	
	if (elle.total==0) {
		zera_aha (sai);
		free_basis_by_ron (&elle);
		return 1;
	}
	
	tam = sai->total = elle.total;
	sai->nv = elle.nv;
	sai->nb = (sai->nv+NB1)/NB;
	copy_win (&elle.win, &sai->win);
	sai->intervs = (int_struc *) xmalloc (sizeof(int_struc)*tam);
	
	for (i=0; i<elle.total; i++) {
		ind1 = elle.ind[i];
		sai->intervs[i].total = 1;
		sai->intervs[i].A = (int *) xmalloc (sizeof(int)*sai->nb);
		sai->intervs[i].B = (int *) xmalloc (sizeof(int)*sai->nb);
		sai->intervs[i].ind = (int *) xmalloc (sizeof(int));
		sai->intervs[i].ind[0] = 0;
		for (ii=0; ii<elle.nb; ii++) {
			sai->intervs[i].A[ii] = elle.A[ind1+ii];
			sai->intervs[i].B[ii] = elle.B[ind1+ii];
		}
	}
	
	free_basis_by_ron (&elle);
	
	return 1;
}

/*=*/ int gera_lower_bounds_crescente (trv_struc_list **s, trv_struc * base, 
                                       mat_str * str) {
	trv_struc * quebrada;
	trv_struc upper;
	int i, j, * vet_apt, total, tem;
	aha_struc * aha;
	trv_struc * esse, base1;
	trv_struc_list * sai, *apt_list;
	int tam_sai;
	int lado;
	FILE * fd;
	
	win_struc win_in, win_ero;
	mat_str str_rot;
	
	if (s == NULL || base == NULL || str == NULL) {
		fprintf (stderr, "gera_lower_bounds () pointers to NULL\n");
		return 0;
	}
	
	quebrada = (trv_struc *) xmalloc (sizeof (trv_struc) * 
											  base->total);
	
	for (i=0; i<base->total; i++) zera_basis_by_ron (&quebrada[i]);
	quebra_base_intervalos (quebrada, base);
	
	aha = (aha_struc *) xmalloc (sizeof (aha_struc) * base->total);
	for (i=0; i<base->total; i++) zera_aha (&aha[i]);
	
	geral = 0;
	
	// começa upper
	
	/* calcula o upper bound */
	zera_basis_by_ron (&upper);
	upper_bound_by_ron (&upper, base, str);
	lado = menor_lado_possivel_da_janela (&(upper.win));
	transform_to_small_window (&(upper.win), lado);
	
	/* transpondo o elemento struturante */
	str_rot.dat = NULL;
	lvrotstr (str, 4, 0, &str_rot);
	
	copy_win (&(base->win), &win_in);
	
//	win_ero.size = win_ero.hsize = win_ero.wsize = 0;

	zera_win_by_ron (&win_ero);
	free_win_by_ron (&win_ero);
	
	/* construindo a janela $W=W'\ominus C^t$ W =W' ⊖ Ct.*/
	erode_win (&win_in, &win_ero, &str_rot);
	
	diminui_desmonta (&upper, &win_ero);
	
	printf("UPPER\n");
	disp_basis_by_ron (&upper);
	
	// fin upper
	
	for (i=0; i<base->total; i++) {
//		aha_crescente_W (&aha[i], &quebrada[i], str);
		aha_crescente_W_by_edu (&aha[i], &quebrada[i], str, &upper);
		if (beatriz) printf ("i = %02d => %03d Pairs\n", i, aha[i].total);
		free_basis_by_ron (&quebrada[i]);
	}
	
	if (quebrada) free (quebrada);
	quebrada = NULL;
	
	/* os conjuntos aha's ja' foram montados */
	/* cada casa de vet_apt[j] guarda um indice de um intervalo de aha[j] */
	vet_apt = (int *) xmalloc (sizeof (int) * base->total);
	
	for (i=0, total=1; i<base->total; i++) {
		vet_apt[i] = 0;
		if (aha[i].total) total *= aha[i].total;
	}
	
	esse = (trv_struc *) xmalloc (sizeof (trv_struc) * base->total);
	for (i=0; i<base->total; i++) zera_basis_by_ron (&esse[i]);
	
	sai = NULL; tam_sai = 0;
	zera_basis_by_ron (&base1);
	if (beatriz) printf ("total de possibilidades = %d\n", total);
	for (i=0; i<total; i++) { /* num. total de prod. cartesiano */
		if (beatriz) {
			printf ("%s\r", " ");
			printf ("%d\r", i);
		}
		
		free_basis_by_ron (&base1);
		// printf ("Vetor de Indices ");
		// printf ("%02d => ", i+1);
		// for (j=0; j<base->total; j++) printf ("%02d ", vet_apt[j]); printf ("\n");
		gera_um_Z (&base1, vet_apt, aha, base->total, esse);
		// printf ("Base Gerada:\n");
		// disp_basis_by_ron (&base1);
		if (base1.total) {
			for (apt_list = sai, tem=0; apt_list && !tem; apt_list = apt_list->prox) {
				tem = igual_basis_by_ron (&apt_list->base, &base1);
			}
			if (!tem) {
				apt_list = (trv_struc_list *) xmalloc (sizeof (trv_struc_list));
				copy_basis_by_ron (&base1, &apt_list->base);
				apt_list->prox = sai;
				sai = apt_list;
				tam_sai++;
				// printf ("tam_sai = %d\n", tam_sai);
			}
		}
	}
	
	fd = fopen ("Z.txt", "w");
	fprintf (fd, "%d\n", tam_sai);
	fclose (fd);
	
	*s = sai;
	
	/* finalizando e matando o que tem que matar */
	i = 0;
	if (beatriz) printf ("Temos %d Lower Bounds:\n", tam_sai);
	while (sai) {
		apt_list = sai;
		sai = sai->prox;
		// printf ("Basis %d:\n", ++i);
		// disp_basis_by_ron (&apt_list->base);
		append_basis_by_ron ("Z.txt", &apt_list->base);
	}
	
	for (i=0; i<base->total; i++) {
		free_aha (&aha[i]);
		free_basis_by_ron (&esse[i]);
	}
	
	// printf ("Temos %d Lower Bounds\n", tam_sai);
	
	if (esse) free (esse);
	if (aha) free (aha);
	if (vet_apt) free (vet_apt);
	return 1;
}

/*=*/ int testa_equacao_dilatacao_crescente (trv_struc * base, mat_str * str, 
                                             trv_struc_list * Z,
											 trv_struc_list * * sai) {
	trv_struc_list * apt_sol, * sol;
	trv_struc_list * apt_Z;
	trv_struc upper, diferenca, desmonta, base_X, base_Y;
	win_struc W_duas_linhas;
	int * idx, i, j, k, kk, ind, * comb, num_comb, tam_idx, tem, cont_sol;
	int rr, ind1, ind2;
	int igual, retorno;
	trv_struc base_D, base_E, base_Z;
	int lado, cont_Z;
	FILE * fd;
	
	if (base == NULL || str == NULL) {
		fprintf (stderr, "testa_equacao_dilatacao_crescente () pointers to NULL\n");
		return 0;
	}
	
	/* calcula Z */
	// le_os_Z (fname, &Z);
	
	sol = NULL; *sai = NULL;
	
	if (Z) {
		zera_basis_by_ron (&base_X);
		zera_basis_by_ron (&base_Y);
		zera_basis_by_ron (&base_D);
		zera_basis_by_ron (&base_E);
		zera_basis_by_ron (&desmonta);
		zera_basis_by_ron (&base_Z);
		cont_sol = 0;
		
		lado = menor_lado_possivel_da_janela (&(base->win));
		transform_to_small_window (&(base->win), lado);
		
		/* calcula o upper bound */
		zera_basis_by_ron (&upper);
		upper_bound_by_ron (&upper, base, str);
		lado = menor_lado_possivel_da_janela (&(upper.win));
		transform_to_small_window (&(upper.win), lado);
		
		/* transfere upper para a janela W */
		// transfer_basis_according_window_menor (&upper, &(Z->base.win));
		
		/* calcula a janela W'' */
//		zera_win_by_ron (&W_duas_linhas);
//		calcula_W_duas_linhas (&W_duas_linhas, &(Z->base.win), str);
//		lado = menor_lado_possivel_da_janela (&W_duas_linhas);
//		transform_to_small_window (&W_duas_linhas, lado);
		
		/* transfere upper para a janela W'' */
		// transfer_basis_according_window (&upper, &W_duas_linhas);
		
		/* Diminui o upper de acordo com a janela W */
		diminui_desmonta (&upper, &Z->base.win);
		
		if (detalhe) disp_basis_by_ron (&upper);
		
		zera_basis_by_ron (&diferenca);
		cont_Z = 1;
		for (apt_Z = Z; apt_Z; apt_Z = apt_Z->prox) {
			
			/* Verifica se Z <= upper */
			free_basis_by_ron (&base_Z);
			copy_basis_by_ron (&(apt_Z->base), &base_Z);
			// transfer_basis_according_window (&base_Z, &W_duas_linhas);
			/* aqui base_Z esta' definido em W'' */
			retorno = verifica_bases_menor (&base_Z, &upper);
			// printf ("retorno = %d\n", retorno);
			
			if (beatriz) 
				printf ("========================================================\n");
			
			/* Se Z nao e' menor ou igual a upper, passe para o proximo Z */
			if (!retorno) {
				if (beatriz) printf ("Lower Bound %d ignorado\n", cont_Z++);
				continue;
			}
			
			if (beatriz) {
				printf ("Lower Bound %d considerado: %d intervalos\n", 
						cont_Z++, apt_Z->base.total);
			}
			
			/* Neste ponto Z<=upper */
			/* Verifica se Z e' uma solucao */
			free_basis_by_ron (&base_X);
			copy_basis_by_ron (&(apt_Z->base), &base_X);
			
			free_basis_by_ron (&base_Y);
			dilate_basis (&base_X, str, &base_Y);
			
			/* transfere base_Y para a janela W' */
			transfer_basis_according_window (&base_Y, &base->win);
			
			igual = igual_basis_by_ron (&base_Y, base);
			if (igual) {
				// printf ("Tem uma solucao\n");
				tem = 0;
				for (apt_sol = sol; apt_sol && !tem; apt_sol = apt_sol->prox) {
					tem = igual_basis_by_ron (&base_X, &apt_sol->base);
				}
				if (!tem) {
					apt_sol = (trv_struc_list *) 
					xmalloc (sizeof(trv_struc_list));
					zera_basis_by_ron (&apt_sol->base);
					copy_basis_by_ron (&base_X, &apt_sol->base);
					apt_sol->prox = sol;
					sol = apt_sol; cont_sol++;
				}
			}
			else { /* igual == 0 */
				if (beatriz) 
					printf ("Lower Bound %d nao considerado: %d intervalos\n", 
							cont_Z++, apt_Z->base.total);
				continue;
			}
			
			/* Agora vamos ver as bases intermediarias entre base_Z e upper */
			// free_basis_by_ron (&base_Z);
			// copy_basis_by_ron (&(apt_Z->base), &base_Z);
			// transfer_basis_according_window (&base_Z, &W_duas_linhas);
			/* aqui base_Z esta' definido em W'' */
			
			/* Diminui o base_W de acordo com a janela W */
			// diminui_desmonta (&base_Z, &apt_Z->base.win);
			/* Calcula a diferenca upper - Z */
			free_basis_by_ron (&diferenca);
			if (beatriz) printf ("Calculando a diferenca\n");
			diff_basis_by_ron (&diferenca, &base_Z, &upper);
			// diminui_diferenca (&diferenca, &apt_Z->base.win);
			
			// printf ("========================================================\n");
			// printf ("Base Z:\n");
			// disp_basis_by_ron (&base_Z);
			// printf ("Base UPPER:\n");
			// disp_basis_by_ron (&upper);
			// printf ("Base Diferenca:\n");
			// disp_basis_by_ron (&diferenca);
			
			free_basis_by_ron (&desmonta);
			if (beatriz) printf ("Desmontando a diferenca\n");
			desmonta_uma_base (&desmonta, &diferenca);
			if (beatriz) printf ("desmonta = %d\n", desmonta.total);
			// printf ("Deu %d\n", testa_desmonta (&desmonta, &diferenca));
			
			// disp_basis_by_ron (&desmonta);
			
			// diminui_desmonta (&desmonta, &apt_Z->base.win);
			// printf ("desmonta diminuido = %d\n", desmonta.total);
			// printf ("Deu diminuido %d\n", testa_desmonta (&desmonta, &diferenca));
			
			// disp_desmonta_by_ron (&desmonta);
			
			// continue; // para ser retirado
			
			for (j=1; j<=desmonta.total; j++) {
				comb = NULL; num_comb = numero_de_combinacoes (desmonta.total, j);
				for (k=0; k<num_comb; k++) {
					if (beatriz) printf ("j = %d e k = %d/%d\n", j, k+1, num_comb);
					if (beatriz) {
						if (k==num_comb-1) {
							printf ("j = %d k = %d sol = %d\n", j, num_comb, cont_sol);
						}
					}
					_combinacoes_ (desmonta.total, j, &comb);
					free_basis_by_ron (&base_D);
					copy_win (&upper.win, &base_D.win);
					base_D.total = base_D.size = j;
					base_D.nv = upper.win.size;
					base_D.nb = (base_D.nv + NB1) / NB;
					base_D.ind = (int *) xmalloc (sizeof(int) * base_D.total);
					base_D.A = (int *) xmalloc (sizeof(int) * base_D.total * 
														base_D.nb);
					base_D.B = (int *) xmalloc (sizeof(int) * base_D.total * 
														base_D.nb);
					for (kk=0; kk<j; kk++) {
						base_D.ind[kk] = kk * base_D.nb;
					}
					for (kk=0; kk<j; kk++) {
						ind1 = desmonta.ind[comb[kk]];
						ind2 = base_D.ind[kk];
						for (rr=0; rr<base_D.nb; rr++) {
							base_D.A[ind2+rr] = desmonta.A[ind1+rr];
							base_D.B[ind2+rr] = (~desmonta.A[ind1+rr]);
						}
					}
					free_basis_by_ron (&base_E);
					copy_basis_by_ron (&base_Z, &base_E);
					free_basis_by_ron (&base_X);
					union_basis_by_ron (&base_E, &base_D, &base_X);
					free_basis_by_ron (&base_D);
					free_basis_by_ron (&base_E);
					/* aqui nos temos uma possivel solucao base_X */
					/* transfere base_X para a janela W */
					transfer_basis_according_window_menor (&base_X, &(apt_Z->base.win));
					
					/* testa se \delta_C base_X = base */
					free_basis_by_ron (&base_Y);
					dilate_basis(&base_X, str, &base_Y);
					
					/* transfere base_Y para a janela W' */
					transfer_basis_according_window (&base_Y, &base->win);
					
					igual = igual_basis_by_ron (&base_Y, base);
					if (igual) {
						tem = 0;
						for (apt_sol = sol; apt_sol && !tem; apt_sol = apt_sol->prox) {
							tem = igual_basis_by_ron (&base_X, &apt_sol->base);
						}
						if (!tem) {
							apt_sol = (trv_struc_list *) 
							xmalloc (sizeof(trv_struc_list));
							zera_basis_by_ron (&apt_sol->base);
							copy_basis_by_ron (&base_X, &apt_sol->base);
							apt_sol->prox = sol;
							sol = apt_sol; cont_sol++;
						}
					}
				}
				if (comb) {
					free (comb);
					// printf ("Alguma coisa cheira mal\n");
				}
			}
		}
		
		free_basis_by_ron (&upper);
		free_basis_by_ron (&diferenca);
		free_basis_by_ron (&base_X);
		free_basis_by_ron (&base_D);
		free_basis_by_ron (&base_E);
		free_basis_by_ron (&desmonta);
		free_basis_by_ron (&base_Z);
		
		fd = fopen ("X.txt", "w");
		fprintf (fd, "%d\n", cont_sol);
		fclose (fd);
		
		if (beatriz) 
			printf ("========================================================\n");
		i = 0;
		if (beatriz) printf ("Temos %d Solucoes\n", cont_sol);
		* sai = sol;
		while (sol) {
			apt_sol = sol;
			sol = sol->prox;
			if (beatriz) {
				printf ("Solucao %d:\n", ++i);
				disp_basis_by_ron (&apt_sol->base);
			}
			append_basis_by_ron ("X.txt", &apt_sol->base);
			// free_basis_by_ron (&apt_sol->base);
			// free (apt_sol);
		}
	}
	
	return 1;
}

/*=*/ int minimo_lado_esquerdo (trv_struc * sai, trv_struc * base) { //determina qual é o intervalo de base que tem menos 1's
	int i, ind, j, section, nshifts, cont;
	int * index, tam_index, menor, primeira_vez, ind2;
	
	if (base == NULL || sai == NULL) {
		fprintf (stderr, "minimo_lado_esquerdo () pointers to NULL\n");
		return 0;
	}
	
	primeira_vez = 1;
	for (i=0; i<base->total; i++) {
		ind = base->ind[i]; cont = 0;
		for (j=0; j<base->nv; j++) {
			section = j/NB;
			nshifts = j%NB;
			cont += ((base->A[ind+section] & mask[nshifts]) == mask[nshifts]);
		}
		if (primeira_vez) {menor=cont; primeira_vez=0;}
		if (menor > cont) menor = cont;
	}
	
	index = (int *) xmalloc (sizeof (int) * base->total);
	tam_index = 0;
	for (i=0; i<base->total; i++) {
		ind = base->ind[i]; cont = 0;
		for (j=0; j<base->nv; j++) {
			section = j/NB;
			nshifts = j%NB;
			cont += ((base->A[ind+section] & mask[nshifts]) == mask[nshifts]);
		}
		if (menor == cont) {
			index[tam_index++] = i;
		}
	}
	
	zera_basis_by_ron (sai);
	sai->total = sai->size = tam_index;
	copy_win (&base->win, &sai->win);
	sai->nv = base->nv;
	sai->nb = base->nb;
	sai->A = (int *) xmalloc (sizeof(int) * sai->total * sai->nb);
	sai->B = (int *) xmalloc (sizeof(int) * sai->total * sai->nb);
	sai->ind = (int *) xmalloc (sizeof(int) * sai->total);
	for (i=0; i<sai->total; i++) sai->ind[i] = i*sai->nb;
	
	for (i=0; i<sai->total; i++) {
		ind = sai->ind[i];
		ind2 = base->ind[index[i]];
		for (j=0; j<sai->nb; j++) {
			sai->A[ind+j] = base->A[ind2+j];
			sai->B[ind+j] = base->B[ind2+j];
		}
	}
	
	if (index) free (index);
	return 1;
}

int transform_to_win3x3(win_struc * win) {
	if (win->size > 9) { win->size = 1; return 1; }
	
	int * new_data;
	int  nshifts;
	int  mask;
	int  cnt;
	int  i, j, section;
	int  data;
	int menorL = 1 << 30, maiorL = -1, menorA = 1 << 30, maiorA = -1, resposta = 0;
	// disp_win_by_ron(win);
	
	if (win == NULL) {
		fprintf (stderr, "R_win () pointer to NULL\n");
		return 0;
	}
	printf("transform_to_win3x3\n");
	disp_win_by_ron(win);
	
	for(i=0; i<win->hsize; i++) {
		for(j=0; j<win->wsize; j++) {
			nshifts = ((i*win->wsize)+j)%NB ;
			mask = 1<<nshifts ;
			section = ((i*win->wsize)+j)/NB ;
			data = win->data[section] ;
			//			printf("mask = %d, section = %d, i = %d j = %d win = %d\n", mask, section, i, j, win->data[section]);
			if((data & mask) == mask) {
				menorL = j < menorL ? j : menorL;
				maiorL = j > maiorL ? j : maiorL;
				menorA = i < menorA ? i : menorA;
				maiorA = i > maiorA ? i : maiorA;
			}
		}
	}
	
	if ((maiorL - menorL) > 2 || (maiorA - menorA) > 2) {
		win->size = 1; return 1; 
	}
	
	cnt = -1;
	new_data = (int *) xmalloc (sizeof(int)*3);
	for (i=0; i < 3; i++) new_data[i]=0;
	
	int centery = menorA;
	int centerx = menorL;
	if (maiorL - menorL == 2) 
		centerx = (maiorL + menorL) / 2;
	if (maiorA - menorA == 2)
		centery = (maiorA + menorA) / 2;
	
	for(i = centery - 1; i <= centery + 1; i++) {
		for(j = centerx - 1; j <= centerx + 1; j++) {
			cnt++;
			if (i < menorA || i > maiorA || j < menorL || j > maiorL) continue;
			nshifts = ((i*win->wsize)+j)%NB ;
			mask = 1<<nshifts ;
			section = ((i*win->wsize)+j)/NB ;
			data = win->data[section] ;
			//			printf("mask = %d, section = %d, i = %d j = %d win = %d\n", mask, section, i, j, win->data[section]);
			if((data & mask) == mask) {
				new_data[0] |= (1<<cnt);
			}
		}
	}
	
	win->hsize = win->wsize = 3;
	if (win->data) free (win->data);
	win->data = new_data;
	
	disp_win_by_ron(win);
	
	return 1;
}

int verify_3x3(win_struc * win) {
	if (win->size > 9) { win->size = 1; return 1; }
	int  nshifts;
	int  mask;
	int  cnt;
	int  i, j, section;
	int  data;
	int menorL = 1 << 30, maiorL = -1, menorA = 1 << 30, maiorA = -1, resposta = 0;
	// disp_win_by_ron(win);
	
	if (win == NULL) {
		fprintf (stderr, "verify_3x3 () pointer to NULL\n");
		return 0;
	}
	disp_win_by_ron(win);
	
	for(i=0; i<win->hsize; i++) {
		for(j=0; j<win->wsize; j++) {
			nshifts = ((i*win->wsize)+j)%NB ;
			mask = 1<<nshifts ;
			section = ((i*win->wsize)+j)/NB ;
			data = win->data[section] ;
			//			printf("mask = %d, section = %d, i = %d j = %d win = %d\n", mask, section, i, j, win->data[section]);
			if((data & mask) == mask) {
				menorL = j < menorL ? j : menorL;
				maiorL = j > maiorL ? j : maiorL;
				menorA = i < menorA ? i : menorA;
				maiorA = i > maiorA ? i : maiorA;
			}
		}
	}
	
	if ((maiorL - menorL) > 2 || (maiorA - menorA) > 2)
		return 0; 
	return 1;
}

/*=*/ int esse_min_lado_esquerdo_min (win_struc * conj, trv_struc * min,
                                      trv_struc * base) {
	int i, j, ind, tam, k, rr;
	win_struc * win_vec, * win_vec_str, esse, win_ero;
	mat_str   * str_vec;
	int * winidx, primeira, tam_str;
	int nshifts, block, win_nshifts, section, menor, primeira_vez;
	
	/* para cada elemento A de min */
	/* faz a erosao por todos elementos de base de mesma cardinalidade que A */
	/* se a erosao der cardinalidade 1, entao este ponto esta'  e conj */
	
	/* para cada elemento de min, gerar uma janela */
	win_vec = (win_struc*) xmalloc (sizeof(win_struc) * min->total);
	
	tam = (min->win.hsize*min->win.wsize+NB1)/NB;
	for (i=0; i<min->total; i++) {
		win_vec[i].size  = 0;
		win_vec[i].hsize = min->win.hsize;
		win_vec[i].wsize = min->win.wsize;
		win_vec[i].data  = (int *) xmalloc (sizeof (int)*tam);
		for (j=0; j<tam; j++) win_vec[i].data[j] = 0;
	}
	
	win_index (&min->win, &winidx);
	
	for (i=0; i<min->total; i++) {
		ind = min->ind[i];
		for (j=0; j<min->nv; j++) {
			nshifts = j%NB; block   = j/NB;
			if ((min->A[ind+block] & mask[nshifts]) != 0) {
				win_nshifts = winidx[j]%NB;
				section     = winidx[j]/NB;
				win_vec[i].data[section] |= mask[win_nshifts];
				(win_vec[i].size)++;
			}
		}
//		printf ("\n");
//		printf ("win_vec[%d]:\n", i);
//		disp_win_by_ron (&win_vec[i]);
//		printf ("\n");
	}
	
	/* para cada elemento de base, gerar uma janela */
	win_vec_str = (win_struc*) xmalloc (sizeof(win_struc) * base->total);
	str_vec     = (mat_str*) xmalloc (sizeof(mat_str) * base->total);
	
	tam = (base->win.hsize*base->win.wsize+NB1)/NB;
	tam_str = (base->win.hsize*base->win.wsize);
	for (i=0; i<base->total; i++) {
		win_vec_str[i].size  = 0;
		str_vec[i].m = win_vec_str[i].hsize = base->win.hsize;
		str_vec[i].n = win_vec_str[i].wsize = base->win.wsize;
		str_vec[i].p = 1;
		str_vec[i].tag = MM_IMG;
		win_vec_str[i].data  = (int *) xmalloc (sizeof (int)*tam);
		str_vec[i].dat = (short *) xmalloc (sizeof (short)*tam_str);
		for (j=0; j<tam; j++) win_vec_str[i].data[j] = 0;
		for (j=0; j<tam_str; j++) str_vec[i].dat[j] = 0;
	}
	
	// disp_basis_by_ron (base);
	// disp_basis_by_ron (min);
	
	for (i=0; i<base->total; i++) {
		ind = base->ind[i];
		for (j=0; j<base->nv; j++) {
			nshifts = j%NB; block   = j/NB;
			if ((base->A[ind+block] & mask[nshifts]) != 0) {
				win_nshifts = winidx[j]%NB;
				section     = winidx[j]/NB;
				win_vec_str[i].data[section] |= mask[win_nshifts];
				(win_vec_str[i].size)++;
				str_vec[i].dat[winidx[j]] = 1;
				// if (i==1) printf ("M %d %d\n", winidx[j], str_vec[i].dat[winidx[j]]);
			}
		}
//		for (rr=0; rr<tam_str; rr++) printf ("%d ", str_vec[i].dat[rr]);
//		printf ("\n");
//		printf ("win_vec_str[%d]:\n", i);
//		disp_win_by_ron (&win_vec_str[i]);
//		printf ("\n");
	}
	/* Agora sim, da' para fazer as contas ... */
	
	zera_win_by_ron (&win_ero);
	zera_win_by_ron (&esse);
	primeira = 1;
	for (i=0; i<min->total; i++) {
		primeira_vez = 1;
		free_win_by_ron (&esse);
		for (j=0; j<base->total; j++) {
//			printf("Sizes win_vec[%d] = %d win_vec_str[%d] = %d\n", i, win_vec[i].size, j, win_vec_str[j].size);
			if (win_vec[i].size == win_vec_str[j].size) {
				free_win_by_ron (&win_ero);
				erode_win (&win_vec[i], &win_ero, &str_vec[j]);
//				printf ("Erosao: %d %d\n", i, j);
//				printf ("win_vec[%d]\n", i);
//				disp_win_by_ron (&win_vec[i]);
//				printf ("win_vec_str[%d]\n", j);
//				disp_win_by_ron (&win_vec_str[j]);
//				printf ("Erosao win_vec[%d] por win_vec_str[%d]\n", i, j);
//				disp_win_by_ron (&win_ero);
//				printf ("Esse\n");
				if (win_ero.size == 1) {
					if (primeira_vez) {
						primeira_vez = 0;
						copy_win (&win_ero, &esse);
//						printf ("Primeira Erosao:\n");
//						disp_win_by_ron (&esse);
//						printf ("\n");
					}
					else {
						if (esse.hsize < win_ero.hsize) {
							transform_to_big_window (&esse, win_ero.hsize);
						}
						else {
							transform_to_big_window (&win_ero, esse.hsize);
						}
						tam = (esse.hsize*esse.wsize + NB1) / NB;
						for (k=0; k<tam; k++) esse.data[k] |= win_ero.data[k];
						esse.size = function_size_win (&esse);
//						printf ("Outras Erosoes \n");
//						disp_win_by_ron (&esse);
//						printf ("\n");
					}
				}
			}
//			printf ("Final: %d\n", i);
//			disp_win_by_ron (&esse);
//			printf ("\n");
		}
		if (primeira) {
			primeira = 0;
			menor = esse.size;
			copy_win (&esse, conj);
		}
		else {
			if (menor > esse.size) {
				free_win_by_ron (conj);
				menor = esse.size;
				copy_win (&esse, conj);
			}
		}
	}
	
//	transform_to_win3x3(conj);
	
	// printf ("menor = %d\n", menor);
	
	/* mantando ... */
	
	free_win_by_ron (&esse);
	free_win_by_ron (&win_ero);
	
	for (i=0; i<min->total; i++) {
		free (win_vec[i].data);
	}
	
	free (win_vec);
	
	for (i=0; i<base->total; i++) {
		free (win_vec_str[i].data);
		free (str_vec[i].dat);
	}
	
	free (win_vec_str);
	free (str_vec);
	if (winidx) free (winidx);
	return 1;
}

/*=*/ int window_to_str (win_struc * win, mat_str * str) {
	int * winidx, tam, j;
	tam = (win->hsize*win->wsize);
	str->m = win->hsize;
	str->n = win->wsize;
	str->p = 1;
	str->tag = MM_IMG;
	str->dat = (short *) xmalloc (sizeof (short)*tam);
	winidx = NULL;
	win_index (win, &winidx);
//	printf("Win h, Win w : %d %d\n", win->hsize, win->wsize);
	for (j=0; j<tam; j++) str->dat[j] = 0;
	for (j=0; j<win->size; j++) str->dat[winidx[j]] = 1; 
	if (winidx) free (winidx);
	return 1;
}

/*=*/ int invariante_window (win_struc * win1, win_struc * win2) {
	int invariante, tam, j, i;
	mat_str str;
	win_struc win_ero, win_dil, * vector, win_aux;
	int points, lado;
	
	zera_win_by_ron (&win_ero);
	zera_win_by_ron (&win_dil);
	zera_win_by_ron (&win_aux);
	
	window_to_str (win2, &str);
	
	vector = (win_struc *) xmalloc (sizeof (win_struc) * str.m * str.n);
	
	// printf ("Window:\n");
	// disp_win_by_ron (win1);
	// printf ("\n");
	
	// printf ("Elemento Estruturante:\n");
	// disp_win_by_ron (win2);
	// printf ("\n");
	
	erode_win (win1, &win_ero, &str);
	// printf ("Invariante: Erosao:\n");
	// disp_win_by_ron (&win_ero);
	// printf ("\n");
	dilate_win (&win_ero, &str, &win_dil, vector, &points);
	lado = menor_lado_possivel_da_janela (&win_dil);
	transform_to_small_window (&win_dil, lado);
	// printf ("Invariante: Dilatacao:\n");
	// disp_win_by_ron (&win_dil);
	// printf ("\n");
	free_vect_win (vector, points);
	free (vector);
	
	copy_win (win1, &win_aux);
	if (win_dil.hsize < win1->hsize) 
		transform_to_big_window (&win_dil, win1->hsize);
	else
		transform_to_big_window (&win_aux, win_dil.hsize);
	
	tam = (win_aux.hsize*win_aux.wsize+NB1)/NB;
	
	invariante = 1;
	for (i=0; i<tam; i++) {
		if (win_aux.data[i] != win_dil.data[i]) {
			invariante = 0;
			break;
		}
	}
	
	free_win_by_ron (&win_ero);
	free_win_by_ron (&win_aux);
	free_win_by_ron (&win_dil);
	free (str.dat);
	
	return invariante;
}


int minimo_S(trv_struc * base, win_struc * S) {
	trv_struc minimo;
	// printf("ini minimo S\n");
	//zera_basis_by_ron (&minimo);
	//zera_win_by_ron (&S);
	// disp_basis_by_ron (base);
	minimo_lado_esquerdo (&minimo, base); //intervalos da base que tem menor quantidade de 1's
//	disp_basis_by_ron (&minimo);
	esse_min_lado_esquerdo_min (S, &minimo, base);	
//	printf ("Menor Elemento Estruturante: N. de Pontos = %d\n", S->size);
//	disp_win_by_ron (S);
	free_basis_by_ron (&minimo);
	// printf("fin minimo S\n");
	return 1;
}

/*=*/ int search_increasing_all (trv_struc * base, win_struc * S, output_search * * sai) {
	win_struc C;
	int * comb, num_comb, tam_idx, *winidx, i, j, k, tem;
	int section, nshifts, tam, invariante;
	mat_str str;
	trv_struc_list * lower;
	win_struc_list * Lista_C, * apt_Lista_C;
	int tam_Lista_C;
	output_search * apt_sai;
	trv_struc_list * apt_trv, * solucao;
	
	zera_win_by_ron (&C);
	str.dat = NULL;
	Lista_C = NULL;
	*sai = NULL;
	
//	// disp_basis_by_ron (base);
//	
//	minimo_lado_esquerdo (&minimo, base); //intervalos da base que tem menor quantidade de 1's
//	
//	// disp_basis_by_ron (&minimo);
//	
//	esse_min_lado_esquerdo_min (&S, &minimo, base);
//	
	// printf ("Menor Elemento Estruturante: N. de Pontos = %d\n", S->size);
	// disp_win_by_ron (S);
	// printf("S %d %d %d\n", S->size, S->hsize, S->wsize);
	
	win_index (S, &winidx); //winidx: valores para i onde encontram-se 1's na janela example: winidx[1] = 4; quer dizer que na posição (0,5) está o segundo 1
	tam_idx = S->size;
	copy_win (S, &C);
	// disp_win_by_ron (&C);
	tam = (S->hsize*S->wsize + NB1) / NB;
	tam_Lista_C = 0;
	if (tam_idx > 9) tam_idx = 9; // não quer dizer que seja de 3x3 porque seja 9
	for (i = tam_idx; i >= 2; i--) {
		printf("Tamanho idx %d\n", tam_idx);
		comb = NULL; num_comb = numero_de_combinacoes (tam_idx, i);
		if (beatriz) printf ("Chegou i = %d / %d, num_comb = %d\n", i, tam_idx, num_comb);
		for (j=0; j<num_comb; j++) {
			_combinacoes_ (tam_idx, i, &comb); // _combinacoes_(5, 3) = 1, 2, 3, --> 1, 2, 4, .... --> 3, 4, 5
			for (k=0; k<tam; k++) C.data[k] = 0;
			C.size = 0;
			for (k=0; k<i; k++) {
				// printf("comb %d = %d\n", k, comb[k]);
				section = winidx[comb[k]] / NB;
				nshifts = winidx[comb[k]] % NB;
				C.data[section] |= mask[nshifts];
				(C.size)++;
			}
			if (!verify_3x3(&C)) continue;
			/* Neste ponto temos um C gerado */
			// printf ("%d/comb (%d, %d) = %d:\n", j+1, tam_idx, i, num_comb);
			disp_win_by_ron (&C);
			/* verifica se C e' invariante de S */
			invariante = invariante_window (S, &C);
			if (invariante) {
				tem = 0;
				for (apt_Lista_C = Lista_C; apt_Lista_C && !tem; apt_Lista_C =
					 apt_Lista_C->prox) {
					if (C.size == apt_Lista_C->win.size) {
						tem = invariante_window (&C, &apt_Lista_C->win);
					}
				}
				if (!tem) {
					apt_Lista_C = (win_struc_list *) 
					xmalloc (sizeof(win_struc_list));
					copy_win (&C, &apt_Lista_C->win);
					apt_Lista_C->prox = Lista_C;
					Lista_C = apt_Lista_C;
					tam_Lista_C++;
				}
			}
		}
		if (comb) free (comb);
	}
	
	if (beatriz) printf ("Temos %d Elementos Estruturantes\n", tam_Lista_C);
	
	while (Lista_C) {
		apt_Lista_C = Lista_C;
		Lista_C = Lista_C->prox;
		if (str.dat) {free (str.dat); str.dat = NULL;}
		window_to_str (&apt_Lista_C->win, &str);
		if (beatriz) {
			printf ("Structuring Element:\n");
			disp_win_by_ron (&apt_Lista_C->win);
			printf ("\n");
		}
		gera_lower_bounds_crescente (&lower, base, &str);
		testa_equacao_dilatacao_crescente (base, &str, lower, &solucao);
		apt_sai = (output_search *) xmalloc (sizeof(output_search));
		// printf("FUA %d %d\n", apt_sai->list_base, apt_sai->list_A_star);
		copy_win (&apt_Lista_C->win, &apt_sai->C);
		apt_sai->list_base = solucao;
		apt_sai->prox = * sai;
		// printf("FUA %d %d\n", apt_sai->list_base, apt_sai->list_A_star);
		* sai = apt_sai;
		free_win_by_ron (&apt_Lista_C->win);
		free (apt_Lista_C);
	}
	
	free_win_by_ron (&C);
	if (winidx) free (winidx);
	if (str.dat) free (str.dat);
	return 1;
}

int str2cfg (int *cfg, int nvars, int * winidx, int m, int n, char * str, int strsize) {
	/*  int  *cfg: cfg[w]  where w=number of variables */
	/*  int  nvars: no. de variaveis */
	/*  int  *winidx:  winidx[w] */
	/*  int  m, n: m and n are the height and the width of the 
	 rectangle corresponding to window domain */
	/*  char *str ;  str[strsize] */
	
    int i ;
    int block, nshifts ;
    int n4bytes, compr;
	
    n4bytes = (nvars+31)/32;
	
    // inicializar cfg
    for (i=0; i<n4bytes; i++) cfg[i] = 0;
	
    for(i=0; i<nvars; i++) {
		nshifts = i%NB;
		block = i/NB;
		if (str[2*winidx[i]] == '1') {
			cfg[block] |= mask[nshifts];
		}
    }
    return 1;
}

int monta_basis_before (trv_struc *basis1, trv_struc *basis2) {
	int       st, i, ind;
	int       * winidx;
	char      strA[3000], strB[3000];
	
	st = win_index (&(basis1->win), &winidx);
	if (st == 0) {
		fprintf (stderr, "monta_basis_before (): win_index() failed\n");
		return 0;
	}
	
	st = copy_basis (basis1, basis2);
	if (st==0) {
		fprintf (stderr,"monta_basis_before (): copy_basis() failed\n");
		return 0;
	}
	
	for(i=0; i<basis1->total; i++) {
		ind = basis1->ind[i];
		cfg2str (&(basis1->A[ind]), basis1->nv, winidx, (basis1->win).hsize,
				 (basis1->win).wsize, strA, 2500);
		cfg2str (&(basis1->B[ind]), basis1->nv, winidx, (basis1->win).hsize,
				 (basis1->win).wsize, strB, 2500);
		str2cfg (&(basis2->A[ind]), basis1->nv, winidx, (basis1->win).hsize,
				 (basis1->win).wsize, strB, 2500);
		str2cfg (&(basis2->B[ind]), basis1->nv, winidx, (basis1->win).hsize,
				 (basis1->win).wsize, strA, 2500);
		
	}
	
}

int chama_negation_basis (trv_struc *basis1, trv_struc *basis2) {
	int st;
	
	st = copy_win (&(basis1->win), &(basis2->win));
	
	if (st == 0) {
		fprintf(stderr, "chama_negation_basis : copy_win() failed\n");
		return 0;
	}
	
	st = negation_basis (basis1, basis2);
	
	if (st == 0){
		fprintf(stderr, "chama_negation_basis : negation_basis() failed\n") ;
		return 0;
	}
	
	return 1;
}

int basis_dual_operator (trv_struc * basis1, trv_struc * basis2) {
	trv_struc basis3;
	
	free_basis_by_ron (basis2);
	zera_basis_by_ron (&basis3);
	monta_basis_before (basis1, &basis3);
	chama_negation_basis (&basis3, basis2);
	
	free_basis_by_ron (&basis3);
	return 1;
}

int base_identidade (trv_struc * base) {
	int identidade;
	int ori_h, ori_w, *winidx, pos;
	int i;
	
	winidx = NULL;
	
	if (base->total != 1 || base->win.size != 1) return 0;
	
	ori_h = (base->win.hsize - 1) / 2;
	ori_w = (base->win.wsize - 1) / 2;
	pos = ori_h * base->win.wsize + ori_w;
	winidx = NULL;
	win_index (&base->win, &winidx);
	
	identidade = (winidx[0] == pos);
	
	if (winidx) free (winidx);
	return identidade;
}

/*=*/ int transpoe_window (win_struc * win) {
	int  nshifts, nshifts1;
	int  mask, mask1;
	int  i, j, section, section1, novo_i, novo_j;
	int  data, i_ori, j_ori;
	win_struc win_aux;
	int X_ker, Y_ker;
	
	if (win == NULL) {
		fprintf (stderr, "transpoe_window () pointer to NULL\n");
		return 0;
	}
	
	copy_win (win, &win_aux);
	
	i_ori = (win->hsize-1)/2;
	j_ori = (win->wsize-1)/2;
	
	for(i=0; i<(win->hsize*win->wsize+NB1)/NB; i++) 
		win_aux.data[i] = 0;
	
	for(i=0; i<win->hsize; i++) {
		for(j=0; j<win->wsize; j++) {
			nshifts = ((i*win->wsize)+j)%NB;
			mask = 1<<nshifts;
			section = ((i*win->wsize)+j)/NB;
			data = win->data[section] ;
			if((data & mask) == mask) {
				X_ker = j - j_ori;
				Y_ker = i_ori - i;
				novo_j = -X_ker + j_ori;
				novo_i =  i_ori + Y_ker;
				nshifts1 = ((novo_i*win->wsize)+novo_j)%NB;
				mask1 = 1<<nshifts1;
				section1 = ((novo_i*win->wsize)+novo_j)/NB;
				win_aux.data[section1] |= mask1;
			}
		}
	}
	free_win_by_ron (win);
	copy_win (&win_aux, win);
	free_win_by_ron (&win_aux);
	return 1;
}


int imprime_output_search (output_search * sai) {
	output_search * apt_sai;
	trv_struc_list * solucao, * apt_sol;
	int lado;
	
	while (sai) {
		apt_sai = sai;
		sai = sai->prox;
		lado = menor_lado_possivel_da_janela (&apt_sai->C);
		transform_to_small_window (&apt_sai->C, lado);
		printf ("Elemento Estruturante de imprime_output_sai:\n");
		disp_win_by_ron (&apt_sai->C);
		solucao = apt_sai->list_base;
		while (solucao) {
			apt_sol = solucao;
			solucao = solucao->prox;
			lado = menor_lado_possivel_da_janela (&apt_sol->base.win);
			transform_to_small_window (&apt_sol->base.win, lado);
			printf ("Pi: total %d\n", apt_sol->base.total, apt_sol->base.size);
			printf ("Pi: nv %d\n", apt_sol->base.nv);
			printf ("Pi: nb %d\n", apt_sol->base.nb);
			disp_basis_by_ron (&apt_sol->base);
		}
	}
	return 1;
}

void disp_A_star(str_A_star * A_star) {
	if (A_star == NULL) return;
	printf("Erosão %d %d %d %d\n", A_star->upper_bound_ero, A_star->f_ero, A_star->g_ero, A_star->h_ero);
	// disp_win_by_ron(&A_star->S_ero);
	
	printf("Dilataçao %d %d %d %d\n", A_star->upper_bound_dil, A_star->f_dil, A_star->g_dil, A_star->h_dil);
	// disp_win_by_ron(&A_star->S_dil);
	
	return;
}

int R_win ( win_struc * win) { //Retangulo mínimo que envolve a Janela
	int  nshifts;
	int  mask;
	int  i, j, section;
	int  data;
	int menorL = 1 << 30, maiorL = -1, menorA = 1 << 30, maiorA = -1, resposta = 0;
	// disp_win_by_ron(win);
	
	if (win == NULL) {
		fprintf (stderr, "R_win () pointer to NULL\n");
		return 0;
	}
	
	for(i=0; i<win->hsize; i++) {
		for(j=0; j<win->wsize; j++) {
			nshifts = ((i*win->wsize)+j)%NB ;
			mask = 1<<nshifts ;
			section = ((i*win->wsize)+j)/NB ;
			data = win->data[section] ;
			if((data & mask) == mask) {
				menorL = j < menorL ? j : menorL;
				maiorL = j > maiorL ? j : maiorL;
				menorA = i < menorA ? i : menorA;
				maiorA = i > maiorA ? i : maiorA;
			}
		}
	}
		
	if ((maiorL - menorL) > (maiorA - menorA))
		resposta = maiorL - menorL + 1;
	else
		resposta = maiorA - menorA + 1;
	 
	// printf("%d %d %d %d\n", menorL, maiorL, menorA, menorA);
	return resposta;
}

int upper_bound(trv_struc * base) {
	int hsizewin = base->win.hsize;
    int wsizewin = base->win.wsize;
	int menorL = 1 << 30, maiorL = -1, menorA = 1 << 30, maiorA = -1, resposta = 0;
	int i, ii, j, hexa;

	/* conseguir y, x das posicoes dos 1's na janela h * w */
	for (ii = 0; ii < hsizewin * wsizewin; ii++) {
		int i = ii / wsizewin;
		int j = ii % wsizewin;
		if (ii % 32 == 0) hexa = base->win.data[ii / 32]; 
        else hexa >>= 1;
        if (hexa & 1 != 0) {
            menorL = j < menorL ? j : menorL;
			maiorL = j > maiorL ? j : maiorL;
			menorA = i < menorA ? i : menorA;
			maiorA = i > maiorA ? i : maiorA;
        }
    }
	int resL = maiorL - menorL + 1;
	int resA = maiorA - menorA + 1;
	if (resL < 3) resL = 3;
	if (resA < 3) resA = 3;
	// printf("Upper Bound %d %d %d\n", base->total, resL, resA);
	// disp_basis_by_ron (base);	
	return base->total * (resL * resA + 1) - 1;
}

int evalua(trv_struc * base, win_struc * C, str_A_star * A_star, int g_father, int erosion) { //if erosion = 1 -> copy dual operator
	trv_struc base1;
	win_struc S;
	
	if (!erosion) {
		minimo_S(base, &S);
		A_star->S_dil = S;
		int lado_maiorR = R_win(&S);
		A_star->h_dil = (lado_maiorR / 2) * 9;
		if (A_star->h_dil < 9) A_star->h_dil = 9;
		lado_maiorR = R_win(C);
		A_star->g_dil = (lado_maiorR / 2) * 9;
		if (A_star->g_dil < 9) A_star->g_dil = 9;
		A_star->g_dil += g_father;
		A_star->f_dil = A_star->h_dil + A_star->g_dil;
		
		A_star->upper_bound_dil = upper_bound(base) + A_star->g_dil;
	
		zera_basis_by_ron (&base1);
		free_basis_by_ron (&base1);
		basis_dual_operator (base, &base1);
	
		minimo_S(&base1, &S);
	
		A_star->S_ero = S;
		lado_maiorR = R_win(&S);
		A_star->h_ero = (lado_maiorR / 2) * 9;
		if (A_star->h_ero < 9) A_star->h_ero = 9;
		lado_maiorR = R_win(C);
		A_star->g_ero = (lado_maiorR / 2) * 9;
		if (A_star->g_ero < 9) A_star->g_ero = 9;
		A_star->g_ero += g_father;
		A_star->f_ero = A_star->h_ero + A_star->g_ero;
		
		A_star->upper_bound_ero = upper_bound(&base1) + A_star->g_ero;
	}
	else {
		minimo_S(base, &S);
		A_star->S_ero = S;
		int lado_maiorR = R_win(&S);
		A_star->h_ero = (lado_maiorR / 2) * 9;
		if (A_star->h_ero < 9) A_star->h_ero = 9;
		lado_maiorR = R_win(C);
		A_star->g_ero = (lado_maiorR / 2) * 9;
		if (A_star->g_ero < 9) A_star->g_ero = 9;
		A_star->g_ero += g_father;
		A_star->f_ero = A_star->h_ero + A_star->g_ero;
		
		A_star->upper_bound_ero = upper_bound(base) + A_star->g_ero;
	
		zera_basis_by_ron (&base1);
		free_basis_by_ron (&base1);
		basis_dual_operator (base, &base1);
	
		minimo_S(&base1, &S);
	
		A_star->S_dil = S;
		lado_maiorR = R_win(&S);
		A_star->h_dil = (lado_maiorR / 2) * 9;
		if (A_star->h_dil < 9) A_star->h_dil = 9;
		lado_maiorR = R_win(C);
		A_star->g_dil = (lado_maiorR / 2) * 9;
		if (A_star->g_dil < 9) A_star->g_dil = 9;
		A_star->g_dil += g_father;
		A_star->f_dil = A_star->h_dil + A_star->g_dil;
		
		A_star->upper_bound_dil = upper_bound(&base1) + A_star->g_dil;
	}
	printf("f1 %d h1 %d g1 %d f2 %d h2 %d g2 %d\n", A_star->f_ero, A_star->h_ero, A_star->g_ero, A_star->f_dil, A_star->h_dil, A_star->g_dil);
	iterative_deepening(A_star);
	return 1;
}


int evalua_A_star (output_search * sai, int g_father, int erosion) { // evalua a saída de search_increasing_all, para cada base corresponde um A*, para dilatação e erosão
	output_search * apt_sai;
	trv_struc_list * solucao, * apt_sol;
	str_A_star_list * A_star_list, * A_star_cur, * primeiro;
	int i, lado, dil, ori, first;
	
	if (sai == NULL) {
		fprintf (stderr, "evalua_A_star () pointers to NULL\n");
		return 0;
	}
	
	while (sai) {
		apt_sai = sai;
		sai = sai->prox;
		solucao = apt_sai->list_base;
		A_star_list = apt_sai->list_A_star;
		first = 1;
		primeiro = A_star_list;
		printf("entro sai %d\n", erosion);
//		disp_win_by_ron(&apt_sai->C);
		while (solucao) {
			printf("entro solucao %d\n", erosion);
			apt_sol = solucao;
			solucao = solucao->prox;
			A_star_list = (str_A_star_list *) xmalloc (sizeof(str_A_star_list));
			if (first) { primeiro = A_star_list; first = 0; }
			A_star_cur = A_star_list;
			A_star_list = A_star_list->prox;
			
			evalua(&apt_sol->base, &apt_sai->C, &A_star_cur->A_star, g_father, erosion);
		}
		apt_sai->list_A_star = primeiro;
	}
	
	return 1;
}

//int imprime_output_search (output_search * sai) {
//	output_search * apt_sai;
//	trv_struc_list * solucao, * apt_sol;
//	int lado;
//	
//	while (sai) {
//		apt_sai = sai;
//		sai = sai->prox;
//		lado = menor_lado_possivel_da_janela (&apt_sai->C);
//		transform_to_small_window (&apt_sai->C, lado);
//		printf ("Elemento Estruturante de imprime_output_sai:\n");
//		disp_win_by_ron (&apt_sai->C);
//		solucao = apt_sai->list_base;
//		while (solucao) {
//			apt_sol = solucao;
//			solucao = solucao->prox;
//			lado = menor_lado_possivel_da_janela (&apt_sol->base.win);
//			transform_to_small_window (&apt_sol->base.win, lado);
//			printf ("Pi: total %d\n", apt_sol->base.total, apt_sol->base.size);
//			printf ("Pi: nv %d\n", apt_sol->base.nv);
//			printf ("Pi: nb %d\n", apt_sol->base.nb);
//			disp_basis_by_ron (&apt_sol->base);
//		}
//	}
//	return 1;
//}

typedef struct elem_pilha {
	int dil;
	win_struc C;
} _elem_pilha_;

_elem_pilha_ pilha[5000];
int topo=0;

// int menor_f = (1 << 30);
// int upper_bound_program = (1 << 30);

// if (A_star_cur->A_star.S_dil.size > 1 || A_star_cur->A_star.S_ero.size > 1) {
	// if (upper_bound_program > menor_f) now!
	// if (A_star_cur->f_dil < upper_bound_program || A_star_cur->f_ero.size < upper_bound_program)

/*=*/ int paralelo_para_sequencial (trv_struc * base, str_A_star * node) {
	output_search * sai1, * apt_sai, * sai2, * sai;
	trv_struc_list * solucao, * apt_sol;
	str_A_star_list * A_star_list, * A_star_cur;
	int i, lado, dil, ori;
	trv_struc base1;
	win_struc win_ori;
	
	// printf("Menores T_T %d %d\n", menor_f, last_f);
	
	printf ("topo = %d\n", topo);
	// disp_win_by_ron(&base->win);
	disp_basis_by_ron (base);
	disp_A_star(node);
	
	if (base_identidade (base)) {
		/* encontrou uma solucao */
		num_sol++;
		printf ("Ini Solucao %d =================================================\n", num_sol);
		for (i=0; i<topo; i++) {
			if (pilha[i].dil == 1) 
				printf ("dil\n");
			else
				printf ("ero\n");
			lado = menor_lado_possivel_da_janela (&pilha[i].C);
			transform_to_small_window (&pilha[i].C, lado);
			disp_win_by_ron (&pilha[i].C);
		}
		printf ("Fim Solucao %d =================================================\n", num_sol);
		exit(1);
		return 1;
	}
	
	zera_win_by_ron (&win_ori);
	zera_basis_by_ron (&base1);

	lado = menor_lado_possivel_da_janela (&base->win);
	transform_to_small_window (&base->win, lado);
	
	if (base->total == 1 && base->win.size == 1) { 
		pilha[topo].dil = 1;
		copy_win (&base->win, &pilha[topo++].C);
		free_basis_by_ron (&base1);
		copy_basis_by_ron (base, &base1);
		free_win_by_ron (&win_ori);
		copy_win (&base->win, &win_ori);
		for (i=0; i<(win_ori.wsize*win_ori.hsize+NB1)/NB; i++) 
			win_ori.data[i] = 0;
		ori = ((win_ori.hsize - 1) / 2) * win_ori.hsize + ((win_ori.wsize - 1)/2);
		win_ori.data[ori/NB] |= mask[ori%NB];
		free_win_by_ron (&base1.win);
		copy_win (&win_ori, &base1.win);
		free_win_by_ron (&win_ori);
		printf ("Saiu base com 1 elemento deslocado\n");
		paralelo_para_sequencial (&base1, NULL);
		free_basis_by_ron (&base1);
		free_win_by_ron (&pilha[topo].C);
		topo--;
		return 1;
	}
	
	printf("Menor f = %d, Last f %d, Upper bound %d\n", menor_f, last_f, upper_bound_program);
	
	if (upper_bound_program < menor_f && menor_f != oo) {
	
		if (upper_bound_program == node->upper_bound_dil && node->S_dil.size > 1) {
			/* encontrou uma solucao */
			num_sol++;
			printf ("Ini Solucao %d =================================================\n", num_sol);
			for (i=0; i<topo; i++) {
				if (pilha[i].dil == 1) 
					printf ("dil\n");
				else
					printf ("ero\n");
				lado = menor_lado_possivel_da_janela (&pilha[i].C);
				transform_to_small_window (&pilha[i].C, lado);
				disp_win_by_ron (&pilha[i].C);
			}
			printf ("Sup de erosões da base\n");
			disp_basis_by_ron (base);
			printf ("Fim Solucao %d =================================================\n", num_sol);
			exit(1);
			return 1;
		}
	
		if (upper_bound_program == node->upper_bound_ero && node->S_ero.size > 1) { // finish!!
			/* encontrou uma solucao */
			num_sol++;
			printf ("Ini Solucao %d =================================================\n", num_sol);
			for (i=0; i<topo; i++) {
				if (pilha[i].dil == 1) 
					printf ("dil\n");
				else
					printf ("ero\n");
				lado = menor_lado_possivel_da_janela (&pilha[i].C);
				transform_to_small_window (&pilha[i].C, lado);
				disp_win_by_ron (&pilha[i].C);
			}
			printf ("Inf de dilatações da base\n");
			free_basis_by_ron (&base1);
			basis_dual_operator (base, &base1);
			printf ("Disp base dual\n");
			disp_basis_by_ron (&base1);
			printf ("Fim Solucao %d =================================================\n", num_sol);
			exit(1);
			return 1;
		}
		
		return 1;
	}
	
	/* pode ser uma dilatacao */
	sai1 = NULL;
	sai2 = NULL;
	
	if (node->S_dil.size > 1 && node->f_dil <= menor_f) {
		printf ("Comeca Search_Increasing_All para dilatacao\n");
		search_increasing_all (base, &node->S_dil, &sai1);
		printf ("Termina Search_Increasing_All para dilatacao\n");
	}
	
	if (sai1 == NULL) {
		printf ("Deu Dilatacao Vazia\n");
	}
	
	if (node->S_ero.size > 1 && node->f_ero <= menor_f) {	
		free_basis_by_ron (&base1);
		basis_dual_operator (base, &base1);
		printf ("Disp base dual\n");
		disp_basis_by_ron (&base1);
		printf ("\n");

		printf ("Comeca Search_Increasing_All para erosao\n");
		search_increasing_all (&base1, &node->S_ero, &sai2);
		printf ("Termina Search_Increasing_All para erosao\n");
	}
	
	if (sai2 == NULL) {
		printf ("Deu Erosao Vazia\n");
	}
	
	if (sai1 == NULL && sai2 == NULL) {
		printf ("Sem saida: chegou a uma folha\n");
		return 1;
	}
	
	printf ("-----------------------------------\n");
	printf ("Saida da Dilatacao:\n");
	imprime_output_search (sai1); 
	printf ("Saida da Erosao\n");
	imprime_output_search (sai2);
	printf ("-----------------------------------\n");
	
	// printf ("-----------------------------------\n");
	// printf ("Evalua parámetros de A*:\n");
	// printf("%d %d\n", node->g_dil, node->g_ero);
	if (sai1 != NULL) {
		evalua_A_star (sai1, node->g_dil, 0); 
	}
	if (sai2 != NULL) {
		evalua_A_star (sai2, node->g_ero, 1);
	}
	// printf ("-----------------------------------\n");
	
	for (i=0; i<2; i++) {
		if (i==0) {sai = sai1; dil = 1;}
		if (i==1) {sai = sai2; dil = 0;}
		printf ("Entro %d\n", i);
		while (sai) {
			printf ("sai\n");
			apt_sai = sai;
			sai = sai->prox;
			solucao = apt_sai->list_base;
			A_star_list = apt_sai->list_A_star;
			while (solucao) {
				printf ("solucao\n");
				apt_sol = solucao;
				solucao = solucao->prox;
				
				free_basis_by_ron (&base1);
				if (dil) {
					printf ("Dilatacao\n");
					copy_basis_by_ron (&apt_sol->base, &base1);
				}
				else {
					printf ("Erosao\n");
					basis_dual_operator (&apt_sol->base, &base1);
				}
				
				disp_basis_by_ron(&base1);
				
				A_star_cur = A_star_list;
				A_star_list = A_star_list->prox;
			
				lado = menor_lado_possivel_da_janela (&apt_sai->C);
				transform_to_small_window (&apt_sai->C, lado);
				copy_win (&apt_sai->C, &pilha[topo].C);
				if (dil == 0) transpoe_window (&pilha[topo].C);
				pilha[topo++].dil = dil;
				printf ("Empilhou: topo = %d\n", topo);
				printf ("Elem. Estruturante:\n");
				disp_win_by_ron (&pilha[topo-1].C);
				printf ("\n");
			
				paralelo_para_sequencial (&base1, &A_star_cur->A_star);
			
				free_win_by_ron (&pilha[topo].C);
				topo--;
			
				printf ("*********************************************\n");
				printf ("* Desempilhou: topo = %02d                    *\n", topo);
				printf ("*********************************************\n");
			
				free_basis_by_ron (&apt_sol->base);
				free_basis_by_ron (&base1);
			}
			free_win_by_ron (&apt_sai->C);
		}
	}
	
	free_win_by_ron (&win_ori);
	printf ("Saiu do final\n");
	return 1;
}