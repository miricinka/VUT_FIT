/* ==========================================================================*/
/*                                                                           */
/*   Nazev programu: proj3.c                                                 */
/*   Popis programu: Jednoducha shlukova analyza: 2D nejblizsi soused.       */
/*   Autor:          Mirka Kolarikova                                        */
/*   Datum zmeny:    9.12.2018                                               */
/*                                                                           */
/* ==========================================================================*/


/**
 * Kostra programu pro 3. projekt IZP 2018/19
 *
 * Jednoducha shlukova analyza: 2D nejblizsi soused.
 * Single linkage
 */
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <math.h> // sqrtf
#include <limits.h> // INT_MAX
#include <string.h>

/*****************************************************************
 * Ladici makra. Vypnout jejich efekt lze definici makra
 * NDEBUG, napr.:
 *   a) pri prekladu argumentem prekladaci -DNDEBUG
 *   b) v souboru (na radek pred #include <assert.h>
 *      #define NDEBUG
 */
#ifdef NDEBUG
#define debug(s)
#define dfmt(s, ...)
#define dint(i)
#define dfloat(f)
#else

// vypise ladici retezec
#define debug(s) printf("- %s\n", s)

// vypise formatovany ladici vystup - pouziti podobne jako printf
#define dfmt(s, ...) printf(" - "__FILE__":%u: "s"\n",__LINE__,__VA_ARGS__)

// vypise ladici informaci o promenne - pouziti dint(identifikator_promenne)
#define dint(i) printf(" - " __FILE__ ":%u: " #i " = %d\n", __LINE__, i)

// vypise ladici informaci o promenne typu float - pouziti
// dfloat(identifikator_promenne)
#define dfloat(f) printf(" - " __FILE__ ":%u: " #f " = %g\n", __LINE__, f)

#endif

/*****************************************************************
 * Deklarace potrebnych datovych typu:
 *
 * TYTO DEKLARACE NEMENTE
 */

struct obj_t { //struktura objektu
    int id; //identifikator
    float x; //souradnice x
    float y; //souradnice y
};

struct cluster_t { //shluk objektu
    int size; //pocet objektu ve shluku
    int capacity; //kapacita shluku (pocet objektu, pro ktere je rezervovano misto v poli)
    struct obj_t *obj; //ukazatel na pole shluku
};

/*****************************************************************
 * Deklarace potrebnych funkci.
 *
 * PROTOTYPY FUNKCI NEMENTE
 *
 * IMPLEMENTUJTE POUZE FUNKCE NA MISTECH OZNACENYCH 'TODO'
 *
 */


int my_err(int errnum, char hlaseni[]){  // #0. moje error hlaseni

  fprintf(stderr,"Error (code:%i) (%s)\n",errnum,hlaseni); 
return 0;     
}


int my_arg_check(int argc, char *argv[]){ // kontrola poctu argumentu programu
    int n = 0;
    
    if(argc == 2 || argc == 3) {}//ok 
    else{
        my_err(600, "Zadan spatny pocet argumentu!!!!");
        return -1;
    }
    
    if (argc == 3){
        char *znaky;

        n = strtol(argv[2], &znaky, 10);
        if(strlen(znaky)>0){
            my_err(601, "Druhy argument (pocet shluku) neni platne cislo!!!!");
            return -1;
        }
        
        if (n < 0)  {
            my_err(602, "Druhy argument (pocet shluku) musi byt nezaporne cislo!!!!");
            return -1;
        }

        if (n == 0){
            my_err(603, "Druhy argument (pocet shluku) musi byt vetsi nez 0!!!!");
            return -1;
        }
        
    }
    
    if (argc == 2){
        n = 1;
    }

return n;  //vraci info o poctu pozadovanych shluku, v pripade chyby -1
}

/*
 Inicializace shluku 'c'. Alokuje pamet pro cap objektu (kapacitu).
 Ukazatel NULL u pole objektu znamena kapacitu 0.
*/
void init_cluster(struct cluster_t *c, int cap)
{
    assert(c != NULL);
    assert(cap >= 0);
    c->size = 0; //v tuto chvili v clusteru neni zadny objekt

    //alokace pameti pro jednotlive objekty prirazene ke clusteru
    if (cap > 0) {
        if ((c->obj = malloc(sizeof(struct obj_t) * cap))) { //TODO zkontrolovat malloc
            c->capacity = cap;
            return;
        }
    }
    if(cap == 0){
    c->capacity = 0;
    c->obj = NULL;}
}

/*
 Odstraneni vsech objektu shluku a inicializace na prazdny shluk.
 */
