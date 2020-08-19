/* ==========================================================================*/
/*                                                                           */
/*   Nazev programu: proj2.c                                                 */
/*   Popis programu: Iteracni vypocty  (2. projekt VUT FIT)                  */
/*   Autor:          Mirka Kolarikova                                        */
/*   Datum zmeny:    16.11.2018                                              */
/*                                                                           */
/* ==========================================================================*/

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#define EPS 1e-8


typedef struct parametr{//struktura pro ulozeni jednotlivych argumentu
	char prikaz[6];
	double x;
	double y;
	int n;
	int chyba;

}Parametr;


double my_abs(double x) {//funkce pro absolutni hodnotu
    return x < 0 ? -x : x;
}


Parametr *osetreni_vstupu (int argc, char *argv[], Parametr *uk_parametr){
	char *konec_stringu;
	
	if(argc == 4 || argc == 5) {}//ok 
	else{
		fprintf(stderr, "Zadan spatny pocet argumentu!!!!\n");
		uk_parametr->chyba = -1;
		return uk_parametr;
	}

	//osetreni argumentu u logaritmu
	if(argc == 4){ 
		char *prvni_argument = "--log";

		if((strcmp(argv[1], prvni_argument)) != 0){
			fprintf(stderr, "prikaz --pow zadan s nespravnym poctem argumentu.\n");
			uk_parametr->chyba = -1;
			return uk_parametr;
		}


		else{//konvert z charu na cisla

			strcpy(uk_parametr->prikaz, argv[1]); //naplneni prikazu do strukrury

			double x = strtod(argv[2], &konec_stringu);

			char *minus_nan = "-nan";
			if (strcmp(argv[2], minus_nan) == 0){
				uk_parametr->chyba = -4;
				return uk_parametr;
			}

			if (strlen(konec_stringu)>0){
				fprintf(stderr, "Argument X neni platne cislo!!!!\n"); 
				uk_parametr->chyba = -1;
				return uk_parametr;
			}
			if (x == -INFINITY){
				uk_parametr->chyba = -2;
				uk_parametr->x = x;
				return uk_parametr;
			}

			if(x < 0){
				uk_parametr->chyba = -2;
				uk_parametr->x = x;
				return uk_parametr;
			}
			if(x == 0){
				uk_parametr->chyba = -3;
				return uk_parametr;
			}
			
			uk_parametr->x = x; //naplneni x do struktury
	
			char *znaky;
			int n = strtol(argv[3], &znaky, 10);
			if(strlen(znaky)>0){
				fprintf(stderr, "Argument N neni platne cislo!!!!\n");
      			uk_parametr->chyba = -1;
				return uk_parametr;
      		}
      		if (n < 0)	{
      			fprintf(stderr, "Argument N (pocet iteraci) nemuze byt zaporne cislo!!!!\n");
      			uk_parametr->chyba = -1;
				return uk_parametr;
      		}
      		if (n == 0){
      		fprintf(stderr, "Argument N (pocet iteraci) musi byt vetsi nez 0!!!!\n");
      			uk_parametr->chyba = -1;
				return uk_parametr;
			}
			uk_parametr->n =n; //naplneni n do struktury
		}

	}

	//osetreni argumentu u power
	if(argc == 5){ 
		char *prvni_argument = "--pow";

		if((strcmp(argv[1], prvni_argument)) != 0){
			fprintf(stderr, "argument --log zadan s nespravnym poctem argumentu.\n");
			uk_parametr->chyba = -1;
			return uk_parametr;
		}
		else{
			strcpy(uk_parametr->prikaz, argv[1]); //naplneni prikazu do struktury

			double x = strtod(argv[2], &konec_stringu);
			
			if (strlen(konec_stringu)>0){
				fprintf(stderr, "Argument X neni platne cislo!!!!\n"); 
				uk_parametr->chyba = -1;
				return uk_parametr;
			}
			 
			char *kladne_nan = "nan";
			if(x == NAN || strcmp(argv[2], kladne_nan) == 0){
				
				uk_parametr->chyba = -5;
				uk_parametr->x = x;
				//return uk_parametr;
			}
			

			if(x < 0){
				fprintf(stderr, "Prirozeny logaritmus ze zaporneho cisla nelze vypocitat a pouzit ho na dalsi vypocty!!!!\n");
				uk_parametr->chyba = -1;
				return uk_parametr;
			}
			uk_parametr->x = x; //naplneni x do struktury

			char *konec_stringu;
			double y = strtod(argv[3], &konec_stringu);
			
			if (strlen(konec_stringu)>0){
				fprintf(stderr, "Argument Y neni platne cislo!!!!\n"); 
				uk_parametr->chyba = -1;
				return uk_parametr;
			}
			
			uk_parametr->y = y; //naplneni y do struktury

			
			if(y == NAN || strcmp(argv[3], kladne_nan) == 0){
				uk_parametr->chyba = -5;
				uk_parametr->y = y;
				//return uk_parametr;
			}
			
			if(uk_parametr->chyba == -5){
				return uk_parametr;
			}


			char *znaky;
			int n = strtol(argv[4], &znaky, 10);
			if(strlen(znaky)>0){
				fprintf(stderr, "Argument N neni platne cislo!!!!\n");
      			uk_parametr->chyba = -1; 
				return uk_parametr;
      		}
      		if (n < 0)	{
      			fprintf(stderr, "Argument N (pocet iteraci) nemuze byt zaporne cislo!!!!\n");
      			uk_parametr->chyba = -1;
				return uk_parametr;
      		}
      		if (n == 0){
      		fprintf(stderr, "Argument N (pocet iteraci) musi byt vetsi nez 0!!!!\n");
      			uk_parametr->chyba = -1;
				return uk_parametr;
			}
      		uk_parametr->n = n; //naplneni n do struktury
		}
	}

return uk_parametr;
}


