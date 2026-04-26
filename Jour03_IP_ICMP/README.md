# Jour 03 : IP & ICMP (Ping) - Bootcamp Cyber

L'IP et l'ICMP forment le socle de la visibilité réseau. Du point de vue cyber, c'est l'étape de la "Reconnaissance".

### 📄 Rapport d'Analyse Cyber
[Consulter le rapport détaillé du Jour 3 (PDF)](./jour3_images.pdf)

### Focus Sécurité & Analyse de Failles (Niveau Bootcamp)
- **FAILLE : Divulgation d'informations**. Les réponses ICMP peuvent révéler le type d'OS (TTL différent) et la topologie du réseau.
- **ATTAQUE : Smurf Attack / DDoS**. Utilisation de l'adresse de diffusion (Broadcast) pour amplifier un ping et noyer une victime sous les réponses ICMP.
- **MÉCANISME DE PROTECTION** :
    - **Filtrage ICMP** : Bloquer les types Echo Request au niveau du pare-feu périmétrique.
    - **Ignorer les pings broadcast** : Désactiver le relais broadcast ICMP dans les configurations noyau.

### Comment Lancer & Tester
1.  Lancer la stack et pinguer `10.0.0.2`.
2.  Testez un ping avec une taille de payload inhabituelle pour voir si la stack le gère.
    ```bash
    ping -s 1400 10.0.0.2
    ```
