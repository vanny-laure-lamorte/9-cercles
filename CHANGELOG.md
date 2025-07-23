## [v1.0] - 2025-07-22
### Cercle 1 : Première version stable de l’application
- Développer deux programmes console capables de communiquer ensemble via le réseau: un client et un serveur.
- S’appuyer sur une class LPTF_Socket, encapsulant les différents Syscalls.
- Éviter d'utiliser les Syscalls réseau en dehors de la class LPTF_Socket ou Thread et Fork.

## [v1.1] - 2025-07-22
### Cercle 1 : Tests unitaires
- Mettre à jour les CMakesLists pour inclure la librairie catch2.
- Rajouter tests unitaires pour les Sockets côté Client et Server (test/test_socket.cpp).
- Mettre à jour le README

## [v2.0] - 2025-07-23  
### Cercle 2 : Conception d’un protocole binaire évolutif

- Collaboration intergroupes pour concevoir un **protocole de communication binaire** structuré et évolutif, pensé pour répondre aux besoins des **9 cercles** du voyage.
- Création de la classe `LPTF_Packet` :
  - Structure binaire contenant `version`, `type`, `length`, `payload`
  - Méthodes `serialize()` / `deserialize()` pour encoder/décoder les données pour le transfert réseau
  - Conçue pour permettre **l’évolution future du protocole** (ajout de champs, compatibilité ascendante, etc.)
- Intégration du protocole dans l’application :
  - `LPTF_Client` et `LPTF_Server` envoient et reçoivent des objets `LPTF_Packet`
  - `LPTF_Socket` enrichie avec `sendMsg()`, `receivePacket()`, `sendAll()`, `receiveAllPackets()`
- Tests unitaires ajoutés (Catch2) :
  - Vérification de la sérialisation / désérialisation
  - Cas d’erreurs (paquets invalides, versions non supportées…)
- Première étape vers un protocole **commun, structuré et interopérable**, base du dialogue à travers les cercles.