void clear_cluster(struct cluster_t *c)
{
    free(c->obj); //opak malloc, tj.uvolneni pameti
    init_cluster(c, 0); //zmenseni rezerevovaneho mista pro objekty ve shluku az na nulu
}

/// Chunk of cluster objects. Value recommended for reallocation.
const int CLUSTER_CHUNK = 10;

/*
 Zmena kapacity shluku 'c' na kapacitu 'new_cap'.
 */
struct cluster_t *resize_cluster(struct cluster_t *c, int new_cap)
{
    // TUTO FUNKCI NEMENTE
    assert(c);
    assert(c->capacity >= 0);
    assert(new_cap >= 0);
    if (c->capacity >= new_cap)
        return c;

    size_t size = sizeof(struct obj_t) * new_cap;

    void *arr = realloc(c->obj, size);
    if (arr == NULL)
        return NULL;

    c->obj = (struct obj_t*)arr;
    c->capacity = new_cap;
    return c;
}

/*
 Prida objekt 'obj' na konec shluku 'c'. Rozsiri shluk, pokud se do nej objekt
 nevejde.
 */
void 
append_cluster(struct cluster_t *c, struct obj_t obj)
{
    if(c->size >= c->capacity)
    {
        if( ! resize_cluster(c, c->capacity + CLUSTER_CHUNK)) {return;}
    }
    //index je od nuly, takze idnex velikosti size predstavuje volnou pozici v poli
    c->obj[c->size] = obj;
    //zvysim o jednicku pocet objektu v clusteru 
    c->size=c->size + 1;
    
    
}

// pomocna funkce pro razeni shluku
static int obj_sort_compar(const void *a, const void *b)
{
    // TUTO FUNKCI NEMENTE
    const struct obj_t *o1 = (const struct obj_t *)a;
    const struct obj_t *o2 = (const struct obj_t *)b;
    if (o1->id < o2->id) return -1;
    if (o1->id > o2->id) return 1;
    return 0;
}



/*
 Razeni objektu ve shluku vzestupne podle jejich identifikatoru.
*/
void sort_cluster(struct cluster_t *c)
{
    // TUTO FUNKCI NEMENTE
    qsort(c->obj, c->size, sizeof(struct obj_t), &obj_sort_compar);
}


/*
 Do shluku 'c1' prida objekty 'c2'. Shluk 'c1' bude v pripade nutnosti rozsiren.
 Objekty ve shluku 'c1' budou serazeny vzestupne podle identifikacniho cisla.
 Shluk 'c2' bude nezmenen.
 */
void merge_clusters(struct cluster_t *c1, struct cluster_t *c2)
{
    assert(c1 != NULL);
    assert(c2 != NULL);
    
    for(int i = 0;i < c2->size; i++){
   
        append_cluster(c1, c2->obj[i]);
    
    }

    sort_cluster(c1);
    
}




/**********************************************************************/
/* Prace s polem shluku */

/*
 Odstrani shluk z pole shluku 'carr'. Pole shluku obsahuje 'narr' polozek
 (shluku). Shluk pro odstraneni se nachazi na indexu 'idx'. Funkce vraci novy
 pocet shluku v poli.
*/
int remove_cluster(struct cluster_t *carr, int narr, int idx)
{
    assert(idx < narr);
    assert(narr > 0);


    int pocet_prubezny = narr -1; //jeden odstranujeme
    clear_cluster(&carr[idx]); // odstraneni pole pro objekty a vynulovani udaju o size cap atd. v clusteru
    for(int i = idx; i < pocet_prubezny; i++){ //odstraneni clusteru z pole a vse se posune o jeden index
        carr[i] = carr[i + 1];
    }
    return pocet_prubezny;
}

/*
 Pocita Euklidovskou vzdalenost mezi dvema objekty.
 */
float obj_distance(struct obj_t *o1, struct obj_t *o2)
{
    assert(o1 != NULL);
    assert(o2 != NULL);
    float vzdalenost=0.0;

    vzdalenost = sqrtf(powf(o1->x - o2->x, 2.0) + powf(o1->y - o2->y, 2.0));

    return vzdalenost;
}



/*
 Pocita vzdalenost dvou shluku.
 */

float cluster_distance(struct cluster_t *c1, struct cluster_t *c2)
{
    assert(c1 != NULL);
    assert(c1->size > 0);
    assert(c2 != NULL);
    assert(c2->size > 0);

    float min_vzdalenost = INFINITY;
    float vzdalenost = 0.0;

    for(int i = 0; i< c1->size; i++){
        for (int j = 0; j < c2->size; j++){

            vzdalenost = obj_distance(&c1->obj[i], &c2->obj[j]);

            if(vzdalenost<min_vzdalenost){
                min_vzdalenost = vzdalenost;
            }
        }
    }
    return min_vzdalenost;
}


