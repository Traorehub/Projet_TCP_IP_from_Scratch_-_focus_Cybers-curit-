# Jour 04 : TCP Handshake (SYN) - Bootcamp Cyber

Le Handshake TCP est le gardien de la session. S'il est mal géré, il devient une arme redoutable pour saturer les serveurs.

### 📄 Rapport d'Analyse Cyber
[Consulter le rapport détaillé du Jour 4 (PDF)](./Jour4_Images.pdf)

### Focus Sécurité & Analyse de Failles (Niveau Bootcamp)
- **FAILLE : État semi-ouvert**. Le serveur garde en mémoire (`TCB`) chaque demande `SYN` entrante en attendant l'ACK final.
- **ATTAQUE : SYN Flood**. Une avalanche de `SYN` forgés avec des IPs sources aléatoires pour saturer la table TCB et empêcher toute nouvelle connexion.
- **MÉCANISME DE PROTECTION** :
    - **SYN Cookies** : Ne pas allouer de TCB avant que le 3ème ACK ne soit validé (via un hachage cryptographique du SEQ).
    - **Filtrage par IP** (Rate Limiting).

### Analyse de la Stack
Relancez `nmap -sS -p 80 10.0.0.2` et observez comment la stack réagit. Tentez d'analyser le temps de garde des connexions `SYN_RECEIVED` avant le timeout.
