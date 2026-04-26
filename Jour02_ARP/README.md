# Jour 02 : Résolution d'Adresses (ARP) - Bootcamp Cyber

Le protocole ARP est essentiel mais intrinsèquement vulnérable. Nous étudions comment il permet la communication L2 et comment il peut être détourné.

### 📄 Rapport d'Analyse Cyber
[Consulter le rapport détaillé du Jour 2 (PDF)](./jour2_images.pdf)

### Focus Sécurité & Analyse de Failles (Niveau Bootcamp)
- **FAILLE : Absence d'authentification**. Le protocole ARP accepte des réponses (`ARP Reply`) même s'il n'a jamais envoyé de requête.
- **ATTAQUE : ARP Poisoning / MiTM**. En envoyant des réponses falsifiées au client et à la passerelle, on se place au milieu du trafic (Man-In-The-Middle).
- **MÉCANISME DE PROTECTION** :
    - **Dynamic ARP Inspection (DAI)** : Fonctionnalité sur les switchs managés pour vérifier la légitimité des réponses ARP.
    - **ARP Statique** : Forcer la correspondance MAC/IP sur les serveurs critiques.

### Comment Lancer & Analyser
1.  **Lancer la stack** : `sudo ./arp_stack`
2.  **Analyser avec Wireshark** : Observez comment l'OS demande l'IP `10.0.0.2` et comment notre stack lui "ment" (ou non) pour établir le lien.
