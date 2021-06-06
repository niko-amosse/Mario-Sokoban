#include <SDL.h>
#include <SDL_image.h>
#include <SDL_mixer.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>


#define SIZE_CASE 34
#define NUMBER_CASE_WIDTH 12
#define NUMBER_CASE_HEIGHT 12

#define WIDTH_WINDOW SIZE_CASE *NUMBER_CASE_WIDTH
#define HEIGHT_WINDOW SIZE_CASE *NUMBER_CASE_HEIGHT

typedef unsigned int uint;
typedef struct coordonnes
{
    uint x;
    uint y;
} position_Object;

uint jouer(SDL_bool *program_launched_niveaux, SDL_bool *program_launched, uint* niv_reussi, int map[NUMBER_CASE_WIDTH][NUMBER_CASE_HEIGHT], SDL_Window *window, SDL_Renderer *renderer, SDL_Surface *image, SDL_Texture *texture);
void afficher_Regles(SDL_bool *program_launched, SDL_Window *window, SDL_Renderer *renderer, SDL_Surface *image, SDL_Texture *texture);
void afficher_Commandes(SDL_bool *program_launched, SDL_Window *window, SDL_Renderer *renderer, SDL_Surface *image, SDL_Texture *texture);
void afficher_Niveaux_Page1(SDL_bool *program_launched, int niv_p1[7][12], position_Object* pos_Mario_niv, SDL_Window *window, SDL_Renderer *renderer, SDL_Surface *image, SDL_Texture *texture);

void afficher_Gagne(const char *name_File,const char *name_File2, SDL_Window *window, SDL_Renderer *renderer, SDL_Surface *image, SDL_Texture *texture, uint x, uint y);


