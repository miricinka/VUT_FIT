/* ==========================================================================*/
/*                                                                           */
/*   Nazev programu: proj1.c                                                 */
/*   Popis programu: Proudovy textovy editor (1. projekt VUT FIT)            */
/*   Autor:          Mirka Kolarikova                                        */
/*   Datum zmeny:    3.11.2018                                               */
/*                                                                           */
/* ==========================================================================*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#define RADEK 1024
#define PRIKAZY 105
#define NAPOVEDA " \n"\
"* NAPOVEDA \n"\
"* Nazev programu    : 'proj1' \n"\
"* Ocekavany argument: JMENO_SOUBORU_S_PRIKAZY\n"\
"* Priklad spusteni  : './proj1 JMENO_SOUBORU_S_PRIKAZY  <VSTUP' \n"\
"* Popis             : Program cte postupne radky textu ze vstupu VSTUP \n"\
"*                   : a na zaklade souboru s prikazy JMENO_SOUBORU_S_PRIKAZY\n"\
"*                   : tyto radky upravi a posle na vystup.\n"\
"*                   : Nezname prikazy jsou ignorovany."



char seznam[PRIKAZY][RADEK];  // seznam radku kam se nacte JMENO_SOUBORU_S_PRIKAZY, definovano globalne
char text_new[RADEK] = ""; //pracovni promenna pro nacitani radku textu


int my_err(int errnum, char hlaseni[]){  // #0. moje error hlaseni

  fprintf(stderr,"Error (code:%i) (%s)\n",errnum,hlaseni); 
return 0;     
}


int my_arg_count(int argc, int cislo){ // #A. kontrola poctu argumentu programu

	if (argc != cislo){
		my_err(600,"Byl vlozen chybny pocet argumentu.");
		printf("%s\n", NAPOVEDA);
		return 600;
	}
return 0;  
}


int my_seznam_prikazu(char *soubor){ // #C. Nacteni seznamu prikazu.

  int pocet_prikazu = 0;
  char radek[RADEK]=""; 
  FILE *soubor1;           // urceno pro JMENO_SOUBORU_S_PRIKAZY
  soubor1 = fopen(soubor, "r");

  if (soubor1 == NULL){  /* Test, zda se podarilo nacist */                
    my_err(601,"Soubor s prikazy nelze otevrit. Zkontrolujte, zda existuje.");
    return 601;
  }

  int i = 0; // pocitadlo radku  prikazu
  while (fscanf(soubor1, " %1023[^\n^\r]", radek) != EOF){ // vypise stream, ale jen plne radky (bez koncoveho "\n" a "\r") 
    i++;  // prvni prikaz dostane index 1
    strncpy(seznam[i],radek,strlen(radek));                      // kopiruji radek do seznamu
  }

  if (i==0){// Pokud bude soubor s prikazy prazdny, tak info a konec:
    my_err(602,"Soubor s prikazy je prazdny.");
    return 602;
  }

  pocet_prikazu = i;  // Zapamatovani si poctu prikazu

  if (fclose(soubor1) == EOF){ 
    my_err(603,"Soubor s prikazy se nepodarilo uzavrit.");
    return 603;
    }  
  return pocet_prikazu;  
}


int my_output(char *text){ // VYSTUP NA STDOUT

   int ret=0;
   ret = fputs(text, stdout); 
   if (ret == EOF) {my_err(604,"Radek textu se nepodarilo odeslat na vystup.");}  
return 0;  
}


char *my_text_bezn(char *text){  // odstranim '\n' z radku textu

   for(int i=0;text[i]!='\0';i++) {if(text[i]=='\r'){text[i]=0;}} 
   for(int i=0;text[i]!='\0';i++) {if(text[i]=='\n'){text[i]=0;}}   
return text; 
}


