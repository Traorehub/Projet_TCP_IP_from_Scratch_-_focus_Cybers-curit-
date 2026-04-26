# TCP/IP Stack From Scratch - Bootcamp Cyber-Sécurité (10 Jours)

Ce dépôt contient une implémentation pédagogique et sécurisée d'une pile TCP/IP complète, écrite en C. Ce projet s'inscrit dans le cadre d'un **Bootcamp Cyber**, dont l'objectif est d'approfondir les connaissances sur les protocoles réseau, d'identifier les vecteurs d'attaques possibles et de comprendre les mécanismes de défense associés.

## 🎯 Objectifs du Bootcamp
- **Approfondissement** : Comprendre le fonctionnement "low-level" du modèle OSI.
- **Analyse de Failles** : Identifier les vulnérabilités de conception des protocoles (ARP Spoofing, TCP Hijacking, DoS).
- **Prototypage Cyber** : Développer des mécanismes de protection (validation de séquence, bounds checking, etc.).

## 🚀 Le Parcours de 10 Jours

| Jour | Module | Focus Cyber & Rapports |
| :--- | :--- | :--- |
| **01** | [Ethernet & TAP](./Jour01_Ethernet_TAP) | [Rapport Cyber](./Jour01_Ethernet_TAP/Jour1_images.pdf) - Sniffing & Injection. |
| **02** | [Protocole ARP](./Jour02_ARP) | [Rapport Cyber](./Jour02_ARP/jour2_images.pdf) - ARP Poisoning & MiTM. |
| **03** | [IP & ICMP](./Jour03_IP_ICMP) | [Rapport Cyber](./Jour03_IP_ICMP/jour3_images.pdf) - Reconnaissance & Smurf Attack. |
| **04** | [TCP SYN](./Jour04_TCP_Handshake) | [Rapport Cyber](./Jour04_TCP_Handshake/Jour4_Images.pdf) - SYN Flood & TCB Exhaustion. |
| **05** | [ESTABLISHED](./Jour05_TCP_Established) | [Rapport Cyber](./Jour05_TCP_Established/Jour5_TCP_IP.pdf) - Hijacking & Sequence Prediction. |
| **06** | [Data Transfer](./Jour06_TCP_Data_Transfer) | [Rapport Cyber](./Jour06_TCP_Data_Transfer/jour6_images.pdf) - DPI & Payload Manipulation. |
| **07** | [Socket API](./Jour07_Socket_API) | [Rapport Cyber](./Jour07_Socket_API/Jour7_images.pdf) - Buffer Overflows & Secure Coding. |
| **08** | [Reliability (RTO)](./Jour08_Retransmission_RTO) | [Rapport Cyber](../Projet%202_Implémentation%20d'une%20Stack%20TCP-IP.pdf) - Slowloris & Timeouts. |
| **09** | [Congestion](./Jour09_Congestion_Control) | [Rapport Cyber](./Jour09_Congestion_Control/Jour%209%20%20Retransmission%20et%20Robustesse%20(Dossier%20Complet).pdf) - Congestion Collapse. |
| **10** | [Termination](./Jour10_TCP_Termination) | [Rapport CTF](./Jour10_TCP_Termination/CTF_Reseau_10Jours.pdf) - RST Injection & Closing Handshake. |

## 🛡️ Focus Sécurité Global
Chaque dossier contient une section dédiée à l'analyse des failles et aux mécanismes de mitigation. L'objectif n'est pas seulement de faire fonctionner la stack, mais de la rendre résiliente aux menaces courantes.

---
*Ce projet a été conçu pour apprendre les fondamentaux du réseau "from scratch" en vue d'une orientation en cybersécurité.*
