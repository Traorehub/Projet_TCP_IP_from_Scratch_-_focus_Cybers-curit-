# Jour 11 : chemin de sortie

Binaire : `tcpstack`. A lancer sur Kali (`/home/kali/jour11`).

## Ce qui change par rapport au Jour 10

- SYN-ACK inverse IP **et** MAC (casse depuis J08)
- `arp_init` / `tcp_init`
- checksum TCP en entree : drop si invalide
- pas de TCB si aucun socket n’ecoute le port (`xbind`)
- RST entrant : SEQ doit egaler RCV.NXT

## Transfert depuis Windows

```powershell
ssh kali@192.168.126.129 "mkdir -p /home/kali/jour11"
scp -r .\Jour11_Chemin_Sortie\* kali@192.168.126.129:/home/kali/jour11/
```

## Run

```bash
cd /home/kali/jour11
make
sudo ./tcpstack
```

Terminal 2 :

```bash
sudo ip addr add 10.0.0.1/24 dev tap0 2>/dev/null || true
sudo ip link set tap0 up
ping -c 3 10.0.0.2
printf 'Salut\n' | nc -N 10.0.0.2 80
# port sans bind -> RST
nc -N 10.0.0.2 81
sudo tcpdump -i tap0 -nn -e 'tcp or icmp or arp'
```

Captures a prendre (et a coller plus tard dans `docs/readme/` si le test passe) :

1. `ping` 3/3
2. tcpdump du SYN-ACK : src `10.0.0.2` / `00:11:22:33:44:55`
3. `nc` envoie `Salut`, log `APPLICATION USER-SPACE`
4. `nc` port 81 : RST, pas de TCB
