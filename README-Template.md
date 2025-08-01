<img src="readme/so_long.png" alt="so_long" width="900"/>

<div align="center">

# So_Long
### A 2D Game Project at 42 School Using MiniLibX

[![Contributors][contributors-shield]][contributors-url]
[![Forks][forks-shield]][forks-url]
[![Stargazers][stars-shield]][stars-url]
[![Issues][issues-shield]][issues-url]
[![License][license-shield]][license-url]

</div>

---

## 🇬🇧 English

<details>
<summary><b>📖 Click to expand/collapse English version</b></summary>

### 📖 About

**So_Long** is a compulsory project for 42 School students. It consists of creating a small 2D game in C using the MiniLibX graphics library. The player must collect all collectibles on the map and escape through the exit, while avoiding walls and enemies.

This project teaches:
- 2D graphics programming with MiniLibX
- Map parsing and validation
- Game loop management and event handling
- Pathfinding algorithms (BFS for valid path checking)
- Sprite animation and collision detection
- Memory management and error handling

### 🧠 Skills Learned

By completing the So_Long project, students develop essential skills in C programming and game development:

- **MiniLibX usage**: Learning to initialize windows, handle events, and draw images with the MiniLibX library.
- **Map parsing**: Reading and validating map files with specific rules (walls, collectibles, exit, player).
- **Game mechanics**: Implementing player movement, collision detection, collectible gathering, and win/lose conditions.
- **Path validation**: Using BFS to ensure a valid path exists from start to exit collecting all items.
- **Animation and timing**: Managing sprite animations and game updates with timing functions like gettimeofday.
- **Error handling**: Robust validation of map files, with clear error messages for invalid configurations.
- **Bonus features**: Adding enemy AI, sprite animations, and on-screen HUD for extra points.
- **Code organization**: Structuring code into modular functions, adhering to 42 norms for documentation and style.
- **Graphics management**: Handling window events, smooth rendering, and user input (keyboard, mouse).

## Approach
From the start, I wanted to move away from the original project and explore a different path. My ambition was to create a top-down hack'n slash game that promised a fluid and enjoyable gameplay loop for the user. However, the time constraints forced me to favor a more familiar approach, while still keeping the two key points mentioned above.<br>

This is how this *"remastered Mario"* was born, taking up the iconic gameplay loop of this legendary game. You start on one of the game's maps, with the mission of collecting as many coins as possible. But be careful, you may not be alone on this treasure-filled path!<br>

![player](readme/player.gif)

I opted for a **2D oblique view**, *offering a dynamic and immersive perspective*. The graphic elements were recovered from the **Kenney website (free assets)** and then reworked in *Photoshop* to integrate perfectly with **the Minilibx environment**.<br>

In order to enrich the gameplay and bring an additional challenge, I introduced high structures and enemies. The player will thus be able to jump to overcome obstacles or dodge hostile monsters.<br>

![monster](readme/monster.gif)

