# Projekt 2 do předmětu IPK 2019/2020

Autor: Mirka Kolaříková

## Packet sniffer
Síťový analyzátor v C, který na určitém síťovém rozhraní zachytává a filtruje pakety.


## Volání programu

./ipk-sniffer -i _rozhraní_ [-p _port_] [--tcp|-t] [--udp|-u] [-n _num_]

-   -i  _eth0_  (rozhraní, na kterém se bude poslouchat. Nebude-li tento parametr uveden, vypíše se seznam aktivních rozhraní)
-   -p  _23_  (bude filtrování paketů na daném rozhraní podle portu; nebude-li tento parametr uveden, uvažují se všechny porty)
-   -t nebo --tcp (bude zobrazovat pouze tcp pakety)
-   -u nebo --udp (bude zobrazovat pouze udp pakety)
-   Pokud nebude -tcp ani -udp specifikováno, uvažují se TCP a UDP pakety zároveň
-   -n  _10_  (určuje počet paketů, které se mají zobrazit; pokud není uvedeno, uvažujte zobrazení pouze 1 paket)

## Příklady spuštění

$ ./ipk-sniffer -i enp0s3  --tcp -n 2  
$ ./ipk-sniffer 
$ ./ipk-sniffer -i enp0s3 -n 10  -u
$ ./ipk-sniffer -i enp0s3 -n 10  --tcp

## Odevzdané soubory
ipk-sniffer.c
Makefile
README
manual.pdf