double taylor_log(double x, unsigned int n){ //Tayloruv polinom pro vypocet prirozeneho logaritmu
	double vysledek = 0;
	if (x == INFINITY){ //logaritmus z nekonecna je nekonecno
		vysledek = INFINITY;
		return vysledek;
	}

	
	if (x < 1){
		double y = 1-x;
		double a = 1;
		double c = 0;
		for (unsigned int i = 1; i <= n; i++){
			a = (y*a);
			c = (c - (a/i));
		} 
		vysledek = c;
	}
	else{
		double y = (x - 1)/x;
		double a = 1;
		double c = 0;
		for (unsigned int i = 1; i <= n; i++){
			a = (y*a);
			c = (c + (a/i));
		}
		vysledek = c;
	}

	return vysledek;
}


double cfrac_log(double x, unsigned int n){ //zretezene zlomky pro vypocet prirozeneho logaritmu
	int m = n-1;
    double vysledek = 0;
    double z = (x-1)/(1+x);
    double cf = 2*m+1; //prvni hodnota

    if (x == INFINITY){ //log z nekonecna je nekonecno
		vysledek = INFINITY;
		return vysledek;
	}

    for(; m>=1; m--){
    	cf = (2*m-1) - ((m*m*z*z)/(cf));
    }

    vysledek = (2*z)/cf;
	return vysledek;
}

double taylor_pow(double x, double y, unsigned int n){
	double vysledek = 0;
	if(x == 0){ //0 na cokoliv je 0
		vysledek = 0;
		return vysledek;
	}
	
	if(y == 0){ //cokoliv na 0 je 1
		vysledek = 1;
		return vysledek;
	}
	if (n == 1){ //v pripade jedne iterace je vysledek 1 //na konci +1 iterace
		vysledek =1;
		return vysledek;
	}
	double log_x = taylor_log(x, n); //prirozeny logaritmus z x
	double citatel = 1;
	double jmenovatel = 1;
	double posloupnost = 1;
	double zlomek;
	n = n-1; //na konci pricitame o jednu iteraci navic

	for (unsigned int i =1; i<=n; i++){
		citatel = citatel*y*log_x;
		jmenovatel = jmenovatel*i;
		zlomek = citatel/jmenovatel;
		posloupnost = posloupnost + zlomek;
	}

	vysledek = posloupnost;
	return vysledek;
}


double taylorcf_pow(double x, double y, unsigned int n){
	double vysledek = 0;
	
	if(x == 0){ //0 na cokoliv je 0
		vysledek = 0;
		return vysledek;
	}
	
	if(y == 0){ //cokoliv na 0 je 1
		vysledek = 1;
		return vysledek;
	}
	if (n == 1){ //v pripade jedne iterace je vysledek 1 //na konci +1 iterace
		vysledek =1;
		return vysledek;
	}
	double log_x = cfrac_log(x, n); //prirozeny logaritmus z x
	double citatel = 1;
	double jmenovatel = 1;
	double posloupnost = 1;
	double zlomek;
	n = n-1; //na konci pricitame o jednu iteraci navic 

	for (unsigned int i =1; i<=n; i++){
		citatel = citatel*y*log_x;
		jmenovatel = jmenovatel*i;
		zlomek = citatel/jmenovatel;
		posloupnost = posloupnost + zlomek;
	}

	vysledek = posloupnost;
	return vysledek;
}


