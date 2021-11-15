#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <time.h>
#include <math.h>
#include <stdbool.h>
#include <string.h>
#define MAX 1024000

/* Sergio Piñeiro Bermúez: sergio.pineiro
   Ismael Verde Costas: isma.verdec */

struct vCotas{
     double sub;
     double aj;
     double sobre;
   };
   
typedef struct {
      int vector[MAX];
      int ultimo;
} monticulo;


void inicializar_semilla() {
       srand(time(NULL));
}

double microsegundos() {
   struct timeval t;
   if (gettimeofday(&t, NULL) < 0)
      return 0.0;
   return (t.tv_usec + t.tv_sec * 1000000.0);
}

void aleatorio(int v [], int n){
    int i, m=2*n+1;
    
     for (i=0; i < n; i++)
        v[i] = (rand() % m) - n;
}

void ascendente(int v [], int n) {
   int i;

   for (i=0; i < n; i++)
         v[i] = i;
}

void descendente(int v [], int n) {
   int i;

   for (i=0; i < n; i++)
      v[i] = n-i;
}


bool ordenado (int v [], int n){
   int i;
   bool result = true;

   for (i=0; i<n-1 && result; i++){
      if (v[i]>v[i+1])
         result=false;
   }
   return result;

}

void listar_vector (int v [], int n){
   int i;

   printf("[");
   for (i=0; i<n; i++){
       printf ( "%3d", v[i]);
   }
   printf("]\n" );

}

int consultarMenor(const monticulo *M){
       return M->vector[0];
}

void hundir(monticulo *M, int i) {
    int hIzq,hDer,j=-1,aux=0;
    do {
        hIzq=2*(i+1)-1;
        hDer=2*(i+1);
        j=i;
        if (hDer<=M->ultimo && M->vector[hDer]< M->vector[i])
            i=hDer;
        if (hIzq<=M->ultimo && M->vector[hIzq]< M->vector[i])
            i=hIzq;
        aux=M->vector[j];
        M->vector[j]=M->vector[i];
        M->vector[i]=aux;          
    }
    while (j!=i);
}

void crearMonticulo(int v[],int n,monticulo *M){
        int i;
        for (i=0;i<n;i++)
           M->vector[i]=v[i];	
        M->ultimo=n-1;
        for(i=(M->ultimo / 2);i>=0;i--)
            hundir(M,i);
}
 
void quitarMenor(monticulo *M){
        M->vector[0] = M ->vector[M->ultimo];
        M->ultimo=M->ultimo-1;
        if (M->ultimo > -1)
          hundir(M,0);
}
void ordMonticulo(int v[], int n){
        int i;
        monticulo M;
        crearMonticulo(v,n,&M);
        for (i=0;i<n;i++) {
            v[i] = consultarMenor(&M);
            quitarMenor(&M);
        }
}

void test(){
   int n=12, v[n];
   monticulo M;
   printf ("Inicialización aleatoria:\n");
   aleatorio(v, n);
   listar_vector(v, n);
   printf("Ordenado? %d\n",ordenado(v, n));
   ordMonticulo(v, n);
   listar_vector(v,n);
   printf("Ordenado? %d\n",ordenado(v, n));
   printf("Inicialización ascendente: \n");
   ascendente(v, n);
   listar_vector(v,n);
   printf("Ordenado? %d\n",ordenado(v, n));
   ordMonticulo (v, n);
   listar_vector(v,n);
   printf("Ordenado? %d\n",ordenado(v, n));
   printf("Inicialización descendente: \n");
   descendente(v, n);
   listar_vector(v, n);
   printf("Ordenado? %d\n",ordenado(v, n));
   ordMonticulo (v, n);
   listar_vector(v,n);
   printf("Ordenado? %d\n",ordenado(v, n));
   printf("CrearMonticulo:\n");
   aleatorio(v,n);
   printf("Vector: "); listar_vector(v, n);
   printf("Montículo: ");crearMonticulo(v,n,&M);
   listar_vector(M.vector,M.ultimo+1);
   printf("ConsultarMenor y comprobar el último\n");
   printf("%d %s",consultarMenor(&M)," es el menor\n");
   printf("%d %s",M.vector[M.ultimo]," es el último\n");
   printf("QuitarMenor\n");
   quitarMenor(&M);
   listar_vector(M.vector,M.ultimo+1);
}

void cabecera1(struct vCotas vC){
    printf("%6s %12s %14s%2.2f %12s%2.3f %14s%2.2f\n", "n", "t(n)",
      "t(n)/n^", vC.sub, "t(n)/n^", vC.aj, "t(n)/n^", vC.sobre); 
}

void cabecera2(struct vCotas vC){
    printf("%6s %12s %16s%1.1f %18s %12s%1.1f\n", "n", "t(n)",
      "t(n)/n^", vC.sub, "t(n)/n*log(n)", "t(n)/n^", vC.sobre); 
}

