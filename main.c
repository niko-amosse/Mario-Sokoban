#include "map_niv.h"
#include "main.h"
//gcc src/main.c -o bin/prog -I include -L lib -lmingw32 -lSDL2main -lSDL2 -lSDL2_image -lSDL2_mixer


int main(int argc ,char **argv){

    printf("\n    **  PATIENTE LE JEU CHARGE !\n\n    **  Il y a du son et des images c'est normal si c'est un peu long ;)\n");

    // Initialisation variable pour la fenetre et le rendu
    SDL_Window *window = NULL;
    SDL_Renderer *renderer = NULL;
    SDL_Surface *image = NULL;
    SDL_Texture *texture = NULL;
    Mix_Chunk *son_fond = NULL;

    // Lancement SDL
    if(SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO)!=0)
        SDL_ExitWithErreur("Initialisation SDL echouee");

    // Lancement SDL_mixer
    if(Mix_OpenAudio(96000, MIX_DEFAULT_FORMAT, 1, 1024) < 0){
        SDL_Log("Error : Initialisation SDL_mixer echouee > %s\n", SDL_GetError());
        SDL_Quit();
        exit(EXIT_FAILURE);
    }
    // Creation du son
    son_fond = Mix_LoadWAV("src/Sunset_Lover.wav");
    if (son_fond == NULL)
        SDL_ExitWithErreur("Impossible de charger le son de fond");

    Mix_AllocateChannels(2);
    Mix_PlayChannel(0, son_fond, -1);
    Mix_VolumeChunk(son_fond, 20);

    // Creation et ouverture fenetre 
    window=SDL_CreateWindow("Mario Sokoban", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED ,WIDTH_WINDOW, HEIGHT_WINDOW, SDL_WINDOW_SHOWN);
    if (window==NULL)
        SDL_ExitWithErreur("Impossible de creer la fenetre");

    // Creation de l'icon
    SDL_Surface* icon=IMG_Load("src/mario_bas.gif");
    SDL_SetWindowIcon(window, icon);
    SDL_FreeSurface(icon);

    // Creation du rendu
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_SOFTWARE);
    if (renderer==NULL)
        SDL_ExitWithErreur("Impossible de creer le rendu");

    /*..................................................*/
    //Jouer

    SDL_bool program_launched = SDL_TRUE;
    while (program_launched){
        SDL_Event event;
        while(SDL_PollEvent(&event)){
            print_Image("src/menu.png", window, renderer, image, texture, 0 ,0);
            SDL_RenderPresent(renderer);
            switch (event.type){
                case SDL_QUIT:
                    program_launched = SDL_FALSE;
                    break; 
                case SDL_KEYDOWN :            
                    switch (event.key.keysym.sym){
                        case SDLK_ESCAPE :
                            program_launched = SDL_FALSE;
                            break;
                        case SDLK_KP_1 :
                            afficher_Commandes(&program_launched, window, renderer, image, texture);
                            break;
                        case SDLK_KP_2 :
                            afficher_Regles(&program_launched, window, renderer, image, texture);
                            break;
                        case SDLK_KP_3 :
                            afficher_Niveaux_Page1(&program_launched, niv, &pos_Mario_niv, window, renderer, image, texture);
                            break;
                        default :
                            break;
                    }                
                default:
                    break;
            }
        }
    }

    /*..................................................*/
    //Destruction dans l'ordre inverse de creation
    SDL_DestroyTexture(texture);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    Mix_FreeChunk(son_fond);
    Mix_CloseAudio();
    SDL_Quit();

    return 0;
}