int my_str_vymena(char *text, char *pattern, char *replacement){ // vraci -1, pokud tam pattern neni

   char *zacatek_pattern_text;
   char *zacatek_zbytek_text;  
   char text_tmp[RADEK] = ""; //pracovni promenna
   int delka_p=strlen(pattern); 
   
   // Vymena retezce     
   if (strstr(text,pattern) == 0){   // hledany vyraz v radku textu neni, prikaz nic nedela  
    return -1;                     
   }
   else{ 
     zacatek_pattern_text = strstr(text,pattern);        // adresa, kde je zacatek pattern  
     zacatek_zbytek_text  = (zacatek_pattern_text +  delka_p); // adresa, kde je nalezeny pattern
     int delka_zacatek_text  = (zacatek_pattern_text -  text);                 
     strncpy(text_tmp, text, delka_zacatek_text); // mam ZACATEK textu!
                                                  // mam STRED textu v replacement
                                                  // mam KONEC textu v dresa_zbytek                                                                
     strcat(text_tmp,replacement);
     strcat(text_tmp,zacatek_zbytek_text);
     strcpy(text_new,text_tmp);                      
   }
return 0;
}


int my_text_replace(char *seznam) {

   char content[RADEK]="";
   char pattern[RADEK]="";
   char replacement[RADEK]="";
   
   strcpy(content,&seznam[1]);
   char oddelovac = content[0];
   
   // nalezeni pattern a replacement
   switch(oddelovac){
     case 32: // oddelovac je mezera (nebo vic mezer), cislo 32 dle ASCII
              //nacita udaje s vyuzitim faktu ze je mezi nimi mezera (nebo vic mezer, ktere jsou chapany jako jedina mezera)
              //POZN.: pokud zadani projektu bylo mysleno jinak, potom by stacila pro tento pripad vetev 'default'
        { sscanf (content,"%s %s",pattern,replacement);                                          
        }
        break;
     default:  
        {                                 
        char *zacatek_pattern_content = (content+1);
        char *nasledujici_oddelovac   = strchr(zacatek_pattern_content,oddelovac);                
        int delka_pattern_content=(nasledujici_oddelovac - zacatek_pattern_content); 
        strncpy(pattern,zacatek_pattern_content,delka_pattern_content); // sestavim si pattern              
        strcpy(replacement,(nasledujici_oddelovac+1)); // sestavim si replacement        
       }             
        break; 
   } // konec switch     
            
   // Otestuji, zda je prikaz v poradku   
   int delka_p=strlen(pattern); 
   int delka_r=strlen(replacement);                        
   if (delka_p==0 || delka_r==0){   // kontrola, zda tam jsou oba, jak pattern, tak replacement
      char info[80]="";             
      sprintf(info,"Soubor s prikazy obsahuje nekompletni prikaz (%s). Bude ignorovan!",seznam);    
      my_err(605,info);
      return 0;      // proste nedelam nic a pokracuji v dalsim prikazu                               
    }

   //vymena retezce (pomoci funkce my_str_vymena)
    int nalezen=0; 
    int pocet=0;
    switch(seznam[0]){
           case 's': // s (varianty PATTERN REPLACEMENT) JEN JEDNOU
              my_str_vymena(text_new,pattern,replacement); 
              break;
           case 'S': // PATTERN REPLACEMENT - nahrazuje vsechny vyskyty PATTERN
              while (nalezen>=0){
                  nalezen= my_str_vymena(text_new,pattern,replacement); 
                  pocet++;
                  if (pocet > RADEK){nalezen=-1;} // konec v pripade zacykleni
              }
           break; 
    }   
                                        
return 0;
}


int my_je_cislo(char *content){   // zjisti, jestli je string cislo a pokud ano, vrati cislo, jinak vrati -1

   if (strlen(content)>0) 
   {  
    long cislo2 = 0;
    char *znaky;
    cislo2 = strtol(content, &znaky, 10);
    if(strlen(znaky)>0){
      return -1;
    }
    else {
      return cislo2;
      if(cislo2 == 0){
        return 1;
      }
    }
   }             
return 0; 
}


char *my_append(char *text){  // funkce pro append  znaku \n

   char text_old[RADEK] = ""; //pracovni promenna
   char text_tmp[RADEK] = ""; //pracovni promenna
   strcpy(text_old,text);
   sprintf(text_tmp,"%s%s",text_old,"\n");       
   strcpy(text,text_tmp);
         
return text; 
}


int my_insert(char *seznam){  // funkce pro iContent

   char content[RADEK]=""; 
   strcpy(content,&seznam[1]); 
   
   my_append(content);   // Pridam koncove /n   
   my_output(content);   // a vytiskneme
return 0; 
}


