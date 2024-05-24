
# DHCP

| comando                    | funzione                         |
| -------------------------- | -------------------------------- |
| `systemctl enable dnsmasq` | avviare il server all'accensione |
| `service dnsmasq start`    | lanciare il server               |

# Traffic shaping

| comando                                                                                                                                            | funzione                                                                    | opzioni                                                                                                                                                                                                    |
| -------------------------------------------------------------------------------------------------------------------------------------------------- | --------------------------------------------------------------------------- | ---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------- |
| `tc [-s] qdisc show dev <iface>`                                                                                                                   | visualizzare le regole qdisc dell'interfaccia                               | `-s`: mostra anche le statistiche                                                                                                                                                                          |
| `tc {qdisc, class} add dev <iface> root [handle <handle>] <qdisc-type> <qdisc-options>`                                                            | impostare la root qdisc (`qdisc`) o classe (`class`)                        | `default <minor>`: indica di usare come default la disciplina che ha come *minor number* `<minor>`<br>`limit <limit>`: permette di accodare massimo `<limit>` pacchetti (per es. nelle discipline `pfifo`) |
| `tc {qdisc, class} add dev <iface> parent <class-handle> [classid <class-id>] <qdisc-type> <qdisc-options>`                                        | aggiungere una qdisc (`qdisc`) o una classe (`class`) a un handle esistente |                                                                                                                                                                                                            |
| `tc filter add dev <iface> protocol <proto> parent <qdisc-handle> <filter-type> match <proto-rule> flowid <class-handle>`                          | aggiungere un filtro (es. `<filter-type> = u32`)                            |                                                                                                                                                                                                            |
| `tc filter show dev <iface>`                                                                                                                       | mostrare i filtri dell'interfaccia                                          |                                                                                                                                                                                                            |
| `tc filter del dev <iface>`                                                                                                                        | eliminare un filtro                                                         |                                                                                                                                                                                                            |
| `tc qdisc del dev <iface> root`                                                                                                                    | eliminare la root qdisc                                                     |                                                                                                                                                                                                            |
| `tc qdisc del dev <iface> handle <handle>`                                                                                                         | eliminare la qdisc corrispondente ad un handle                              |                                                                                                                                                                                                            |
| `tc {qdisc, class} add dev <iface> {root, parent <handle>} [classid <class-id>] tbf rate <rate> [ceil <max-rate>] burst <burst> latency <latency>` | configurare una qdisc di tipo tbf                                           |                                                                                                                                                                                                            |

## Classificazione qdisc

| classless         | classful |
| ----------------- | -------- |
| `pfifo_fast`      | `prio`   |
| `tbf`             | `cbq`    |
| `sfq`             | `htb`    |
| `red`             | -        |
| `codel, fq_codel` | -        |
