/**************************************/
// Asmae MEGGARZI    ----    EI2I4 II //

/*************************************/
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#define TMP_STR_SIZE 256
#define MEM_MAX 0x81000000
#define MEM_OK  0x80002001
#define Taille 700

unsigned char* tab_image_source;
unsigned char* tab_image_final;
//Largeur de l'image
unsigned int width; 
//Hauteur de l'image
unsigned int height; 
unsigned int Largeur_globale; 
//Variable qui permet de récupérer la valeur max du ou des pixels contenu dans l'image
unsigned char color_max;
//Variable de type énumérée qui permet d'identifier si une image est de type Binaire ou ASCII
enum format {BIN, ASCII} pgm_form;
//Prototype de la fonction qui permet de lire une image et de récupérer son contenu
void open_file (char* fichier_source);
//Prototype de la fonction qui permet d'enregistrer dans un fichier image (.pgm) l'image filtrée situé dans un emplacement mémoire du DSP pointé par le pointeur "tab_image_final"
void write_file ( unsigned m);
int tab[Taille][Taille];
int resultats[1000000];
int tab_test[1000000];
int max ;
int largeur;
int i ,j , i_filtre,j_filtre;

//Fonction qui permet de lire une image et de récupérer son contenu
void open_file (char* fichier_source) {

	int flag=0;
	char tmp_str[TMP_STR_SIZE];
	char caractere;
	char temp[4];
	unsigned long int val = 0;
	int i = 0;
	int caractereLu;
	FILE* source = NULL;
	unsigned char test;
        int compteur=0;
        
	width = 0;
	height = 0;
	color_max = 0;
	tab_image_source = NULL;
	tab_image_final = NULL;
	//Ouverture du fichier contenant l'image brut
	source = fopen (fichier_source, "r");
	//On vérifie que le fichier est bien existant
	//Si ce n'est pas le cas, on lève une exception et on affiche un message d'erreur
	if (!source) 
	{
		//Affichage du message d'erreur
		fprintf (stderr, "Fichier inexistant\n"); 
		return;
	}
	do 
	{ 
		fgets(tmp_str, TMP_STR_SIZE, source); 
	} 
	while (tmp_str[0]=='#');
	//On identifie le type d'image qui a été ouvert
	//On verifie le type est bien connu , type "P" pour Pgm.
	if (tmp_str[0]=='P') {
		//On regarde si l'image est de type P2
		//Si c'est le cas alors l'image est de type de ASCII
		if (tmp_str[1]=='2')
		{ 
			pgm_form=ASCII; 
		}
		//Sinon si l'image de type P5 alors l'image est de type binaire
		else if (tmp_str[1]=='5')
		{ 
			pgm_form=BIN;
		}
		//Sinon si l'image n'est ni du type P5 ou P2 on affiche un message d'erreur indiquant que le format n'est pas connu
		else 
		{ 
			//Affichage du message d'erreur
			fprintf (stderr, "Erreur de format P2 P5\n");
			//Fermeture du fichier contenant l'image brut
			fclose(source); 
			return; 
		}
	}
	//Si le format n'est pas de Type Px(P2 ou P5) , on affiche un message d'erreur
	else 
	{
		//Affichage du message d'erreur
		fprintf (stderr, "Erreur de format P2 P5\n"); 
		//Fermeture du fichier contenant l'image brut
		fclose(source); 
		return; 
	}
	
	// le fichier est lisible correctement 
	do 
	{ 
		fgets(tmp_str, TMP_STR_SIZE, source);
	}while (tmp_str[0]=='#');
	//On recupère le nombre de ligne et de colonne de l'image
	sscanf(tmp_str, "%d %d", &(width), &(height)); 
	//Si l'image fait 4*4 on affiche un message d'erreur car elle est trop petite pour être taitée
	if ((width<4)||(height<4)) 
	{
		//Affichage du message d'erreur
		printf("Impossible image trop petite \n");
		//Fermeture du fichier contenant l'image brut
		fclose(source);
		return;
	}
	fgets(tmp_str, TMP_STR_SIZE, source);
	//On recupère la valeur maximale du ou des pixels de l'image brut
	sscanf(tmp_str, "%d", &(color_max));
	//La valeur d'un pixel en hexadecimal est compris entre 0 et 255
	//Si cette valeur est supérieur à 255 alors on affiche un message d'erreur
	if (color_max>255) 
	{
		//Affichage du message d'erreur
		printf("Impossible color_max > 255\n");
		//Fermeture du fichier contenant l'image brut
		fclose(source);
		return;
	}
	//Sinon si cette valeur est inférieur à 0 alors on affiche un message d'erreur
	else if(color_max<0) 
	{
		//Affichage du message d'erreur
		printf("Impossible color_max < 0\n");
		//Fermeture du fichier contenant l'image brut
		fclose(source);
		return;
	}
	//On positionnne le pointeur tab_image_source vers l'emplacement mémoire du DSP qui contiendra l'image brut
	tab_image_source = (unsigned char*) MEM_OK;
	//On positionnne le pointeur tab_image_final vers l'emplacement mémoire du DSP qui contiendra l'image filtré
	//tab_image_final  = (unsigned char*) (MEM_OK + (width*height));
	//Si la taille de l'image brut est supérieure à taille mémoire alloué pour le stockage de celle-ci dans le DSP
	//alors on affiche un message d'erreur
	if ((((width*height)+1)*2)> MEM_MAX) 
	{
		//Affichage du message d'erreur
		printf("Impossible memoire insuffisante pour charger l'image d'origine et celle modifie\n");
		//Fermeture du fichier contenant l'image brut
		fclose(source);
		return; 
	}


	j=0;
	i=0;
	val=0;
	
	// entourer l'image avec des zeros 
	for ( int a = 0 ; a<height+2;a++){
		tab [0][a]='0'; 
	}
   	for (int a = 0 ; a<width+2;a++){
		tab [a][0]='0'; 	
	}
     	for (int a = 0 ; a<height+2;a++){
     		tab [width+2][a]='0'; 
 	}     
 
	for (int a = 0 ; a<width+2;a++){
     		tab [a][height+2]='0'; 
	}
		
	// lecture et stockage
	
	caractereLu=fgetc(source);   
	caractere=(char)caractereLu;


 	while(caractereLu!=EOF){

       		if(caractere!=' ' && caractere != '\n'){
			flag = 1;
              		temp[i]=caractere;
              		i=i+1;

        	}
        	else if(caractere == ' ' || caractere == '\n'){
			
			if(flag == 1) {
				flag = 0;
				temp[i]='\0';
				test = atoi(temp);
				tab_test[val]=test;
				i=0;
				strcpy(temp," ");
				val++;

			}

		}

		caractereLu=fgetc(source);
		caractere=(char)caractereLu;
    	}
	// convertir d'un tableau d'une seule dimension a un tableau de deux dimensions
    	for ( int ii=1; ii<height+2 ; ii++){
    		for ( int jj=2; jj<width+2 ; jj++){
    
   			 tab[ii][jj]=tab_test[compteur];
       			 compteur++;
    		}
    	}			   
	fclose(source);
	printf ("\n image écrite en mémoire \n");
	printf("\n");
}


