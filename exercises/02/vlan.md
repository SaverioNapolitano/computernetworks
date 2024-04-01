# VLAN

Dopo aver aperto il file `vlan.mar` e cliccato su `Start all`, è possibile notare come tutto sia già stato configurato, controllando su ogni macchina i file `/etc/network/interfaces` e `etc/hosts`:
```
h1$ nano /etc/network/interfaces
h1$ nano /etc/hosts

h2$ nano /etc/network/interfaces
h2$ nano /etc/hosts

h3$ nano /etc/network/interfaces
h3$ nano /etc/hosts

h4$ nano /etc/network/interfaces
h4$ nano /etc/hosts
```

Per verificare il corretto funzionamento delle vlan, eseguire il comando
```
h1$ arping h2
```
E notare come esso vada in `Timeout`. È anche possibile osservare, eseguendo il comando
```
h2$ tcpdump
```
Come l'host `h2` non riceva la query ARP. Ciò accade perché `h1` e `h2` sono su due **vlan diverse**.

Se invece si esegue il comando
```
h1$ arping h3
```
Si vedrà come la comunicazione avvenga normalmente, essendo `h1` e `h3` sulla **stessa vlan**.

Per vedere e/o modificare lo script con cui vengono inizializzati gli switch, all'interno di marionnet, cliccare sull'icona dello switch nella colonna di sinistra, cliccare su `Modify`, cliccare sul nome dello switch (es. `s1`) e, nella nuova schermata che compare, cliccare su `Edit` di fianco a `Startup configuration`. A questo punto sarà possibile visualizzare lo script commentato di configurazione delle vlan (in questo caso lo script è simmetrico rispetto ai due switch).

**NB: Assicurarsi, nella schermata di modifica dello switch, che siano spuntate le caselle corrispondenti a `Show VDE terminal` e `Startup configuration`.**

Eseguendo sul terminale corrispondente allo switch `s1` il comando
```
vde$ vlan/print
```
Sarà possibile avere conferma della modalità con cui le vlan sono associate alle porte corrispondenti.