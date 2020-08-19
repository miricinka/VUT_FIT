# Operační systémy

## [První projekt](https://github.com/miricinka/VUT_FIT/blob/master/IOS/proj1/dirgraph)
* **body**: 15/15
* [**zadání**](https://github.com/miricinka/VUT_FIT/blob/master/IOS/proj1/projekt1.pdf)
<pre>
#-- automaticke hodnoceni -----------------------------
1:ok: jednoduchy adresar
1:ok: jednoduche soubory
1:ok: spusteni bez parametru
2:ok: soubory s jednou velikosti
2:ok: soubory s ruznymi velikostmi
2:ok: normalizovany histogram delek
1:ok: prazdny adresar
1:ok: prazdny soubor
1:ok: skryte soubory
1:ok: skryte adresare
1:ok: filtr -i (BRE)
1:ok: filtr -i (ERE)
2:ok: komplexni priklad
1:ok: castecne zbesile nazvy
0:ok: docasne soubory
#------------------------------------------------------
18:celkove score (max pro hodnoceni 15)
15:celkem bodu za projekt
</pre>

## [Druhý projekt](https://github.com/miricinka/VUT_FIT/blob/master/IOS/proj2/proj2.c)
* **body**: 15/15
* [**zadání**](https://github.com/miricinka/VUT_FIT/blob/master/IOS/proj2/projekt2.pdf)
<pre>
#-- automaticke hodnoceni -----------------------------
= make
:ok:make
= prepare tests: resources
:kontrola syntaxe vystupu => check_syntax.out
= base_* : zakladni testy
:ok:test_a_base_counter: navratovy kod je 0
1:ok:test_a_base_counter
1:ok:test_b_base_judge: posloupnost JUDGE ok
1:ok:test_c_base_imm: posloupnost IMM ok
2:ok:test_d_base_sync: synchronizace
2:ok:test_e_base_syncnums: synchronizace
= rozsirene testy
:ok:test_g_counter: navratovy kod je 0
1:ok:test_g_counter
0:fail:test_h_judge: chyba v posloupnosti u JUDGE (vizte soubor JUDGE-1-*)
1:ok:test_i_imm: posloupnost IMM ok
2:ok:test_j_sync: synchronizace
= specialni testy
2:ok:test_n_sync_sleep: synchronizace (castejsi prepinani procesu)
2:ok:test_o_sync_nosleep: synchronizace (sleep -> 0)
= test spravneho ukonceni pri chybe
1:ok:test_q_error_1: osetreni chybneho vstupu
= resources
: pocet procesu ok (4, mel by byt 4)
: pocet volani wait (waitpid) ok
:ok: pripojeni ke sdilene pameti a uvolneni je korektni
:ok: korektni uvolneni pojmenovane semafory
#------------------------------------------------------
16:celkove score (max pro hodnoceni 15)
15:celkem bodu za projekt

</pre>