// valeur absolue  
int abs(int a) {
	if(a < 0) a=-a;
	return a;
} 

void traitement_file()
{  
	int filtre[3][3];
	int p =0;
	// Cration de  
	for (  i_filtre = 0 ; i_filtre<3; i_filtre++){
    	
	    	for ( int j_filtre = 0 ; j_filtre<3;j_filtre++) filtre[i_filtre][j_filtre]=-1;
	}
	filtre[1][1]=8;
  	largeur = width ;

	// Faire la somme des multiplications
	for ( int i =1; i<height+1 ; i++){
		for ( int j =1; j<largeur+1 ; j++){
			resultats[p] = tab [i-1][j-1] * filtre[0][0]+tab [i-1][j] * filtre[0][1]+tab [i-1][j+1] * filtre[0][2]
					+tab [i][j-1] * filtre[1][0]+tab [i][j] * filtre[1][1]+tab [i][j+1] * filtre[1][2]
					+tab [i+1][j-1] * filtre[2][0]+tab [i+1][j] * filtre[2][1]+tab [i+1][j+1] * filtre[2][2] ;
		p++;
		}
	}
	int lim_boucle=(height)*(largeur);
	// valeur absolue des valeurs
	for ( int k = 0 ; k<lim_boucle;k++){
		resultats[k]= abs(resultats[k]);
	}
	//Division par 8 le résultats
	for ( int h = 0 ; h<lim_boucle;h++){
		resultats[h] = resultats[h]/8;
	} 

	// recherche de la valeur max dans le résultat
	max =0; 
	for ( int m = 0 ; m<lim_boucle; m++) {
		if ( resultats[m] > max ){
			max = resultats [m];  
		}
	}

	// choisir un seuil et remplacer par 0 ou max
	for ( int d = 0 ; d<lim_boucle ; d++) {
 
		if ( resultats[d] <= max/12) {
			resultats[d] = 0;
		}
		if ( resultats[d]  >  max/12) {
			resultats[d] = max; 
		}	
	}
}


