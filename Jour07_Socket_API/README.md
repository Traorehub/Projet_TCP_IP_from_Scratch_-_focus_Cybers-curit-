# Jour 07 : API Socket Abstraite - Bootcamp Cyber

L'API Socket est l'interface entre le monde "réseau" et le monde "applicatif". C'est une frontière critique pour la sécurité.

### 📄 Rapport d'Analyse Cyber
[Consulter le rapport détaillé du Jour 7 (PDF)](./Jour7_images.pdf)

### Focus Sécurité & Analyse de Failles (Niveau Bootcamp)
- **FAILLE : Buffer Overflow (Mémoire)**. Si l'application lit dans un buffer de socket sans vérifier sa taille, elle risque un écrasement de pile.
- **ATTAQUE : Remote Code Execution (RCE)**. En provoquant un buffer overflow via un payload réseau malveillant, un attaquant peut prendre le contrôle du processus serveur.
- **MÉCANISME DE PROTECTION** :
    - **Bound Checking** : Toujours vérifier `len` par rapport à la taille du buffer alloué (ce que nous avons fait dans `socket_push_data`).
    - **ASLR & NX bits** : Protections au niveau de l'OS pour empêcher l'exécution de code dans la pile.

### Analyse de Code (Cyber)
Examinez la fonction `socket_push_data` dans `socket.c`. Voyez-vous comment nous limitons l'écriture à `RECV_BUFFER_SIZE` ? C'est la base du codage sécurisé.
