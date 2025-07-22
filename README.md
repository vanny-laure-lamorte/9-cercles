# 9 Cercles - Ressources Réseau

## 📁 Version

**Premier cercle (v1.0)**

---
## 🔌 Définitions

- **Socket** : Un socket est un point de communication qui permet à deux programmes d’échanger des données sur un réseau.
- **TCP / UDP** :
  - **TCP** : Protocole fiable qui garantit la livraison des données dans le bon ordre.
  - **UDP** : Protocole plus rapide mais ne garantit ni la livraison ni l’ordre des données.

---

## ⚙️ Fonctions de base

| Fonction             | Serveur | Client | Description                                 |
|----------------------|---------|--------|---------------------------------------------|
| `createSocket()`     | ✅       | ✅      | Crée une socket TCP                         |
| `closeSocket()`      | ✅       | ✅      | Ferme une socket TCP                        |
| `bindSocket()`       | ✅       | ❌      | Associe la socket à une IP et un port       |
| `listenSocket()`     | ✅       | ❌      | Prépare la socket à accepter des connexions |
| `acceptConnection()` | ✅       | ❌❌     | Accepte une connexion entrante              |
| `connectToServer()`  | ❌❌     | ✅      | Connecte le client à un serveur             |
| `sendMessage()`      | ✅       | ✅      | Envoie une information                      |
| `receiveMessage()`   | ✅       | ✅      | Reçoit une information                      |

---

## ✅ Scénarios de test

| Cible  | Required | Description                                            |
|--------|----------|--------------------------------------------------------|
| Server | ✅        | Créer une socket                                     |
| Server | ❌        | Bind sur une IP invalide                             |
| Server | ✅        | Bind sur une IP valide                               |
| Client | ✅        | Créer une socket                                     |
| Client | ❌        | Connecter à une IP invalide                          |
| Client | ✅        | Créer puis fermer une socket                         |
| Client | ❌        | Envoyer un message si non connecté                   |
| Client | ✅        | Envoyer un message si connecté (requierd server.exe) |

---

## 🛠️ Commandes de compilation

### 🔹 Client

```bash
cmake -S ./client -B client/build -G "Ninja"
cmake --build client/build
ctest --test-dir client/build --output-on-failure
.\client\build\client_tests.exe
.\client\client.exe
```

### 🔹 Server

```bash
cmake -S ./server -B server/build -G "Ninja"
cmake --build server/build
ctest --test-dir server/build --output-on-failure
.\server\build\server_tests.exe
.\server\server.exe
```
---

**Deuxième cercle (v2.0)**

## 🧱 Structure des Classes Principales

### 🔷 `LPTF_Packet`
- Gère l'encapsulation des messages réseau sous forme binaire.
- Chaque paquet contient :
  - `version` : version du protocole
  - `type` : identifiant du type de message
  - `length` : longueur du contenu
  - `payload` : contenu du message (binaire ou texte)
- Fournit des méthodes :
  - `serialize()` : convertit l'objet en flux binaire
  - `deserialize()` : reconstruit un paquet depuis un flux réseau
---

### 🔷 `LPTF_Socket`
- Encapsule les appels systèmes réseau (`WinSock2`) pour éviter tout `syscall` direct dans les classes client/serveur.
- Méthodes :
  - `createSocket()`, `closeSocket()`
  - `bindSocket(ip, port)`, `listenSocket(backlog)`
  - `acceptConnection()`
  - `connectToServer(ip, port)`
  - `sendMessage(void*, int)` / `receiveMessage(void*, int)`

---

### 🔷 `LPTF_Client`
- Représente un client réseau.
- Utilise `LPTF_Socket` pour se connecter au serveur.
- Sérialise les messages texte via `LPTF_Packet` pour les envoyer.
- Attend des réponses structurées et les désérialise.

---

### 🔷 `LPTF_Server`
- Lance un serveur TCP en écoutant sur une IP/port donné.
- Accepte une connexion client.
- Reçoit les paquets via `LPTF_Socket`, les interprète avec `LPTF_Packet`.
- Répond dynamiquement (écho ou traitement basique).

---

## 🧪 Tests
- Utilisation de [Catch2](https://github.com/catchorg/Catch2) pour tester :
  - Création et fermeture de sockets
  - Connexions client/serveur
  - Échecs attendus (mauvais IP, ports occupés)
  - Envoi/réception de paquets `LPTF_Packet`

---

## 🚀 Lancer le projet

### Côté serveur
```bash
cd Server/build
./server.exe
