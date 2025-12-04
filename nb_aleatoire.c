#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int main()
{
   int bs;
      
   /* Initialisation du générateur de nombres aléatoires */
   srand((unsigned int) time(NULL));
   printf("Donnez la borne sup. de l'intervalle : ");
   scanf("%d",&bs);  
   /* Générer un nombre compris entre 0 et bs */
	printf("Nbre aléatoire <= %d : %d\n", bs, rand()%(bs+1));
}