int my_after(char *text, char *seznam){  // funkce pro aContent

   char text_old[RADEK] = ""; //pracovni promenna
   char content[RADEK]=""; 

   strcpy(text_old,text);   
   strcpy(content,&seznam[1]); 
   my_text_bezn(text_old);   
   my_append(content);      
   sprintf(text_new,"%s%s",text_old,content);  
return 0; 
}


int my_before(char *text, char *seznam){  // funkce pro bContent

   char content[RADEK]="";   
   char text_old[RADEK] = ""; //pracovni promenna

   strcpy(content,&seznam[1]); 
   strcpy(text_old,text); 
   sprintf(text_new,"%s%s",content,text_old);      
return 0; 
}


int my_delete(char *seznam){  // funkce pro delete

   char content[RADEK]="";  
   strcpy(content,&seznam[1]); // DELETE muze obsahovat prikaz pro vic radku
   int delete=1; 
   
   if (strlen(content)!=0) // provedeme kontrolu, zda obsahuje cislo (a ne nejake napr. abc)
   {       
     delete=my_je_cislo(content);
     if (delete>=1)  { }
     else  {
      delete=1;
      my_err(606,"Prikaz obsahuje chybne zadany pocet vymazu radku! Bude proveden pouze vymaz jednoho radku!"); }
   }        
return delete; 
}


int my_next(char *seznam){  // funkce pro next

   char content[RADEK]="";  
   strcpy(content,&seznam[1]); // NEXT muze obsahovat prikaz pro vic radku
   int next=1;                                             
           
   if (strlen(content)!=0) // provedeme kontrolu, zda obsahuje cislo (a ne nejake napr. abc)
   {       
     next=my_je_cislo(content);
     if (next>=1) {} // OK
     else { 
      next=1;
      my_err(607,"Prikaz obsahuje chybne zadany pocet posunu radku! Bude proveden posun pouze o jeden radek!");}
   }             
return next; 
}


int my_gotox(char *seznam, int pocet_prikazu){   // vrací cislo prikazu na ktery se ma skocit, jinak -1 
 
   char content[RADEK]="";  
   strcpy(content,&seznam[1]); 
   int cislo=-1;
      
   // A. Nejprve zkontroluji, zda prikaz obsahuje cislo
   if (strlen(content)!=0){  // provedeme kontrolu, zda obsahuje cislo (a ne nejake napr. abc)         
     cislo=my_je_cislo(content);
       if (cislo>=1) {} // OK
       else{ my_err(608,"Prikaz obsahuje chybne zadany pocet v prikazu GOTOX! Prikaz je ignorovan!");
        return -1; }              
   } 
   else  {my_err(608,"Prikaz obsahuje chybne zadany pocet v prikazu GOTOX! Prikaz je ignorovan!"); 
        return -1;}  
 
  if (cislo > pocet_prikazu){
       my_err(609,"Prikaz GOTOX obsahuje cislo vetsi nez je pocet prikazu! Prikaz je ignorovan!"); 
       return -1;}
  if (cislo < 1){
       my_err(610,"Prikaz GOTOX obsahuje neplatne cislo! Prikaz je ignorovan!"); 
       return -1;}  
return cislo; 
}


int my_default(char *seznam){  // nalezena neznama hodnota v radku prikazu

   char info[RADEK]="";
   // proste nedelam nic a nactu dalsi radek textu z originalniho zdroje   
   sprintf(info,"Soubor s prikazy obsahuje neznamy prikaz (%s)! Bude ignorovan.",seznam);    
   my_err(611,info);           

return 0; 
}


