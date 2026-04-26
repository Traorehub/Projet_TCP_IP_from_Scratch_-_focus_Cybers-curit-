# Jour 09 : Contrôle de Congestion - Bootcamp Cyber

La congestion est une arme de déni de service à l'échelle du réseau global.

### 📄 Rapport d'Analyse Cyber
[Consulter le dossier complet sur la Robustesse (PDF)](./Jour%209%20%20Retransmission%20et%20Robustesse%20(Dossier%20Complet).pdf)

### Focus Sécurité & Analyse de Failles (Niveau Bootcamp)
- **FAILLE : Manipulation de la fenêtre (CWND)**.
- **ATTAQUE : Optimistic ACKing**. Un client peut envoyer des ACKs pour des données qu'il n'a pas encore reçues pour forcer le serveur à augmenter sa vitesse d'envoi et saturer le lien.
- **MÉCANISME DE PROTECTION** :
    - **SACK (Selective ACK)** : Permet de confirmer précisément quels segments sont arrivés.
    - **Vérification stricte des ACKs** : Ne jamais incrémenter le `cwnd` sur des ACKs incohérents avec le `SEQ` actuel.

### Question de Réflexion
Si un attaquant force votre stack à passer son `ssthresh` à 1 segment, quel est l'impact sur vos performances applicatives ?
