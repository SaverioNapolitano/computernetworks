# File di configurazione

| percorso                  | funzione                                                                                                                                                                                                                                          |
| ------------------------- | ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------- |
| `/etc/hosts`              | contiene i nomi degli host associati ai rispettivi indirizzi IP, serve per assegnare in modo permanente l'hostname della macchina che si sta configurando, ogni conflitto viene risolto dando la precedenza alla **prima** corrispondenza trovata |
| `/etc/network/interfaces` | serve per configurare permanentemente le interfacce di rete del sistema                                                                                                                                                                           |
| `/etc/sysctl.conf`        |                                                                                                                                                                                                                                                   |
| `/etc/services`           | contiene le well-known ports                                                                                                                                                                                                                      |
| `/etc/dnsmasq.conf`       | serve per configurare il server DHCP                                                                                                                                                                                                              |
| `/etc/resolv.conf` | serve per configurare il DNS |
## Esempio di configurazione dei file

### Configurazione DNS

```
# file di configurazione /etc/resolv.conf

nameserver <iface_IP>
```

Dove `<iface_IP>` è l'indirizzo IP dell'interfaccia del DNS server con cui comunica l'host.

### Configurazione di una interfaccia Ethernet fisica

```
# file di configurazione /etc/network/interfaces

auto <interface>
iface <interface> inet static
	address <ip_addr>
	...
	post-up <command>
```

| elemento           | significato                                                                                                      |
| ------------------ | ---------------------------------------------------------------------------------------------------------------- |
| `auto <interface>` | quando avvio la macchina voglio che la configurazione per l'interfaccia `interface` sia automaticamente caricata |
| `inet`             | voglio configurare l'interfaccia secondo lo standard IPv4                                                        |
| `iface <mode>`     | specifico la modalità (`mode`) con cui voglio configurare l'interfaccia                                          |

| modalità          | significato                                                                                                                                                                                                                                                                      |
| ----------------- | -------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------- |
| `dhcp`            | inizializzazione dell'interfaccia in modo automatico col protocollo dhcp (non servono altri parametri)                                                                                                                                                                           |
| `loopback`        | interfaccia locale (127.0.0.1) per far comunicare processi sulla stessa macchina (passare per la scheda di rete ogni volta degrada le prestazioni), ha una MTU di 64K perché sulla stessa macchina non si possono avere conflitti (si ha quindi il massimo throughput possibile) |
| `static <params>` | voglio configurare l'interfaccia in modo statico (seguono i parametri `<params>`)                                                                                                                                                                                                |

| parametro                       | significato                                                                                                                                                                                                           |
| ------------------------------- | --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------- |
| `address <ip_addr>[/<netmask>]` | specifico che all'interfaccia verrà associato come indirizzo IP `<ip_addr>`, opzionalmente posso esplicitare la netmask sotto forma di numero di bit che la costituiscono (`netmask` è un numero che varia da 0 a 32) |
| `network <network>`             | specifico l'indirizzo della rete (`<network>`)                                                                                                                                                                        |
| `broadcast <broadcast>`         | specifico l'indirizzo di broadcast `<broadcast>`                                                                                                                                                                      |
| `gateway <ip_addr_gateway>`     | specifico il **default gateway**, da utilizzare nel caso la configurazione di rete lo richieda                                                                                                                        |
| `post-up <command>`             | specifico un comando (`<command>`) da eseguire ogni volta che la scheda è stata configurata                                                                                                                           |

### Configurazione di una VLAN

```
# file di configurazione /etc/network/interfaces

auto <interface>.<N>
iface <interface>.<N> inet static
	address <ip_addr>/<netmask>
	gateway <ip_addr_gateway>
```

`<interface>` è la scheda di rete fisica, `<N>` è il VLAN ID della scheda di rete virtuale
### Configurazione dei nomi degli hosts

```
# file di configurazione /etc/hosts

127.0.0.1   localhost
192.168.1.1 h1
192.168.1.2 h2
```

### Configurazione server DHCP

```
# file di configurazione /etc/dnsmasq.conf

no-resolv
read-ethers
interface=<iface>
domain=reti.org
dhcp-option=option:router,<ip_addr_router>
dhcp-option=option:dns-server,<ip_addr_dns_server>
dhcp-range=<min_IP>,<max_IP>,<lease_time>
dhcp-host=<mac_addr>,<hostname>,<ip_addr>,<lease_time>
address=/<site_name>/<ip_addr>
```

| elemento                                                 | significato                                                                                                                         |
| -------------------------------------------------------- | ----------------------------------------------------------------------------------------------------------------------------------- |
| `no-resolv`                                              | non mi interessa specificare altri nameservers/non cercare altri nameservers                                                        |
| `read-ethers`                                            | leggi il file `/etc/ethers`                                                                                                         |
| `interface=<iface>`                                      | interfaccia su cui è in ascolto/interfaccia di rete per DHCP                                                                        |
| `domain=<name>`                                          | specificare dominio implicito (aggiunto in coda se non scritto direttamente)                                                        |
| `dhcp-option`                                            | aggiungere parametri di configurazione nella risposta                                                                               |
| `dhcp-range=<min_IP>,<max_IP>,<lease_time>`              | range di indirizzi che posso assegnare dinamicamente + lease time                                                                   |
| `dhcp-host=<mac_addr>,<hostname>,<ip_addr>,<lease_time>` | indico la configurazione statica per i nodi che la richiedono (in alternativa si possono usare i file `/etc/ethers`ed `/etc/hosts`) |
| `address`                                                | sovrascrivo l'indirizzo (si può usare anche `/etc/hosts`)                                                                           |
| `dhcp-option=option:router,<ip_addr_router>`             | specifico il default gateway                                                                                                        |

### Configurazione interfaccia di rete DHCP

#### MAC non noto

```
file di configurazione /etc/network/interfaces

auto <interface>
iface <interface> inet dhcp
```

#### MAC noto

```
file di configurazione /etc/network/interfaces

auto <interface>
iface <interface> inet dhcp
	hwaddress ether <mac_address>
```

