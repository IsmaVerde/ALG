#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <time.h>
#include <math.h>
#include <stdbool.h>
#include <string.h>


/* Sergio Piñeiro Bermúez: sergio.pineiro
   Ismael Verde Costas: isma.verdec */

struct vCotas{
     double sub;
     double aj;
     double sobre;
   };



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



void ord_ins (int v [], int n){
   int i, x, j;

   for (i=1; i<n; i++){
       x=v[i];
       j=i-1;
       while (j>=0 && v[j]>x){
       	   v[j+1]= v[j];
       	   j=j-1;
       }      
       v[j+1]=x;
   }
}

void ord_shell (int v [], int n){
  int incremento = n, i, tmp, j;
  bool seguir = false;
  do {
      incremento = incremento / 2;
      for (i=incremento; i<n; i++){
          tmp=v[i];
          j = i;
          seguir= true;
          while (j-incremento >= 0 && seguir){
                if (tmp< v[j-incremento]){
                   v[j]=v[j-incremento];
                   j= j-incremento;
                }
                else seguir = false;                
          }
          v[j]= tmp;
      }
  }
  while (incremento > 0);

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

void test(char *sa, void  (*alg) (int v [], int n)){
   int n = 14;
   int vector [n];

   printf ("Inicialización aleatoria:\n");
   aleatorio(vector, n);
   listar_vector(vector, n);
   printf("Ordenado ? %d\n", ordenado(vector, n));
   alg(vector, n);
   printf("Ordenación %s: \n", sa);
   listar_vector(vector,n);
   printf("Ordenado ? %d\n", ordenado(vector,n));
   printf("Inicialización ascendente: \n");
   ascendente(vector, n);
   listar_vector(vector,n);
   printf("Ordenado ? %d\n", ordenado(vector, n));
   alg (vector, n);
   printf("Ordenación %s: \n", sa);
   listar_vector(vector,n);
   printf("Ordenado ? %d\n", ordenado(vector,n));
   printf("Inicialización descendente: \n");
   descendente(vector, n);
   listar_vector(vector, n);
   printf("Ordenado ? %d\n", ordenado(vector, n));
   alg (vector, n);
   printf("Ordenación %s: \n", sa);
   listar_vector(vector,n);
   printf("Ordenado ? %d\n", ordenado(vector,n));

}

void cabecera1(struct vCotas vC){
    printf("%6s %12s %14s%2.2f %12s%2.3f %14s%2.2f\n", "n", "t(n)",
      "t(n)/n^", vC.sub, "t(n)/n^", vC.aj, "t(n)/n^", vC.sobre); 
}

void cabecera2(struct vCotas vC){
    printf("%6s %12s %16s%1.1f %18s %12s%1.1f\n", "n", "t(n)",
      "t(n)/n^", vC.sub, "t(n)/n*log2(n)", "t(n)/n^", vC.sobre); 
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

void tiempo (char *sa, char *si, struct vCotas vC,
              void  (*alg) (int v [], int n),
              void  (*ini) (int v [], int n)){
    int n = 500; int v [32000]; int k = 100;
    int i; bool dir = false;
    double tInicial, tFinal, tTotal;

    printf("\nInicializiación %s\n\n", si );
    if ((dir=(strncmp(sa,"S",1)==0 && strncmp(si,"as",2)==0)))cabecera2(vC);
    else cabecera1(vC);
    for (n=n; n<=32000; n=n*2){
        printf("%7d", n);
        ini(v, n);        
        tInicial=microsegundos();
        alg(v,n);
        tFinal=microsegundos();
        tTotal=tFinal - tInicial;
        
         if (tTotal < 500){
              tInicial=microsegundos();
              for(i=0; i<k; i++){
                    ini(v, n);
                    alg(v, n); }
              tFinal = microsegundos();
              tTotal = tFinal - tInicial;
              tInicial = microsegundos();
              for(i=0; i<k; i++){ ini(v, n);}
              tFinal=microsegundos();
              tTotal= (tTotal-(tFinal-tInicial))/k;        
              printf("%14.3f*", tTotal);
         }
         else { printf("%14.3f", tTotal);}
         if (dir) cotas2(n, tTotal,  vC);
         else cotas1(n, tTotal, vC);
     }
}




int main (){
  char sa [20];
  char si [20];
  struct vCotas vC;
  
  inicializar_semilla();
  strcpy(sa, "por Inserccion");
  test(sa, ord_ins);
  printf("\n\n");
  strcpy(sa, "shell");
  test (sa, ord_shell);

  printf("\nMedición de tiempo algoritmo por Inserccion\n");
  strcpy(sa, "Inserccion");
  vC.sub=1.8; vC.sobre=2.2; vC.aj=2;
  strcpy(si, "aleatoria");
  tiempo(sa, si, vC, ord_ins, aleatorio); 
  vC.sub=0.8; vC.sobre=1.2; vC.aj=1;
  strcpy(si, "ascendente");
  tiempo(sa, si, vC, ord_ins, ascendente);
  vC.sub=1.8; vC.sobre=2.2; vC.aj=2;
  strcpy(si, "descendente");
  tiempo(sa, si, vC, ord_ins, descendente);

  printf("\nMedición de tiempo algoritmo Shell \n");
  strcpy(sa, "Shell");
  vC.sub=1; vC.sobre=1.4; vC.aj=1.195;
  strcpy(si, "aleatoria");
  tiempo(sa, si, vC, ord_shell, aleatorio);
  vC.sub=1; vC.sobre=1.2; vC.aj=1;
  strcpy(si, "ascendente");
  tiempo(sa, si, vC, ord_shell, ascendente);
  vC.sub=0.97; vC.sobre=1.37; vC.aj=1.165;
  strcpy(si, "descendente");
  tiempo(sa, si, vC, ord_shell, descendente);
}
                                                                                                           
