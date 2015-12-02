#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>

int distancia(int xi, int yi, int xj, int yj){
	int distemp, distancia;
	distemp = ((xi-xj)^2) + ((yi + yj)^2);
	distancia = sqrt(distemp);
	return distancia;
}

int main()
{
	int a[] = {1,2,3,4};
	int x=0, y=0;

	for(x=0;x<4;x++){
		for(y=0;y<4;y++){
			if(y > x){
				printf("%d %d\n", a[x], a[y]);
			}
		}
	}
	return 0;
}