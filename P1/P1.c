#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <time.h>
#include <math.h>


int sumaSubMax1 ( int v[], int n) {
    
    int sumaMax=0;
    int i,j;
    int estaSuma;

    for (i=0; i<n; i++) {
       estaSuma=0;
       for (j=i; j<n; j++) {
           estaSuma+=v[j];
           if (estaSuma > sumaMax) {
              sumaMax=estaSuma;
           }
       }
     }
     return sumaMax;
}

int sumaSubMax2 (int v[], int n) {

     int estaSuma=0, sumaMax=0;
     int i;
   
     for (i=0; i<n; i++) {
         estaSuma+=v[i];
          if (estaSuma>sumaMax){
             sumaMax=estaSuma;
          }
          else if (estaSuma<0){
                   estaSuma=0; 
                }
                
     }
     return sumaMax;
}



void inicializar_semilla () {
   srand(time(NULL));
}

void aleatorio (int v [], int n) {
     int i, m=2*n+1;
      
     for (i=0; i<n; i++)
         v[i] = (rand() %m) -n;

}

void listar_vector (int v[], int n) {
      int i;
      
      printf("[");
      for (i=0; i<n; i++)
          printf("%2d ", v[i]);
   
          
      printf("]");
}

double microsegundos() {
   struct timeval t;
   if (gettimeofday(&t, NULL) < 0)
      return 0.0;
   return (t.tv_usec + t.tv_sec * 1000000.0);
}

void cabecera1 (){
    printf("%6s %12s %18s %15s %17s\n", "n", "t(n)", 
			"t(n)/n^1.8", "t(n)/n^2", "t(n)/n^2.2"); 
}

void cabecera2 (){
    printf("%6s %12s %18s %15s %17s\n", "n", "t(n)",
			"t(n)/n^0.8", "t(n)/n", "t(n)/n^1.2"); 
}

void cotas (int n, double tTotal, double cti, double cta, double cts) {
    double sub, sobre, ajust;
    
    sub = tTotal/pow((double)n, cti);
    ajust = tTotal/pow((double)n, cta);
    sobre= tTotal/pow(n, cts);
    printf ("%16.6f %16.6f %16.6f\n", sub, ajust, sobre);     

}


void test1() {
    int a,b,i;
    int v[6][5] = {
		{-9, 2, -5, -4, 6},
		{4, 0, 9, 2, 5},
		{-2, -1, -9, -7, -1},
		{9, -2, 1, -7, -8},
		{15, -2, -5, -4, 16},
		{7, -5, 6, 7, -7}}; 
    printf("Test 1:\n");
    printf("%20s%15s%15s\n", "", "sumaSubMax1", "sumaSubMax2");
    for (i=0;i<6;i++) {
        listar_vector(v[i], 5);

    a = sumaSubMax1(v[i],5);
    b = sumaSubMax2(v[i],5);
    printf("%15d%15d\n", a,b);
    }
}


void test2 () {
     int i; 
     int v[9];    


     printf("\nTest 2:\n\n "); 
     printf(" %6s ", "Vectores pseudoaleatorios ");
     printf (" %12s %14s\n", "sumaSubMax1", "sumaSubMax2");
     for (i=0; i<10; i++){
         aleatorio(v,9);
         listar_vector(v,9);
         printf(" %8d  %13d \n", sumaSubMax1(v,9), sumaSubMax2(v,9));
        }

}

void tiempo1 (){
    int n = 500;
    int v [32000];
    int k = 300;
    int i;
    double subestimada=1.8, ajustada = 2, sobrestimada=2.2;
    double tInicial, tFinal, tTotal;
   
    printf ("Medición del algoritmo sumaSubMax1:\n\n");
    cabecera1();
    for (n=n; n<=32000; n=n*2){
        printf("%7d", n);
        aleatorio(v, n);        
        tInicial=microsegundos();
        sumaSubMax1(v,n);
        tFinal=microsegundos();
        tTotal=tFinal - tInicial;
        
         if (tTotal < 500){
              tInicial=microsegundos();
              for(i=0; i<k; i++){
                    aleatorio(v, n);
                    sumaSubMax1(v, n); }
              tFinal = microsegundos();
              tTotal = tFinal - tInicial;
              tInicial = microsegundos();
              for(i=0; i<k; i++){ aleatorio(v, n);}
              tFinal=microsegundos();
              tTotal= (tTotal-(tFinal-tInicial))/k;        
              printf("%14.3f*", tTotal);
         }
         else { printf("%14.3f", tTotal);}
         cotas(n, tTotal, subestimada, ajustada, sobrestimada);
     }
  
}


void tiempo2 (){
    int n = 500;
    int v [32000];
    int k = 300;
    int i;
    double subestimada=0.8, ajustada = 1, sobrestimada=1.2;
    double tInicial, tFinal, tTotal;
   
    printf ("Medición del algoritmo sumaSubMax2:\n\n");
    cabecera2();
    for (n=n; n<=32000; n=n*2){
        printf("%7d", n);
        aleatorio(v, n);        
        tInicial=microsegundos();
        sumaSubMax2(v,n);
        tFinal=microsegundos();
        tTotal=tFinal - tInicial;
        
         if (tTotal < 500){
              tInicial=microsegundos();
              for(i=0; i<k; i++){
                    aleatorio(v, n);
                    sumaSubMax2(v, n); }
              tFinal = microsegundos();
              tTotal = tFinal - tInicial;
              tInicial = microsegundos();
              for(i=0; i<k; i++){ aleatorio(v, n);}
              tFinal=microsegundos();
              tTotal= (tTotal-(tFinal-tInicial))/k;        
              printf("%14.3f*", tTotal);
         }
         else { printf("%14.3f", tTotal);}
         cotas(n, tTotal, subestimada, ajustada, sobrestimada);
   
     }
    
}


int main () {
   inicializar_semilla();
   test1();
   test2();
   printf("\n");
   tiempo1();
   printf("\n\n");
   tiempo2();
   return 0;
}