uint jouer (SDL_bool* program_launched_niveaux,SDL_bool* program_launched, uint* niv_reussi, int mapi[NUMBER_CASE_WIDTH][NUMBER_CASE_HEIGHT],SDL_Window* window, SDL_Renderer* renderer, SDL_Surface *image, SDL_Texture *texture){  
    int map[NUMBER_CASE_WIDTH][NUMBER_CASE_HEIGHT];
    copierMap(map, mapi);
    print_MapGame(map, window, renderer, image, texture);

    Mix_Chunk *okay = NULL;
    okay = Mix_LoadWAV("src/okay.wav");
    if (okay == NULL){
        SDL_ExitWithErreur("Impossible de charger le son d'une caisse okay");
    }
    Mix_VolumeChunk(okay, 500);
    Mix_Chunk *bip = NULL;
    bip = Mix_LoadWAV("src/bip.wav");
    if (bip == NULL){
        Mix_FreeChunk(okay);
        SDL_ExitWithErreur("Impossible de charger le son d'une caisse okay");
    }

    position_Object pos_Mario;
    position_Mario(map,&pos_Mario);

    const uint nombre_objectif = number_Objectif(map);
    position_Object pos_Objectif[nombre_objectif];
    remplir_Pos_Objectif(map, pos_Objectif);


    SDL_bool program_launched_jouer = SDL_TRUE;

    while (program_launched_jouer){
        SDL_Event event;
        while(SDL_PollEvent(&event)){
            switch (event.type){
                case SDL_QUIT:
                    program_launched_jouer = SDL_FALSE;
                    *program_launched_niveaux = SDL_FALSE;
                    *program_launched = SDL_FALSE;
                    break; 
                case SDL_KEYDOWN :            
                    switch (event.key.keysym.sym){
                        case SDLK_ESCAPE :
                            return 0;
                            break; 
                        case SDLK_UP : 
                            up_Mario_Jeu(map, bip, &pos_Mario, pos_Objectif, nombre_objectif, window, renderer, image, texture);
                            break;
                        case SDLK_DOWN :
                            down_Mario_Jeu(map, bip, &pos_Mario, pos_Objectif, nombre_objectif, window, renderer, image, texture);
                            break;
                        case SDLK_LEFT :
                            left_Mario_Jeu(map, bip, &pos_Mario, pos_Objectif, nombre_objectif, window, renderer, image, texture);
                            break;
                        case SDLK_RIGHT :
                            right_Mario_Jeu(map, bip, &pos_Mario, pos_Objectif, nombre_objectif, window, renderer, image, texture);
                            break;
                        default :
                            break;
                    }                
                default:
                    break;
            }
            if(number_CaseOk(map)==nombre_objectif){
                SDL_Delay(100);
                Mix_PlayChannel(1, okay, 0);
                (*niv_reussi) ++;
                afficher_Gagne("src/fond_gris.png", "src/gagne.png", window, renderer, image, texture, 0 ,0);
                while(SDL_WaitEvent(&event)){
                    if(event.type == SDL_QUIT){
                        program_launched_jouer = SDL_FALSE;
                        *program_launched_niveaux = SDL_FALSE;
                        *program_launched = SDL_FALSE;
                        return 0;
                    }
                    switch (event.key.keysym.sym){
                        case SDLK_KP_1 : 
                            return 1;
                            break;
                        case SDLK_KP_2 :
                            return 2;
                            break;
                        case SDLK_KP_3 :
                            return 3;
                            break;
                        default :
                            break; 
                        Mix_FreeChunk(okay);
                        Mix_FreeChunk(bip);
                    }
                }
            }
        }
    }
}

