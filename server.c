#include<stdio.h>
#include<rpc/xdr.h>
#include<rpc/rpc.h>
#include<rpc/types.h>
#define PROG 0x23456789
#define VERSION 1
#define NUM_PROC_FACT 1
#define NUM_PROC_MINMAXMOY 2
#define NUM_PROC_SOMME 3


// factoriel -------------------------------------------------

int *fact(int *n){
        static int r;
	int f=1;
	for(int i=1;i<=*n;i++)
	 f=f*i;
        r=f;
	return &r;
}
//MinMaxMoy------------------------------------------------------
struct minmaxmoy{
	int T[5];
	int min;
	int max;
	double moy;
}minmaxmoy;
bool_t xdr_minmaxmoy(XDR *xdr_ptr, struct minmaxmoy *data){
	return (xdr_int(xdr_ptr, &data->max) && xdr_int(xdr_ptr, &data->min) && xdr_int(xdr_ptr, &data->moy) && xdr_vector(xdr_ptr,(char*)data->T,5,sizeof(int),(xdrproc_t)xdr_int));
}
struct minmaxmoy * p_minmaxmoy(struct minmaxmoy *data){
	static struct minmaxmoy result;
	int s=0;
	result.min = 1000;
	result.max = -1000;
	result.moy = 0;
	for(int i=0;i<5;i++){
	  s=s+data->T[i];
	}
	result.moy=s/5;
	for(int i=0; i<5; i++){  
	       
		if(data->T[i]<=result.min) result.min = data->T[i];
		if(data->T[i]>=result.max) result.max = data->T[i];
	}
	
	return &result;
}

// somme ----------------------------------------------
struct somme{
	int x;
	int y;
	
}somme;
double * sum(struct somme *data){
	static double resultat;
	resultat = data->x + data->y; 	
	return &resultat;
}
bool_t xdr_somme(XDR *xdr_ptr, struct somme *data){
	return (xdr_int(xdr_ptr, &data->x) && xdr_int(xdr_ptr, &data->y));
}
// ------------------------------------------------------------
main(){
    registerrpc(PROG, VERSION, NUM_PROC_FACT, fact, xdr_int, xdr_int);
    registerrpc(PROG, VERSION, NUM_PROC_MINMAXMOY, p_minmaxmoy, xdr_minmaxmoy, xdr_minmaxmoy);
    registerrpc(PROG, VERSION, NUM_PROC_SOMME, sum, xdr_somme, xdr_double);
    svc_run();
}
