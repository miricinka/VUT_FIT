# Projekt 1 do předmětu IPK 2019/2020

## Autor
Mirka Kolaříková, <xkolar76@stud.fit.cz>


## Zadání

Server, který komunikuje s protokolem HTTP a zajišťuje překlad doménových jmen. Podporovány jsou dvě operace:
**GET** = zpracování jednoho dotazu 
**POST** = zpracování více dotazů

## Moje řešení

Server je implementován v jazyce Python. Použité knihovny jsou *socket*, *sys* a *re*. 
Server je vytvořen pomocí základních API funkcí:
-   `socket()`
-   `bind()`
-   `listen()`
-   `accept()`
-   `connect()`
-   `send()`
-   `close()`

Klientova zpráva je po přijetí dekódována, a jsou zkontrolovány její formální požadavky.  V případě chybného požadavku je vracena chyba *400 Bad Request*, nebo *405 Method Not Allowed*.

K získání adresy z IP adresy je použita funkce **gethostbyaddr()**, obdobně pak k získání IP adresy z adresy je volána funkce **gethostbyname()**. V případě chybné nebo neexistující adresy či IP adresy je vracena chyba *404 Page Not Found*. V případě nalezení odpovědi je výsledek *200 OK*. Na kontrolu formátu IP adresy je použit regulární výraz.

V případě hromadného zpravocání dotazů (operace **GET**), jsou chybné dotazy ignorovány. Je-li alespoň jeden dotaz správný, je výsledek *200 OK*. V jiném případě je výsledek *404 Bad request*. 