*(The UI outlines (heart, coins, and step counter) are in black because my WSL has transparency issues with Minilibx, which will work correctly once on MacOS. I'm sorry again for this inconvenience, which bothers me slightly.)*<br>

![game](readme/game.png)

### **Features**

**Breadth-First Search (BFS) algorithm:** *Used to determine if an exit is possible from the player's position on the map.*<br>

**Jumping ability:** *The player can now jump and descend more quickly by pressing the down arrow while in the air.*<br>

**Various animations:** *Added animations for monsters, coins, and the player.*<br>

**Player HUD:** *Displays information such as health, coins collected, and move count.*<br>

**Basic AI for monsters:** *Monsters follow a simple path and have a hitbox that can damage the player.*<br>

### **Features to be added:**

**Player-centered camera:** *To create a sense of progression in the game.*<br>

**Improved hitboxes and asset clipping:** *To prevent collision issues and improve the overall look of the game.*<br>

**Win and lose screens:** *To indicate the end of the game and provide feedback to the player.*<br>

**Scaling assets to 100x100:** *To reduce the player's X-axis and provide a better sense of movement.*<br>

### 📋 Table of Contents

- [Features](#features)
- [Installation](#installation)
- [Usage](#usage)
- [Project Structure](#project-structure)
- [Compilation](#compilation)
- [Function Reference](#function-reference)
- [Credits](#credits)

<a name="features"></a>

### ✨ Features

- **Complete 2D game implementation** with player movement, collectibles, and exit
- **Map validation** ensuring rectangular maps, walls, valid path, and required elements
- **Sprite animations** for player walking, jumping, and enemies
- **HUD display** showing lives, coins collected, and move count
- **Enemy AI** with patrolling monsters that can damage the player
- **Bonus gravity and jumping** mechanics for enhanced gameplay
- **Strict C compliance** with 42 School norming standards

<a name="installation"></a>

### 🚀 Installation

```bash
# Clone the repository
git clone https://github.com/HaruSnak/42-so_long
cd 42-so_long
```

<a name="usage"></a>

### 💻 Usage

Compile and run the game with a map file:

```bash
make
./so_long maps/map3.ber
```

Controls:
- **WASD or Arrow keys**: Move the player
- **ESC**: Quit the game
- **Mouse click**: Interact with menu

<a name="project-structure"></a>

### 📂 Project Structure

```
42-so_long/
├── Makefile                    # Build script
├── so_long.h                   # Main header file
├── so_long.c                   # Main game entry point
├── LICENSE                     # License file
├── README.md                   # This file
├── README-Template.md          # Template for README
├── assets/                     # Game sprites and textures
│   ├── heart/
│   ├── monsters/
│   ├── player/
│   ├── tiles/
│   └── ui/
├── includes/
│   ├── so_long.h
│   └── libft/                  # Custom library
├── maps/                       # Map files (.ber)
├── minilibx-linux/             # MiniLibX library
├── readme/                     # README assets
└── srcs/                       # Source files
    ├── bonus/
    ├── errors/
    ├── game/
    ├── map/
    ├── parsing/
    ├── player/
    └── ui/
```

<a name="compilation"></a>

### 🔧 Compilation

Compile the project using the Makefile:

```bash
make          # Compile the game
make clean    # Remove object files
make fclean   # Remove executable and object files
make re       # Recompile everything
```

<a name="function-reference"></a>

### 📚 Function Reference

#### Main Functions
- [`main`](srcs/so_long.c) - Game initialization and loop
- [`ft_parse_base`](srcs/parsing/parsing.c) - Map parsing and validation
- [`ft_input_player`](srcs/player/player.c) - Player input handling
- [`ft_draw_map`](srcs/map/draw.c) - Map rendering

#### Key Features
- **BFS Pathfinding**: Ensures valid path from player to exit
- **Collision Detection**: Prevents movement through walls
- **Animation System**: Handles sprite changes for movement
- **HUD Management**: Displays game stats on screen

### 👨‍🎓 Note
<p align="left">
    <img src="https://image.noelshack.com/fichiers/2024/11/2/1710270009-125.png"
         alt="125/100" width="216" height="164">
</p>

<a name="credits"></a>

### 📖 Credits

- **42 School Norm**: [Official C Coding Standard](https://cdn.intra.42.fr/pdf/pdf/960/norme.en.pdf)
- **MiniLibX Documentation**: [Harm-Smits Guide](https://harm-smits.github.io/42docs/libs/minilibx/getting_started.html)
- **Free Assets**: [Kenney.nl](https://kenney.nl/assets/pixel-platformer)

### 📄 License

This project is licensed under the **MIT License** - see the [LICENSE](LICENSE) file for details.

</details>

---

## 🇫🇷 Français

<details>
<summary><b>📖 Cliquez pour développer/réduire la version française</b></summary>

### 📖 À propos

**So_Long** est un projet obligatoire pour les étudiants de l'école 42. Il s'agit de créer un petit jeu 2D en C utilisant la bibliothèque graphique MiniLibX. Le joueur doit collecter tous les collectibles sur la carte et s'échapper par la sortie, tout en évitant les murs et les ennemis.

Ce projet enseigne :
- La programmation graphique 2D avec MiniLibX
- Le parsing et la validation de cartes
- La gestion des boucles de jeu et des événements
- Les algorithmes de recherche de chemin (BFS pour vérifier un chemin valide)
- L'animation de sprites et la détection de collisions
- La gestion de la mémoire et des erreurs

### 🧠 Compétences acquises

En complétant le projet So_Long, les étudiants développent des compétences essentielles en programmation C et développement de jeux :

- **Utilisation de MiniLibX** : Apprendre à initialiser des fenêtres, gérer les événements et dessiner des images avec la bibliothèque MiniLibX.
- **Parsing de cartes** : Lire et valider des fichiers de carte avec des règles spécifiques (murs, collectibles, sortie, joueur).
- **Mécaniques de jeu** : Implémenter le mouvement du joueur, la détection de collisions, la collecte d'objets et les conditions de victoire/défaite.
- **Validation de chemin** : Utiliser BFS pour s'assurer qu'un chemin valide existe du départ à la sortie en collectant tous les objets.
- **Animation et timing** : Gérer les animations de sprites et les mises à jour du jeu avec des fonctions de timing comme gettimeofday.
- **Gestion d'erreurs** : Validation robuste des fichiers de carte, avec des messages d'erreur clairs pour les configurations invalides.
- **Fonctionnalités bonus** : Ajouter une IA ennemie, des animations de sprites et un HUD à l'écran pour des points supplémentaires.
- **Organisation du code** : Structurer le code en fonctions modulaires, en respectant les normes 42 pour la documentation et le style.
- **Gestion graphique** : Gérer les événements de fenêtre, le rendu fluide et les entrées utilisateur (clavier, souris).

## Approche
Dès le départ, je voulais m'éloigner du projet original et explorer un chemin différent. Mon ambition était de créer un jeu de hack'n slash en vue aérienne qui promettait une boucle de gameplay fluide et agréable pour l'utilisateur. Cependant, les contraintes de temps m'ont forcé à privilégier une approche plus familière, tout en gardant les deux points clés mentionnés ci-dessus.<br>

C'est ainsi que ce *"Mario remasterisé"* est né, reprenant la boucle de gameplay iconique de ce jeu légendaire. Vous commencez sur l'une des cartes du jeu, avec pour mission de collecter autant de pièces que possible. Mais attention, vous pourriez ne pas être seul sur ce chemin rempli de trésors !<br>

![player](readme/player.gif)

J'ai opté pour une **vue oblique 2D**, *offrant une perspective dynamique et immersive*. Les éléments graphiques ont été récupérés sur le **site web Kenney (assets gratuits)** et retravaillés dans *Photoshop* pour s'intégrer parfaitement avec **l'environnement Minilibx**.<br>

Afin d'enrichir le gameplay et apporter un défi supplémentaire, j'ai introduit des structures élevées et des ennemis. Le joueur pourra ainsi sauter pour surmonter les obstacles ou esquiver les monstres hostiles.<br>

![monster](readme/monster.gif)

*(Les contours de l'interface utilisateur (cœur, pièces et compteur de pas) sont en noir car mon WSL a des problèmes de transparence avec Minilibx, qui fonctionneront correctement une fois sur MacOS. Je m'excuse encore pour cette gêne, qui me dérange légèrement.)*<br>

![game](readme/game.png)

### **Fonctionnalités**

**Algorithme de recherche en largeur (BFS) :** *Utilisé pour déterminer si une sortie est possible depuis la position du joueur sur la carte.*<br>

**Capacité de saut :** *Le joueur peut maintenant sauter et descendre plus rapidement en appuyant sur la flèche vers le bas en l'air.*<br>

**Diverses animations :** *Ajout d'animations pour les monstres, les pièces et le joueur.*<br>

**HUD du joueur :** *Affiche des informations telles que la santé, les pièces collectées et le nombre de mouvements.*<br>

**IA basique pour les monstres :** *Les monstres suivent un chemin simple et ont une hitbox qui peut endommager le joueur.*<br>

### **Fonctionnalités à ajouter :**

**Caméra centrée sur le joueur :** *Pour créer un sentiment de progression dans le jeu.*<br>

**Amélioration des hitboxes et du clipping des assets :** *Pour éviter les problèmes de collision et améliorer l'apparence générale du jeu.*<br>

**Écrans de victoire et de défaite :** *Pour indiquer la fin du jeu et fournir des retours à l'utilisateur.*<br>

**Mise à l'échelle des assets à 100x100 :** *Pour réduire l'axe X du joueur et fournir un meilleur sentiment de mouvement.*<br>

### 📋 Table des matières

- [Caractéristiques](#caractéristiques)
- [Installation](#installation-1)
- [Utilisation](#utilisation)
- [Structure du projet](#structure-du-projet)
- [Compilation](#compilation-1)
- [Référence des fonctions](#référence-des-fonctions)
- [Crédits](#crédits-1)

<a name="caractéristiques"></a>

### ✨ Caractéristiques

- **Implémentation complète de jeu 2D** avec mouvement du joueur, collectibles et sortie
- **Validation de carte** assurant des cartes rectangulaires, murs, chemin valide et éléments requis
- **Animations de sprites** pour la marche du joueur, le saut et les ennemis
- **Affichage HUD** montrant les vies, pièces collectées et compteur de mouvements
- **IA ennemie** avec monstres patrouilleurs pouvant endommager le joueur
- **Mécaniques bonus de gravité et saut** pour un gameplay amélioré
- **Conformité stricte C** avec les normes de l'école 42

<a name="installation-1"></a>

### 🚀 Installation

```bash
# Cloner le dépôt
git clone https://github.com/HaruSnak/42-so_long
cd 42-so_long
```

<a name="utilisation"></a>

### 💻 Utilisation

Compilez et lancez le jeu avec un fichier de carte :

```bash
make
./so_long maps/map3.ber
```

Contrôles :
- **WASD ou flèches** : Déplacer le joueur
- **ESC** : Quitter le jeu
- **Clic souris** : Interagir avec le menu

<a name="structure-du-projet"></a>

### 📂 Structure du projet

```
42-so_long/
├── Makefile                    # Script de build
├── so_long.h                   # Fichier d'en-tête principal
├── so_long.c                   # Point d'entrée principal du jeu
├── LICENSE                     # Fichier de licence
├── README.md                   # Ce fichier
├── README-Template.md          # Template pour README
├── assets/                     # Sprites et textures du jeu
│   ├── heart/
│   ├── monsters/
│   ├── player/
│   ├── tiles/
│   └── ui/
├── includes/
│   ├── so_long.h
│   └── libft/                  # Bibliothèque personnalisée
├── maps/                       # Fichiers de carte (.ber)
├── minilibx-linux/             # Bibliothèque MiniLibX
├── readme/                     # Ressources README
└── srcs/                       # Fichiers sources
    ├── bonus/
    ├── errors/
    ├── game/
    ├── map/
    ├── parsing/
    ├── player/
    └── ui/
```

<a name="compilation-1"></a>

### 🔧 Compilation

Compilez le projet en utilisant le Makefile :

```bash
make          # Compiler le jeu
make clean    # Supprimer les fichiers objets
make fclean   # Supprimer l'exécutable et les fichiers objets
make re       # Recompiler tout
```

<a name="référence-des-fonctions"></a>

### 📚 Référence des fonctions

#### Fonctions principales
- [`main`](srcs/so_long.c) - Initialisation et boucle du jeu
- [`ft_parse_base`](srcs/parsing/parsing.c) - Parsing et validation de la carte
- [`ft_input_player`](srcs/player/player.c) - Gestion des entrées joueur
- [`ft_draw_map`](srcs/map/draw.c) - Rendu de la carte

#### Fonctionnalités clés
- **Recherche de chemin BFS** : Assure un chemin valide du joueur à la sortie
- **Détection de collisions** : Empêche le mouvement à travers les murs
- **Système d'animation** : Gère les changements de sprites pour le mouvement
- **Gestion HUD** : Affiche les statistiques du jeu à l'écran

### 👨‍🎓 Note
<p align="left">
    <img src="https://image.noelshack.com/fichiers/2024/11/2/1710270009-125.png"
         alt="125/100" width="216" height="164">
</p>

<a name="crédits-1"></a>

### 📖 Crédits

- **Norme 42** : [Standard C officiel](https://cdn.intra.42.fr/pdf/pdf/960/norme.en.pdf)
- **Documentation MiniLibX** : [Guide Harm-Smits](https://harm-smits.github.io/42docs/libs/minilibx/getting_started.html)
- **Assets gratuits** : [Kenney.nl](https://kenney.nl/assets/pixel-platformer)

### 📄 Licence

Ce projet est sous licence **MIT** - voir le fichier [LICENSE](LICENSE) pour plus de détails.

</details>

---

[contributors-shield]: https://img.shields.io/github/contributors/HaruSnak/42-so_long.svg?style=for-the-badge
[contributors-url]: https://github.com/HaruSnak/42-so_long/graphs/contributors
[forks-shield]: https://img.shields.io/github/forks/HaruSnak/42-so_long.svg?style=for-the-badge
[forks-url]: https://github.com/HaruSnak/42-so_long/network/members
[stars-shield]: https://img.shields.io/github/stars/HaruSnak/42-so_long.svg?style=for-the-badge
[stars-url]: https://github.com/HaruSnak/42-so_long/stargazers
[issues-shield]: https://img.shields.io/github/issues/HaruSnak/42-so_long.svg?style=for-the-badge
[issues-url]: https://github.com/HaruSnak/42-so_long/issues
[linkedin-shield]: https://img.shields.io/badge/-LinkedIn-black.svg?style=for-the-badge&logo=linkedin&colorB=555
[linkedin-url]: https://www.linkedin.com/in/shany-moreno-5a863b2aa
[license-shield]: https://img.shields.io/github/license/HaruSnak/42-so_long.svg?style=for-the-badge
[license-url]: https://github.com/HaruSnak/42-so_long/blob/master/LICENSE
