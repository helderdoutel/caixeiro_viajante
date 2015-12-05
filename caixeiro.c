#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define MAX 300

void escrevearquivo(FILE *f, char *nome, int dim, float tempo, int *vtc){
	int x=0;
	fprintf(f, "NAME: %s\n", nome);
	fprintf(f, "COMMENT: solucao obtida para %s\n", nome);
	fprintf(f, "TYPE: TSP\n");
	fprintf(f, "NAME: %d\n", dim);
	fprintf(f, "TOTAL_WEIGHT: %f\n", tempo);
	fprintf(f, "TOUR_SECTION\n");
	for(;x<dim;x++){
		fprintf(f, "%d\n", vtc[x]);
	}
	fprintf(f, "EOF\n");
}

float menordistancia(FILE *f, int dim, float adj[][dim], int *menor){
	char ch[MAX] = "INICIO";
	FILE *ff = fopen("teste", "w+");
	int x, j, i;
	int ciclo[dim];
	float dist, menordist = -1.0;
	int conta = 0;
	do{
		dist = 0;
		fgets(ch, MAX, f);
		for(x=0;x<dim;x++){
			fscanf(f, "%d", &ciclo[x]);
			printf("%d", ciclo[x]);
			fprintf(ff, "%d ", ciclo[x]);
			conta++;
		}
		printf("\n", ciclo[x]);
		for(x=0;x<dim;x++){
			if(x<dim-1){
				printf("%f + %f\n", dist, adj[(ciclo[x]-1)][(ciclo[x+1]-1)]);
				dist = dist + adj[(ciclo[x]-1)][(ciclo[x+1]-1)];
			}
			else{
				printf("%f + %f\n", dist, adj[(ciclo[x]-1)][(ciclo[0]-1)]);
				printf("x %d y %d \n", ciclo[x]-1, ciclo[0]-1);
				dist = dist + adj[(ciclo[x]-1)][(ciclo[0]-1)];
			}
		}
		fprintf(ff, "%f\n", dist);
		if(menordist == -1.0){
			menordist = dist;
			for(x=0;x<dim;x++){
				menor[x] = ciclo[x];
			}
		}
		if(menordist > dist){
			menordist = dist;
			for(x=0;x<dim;x++){
				menor[x] = ciclo[x];
			}
		}
	}while(strcmp(ch, "EOF"));

	printf("%d menor distancia = %f\n", conta, menordist);

	for(x=0;x<dim;x++){
		printf("%d ", menor[x]);
	}
	
   	printf("\n");
   	return dist;
}
 
void swap(int *x, int *y)
{
    int temp;
    temp = *x;
    *x = *y;
    *y = temp;
}
 
void permute(int *a, int l, int r, FILE *f)
{
   int i, x=0;
   if (l == r){
   		for(x=0;x<=r;x++){
   			printf("%d ", a[x]);
   			fprintf(f, "%d ", a[x]);
   		}
   		printf("\n");
   		fprintf(f, "\n");
   }	
   else
   {
       for (i = l; i <= r; i++)
       {
          swap((a+l), (a+i));
          permute(a, l+1, r, f);
          swap((a+l), (a+i));
       }
   }
}

float distancia2d(int xi, int yi, int xj, int yj){
	float distemp, distancia;
	distemp = ((xi-xj)*(xi-xj)) + ((yi - yj)*(yi - yj));
	distancia = sqrt(distemp);
	return distancia;
}

float distancia3d(int xi, int yi, int zi, int xj, int yj, int zj){
	float distemp, distancia;
	distemp = ((xi-xj)*(xi-xj)) + ((yi - yj)*(yi - yj)) + ((zi - zj)*(zi - zj));
	distancia = sqrt(distemp);
	return distancia;
}

