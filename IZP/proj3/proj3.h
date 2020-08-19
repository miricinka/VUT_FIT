/**
 * Kostra hlavickoveho souboru 3. projekt IZP 2018/19
 * a pro dokumentaci Javadoc.
 */

/**
 * \brief   Dokumentace tretiho projektu IZP
 * \file    proj3.h
 * \author  Mirka Kolarikova
 * \date    12.12.2018
 *
 * Dokumentace programu provadejici jednoduchou shlukovou analyzu, pomoci metody nejblizsiho souseda.
 */

/**  \defgroup grupa_1 Datove struktury
 * 
 */

//@{
/**  \struct obj_t
 * 
 *   Struktura nesouci informace o kazdem bodu (jeho ID, souradnice X a Y)
 */
struct obj_t {
    int id;  /**< Identifikator objektu        */
    float x; /**< Souradnice objektu na ose X  */
    float y; /**< Souradnuce objektu na ose Y  */
};

/**  \struct cluster_t
 * 
 *   Struktura nesouci informace o shluku bodu.
 */
struct cluster_t {
    int size;          /**< Aktualni velikost (pocet objektu ve shluku)  */
    int capacity;      /**< Kapacita shluku (pocet objektu, pro ktere je rezervovano misto v poli)  */
    struct obj_t *obj; /**< Adresa mista, kam se ukladaji jednotlive objekty   */
};
//@}

/**
 * \brief                   Tisk chyboveho hlaseni. 
 * \param          errnum   Cislo chyby
 * \param          hlaseni  Chybova hlaska 
 * \return                  0 pokud vse ok               
 *
 *Tiskne cislo chyby a chybovou hlasku na stderr.
 */
int my_err(int errnum, char hlaseni[]);

/**
 * \brief                   Kontrola vstupnich argumentu. 
 * \param          argc     Pocet vstupnich argumentu
 * \param          argv     Ukazatel na argumenty 
 * \return                  pocet pozadovanych shluku, v pripade chyby -1             
 *
 * Kontrola poctu vstupnich argumentu (v pripade nezadani 3. argumentu, pozadovany pocet shluku nastaven na jeden) a jejich spravnosti (argument nesouci informaci o pozadovanem poctu shluku nesmi byt mensi nez 0). V pripade nalezeni chyby tisknuta chybova hlaska pomoci funkce my_err.
 */
int my_arg_check(int argc, char *argv[]);

/**
 * \brief                   Inicializace shluku. 
 * \param[in, out] c        Ukazatel na strukturu shluku
 * \param          cap      pozadovana kapacita               
 *
 * Dynamicky alokuje pamet pro jednotlive kapacity objektu (cap). Ukazatel NULL u pole objektu znamena kapacitu 0.
 */
void init_cluster(struct cluster_t *c, int cap);

/**
 * \brief                   Inicializace shluku na prazdny shluk
 * \param[in, out] c        Ukazatel na strukturu shluku
 *
 *Uvolneni pameti, ktera byla alokovana funkci malloc, a nasledne zmenseni rezevovaneho mista pro objekty az na nulu.
 */
void clear_cluster(struct cluster_t *c);

///hodnota, o kterou se navysuje alokovana pamet
extern const int CLUSTER_CHUNK;


/**
 * \brief                  Zvetseni kapacity pro ulozeni objektu shluku
 * \param[in, out] c       Ukazatel na strukturu shluku
 * \param          new_cap Pozadovana hodnota zvetseni
 * \return         c       Ukazatel na strukturu shluku
 *
 * Na zaklade vstupniho parametru pozadovaneho rozsireni se dynamicky rozsiri alokovana pamet pro dalsi objekty.
 */
struct cluster_t *resize_cluster(struct cluster_t *c, int new_cap);

/**
 * \brief                  Pridani objektu ke shluku
 * \param[in, out] c       Ukazatel na strukturu shluku
 * \param          obj     Struktura objektu
 *
 * Prida objekt 'obj' na konec shluku 'c'. Rozsiri shluk, pokud se do nej objekt nevejde.
 */
void append_cluster(struct cluster_t *c, struct obj_t obj);

/**
 * \brief                  Slouceni dvou shluku
 * \param[in, out] c1      Ukazatel na strukturu shluku 1
 * \param[in, out] c2      Ukazatel na strukturu shluku 2
 *
 * Do shluku 'c1' prida objekty 'c2'. Shluk 'c1' bude v pripade nutnosti rozsiren.
 Objekty ve shluku 'c1' budou serazeny vzestupne podle identifikacniho cisla.
 Shluk 'c2' bude nezmenen.
 */
void merge_clusters(struct cluster_t *c1, struct cluster_t *c2);

