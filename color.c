#include<stdio.h>

// '\e' = '%1B' = caractere escape

int main()
{
	char c='A';
	
	printf("\e[1;30mHello World %c\e[0m\n",c); // noir gras
	printf("\e[1;31mHello World %c\e[0m\n",c); // rouge gras
	printf("\e[1;32mHello World %c\e[0m\n",c); // vert gras
	printf("\e[1;33mHello World %c\e[0m\n",c); // jaune gras
	
		
	printf("\nbonjour\n");
	
	printf("\e[1;34mHello World %c\e[0m\n",c); // bleu gras
	printf("\e[1;35mHello World %c\e[0m\n",c); // violet gras
	printf("\e[1;36mHello World %c\e[0m\n",c); // cyan gras
	printf("\e[1;37mHello World %c\e[0m\n",c); // blanc gras
	
	printf("\nbonjour\n");
	
	printf("\e[22;30mHello World %c\e[0m\n",c);
	printf("\e[22;31mHello World %c\e[0m\n",c);
	printf("\e[22;32mHello World %c\e[0m\n",c);
	printf("\e[22;33mHello World %c\e[0m\n%c\n",c, c);
	
	return 0;
}
