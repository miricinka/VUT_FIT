
# projekt do předmětu Síťové aplikace a správa sítí
### Rozšíření SNMP agenta
### Autor: Mirka Kolaříková <xkolar76@stud.fit.vutbr.cz>
### Popis 
Rozšíření agenta o modul isa-project-module, který definuje 4 nové objekty:
- .1.3.6.1.3.22.1: read-only string obsahující login xkolar76
- .1.3.6.1.3.22.2: read-only string, který vrátí aktuální UTC čas naformátovaný podle RFC 339
- .1.3.6.1.3.22.3: read-write integer, defaultní hodnota je 666
- .1.3.6.1.3.22.4:  read-only string obsahující typ operačního systému


### Návod na použití
1. MY-MIB.txt dáme do složky /usr/share/snmp/mibs a načteme ji do terminálu pomocí export MIBS="+MY-MIB" a zdrojové soubory přeložíme pomocí příkazu *make*
2. V prvním okně spustíme daemon pro SNMPD agenta s debugovacími přepínači:
    ```
    $ sudo snmpd -f -L -DnstAgentPluginObject,dlmod
    ```
3. V druhém okně pomocí příkazu snmpset vytvoříme nový řádek v dlmod tabulce:
     ```
    $ snmpset localhost UCD-DLMOD-MIB::dlmodStatus.1 i create
    ```
4. Vlastnosti řádku nastavíme tak, aby ukazoval na náš objekt a dáme mu jméno (upravit paht/to/file):
     ```
    $ snmpset localhost UCD-DLMOD-MIB::dlmodName.1 s "isa_project_module" UCD-DLMOD-MIB::dlmodPath.1 s "/path/to/file/isa_project_module.so"
    ```
5. Načteme shared object soubor do běžícího agenta:
     ```
    $ snmpset localhost UCD-DLMOD-MIB::dlmodStatus.1 i load
    ```

### Příklady použití
- snmpget localhost  MY-MIB::operatingSystem.0
- snmpget localhost  MY-MIB::timeString.0
- snmpget localhost  .1.3.6.1.3.22.1.0
- snmpset localhost MY-MIB::readwriteInt.0 i 89
- snmptranslate -On MY-MIB::loginString.0
- snmptranslate .1.3.6.1.3.22.2

### Odevzdané soubory
- `Makefile`
- `README`
- `Manual.pdf`
- `isa_project_module.c`
- `isa_project_module.h`
- `MY-MIB.txt`
