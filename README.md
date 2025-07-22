# 9 Cercles

## 📁 Version

**Premier cercle (v1.1)**

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
| Server | ❌        | Bind deux fois sur un même port                      |
| Server | ✅        | Bind sur un port valide et listen                    |
| Client | ✅        | Créer une socket                                     |
| Client | ❌        | Connecter à une IP invalide                          |
| Client | ✅        | Créer puis fermer une socket                         |
| Client | ✅        | Envoyer un message si connecté (requierd server.exe) |

---

## 🛠️ Commandes de compilation

### 🔹 Client

```bash
cmake -S ./client -B client/build -G "Ninja"
cmake --build client/build
.\client\client.exe

ctest --test-dir client/build --output-on-failure
.\client\build\client_tests.exe
```

### 🔹 Server

```bash
cmake -S ./server -B server/build -G "Ninja"
cmake --build server/build
.\server\server.exe

ctest --test-dir server/build --output-on-failure
.\server\build\server_tests.exe
```
---