void afficher_Gagne(const char *name_File1, const char *name_File2, SDL_Window *window, SDL_Renderer *renderer, SDL_Surface *image, SDL_Texture *texture, uint x, uint y)
{
    image = IMG_Load(name_File1);
    if (image == NULL)
    {
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        SDL_ExitWithErreur("Impossible de charger l'image");
    }

    SDL_SetColorKey(image, SDL_TRUE, SDL_MapRGB(image->format, 255, 0, 0));
    SDL_SetSurfaceAlphaMod(image, 180);
    
    texture = SDL_CreateTextureFromSurface(renderer, image);
    SDL_FreeSurface(image);

    if (texture == NULL)
    {
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        SDL_ExitWithErreur("Impossible de creer la texture");
    }
    SDL_Rect rectangle;

    if (SDL_QueryTexture(texture, NULL, NULL, &rectangle.w, &rectangle.h) != 0)
    {
        SDL_DestroyTexture(texture);
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        SDL_ExitWithErreur("Impossible de charger la texture");
    }

    rectangle.x = x;
    rectangle.y = y;

    if (SDL_RenderCopy(renderer, texture, NULL, &rectangle) != 0)
    {   
        SDL_DestroyTexture(texture);
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        SDL_ExitWithErreur("Impossible d'afficher la texture");
    }
    SDL_DestroyTexture(texture);
    //
    //

    image = IMG_Load(name_File2);
    if (image == NULL)
    {
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        SDL_ExitWithErreur("Impossible de charger l'image");
    }

    SDL_SetColorKey(image, SDL_TRUE, SDL_MapRGB(image->format, 255,255,255));
    //SDL_SetSurfaceAlphaMod(image, 180);

    texture = SDL_CreateTextureFromSurface(renderer, image); // Creation de la texture de la surface
    SDL_FreeSurface(image);                                  // Liberation de la surface

    if (texture == NULL)
    {
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        SDL_ExitWithErreur("Impossible de creer la texture");
    }

    if (SDL_QueryTexture(texture, NULL, NULL, &rectangle.w, &rectangle.h) != 0)
    {
        SDL_DestroyTexture(texture);
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        SDL_ExitWithErreur("Impossible de charger la texture");
    }

    rectangle.x = x;
    rectangle.y = y;

    if (SDL_RenderCopy(renderer, texture, NULL, &rectangle) != 0)
    {   
        SDL_DestroyTexture(texture);
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        SDL_ExitWithErreur("Impossible d'afficher la texture");
    }
    
    SDL_DestroyTexture(texture);
    SDL_RenderPresent(renderer);
}
void afficher_Regles(SDL_bool* program_launched, SDL_Window* window, SDL_Renderer* renderer, SDL_Surface *image, SDL_Texture *texture){
    SDL_bool program_launched_regles = SDL_TRUE;
    int tab[9]={3,0,0,7,0,0,0,0,2};

    uint positionMario=0;
    print_Image("src/regles.png", window, renderer, image, texture, 0 ,0);
    afficher_tableau(tab, window, renderer, image, texture);
    SDL_RenderPresent(renderer);
    while (program_launched_regles){
        SDL_Event event;
        while(SDL_PollEvent(&event)){
            switch (event.type){
                case SDL_QUIT :
                    program_launched_regles = SDL_FALSE;
                    *program_launched = SDL_FALSE;
                    break; 
                case SDL_KEYDOWN :            
                    switch (event.key.keysym.sym){
                        case SDLK_ESCAPE :
                            program_launched_regles = SDL_FALSE;
                            break;
                        case SDLK_LEFT :
                            if(positionMario!=0){
                                positionMario--;
                                tab[positionMario]=MARIO_GAUCHE;
                                tab[positionMario+1]=VIDE;
                                afficher_tableau(tab, window, renderer, image, texture);
                            }
                            break;
                        case SDLK_RIGHT :
                            if(tab[positionMario+1]==CAISSE && tab[positionMario+2]==OBJECTIF){
                                positionMario++;
                                tab[positionMario]=MARIO_DROITE;
                                tab[positionMario+1]=CAISSE_OK;
                                tab[positionMario-1]=VIDE;
                                afficher_tableau(tab, window, renderer, image, texture);
                            }                         
                            else if (tab[positionMario+1]==CAISSE && tab[positionMario+2]==VIDE){
                                positionMario++;
                                tab[positionMario]=MARIO_DROITE;
                                tab[positionMario+1]=CAISSE;
                                tab[positionMario-1]=VIDE;
                                afficher_tableau(tab, window, renderer, image, texture);
                            }   
                            else{
                                positionMario++;
                                tab[positionMario]=MARIO_DROITE;
                                tab[positionMario-1]=VIDE;
                                afficher_tableau(tab, window, renderer, image, texture);
                            }
                            break;
                        default :
                            break;
                    }                
                default:
                    break;
            }
            SDL_RenderPresent(renderer);
            if(tab[8]==CAISSE_OK){
                program_launched_regles = SDL_FALSE;
                break;
            }
        }
    }
}
void afficher_Commandes(SDL_bool* program_launched, SDL_Window* window, SDL_Renderer* renderer, SDL_Surface *image, SDL_Texture *texture){
    SDL_bool program_launched_commandes = SDL_TRUE;
    int tab[9]={3,0,0,7,0,0,0,0,2};

    uint positionMario=0;
    print_Image("src/commandes.jpg", window, renderer, image, texture, 0 ,0);
    afficher_tableau(tab, window, renderer, image, texture);
    SDL_RenderPresent(renderer);
    while (program_launched_commandes){
        SDL_Event event;
        while(SDL_PollEvent(&event)){
            switch (event.type){
                case SDL_QUIT:
                    program_launched_commandes = SDL_FALSE;
                    *program_launched = SDL_FALSE;
                    break; 
                case SDL_KEYDOWN :            
                    switch (event.key.keysym.sym){
                        case SDLK_ESCAPE :
                            program_launched_commandes = SDL_FALSE;
                            break;
                        case SDLK_LEFT :
                            if(positionMario!=0){
                                positionMario--;
                                tab[positionMario]=MARIO_GAUCHE;
                                tab[positionMario+1]=VIDE;
                                afficher_tableau(tab, window, renderer, image, texture);
                            }
                            break;
                        case SDLK_RIGHT :
                            if(tab[positionMario+1]==CAISSE && tab[positionMario+2]==OBJECTIF){
                                positionMario++;
                                tab[positionMario]=MARIO_DROITE;
                                tab[positionMario+1]=CAISSE_OK;
                                tab[positionMario-1]=VIDE;
                                afficher_tableau(tab, window, renderer, image, texture);
                            }                         
                            else if (tab[positionMario+1]==CAISSE && tab[positionMario+2]==VIDE){
                                positionMario++;
                                tab[positionMario]=MARIO_DROITE;
                                tab[positionMario+1]=CAISSE;
                                tab[positionMario-1]=VIDE;
                                afficher_tableau(tab, window, renderer, image, texture);
                            }   
                            else{
                                positionMario++;
                                tab[positionMario]=MARIO_DROITE;
                                tab[positionMario-1]=VIDE;
                                afficher_tableau(tab, window, renderer, image, texture);
                            }
                            break;
                        default :
                            break;
                    } 

                default:
                    break;
            }
            SDL_RenderPresent(renderer);
            if(tab[8]==CAISSE_OK){
                program_launched_commandes = SDL_FALSE;
                break;
            }
        }
    }
}