int my_editor(int pocet_prikazu){

  int porovnani = 0;
  int posledni = 0;       //pomocna promenna, uplatni se pri testu tisku posledniho rozpracovaneho zanamu, 1=zaznam vytisnen
  int quit=0;
  int next=0;
  int delete=0;
  int i = 1;               // pomocne pocitadlo, odpovida poradovemu
  int zpracovavam=1;       // poradove cislo prave zpracovavaneho prikazu
  int cislo_gotox=0;
  char posledni_prikaz= ' ';
  int byl_jsem_tady[PRIKAZY]={0};  // pole s priznaky, ze je Gecko prislusneho radku prikazu opakovano (pro stejny radek textu)
  int find=0;               //pro fPATTERN
  char f_pattern[RADEK]="";  // pro prikaz cX (konec prikazu)
  char cx[RADEK]="";         // pro prikaz cX (zacatek prikazu)
  char text_orig[RADEK] = "";

   while (fgets(text_new, sizeof(text_new), stdin) != NULL)  // vypise stream vcetne prazdnych radku, avsak i vcetne koncoveho "\n".
    {
      i=1; // zacinam s pocitadlem zase znovu 

      //ulozeni puvodniho tvaru do text original

      strcpy(text_orig, text_new);


      // Inicializace promenne pro kontrolu zacykleni (pri nacteni noveho textu je nastaveno na nulu)
      for (int j = 0; j <= pocet_prikazu; ++j) {byl_jsem_tady[j]=0;  }   
        
        // A. Detekovano Delete:
        if (delete>1) // jednou se radek jiz zahodil (bez odeslani na stdout) v ramci detekce n v cyklu for
        {
            delete = delete -1;
            continue;
        }
        delete=0;
        
        // B. Detekovano Next
        // po nacteni radku textu zatim dalsi nacist nechci (dokud nezpracuji prikazy).
        // Neplati v pripade, ze zpracovavam prikaz nPocet
        if (next>1) // jednou se to jiz vypsalo v ramci detekce n v cyklu for
        {
            next = next -1;
            my_output(text_new);
            continue;
        }
        next=0;

        // C. Detekovano Find, tj. hledam radek, kde je dany vyraz
         if (find>0){                   
             if (strstr(text_new,f_pattern) == 0) // hledany vyraz v radku textu neni, zkusim dalsi radek             
             {
              my_output(text_new);
              continue;}              
         }
        find=0;
        // D1. Pokud vsechny prikazy jiz precteny, tak se ostatni nemodifikovane zaznamy poslou na stdout
        if ((posledni_prikaz != 'g') && ((zpracovavam -1) == pocet_prikazu)){
            my_output(text_new);
        }
       
        for(i = zpracovavam; i<= pocet_prikazu; i++)
        {
          if (quit==1)  {break;}   // pokud q, tak vyskocim z cyklu FOR
          if (delete>0) {break;}   // pokud d, tak vyskocim z cyklu FOR
          if (next>0)   {break;}   // pokud n, tak vyskocim z cyklu FOR 
          if (find>0)   {break;}   // pokud f, tak vyskocim z cyklu FOR         
          posledni_prikaz = seznam[i][0];
         
          switch(seznam[i][0])
          {
          case 'i': // iCONTENT (insert) vloží pøed aktuální øádek øádek s obsahem CONTENT.
                my_insert(seznam[i]); //vlozi novy radek
                //next = 1;  //posun na dalsi radek
                break;                                                 
          case 'a': // aCONTENT (after/append) vloží na konec aktuálního øádku obsah CONTENT.
                my_after(text_new, seznam[i]);
                break;                                                  
          case 'b': // bCONTENT (before) vloží na zaèátek aktuálního øádku obsah CONTENT.
                my_before(text_new, seznam[i]);
                break;
          case 'q': // q (quit) ukonèí editaci, další øádky nezpracovává a ani netiskne.
                quit=1;
                porovnani = strcmp(text_new, text_orig);

                //my_output(text_new);
                break;
          case 'd': // d (delete) smaže aktuální øádek nebo smaze vic radku
                delete=my_delete(seznam[i]);
                break;
          case 'n': // n (next) posun aktuálního øádku o jeden níž, tj. tiskne aktuální øádek nebo vic radku                  
                next = my_next(seznam[i]);
                my_output(text_new);                                 
                break;
          case 'r': // r (remove EOL) na aktuálním øádku odstraní znak konce øádku.
                my_text_bezn(text_new);                                  
                break;
          case 's': // s (varianty PATTERN REPLACEMENT)                   
                my_text_replace(seznam[i]);                              
                break;
          case 'S': // PATTERN REPLACEMENT - nahrazuje vsechny vyskyty PATTERN
                my_text_replace(seznam[i]);    // stejna fce jako u 's' (vetvi se uvnitr)
                break;
          case 'e': // e (append EOL  pøidej na konec aktuálního øádku znak konce øádku.
                my_append(text_new);                                     
                break;
          case 'g': // gX   s aktuálním pøíkazem pro editaci na X-tý pøíkaz.                                                                              
               // g.0. Kontrola zacykleni :
                if (byl_jsem_tady[i] < 1) {byl_jsem_tady[i]=byl_jsem_tady[i] + 1;}
                else {my_err(612,"Bylo detekovano zacykleni. Program je ukoncen.");
                       quit=1;} 
                // g.1. Vlastni nastaveni skoku na jiny prikaz:
                cislo_gotox = my_gotox(seznam[i], pocet_prikazu);    // zkontroluje cislo, na ktere se ma skocit         // GGG                 
                if (cislo_gotox>0){
                    i=cislo_gotox-1;                 // Hodnota -1 je tam proto, protoze pred navratem do cyklu for (na jeho konci) dochází k i++ automaticky
                    zpracovavam=cislo_gotox-1;}     //  Tady taky je minus jedna (protoza na konci cyklu for je zpracovavam++)                                                               
                break; 
          case 'f': // fPattern (find) posun aktuálního øádku smìrem dolù na øádek obsahující øetìzec PATTERN.                  
                strcpy(f_pattern,&seznam[i][1]);
                my_output(text_new);  
                find=1;                                         
                break;  
          case 'c': // cX Pattern (conditioned goto)                             
                
                // c.1. zkontroluji parametry prikazu:
                sscanf (&seznam[i][0],"%s %s",cx,f_pattern); // ssanf to mezi sebou oddeli podle mezery mezi X a pattern
                if (strstr(text_new,f_pattern) == 0)   // hledany vyraz v radku textu neni, prikaz je ignorovan
                {break;}    
                // c.2. Vlastni nastaveni dalsiho skoku:
                cislo_gotox=my_gotox(cx, pocet_prikazu);           // zkontroluje cislo, na ktere se ma skocit
                if (cislo_gotox>0){
                  i=cislo_gotox-1;                 // Hodnota -1 je tam proto, protoze pred navratem do cyklu for (na jeho konci) dochází k i++ automaticky
                  zpracovavam=cislo_gotox-1;}     //  Tady taky je minus jedna (protoza na konci cyklu for je zpracovavam++)
                  posledni_prikaz = 'g';                 //v tuto chvili se jedna o obdobu prikazu g         
                  // c.3. Kontrola zacykleni :
                if (byl_jsem_tady[i] < 1) {byl_jsem_tady[i]=byl_jsem_tady[i] + 1;}
                else {my_err(612,"Bylo detekovano zacykleni. Program je ukoncen.");
                       quit=1;}                                 
                break;                                         
          default:
                my_default(seznam[i]);
                break;
        } // konec switch   
          zpracovavam++;                                                        
      }   // konec cyklu for

       //D2. Pokud vsechny prikazy jiz precteny, tak se posledni rozpracpvany zaznam posle na stdout
        if ((posledni == 0) && (posledni_prikaz != 'g') && (posledni_prikaz != 'd') 
          && (posledni_prikaz != 'n') && (posledni_prikaz != 'f')
          && ((zpracovavam -1) == pocet_prikazu)){
            if(posledni_prikaz=='q' && porovnani == 0){}
              else {my_output(text_new);}

            
            posledni = 1;
        }

      // E. Pokud bylo detekovano quit: tak konec cteni text a konec cteni prikazu, tj. RETURN zpìt do main
      if (quit==1)
         {return 0;}
         
    } // konec while
return 0;
}


int main(int argc, char *argv[]){

   int konec=0; // 0=pokracuj, nenulova=KONEC. Pomocna promenna, v ktere si testuji, zda volat RETURN v main
   int pocet_prikazu=0;

/* A. Kontrola poctu argumentu */
   konec = my_arg_count(argc,2);  
   if (konec != 0) {return konec;}       
    
/* B. Nacteni souboru s prikazy, tj. JMENO_SOUBORU_S_PRIKAZY  do seznamu prikazu  */    
   pocet_prikazu = my_seznam_prikazu(argv[1]);  
   if (pocet_prikazu == 0) {return konec;} 
 
/* C. Nacteni radku vstupu (textu ) z stdin a  poslani vystupu na stdout */ 
   konec = my_editor(pocet_prikazu);  
   if (konec != 0) {return konec;} 
          
return 0;
}