/*
 Funkce najde dva nejblizsi shluky. V poli shluku 'carr' o velikosti 'narr'
 hleda dva nejblizsi shluky. Nalezene shluky identifikuje jejich indexy v poli
 'carr'. Funkce nalezene shluky (indexy do pole 'carr') uklada do pameti na
 adresu 'c1' resp. 'c2'.
*/
void find_neighbours(struct cluster_t *carr, int narr, int *c1, int *c2)
{
    assert(narr > 0);
    //pokud zbyl posledni cluster
    if (narr == 1){
        *c1 = 0;
        *c2 = 0;
        return;
    }


    float min_vzdalenost = INFINITY;
    float vzdalenost = 0.0;

    for(int i = 0; i< narr; i++){
        for (int j = i + 1; j < narr; j++){

            vzdalenost = cluster_distance(&carr[i], &carr[j]);

            
            if(vzdalenost<min_vzdalenost){
                min_vzdalenost = vzdalenost;
                *c1 = i;
                *c2 = j;
            }
        }
    }
    return;
}


/*
 Tisk shluku 'c' na stdout.
*/
void print_cluster(struct cluster_t *c)
{
    // TUTO FUNKCI NEMENTE
    for (int i = 0; i < c->size; i++)
    {
        if (i) putchar(' ');
        printf("%d[%g,%g]", c->obj[i].id, c->obj[i].x, c->obj[i].y);
    }
    putchar('\n');
}


/*
 Ze souboru 'filename' nacte objekty. Pro kazdy objekt vytvori shluk a ulozi
 jej do pole shluku. Alokuje prostor pro pole vsech shluku a ukazatel na prvni
 polozku pole (ukalazatel na prvni shluk v alokovanem poli) ulozi do pameti,
 kam se odkazuje parametr 'arr'. Funkce vraci pocet nactenych objektu (shluku).
 V pripade nejake chyby uklada do pameti, kam se odkazuje 'arr', hodnotu NULL.
*/
int load_clusters(char *filename, struct cluster_t **arr)
{
    assert(arr != NULL);
    *arr = NULL;
                
    FILE *soubor;
    soubor = fopen(filename, "r");
    if (soubor == NULL){  //Test, zda se podarilo nacist               
        my_err(604,"Soubor s prikazy nelze otevrit. Zkontrolujte, zda existuje.");
        fclose(soubor);
        *arr = NULL;
        return -1;
    }
    char radek[500]="";   // radek obsahuje trikrat INT, tj. nebude delsi
    int pocet_objektu = 0;
    char pismena_za[1000];
    struct obj_t obj;              //lokalni promenna pro objekt 
    struct cluster_t *cluster;     //lokalni promenna pro shluk
    int x = 0; //pomocna promena pro kontrolu sscanf
    int y = 0;
    int a = 0;
    int b = 0;
    int c = 0;
        
    int i = 0; // pocitadlo radku  prikazu
    while (fscanf(soubor, " %1023[^\n^\r]", radek) != EOF){ // vypise stream, ale jen plne radky (bez koncoveho "\n" a "\r") 
          i++;  // prvni radek dostane index 1
          //printf("%s\n", radek); //LADENI
          if (i ==1){
            x = sscanf(radek, "count=%d%s",&pocet_objektu, pismena_za);
           // printf("x je: %d\n", x); //LADENI
            if (x != 1){
                my_err(605, "Chybne zadan prvni radek souboru!!!");
                fclose(soubor);
                *arr = NULL;
                return -1;
            }
            
            // alokace pameti pro jednotlive clustery
              *arr = malloc(pocet_objektu * sizeof(struct cluster_t)) ;                           
	            if (*arr == NULL) {
                        my_err(6051, "Nepodarila se alokovat pamet pro pole shluku!!!");
                        fclose(soubor);
                        return -1;
	            }
	          // kazdy cluster bude mit svoje pole pro ulozeni bodu, tj. init_cluster musi obsahovat malloc
	             for (int k = 0; k < pocet_objektu; k++) {
		                 init_cluster(&(*arr)[k], CLUSTER_CHUNK);
	             }       
			        continue;
          }
          if (i>1){
              y = sscanf(radek, "%d %d %d%s",&a, &b, &c, pismena_za);
              //printf("y je: %d, a = %d, b = %d, c = %d\n", y, a , b , c); //LADENI
              if (y != 3){ //kontrola vstupu
                  my_err(605, "Chybne nejaky radek souboru!!!");
                  fclose(soubor);
                  *arr = NULL;
                  return -1;
              }
              if (b<0 || b>1000 || c<0 || c>1000){ //spravne souradnice?
                  my_err(607, "Chybe zadana souradnice objedku!!!!");
                  fclose(soubor);
                  *arr = NULL;
                  return -1;
              }
            // Dodelat kontrolu, ze ID je unikatni TODO
            }
            //  naplneni id a souradnic do struktury
                obj.id = a;
                obj.x =  b;
                obj.y =  c;
                // zapamatuji si adresu
                // protoze index prvniho prvku pole ma byt nula a protoze
                // prvni datovy radek ma i=2, tak tu dvojku musim odecist (i-2 )
                cluster = &(*arr)[i - 2];
                
               //zavolame funkci, ktera objekt (tj. bod v prostoru) priradi do clusteru(skupiny)    
                append_cluster(cluster, obj);
                // printf("Cluster je %d %d %i\n", cluster->size, cluster->capacity, &cluster->obj);         // LADENI
                if (cluster->size != 1) {
                       my_err(608,"Nepodarilo se alokovat pamet a vlozit objekt do clusteru.");
                       //uvolenni pameti, protoze stejne neni potreba
                       clear_cluster(*arr);
                       *arr = NULL;
                       fclose(soubor);
                       return -1;
                }


        if(i == pocet_objektu+1) {break;} //kontrola jestli uz je nacteny dostatecny pocet objektu pdle infa v prvnim radku
                                          // dalsi se podle zadani maji ignorovaty
     }  // END WHILE
     
     
     if (pocet_objektu < i-1){ //mensi pocet objektu nez uvedeno? v i je navic prvni informacni radek
        my_err(606, "Pocet objektu je nizsi nez je uvedeno na prvnim radku.");
        clear_cluster(*arr);
        *arr = NULL;
        fclose(soubor);
        return -1;
     }   
     
     // Dodelat kontrolu, ze pocet pozadovanych shluku neni vetsi, nez pocet vychozich objektu 

    if (fclose(soubor) == EOF){ 
        my_err(605,"Soubor s prikazy se nepodarilo uzavrit.");
        *arr = NULL;
        return -1;
    }

    return pocet_objektu;
}