void afficher_Niveaux_Page1(SDL_bool* program_launched, int niv[7][12], position_Object* pos_Mario_niv, SDL_Window* window, SDL_Renderer* renderer, SDL_Surface *image, SDL_Texture *texture){
    uint suite;
    uint niv_reussi = 0;
    uint code_secret=0;
    int vrai_niv[7][12];
    position_Object pos_niv[10] = {
    {2,2},
    {2,4},
    {4,2},
    {4,4},
    {6,2},
    {6,4},
    {8,2},
    {8,4},
    {10,2},
    {10,4},
    };
    
    print_Niv(niv, window, renderer, image, texture);
    SDL_bool program_launched_niveaux_page1 = SDL_TRUE;
    
    while (program_launched_niveaux_page1){
        SDL_Event event;
        while(SDL_PollEvent(&event)){
            switch (event.type){
                case SDL_QUIT:
                    program_launched_niveaux_page1 = SDL_FALSE;
                    *program_launched = SDL_FALSE;
                    break; 
                case SDL_KEYDOWN :            
                    switch (event.key.keysym.sym){
                        case SDLK_ESCAPE :
                            program_launched_niveaux_page1 = SDL_FALSE;
                            break;
                        case SDLK_UP : 
                            up_Mario_Niv(&program_launched_niveaux_page1, program_launched, &niv_reussi, niv, pos_Mario_niv, pos_niv, window, renderer, image, texture);
                            break;
                        case SDLK_DOWN :
                            down_Mario_Niv(&program_launched_niveaux_page1, program_launched, &niv_reussi, niv, pos_Mario_niv, pos_niv, window, renderer, image, texture);
                            break;
                        case SDLK_LEFT :
                            left_Mario_Niv(&program_launched_niveaux_page1, niv, pos_Mario_niv, pos_niv, window, renderer, image, texture);
                            break;
                        case SDLK_RIGHT :
                            right_Mario_Niv(&program_launched_niveaux_page1, niv, pos_Mario_niv, pos_niv, window, renderer, image, texture);
                            break;
                        case SDLK_KP_2 :
                            if(code_secret == 0){
                                code_secret ++;
                            }
                            else if(code_secret == 1){
                                code_secret ++;
                            }
                            else if(code_secret == 4){
                                code_secret = 1;
                            }
                            else{code_secret=4;}
                            break;
                        case SDLK_KP_0 :
                            if(code_secret == 2){
                                code_secret ++;
                            }
                            else{code_secret=4;}
                            break;
                        case SDLK_KP_3 :
                            if(code_secret == 3){
                            niv[4][2]=CAISSE;
                            niv[4][4]=CAISSE;
                            niv[4][6]=CAISSE;
                            niv[4][8]=CAISSE;
                            niv[4][10]=CAISSE;
                            niv[2][2]=CAISSE;
                            niv[2][4]=CAISSE;
                            niv[2][6]=CAISSE;
                            niv[2][8]=CAISSE;
                            niv[2][10]=CAISSE;
                            print_Niv(niv, window, renderer, image, texture);
                            code_secret ++;
                            }
                            else{code_secret=4;}
                            break; 
                        case SDLK_KP_9 :
                            if(code_secret == 4){
                                for (uint i=niv_reussi+1; i<10; i++){
                                    niv[pos_niv[i].y][pos_niv[i].x]=CAISSE_BLOQUEE;
                                }
                                print_Niv(niv, window, renderer, image, texture);
                                code_secret = 0;
                            }
                            break;
                        default :
                            break;
                    }                
                default:
                    break;
            }
        }
    }

}

