#include <stdio.h>
#include <stdlib.h>
#include <SDL.h>
#include "tree.h"

#define Largeur 672
#define Hauteur 672


/*Fonction de l'arbre binaire*/

void main_arbre()
{
    Tree *arbre = join_tree(join_tree(new_tree(8),new_tree(3),2),new_tree(4),1);

    print_tree_prefix(arbre);
    printf("Nombre de noeuds : %d\n", count_tree_nodes(arbre));

    clean_tree(arbre);
}

/**
* Créée un nouvel Arbre
* @param x La valeur de la racine
* @return Le nouvel arbre créé
*/
Tree *new_tree(int x)
{
	Tree *tr = malloc(sizeof(*tr));

	if(tr == NULL)
	{
		fprintf(stderr, "Erreur allocation memoire.\n");
		exit(EXIT_FAILURE);
	}

	tr->value = x;
	tr->parent = NULL;
	tr->tleft = NULL;
	tr->tright = NULL;

	

	return tr;
}

/*-----------------------------------------------------------------------*/

/**
* Nettoie un arbre
* @param tr L'arbre à vider de ses valeurs
*/
void clean_tree(Tree *tr)
{
	if(tr == NULL)
		return;

	//Aide pour le développeur
	//printf("Suppression de %d...\n", tr->value);

	clean_tree(tr->tleft);
	clean_tree(tr->tright);

	free(tr);
}

/*-----------------------------------------------------------------------*/

/**
* Joint deux arbres pour n'en former qu'un
* @param left L'arbre de gauche
* @param right L'arbre de droite
* @param node Le noeud qui lie les deux arbres
* @return Le nouvel arbre formé
*/
Tree *join_tree(Tree *left, Tree *right, int node)
{
	Tree *tr = new_tree(node);

	tr->tleft = left;
	tr->tright = right;

	if(left != NULL)
		left->parent = tr;
	if(right != NULL)
		right->parent = tr;

	return tr;
}

/*-----------------------------------------------------------------------*/

/**
* Affiche un arbre récursivement (parcours préfixé)
* @param tr L'arbre à parcourir
*/
void print_tree_prefix(Tree *tr)
{
	if(tr == NULL)
		return;

	if(tr->parent != NULL)
    	printf("(Joueur %d) -> %d Points \n", tr->parent->value, tr->value);
    else
    	printf("\n(%d) Points\n", tr->value);

    if(tr->tleft != NULL)
    	print_tree_prefix(tr->tleft);

    if(tr->tright != NULL)
    	print_tree_prefix(tr->tright);
}

/*-----------------------------------------------------------------------*/

/**
* Compte le nombre de noeuds d'un arbre
* @param tr L'arbre dont il faut compter les noeuds
* @return Le nombre de noeuds de l'arbre binaire
*/
int count_tree_nodes(Tree *tr)
{
	if(tr == NULL)
		return 0;

	return (count_tree_nodes(tr->tleft) + count_tree_nodes(tr->tright) + 1);	
}

/*----------LANCMENT DE LA SDL----------*/

//Fonction raccourci d'erreur et Initialisation
void SDL_ExitError(const char *message)
{
    SDL_Log("Erreur : %s -> %s\n",message,SDL_GetError()) ;
    SDL_Quit();
    exit(EXIT_FAILURE);
}
void afficherfond(SDL_Renderer *rendu,char *Textebas,char *Textehaut);
void Affichermenu1(SDL_Renderer *rendu);


/*----------Fonction Principale pour le lancement du jeu----------*/
SDL_bool Jouer(SDL_Renderer *rendu) ;

int main(int argc, char **argv) //! PROBLEME LANCEMENT DEPUIS LE .exe
{
    SDL_Window *window ;
    SDL_Renderer *rendu ;

//Lancement SDL
    if  (SDL_Init(SDL_INIT_VIDEO)!=0) SDL_ExitError("Initialisation") ;

//Création fenêtre + rendu

    window=SDL_CreateWindow("Labyrinthe",SDL_WINDOWPOS_CENTERED,SDL_WINDOWPOS_CENTERED,Largeur,Hauteur,0);
    rendu=SDL_CreateRenderer(window,-1,SDL_RENDERER_SOFTWARE);

    Affichermenu1(rendu);
    /*----------------------------------------------------------*/
    SDL_bool Lancement = SDL_TRUE;
    SDL_Event event;
    while(Lancement){
        while(SDL_PollEvent(&event)){
            switch(event.type){
                case(SDL_QUIT):
                {
                    Lancement=SDL_FALSE;
                    break;
                }
                default:
                {
                    break;
                }
                case(SDL_MOUSEBUTTONDOWN):
                {
                    if(event.button.button ==SDL_BUTTON_LEFT){
                        if(event.motion.x>=140&&event.motion.x<605){
                            if(event.motion.y>200&&event.motion.y<=336){
                            //printf("Jeu en Cours");
                            printf("\n") ; 
                            Lancement=Jouer(rendu); 
                            }
                            else if (event.motion.y>=400&&event.motion.y<=536){
                                Lancement=SDL_FALSE;
                            }
                        }
                    }
                break;
                }
            }
        }
    }
    /*----------------------------------------------------------*/
    SDL_DestroyRenderer(rendu);
    SDL_DestroyWindow(window) ;
    SDL_Quit();
    return(0);
}