int main(int argc, char* argv[]){
	int i = 0, dim, j, k, x, y, z, id;
	char ch[MAX], aux[10], nome[MAX];
	clock_t c1, c2;
	double tempo;
	float md;

	FILE *f = fopen(argv[2], "r");
	FILE *cp = fopen("cache", "w+");
	FILE *n = fopen(argv[3], "w+");

	c1 = clock();

	for(i=0;strcmp(ch, "EOF") != 0;i++){	
		if(i == 3){
			fscanf(f, "%s", aux); // Consome "DIMENSION:"
			fscanf(f, "%d", &dim); // Pega o número de cidades
			printf("%s %d\n",aux, dim);
		}
		if(i > 3){
			fgets(ch, MAX, f); // consome \n
			fgets(ch, MAX, f); // consome tipo arquivo
			if(ch[22] == '2'){ // Verifica se existe EUC_2D
				fgets(ch, MAX, f); // Pula linha
				int vtc[dim][3];
				float adj[dim][dim];
				int mp[dim];
				for(j=0;j<dim;j++){
					fscanf(f, "%d %d %d", &id, &x, &y);
					mp[j] = id;
					vtc[j][0] = id;
					vtc[j][1] = x;
					vtc[j][2] = y;
				}
				for(i=0;i<dim;i++){
					for(j=0;j<dim;j++){
						adj[i][j] = distancia2d(vtc[i][1], vtc[i][2], vtc[j][1], vtc[j][2]);
					}
				}
				for(j=0;j<dim;j++){
					printf("id=%d\n", vtc[j][0]);
					printf("x=%d ", vtc[j][1]);
					printf("y=%d\n", vtc[j][2]);
				}
				for(i=0;i<dim;i++){
					for(j=0;j<dim;j++){
						printf("%f ", adj[i][j]);
					}
					printf("\n");
				}
				permute(mp, 0, dim-1, cp);
				fprintf(cp, "EOF");
				fclose(cp);
				FILE *dists = fopen("cache", "r");
				int menor[dim];
				md = menordistancia(dists, dim, adj, menor);
				c2 = clock();
				tempo = (double)(c2 - c1)*10/ CLOCKS_PER_SEC; //ms
				printf("tempo = %f\n", tempo);
				escrevearquivo(n, nome, dim, tempo, menor);
				fclose(n);	
			}

			if(ch[22] == '3'){ // Verifica se existe EUC_3D
				fgets(ch, MAX, f); // Pula linha
				int vtc[dim][4];
				float adj[dim][dim];
				int mp[dim];
				for(j=0;j<dim;j++){
					fscanf(f, "%d %d %d %d", &id, &x, &y, &z);
					mp[j] = id;
					vtc[j][0] = id;
					vtc[j][1] = x;
					vtc[j][2] = y;
					vtc[j][3] = z;
				}
				for(i=0;i<dim;i++){
					for(j=0;j<dim;j++){
						adj[i][j] = distancia3d(vtc[i][1], vtc[i][2], vtc[i][3], vtc[j][1], vtc[j][2], vtc[j][3]);
					}
				}
				for(j=0;j<dim;j++){
					printf("id=%d\n", vtc[j][0]);
					printf("x=%d ", vtc[j][1]);
					printf("y=%d\n", vtc[j][2]);
					printf("z=%d\n", vtc[j][3]);
				}
				for(i=0;i<dim;i++){
					for(j=0;j<dim;j++){
						printf("%f ", adj[i][j]);
					}
					printf("\n");
				}
				permute(mp, 0, dim-1, cp);
				fprintf(cp, "EOF");
				fclose(cp);
				FILE *dists = fopen("cache", "r");
				int menor[dim];
				md = menordistancia(dists, dim, adj, menor);
				c2 = clock();
				tempo = (double)(c2 - c1)*10/ CLOCKS_PER_SEC; //ms
				printf("tempo = %f\n", tempo);
				escrevearquivo(n, nome, dim, tempo, menor);
				fclose(n);	

			}

			if(strcmp(ch, "EOF")){ // Verifica se existe EXPLICIT
				printf("acabou\n", dim);
				fclose(f);
				break;
			}
		}
		if(i < 3){
			if(i==0){
				fscanf(f, "%s", ch);
				fscanf(f, "%s", nome);
				fgets(ch, MAX, f); // Armazena cada linha em c1
				printf("%s\n", nome);
			}
			else{
				fgets(ch, MAX, f); // Armazena cada linha em c1
				printf("%s", ch);
			}
		}
	}

	return 0;
}