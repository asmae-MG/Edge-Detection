# Introduction
Les contours sont les parties les plus informatives d’une image. Pour la détection des objets, on utilise des algorithmes qui permettent de traiter l’image pour ensuite détecter leurs
contours. Dans ce contexte, notre projet consiste à mettre en évidence les contours d’une image pgm.

# Objectif
L’objectif du projet est de Détecter les contours d'une image au format pgm en utilisant un filtre. Le code est écrit en language C.  
![alt text](https://github.com/asmae-MG/Edge-Detection/blob/main/Assets/filtre.png)

# Format PGM
Le format PGM (Portable Graymap File Format) permet de stocker des images avec des nuances 
de gris. A chaque pixel on associe un nombre entre 0 et 256 (donc codé sur un octet) exprimant 
différentes teintes de gris de 0 noir à 255 blancs. Ce type d’image peut être enregistrés sous 
forme binaire ou texte brut (ASCII). La manière dont le fichier PGM a été enregistré apparaît 
dans l'en-tête du fichier. Si l'en-tête contient la chaîne « P2 », il s'agit d'un format de type 
texte, s'il contient la chaîne « P5 », il s'agit d'une représentation binaire. 
![alt text](https://github.com/asmae-MG/Edge-Detection/blob/main/Assets/imagePMG.png)

# Mise en œuvre 
Pour réaliser ce projet j’ai suivi les étapes ci-dessous :
1. La lecture de l’image 
2. Stockage des différents pixels
3. Traitement de l’image :
- Création du filtre (à la fin on multipliera par 1/8) 
![alt text](https://github.com/asmae-MG/Edge-Detection/blob/main/Assets/filtre_par_8.png)
- Somme des multiplications : appliquer l’équation suivante pour chaque pixel et stocke le résultat dans un tableau
![alt text](https://github.com/asmae-MG/Edge-Detection/blob/main/Assets/SommeMultiplication.png)

4. Restituer le filtre en multipliant les pixels par 1/8
5. Chercher la valeur max dans le nouveau tableau contenant les pixels traité
6. Imposer un seuil
![alt text](https://github.com/asmae-MG/Edge-Detection/blob/main/Assets/seuil.png)
![alt text](https://github.com/asmae-MG/Edge-Detection/blob/main/Assets/seuil%20correct.png)
8. Après avoir fixé un seuil on écrit le tableau résultat des pixels dans un fichier.pgm et 
on obtient note image avec les différents contours. Voici quelques exemples :
![alt text](https://github.com/asmae-MG/Edge-Detection/blob/main/Assets/Transformation.png)
![alt text](https://github.com/asmae-MG/Edge-Detection/blob/main/Assets/Transformation2.png)
