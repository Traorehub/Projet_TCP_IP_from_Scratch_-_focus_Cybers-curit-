# Jour 06 : Transfert de Données - Bootcamp Cyber

Les données circulent, mais qui les valide ? Nous étudions les vulnérabilités de la manipulation de charges utiles (Payload).

### 📄 Rapport d'Analyse Cyber
[Consulter le rapport détaillé du Jour 6 (PDF)](./jour6_images.pdf)

### Focus Sécurité & Analyse de Failles (Niveau Bootcamp)
- **FAILLE : Segmentation et Réassemblage**. Les attaques par chevauchement de segments peuvent contourner les pare-feu applicatifs (WAF).
- **ATTAQUE : Payload Injection & Data Leakage**. Intercepter des données non chiffrées (Logins, Passwords) via du sniffing passif.
- **MÉCANISME DE PROTECTION** :
    - **Filtrage de contenu profond (DPI)** : Analyser non seulement les headers mais aussi le payload au niveau du pare-feu.
    - **Segmentation Robuste** : Rejeter les segments TCP avec des offsets incohérents.

### Comment Tester
Capturez le trafic avec `tcpdump` pendant que vous envoyez des messages secrets via `nc`.