/**
 * \brief                  Odstraneni shluku z pole shluku
 * \param[in, out] carr    Ukazatel na strukturu shluku
 * \param          narr    Prubezny pocet polozek shluku v poli
 * \param          idx     Na indexu idx se nachazi shluk pro odstraneni
 * \return                 Novy pocet shluku v poli
 *
 * Odstrani shluk z pole shluku 'carr'. Pole shluku obsahuje 'narr' polozek
 (shluku). Shluk pro odstraneni se nachazi na indexu 'idx'. Funkce vraci novy
 pocet shluku v poli.
 */
int remove_cluster(struct cluster_t *carr, int narr, int idx);

/**
 * \brief                  Euklidovska vzdalenost mezi dvema objekty
 * \param[in, out] o1      Ukazatel na strukturu objektu 1
 * \param[in, out] o2      Ukazatel na strukturu objektu 2
 * \return                 Euklidovska vzdalenost mezi dvema objekty
 *
 * Pomoci Pythagorovy vety pocita euklidovsou vzdalenost mezi dvema objekty.
 */
float obj_distance(struct obj_t *o1, struct obj_t *o2);

/**
 * \brief                  Euklidovska vzdalenost mezi dvema shluky
 * \param[in, out] c1      Ukazatel na strukturu shluku 1
 * \param[in, out] c2      Ukazatel na strukturu shluku 2
 * \return                 Euklidovska vzdalenost mezi dvema shluky
 *
 * Pomoci Pythagorovy vety pocita euklidovsou vzdalenost mezi dvema shluky(kazdy bod z jednoho shluku s kazdym bodem z druheho shluku).
 */
float cluster_distance(struct cluster_t *c1, struct cluster_t *c2);

/**
 * \brief                  Nalezeni dvou nejblizsich shluku
 * \param[in, out] carr    Ukazatel na strukturu shluku
 * \param          narr    Velikost shluku
 * \param[in, out] c1      Ukazatel na index shluku 1
 * \param[in, out] c2      Ukazatel na index shluku 2
 *
 * Funkce najde dva nejblizsi shluky. V poli shluku 'carr' o velikosti 'narr'
 hleda dva nejblizsi shluky. Nalezene shluky identifikuje jejich indexy v poli
 'carr'. Funkce nalezene shluky (indexy do pole 'carr') uklada do pameti na
 adresu 'c1' resp. 'c2'.
 */
void find_neighbours(struct cluster_t *carr, int narr, int *c1, int *c2);

/**
 * \brief                  Setrideni objektu ve shluku podle ID
 * \param[in, out] c       Ukazatel na strukturu shluku
 * \pre
 * id objektu je unikatni
 * \post
 * ve shluku jsou serazeny objekty vzestupne
 *
 * Razeni objektu ve shluku vzestupne podle jejich identifikatoru.
 */
void sort_cluster(struct cluster_t *c);

/**
 * \brief          Tisk shluku
 * \param[in] c    Ukazatel na strukturu shluku
 *
 * Tisk jednoho shluku na stdout.
 */
void print_cluster(struct cluster_t *c);

/**
 * \brief                  Prvotni nacteni shluku
 * \param[in] filename     Ukazatel na vstupni soubor
 * \param[in, out] arr     Ukazatel na pole shluku
 *
 * Ze souboru 'filename' nacte objekty. Pro kazdy objekt vytvori shluk a ulozi
 jej do pole shluku. Alokuje prostor pro pole vsech shluku a ukazatel na prvni
 polozku pole (ukalazatel na prvni shluk v alokovanem poli) ulozi do pameti,
 kam se odkazuje parametr 'arr'. Funkce vraci pocet nactenych objektu (shluku).
 V pripade nejake chyby uklada do pameti, kam se odkazuje 'arr', hodnotu NULL.
 */
int load_clusters(char *filename, struct cluster_t **arr);

/**
 * \brief                  Kondenzace dvou shluku.
 * \param      clusters    Ukazatel na strukturu shluku
 * \param      vychozi     Vychozi pocet shluku
 * \param      konecny     Pozadovany pocet shluku
 * \return 
 *
 * Nalezeni dvou nejblizsih shluku (find_neigbours) a jejich slouceni (merge_clusters). Nasledne odstrani nepotrebneho shluku z pameti (remove_cluster).
 */
int kondenzace(struct cluster_t *clusters, int vychozi, int konecny);

/**
 * \brief          Tisk pole shluku
 * \param[in] carr    Ukazatel na strukturu shluku
 * \param     narr Pozadovany pocet shluku na tisk
 *
 * Tisk pole shluku. Parametr 'carr' je ukazatel na prvni polozku (shluk).
 Tiskne se prvnich 'narr' shluku.
 */
void print_clusters(struct cluster_t *carr, int narr);
