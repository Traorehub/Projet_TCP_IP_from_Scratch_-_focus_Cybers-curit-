# Jour 08 : Retransmission & RTO - Bootcamp Cyber

La fiabilité apporte une nouvelle surface d'attaque : la gestion du temps (Timeouts).

### 📄 Rapport d'Analyse Cyber
[Consulter le rapport Projet (PDF)](../Projet%202_Implémentation%20d'une%20Stack%20TCP-IP.pdf)

### Focus Sécurité & Analyse de Failles (Niveau Bootcamp)
- **FAILLE : Épuisement des ressources par persistance**.
- **ATTAQUE : Slowloris / Low & Slow**. En envoyant des données très lentement ou en forçant des retransmissions constantes, un attaquant peut occuper tous les TCB d'un serveur pendant une durée indéterminée.
- **MÉCANISME DE PROTECTION** :
    - **Ajustement du RTO** : Utiliser des algorithmes dynamiques (Karn/Partridge) pour s'adapter à la latence réelle.
    - **Max Retransmission Limit** : Fermer impitoyablement les connexions qui échouent trop souvent.

### Analyse Expérimentale
Observez comment le `curr_rto` double à chaque essai. C'est le mécanisme de **Backoff Exponentiel**, essentiel pour éviter l'effondrement du réseau en cas d'attaque par saturation.
