# Jour 05 : L'État ESTABLISHED - Bootcamp Cyber

Une connexion établie n'est pas synonyme de sécurité. C'est ici que commence le détournement de session.

### 📄 Rapports d'Analyse Cyber
- [Rapport technique Jour 5 (PDF)](./Jour5_TCP_IP.pdf)
- [Images d'analyse du handshake](./Jour5_images.pdf)

### Focus Sécurité & Analyse de Failles (Niveau Bootcamp)
- **FAILLE : Prévisibilité des numéros de séquence (ISN)**.
- **ATTAQUE : TCP Session Hijacking**. Si un attaquant peut deviner ou intercepter le prochain numéro de séquence (`SEQ`), il peut injecter des commandes dans une session existante à la place de l'utilisateur légitime.
- **MÉCANISME DE PROTECTION** :
    - **Randomisation de l'ISN** : Utiliser des générateurs de nombres pseudo-aléatoires cryptographiques pour le premier `SEQ`.
    - **Chiffrement (SSL/TLS)** : Protéger le contenu de la session pour rendre le hijacking inutile.

### Défi Cyber
Essayez de forcer notre stack à utiliser un `SEQ` prévisible et tentez de "spoofer" un ACK final depuis une autre source.
