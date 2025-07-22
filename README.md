# 9 Cercles

## 📁 Version

**Premier cercle (v1.0)**

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

## 🛠️ Commandes de compilation

### 🔹 Client

```bash
cmake -S ./client -B client/build -G "Ninja"
cmake --build client/build
.\client\client.exe
```

### 🔹 Server

```bash
cmake -S ./server -B server/build -G "Ninja"
cmake --build server/build
.\server\server.exe
```
---