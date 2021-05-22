
# Mikroprocesorové a vestavěné systémy

## [ Měření výpočetních režií modulem Data Watchpoint and Trace (DWT)](https://github.com/miricinka/VUT_FIT/tree/master/IZV/projekt_1_cast)
* **body**: 13.8/14
* **Komentář učitele k hodnocení:**  
 <pre>
 Chvályhodné nadšení pro řešení projektu, snaha řešit projekt včas, kvalitně a/nebo nad rámec vymezený zadáním ( 1 z 1 b.).
 Řešení je zcela funkční ( 5 z 5 b. ). 
 Dekompozice řešení je na velmi dobré úrovni ( .85 z 1 b. ).
 Způsob řešení je zcela pochopitelný a čitelný ( 1 z 1 b. ).
 Uživatelská přívětivost řešení je na vysoké úrovni ( 1 z 1 b. ). 
 Úvod do řešené problematiky (motivace, přehled použitých HW/SW prostředků 
 (moduly a jejich registry, knihovny a jejich API atp.), s tématem/řešením 
 souvisejících principů, metod atd.) v dokumentaci je zpracován chvályhodně ( 1 z 1 b. ). 
 Popis způsobu řešení v dokumentaci je zpracován chvályhodně ( 2 z 2 b. ). 
 Zhodnocení řešení v dokumentaci je provedeno velmi kvalitně ( .9 z 1 b. ). 
 Prezentace řešení byla chályhodná ( 1 z 1 b. ).  
  
Pozn.: výsledné hodnocení ( 13.8 ) vzniklo zaokrouhlením součtu dílčích, výše uvedených,
bodových hodnocení (výsledný součet je 13.75), násobeným ohodnocením míry funkčnosti řešení
(míra je K1 + K2 * 5 / 5 [kde K1=0.25, K2=0.75 jsou konstanty hodnoticího algoritmu] = 1).  
  
Příkladné řešení - prakticky nemám připomínky. Kód mohl být lépe dekomponován, např. v main().
</pre>
**zadání**:
-   Seznamte se s principem tvorby vestavných aplikací v jazyce C založených na mikrokontroléru  **Kinetis K60** (s jádrem ARM Cortex-M4) fy Freescale v prostředí  **Kinetis Design Studio (KDS)** nebo **MCUXpresso**.
-   V jazyce C (tak i dále) vytvořte  **projekt demonstrující možnosti měření výpočetních režií pomocí modulu Data Watchpoint and Trace (DWT)**  dostupného na mikrokontroléru Kinetis K60 z desky platformy FITkit 3.  **UPŘESNĚNÍ: 1)** Připravte si několik úseků programů - alespoň jeden úsek pro každou z konstrukcí typu sekvence, selekce, iterace (iteraci omezte na konečný počet cyklů) a dva algoritmy ze zvolené třídy algoritmů (např. řadicích - bubblesort, qicksort) - ve variantách bez přerušení/výjimek a s přerušením s různými prodlevami mezi přerušeními.  **2)** S využitím modulu DWT opakovaně odměřte režie (zejm. počet cyklů CPU, cyklů CPU strávených ve spánku a cyklů CPU strávených zpracováním přerušení) spojené s prováděním každého z úseků.  **3)** Výsledky opakovaných měření vhodně shrňte (min., max., průměr apod.) a interpretujte.
-   Vytvořte přehlednou **dokumentaci** k přípravě, způsobu realizace, k funkčnosti a vlastnostem řešení projektu.