void my_print_log(Parametr *uk_parametr){ //tisk hodnot logaritmu

	double a = log(uk_parametr->x);
	double b = cfrac_log(uk_parametr->x,uk_parametr->n);
	double c = taylor_log(uk_parametr->x,uk_parametr->n);

	if (uk_parametr->chyba == -2){
		printf("       log(%g) = %.12g\n", uk_parametr->x, a);
		printf(" cfrac_log(%g) = %.12g\n", uk_parametr->x, NAN);
		printf("taylor_log(%g) = %.12g\n", uk_parametr->x, NAN);
	}
	else if (uk_parametr->chyba == -3){
		printf("       log(%g) = %.12g\n", uk_parametr->x, a);
		printf(" cfrac_log(%g) = %.12g\n", uk_parametr->x, -INFINITY);
		printf("taylor_log(%g) = %.12g\n", uk_parametr->x, -INFINITY);
	}
	else if (uk_parametr->chyba == -4){
		printf("       log(%s) = %s\n", "-nan", "-nan");
		printf(" cfrac_log(%s) = %s\n", "-nan", "-nan");
		printf("taylor_log(%s) = %s\n", "-nan", "-nan");
	}
	
	else{
		printf("       log(%g) = %.12g\n", uk_parametr->x, a);
		printf(" cfrac_log(%g) = %.12g\n", uk_parametr->x, b);
		printf("taylor_log(%g) = %.12g\n", uk_parametr->x, c);
	}

}


void my_print_pow(Parametr *uk_parametr){ //tisk hodnot mocniny
	double a = pow(uk_parametr->x, uk_parametr->y);


	if(uk_parametr->chyba == -5){
		printf("         pow(%g,%g) = %.12g\n", uk_parametr->x,uk_parametr->y, a);
		printf("  taylor_pow(%g,%g) = %.12g\n", uk_parametr->x,uk_parametr->y, NAN);
		printf("taylorcf_pow(%g,%g) = %.12g\n", uk_parametr->x,uk_parametr->y, NAN);

	}
	else{
		double b = taylor_pow(uk_parametr->x, uk_parametr->y, uk_parametr->n);
		double c = taylorcf_pow(uk_parametr->x, uk_parametr->y, uk_parametr->n);
		printf("         pow(%g,%g) = %.12g\n", uk_parametr->x,uk_parametr->y, a);
		printf("  taylor_pow(%g,%g) = %.12g\n", uk_parametr->x,uk_parametr->y, b);
		printf("taylorcf_pow(%g,%g) = %.12g\n", uk_parametr->x,uk_parametr->y, c);
	}
}


int main(int argc, char *argv[]){
	Parametr parametr = {"", 0.0, 0.0, 0, 0}; //inicializace struktury
	Parametr *uk_parametr = &parametr;

	//osetreni vstupu
	uk_parametr = osetreni_vstupu(argc, argv, uk_parametr);
	//printf("Test err: %d prikaz je: %s X je: %lf Y je: %lf N je: %d\n", uk_parametr->chyba, uk_parametr->prikaz, uk_parametr->x, uk_parametr->y, uk_parametr->n);
	if (uk_parametr->chyba == -1){return 1;}
	if (uk_parametr->chyba == -2){
		my_print_log(uk_parametr);
		return 0;
	}
	if (uk_parametr->chyba == -3){
		my_print_log(uk_parametr);
		return 0;
	}
	if (uk_parametr->chyba == -4){
		my_print_log(uk_parametr);
		return 0;
	}
	if (uk_parametr->chyba == -5){
		my_print_pow(uk_parametr);
		return 0;
	}


	//volani funkce pro tisk vysledku logaritmu
	if(argc == 4){
		my_print_log(uk_parametr);	
	}

	//volani funkce pro tisk vysledku power
	if(argc == 5){
		my_print_pow(uk_parametr);
	}


	return 0;
}