int kondenzace(struct cluster_t *clusters, int vychozi, int konecny){
    int c1_index_clusteru;
    int c2_index_clusteru;
    int prubezny = vychozi;
 
    while(prubezny > konecny){
        //nalezeni nejblizsiho shluku
        find_neighbours(clusters, prubezny, &c1_index_clusteru, &c2_index_clusteru);
        //slouceni
        merge_clusters(&clusters[c1_index_clusteru], &clusters[c2_index_clusteru]);   
        // odstraneni uz nepotrebneho    
        prubezny = remove_cluster(clusters, prubezny, c2_index_clusteru);
       
    }
    return prubezny;
}


/*
 Tisk pole shluku. Parametr 'carr' je ukazatel na prvni polozku (shluk).
 Tiskne se prvnich 'narr' shluku.
*/
void print_clusters(struct cluster_t *carr, int narr)
{
    printf("Clusters:\n");
    for (int i = 0; i < narr; i++)
    {
        printf("cluster %d: ", i);
        print_cluster(&carr[i]);
    }
}


int main(int argc, char *argv[])
{
    struct cluster_t *clusters;

    int pocet_konecny = 0; //pocet shluku (pozadovanych)
    int pocet_vychozi = 0;
    int finalni =0;

    // #A. kontrola argumentu a ziskani poctu pozadovanych shluku
    pocet_konecny = my_arg_check(argc, argv);  
    if (pocet_konecny == -1) {return -1;} 

    // #B. nacteni souboru s objekty   
    pocet_vychozi = load_clusters(argv[1], &clusters);
    if (pocet_vychozi == -1) {return -1;}
     
    // #C. Vypis toho, co udelala load_clusters
		//print_clusters(clusters, pocet_vychozi);

    // #D. spojovani shluku na pozadovany konecny pocet shluku
   finalni=kondenzace(clusters, pocet_vychozi, pocet_konecny);
     
    // #E. Vypis toho, co zustalo v clusters
		print_clusters(clusters, finalni);
         
    
    //#X. uvolenni pameti pro objekty v poli
			for (int i = 0; i < finalni; i++) {
		      clear_cluster(&clusters[i]);
          } 
   
    // #Y. uvolneni pameti pro clustery
    free(clusters);
    
    
    return 0;
}