void afficher_tableau(uint tab[], SDL_Window* window, SDL_Renderer* renderer, SDL_Surface *image, SDL_Texture *texture){
    uint i;
    for (i=0; i<9; i++){
        switch(tab[i]){
                case VIDE:
                    print_Image("src/vide.png", window, renderer, image, texture, 50+i*34, 330);
                    break;
                case OBJECTIF:
                    print_Image("src/vide.png", window, renderer, image, texture, 50+i*34, 330);
                    print_Image("src/objectif.png", window, renderer, image, texture, 50+i*34, 330);
                    break;
                case MARIO_BAS: 
                    print_Image("src/vide.png", window, renderer, image, texture, 50+i*34, 330);
                    print_Image("src/mario_bas.gif", window, renderer, image, texture, 50+i*34, 330);
                    break;
                case MARIO_GAUCHE: 
                    print_Image("src/vide.png", window, renderer, image, texture, 50+i*34, 330);
                    print_Image("src/mario_gauche.gif", window, renderer, image, texture, 50+i*34, 330);
                    break;
                case MARIO_DROITE: 
                    print_Image("src/vide.png", window, renderer, image, texture, 50+i*34, 330);
                    print_Image("src/mario_droite.gif", window, renderer, image, texture, 50+i*34, 330);
                    break;
                case CAISSE:
                    print_Image("src/caisse.jpg", window, renderer, image, texture, 50+i*34, 330);
                    break;
                case CAISSE_OK:
                    print_Image("src/caisse_ok.jpg", window, renderer, image, texture, 50+i*34, 330);
                    break;
        }
    }
    for (i=0; i<11; i++){
        print_Image("src/mur.jpg", window, renderer, image, texture, 16+i*34, 296);
    }
    for (i=0; i<11; i++){
        print_Image("src/mur.jpg", window, renderer, image, texture, 16+i*34, 364);
    }
    print_Image("src/mur.jpg", window, renderer, image, texture, 16, 330);
    print_Image("src/mur.jpg", window, renderer, image, texture, 356, 330);
    
}