/*----------Image de Fond dans l'accueil du Jeu avec les options Jouer et Quitter----------*/
void afficherfond (SDL_Renderer *rendu,char *Textebas,char *Textehaut) {

    /*--------------- Affichage Image Background --------------- */

    SDL_Surface *surfacefont=NULL;
    surfacefont = SDL_LoadBMP("src/texture/menu.bmp"); //  Récupération de l'image du Menu
    SDL_Texture *texturemenu=NULL;
    texturemenu=SDL_CreateTextureFromSurface(rendu,surfacefont);
    SDL_FreeSurface(surfacefont);
    SDL_Rect r;
    SDL_QueryTexture(texturemenu,NULL,NULL,&r.w,&r.h);
    r.x=0;
    r.y=0;
    SDL_RenderCopy(rendu,texturemenu,NULL,&r);
    SDL_DestroyTexture(texturemenu);

    /*--------------- Affichage des rectangles gris --------------- */

    surfacefont=SDL_LoadBMP("src/texture/font.bmp"); // Récupération de l'image 
    SDL_Texture *texturefont=NULL; //Initialisation de la texture 
    texturefont=SDL_CreateTextureFromSurface(rendu,surfacefont); //Création de la Texture 
    SDL_FreeSurface(surfacefont); // la surface du font est au l'arrière plan pour que les rectangle gris soit devant 
    SDL_QueryTexture(texturefont,NULL,NULL,&r.w,&r.h); // On créer une texture rectangulaire 
    r.x=100; //position x du premier rectangle
    r.y=200; //position y du premier rectangle 
    SDL_RenderCopy(rendu,texturefont,NULL,&r); // Affichage du rendu de la texture 
    r.x=100; //position x du deuxieùme rectangle 
    r.y=400; //position y du deuxieme rectangle 
    SDL_RenderCopy(rendu,texturefont,NULL,&r);   // Affichage du rendu de la texture  
    SDL_DestroyTexture(texturefont); // On detruit l'image de fond 


 surfacefont=SDL_LoadBMP(Textebas);//Initialisation de la texture  
    texturefont=SDL_CreateTextureFromSurface(rendu,surfacefont); //Création de Surface pour la texture 
    SDL_FreeSurface(surfacefont);   // la surface du rectangle gris est au l'arrière plan pour que les rectangle gris soit devant 
    SDL_QueryTexture(texturefont,NULL,NULL,&r.w,&r.h);  // On créer une texture rectangulaire 
    r.x=160; //position x du texte du Haut 
    r.y=410; //position y du texte du Haut 
    SDL_RenderCopy(rendu,texturefont,NULL,&r);// Affichage du rendu de la texture 
    SDL_DestroyTexture(texturefont);// On detruit l'image de du rectrangle gris 

    surfacefont=SDL_LoadBMP(Textehaut);
    texturefont=SDL_CreateTextureFromSurface(rendu,surfacefont);
    SDL_FreeSurface(surfacefont);
    SDL_QueryTexture(texturefont,NULL,NULL,&r.w,&r.h);
    r.x=160;
    r.y=210;
    SDL_RenderCopy(rendu,texturefont,NULL,&r);
    SDL_DestroyTexture(texturefont);
}

 /*--------------- Affichage du menu à l'écran --------------- */
void Affichermenu1(SDL_Renderer *rendu){
    SDL_RenderClear(rendu);
    afficherfond(rendu,"src/texture/quitter.bmp","src/texture/jouer.bmp"); // Récupération de l'image Jouer );
    SDL_RenderPresent(rendu);
}

 /*--------------- Affichage du sous menu Jouer --------------- */

#define HauteurBloc 32
#define LargeurBloc 32

#define Haut 0
#define Bas 1
#define Droite 2
#define Gauche 3

