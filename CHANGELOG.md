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