# Rapport RayTracing
Caporal C, Soubeiran C

## Introduction
Le raytracing est une méthode pour générer des scènes réalistes sur ordinateur. Le principe est de créer un espace virtuel sur ordinateur (composé d'élements définis par l'utilisateur ou l'utilisatrice) ainsi qu'un écran et une caméra. Le raytracing va ensuite parcourir l'espace en le discrétisant en envoyant des rayons dans l'espace. Ces rayons se propagent dans le l'espace virtuel et en fonction de la couleur et le type des objets qu'ils rencontrent, renvoient une couleur à afficher sur l'écran.
Notre RayTracing est codé en C++ et propose une modélisation simple du déplacement des rayons et deux formes (sphère et plan).


## Compilation et lancement du projet
**Pour compiler le projet**, se placer dans dossier "objectraytracer/src" et entrer la commande : 
```bash
make clean && make 
```
Pour executer le fichier avec une configuration *default.xml*:
```bash
./objectraytracer input/default.xml
```
Les dépendances sont précisées dans le makefile.

Vous trouverez dans le repertoire *output* l'image finale en ppm. 
Pour les textures indiquez le path local ou globale, et indider sa dimension en x et y dans le fichier xml. La variable hasText si elle est à 0 ne prends pas en compte de texture et donne à la surface la couleur qui lui est attribuée. Un partie de ce rapport est dedier à l'explication du loading via xml.
*Remarques:*
- *l'algorithme est compatible sur linux, il a été conçu sous Ubuntu 18.04 ou équivalent sur windows*
- *l'algorithme renvoit des fichiers .ppm, pour les ouvrir sous linux vous pouvez utiliser: *

## Architecture du dossier
```bash
objectraytracer
├── input                       
├── output
└── src
    ├── Archives
    ├── Class_Object
    └── xml_first_test
```
- **Makefile**: est à la racine du repertoire, il permet la compilation du projet
- **input**: permet de déposer des fichier xml pour la création de la scène, ou à term des fichiers .conf
- **output**: regroupe les images de sorties de l'algorithme
- **src**: regroupe tous les codes sources du projet:
```bash
└── src
├── Archives
├── Class_Object
│    ├── Ray.hpp
│    ├── Sphere.cpp
│    ├── Sphere.hpp
│    ├── Surface.cpp
│    ├── Surface.hpp
│    ├── Texture.hpp
│    ├── element.cpp
│    └── element.hpp
├── main.cpp
├── makefile
├── scene.cpp
├── scene.hpp
├── scene.xml
├── stb_image.h 
├── xml_element_reading.cpp
└── xml_first_test                    
```
Le dossier *Archives* nous permet de sauvegarder des images de précédents rendus. *Class_Object* comprend les codes pour la génération d'objets, *xml_first_test* comprends les codes pour la lecture de fichiers *.xml* pour la génération de scène. 
## Ce que notre projet intègre
- Notre projet permet de générer des scénes avec une fenêtre de taille réglable. Il est également **possible de régler la résolution de l'image** (l'image de sortie sera multiplié par ce nombre mais la taille de la fenêtre au sein de la scène reste la même).
- Il est possible d'ajouter **des sphères** et **des plans** au sein de la scène.
    - Pour les sphère: son rayon, sa position, sa couleur et transparance sont réglables.
    - Pour un plan: sa normale, sa position, sa couleur, transparance et sa texture sont réglables.
- La scène implémente **les ombres**. Plus précisément la diffusion de la lumière sur une surface et l'aparition de spécular est implémenté celon le modèle de Phong.
- La **reflexion** et la **refractions** des rayons est fonctionnel et utilise les lois de Fresnel pour calculer la répartition energétique d'un rayon incident dans ces deux reyons.
- Il est **possible d'ajouter de la texture** aux plans grace à un fichier *.jpg*. Il est possible également de **régler la taille du motif**.
- La possibilité de **définir une scène avec un fichier .xml**. Il est possible de déposer des fichiers xml dans le repertoire input. Cela permet d'éviter de devoir re-compiler le code entre chaque changement de scène. 

## Architecture du Projet:

