# Jour 11 : chemin de sortie

Binaire `tcpstack`. Les adresses TAP (`10.0.0.1` / `10.0.0.2`) sont celles des
comptes rendus precedents ; elles se changent dans le code et via `ip`.

## Modifications par rapport au jour 10

- inversion des adresses IP et MAC lors de l'emission du SYN-ACK
- appels a `arp_init` et `tcp_init`
- verification du checksum TCP a l'entree (paquet ignore si invalide)
- allocation d'un TCB seulement si un socket ecoute le port
- RST entrant accepte si SEQ est egal a RCV.NXT

## Compilation

```bash
make
sudo ./tcpstack
```

Transfert (remplacer `USER` et `HOST`) :

```bash
scp -r Jour11_Chemin_Sortie USER@HOST:~/jour11/
```

Second terminal, cote hote TAP :

```bash
sudo ip addr add 10.0.0.1/24 dev tap0 2>/dev/null || true
sudo ip link set tap0 up
ping -c 3 10.0.0.2
printf 'Salut\n' | nc -N 10.0.0.2 80
nc -N 10.0.0.2 81
```