char map[21][21] ={
    "040000000000000000000", //1
    "011112211111111111110", //2
    "013132233312212313310", //3
    "011111211111112113310", //4
    "013333133122211212110", //5
    "011333311121121211130", //6
    "011112122111221212210", //7
    "031122121121221212210", //8
    "013122121221121212210", //9
    "011122121112121211210", //10
    "012111121312121221110", //11
    "012212122111111221130", //12
    "011122212122121333110", //13
    "012112111121221313310", //14
    "012112113211113113310", //15
    "012122123212221111110", //16
    "013122121212221232210", //17
    "011112121313212231110", //18
    "032122121213113331210", //19
    "011111111111111113110", //20
    "000000000000000000000"  //21
};

void affichermap(SDL_Renderer *rendu,SDL_Texture *palette);
void afficherperso(SDL_Renderer *renduperso,SDL_Texture *paletteperso,SDL_Rect *emplacement,int numeroPerso);
void deplacement(SDL_Rect *emplacement,int direction);
SDL_bool VerifierCaseJoueur(SDL_Rect emplacementJ);



// Commande principale pour lancer le jeu 

SDL_bool Jouer(SDL_Renderer *rendu) {

 SDL_Surface *surface;
    surface = SDL_LoadBMP ("src/texture/Palettedebloc.bmp");
    SDL_Texture *palette;
    palette = SDL_CreateTextureFromSurface(rendu,surface);
    SDL_FreeSurface(surface);
    affichermap(rendu,palette);
    SDL_RenderPresent(rendu);
    
    /*--------------- Chargement Texture Joueur --------------- */

    surface = SDL_LoadBMP("src/texture/perso.bmp"); // Récupération de l'image 
    SDL_Texture *paletteperso;
    paletteperso = SDL_CreateTextureFromSurface(rendu,surface); 
    SDL_FreeSurface(surface);

    /* --------------- Création du joueur  --------------- */

    SDL_Rect emplacement;
    emplacement.h=HauteurBloc;
    emplacement.w=LargeurBloc;
    emplacement.x=608; // On initialise le personnage à gauche  
    emplacement.y=608;// On initialise le personnage à bas
    afficherperso(rendu,paletteperso,&emplacement,1);

    
    SDL_bool Lancement = SDL_TRUE; // Le jeu est en exécution
    SDL_Event event; // on créé un événement quelconque que soit une touche clavier ou un clique souris 
    while(Lancement){
        while(SDL_PollEvent(&event)){ // On attend que un évènement se passe 
            switch(event.type){
                case(SDL_QUIT): // quand on clique sur la croix rouge ce la quitte le programme 
                {
                    SDL_DestroyTexture(paletteperso);
                    SDL_DestroyTexture(palette);
                    return (Lancement=SDL_FALSE); 
                    break;
                }
                case(SDL_KEYDOWN):
                {
                    switch (event.key.keysym.sym) {
                        case(SDLK_z):{
                            deplacement(&emplacement,Haut); // la touche Z permet de se déplacer vers le haut 
                            break;
                        }
                        case(SDLK_q):{
                            deplacement(&emplacement,Gauche);// la touche Q  permet de se déplacer vers la gauche
                            
                            break;
                        }
                        case(SDLK_s):{
                            deplacement(&emplacement,Bas);// la touche S permet de se déplacer vers le bas  
                            break;
                        }
                        case(SDLK_d):{
                            deplacement(&emplacement,Droite); // la touche D permet de se déplacer vers la droite 
                            break;
                        }
                    }
                    break;
                }
                default:
                {
                    break;
                }
            }
        }
        affichermap(rendu,palette); // Les déplacements se font sur la carte du labyrinthe 
        afficherperso(rendu,paletteperso,&emplacement,1); // Le personnage est sur le labyrinthe 
        SDL_RenderPresent(rendu); 
        if (Lancement!=SDL_FALSE){
            Lancement = VerifierCaseJoueur(emplacement);
            }
    }
    SDL_DestroyTexture(paletteperso); // quand le jeu se quitte la texture du perso se détruit 
    SDL_DestroyTexture(palette) ; //  quand le jeu se quitte les texture des murs se détruisent
    return(SDL_TRUE);
}

