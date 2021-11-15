#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <time.h>
#include <math.h>
#include <stdbool.h>
#include <string.h>
#include <limits.h>
#define TAM_MAX 1000

/* Sergio Piñeiro Bermúez: sergio.pineiro
   Ismael Verde Costas: ismael.verdec */

typedef int ** matriz;


double microsegundos() {
   struct timeval t;
   if (gettimeofday(&t, NULL) < 0)
      return 0.0;
   return (t.tv_usec + t.tv_sec * 1000000.0);
}

void listar_matriz (matriz m, int n){
   int i,j;
   
   for(j=0;j<n;j++) {   
      printf("[");
      for (i=0; i<n; i++){
         printf ( "%4d", m[j][i]);
      }
      printf(" ]\n");
   }
}

matriz crearMatriz(int n) {
   int i;
   matriz aux;
   if ((aux = malloc(n*sizeof(int *))) == NULL)
      return NULL;
   for (i=0; i<n; i++)
      if ((aux[i] = malloc(n*sizeof(int))) == NULL)
         return NULL;
   return aux;
}

/* Inicializacion pseudoaleatoria [1..TAM_MAX] de un grafo completo
no dirigido con n nodos, representado por su matriz de adayencia */
void iniMatriz(matriz m, int n) {
   int i, j;
   for (i=0; i<n; i++)
      for (j=i+1; j<n; j++)
         m[i][j] = rand() % TAM_MAX + 1;
   for (i=0; i<n; i++)
      for (j=0; j<=i; j++)
         if (i==j)
            m[i][j] = 0;
         else
            m[i][j] = m[j][i];
}

void liberarMatriz(matriz m, int n) {
   int i;
   for (i=0; i<n; i++)
      free(m[i]);
   free(m);
}


void dijkstra(matriz grafo, matriz distancias, int tam) {
  int n, i, j,min, v=0;
  int *noVisitados = malloc(tam*sizeof(int));
  for (n=0; n<tam; n++) {
    for (i=0; i<tam; i++) {
      noVisitados[i] = 1;
      distancias[n][i] = grafo[n][i];
    }
    noVisitados[n] = 0;
   
    j=0;
    while(j<tam-2){
       min= INT_MAX;
       for(i=0; i<tam; i++)
           if(noVisitados[i]==1)
              if(distancias[n][i]<min ){
                min = distancias[n][i];
                v = i;
              }
      
       noVisitados[v]=0;
       for(i=0; i<tam; i++)
           if(noVisitados[i]==1)
              if(distancias[n][i] > distancias[n][v] + grafo[v][i])
                 distancias[n][i] = distancias[n][v] + grafo[v][i];
       
       j++;
    }
  }
  free(noVisitados);
}

void test(){
   int n=4,n2=5;
   matriz distancias = crearMatriz(n);
   matriz distancias2 = crearMatriz(n2);
   matriz m = crearMatriz(n);
   m[0][0]= 0; m[0][1]= 1; m[0][2]= 4; m[0][3]= 7;
   m[1][0]= 1; m[1][1]= 0; m[1][2]= 2; m[1][3]= 8;
   m[2][0]= 4; m[2][1]= 2; m[2][2]= 0; m[2][3]= 3;
   m[3][0]= 7; m[3][1]= 8; m[3][2]= 3; m[3][3]= 0;
   matriz m2 = crearMatriz(n2);
   m2[0][0]= 0; m2[0][1]= 1; m2[0][2]= 8; m2[0][3]= 4; m2[0][4]= 7;
   m2[1][0]= 1; m2[1][1]= 0; m2[1][2]= 2; m2[1][3]= 6; m2[1][4]= 5;
   m2[2][0]= 8; m2[2][1]= 2; m2[2][2]= 0; m2[2][3]= 9; m2[2][4]= 5;
   m2[3][0]= 4; m2[3][1]= 6; m2[3][2]= 9; m2[3][3]= 0; m2[3][4]= 3;
   m2[4][0]= 7; m2[4][1]= 5; m2[4][2]= 5; m2[4][3]= 3; m2[4][4]= 0;
   
   printf("Matriz de adyacencia: GRAFO 1\n");
   listar_matriz(m,n);
   printf("\nInicialización matriz distancias:\n");
   listar_matriz(distancias,n);
   dijkstra(m, distancias, n);
   printf("\nMatriz de distancias mínimas:\n");
   listar_matriz(distancias,n);
   liberarMatriz(m, n);
   liberarMatriz(distancias, n);

   printf("\nMatriz de adyacencia: GRAFO 2\n");
   listar_matriz(m2,n2);
   printf("\nInicialización matriz distancias:\n");
   listar_matriz(distancias2,n2);
   dijkstra(m2, distancias2, n2);
   printf("\nMatriz de distancias mínimas:\n");
   listar_matriz(distancias2,n2);
   liberarMatriz(m2, n2); 
   liberarMatriz(distancias2, n2);
}

void cabecera (){
    printf("%6s %14s %16s %17s %17s\n", "n", "t(n)",
      "t(n)/n^2.6", "t(n)/n^2.84", "t(n)/n^3.1"); 
}


void cotas (int n, double tTotal) {
    double sub, ajust, sobre;

    sub = tTotal/pow((double)n, 2.6);
    ajust = tTotal/pow((double)n, 2.84);
    sobre= tTotal/pow((double)n, 3.1);
    printf ("%16.6f %16.6f %16.6f\n", sub, ajust, sobre);     
}

void tiempo_Dijkstra (){
    int n = 10; int k = 10000; int i;
    matriz m,d;
    double tInicial=0, tFinal=0, tTotal=0;
                                              
    printf("\nInicializiación aleatoria\n\n");
    cabecera();
    for (n=n; n<=TAM_MAX; n=n*2){
        printf("%7d", n);
        m=crearMatriz(n);
        d=crearMatriz(n);
        iniMatriz(m,n);      
        tInicial=microsegundos();
        dijkstra(m,d,n);
        tFinal=microsegundos();
        tTotal=tFinal - tInicial;
          
        if (tTotal < 500){
              tInicial=microsegundos();
              for(i=0; i<k; i++){
                    iniMatriz(m,n);
                    dijkstra(m,d,n); }
              tFinal = microsegundos();
              tTotal = tFinal - tInicial;
              tInicial = microsegundos();
              for(i=0; i<k; i++){ iniMatriz(m,n);}
              tFinal=microsegundos();
              tTotal= (tTotal-(tFinal-tInicial))/k;        
              printf("%14.3f*", tTotal);
         }
         else { printf("%14.3f ", tTotal);}
         cotas(n, tTotal);
      
        liberarMatriz(m,n);
        liberarMatriz(d,n);
    }
}

int main (){

  printf("Test\n");
  test();

  printf("\n\nMedición de distancias mínimas por DIJKSTRA :\n\n");
  tiempo_Dijkstra();


}
                                                                                                           
