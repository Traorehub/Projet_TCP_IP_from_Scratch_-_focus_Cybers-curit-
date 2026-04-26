# Jour 10 : Fermeture & CTF Final - Bootcamp Cyber

Le point final du cycle de vie d'une connexion, et l'épreuve de vérité pour la stack.

### 📄 Rapport d'Analyse Cyber & CTF
[Consulter le rapport final (PDF)](./CTF_Reseau_10Jours.pdf)

### Focus Sécurité & Analyse de Failles (Niveau Bootcamp)
- **FAILLE : RST-Injection (Blind)**.
- **ATTAQUE : Connection Reset Attack**. Un attaquant envoie un paquet avec le bit `RST` et un numéro de séquence "proche" de celui attendu par la cible pour fermer la connexion prématurément.
- **MÉCANISME DE PROTECTION** :
    - **Sequence Number Validation** : Notre stack n'accepte le RST que si le numéro de séquence est **exactement** celui attendu (`conn->ack`), ce qui réduit drastiquement les chances de réussite d'une attaque aveugle.

### 🏁 Bilan du Bootcamp
Au terme de ces 10 jours, vous avez construit un système capable de résister aux attaques basiques et vous comprenez les mécanismes profonds qui régissent la sécurité des communications sur Internet.