![](https://markdown.data-ensta.fr/uploads/upload_a3a106fe363e3025432a271b9eb90f08.png)

- La classe abstraite **Element** permet de gerer les éléments à ajouter dans une scène, de cette classe dérivent:
    - Sphere: qui permet d'implémenter des sphères
    - Surface: qui permet d'implementer des plans
    - Light: qui permet d'implémenter des lumières
- La classe **Scene**: permet de gerer la simulation, la fonction render_scene permet de lancer le rendu à partir des élements de la scène (ajouté par la fonction addElement()).
- La classe **Ray**, permet de gérer les rayons. Les rayons sont des arrays, cette classe permet notamment de gerer toutes les opérations vectorielles necessaire. Nous avions codé cette classe pensant gagner en performence comparativement à l'utilisation de std::vector, nous pensons que ce choix n'était pas pertinant, il nous a demandé du temps d'implémentation et à soulevé de nombreuses problématiques qui ont ralentis notre projet.
- La classe **Texture**, permet de stocker les valeurs d'une image sevant de texture pour des objets. 
- **xml_element_reading**: permet de lire le fichier xml placé dans le repertoire input. 

## Feedback sur le moteur de rendu
#### Lumières
Les lumières sont des Elements placés dans la scène. Il n'y a pas d'atténuation de la lumière sur de très longue distance. Ainsi, les plans qui sont très loin, mais face à la caméra paraissent très éclairés (car leur angle varie très peu lors du calcul d'une couleur qui diffuse) alors qu'ils ne devraient pas dans le monde de tous les jours. 
Nous aurions pu ajouter un fog progressif selon la normal du vecteur entre la source lumineuse et l'objet à illuminer.
#### Fresnel
Le fresnel implémenté semble plus ou moins réaliste. En effet, il y a bien un gradient dans le coefficient qui réparti la couleur entre la couleur de la reflection et la couleur de la réfraction en fonction de l'angle de vu, mais nous trouvons que la couleur de la réflection n'est pas assez visible. Dans certain cas, la réflection n'est visible que par une simple ombre sur la surface, ce que nous n'avons pas l'impression d'observer dans la vie de tous les jours.

![fadFresnel](https://markdown.data-ensta.fr/uploads/upload_925122b9529f045620ed0f5f8e239e6c.png)

(Figure 1) : Sur cette image on voit sur la boule réflechissante et transparante que la réflection de la grosse boule rouge n'est rouge que sur l'extrème bord de sa surface.
#### Formes
Les formes que nous avons implémentés sont les sphères et les plans.
Après expérience, nous aurions du commencer par implémenter le triangle, comme la plupart des moteurs de rendu (Blender, Cinéma4D). Cela nous aurait permis de charger n'importe quelle forme complexe sur internet et en avoir le rendu.
A noter cependant que les sphères en rendu triangle renderont moins bien que nos Sphères car plus discrétisées. Il aurait fallu ajouter de l'antialiasing
#### Anti Aliasing
Les gradients sont bien rendus. Cependant les ombres ont parfois un rendu bizarre, très tranché. Nous pensons qu'un peu d'anti aliasing permettrait un rendu plus réaliste.
#### Gestion des vecteurs
Pour gagner en performance, nous utilisons pour représenter nos vecteurs uniquement des arrays (int[] ou float[]). Nous n'avons pas pu tester si c'est beaucoup plus rapide qu'en utilisant des vecteur<FLOAT> ou vecteur<INT>.
Cependant garder des arrays nous a donné des difficultés pour passer en argument nos arrays et les initaliser, et faire des opérations mathématiques dessus.
Si c'était à refaire, nous passerions plus de temps à définir une classe de Vect3 avec les opérations de base de vecteur pour gagner du temps et des lignes de codes.
    
#### Performance 
Pour parraléliser nous utilisons openmp.
Nous utilions le pragma "#pragma omp parralel for loop". Cependant, les pixels doivent être écrits dans l'ordre, il faut que la liste qui sauvegarde les résultats soit partagé. Malheureusement, il y a une limite dans le capacité de l'ordinateur a garder en mémoire un array. En particulier, pour une image de 300*300 avec 3 channels et une résolution de 3, cela fait "2430000 int" a garder en mémoire ce qui conduit à une erreur de compilation. 
    Nous avons donc opté pour ne paralléliser uniquement sur les colonnes de l'image pour pouvoir garder en mémoire l'ensemble de l'array des résultats. 
    Cette solution est bancale car on peut imaginer qu'avec une résolution trop grande et une hauteur de l'image trop grande, on dépasse encore la limite de mémoire. De plus, cette limite dépend de l'ordinateur. Il faudrait donc passer plus de temps pour optimiser en parrallèle le programme
    
```C

    
    float res = 2;
    float w = this->m_width / 2.;
    float h = this->m_height / 2.;
    float eye[3] = {w, -600, h};
    //initialisation for ppm rendering
    std::ofstream ofs("name.ppm", std::ios::out | std::ios::binary);
    ofs << "P6\n"
        << this->m_width * (int)res << " " << this->m_height * (int)res << "\n255\n";
    //end of initialisation
    int i, j;
     // each pixel of the screen

    for (j = 0; j < m_height * res; ++j) //lines
    {
        int list_result[(int)(3 * m_width * res)];
#pragma omp parallel for private(i) shared(j, list_result) num_threads(3)
        for (i = 0; i < (int)(m_width * res); ++i) //column
        {
            float *pixel = new float[3];
            pixel[0] = i / (float)res;
            pixel[1] = 0;
            pixel[2] = j / (float) res;
            Ray primary_ray = Ray(eye, pixel);
            int result[3];
            send_Ray(primary_ray, 0, result);
            list_result[(int)(3 * i )] = (result[0]);
            list_result[(int)(3 * i + 1)] = (result[1]);
            list_result[(int)(3 * i + 2)] = (result[2]);
        }
        for (int k = 0; k <3* m_width * res; ++k)
        {
            ofs << (unsigned char)list_result[k];
        }
    }
    ofs.close();
```

![](https://markdown.data-ensta.fr/uploads/upload_629751d611b847913b7a81e9070de0e1.png)

(Résultats de la parralélisation avec 3 threads car l'ordinateur servant à la simulation dispose de 2 coeurs de 2 threads dont un qui tourne surement pour autre chose (4 threads est plus lent que 1 thread)

## Modèles mathématiques:
#### Frenel 
Le modèle de Frenel pour la répartition de l'énergie d'un rayon incident entre le rayon transmit ( ou refracté) et réfléchie, se base sur la formule suivante où $\theta_1$ est l'angle d'incidence et $\theta_2$ l'angle réfléchis et $n_1, n_2$ l'indice de refraction dans le millieu du rayon en question:
$$ F_{Rs}=\frac{n_2cos\theta_1-n_1cos\theta_2}{n_2cos\theta2+n_2cos\theta_1}^2 $$
$$ F_{Rt}=\frac{n_1cos\theta_2-n_2cos\theta_1}{n_2cos\theta_2+n_2cos\theta_1}^2 $$
et:
$$F_R=\frac{1}{2}(F_{Rs}+F_{Rt}), F_T=1-F_R$$

Pour coder le calcul du coefficient de Frenel $F_R$ à partir d'un rayon on calcul la valeurs des cosinus et sinus par produit scalaire entre le rayon incident et la normale à la surface.
Le milieu extérieur est considéré comme étant de l'air d'indice de refraction $n=1$ et le second milieux d'indice $n_1$.

La fonction est implémenté comme méthode de la classe Ray:
```c++
    float getFresnelCoeff(Ray normal, float n1)
    {
        float fr = 0;
        float cosi = normal.dotLight(*this); //calculation of cosine by scalar produc
        float etai = 1, etat = n1; //refractive index of both medium
        if (cosi > 0)
        {
            std::swap(etai, etat);
        }
        // Compute sini using Snell's law
        float sint = etai / etat * sqrtf(std::max(0.f, 1 - cosi * cosi));
        // Total internal reflection
        if (sint >= 1)
        {
            fr = 1;
        }
        else
        {
            float cost = sqrtf(std::max(0.f, 1 - sint * sint));
            cosi = fabsf(cosi); //absolute value
            float Rs = ((etat * cosi) - (etai * cost)) / ((etat * cosi) + (etai * cost));
            float Rp = ((etai * cosi) - (etat * cost)) / ((etai * cosi) + (etat * cost));
            fr = (Rs * Rs + Rp * Rp) / 2; //square Fresnel value
        }
        return fr;
    }
```
Pour calculer la couleurs d'un pixel coefficiente la couleur apporté par la refration et celle apporté par la reflexion sur les objets de l'environnement (ou à default la couleur d'arrière plan):
```c++
float fr = sqrt(ray.getFresnelCoeff(normal, frontElement->getTransparency()));
result[0] = (1 - fr) * (double)refraction_color[0] + fr * (double)reflexion_color[0];
result[1] = (1 - fr) * (double)refraction_color[1] + fr * (double)reflexion_color[1];
result[2] = (1 - fr) * (double)refraction_color[2] + fr * (double)reflexion_color[2];
```

![](https://markdown.data-ensta.fr/uploads/upload_3b078ce4f27b9dd1becace62bcc6c2a9.png)

représentation des coefficients de Frenel dans le cas Air/Verre

Ce sont ces fonctions qui nous permettent notamment d'avoir un halo coloré sur le bord des objets. Dans la figure suivante on peut remarquer sur la partie inférieur de la sphére un halo bleu cyan. Ceci est du au fait qu'au bord de la sphère les angles à la normale sont trés importants, par conséquent $F_{Rs}$ est proche de 1 et la couleur du plan est dominante, en se raprochant du centre de la sphére les angles sont de plus en plus faibles et la transmission / refraction apporte du noir de la par de la couleur de fond.

![](https://markdown.data-ensta.fr/uploads/upload_bb13d0f639712e82e2598391dfd4fb22.png)


#### Specular
Pour ajouter un effet de brillance (effet plastique) aux objets nous avons ajouté un "specular" qui coeficiente les couleurs calculés par Frenel, et du blanc. Cela ajoute notamment une zone de point blanc sur les objets, mais également sur le sol, comme si les rayons convergaient (figure 2).
```c++
result[0] = (1 - specularContribution) * result[0] + specularContribution * 255;
result[1] = (1 - specularContribution) * result[1] + specularContribution * 255;
result[2] = (1 - specularContribution) * result[2] + specularContribution * 255;
```

![](https://markdown.data-ensta.fr/uploads/upload_8247895bb997aa9c25591d64e657fe73.png)

(Figure 2) : on remarque un point blanc sur les objets qui par refraction apparait sur le "sol".

#### Textures
Pour les textures des plans nous avons décidés de proceder de la manière suivante: 
- à partir de la normale du plan, nous cherchons deux vecteurs normaux à la normale du plan et dans le plan. Ce sont deux vecteurs générateurs du plan
- Pour tous les rayons qui intersectent le plan, on calcul le point d'intersection
- à partir de ce point on crée un vecteur entre le point lié à la normale du plan. 
- On projette ce vecteur sur les deux vecteurs générateurs du plan on appelera le résultats des projections de ce veteurs sur les deux autres $u$ et $v$
- On applique un modulo $n$ à $u$ et $v$. $n$ est la taille de l'image de texture dans le monde virtuel. Ainsi on obtient la couleur du pixel dans l'image que l'on désire récuperer. Le motif se repete de cette manière à l'infini sur le plan.
- On donne la couleur au pixel.

Si le résultat du produit scalaire est négatif, une petite manipulation est necessaire pour ne pas inverser l'image.
Enfin, pour récuperer la couleur d'un pixel d'une image servant de texture, nous avons utilisés la librairie **stbimage**.

```c++
    if (m_hasTexture) // double check element has a texture
    {

        Ray new_ray = Ray(m_point, point);
        u = m_first_dimension.dotIntersection(new_ray); // get the contribution of first dimension
        v = m_second_dimension.dotIntersection(new_ray); // get the contribution of first dimension
        int n = 300; // n is the size of the image in the virtual world. Bigger n gives bigger image (and less)
        u = fmod(u, n); 
        v = fmod(v, n);
        u = u / n;
        v = v / n;
        if (u < 0) // if u or v is negative, get the correspondant point on image
        {
            u = 1 + u;
        }
        if (v < 0)
        {
            v = 1 + v;
        }
        m_texture.getValueUV(u, v, color);
        return true;
    }
```
getValueUV:
```c++
    void getValueUV(float u, float v, int color[3]) const {
	int i = u*m_xsize;
	int j = v*m_ysize;

	if (i < 0) i = 0;
	if (j < 0) j = 0;

	if (i > m_xsize - 1) i = m_xsize - 1;
	if (j > m_ysize - 1) j = m_ysize - 1;

	// std::cout << "i: " << i << " j: " << j << std::endl;

	color[0] = int(m_colors[3 * i + 3 * m_xsize*j]) ;
	color[1] = int(m_colors[3 * i + 3 * m_xsize*j+1]) ;
	color[2] = int(m_colors[3 * i + 3 * m_xsize*j+2]) ;

	return;
}
```
On obtient alors des images texturisés: 

![](https://markdown.data-ensta.fr/uploads/upload_9c05f5e24d72daf8dc25286581b669c5.png)

![](https://markdown.data-ensta.fr/uploads/upload_7c20490631931b7582dda9ab63811764.png)

## Rendu à partir de fichiers XML

Le fichier xml_element_reading permet de recuper le fichier envoyé lors de la compilation.
Ce code itère sur les balises xml en lisant les attibus de celle-ci. 

L'architecture du fichier est définie par le diagramme suivant:

![](https://markdown.data-ensta.fr/uploads/upload_5febe178279568032d953b2e79cea237.png)

Voici un exemple de fichier xml de configuration:
```xml
<?xml version="1.0" ?>
<shapes>
	<scene w="300" h="300" cam="-600" resolution="5"/>
	<sphere x="300" y="500" z="150" r="255" g="0" b="0" radius="100" alpha="1.2"/>
	<sphere x="0" y="500" z="150" r="255" g="0" b="0" radius="100" alpha="0"/>
	<surface x="150" y="500" z="300" r="100" g="255" b="0" u="0" v="0" w="-1" alpha="0" hasText="1" xsize="1816" ysize="1588" nchannel="3" path="input/ipParis2.jpg"/>
	<surface x="0" y="500" z="300" r="100" g="255" b="100" u="1" v="0" w="0" alpha="0" hasText="0" xsize="0" ysize="0" nchannel="0" path="none"/>
	<light x="150" y="100" z="100" r="0" g="255" b="0" intensity="0"/>
</shapes>

```
*remarque: la variable hasText si elle est à 0 ne prends pas en compte de texture et donne à la surface la couleur qui lui est attribuée.*

il génère la scène suivante: 
![](https://markdown.data-ensta.fr/uploads/upload_bb2a15bc70a2cfca2b59c8e10f4b125e.PNG)




## Methodologie de travail

Dans le cadre de ce projet nous avons décidé d'héberger notre projet sur gitlab de Data (association d'informatique de l'école). Cela nous permet de travailler à distance et de faire du versionning. De plus nous avons utilisé très régulièrement la fonctionnalité liveShare de VSC pour travailler conjointement.

Le diagramme suivant récapitule les étapes de notre projet par ordre chronologique, il récapitule également les différentes branch que nous avons employés et les merges:
![](https://markdown.data-ensta.fr/uploads/upload_ae327d28c4d3c962a5beefdfb8e567c4.png)!

Nous avons régulièrement codés à deux pour cumuler nos compétences.

## Annexes: 
### Diagramme d'état:
![](https://markdown.data-ensta.fr/uploads/upload_c5694a223cf90db9668dffcfeab954a0.png)



## Bibliography
https://www.scratchapixel.com/lessons/3d-basic-rendering/introduction-to-ray-tracing/how-does-it-work
https://en.wikipedia.org/wiki/Ray_tracing_(graphics)
http://wwwx.cs.unc.edu/~rademach/xroads-RT/RTarticle.html
https://www.scratchapixel.com/lessons/3d-basic-rendering/minimal-ray-tracer-rendering-simple-shapes/ray-sphere-intersection
