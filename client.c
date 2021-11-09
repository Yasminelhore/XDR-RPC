#include<stdio.h>
#include<rpc/xdr.h>

#define PROG 0x23456789
#define VERSION 1
#define MACHINE "localhost"
#define NUM_PROC_FACT 1
#define NUM_PROC_MINMAXMOY 2
#define NUM_PROC_SOMME 3
// MinMaxMoy ------------------------------------------------------------------------------------------------------------------------------
struct minmaxmoy{
	int T[5];
	int min;
	int max;
	double moy;
}minmaxmoy;
bool_t xdr_minmaxmoy(XDR *xdr_ptr, struct minmaxmoy *data){
	return (xdr_int(xdr_ptr, &data->max) && xdr_int(xdr_ptr, &data->min) && xdr_double(xdr_ptr, &data->moy) && xdr_vector(xdr_ptr,(char*)data->T,5,sizeof(int),(xdrproc_t)xdr_int));
}
// somme ------------------------------------------------------------------------------------------------------------------------
struct somme{
	int x;
	int y;	
}somme;
bool_t xdr_somme(XDR *xdr_ptr, struct somme *data){
	return (xdr_int(xdr_ptr, &data->x) && xdr_int(xdr_ptr, &data->y) );
}
// -------------------------------------------------------------------------------------------------------------------------------------
main()
{

  int c;
  int resultat_fact, data_fact;
  struct minmaxmoy data_minmaxmoy;
  struct minmaxmoy result_minmaxmoy;
  struct somme data_somme;
  int resultat_somme;
  int i;
refaire :
  printf("[1] Pour le calcule de X!\n");
  printf("[2] Pour le MinMax et la Moyenne\n");
  printf("[3] Pour la somme\n");
  printf("[4] Pour quitter\n");
  printf("Choix: ");
  scanf("%d", &c);
  switch(c){
  // ------------------------- CASE 1 :
    case 1:
        printf("Donner la valeur à calculée :");
        scanf("%d", &data_fact);
        callrpc(MACHINE, PROG, VERSION, NUM_PROC_FACT, xdr_int, (char*)&data_fact, xdr_int, (char*)&resultat_fact);
        printf("Calcul\n");
        printf("%d! = %d\n", data_fact, resultat_fact);
      goto refaire;
      break;
  // ------------------------- CASE 2 :
    case 2:
      printf("Donner 5 valeurs :\n");
      for(i=0; i<5; i++){
        printf("T(%d) = ", i);
        scanf("%d", &(data_minmaxmoy.T[i]));
      }
      callrpc(MACHINE, PROG, VERSION, NUM_PROC_MINMAXMOY, xdr_minmaxmoy, (char*)&data_minmaxmoy, xdr_minmaxmoy, (char*)&result_minmaxmoy);
      printf("Min = %d, Max = %d\n", result_minmaxmoy.min, result_minmaxmoy.max);
      printf("Moyenne = %f \n",result_minmaxmoy.moy);
      goto refaire;
      break;
  // ------------------------- CASE 3 :
    case 3:
      printf("Donner la première valeur :");	scanf("%d", &(data_somme.x));
      printf("Donner la deuxième valeur :");	scanf("%d", &(data_somme.y));
     
      callrpc(MACHINE, PROG, VERSION, NUM_PROC_SOMME,xdr_somme, (char*)&data_somme, xdr_double, (char*)&resultat_somme);
      printf("%d + %d = %d\n", data_somme.x, data_somme.y, resultat_somme);
      goto refaire;
      break;      
    default: exit(0);
  }
}
