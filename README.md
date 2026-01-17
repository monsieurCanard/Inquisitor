# Inquisitor - ARP Poisoning

Ce projet est une preuve de concept (PoC) et un environnement d'apprentissage pour comprendre et exécuter une attaque ARP Poisoning (Man-in-the-Middle).

L'architecture repose sur un réseau Docker sécurisé contenant un trafic légitime, que nous allons intercepter depuis la machine hôte.

## 🏗 Architecture

L'environnement se compose de :
1.  **Serveur FTP** (Container `ftp-server`) : Utilise `proftpd` pour recevoir des fichiers.
2.  **Client FTP** (Container `ftp-client`) : Utilise `lftp` pour envoyer inlassablement un fichier vers le serveur.
3.  **L'Attaquant** (Machine Hôte) : Un programme en C qui s'exécute sur votre machine pour manipuler les tables ARP du réseau Docker.

## 🚀 Mise en place de l'environnement

### 1. Démarrer le réseau victime

Nous utilisons `docker-compose` pour lancer le client et le serveur FTP dans un réseau isolé.

```bash
docker-compose up -d
```

Cela va créer :
*   Le réseau `inquisitor_inquisitor-net`
*   Le container serveur
*   Le container client qui commence immédiatement à envoyer des données.

### 2. Identifier l'interface d'attaque

Puisque l'attaque se déroule depuis l'hôte, nous devons identifier l'interface réseau virtuelle (Bridge) crée par Docker pour ce projet.

1.  **Récupérer l'ID du réseau Docker :**

    ```bash
    docker network ls | grep inquisitor
    # ou pour avoir le nom complet de l'interface bridge dans les détails
    docker inspect inquisitor_inquisitor-net
    ```

    Notez l'ID du réseau (ex: `43eccdb5951e...`).

2.  **Trouver l'interface sur l'hôte :**

    Docker crée une interface bridge nommée `br-<ID_RESEAU>` (les 12 premiers caractères de l'ID).
    Vous pouvez confirmer l'interface associée à la gateway (souvent `172.19.0.1` ou `172.x.x.1`) :

    ```bash
    ip addr | grep 172.19.0.1
    ```

    *Exemple de sortie :*
    `inet 172.19.0.1/16 brd 172.19.255.255 scope global br-43eccdb5951e`

    Ici, l'interface cible est **`br-43eccdb5951e`**.

## ⚔️ Exécution de l'attaque

Le code source de l'attaquant se trouve dans le dossier `inquisitor/`.

### Configuration de l'interface

Avant de compiler, assurez-vous que le code (`placer.c`) pointe vers la bonne interface réseau que vous avez identifiée à l'étape précédente.

### Compilation et exécution

```bash
cd inquisitor
gcc placer.c -o arpoiso
sudo ./arpoiso
```

Le programme va maintenant pouvoir écouter et injecter des paquets ARP sur le bridge Docker.

## 📝 Étapes de l'attaque (Roadmap)

1.  **Setup** : Mise en place du Docker network (Fait).
2.  **Interception** : Intercepter le trafic ARP.
3.  **Spoofing** : Manipuler les tables ARP (Envoyer des réponses ARP falsifiées).
4.  **Sniffing** : Récupérer les paquets FTP qui transitent.

## ⚠️ Avertissement

Ce projet est à but purement éducatif. N'utilisez ces techniques que sur des réseaux dont vous êtes le propriétaire ou avec l'autorisation explicite des administrateurs.