//Fonction qui permet d'enregistrer dans un fichier image (.pgm) l'image filtrée situé dans un emplacement mémoire du DSP pointé par le pointeur "tab_image_final"
void write_file ( unsigned m){
	
	unsigned long int val = 0;
	FILE* nouveau = NULL;
	width =largeur;
	int origine = width*height;
	//On creee le fichier qui contiendra l'image filtré
	nouveau=fopen("image_filtre.pgm","w");
	//Si le fichier n'a pa pu être creee on affiche un message d'erreur
	if (nouveau == NULL) {
		//Affichage du message d'erreur
		printf("Impossible d'ouvrir le fichier nouveau image_filtre.pgm \n");
		return;
	}
	//On creee l'entête
	//Si le fichier brut est de type Binaire, alors dans le fichier contenant l'image filtré on met en entête P5
	
	if (pgm_form==BIN) {
		fputs("P5\n", nouveau); 
	} 
	//Sinon on met en entête P2
	else {
		fputs("P2\n", nouveau); 
	}
	//On place à la suite du type de l'image , sa longueur et sa largeur
	fprintf(nouveau, "%i %i\n", width, height);
	//On place à la suite de la longueur et de la largeur de l'image, la valeur maximale du ou des pixels de l'image filtré 
	fprintf(nouveau, "%i\n", m);
	//On place à la suite de l'entête du fichier filtré, le contenu des différents pixels sur lesquels le filtre a été appliqué
	
	for (val = 0; val < (width*height)-1; val++) {
		
		//Si le format de l'image brut est de type ASCII
		//alors l'ecriture dans le fichier ce fera sous format hexadecimal
		if (pgm_form == ASCII) {
			fprintf(nouveau, "%i\n", resultats[val]);

		}
		//Sinon si le format de l'image brut est de type Binaire 
		//alors l'ecriture dans le fichier ce fera sous format caractère
		else {
			fprintf(nouveau, "%i\n", resultats[val]);
		}
	}
	fclose(nouveau);
	printf("fichier ecrit \n");
}


//Programme principal
int main(void)
{
	printf("Debut");
	//Appel de la fonction qui permet de lire les images et de récupérer son contenu
	//open_file ("lena.ascii.pgm");
	//open_file ("gator.ascii.pgm");
	//open_file ("dragon.ascii.pgm");
	//open_file ("pepper.ascii.pgm");
	open_file ("casablanca.ascii.pgm");
	traitement_file();
	write_file (max);
	return 0;
}