int niv[7][12] = {
    {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
    {1, 1, 2, 1, 2, 1, 2, 1, 2, 1, 2, 1},
    {1, 1, 7, 1, 9, 1, 9, 1, 9, 1, 9, 1},
    {0, 6, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {1, 1, 9, 1, 9, 1, 9, 1, 9, 1, 9, 1},
    {1, 1, 2, 1, 2, 1, 2, 1, 2, 1, 2, 1},
    {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
};
position_Object pos_Mario_niv = {1,3};

enum typeCase
{
    VIDE,
    MUR,
    OBJECTIF,
    MARIO_BAS,
    MARIO_HAUT,
    MARIO_GAUCHE,
    MARIO_DROITE,
    CAISSE,
    CAISSE_OK,
    CAISSE_BLOQUEE
};

void SDL_ExitWithErreur(const char *message);

void print_Image(const char *name_File, SDL_Window *window, SDL_Renderer *renderer, SDL_Surface *image, SDL_Texture *texture, uint x, uint y);
void print_MapGame(int map[NUMBER_CASE_WIDTH][NUMBER_CASE_HEIGHT], SDL_Window *window, SDL_Renderer *renderer, SDL_Surface *image, SDL_Texture *texture);

void up_Mario_Jeu(int map[NUMBER_CASE_WIDTH][NUMBER_CASE_HEIGHT],Mix_Chunk *bip, position_Object *pos_Mario, position_Object objectif[], uint nombre_objectif, SDL_Window *window, SDL_Renderer *renderer, SDL_Surface *image, SDL_Texture *texture);
void down_Mario_Jeu(int map[NUMBER_CASE_WIDTH][NUMBER_CASE_HEIGHT],Mix_Chunk *bip, position_Object *pos_Mario, position_Object objectif[], uint nombre_objectif, SDL_Window *window, SDL_Renderer *renderer, SDL_Surface *image, SDL_Texture *texture);
void left_Mario_Jeu(int map[NUMBER_CASE_WIDTH][NUMBER_CASE_HEIGHT],Mix_Chunk *bip, position_Object *pos_Mario, position_Object objectif[], uint nombre_objectif, SDL_Window *window, SDL_Renderer *renderer, SDL_Surface *image, SDL_Texture *texture);
void right_Mario_Jeu(int map[NUMBER_CASE_WIDTH][NUMBER_CASE_HEIGHT],Mix_Chunk *bip, position_Object *pos_Mario, position_Object objectif[], uint nombre_objectif, SDL_Window *window, SDL_Renderer *renderer, SDL_Surface *image, SDL_Texture *texture);

const uint number_Objectif(int map[NUMBER_CASE_WIDTH][NUMBER_CASE_HEIGHT]);
uint number_CaseOk(int map[NUMBER_CASE_WIDTH][NUMBER_CASE_HEIGHT]);
void position_Mario(int map[NUMBER_CASE_WIDTH][NUMBER_CASE_HEIGHT], position_Object *pos_Mario);
void remplir_Pos_Objectif(int map[NUMBER_CASE_WIDTH][NUMBER_CASE_HEIGHT], position_Object objectif[]);
bool mario_OnObjectif(position_Object pos_Mario, position_Object objectif[], uint nombre_objectif);
bool structures_Equals(position_Object struct1, position_Object struct2);
void afficher_tableau(uint tab[], SDL_Window *window, SDL_Renderer *renderer, SDL_Surface *image, SDL_Texture *texture);
void copierMap(int map[NUMBER_CASE_WIDTH][NUMBER_CASE_HEIGHT], int map1[NUMBER_CASE_WIDTH][NUMBER_CASE_HEIGHT]);

void copierNiv(int niv[7][12], int nivi[7][12]);
void print_Niv(int niv[7][12], SDL_Window *window, SDL_Renderer *renderer, SDL_Surface *image, SDL_Texture *texture);

void up_Mario_Niv(SDL_bool *program_launched_niveaux, SDL_bool *program_launched, uint* niv_reussi, int niv[7][12], position_Object *pos_Mario, position_Object objectif[], SDL_Window *window, SDL_Renderer *renderer, SDL_Surface *image, SDL_Texture *texture);
void down_Mario_Niv(SDL_bool *program_launched_niveaux, SDL_bool *program_launched, uint* niv_reussi, int niv[7][12], position_Object *pos_Mario, position_Object objectif[], SDL_Window *window, SDL_Renderer *renderer, SDL_Surface *image, SDL_Texture *texture);
void left_Mario_Niv(SDL_bool *program_launched_niveaux_page1, int niv[7][12], position_Object *pos_Mario, position_Object objectif[], SDL_Window *window, SDL_Renderer *renderer, SDL_Surface *image, SDL_Texture *texture);
void right_Mario_Niv(SDL_bool *program_launched_niveaux_page1, int niv[7][12], position_Object *pos_Mario, position_Object objectif[], SDL_Window *window, SDL_Renderer *renderer, SDL_Surface *image, SDL_Texture *texture);

void SDL_ExitWithErreur(const char *message)
{
    SDL_Log("Error : %s > %s\n", message, SDL_GetError());
    Mix_CloseAudio();
    SDL_Quit();
    exit(EXIT_FAILURE);
}

void print_Image(const char *name_File, SDL_Window *window, SDL_Renderer *renderer, SDL_Surface *image, SDL_Texture *texture, uint x, uint y)
{
    image = IMG_Load(name_File);
    if (image == NULL)
    {
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        SDL_ExitWithErreur("Impossible de charger l'image");
    }

    texture = SDL_CreateTextureFromSurface(renderer, image); // Creation de la texture de la surface
    SDL_FreeSurface(image);                                  // Liberation de la surface

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
}

void print_MapGame(int map[NUMBER_CASE_WIDTH][NUMBER_CASE_HEIGHT], SDL_Window *window, SDL_Renderer *renderer, SDL_Surface *image, SDL_Texture *texture)
{
    for (uint y = 0; y < NUMBER_CASE_HEIGHT; y++)
    {
        for (uint x = 0; x < NUMBER_CASE_WIDTH; x++)
        {
            switch (map[y][x])
            {
            case VIDE:
                print_Image("src/vide.png", window, renderer, image, texture, 34 * x, 34 * y);
                break;
            case MUR:
                print_Image("src/mur.jpg", window, renderer, image, texture, 34 * x, 34 * y);
                break;
            case OBJECTIF:
                print_Image("src/objectif.png", window, renderer, image, texture, 34 * x, 34 * y);
                break;
            case MARIO_BAS:
                print_Image("src/vide.png", window, renderer, image, texture, 34 * x, 34 * y);
                print_Image("src/mario_bas.gif", window, renderer, image, texture, 34 * x, 34 * y);
                break;
            case MARIO_HAUT:
                print_Image("src/vide.png", window, renderer, image, texture, 34 * x, 34 * y);
                print_Image("src/mario_haut.gif", window, renderer, image, texture, 34 * x, 34 * y);
                break;
            case MARIO_GAUCHE:
                print_Image("src/vide.png", window, renderer, image, texture, 34 * x, 34 * y);
                print_Image("src/mario_gauche.gif", window, renderer, image, texture, 34 * x, 34 * y);
                break;
            case MARIO_DROITE:
                print_Image("src/vide.png", window, renderer, image, texture, 34 * x, 34 * y);
                print_Image("src/mario_droite.gif", window, renderer, image, texture, 34 * x, 34 * y);
                break;
            case CAISSE:
                print_Image("src/caisse.jpg", window, renderer, image, texture, 34 * x, 34 * y);
                break;
            case CAISSE_OK:
                print_Image("src/caisse_ok.jpg", window, renderer, image, texture, 34 * x, 34 * y);
                break;
            default:
                SDL_ExitWithErreur("Affichage de chaque case");
            }
        }
    }
    SDL_RenderPresent(renderer);
    SDL_DestroyTexture(texture);
}

void up_Mario_Jeu(int map[NUMBER_CASE_WIDTH][NUMBER_CASE_HEIGHT],Mix_Chunk *bip, position_Object *pos_Mario, position_Object objectif[], uint nombre_objectif, SDL_Window *window, SDL_Renderer *renderer, SDL_Surface *image, SDL_Texture *texture)
{
    if (pos_Mario->y != 0)
    {
        if (map[(pos_Mario->y) - 1][pos_Mario->x] == VIDE || map[(pos_Mario->y) - 1][pos_Mario->x] == OBJECTIF)
        {
            if (mario_OnObjectif(*pos_Mario, objectif, nombre_objectif))
            {
                pos_Mario->y--;
                map[pos_Mario->y][pos_Mario->x] = MARIO_HAUT;
                map[(pos_Mario->y) + 1][pos_Mario->x] = OBJECTIF;
            }
            else
            {
                pos_Mario->y--;
                map[pos_Mario->y][pos_Mario->x] = MARIO_HAUT;
                map[(pos_Mario->y) + 1][pos_Mario->x] = VIDE;
            }
        }
        else if (pos_Mario->y > 1 && (map[(pos_Mario->y) - 1][pos_Mario->x] == CAISSE || map[(pos_Mario->y) - 1][pos_Mario->x] == CAISSE_OK))
        {
            if (map[(pos_Mario->y) - 2][pos_Mario->x] == OBJECTIF)
            {   
                Mix_PlayChannel(1,bip, 0);
                if (mario_OnObjectif(*pos_Mario, objectif, nombre_objectif))
                {
                    pos_Mario->y--;
                    map[pos_Mario->y][pos_Mario->x] = MARIO_HAUT;
                    map[(pos_Mario->y) - 1][pos_Mario->x] = CAISSE_OK;
                    map[(pos_Mario->y) + 1][pos_Mario->x] = OBJECTIF;
                }
                else
                {
                    pos_Mario->y--;
                    map[pos_Mario->y][pos_Mario->x] = MARIO_HAUT;
                    map[(pos_Mario->y) - 1][pos_Mario->x] = CAISSE_OK;
                    map[(pos_Mario->y) + 1][pos_Mario->x] = VIDE;
                }
            }
            else if (map[(pos_Mario->y) - 2][pos_Mario->x] == VIDE)
            {
                if (mario_OnObjectif(*pos_Mario, objectif, nombre_objectif))
                {
                    pos_Mario->y--;
                    map[pos_Mario->y][pos_Mario->x] = MARIO_HAUT;
                    map[(pos_Mario->y) - 1][pos_Mario->x] = CAISSE;
                    map[(pos_Mario->y) + 1][pos_Mario->x] = OBJECTIF;
                }
                else
                {
                    pos_Mario->y--;
                    map[pos_Mario->y][pos_Mario->x] = MARIO_HAUT;
                    map[(pos_Mario->y) - 1][pos_Mario->x] = CAISSE;
                    map[(pos_Mario->y) + 1][pos_Mario->x] = VIDE;
                }
            }
        }
        print_MapGame(map, window, renderer, image, texture);
    }
}

void down_Mario_Jeu(int map[NUMBER_CASE_WIDTH][NUMBER_CASE_HEIGHT],Mix_Chunk *bip, position_Object *pos_Mario, position_Object objectif[], uint nombre_objectif, SDL_Window *window, SDL_Renderer *renderer, SDL_Surface *image, SDL_Texture *texture)
{
    if (pos_Mario->y != NUMBER_CASE_HEIGHT - 1)
    {
        if (map[(pos_Mario->y) + 1][pos_Mario->x] == VIDE || map[(pos_Mario->y) + 1][pos_Mario->x] == OBJECTIF)
        {
            if (mario_OnObjectif(*pos_Mario, objectif, nombre_objectif))
            {
                pos_Mario->y++;
                map[pos_Mario->y][pos_Mario->x] = MARIO_BAS;
                map[(pos_Mario->y) - 1][pos_Mario->x] = OBJECTIF;
            }
            else
            {
                pos_Mario->y++;
                map[pos_Mario->y][pos_Mario->x] = MARIO_BAS;
                map[(pos_Mario->y) - 1][pos_Mario->x] = VIDE;
            }
        }
        else if (pos_Mario->y < NUMBER_CASE_HEIGHT - 2 && (map[(pos_Mario->y) + 1][pos_Mario->x] == CAISSE || map[(pos_Mario->y) + 1][pos_Mario->x] == CAISSE_OK))
        {
            if (map[(pos_Mario->y) + 2][pos_Mario->x] == OBJECTIF)
            {
                Mix_PlayChannel(1,bip, 0);
                if (mario_OnObjectif(*pos_Mario, objectif, nombre_objectif))
                {
                    pos_Mario->y++;
                    map[pos_Mario->y][pos_Mario->x] = MARIO_BAS;
                    map[(pos_Mario->y) + 1][pos_Mario->x] = CAISSE_OK;
                    map[(pos_Mario->y) - 1][pos_Mario->x] = OBJECTIF;
                }
                else
                {
                    pos_Mario->y++;
                    map[pos_Mario->y][pos_Mario->x] = MARIO_BAS;
                    map[(pos_Mario->y) + 1][pos_Mario->x] = CAISSE_OK;
                    map[(pos_Mario->y) - 1][pos_Mario->x] = VIDE;
                }
            }
            else if (map[(pos_Mario->y) + 2][pos_Mario->x] == VIDE)
            {
                if (mario_OnObjectif(*pos_Mario, objectif, nombre_objectif))
                {
                    pos_Mario->y++;
                    map[pos_Mario->y][pos_Mario->x] = MARIO_BAS;
                    map[(pos_Mario->y) + 1][pos_Mario->x] = CAISSE;
                    map[(pos_Mario->y) - 1][pos_Mario->x] = OBJECTIF;
                }
                else
                {
                    pos_Mario->y++;
                    map[pos_Mario->y][pos_Mario->x] = MARIO_BAS;
                    map[(pos_Mario->y) + 1][pos_Mario->x] = CAISSE;
                    map[(pos_Mario->y) - 1][pos_Mario->x] = VIDE;
                }
            }
        }
        print_MapGame(map, window, renderer, image, texture);
    }
}

void left_Mario_Jeu(int map[NUMBER_CASE_WIDTH][NUMBER_CASE_HEIGHT],Mix_Chunk *bip, position_Object *pos_Mario, position_Object objectif[], uint nombre_objectif, SDL_Window *window, SDL_Renderer *renderer, SDL_Surface *image, SDL_Texture *texture)
{
    if (pos_Mario->x != 0)
    {
        if (map[pos_Mario->y][pos_Mario->x - 1] == VIDE || map[pos_Mario->y][pos_Mario->x - 1] == OBJECTIF)
        {
            if (mario_OnObjectif(*pos_Mario, objectif, nombre_objectif))
            {
                pos_Mario->x--;
                map[pos_Mario->y][pos_Mario->x] = MARIO_GAUCHE;
                map[pos_Mario->y][pos_Mario->x + 1] = OBJECTIF;
            }
            else
            {
                pos_Mario->x--;
                map[pos_Mario->y][pos_Mario->x] = MARIO_GAUCHE;
                map[pos_Mario->y][pos_Mario->x + 1] = VIDE;
            }
        }
        else if (pos_Mario->x > 1 && (map[pos_Mario->y][pos_Mario->x - 1] == CAISSE || map[pos_Mario->y][pos_Mario->x - 1] == CAISSE_OK))
        {
            if (map[pos_Mario->y][pos_Mario->x - 2] == OBJECTIF)
            {
                Mix_PlayChannel(1,bip, 0);
                if (mario_OnObjectif(*pos_Mario, objectif, nombre_objectif))
                {
                    pos_Mario->x--;
                    map[pos_Mario->y][pos_Mario->x] = MARIO_GAUCHE;
                    map[pos_Mario->y][pos_Mario->x - 1] = CAISSE_OK;
                    map[pos_Mario->y][pos_Mario->x + 1] = OBJECTIF;
                }
                else
                {
                    pos_Mario->x--;
                    map[pos_Mario->y][pos_Mario->x] = MARIO_GAUCHE;
                    map[pos_Mario->y][pos_Mario->x - 1] = CAISSE_OK;
                    map[pos_Mario->y][pos_Mario->x + 1] = VIDE;
                }
            }
            else if (map[pos_Mario->y][pos_Mario->x - 2] == VIDE)
            {
                if (mario_OnObjectif(*pos_Mario, objectif, nombre_objectif))
                {
                    pos_Mario->x--;
                    map[pos_Mario->y][pos_Mario->x] = MARIO_GAUCHE;
                    map[pos_Mario->y][pos_Mario->x - 1] = CAISSE;
                    map[pos_Mario->y][pos_Mario->x + 1] = OBJECTIF;
                }
                else
                {
                    pos_Mario->x--;
                    map[pos_Mario->y][pos_Mario->x] = MARIO_GAUCHE;
                    map[pos_Mario->y][pos_Mario->x - 1] = CAISSE;
                    map[pos_Mario->y][pos_Mario->x + 1] = VIDE;
                }
            }
        }
        print_MapGame(map, window, renderer, image, texture);
    }
}

void right_Mario_Jeu(int map[NUMBER_CASE_WIDTH][NUMBER_CASE_HEIGHT],Mix_Chunk *bip, position_Object *pos_Mario, position_Object objectif[], uint nombre_objectif, SDL_Window *window, SDL_Renderer *renderer, SDL_Surface *image, SDL_Texture *texture)
{
    if (pos_Mario->x != NUMBER_CASE_HEIGHT - 1)
    {
        if (map[pos_Mario->y][pos_Mario->x + 1] == VIDE || map[pos_Mario->y][pos_Mario->x + 1] == OBJECTIF)
        {
            if (mario_OnObjectif(*pos_Mario, objectif, nombre_objectif))
            {
                pos_Mario->x++;
                map[pos_Mario->y][pos_Mario->x] = MARIO_DROITE;
                map[pos_Mario->y][pos_Mario->x - 1] = OBJECTIF;
            }
            else
            {
                pos_Mario->x++;
                map[pos_Mario->y][pos_Mario->x] = MARIO_DROITE;
                map[pos_Mario->y][pos_Mario->x - 1] = VIDE;
            }
        }
        else if (pos_Mario->y < NUMBER_CASE_WIDTH - 2 && (map[pos_Mario->y][pos_Mario->x + 1] == CAISSE || map[pos_Mario->y][pos_Mario->x + 1] == CAISSE_OK))
        {
            if (map[pos_Mario->y][pos_Mario->x + 2] == OBJECTIF)
            {
                Mix_PlayChannel(1,bip, 0);
                if (mario_OnObjectif(*pos_Mario, objectif, nombre_objectif))
                {
                    pos_Mario->x++;
                    map[pos_Mario->y][pos_Mario->x] = MARIO_DROITE;
                    map[pos_Mario->y][pos_Mario->x + 1] = CAISSE_OK;
                    map[pos_Mario->y][pos_Mario->x - 1] = OBJECTIF;
                }
                else
                {
                    pos_Mario->x++;
                    map[pos_Mario->y][pos_Mario->x] = MARIO_DROITE;
                    map[pos_Mario->y][pos_Mario->x + 1] = CAISSE_OK;
                    map[pos_Mario->y][pos_Mario->x - 1] = VIDE;
                }
            }
            else if (map[pos_Mario->y][pos_Mario->x + 2] == VIDE)
            {
                if (mario_OnObjectif(*pos_Mario, objectif, nombre_objectif))
                {
                    pos_Mario->x++;
                    map[pos_Mario->y][pos_Mario->x] = MARIO_DROITE;
                    map[pos_Mario->y][pos_Mario->x + 1] = CAISSE;
                    map[pos_Mario->y][pos_Mario->x - 1] = OBJECTIF;
                }
                else
                {
                    pos_Mario->x++;
                    map[pos_Mario->y][pos_Mario->x] = MARIO_DROITE;
                    map[pos_Mario->y][pos_Mario->x + 1] = CAISSE;
                    map[pos_Mario->y][pos_Mario->x - 1] = VIDE;
                }
            }
        }
        print_MapGame(map, window, renderer, image, texture);
    }
}

const uint number_Objectif(int map[NUMBER_CASE_WIDTH][NUMBER_CASE_HEIGHT])
{
    uint i = 0;
    for (uint y = 0; y < NUMBER_CASE_HEIGHT; y++)
    {
        for (uint x = 0; x < NUMBER_CASE_WIDTH; x++)
        {
            if (map[y][x] == OBJECTIF || map[y][x] == CAISSE_OK)
            {
                i++;
            }
        }
    }
    return i;
}

uint number_CaseOk(int map[NUMBER_CASE_WIDTH][NUMBER_CASE_HEIGHT])
{
    uint i = 0;
    for (uint y = 0; y < NUMBER_CASE_HEIGHT; y++)
    {
        for (uint x = 0; x < NUMBER_CASE_WIDTH; x++)
        {
            if (map[y][x] == CAISSE_OK)
            {
                i++;
            }
        }
    }
    return i;
}

void position_Mario(int map[NUMBER_CASE_WIDTH][NUMBER_CASE_HEIGHT], position_Object *pos_Mario)
{
    for (uint y = 0; y < NUMBER_CASE_HEIGHT; y++)
    {
        for (uint x = 0; x < NUMBER_CASE_WIDTH; x++)
        {
            if (map[y][x] == MARIO_BAS)
            {
                pos_Mario->x = x;
                pos_Mario->y = y;
                break;
            }
        }
    }
}

void remplir_Pos_Objectif(int map[NUMBER_CASE_WIDTH][NUMBER_CASE_HEIGHT], position_Object objectif[])
{
    uint i = 0;
    for (uint y = 0; y < NUMBER_CASE_HEIGHT; y++)
    {
        for (uint x = 0; x < NUMBER_CASE_WIDTH; x++)
        {
            if (map[y][x] == OBJECTIF || map[y][x] == CAISSE_OK)
            {
                objectif[i].x = x;
                objectif[i].y = y;
                i++;
            }
        }
    }
}

bool mario_OnObjectif(position_Object pos_Mario, position_Object objectif[], uint nombre_objectif)
{
    for (uint i = 0; i < nombre_objectif; i++)
    {
        if (structures_Equals(pos_Mario, objectif[i]))
        {
            return true;
        }
    }
    return false;
}

bool structures_Equals(position_Object struct1, position_Object struct2)
{
    return (struct1.x == struct2.x && struct1.y == struct2.y);
}

void copierMap(int map[NUMBER_CASE_WIDTH][NUMBER_CASE_HEIGHT], int map1[NUMBER_CASE_WIDTH][NUMBER_CASE_HEIGHT])
{
    for (uint y = 0; y < NUMBER_CASE_HEIGHT; y++)
    {
        for (uint x = 0; x < NUMBER_CASE_WIDTH; x++)
        {
            map[y][x] = map1[y][x];
        }
    }
}

void copierNiv(int niv[7][12], int nivi[7][12])
{
    for (uint y = 0; y < 7; y++)
    {
        for (uint x = 0; x < 12; x++)
        {
            niv[y][x] = nivi[y][x];
        }
    }
}

void print_Niv(int niv[7][12], SDL_Window *window, SDL_Renderer *renderer, SDL_Surface *image, SDL_Texture *texture)
{
    print_Image("src/niveaux.jpg", window, renderer, image, texture, 0, 0);
    for (uint y = 1; y < 6; y++)
    {
        for (uint x = 0; x < 12; x++)
        {
            if (x == 0 && y == 1)
            {
                print_Image("src/bandemurgauche.png", window, renderer, image, texture, 0, 170);
            }
            if (!(x == 0 && y == 1) && !(x == 0 && y == 2))
            {
                switch (niv[y][x])
                {
                case VIDE:
                    print_Image("src/vide.png", window, renderer, image, texture, 34 * x, 170 + 34 * y);
                    break;
                case MUR:
                    print_Image("src/mur.jpg", window, renderer, image, texture, 34 * x, 170 + 34 * y);
                    break;
                case OBJECTIF:
                    print_Image("src/objectif.png", window, renderer, image, texture, 34 * x, 170 + 34 * y);
                    break;
                case MARIO_BAS:
                    print_Image("src/vide.png", window, renderer, image, texture, 34 * x, 170 + 34 * y);
                    print_Image("src/mario_bas.gif", window, renderer, image, texture, 34 * x, 170 + 34 * y);
                    break;
                case MARIO_HAUT:
                    print_Image("src/vide.png", window, renderer, image, texture, 34 * x, 170 + 34 * y);
                    print_Image("src/mario_haut.gif", window, renderer, image, texture, 34 * x, 170 + 34 * y);
                    break;
                case MARIO_GAUCHE:
                    print_Image("src/vide.png", window, renderer, image, texture, 34 * x, 170 + 34 * y);
                    print_Image("src/mario_gauche.gif", window, renderer, image, texture, 34 * x, 170 + 34 * y);
                    break;
                case MARIO_DROITE:
                    print_Image("src/vide.png", window, renderer, image, texture, 34 * x, 170 + 34 * y);
                    print_Image("src/mario_droite.gif", window, renderer, image, texture, 34 * x, 170 + 34 * y);
                    break;
                case CAISSE:
                    print_Image("src/caisse.jpg", window, renderer, image, texture, 34 * x, 170 + 34 * y);
                    break;
                case CAISSE_OK:
                    print_Image("src/caisse_ok.jpg", window, renderer, image, texture, 34 * x, 170 + 34 * y);
                    break;
                case CAISSE_BLOQUEE:
                    print_Image("src/caisse_bloquee.jpg", window, renderer, image, texture, 34 * x, 170 + 34 * y);
                    break;
                default:
                    SDL_ExitWithErreur("Affichage de chaque case");
                }
            }
        }
    }
    print_Image("src/bandemurhaut.png", window, renderer, image, texture, 34, 170);
    print_Image("src/bandemurbas.png", window, renderer, image, texture, 0, 374);
    SDL_RenderPresent(renderer);
}

void up_Mario_Niv(SDL_bool *program_launched_niveaux, SDL_bool *program_launched, uint* niv_reussi, int niv[7][12], position_Object *pos_Mario, position_Object objectif[], SDL_Window *window, SDL_Renderer *renderer, SDL_Surface *image, SDL_Texture *texture)
{
    if (niv[pos_Mario->y - 1][pos_Mario->x] == CAISSE)
    {
        pos_Mario->y--;
        for (uint i = 0; i < 5; i++)
        {
            if (structures_Equals(*pos_Mario, objectif[i * 2]))
            {
                niv[pos_Mario->y + 1][pos_Mario->x] = VIDE;
                niv[pos_Mario->y][pos_Mario->x] = MARIO_HAUT;
                niv[pos_Mario->y - 1][pos_Mario->x] = CAISSE_OK;
                print_Niv(niv, window, renderer, image, texture);
                SDL_Delay(500);
                if(2*i+1 ==1){
                    switch(jouer(program_launched_niveaux, program_launched, niv_reussi, map1, window, renderer, image, texture)){                       
                        case 0 :
                            pos_Mario->y++;
                            niv[pos_Mario->y-2][pos_Mario->x]=OBJECTIF;
                            niv[pos_Mario->y][pos_Mario->x]=MARIO_HAUT;
                            niv[pos_Mario->y-1][pos_Mario->x]=CAISSE;
                            print_Niv(niv, window, renderer, image, texture);
                                break;
                        case 1 :
                            pos_Mario->y++;
                            niv[pos_Mario->y-2][pos_Mario->x]=OBJECTIF;
                            niv[pos_Mario->y][pos_Mario->x]=MARIO_HAUT;
                            niv[pos_Mario->y-1][pos_Mario->x]=CAISSE;
                            niv[4][2]=CAISSE;
                            print_Niv(niv, window, renderer, image, texture);
                            SDL_Delay(500);
                            up_Mario_Niv(program_launched_niveaux, program_launched, niv_reussi, niv, pos_Mario, objectif, window, renderer, image, texture);
                            break;
                        case 2 :
                            pos_Mario->y++;
                            niv[pos_Mario->y-2][pos_Mario->x]=OBJECTIF;
                            niv[pos_Mario->y][pos_Mario->x]=MARIO_HAUT;
                            niv[pos_Mario->y-1][pos_Mario->x]=CAISSE;
                            niv[4][2]=CAISSE;
                            print_Niv(niv, window, renderer, image, texture);
                            break;
                        case 3 :
                            pos_Mario->y++;
                            niv[pos_Mario->y-2][pos_Mario->x]=OBJECTIF;
                            niv[pos_Mario->y][pos_Mario->x]=MARIO_HAUT;
                            niv[pos_Mario->y-1][pos_Mario->x]=CAISSE;
                            niv[4][2]=CAISSE;
                            print_Niv(niv, window, renderer, image, texture);
                            SDL_Delay(500);
                            down_Mario_Niv(program_launched_niveaux, program_launched, niv_reussi, niv, pos_Mario, objectif, window, renderer, image, texture);
                            break;
                        default :
                            break;
                    }
                }
                else if (2*i+1 == 3){
                    switch(jouer(program_launched_niveaux, program_launched, niv_reussi, map3, window, renderer, image, texture)){                       
                        case 0 :
                            pos_Mario->y++;
                            niv[pos_Mario->y-2][pos_Mario->x]=OBJECTIF;
                            niv[pos_Mario->y][pos_Mario->x]=MARIO_HAUT;
                            niv[pos_Mario->y-1][pos_Mario->x]=CAISSE;
                            print_Niv(niv, window, renderer, image, texture);
                                break;
                        case 1 :
                            pos_Mario->y++;
                            niv[pos_Mario->y-2][pos_Mario->x]=OBJECTIF;
                            niv[pos_Mario->y][pos_Mario->x]=MARIO_HAUT;
                            niv[pos_Mario->y-1][pos_Mario->x]=CAISSE;
                            niv[4][4]=CAISSE;
                            print_Niv(niv, window, renderer, image, texture);
                            SDL_Delay(500);
                            up_Mario_Niv(program_launched_niveaux, program_launched, niv_reussi, niv, pos_Mario, objectif, window, renderer, image, texture);
                            break;
                        case 2 :
                            pos_Mario->y++;
                            niv[pos_Mario->y-2][pos_Mario->x]=OBJECTIF;
                            niv[pos_Mario->y][pos_Mario->x]=MARIO_HAUT;
                            niv[pos_Mario->y-1][pos_Mario->x]=CAISSE;
                            niv[4][4]=CAISSE;
                            print_Niv(niv, window, renderer, image, texture);
                            break;
                        case 3 :
                            pos_Mario->y++;
                            niv[pos_Mario->y-2][pos_Mario->x]=OBJECTIF;
                            niv[pos_Mario->y][pos_Mario->x]=MARIO_HAUT;
                            niv[pos_Mario->y-1][pos_Mario->x]=CAISSE;
                            niv[4][4]=CAISSE;
                            print_Niv(niv, window, renderer, image, texture);
                            SDL_Delay(500);
                            down_Mario_Niv(program_launched_niveaux, program_launched, niv_reussi, niv, pos_Mario, objectif, window, renderer, image, texture);
                            break;
                        default :
                            break;
                    }
                }
                else if (2*i+1 == 5){
                    switch(jouer(program_launched_niveaux, program_launched, niv_reussi, map5, window, renderer, image, texture)){                       
                        case 0 :
                            pos_Mario->y++;
                            niv[pos_Mario->y-2][pos_Mario->x]=OBJECTIF;
                            niv[pos_Mario->y][pos_Mario->x]=MARIO_HAUT;
                            niv[pos_Mario->y-1][pos_Mario->x]=CAISSE;
                            print_Niv(niv, window, renderer, image, texture);
                                break;
                        case 1 :
                            pos_Mario->y++;
                            niv[pos_Mario->y-2][pos_Mario->x]=OBJECTIF;
                            niv[pos_Mario->y][pos_Mario->x]=MARIO_HAUT;
                            niv[pos_Mario->y-1][pos_Mario->x]=CAISSE;
                            niv[4][6]=CAISSE;
                            print_Niv(niv, window, renderer, image, texture);
                            SDL_Delay(500);
                            up_Mario_Niv(program_launched_niveaux, program_launched, niv_reussi, niv, pos_Mario, objectif, window, renderer, image, texture);
                            break;
                        case 2 :
                            pos_Mario->y++;
                            niv[pos_Mario->y-2][pos_Mario->x]=OBJECTIF;
                            niv[pos_Mario->y][pos_Mario->x]=MARIO_HAUT;
                            niv[pos_Mario->y-1][pos_Mario->x]=CAISSE;
                            niv[4][6]=CAISSE;
                            print_Niv(niv, window, renderer, image, texture);
                            break;
                        case 3 :
                            pos_Mario->y++;
                            niv[pos_Mario->y-2][pos_Mario->x]=OBJECTIF;
                            niv[pos_Mario->y][pos_Mario->x]=MARIO_HAUT;
                            niv[pos_Mario->y-1][pos_Mario->x]=CAISSE;
                            niv[4][6]=CAISSE;
                            print_Niv(niv, window, renderer, image, texture);
                            SDL_Delay(500);
                            down_Mario_Niv(program_launched_niveaux, program_launched, niv_reussi, niv, pos_Mario, objectif, window, renderer, image, texture);
                            break;
                        default :
                            break;
                    }
                }
                else if (2*i+1 == 7){
                    switch(jouer(program_launched_niveaux, program_launched, niv_reussi, map7, window, renderer, image, texture)){                       
                        case 0 :
                            pos_Mario->y++;
                            niv[pos_Mario->y-2][pos_Mario->x]=OBJECTIF;
                            niv[pos_Mario->y][pos_Mario->x]=MARIO_HAUT;
                            niv[pos_Mario->y-1][pos_Mario->x]=CAISSE;
                            print_Niv(niv, window, renderer, image, texture);
                                break;
                        case 1 :
                            pos_Mario->y++;
                            niv[pos_Mario->y-2][pos_Mario->x]=OBJECTIF;
                            niv[pos_Mario->y][pos_Mario->x]=MARIO_HAUT;
                            niv[pos_Mario->y-1][pos_Mario->x]=CAISSE;
                            print_Niv(niv, window, renderer, image, texture);
                            SDL_Delay(500);
                            up_Mario_Niv(program_launched_niveaux, program_launched, niv_reussi, niv, pos_Mario, objectif, window, renderer, image, texture);
                            break;
                        case 2 :
                            pos_Mario->y++;
                            niv[pos_Mario->y-2][pos_Mario->x]=OBJECTIF;
                            niv[pos_Mario->y][pos_Mario->x]=MARIO_HAUT;
                            niv[pos_Mario->y-1][pos_Mario->x]=CAISSE;
                            niv[4][8]=CAISSE;
                            print_Niv(niv, window, renderer, image, texture);
                            break;
                        case 3 :
                            pos_Mario->y++;
                            niv[pos_Mario->y-2][pos_Mario->x]=OBJECTIF;
                            niv[pos_Mario->y][pos_Mario->x]=MARIO_HAUT;
                            niv[pos_Mario->y-1][pos_Mario->x]=CAISSE;
                            niv[4][8]=CAISSE;
                            print_Niv(niv, window, renderer, image, texture);
                            SDL_Delay(500);
                            down_Mario_Niv(program_launched_niveaux, program_launched, niv_reussi, niv, pos_Mario, objectif, window, renderer, image, texture);
                            break;
                        default :
                            break;
                    }
                }
                /*else if (2*i+1 == 9){
                    switch(jouer(program_launched_niveaux, program_launched, niv_reussi, map9, window, renderer, image, texture)){                       
                        case 0 :
                            pos_Mario->y++;
                            niv[pos_Mario->y-2][pos_Mario->x]=OBJECTIF;
                            niv[pos_Mario->y][pos_Mario->x]=MARIO_HAUT;
                            niv[pos_Mario->y-1][pos_Mario->x]=CAISSE;
                            print_Niv(niv, window, renderer, image, texture);
                                break;
                        case 1 :
                            pos_Mario->y++;
                            niv[pos_Mario->y-2][pos_Mario->x]=OBJECTIF;
                            niv[pos_Mario->y][pos_Mario->x]=MARIO_HAUT;
                            niv[pos_Mario->y-1][pos_Mario->x]=CAISSE;
                            print_Niv(niv, window, renderer, image, texture);
                            SDL_Delay(500);
                            up_Mario_Niv(program_launched_niveaux, program_launched, niv_reussi, niv, pos_Mario, objectif, window, renderer, image, texture);
                            break;
                        case 2 :
                            pos_Mario->y++;
                            niv[pos_Mario->y-2][pos_Mario->x]=OBJECTIF;
                            niv[pos_Mario->y][pos_Mario->x]=MARIO_HAUT;
                            niv[pos_Mario->y-1][pos_Mario->x]=CAISSE;
                            niv[4][10]=CAISSE;
                            print_Niv(niv, window, renderer, image, texture);
                            break;
                        case 3 :
                            pos_Mario->y++;
                            niv[pos_Mario->y-2][pos_Mario->x]=OBJECTIF;
                            niv[pos_Mario->y][pos_Mario->x]=MARIO_HAUT;
                            niv[pos_Mario->y-1][pos_Mario->x]=CAISSE;
                            niv[4][10]=CAISSE;
                            print_Niv(niv, window, renderer, image, texture);

SDL_Delay(500);                            down_Mario_Niv(program_launched_niveaux, program_launched, niv_reussi, niv, pos_Mario, objectif, window, renderer, image, texture);
                            break;
                        default :
                            break;
                    }
                }*/
                else{
                    pos_Mario->y++;
                    niv[pos_Mario->y-2][pos_Mario->x]=OBJECTIF;
                    niv[pos_Mario->y][pos_Mario->x]=MARIO_HAUT;
                    niv[pos_Mario->y-1][pos_Mario->x]=CAISSE;
                    print_Niv(niv, window, renderer, image, texture);
                }
            }
        }
    } 
}

void down_Mario_Niv(SDL_bool *program_launched_niveaux, SDL_bool *program_launched, uint* niv_reussi, int niv[7][12], position_Object *pos_Mario, position_Object objectif[], SDL_Window *window, SDL_Renderer *renderer, SDL_Surface *image, SDL_Texture *texture)
{
    uint suite;
    if (niv[pos_Mario->y + 1][pos_Mario->x] == CAISSE)
    {
        pos_Mario->y++;
        for (uint j = 0; j < 5; j++)
        {
            if (structures_Equals(*pos_Mario, objectif[1 + (j * 2)]))
            {
                niv[pos_Mario->y - 1][pos_Mario->x] = VIDE;
                niv[pos_Mario->y][pos_Mario->x] = MARIO_BAS;
                niv[pos_Mario->y + 1][pos_Mario->x] = CAISSE_OK;
                print_Niv(niv, window, renderer, image, texture);
                SDL_Delay(500);
                if(j * 2 + 2 == 2){
                    switch(jouer(program_launched_niveaux, program_launched, niv_reussi, map2, window, renderer, image, texture)){                       
                        case 0 :
                            pos_Mario->y--;
                            niv[pos_Mario->y+2][pos_Mario->x]=OBJECTIF;
                            niv[pos_Mario->y][pos_Mario->x]=MARIO_HAUT;
                            niv[pos_Mario->y+1][pos_Mario->x]=CAISSE;
                            print_Niv(niv, window, renderer, image, texture);
                                break;
                        case 1 :
                            pos_Mario->y--;
                            niv[pos_Mario->y+2][pos_Mario->x]=OBJECTIF;
                            niv[pos_Mario->y][pos_Mario->x]=MARIO_HAUT;
                            niv[pos_Mario->y+1][pos_Mario->x]=CAISSE;
                            niv[2][4]=CAISSE;
                            print_Niv(niv, window, renderer, image, texture);
                            SDL_Delay(500);
                            down_Mario_Niv(program_launched_niveaux, program_launched, niv_reussi, niv, pos_Mario, objectif, window, renderer, image, texture);
                            break;
                        case 2 :
                            pos_Mario->y--;
                            niv[pos_Mario->y+2][pos_Mario->x]=OBJECTIF;
                            niv[pos_Mario->y][pos_Mario->x]=MARIO_HAUT;
                            niv[pos_Mario->y+1][pos_Mario->x]=CAISSE;
                            niv[2][4]=CAISSE;
                            print_Niv(niv, window, renderer, image, texture);
                            break;
                        case 3 :
                            pos_Mario->y--;
                            niv[pos_Mario->y+2][pos_Mario->x]=OBJECTIF;
                            niv[pos_Mario->y][pos_Mario->x]=MARIO_HAUT;
                            niv[pos_Mario->y+1][pos_Mario->x]=CAISSE;
                            niv[2][4]=CAISSE;
                            print_Niv(niv, window, renderer, image, texture);
                            SDL_Delay(500);
                            right_Mario_Niv(program_launched_niveaux, niv, pos_Mario, objectif, window, renderer, image, texture);
                            SDL_Delay(500);
                            right_Mario_Niv(program_launched_niveaux, niv, pos_Mario, objectif, window, renderer, image, texture);
                            SDL_Delay(500);
                            up_Mario_Niv(program_launched_niveaux, program_launched, niv_reussi, niv, pos_Mario, objectif, window, renderer, image, texture);
                            break;
                        default :
                            break;
                    }
                }
                else if(j * 2 + 2 == 4){
                    switch(jouer(program_launched_niveaux, program_launched, niv_reussi, map4, window, renderer, image, texture)){                       
                        case 0 :
                            pos_Mario->y--;
                            niv[pos_Mario->y+2][pos_Mario->x]=OBJECTIF;
                            niv[pos_Mario->y][pos_Mario->x]=MARIO_HAUT;
                            niv[pos_Mario->y+1][pos_Mario->x]=CAISSE;
                            print_Niv(niv, window, renderer, image, texture);
                                break;
                        case 1 :
                            pos_Mario->y--;
                            niv[pos_Mario->y+2][pos_Mario->x]=OBJECTIF;
                            niv[pos_Mario->y][pos_Mario->x]=MARIO_HAUT;
                            niv[pos_Mario->y+1][pos_Mario->x]=CAISSE;
                            niv[2][6]=CAISSE;
                            print_Niv(niv, window, renderer, image, texture);
                            SDL_Delay(500);
                            down_Mario_Niv(program_launched_niveaux, program_launched, niv_reussi, niv, pos_Mario, objectif, window, renderer, image, texture);
                            break;
                        case 2 :
                            pos_Mario->y--;
                            niv[pos_Mario->y+2][pos_Mario->x]=OBJECTIF;
                            niv[pos_Mario->y][pos_Mario->x]=MARIO_HAUT;
                            niv[pos_Mario->y+1][pos_Mario->x]=CAISSE;
                            niv[2][6]=CAISSE;
                            print_Niv(niv, window, renderer, image, texture);
                            break;
                        case 3 :
                            pos_Mario->y--;
                            niv[pos_Mario->y+2][pos_Mario->x]=OBJECTIF;
                            niv[pos_Mario->y][pos_Mario->x]=MARIO_HAUT;
                            niv[pos_Mario->y+1][pos_Mario->x]=CAISSE;
                            niv[2][6]=CAISSE;
                            print_Niv(niv, window, renderer, image, texture);
                            SDL_Delay(500);
                            right_Mario_Niv(program_launched_niveaux, niv, pos_Mario, objectif, window, renderer, image, texture);
                            SDL_Delay(500);
                            right_Mario_Niv(program_launched_niveaux, niv, pos_Mario, objectif, window, renderer, image, texture);
                            SDL_Delay(500);
                            up_Mario_Niv(program_launched_niveaux, program_launched, niv_reussi, niv, pos_Mario, objectif, window, renderer, image, texture);                            break;
                        default :
                            break;
                    }
                }
                else if(j * 2 + 2 == 6){
                    switch(jouer(program_launched_niveaux, program_launched, niv_reussi, map6, window, renderer, image, texture)){                       
                        case 0 :
                            pos_Mario->y--;
                            niv[pos_Mario->y+2][pos_Mario->x]=OBJECTIF;
                            niv[pos_Mario->y][pos_Mario->x]=MARIO_HAUT;
                            niv[pos_Mario->y+1][pos_Mario->x]=CAISSE;
                            print_Niv(niv, window, renderer, image, texture);
                                break;
                        case 1 :
                            pos_Mario->y--;
                            niv[pos_Mario->y+2][pos_Mario->x]=OBJECTIF;
                            niv[pos_Mario->y][pos_Mario->x]=MARIO_HAUT;
                            niv[pos_Mario->y+1][pos_Mario->x]=CAISSE;
                            niv[2][8]=CAISSE;
                            print_Niv(niv, window, renderer, image, texture);
                            SDL_Delay(500);
                            down_Mario_Niv(program_launched_niveaux, program_launched, niv_reussi, niv, pos_Mario, objectif, window, renderer, image, texture);
                            break;
                        case 2 :
                            pos_Mario->y--;
                            niv[pos_Mario->y+2][pos_Mario->x]=OBJECTIF;
                            niv[pos_Mario->y][pos_Mario->x]=MARIO_HAUT;
                            niv[pos_Mario->y+1][pos_Mario->x]=CAISSE;
                            niv[2][8]=CAISSE;
                            print_Niv(niv, window, renderer, image, texture);
                            break;
                        case 3 :
                            pos_Mario->y--;
                            niv[pos_Mario->y+2][pos_Mario->x]=OBJECTIF;
                            niv[pos_Mario->y][pos_Mario->x]=MARIO_HAUT;
                            niv[pos_Mario->y+1][pos_Mario->x]=CAISSE;
                            niv[2][8]=CAISSE;
                            print_Niv(niv, window, renderer, image, texture);
                            SDL_Delay(500);
                            right_Mario_Niv(program_launched_niveaux, niv, pos_Mario, objectif, window, renderer, image, texture);
                            SDL_Delay(500);
                            right_Mario_Niv(program_launched_niveaux, niv, pos_Mario, objectif, window, renderer, image, texture);
                            SDL_Delay(500);
                            up_Mario_Niv(program_launched_niveaux, program_launched, niv_reussi, niv, pos_Mario, objectif, window, renderer, image, texture);                            break;
                        default :
                            break;
                    }
                }
                /*else if(j * 2 + 2 == 8){
                    switch(jouer(program_launched_niveaux, program_launched, niv_reussi, map8, window, renderer, image, texture)){                       
                        case 0 :
                            pos_Mario->y--;
                            niv[pos_Mario->y+2][pos_Mario->x]=OBJECTIF;
                            niv[pos_Mario->y][pos_Mario->x]=MARIO_HAUT;
                            niv[pos_Mario->y+1][pos_Mario->x]=CAISSE;
                            print_Niv(niv, window, renderer, image, texture);
                                break;
                        case 1 :
                            pos_Mario->y--;
                            niv[pos_Mario->y+2][pos_Mario->x]=OBJECTIF;
                            niv[pos_Mario->y][pos_Mario->x]=MARIO_HAUT;
                            niv[pos_Mario->y+1][pos_Mario->x]=CAISSE;
                            niv[2][10]=CAISSE;
                            print_Niv(niv, window, renderer, image, texture);

SDL_Delay(500);                            down_Mario_Niv(program_launched_niveaux, program_launched, niv_reussi, niv, pos_Mario, objectif, window, renderer, image, texture);
                            break;
                        case 2 :
                            pos_Mario->y--;
                            niv[pos_Mario->y+2][pos_Mario->x]=OBJECTIF;
                            niv[pos_Mario->y][pos_Mario->x]=MARIO_HAUT;
                            niv[pos_Mario->y+1][pos_Mario->x]=CAISSE;
                            niv[2][10]=CAISSE;
                            print_Niv(niv, window, renderer, image, texture);
                            break;
                        case 3 :
                            pos_Mario->y--;
                            niv[pos_Mario->y+2][pos_Mario->x]=OBJECTIF;
                            niv[pos_Mario->y][pos_Mario->x]=MARIO_HAUT;
                            niv[pos_Mario->y+1][pos_Mario->x]=CAISSE;
                            niv[2][10]=CAISSE;
                            print_Niv(niv, window, renderer, image, texture);
                            SDL_Delay(500);
                            right_Mario_Niv(program_launched_niveaux, niv, pos_Mario, objectif, window, renderer, image, texture);
                            SDL_Delay(500);
                            right_Mario_Niv(program_launched_niveaux, niv, pos_Mario, objectif, window, renderer, image, texture);
                            SDL_Delay(500);
                            up_Mario_Niv(program_launched_niveaux, program_launched, niv_reussi, niv, pos_Mario, objectif, window, renderer, image, texture);                            break;
                        default :
                            break;
                    }
                }*/
                else if(j * 2 + 2 == 10){;
                    switch(jouer(program_launched_niveaux, program_launched, niv_reussi, map10, window, renderer, image, texture)){                       
                        case 0 :
                            pos_Mario->y--;
                            niv[pos_Mario->y+2][pos_Mario->x]=OBJECTIF;
                            niv[pos_Mario->y][pos_Mario->x]=MARIO_HAUT;
                            niv[pos_Mario->y+1][pos_Mario->x]=CAISSE;
                            print_Niv(niv, window, renderer, image, texture);
                                break;
                        case 1 :
                            pos_Mario->y--;
                            niv[pos_Mario->y+2][pos_Mario->x]=OBJECTIF;
                            niv[pos_Mario->y][pos_Mario->x]=MARIO_HAUT;
                            niv[pos_Mario->y+1][pos_Mario->x]=CAISSE;
                            print_Niv(niv, window, renderer, image, texture);
                       
                       SDL_Delay(500);     down_Mario_Niv(program_launched_niveaux, program_launched, niv_reussi, niv, pos_Mario, objectif, window, renderer, image, texture);
                            break;
                        case 2 :
                            pos_Mario->y--;
                            niv[pos_Mario->y+2][pos_Mario->x]=OBJECTIF;
                            niv[pos_Mario->y][pos_Mario->x]=MARIO_HAUT;
                            niv[pos_Mario->y+1][pos_Mario->x]=CAISSE;
                            niv[4][2]=CAISSE;
                            print_Niv(niv, window, renderer, image, texture);
                            break;
                        case 3 :
                            pos_Mario->y--;
                            niv[pos_Mario->y+2][pos_Mario->x]=OBJECTIF;
                            niv[pos_Mario->y][pos_Mario->x]=MARIO_HAUT;
                            niv[pos_Mario->y+1][pos_Mario->x]=CAISSE;
                            niv[4][2]=CAISSE;
                            print_Niv(niv, window, renderer, image, texture);
                            SDL_Delay(500);
                            right_Mario_Niv(program_launched_niveaux, niv, pos_Mario, objectif, window, renderer, image, texture);
                            SDL_Delay(500);
                            right_Mario_Niv(program_launched_niveaux, niv, pos_Mario, objectif, window, renderer, image, texture);
                            SDL_Delay(500);
                            up_Mario_Niv(program_launched_niveaux, program_launched, niv_reussi, niv, pos_Mario, objectif, window, renderer, image, texture);
                            break;
                        default :
                            break;
                    }
                }
                else{
                    pos_Mario->y--;
                    niv[pos_Mario->y+2][pos_Mario->x]=OBJECTIF;
                    niv[pos_Mario->y][pos_Mario->x]=MARIO_HAUT;
                    niv[pos_Mario->y+1][pos_Mario->x]=CAISSE;
                    print_Niv(niv, window, renderer, image, texture);
                }
            }
        }
    } 
}

void left_Mario_Niv(SDL_bool *program_launched_niveaux_page1, int niv[7][12], position_Object *pos_Mario, position_Object objectif[], SDL_Window *window, SDL_Renderer *renderer, SDL_Surface *image, SDL_Texture *texture)
{
    if (pos_Mario->x == 0)
    {
        *program_launched_niveaux_page1 = SDL_FALSE;
    }
    else
    {
        pos_Mario->x--;
        niv[pos_Mario->y][pos_Mario->x + 1] = VIDE;
        niv[pos_Mario->y][pos_Mario->x] = MARIO_GAUCHE;
        print_Niv(niv, window, renderer, image, texture);
    }
}

void right_Mario_Niv(SDL_bool *program_launched_niveaux_page1, int niv[7][12], position_Object *pos_Mario, position_Object objectif[], SDL_Window *window, SDL_Renderer *renderer, SDL_Surface *image, SDL_Texture *texture)
{
    if (pos_Mario->x == 11)
    {
        *program_launched_niveaux_page1 = SDL_FALSE;
    }
    else
    {
        pos_Mario->x++;
        niv[pos_Mario->y][pos_Mario->x - 1] = VIDE;
        niv[pos_Mario->y][pos_Mario->x] = MARIO_DROITE;
        print_Niv(niv, window, renderer, image, texture);
    }
}
