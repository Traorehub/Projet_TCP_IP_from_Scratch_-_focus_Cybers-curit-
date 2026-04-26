# Jour 01 : Ethernet & Interface TAP (Bootcamp Cyber)

L'objectif de cette première journée est de créer la "porte d'entrée" de notre stack réseau : l'interface **TUN/TAP**. Dans le cadre de ce bootcamp cyber, nous analysons comment le trafic circule au niveau le plus bas du modèle OSI.

### 📄 Rapport d'Analyse Cyber
[Consulter le rapport détaillé du Jour 1 (PDF)](./Jour1_images.pdf)

### Concepts Clés (Niveau Bootcamp)
- **TUN/TAP** : Interface virtuelle. Vital pour le prototypage d'outils de sécurité.
- **Trames Brutes** : Accès direct aux headers Ethernet pour la détection d'intrusions (IDS) ou l'évasion de périmètre.

### Focus Sécurité & Analyse de Failles
L'interface TAP permet d'écouter tout le trafic sans être vu (Mode Promiscuité déguisé).
- **VULNÉRABILITÉ** : Si une application peut lire `/dev/net/tun`, elle peut sniffer tout le trafic local.
- **ATTAQUE** : Injection de paquets forgés (Packet Injection) pour manipuler les protocoles supérieurs.
- **MITIGATION** : Restrictions de privilèges (Capabilities Linux) pour limiter l'accès aux interfaces réseau virtuelles.

### Comment Lancer
1.  **Compiler** : `make`
2.  **Lancer avec privilèges** : `sudo ./ether_stack`