// Fonction pour affichre la carte 
void affichermap(SDL_Renderer *rendu,SDL_Texture *palette){   
    int i,j; // la carte sera une matrice en indice i et j 
    SDL_Rect source; // la matrice sera composé de rectangle  
    source.h=HauteurBloc; // de 32 pixel de hauteur défini dans les macro #define
    source.w=LargeurBloc; // de 32 pixel de largeur défini dans les macro #define
    source.y=0; //les bloc commencerons en haut à gauche
    SDL_Rect dest; 
    dest.h=HauteurBloc;
    dest.w=LargeurBloc;
    char c[2];
    for(i=0;i<21;i++){ // création de la matrice en longeur 
        for(j=0;j<21;j++){ // création de matrice en largeur
            dest.x=i*LargeurBloc; // chaque bloc dans la matrice s'affichera en x par rapport a i 
            dest.y=j*HauteurBloc;// chaque bloc dans la matrice s'affichera en y par rapport a j
            c[0]=map[i][j]; // on construit donc la carte 
            c[1]='\0';
           
            if(atoi(c)==1){ // si dans la carte du tableau de caractère est égale à 1 se sera la texture 1 
                source.x=LargeurBloc*1;
                SDL_RenderCopy(rendu,palette,&source,&dest);
            }
            if(atoi(c)==0){ // si dans la carte du tableau de caractère est égale à 1 se sera la texture 0
                source.x=LargeurBloc*0; 
                SDL_RenderCopy(rendu,palette,&source,&dest);
            }
             if(atoi(c)==2){ // si dans la carte du tableau de caractère est égale à 1 se sera la texture 2 
                source.x=LargeurBloc*2;
                SDL_RenderCopy(rendu,palette,&source,&dest);
            }
            if(atoi(c)==3){
                source.x=LargeurBloc*3; // si dans la carte du tableau de caractère est égale à 1 se sera la texture 3
                SDL_RenderCopy(rendu,palette,&source,&dest);
            }
            if(atoi(c)==4){
                source.x=LargeurBloc*4; // si dans la carte du tableau de caractère est égale à 1 se sera la texture 4
                SDL_RenderCopy(rendu,palette,&source,&dest);
            }
        }
    }
}

//cette fonction affiche le personnage 
void afficherperso(SDL_Renderer *renduperso,SDL_Texture *paletteperso,SDL_Rect *emplacement,int numeroPerso){
    SDL_Rect source; // le personnage sera sous forme de rectangle 
    source.h=HauteurBloc; // il aura aussi une taille de 32 pixel de hauteur 
    source.w=LargeurBloc; // il aura également une taille de 32 pixel de largueur 
    source.x=32*(numeroPerso-1); // la source du personnage sera a 0 mais décaller de 32 pixel 
    source.y=0;  // également en y 
    SDL_RenderCopy(renduperso,paletteperso,&source,emplacement); // on copie la texture sur le rendu du personnage 
}

//cette fonction gère la différence des bloc selon les déplacement 
void deplacement(SDL_Rect *emplacement,int direction){
    char c[2];
    c[1]='\0';
    int x = emplacement->x/32;
    int y = emplacement->y/32;
    switch (direction)
    {
    case Haut:
        c[0]=map[x][y-1];
        if(atoi(c)==1||atoi(c)==4){ //  si les bloc sont la texture 1 ou 4 (le drapeau) il peut se déplacer vers le haut de 32 pixel donc un bloc 
            emplacement->y-=32;
        }
        break;
    case Bas:
        c[0]=map[x][y+1];
       if(atoi(c)==1||atoi(c)==4){//  si les bloc sont la texture 1 ou 4 (le drapeau) il peut se déplacer vers le bas de 32 pixel donc un bloc
            emplacement->y+=32;
        }
        break;
    case Gauche:
        c[0]=map[x-1][y];
        if(atoi(c)==1||atoi(c)==4){ //  si les bloc sont la texture 1 ou 4 (le drapeau) il peut se déplacer vers la gauche de 32 pixel donc un bloc
            emplacement->x-=32;
        }
        break;
    case Droite:
        c[0]=map[x+1][y];
        if(atoi(c)==1||atoi(c)==4){ //  si les bloc sont la texture 1 ou 4 (le drapeau) il peut se déplacer vers la droite de 32 pixel donc un bloc
            emplacement->x+=32;
        }
        break;

    default:
        break;
    }
}

// Cette fonction gère la fin de la partie 
SDL_bool VerifierCaseJoueur(SDL_Rect emplacementJ){ // on verrifie l'enplacement du joueur 
    char c[2];
    c[1]='\0';
    int xJ1 = emplacementJ.x/32; // si sa position x
    int yJ1 = emplacementJ.y/32; // et sa position y 
    c[0]=map[xJ1][yJ1]; // se retrouve sur la carte 
    if(atoi(c)==4){  // sur le bloc du drapeau 
        printf("\nVoici le Tableau des Score des Joueurs ");  // le tableau des scores s'affiche 
        main_arbre(); // avec les score sous forme d'arbre 
        printf("\nVictoire du Joueur");  // et le joueur est sortie du labyrinthe 
        
        return(SDL_FALSE); // on renvoi faux dans la bibliothèque SDL pour que le joueur ne peut plus jouer 
    }
    else{return(SDL_TRUE);} // sinon le jeu continue
}

