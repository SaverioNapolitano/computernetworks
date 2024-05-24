
# vde_switch

| comando                            | funzione                                                                                                                                          |
| ---------------------------------- | ------------------------------------------------------------------------------------------------------------------------------------------------- |
| `help [command]`                   | dare informazioni sul comando specificato (`command`), se non specificato nessun comando fornisce informazioni su tutti i comandi                 |
| `vlan/create <vlan_id>`            | creare la VLAN con ID `<vlan_id>` (numero) sullo switch                                                                                           |
| `port/setvlan <port_id> <vlan_id>` | impostare la VLAN con ID `<vlan_id>` (numero) in modalità **port-based** (*untagged*, quindi con un *access-link*) sulla porta numero `<port_id>` |
| `vlan/addport <vlan_id> <port_id>` | impostare la VLAN con ID `<vlan_id>` (numero) in modalità **tagged-based** (quindi con un *trunk-link*) sulla porta numero `<port_id>`            |
| `vlan/print`                       | mostrare informazioni sulle VLAN, ad esempio se il link è di tipo *trunk* (`tagged=1`) o *access* (`tagged=0`)                                    |
| `hash/print`                       | mostrare il MAC address associato a ogni porta                                                                                                    |