void cabecera_crear(struct vCotas vC){
    printf("%6s %12s %15s%1.2f %13s%1.2f %13s%1.2f\n", "n", "t(n)", 
           "t(n)/n^",vC.sub, "t(n)/n^",vC.aj, "t(n)/n^",vC.sobre); 
}

void cotas1 (int n, double tTotal, struct vCotas vC) {
    double sub, ajust, sobre;

    sub = tTotal/pow((double)n, vC.sub);
    ajust = tTotal/pow((double)n, vC.aj);
    sobre= tTotal/pow(n, vC.sobre);
    printf ("%16.6f %16.6f %16.6f\n", sub, ajust, sobre);     
}

void cotas2 (int n, double tTotal, struct vCotas vC) {
    double sub, ajust, sobre;
    
    sub = tTotal/pow((double)n, vC.sub);
    ajust = tTotal/(n*log(n));
    sobre= tTotal/pow(n, vC.sobre);
    printf ("%16.6f %16.6f %16.6f\n", sub, ajust, sobre);
}

void cotas_crear (int n, double tTotal, struct vCotas vC) {
    double sub, sobre, ajust;
    
    sub = tTotal/pow(n, vC.sub);
    ajust = tTotal/pow(n, vC.aj);
    sobre= tTotal/pow(n, vC.sobre);
    printf ("%16.6f %16.6f %16.6f\n", sub, ajust, sobre);     
}    


void tiempo_Ord (void (*cotas) (int n, double tTotal, struct vCotas vC),
              void (*cabecera) (struct vCotas vC),
              struct vCotas vC,
              char * in,
              void  (*ini) (int v [], int n)){
    int n = 500; int v [MAX]; int k = 1000;
    int i; 
    double tInicial=0, tFinal=0, tTotal=0;


    printf("\nInicializiación %s\n\n", in );
    cabecera(vC);
    for (n=n; n<=MAX; n=n*2){
      if (tTotal<1000000) {
        printf("%7d", n);
        ini(v, n);        
        tInicial=microsegundos();
        ordMonticulo(v,n);
        tFinal=microsegundos();
        tTotal=tFinal - tInicial;
        
         if (tTotal < 500){
              tInicial=microsegundos();
              for(i=0; i<k; i++){
                    ini(v, n);
                    ordMonticulo(v, n); 
              }
              tFinal = microsegundos();
              tTotal = tFinal - tInicial;
              tInicial = microsegundos();
              for(i=0; i<k; i++){ ini(v, n);}
              tFinal=microsegundos();
              tTotal= (tTotal-(tFinal-tInicial))/k;        
              printf("%14.3f*", tTotal);
         }
         else { printf("%14.3f", tTotal);}
         cotas(n, tTotal,  vC);
     }
  }
}

void tiempo_Crear(struct vCotas vC){
    int i, k=1000, n = 500, v[MAX];
    double tInicial=0, tFinal=0, tTotal=0;
    monticulo M;
    printf ("Creación de montículo\n\n");
    cabecera_crear(vC);
    for (n=n; n<=MAX; n=n*2){
      if (tTotal<1000000){
        printf("%7d", n);
        aleatorio(v, n);        
        tInicial=microsegundos();
        crearMonticulo(v,n,&M);
        tFinal=microsegundos();
        tTotal=tFinal - tInicial;
        if (tTotal < 500){
              tInicial=microsegundos();
              for(i=0; i<k; i++){
                 aleatorio(v, n);
                 crearMonticulo(v,n,&M);
              }
              tFinal = microsegundos();
              tTotal = tFinal - tInicial; 
              tInicial = microsegundos();
              for(i=0; i<k; i++) { aleatorio(v, n);}
              tFinal=microsegundos();
              tTotal= (tTotal-(tFinal-tInicial))/k;        
              printf("%14.3f*", tTotal);
         }
         else printf("%14.3f", tTotal);
         cotas_crear(n, tTotal, vC);
      }
    }    
}


int main (){
  char in [20];
  struct vCotas vC;
  
  inicializar_semilla();
  printf("Test\n");
  test();
  
  printf("\nMedición de creación de montículo:\n");
  vC.sub=0.8; vC.aj=1; vC.sobre=1.2;
  tiempo_Crear(vC);
  printf("\nCrear un montículo a partir de un vector cualquiera");
  printf(" se ejecuta en un tiempo O(n)\n");  
  
  printf("\nMedición de ordenacion por montículo:\n");
  vC.sub=1.0; vC.sobre=1.45; 
  strcpy(in, "aleatoria");
  tiempo_Ord(cotas2, cabecera2, vC, in, aleatorio); 
  vC.sub=1; vC.sobre=1.45; 
  strcpy(in, "ascendente");
  tiempo_Ord(cotas2, cabecera2, vC, in, ascendente);
  vC.sub=1; vC.sobre=1.45; 
  strcpy(in, "descendente");
  tiempo_Ord(cotas2, cabecera2, vC, in, descendente);
}
                                                                                                           
