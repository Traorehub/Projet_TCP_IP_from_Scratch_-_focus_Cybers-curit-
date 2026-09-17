# TCP/IP from scratch

Pile TCP/IP userspace en C, sur une interface TAP Linux. Reference :
[saminiir/level-ip](https://github.com/saminiir/level-ip).

Les dossiers sont numerotes `Jour01` ... `Jour11` (**deux chiffres**) pour que
GitHub les affiche dans l'ordre : `Jour11` vient **apres** `Jour10`, jamais
avant `Jour02`.

Travail en cours : **[Jour 11, chemin de sortie](./Jour11_Chemin_Sortie)**
(`tcpstack`).

## Resultats experimentaux (Kali, `tap0`, `10.0.0.2`)

Les figures ci-dessous sont des extraits de sessions de laboratoire, pas des illustrations decoratives.

**Figure 1.** Creation de l'interface `tap0` et reception des premieres trames Ethernet.

![tap0 cree, premieres trames](docs/readme/01-tap0-opentap.png)

**Figure 2.** Echo ICMP : trois reponses recues depuis `10.0.0.2` (perte nulle).

![ping 10.0.0.2 reussi](docs/readme/04-ping-ok.png)

**Figure 3.** Etablissement TCP sur le port 80 : ARP, SYN, SYN-ACK, ACK, etat ESTABLISHED.

![handshake TCP ESTABLISHED](docs/readme/05-tcp-established.png)

**Figure 4.** Balayage `nmap -sS -p 80` : port ouvert, adresse MAC `00:11:22:33:44:55`.

![nmap port 80 open](docs/readme/06-nmap-port80.png)

**Figure 5.** Requete HTTP `GET /` recu par la couche socket (client `curl`).

![curl GET HTTP recu par la stack](docs/readme/10-curl-http.png)

## Jour 11 (a valider sur Kali)

Code : [`Jour11_Chemin_Sortie/`](./Jour11_Chemin_Sortie)

- SYN-ACK avec IP **et** MAC inversees (regression J08-J10)
- checksum TCP verifie en entree
- SYN refuse si `xbind` n'ecoute pas ce port
- RST accepte seulement si SEQ == RCV.NXT

Preuve attendue : `ping -c 3 10.0.0.2` et `nc 10.0.0.2 80`, SYN-ACK correct dans tcpdump.

## Envoi vers Kali (`192.168.126.129`)

Depuis la racine du depot, PowerShell :

```powershell
ssh kali@192.168.126.129 "mkdir -p /home/kali/jour11"
scp -r .\Jour11_Chemin_Sortie\* kali@192.168.126.129:/home/kali/jour11/
ssh kali@192.168.126.129
```

Sur la VM :

```bash
cd /home/kali/jour11
make
sudo ./tcpstack
```

Autre terminal Kali :

```bash
sudo ip addr add 10.0.0.1/24 dev tap0 2>/dev/null
sudo ip link set tap0 up
ping -c 3 10.0.0.2
printf 'Salut\n' | nc -N 10.0.0.2 80
sudo tcpdump -i tap0 -nn -e tcp
```

Les jours suivants iront dans `/home/kali/jour12`, etc. Meme padding dans le repo : `Jour12_...`.

## Parcours

| Dossier | Sujet |
|---|---|
| [Jour01_Ethernet_TAP](./Jour01_Ethernet_TAP) | TAP + Ethernet |
| [Jour02_ARP](./Jour02_ARP) | ARP |
| [Jour03_IP_ICMP](./Jour03_IP_ICMP) | IPv4 / ICMP |
| [Jour04_TCP_Handshake](./Jour04_TCP_Handshake) | SYN / SYN-ACK |
| [Jour05_TCP_Established](./Jour05_TCP_Established) | ESTABLISHED |
| [Jour06_TCP_Data_Transfer](./Jour06_TCP_Data_Transfer) | Donnees in-order |
| [Jour07_Socket_API](./Jour07_Socket_API) | API socket in-process |
| [Jour08_Retransmission_RTO](./Jour08_Retransmission_RTO) | RTO (esquisse) |
| [Jour09_Congestion_Control](./Jour09_Congestion_Control) | cwnd (non cable) |
| [Jour10_TCP_Termination](./Jour10_TCP_Termination) | FIN passif |
| [Jour11_Chemin_Sortie](./Jour11_Chemin_Sortie) | **en cours**, output path |

Jours 01-10 = archive. A partir de 11, un dossier = un increment testable, pas une copie cosmetique.
