/*
Autor: ARTUR.
Data: 04/09/2013
*/
// Biblioteca de E/S.
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
//#include <sstream.h>

// Bibliotecas SDL.
#include "SDL/SDL.h"
#include "SDL/SDL_image.h"
#include "SDL/SDL_ttf.h"
#include "SDL/SDL_mixer.h"
//#include <SDL.h>

#define FRAMERATE 10

#define TRUE  1
#define FALSE 0

#define DIM 8
#define DIM2 999
#define P_BLOCO 5
#define TAMANHO_BLOCO 50

#define TEMPO_JOGO 180

#define BAIXO 200
#define MEDIO 500
#define ALTO 1000

#define TAMANHO_NOME 4
#define MAX_ARRAY_RECORDE 3

#define TAMANHO_NOME_ACHV 40
#define MAX_ARRAY_ACHV 9

int pontuacao=0; //SIM, global :D
SDL_Surface *screen;
Mix_Chunk *explosion_peca = NULL;
Mix_Chunk *click_som = NULL;
Mix_Chunk *peca_click_som = NULL;


//RECORDES-------------------------------------------
typedef struct 
{
    char nome_player[TAMANHO_NOME];
    int pontuacao_player;    

} RECORDE;

typedef struct 
{
    RECORDE vetor[MAX_ARRAY_RECORDE];
    int n;  

} ARRAY_RECORDES;
//RECORDES-------------------------------------------

//ACHVS-------------------------------------------
typedef struct 
{
    char *nome_achv;
    int have;    

} ACHV;

typedef struct 
{
    ACHV vetor[MAX_ARRAY_ACHV]; 

} ARRAY_ACHVS;

ARRAY_ACHVS g_array_achvs;
//ACHVS-------------------------------------------


typedef struct 
{
    SDL_Surface *redImagem;    
    SDL_Surface *blueImagem;    
    SDL_Surface *greenImagem;    
    SDL_Surface *pinkImagem;    
    SDL_Surface *yellowImagem;    
    SDL_Surface *orangeImagem;
    

} JEWELS;
typedef struct 
{   
    SDL_Rect block_rec;
    int block_numero;  

    SDL_Rect block_rect; 

} JEWELS_BLOCK;

typedef struct 
{
    SDL_Rect block_click;
    

} JEWELS_MOVES;

typedef struct 
{
    int primeiro, segundo;

} ELEMENTOS_NUM;

typedef struct 
{
    int repeticao;

    int vetor_verificador[DIM2];
    int contador_verificador;

    int vetor_apagador[DIM2];
    int contador_apagador;    

} VETORES_VERIFICADORES;

//PréJogo
int numero_Rand();
void criar_matriz(int [DIM][DIM]);

//DuranteJOGO--------------

//MUSICA----------
void toca_musica_menu();
void toca_musica_jogo();
void toca_efeito_explo(int);
void toca_efeito_click();
void toca_efeito_click_peca();

//ORIGINAL
void matriz_original(int [DIM][DIM], JEWELS *, SDL_Surface **, JEWELS_BLOCK [DIM*DIM]);
void explode_jewels(int [DIM][DIM], VETORES_VERIFICADORES*);
void animacao_block_descendo(int [DIM][DIM], VETORES_VERIFICADORES*, JEWELS *, SDL_Surface **, JEWELS_BLOCK [DIM*DIM]);
//AUX
void tranfere_para_aux(int [DIM][DIM], int [DIM][DIM], int,int, int,int, JEWELS_BLOCK [DIM*DIM], JEWELS *);
void tranfere_para_original(int [DIM][DIM], int [DIM][DIM]);
void matriz_aux(int [DIM][DIM], int [DIM][DIM], JEWELS_BLOCK [DIM*DIM], int, int ,int ,int, JEWELS *);
//Publico
void salva (VETORES_VERIFICADORES* , int , int );
int verifica (VETORES_VERIFICADORES* ,int , int );
void salva_apagar (VETORES_VERIFICADORES* , int, int );

//--------------

//Escrever na tela
void apply_surface( int , int , SDL_Surface* , SDL_Surface*  );


int intro_jogo(int *); // INTRODUÇÃO
int menu_inicial(int *);//MENU

int jogo_play(int *, int );


void defineImg_blocks(JEWELS *, int, JEWELS_BLOCK [DIM*DIM]);
void desenha_matriz(JEWELS *, SDL_Surface **, int [DIM][DIM], JEWELS_BLOCK [DIM*DIM]);

//GAME OVER
int game_over_analisar_moves(int [DIM][DIM], int[4]);
void no_more_moves_display();
void dica_function(JEWELS_BLOCK [DIM*DIM], int [DIM][DIM]);

//RECORDES--------------------------------------
int escrever_recorde(int *);
int mostrar_recorde(int *, int );

RECORDE elemento(ARRAY_RECORDES *, int);
int tamanho(ARRAY_RECORDES *);
void insere(ARRAY_RECORDES *, RECORDE);

void remove_posicao(ARRAY_RECORDES *, int);
void sort(ARRAY_RECORDES *);
void bubble_sort(RECORDE *, int);
//RECORDES-----------------------------------------

//ACHVS---------------------------------------------------
void cria_achvs();

void achvs (int [DIM][DIM], VETORES_VERIFICADORES **);

void demon_achv (int [DIM][DIM], VETORES_VERIFICADORES ***);
void anjo_achv (int [DIM][DIM], VETORES_VERIFICADORES ***);
void thriller_achv (int [DIM][DIM], VETORES_VERIFICADORES ***);
void et_achv (int [DIM][DIM], VETORES_VERIFICADORES ***);
void monstro_achv (int [DIM][DIM], VETORES_VERIFICADORES ***);
void robo_achv (int [DIM][DIM], VETORES_VERIFICADORES ***);

int mostrar_achvs(int *, int );
//ACHVS-------------------------------------------------------------

//ATOI
void itoa(int, char []);
void reverse(char []);



int init()
{
    //Initialize all SDL subsystems
    if( SDL_Init( SDL_INIT_EVERYTHING ) == -1 )
    {
        return 1;    
    }  
    
    //Initialize SDL_ttf
    if( TTF_Init() == -1 )
    {
        return 1;    
    }

    //Initialize SDL_mixer
    if( Mix_OpenAudio( 22050, MIX_DEFAULT_FORMAT, 2, 4096 ) == -1 )
    {
        return 1;    
    }

    SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO);
    
    //Set the window caption
    SDL_WM_SetCaption( "Apocalypto", NULL );


    srand( (unsigned)time(NULL) ); // seed do rand
    
    //If everything initialized fine
    return 0;
}

int main(int argc, char** argv)
{
    init();
    int quit  = 0; //Variavel que indica a saida do programa.

    // Chama Introdução
    intro_jogo(&quit);

    if (quit == 1)
    {
        return 0;
    }

    cria_achvs();

    //Chama Menu
    menu_inicial(&quit);

    return 0;
}


int intro_jogo(int *p_quit)
{
    //MUSICA=====================================================
    Mix_Music *intro_musica;
    intro_musica = Mix_LoadMUS( "./som/intro/Scary8-bitmusic.mp3" );
    Mix_FadeInMusic(intro_musica, -1, 2000);
    //MUSICA==========================================================

    int mouse_x = 0, mouse_y = 0;
    int flags = 0;

    int quit = 0;

    SDL_Surface* screen2;

    SDL_Event event; 

    SDL_Init(SDL_INIT_VIDEO);
    screen2 = SDL_SetVideoMode(800,600,16,flags);  

    SDL_Surface *cenario;
    cenario = IMG_Load( "./img/intro/map.png" );
    SDL_Surface *smoke;
    smoke = IMG_Load( "./img/intro/smoke.png" );
    SDL_Surface *fire;
    fire = IMG_Load( "./img/intro/fire.png" );
    SDL_Surface *old_man;
    old_man = IMG_Load( "./img/intro/oldman.png" );

    
    SDL_Rect smoke_rec; 
    smoke_rec.x = 0;
    smoke_rec.y = 0;
    smoke_rec.w = 34;
    smoke_rec.h = 34;
    SDL_Rect smoke_rect; 
    smoke_rect.x = 0;
    smoke_rect.y = 0;
    smoke_rect.w = 34;
    smoke_rect.h = 34;

    SDL_Rect fire_rec; 
    fire_rec.x = 0;
    fire_rec.y = 0;
    fire_rec.w = 26;
    fire_rec.h = 36;
    SDL_Rect fire_rect; 
    fire_rect.x = 0;
    fire_rect.y = 0;
    fire_rect.w = 26;
    fire_rect.h = 36;

    SDL_Rect old_man_rec; 
    old_man_rec.x = 0;
    old_man_rec.y = 0;
    old_man_rec.w = 30;
    old_man_rec.h = 32;
    SDL_Rect old_man_rect; 
    old_man_rect.x = 260;
    old_man_rect.y = 630;
    old_man_rect.w = 30;
    old_man_rect.h = 32;

    int verifica_x = 260, verifica_y = 630;


    //TEXTOS--------------------------------------------------------
    TTF_Font *font = NULL;
    TTF_Font *font_hist = NULL;
    SDL_Color textColor = { 255, 255, 255 };
    font = TTF_OpenFont( "./fontes/diablo_h.ttf", 28 );
    font_hist = TTF_OpenFont( "./fontes/diablo_h.ttf", 18 );

    //SKIP=====================
    SDL_Rect offset_skip;
    offset_skip.x = 20;
    offset_skip.y = 550;

    SDL_Surface *message = NULL;
    char text1[]="Skip";

    int contador_texto =0;
    //FIM SKIP=====================


    SDL_Surface *primeira_fala = NULL;
    char fala1[]="Enfim encontrei a setima igreja nessa ilha";

    primeira_fala = TTF_RenderText_Solid( font_hist, fala1, textColor );

    SDL_Surface *segunda_fala = NULL;
    char fala2[]="Nao pode ser, o livro ja foi aberto!";

    segunda_fala = TTF_RenderText_Solid( font_hist, fala2, textColor );

    SDL_Surface *terceira_fala = NULL;
    char fala3[]="Alguem tem que impedir isso, pois o tempo esta proximo!";

    terceira_fala = TTF_RenderText_Solid( font_hist, fala3, textColor );

    //TEXTOS--------------------------------------------------------


    //MOVIMENTOS-----(0 = nao completo ;; 1 = completo ;; 2 = em espera)---------------------------------------------------
    int mov1=0, mov2=2, mov3=2, mov4=2;

    //piscar no final
    int mov4_efeito=0;

    //pra ficar o texto um tempo
    int espera_mov1=0, espera_mov2=0, espera_mov3=0;

    //FIM MOVIMENTOS--------------------------------------------------------

    while (!quit)
    {
        int t1 = SDL_GetTicks(); 

        // Pega eventos do teclado/mouse.
        while(SDL_PollEvent(&event))
        {
          // Se fechou a janela.
            if (event.type == SDL_QUIT)
            {
                quit = TRUE;
                *p_quit = TRUE;
            } 

        }

        SDL_FillRect(screen2, NULL, SDL_MapRGB(screen2->format,6,6,6));

        SDL_BlitSurface( cenario, NULL, screen2, NULL );

        //smokes
        smoke_rect.x = 512;
        smoke_rect.y = 92;
        SDL_BlitSurface( smoke, &smoke_rec, screen2, &smoke_rect );
        smoke_rect.x = 556;
        smoke_rect.y = 86;
        SDL_BlitSurface( smoke, &smoke_rec, screen2, &smoke_rect );
        smoke_rect.x = 590;
        smoke_rect.y = 98;
        SDL_BlitSurface( smoke, &smoke_rec, screen2, &smoke_rect );
        smoke_rect.x = 514;
        smoke_rect.y = 128;
        SDL_BlitSurface( smoke, &smoke_rec, screen2, &smoke_rect );
        smoke_rect.x = 586;
        smoke_rect.y = 140;
        SDL_BlitSurface( smoke, &smoke_rec, screen2, &smoke_rect );
        smoke_rect.x = 630;
        smoke_rect.y = 106;
        SDL_BlitSurface( smoke, &smoke_rec, screen2, &smoke_rect );

        if (smoke_rec.x < 1360)
        {
            smoke_rec.x += 34;

        }else if (smoke_rec.x == 1360)
        {
            smoke_rec.x = 0;
        }

        //FOGO
        fire_rect.x = 595;
        fire_rect.y = 54;
        SDL_BlitSurface( fire, &fire_rec, screen2, &fire_rect );

        if (fire_rec.x < 52)
        {
            fire_rec.x += 26;

        }else if (fire_rec.x == 52)
        {
            fire_rec.x = 0;
        }

        //VELHO----------------------------------------------------------------
        
        //MOVIMENTO 1
        if (mov1 == 0)
        {
            if ( ((old_man_rect.x + 5) > 354) )
            {
                old_man_rect.x = 354;

            }else if (old_man_rect.x < 354)
            {
                old_man_rect.x += 5;

            }

            if ( old_man_rect.y - 5 < 258 )
            {
                old_man_rect.y = 258;

            }else if (old_man_rect.y > 258)
            {
                old_man_rect.y -= 5;

            }

            if (verifica_x != old_man_rect.x)
            {
                if (old_man_rec.x < 60)
                {
                    old_man_rec.x += 30;
                    old_man_rec.y = 0;

                }else if (old_man_rec.x == 60)
                {
                    old_man_rec.x = 0;
                    old_man_rec.y = 0;
                }

                verifica_x = old_man_rect.x;

            }else if (verifica_y != old_man_rect.y)
            {
                if (old_man_rec.x < 60)
                {
                    old_man_rec.x += 30;
                    old_man_rec.y = 32;

                }else if (old_man_rec.x == 60)
                {
                    old_man_rec.x = 0;
                    old_man_rec.y = 32;
                }

                verifica_y = old_man_rect.y;
            }

            if (old_man_rect.x == 354 && old_man_rect.y == 258)
            {
                if (espera_mov1 == 0)
                {
                    espera_mov1 = SDL_GetTicks();
                }

                if (t1 - espera_mov1 >= 5000)
                {
                    mov1 = 1;
                    mov2 = 0;
                }

                apply_surface( ( (screen2->w / 2) - (primeira_fala->w / 2)), ( (screen2->h) - (primeira_fala->h)), primeira_fala, screen2 );
            }

        }

        //MOVIMENTO 2
        if (mov2 == 0)
        {
            if ( ((old_man_rect.x + 5) > 550) )
            {
                old_man_rect.x = 550;

            }else if (old_man_rect.x < 550)
            {
                old_man_rect.x += 5;

            }

            if (verifica_x != old_man_rect.x)
            {
                if (old_man_rec.x < 60)
                {
                    old_man_rec.x += 30;
                    old_man_rec.y = 0;

                }else if (old_man_rec.x == 60)
                {
                    old_man_rec.x = 0;
                    old_man_rec.y = 0;
                }

                verifica_x = old_man_rect.x;

            }

            if (old_man_rect.x == 550 && old_man_rect.y == 258)
            {
                old_man_rec.y = 32;

                if (espera_mov2 == 0)
                {
                    espera_mov2 = SDL_GetTicks();
                }

                if (t1 - espera_mov2 >= 5000)
                {
                    mov2 = 1;
                    mov3 = 0;
                }

                apply_surface( ( (screen2->w / 2) - (segunda_fala->w / 2)), ( (screen2->h) - (segunda_fala->h)), segunda_fala, screen2 );
            }

        }

        //MOVIMENTO 3
        if (mov3 == 0)
        {
            if ( old_man_rect.y - 5 < 140 )
            {
                old_man_rect.y = 140;

            }else if (old_man_rect.y > 140)
            {
                old_man_rect.y -= 5;

            }

            if (verifica_y != old_man_rect.y)
            {
                if (old_man_rec.x < 60)
                {
                    old_man_rec.x += 30;
                    old_man_rec.y = 32;

                }else if (old_man_rec.x == 60)
                {
                    old_man_rec.x = 0;
                    old_man_rec.y = 32;
                }

                verifica_y = old_man_rect.y;
            }

            if (old_man_rect.x == 550 && old_man_rect.y == 140)
            {
                if (espera_mov3 == 0)
                {
                    espera_mov3 = SDL_GetTicks();
                }

                if (t1 - espera_mov3 >= 5000)
                {
                    mov3 = 1;
                    mov4 = 0;
                }

                apply_surface( ( (screen2->w / 2) - (terceira_fala->w / 2)), ( (screen2->h) - (terceira_fala->h)), terceira_fala, screen2 );
            }

        }


        SDL_BlitSurface( old_man, &old_man_rec, screen2, &old_man_rect );

        //Tela piscando
        if (mov4 == 0)
        {
            if (mov4_efeito < 10)
            {
                mov4_efeito += 1;

                if (mov4_efeito%2 == 0)
                {
                    SDL_FillRect(screen2, NULL, SDL_MapRGB(screen2->format,6,6,6));
                }else
                {
                    SDL_FillRect(screen2, NULL, SDL_MapRGB(screen2->format,200,200,200));
                }

            }else
            {
                SDL_FreeSurface(screen2);
                SDL_FreeSurface(cenario);
                SDL_FreeSurface(smoke);
                SDL_FreeSurface(fire);
                SDL_FreeSurface(old_man);
                SDL_FreeSurface(primeira_fala);
                SDL_FreeSurface(segunda_fala);
                SDL_FreeSurface(terceira_fala);
                
                return 0;
            }
            
        }

        //SKip
        message = TTF_RenderText_Solid( font, text1, textColor );

        if (contador_texto < 50)
        {
            textColor.r -= 4;
            textColor.g -= 4;
            textColor.b -= 4;   

            contador_texto += 1;

            
        }else if (contador_texto == 50)
        {
            textColor.r += 4;
            textColor.g += 4;
            textColor.b += 4;

            if (textColor.r == 255 && textColor.g == 255 && textColor.b == 255)
            {
                contador_texto = 0;
            }
                
        } 

        //TEXTO
        SDL_BlitSurface( message, NULL, screen2, &offset_skip );


        if( event.type == SDL_MOUSEBUTTONUP )
        {                
            if( event.button.button == SDL_BUTTON_LEFT)
            {

                //Get the mouse offsets
                mouse_x = event.button.x;
                mouse_y = event.button.y;
                
                //Se apertou SKIP
                if( ( mouse_x > offset_skip.x ) && ( mouse_x < (offset_skip.x + 40 ) ) && ( mouse_y > offset_skip.y ) && ( mouse_y < (offset_skip.y + 28) ) )
                {                    
                    quit = TRUE;

                } 
            }

            event.button.type = -1;
        }


        SDL_Flip(screen2);

        int t2 = SDL_GetTicks(); 
        int wait = t2 - t1; 
        wait = (1000/FRAMERATE) - wait; 
       
        if(wait > 0)
        { 
            SDL_Delay(wait);

        } 

    }
    
    return 0;
}
int menu_inicial(int *p_quit)
{
    toca_musica_menu();

    int mouse_x = 0, mouse_y = 0;
    int flags = 0;

    int quit = 0;

    SDL_Surface* screen_menu;

    SDL_Event event; 

    SDL_Init(SDL_INIT_VIDEO);
    screen_menu = SDL_SetVideoMode(800,600,16,flags);  


    //FUNDO ~~~~~~~~~~~~~~~~~~~
    SDL_Surface *fundo_cena;
    fundo_cena = IMG_Load( "./img/fundo/fundomenu.png" );
    SDL_Rect fundo_cena_rec; 
    fundo_cena_rec.x = 0;
    fundo_cena_rec.y = 0;
    fundo_cena_rec.w = 800;
    fundo_cena_rec.h = 600;

    SDL_Surface *fundo_ceu;
    fundo_ceu = IMG_Load( "./img/fundo/fundomenu2.png" );
    SDL_Rect fundo_ceu_rec; 
    fundo_ceu_rec.x = 0;
    fundo_ceu_rec.y = 0;
    fundo_ceu_rec.w = 800;
    fundo_ceu_rec.h = 600;

    int fundo_efeito1=0, fundo_efeito2=0, fundo_efeito_contador=0;
    int ultimo_rando=0;

    SDL_Surface *meteor;
    meteor = IMG_Load( "./img/fundo/meteor.png" );
    SDL_Rect meteor_rect; 
    meteor_rect.x = 0;
    meteor_rect.y = 0;

    //FIM FUNDO ~~~~~~~~~~~~~~~~~~~

    SDL_Surface *logo;
    logo = IMG_Load( "./img/logo/logo.png" );
    SDL_Surface *vaca;
    vaca = IMG_Load( "./img/logo/vaca_logo.png" );
    
    SDL_Rect logo_rect; 
    logo_rect.x = 620;
    logo_rect.y = 100;
    logo_rect.w = 22;
    logo_rect.h = 17;

    int vaca_move = 0;

    SDL_Rect logo_rect2; 
    logo_rect2.x = 75;
    logo_rect2.y = 10;
    logo_rect2.w = 650;
    logo_rect2.h = 147;


    //TEXTOS--------------------------------------------------------
    TTF_Font *font = NULL;
    SDL_Color textColor = { 184, 227, 41 };
    font = TTF_OpenFont( "./fontes/mkda.ttf", 35 );

    SDL_Surface *jogar_message = NULL;
    SDL_Rect offset_jogar;
    offset_jogar.x = 310;
    offset_jogar.y = 210;

    SDL_Surface *recordes_message = NULL;
    SDL_Rect offset_recordes;
    offset_recordes.x = 310;
    offset_recordes.y = 290;

    SDL_Surface *conquistas_message = NULL;
    SDL_Rect offset_conquistas;
    offset_conquistas.x = 310;
    offset_conquistas.y = 370; 
    
    char jogar[]="Jogar";
    char recordes[]="Recordes";
    char conquistas[]="Conquistas";


    jogar_message = TTF_RenderText_Solid( font, jogar, textColor );
    recordes_message = TTF_RenderText_Solid( font, recordes, textColor );
    conquistas_message = TTF_RenderText_Solid( font, conquistas, textColor );

    //icons ~~~~~~~~~~~~~~~~~~
    SDL_Surface *icon_menu;
    icon_menu = IMG_Load( "./img/menu/menu_icons.png" );

    SDL_Rect icon_jogar_rec; 
    icon_jogar_rec.x = 0;
    icon_jogar_rec.y = 0;
    icon_jogar_rec.w = 60;
    icon_jogar_rec.h = 60;
    SDL_Rect icon_jogar_rect; 
    icon_jogar_rect.x = 240;
    icon_jogar_rect.y = 200;

    SDL_Rect icon_recordes_rec; 
    icon_recordes_rec.x = 0;
    icon_recordes_rec.y = 61;
    icon_recordes_rec.w = 60;
    icon_recordes_rec.h = 60;
    SDL_Rect icon_recordes_rect; 
    icon_recordes_rect.x = 240;
    icon_recordes_rect.y = 280;

    SDL_Rect icon_conquistas_rec; 
    icon_conquistas_rec.x = 0;
    icon_conquistas_rec.y = 122;
    icon_conquistas_rec.w = 60;
    icon_conquistas_rec.h = 60;
    SDL_Rect icon_conquistas_rect; 
    icon_conquistas_rect.x = 240;
    icon_conquistas_rect.y = 360;

    //TEXTOS--------------------------------------------------------


    while (!quit)
    {

        
        int t1 = SDL_GetTicks();
        fundo_efeito1 = SDL_GetTicks();
        pontuacao = 0;

        // Pega eventos do teclado/mouse.
        while(SDL_PollEvent(&event))
        //while(SDL_WaitEvent (&event))
        {
          // Se fechou a janela.
            if (event.type == SDL_QUIT)
            {
                *p_quit = TRUE;
                quit = TRUE;
            } 

        }

        SDL_FillRect(screen_menu, NULL, SDL_MapRGB(screen_menu->format,255,255,255));

        //BLIT FUNDO 
        SDL_BlitSurface( fundo_ceu, &fundo_ceu_rec, screen_menu, NULL );
        SDL_BlitSurface( meteor, NULL, screen_menu, &meteor_rect );

        SDL_BlitSurface( fundo_cena, &fundo_cena_rec, screen_menu, NULL );
        

        if ((fundo_efeito1 - fundo_efeito2) >= 250)
        {
            if (fundo_efeito_contador < 3)
            {
                fundo_cena_rec.x += 800;
                fundo_efeito_contador += 1;

            }else if (fundo_efeito_contador >= 3)
            {
                fundo_cena_rec.x -= 800;

                if (fundo_cena_rec.x == 0)
                {
                    fundo_efeito_contador = 0;
                }
            }

            fundo_efeito2 = SDL_GetTicks();
        }

        if ((fundo_efeito1 - fundo_efeito2) >= 50)
        {
            if (meteor_rect.x < 125)
            {
                meteor_rect.x += 3;
                meteor_rect.y += 2;
            }else
            {
                

                meteor_rect.x = 0;

                do
                {
                    meteor_rect.y = rand()%50;

                } while (meteor_rect.y > (ultimo_rando + 20) || meteor_rect.y < (ultimo_rando - 20));    

                ultimo_rando = meteor_rect.y;
;
            }
        }


        SDL_BlitSurface( logo, NULL, screen_menu, &logo_rect2 );

        //movimento VACA~~~~~~~~~
        SDL_BlitSurface( vaca, NULL, screen_menu, &logo_rect );

        if (vaca_move <= 45)
        {
            logo_rect.y -= 2;
            vaca_move += 2;

        }else if (vaca_move > 45)
        {
            logo_rect.y += 2;

            if (logo_rect.y == 100)
            {
                vaca_move = 0;
            }
        }


        //TEXTO e Icons
        SDL_BlitSurface( icon_menu, &icon_jogar_rec, screen_menu, &icon_jogar_rect );
        SDL_BlitSurface( jogar_message, NULL, screen_menu, &offset_jogar );

        SDL_BlitSurface( icon_menu, &icon_recordes_rec, screen_menu, &icon_recordes_rect );
        SDL_BlitSurface( recordes_message, NULL, screen_menu, &offset_recordes );

        SDL_BlitSurface( icon_menu, &icon_conquistas_rec, screen_menu, &icon_conquistas_rect );
        SDL_BlitSurface( conquistas_message, NULL, screen_menu, &offset_conquistas );     




        if( event.type == SDL_MOUSEBUTTONUP )
        {                

            if( event.button.button == SDL_BUTTON_LEFT)
            {

                //Get the mouse offsets
                mouse_x = event.button.x;
                mouse_y = event.button.y;
                
                //JOGAR
                if( ( mouse_x > icon_jogar_rect.x ) && ( mouse_x < ( offset_jogar.x + offset_jogar.w) ) && ( mouse_y > icon_jogar_rect.y ) && ( mouse_y < (icon_jogar_rect.y + icon_jogar_rect.h) ) )
                {

                    toca_efeito_click();
                    jogo_play(&quit, flags);                    
                    toca_musica_menu();

                }else //RECORDES
                if( ( mouse_x > icon_recordes_rect.x ) && ( mouse_x < ( offset_recordes.x + offset_recordes.w) ) && ( mouse_y > icon_recordes_rect.y ) && ( mouse_y < (icon_recordes_rect.y + icon_recordes_rect.h) ) )
                {   
                    toca_efeito_click();
                    mostrar_recorde(&quit, flags);                   

                } else //ACHV
                if( ( mouse_x > icon_conquistas_rect.x ) && ( mouse_x < ( offset_conquistas.x + offset_conquistas.w) ) && ( mouse_y > icon_conquistas_rect.y ) && ( mouse_y < (icon_conquistas_rect.y + icon_conquistas_rect.h) ) )
                {
                    toca_efeito_click();    
                    mostrar_achvs(&quit, flags);            

                }

            }            

            event.button.type = -1;
        }

        SDL_Flip(screen_menu);
       
        int t2 = SDL_GetTicks(); 
        int wait = t2 - t1; 
        wait = (1000/FRAMERATE) - wait;        
       
        if(wait > 0)
        { 
            SDL_Delay(wait);

        }
        

    }
    
    return 0;
}

void toca_musica_menu()
{
    //MUSIC========================================================================
    
    Mix_Music *menu_bib;
    menu_bib = Mix_LoadMUS( "./som/menu/backinblack.mp3" );

    Mix_Music *menu_hth;
    menu_hth = Mix_LoadMUS( "./som/menu/highwaytohell.mp3" );

    Mix_Music *menu_wttj;
    menu_wttj = Mix_LoadMUS( "./som/menu/WelcomeToTheJungle.mp3" );    

    int musica_tocar;

    musica_tocar = rand()%3;

    if (musica_tocar == 0)
    {
        Mix_FadeInMusic(menu_bib, -1, 5000);

    }else if (musica_tocar == 1)
    {
        Mix_FadeInMusic(menu_wttj, -1, 5000);
        Mix_SetMusicPosition(3.0);

    }else
    {
        Mix_FadeInMusic(menu_hth, -1, 5000);
        Mix_SetMusicPosition(3.0);
        
    }

    //MUSIC========================================================================
}

void toca_musica_jogo()
{
    //MUSIC========================================================================
    Mix_Music *jogo_cslt;
    jogo_cslt = Mix_LoadMUS( "./som/jogo/CircaSurviveLivingTogether.mp3" );

    Mix_Music *jogo_rpa;
    jogo_rpa = Mix_LoadMUS( "./som/jogo/RadioheadParanoidAndroid.mp3" );

    int musica_tocar;

    musica_tocar = rand()%2;

    if (musica_tocar == 0)
    {
        Mix_FadeInMusic(jogo_cslt, -1, 2000);

    }else if (musica_tocar == 1)
    {
        Mix_FadeInMusic(jogo_rpa, -1, 2000);

    }

    //MUSIC========================================================================

}

void toca_efeito_explo(int tipo)
{
    //MUSIC========================================================================

    Mix_FreeChunk( explosion_peca );

    if (tipo == 1)
    {
        explosion_peca = Mix_LoadWAV( "./som/efeitos/Teleport.wav" );

        Mix_PlayChannel( 0, explosion_peca, 0 );

    }else if (tipo == 2)
    {
        explosion_peca = Mix_LoadWAV(  "./som/efeitos/explosion.wav" );

        Mix_PlayChannel( 0, explosion_peca, 0 );

    }else if (tipo == 3)
    {
        explosion_peca = Mix_LoadWAV(  "./som/efeitos/Zombie3.wav" );

        Mix_PlayChannel( 0, explosion_peca, 0 );

    }else if (tipo == 4)
    {
        explosion_peca = Mix_LoadWAV(  "./som/efeitos/Teleport.wav" );

        Mix_PlayChannel( 0, explosion_peca, 0 );

    }else if (tipo == 5)
    {
        explosion_peca = Mix_LoadWAV(  "./som/efeitos/Explosion5.flac" );

        Mix_PlayChannel( 0, explosion_peca, 0 );


    }else if (tipo == 6)
    {
        explosion_peca = Mix_LoadWAV(  "./som/efeitos/8Bitrobot.wav" );

        Mix_PlayChannel( 0, explosion_peca, 0 );
        
    }    

    //MUSIC========================================================================

}
void toca_efeito_click()
{
    //MUSIC========================================================================

    Mix_FreeChunk( click_som );

    click_som = Mix_LoadWAV( "./som/click.wav" );

    Mix_PlayChannel( 0, click_som, 0 );
 

    //MUSIC========================================================================

}
void toca_efeito_click_peca()
{
    //MUSIC========================================================================

    Mix_FreeChunk( peca_click_som );

    peca_click_som = Mix_LoadWAV( "./som/laser.wav" );

    Mix_PlayChannel( 0, peca_click_som, 0 );
 

    //MUSIC========================================================================

}


int jogo_play(int *quit, int flags)
{     
    toca_musica_jogo();

    //GAMBIARRA
    int vetor_gambiarra[4];

    //Messages-------------------------
    TTF_Font *font = NULL;
    SDL_Color textColor = { 255, 255, 255 };
    font = TTF_OpenFont( "./fontes/diablo_h.ttf", 28 );

    //PONTO
    char ponto_int[10];
 
    //TEMPO
    char tempo_int[3];

    //Messages FIM----------------------------

    //TIME-------------------------------
    int tempo_total = TEMPO_JOGO + (SDL_GetTicks() / 1000);

    int running = 1;
    int time_aux = 0;

    //TIME FIM-----------------------------

    int click_jewel=0;
    int mouse_x = 0, mouse_y = 0, motion_x = 0, motion_y = 0;
    int linha_click=55, coluna_click=55, clicks=0, linha2_click=55, coluna2_click=55 ;
    int linha_motion =55, coluna_motion =55;
    int matriz[DIM][DIM];
    int matriz_aux[DIM][DIM];

    criar_matriz(matriz);

    int rec_tam = 50;

    SDL_Rect block_clicked;

    JEWELS jewels;
    JEWELS_MOVES jewels_move;
    ELEMENTOS_NUM elementos_num;

    JEWELS_BLOCK vetor_jewels_block[DIM*DIM];

    defineImg_blocks(&jewels, rec_tam, vetor_jewels_block);        

    jewels_move.block_click.x = 0;
    jewels_move.block_click.y = 50;
    jewels_move.block_click.w = 50;
    jewels_move.block_click.h = 50;

    
    SDL_Event event;    
    
    // Inicializa SDL video.
    SDL_Init(SDL_INIT_VIDEO);
    screen = SDL_SetVideoMode(800,600,16,flags);

    SDL_Rect tab_rect; 
    tab_rect.x = 300;
    tab_rect.y = 100;
    tab_rect.w = 400;
    tab_rect.h = 400;

    //IMG -- FUNDO, ICONS-----------------------------------------------------
    //FUNDO ~~~~~~~~~~~~~~~~~~~
    SDL_Surface *fundo;
    fundo = IMG_Load( "./img/fundo/fundojogo.png" );
    SDL_Rect fundo_rec; 
    fundo_rec.x = 0;
    fundo_rec.y = 0;
    fundo_rec.w = 800;
    fundo_rec.h = 600;

    int fundo_efeito1=0, fundo_efeito2=0, fundo_efeito_contador=0;

    SDL_Surface *fundo_pause;
    fundo_pause = IMG_Load( "./img/fundo/fundopause.png" );
    SDL_Rect fundo_pause_rect; 
    fundo_pause_rect.x = 0;
    fundo_pause_rect.y = 0;
    fundo_pause_rect.w = 800;
    fundo_pause_rect.h = 600;

    //Icons ~~~~~~~~~~~~~~~~~~~~
    SDL_Surface *icon_menu;
    icon_menu = IMG_Load( "./img/icons_jogo/menu.png" );
    SDL_Rect icon_menu_rec; 
    icon_menu_rec.x = 0;
    icon_menu_rec.y = 0;
    icon_menu_rec.w = 110;
    icon_menu_rec.h = 100;
    SDL_Rect icon_menu_rect; 
    icon_menu_rect.x = 0;
    icon_menu_rect.y = 0;

    SDL_Surface *icon_pause;
    icon_pause = IMG_Load( "./img/icons_jogo/pause.png" );
    SDL_Rect icon_pause_rec; 
    icon_pause_rec.x = 0;
    icon_pause_rec.y = 0;
    icon_pause_rec.w = 64;
    icon_pause_rec.h = 64;
    SDL_Rect icon_pause_rect; 
    icon_pause_rect.x = 0;
    icon_pause_rect.y = 200;

    SDL_Surface *icon_dica;
    icon_dica = IMG_Load( "./img/icons_jogo/dica.png" );
    SDL_Rect icon_dica_rec; 
    icon_dica_rec.x = 0;
    icon_dica_rec.y = 0;
    icon_dica_rec.w = 62;
    icon_dica_rec.h = 62;
    SDL_Rect icon_dica_rect; 
    icon_dica_rect.x = 0;
    icon_dica_rect.y = 280;

    SDL_Surface *icon_score;
    icon_score = IMG_Load( "./img/icons_jogo/score.png" );
    SDL_Rect icon_score_rec; 
    icon_score_rec.x = 0;
    icon_score_rec.y = 0;
    icon_score_rec.w = 49;
    icon_score_rec.h = 49;
    SDL_Rect icon_score_rect; 
    icon_score_rect.x = 300;
    icon_score_rect.y = 25;

    SDL_Surface *icon_time;
    icon_time = IMG_Load( "./img/icons_jogo/time.png" );
    SDL_Rect icon_time_rect; 
    icon_time_rect.x = 500;
    icon_time_rect.y = 15;

    int zil_efeito = 0;

    //IMG -- FUNDO, ICONS-----------------------------------------------------  


    while (!*quit)
    {
        int t1 = SDL_GetTicks(); 
        fundo_efeito1 = SDL_GetTicks(); 

        // Pega eventos do teclado/mouse.
        while(SDL_PollEvent(&event))
        {
          // Se fechou a janela.
            if (event.type == SDL_QUIT)
            {
                *quit = TRUE;
                return 0;

            } 

        }

        int tempo_verificador; //Sem isso o time fica com bug, quando faz um achv em 2 segundos ou menos o tempo fica infinito, parece que nao tava reconhecendo: (tempo_total - (SDL_GetTicks() / 1000)) <= 0

        //CRIA==============================

        SDL_Surface *message_tempo_int = NULL;
        SDL_Surface *message_ponto_int = NULL;

        itoa(pontuacao, ponto_int);
        message_ponto_int = TTF_RenderText_Solid( font, ponto_int, textColor );

        if (running == 1)
        {
            tempo_verificador = (tempo_total - (SDL_GetTicks() / 1000));
            itoa((tempo_total - (SDL_GetTicks() / 1000)), tempo_int);
            message_tempo_int = TTF_RenderText_Solid( font, tempo_int, textColor );
        }else
        {
            tempo_verificador = time_aux;
            itoa(time_aux, tempo_int);
            message_tempo_int = TTF_RenderText_Solid( font, tempo_int, textColor );
        }

        //Se acabou o tempo...
        if ( tempo_verificador <= 0)
        {
            if (time_aux == 0)
            {
                escrever_recorde(quit);
                return 0;   
            }
                     
        }      


        SDL_FillRect(screen, NULL, SDL_MapRGB(screen->format,255,255,255));


        //Blit FUNDO -------------------------------------------,

        SDL_BlitSurface( fundo, &fundo_rec, screen, NULL );  

        if ((fundo_efeito1 - fundo_efeito2) >= 250)
        {
            if (fundo_efeito_contador < 3)
            {
                fundo_rec.x += 800;
                fundo_efeito_contador += 1;

            }else if (fundo_efeito_contador >= 3)
            {
                fundo_rec.x -= 800;

                if (fundo_rec.x == 0)
                {
                    fundo_efeito_contador = 0;
                }
            }

            fundo_efeito2 = SDL_GetTicks();
        }

        //Blit FUNDO -------------------------------------------
       
        apply_surface( 375, 35, message_ponto_int, screen );
        
        apply_surface( 575, 35, message_tempo_int, screen );


        //BLIT ICONS--------------------------------------------------------------

        SDL_BlitSurface( icon_menu, &icon_menu_rec, screen, &icon_menu_rect ); 

        SDL_BlitSurface( icon_pause, &icon_pause_rec, screen, &icon_pause_rect ); 
        SDL_BlitSurface( icon_dica, &icon_dica_rec, screen, &icon_dica_rect ); 

        SDL_BlitSurface( icon_score, &icon_score_rec, screen, &icon_score_rect ); 
        SDL_BlitSurface( icon_time, NULL, screen, &icon_time_rect ); 

        //Efeito Zil~~~~~
        if (zil_efeito <= 20)
        {
            icon_time_rect.y += 2;
            zil_efeito += 2;
        
        }else if (zil_efeito > 20)
        {
            icon_time_rect.y -= 2;

            if (icon_time_rect.y == 15)
            {
                zil_efeito = 0;
            }
        } 

        if (pontuacao > ALTO)
        {
            icon_score_rec.x = 100;
        }else
        if (pontuacao > MEDIO)
        {
            icon_score_rec.x = 50;
        }

        //BLIT ICONS--------------------------------------------------------------  

        //PARA funcionar o mouse over------------------------------------
        defineImg_blocks(&jewels, rec_tam, vetor_jewels_block); 

        icon_menu_rec.x = 0;
        icon_dica_rec.x = 0;
        icon_dica_rec.x = 0;


        if( event.type == SDL_MOUSEMOTION )
        {

            motion_x = event.motion.x;
            motion_y = event.motion.y;
            
            //If the mouse is over the button
            if( ( motion_x > tab_rect.x ) && ( motion_x < (tab_rect.x + tab_rect.w) ) && ( motion_y > tab_rect.y ) && ( motion_y < (tab_rect.y + tab_rect.h) ) )
            {
                linha_motion = ( (motion_y - tab_rect.y) / rec_tam );
                coluna_motion = ( (motion_x - tab_rect.x) / rec_tam );

                vetor_jewels_block[((DIM-1) * linha_motion) + linha_motion + coluna_motion].block_rec.x = 50; 
                vetor_jewels_block[((DIM-1) * linha_motion) + linha_motion + coluna_motion].block_rec.y = 50;
                   
            }else 
            if( ( motion_x > icon_menu_rect.x ) && ( motion_x < (icon_menu_rect.x + icon_menu_rec.w) ) && ( motion_y > icon_menu_rect.y ) && ( motion_y < (icon_menu_rect.y + icon_menu_rec.h) ) )
            {
                icon_menu_rec.x = 112;
            }else 
            if( ( motion_x > icon_dica_rect.x ) && ( motion_x < (icon_dica_rect.x + icon_dica_rec.w) ) && ( motion_y > icon_dica_rect.y ) && ( motion_y < (icon_dica_rect.y + icon_dica_rec.h) ) )
            {
                icon_dica_rec.x = 62;

            }
              
        } 

        

        desenha_matriz(&jewels, &screen, matriz, vetor_jewels_block);

        matriz_original(matriz, &jewels, &screen, vetor_jewels_block);
        


        if( event.button.type == SDL_MOUSEBUTTONUP )
        {
            if( event.button.button == SDL_BUTTON_LEFT)
            {
                //Get the mouse offsets
                mouse_x = event.button.x;
                mouse_y = event.button.y;
                
                //Se clicou na matriz
                if( ( mouse_x > tab_rect.x ) && ( mouse_x < (tab_rect.x + tab_rect.w) ) && ( mouse_y > tab_rect.y ) && ( mouse_y < (tab_rect.y + tab_rect.h) ) )
                {
                    running = 1; 

                    toca_efeito_click_peca();                   

                    if (clicks == 0)
                    {                        
                        click_jewel = click_jewel == 1 ? 0 : 1;

                        linha_click = ( (mouse_y - tab_rect.y) / rec_tam );
                        coluna_click = ( (mouse_x - tab_rect.x) / rec_tam );

                        elementos_num.primeiro = matriz[linha_click][coluna_click];
                        clicks += 1;

                        block_clicked.x = (( (mouse_x - tab_rect.x) / rec_tam ) * rec_tam) +tab_rect.x;
                        block_clicked.y = (( (mouse_y - tab_rect.y) / rec_tam ) * rec_tam) +tab_rect.y;
                        block_clicked.w = 50;
                        block_clicked.h = 50;

                    }else if (clicks == 1)
                    {
                        if ( ( (mouse_y - tab_rect.y) / rec_tam ) != linha_click || ( (mouse_x - tab_rect.x) / rec_tam ) != coluna_click )
                        {
                            // São vizinhos por coluna?!
                            if ( ( (mouse_y - tab_rect.y) / rec_tam ) == linha_click && ( ( (mouse_x - tab_rect.x) / rec_tam ) == coluna_click - 1 || ( (mouse_x - tab_rect.x) / rec_tam ) == coluna_click + 1  ))
                            {
                                click_jewel = 0;

                                linha2_click = ( (mouse_y - tab_rect.y) / rec_tam );
                                coluna2_click = ( (mouse_x - tab_rect.x) / rec_tam );

                                elementos_num.segundo = matriz[linha2_click][coluna2_click];
                                clicks -= 1;

                                tranfere_para_aux(matriz, matriz_aux, linha_click,coluna_click, linha2_click,coluna2_click, vetor_jewels_block, &jewels);

                            }else
                            // São vizinhos por linha?!
                            if ( ( (mouse_x - tab_rect.x) / rec_tam ) == coluna_click && ( ( (mouse_y - tab_rect.y) / rec_tam ) == linha_click - 1 || ( (mouse_y - tab_rect.y) / rec_tam ) == linha_click + 1  ))
                            {
                                click_jewel = 0;

                                linha2_click = ( (mouse_y - tab_rect.y) / rec_tam );
                                coluna2_click = ( (mouse_x - tab_rect.x) / rec_tam );

                                elementos_num.segundo = matriz[linha2_click][coluna2_click];
                                clicks -= 1;

                                tranfere_para_aux(matriz, matriz_aux, linha_click,coluna_click, linha2_click,coluna2_click, vetor_jewels_block, &jewels);

                                
                            }else
                            { //Se nao for vizinho encara como primeiro click

                                linha_click = ( (mouse_y - tab_rect.y) / rec_tam );
                                coluna_click = ( (mouse_x - tab_rect.x) / rec_tam );

                                elementos_num.primeiro = matriz[linha_click][coluna_click];
                                clicks = 1;

                                block_clicked.x = (( (mouse_x - tab_rect.x) / rec_tam ) * rec_tam) + tab_rect.x;
                                block_clicked.y = (( (mouse_y - tab_rect.y) / rec_tam ) * rec_tam) + tab_rect.y;
                                block_clicked.w = rec_tam;
                                block_clicked.h = rec_tam;

                            }
                            
                        }
                        
                    }

                    
                }else //Apertou Pause
                if( ( mouse_x > icon_pause_rect.x ) && ( mouse_x < (icon_pause_rect.x + icon_pause_rec.w) ) && ( mouse_y > icon_pause_rect.y ) && ( mouse_y < (icon_pause_rect.y + icon_pause_rec.h) ) )
                {
                    toca_efeito_click();

                    if (running == 1)
                    {
                        icon_pause_rec.x = 64;
                        time_aux = (tempo_total - (SDL_GetTicks() / 1000));
                        running = 0;

                    }else
                    {   
                        icon_pause_rec.x = 0;
                        tempo_total = (time_aux + (TEMPO_JOGO - (TEMPO_JOGO - (SDL_GetTicks() / 1000)) )) ;
                        running = 1;
                        time_aux = 0;
                    }

                }else //Apertou DICA
                if( ( mouse_x > icon_dica_rect.x ) && ( mouse_x < (icon_dica_rect.x + icon_dica_rec.w) ) && ( mouse_y > icon_dica_rect.y ) && ( mouse_y < (icon_dica_rect.y + icon_dica_rec.h) ) )
                {   
                    toca_efeito_click();  

                    if (pontuacao > 10)
                    {
                        icon_dica_rec.x = 124;  
                        dica_function(vetor_jewels_block, matriz);   
                        pontuacao -= 10; 
                    }                       

                }else //Apertou MENU
                if( ( mouse_x > icon_menu_rect.x ) && ( mouse_x < (icon_menu_rect.x + icon_menu_rec.w) ) && ( mouse_y > icon_menu_rect.y ) && ( mouse_y < (icon_menu_rect.y + icon_menu_rec.h) ) )
                {   
                    toca_efeito_click();                    

                    SDL_FreeSurface(screen);
                    SDL_FreeSurface(fundo); 
                    SDL_FreeSurface(fundo_pause);      

                    SDL_FreeSurface(message_ponto_int);
                    SDL_FreeSurface(message_tempo_int);

                    SDL_FreeSurface(jewels.redImagem  );
                    SDL_FreeSurface( jewels.blueImagem  );
                    SDL_FreeSurface(jewels.greenImagem );
                    SDL_FreeSurface(jewels.pinkImagem );
                    SDL_FreeSurface(jewels.yellowImagem);
                    SDL_FreeSurface(jewels.orangeImagem); 

                    SDL_FreeSurface(icon_pause);
                    SDL_FreeSurface(icon_dica );
                    SDL_FreeSurface(icon_score);
                    SDL_FreeSurface(icon_time);
                    SDL_FreeSurface(icon_menu);
                    

                    return 0;

                }
            }
        
            event.button.type = -1;
        }

        if (running == 0)
        {
            SDL_BlitSurface( fundo_pause, NULL, screen, &fundo_pause_rect );
        }

        if(click_jewel == 1)
        {            
            int elemento_block_clicked = elementos_num.primeiro ;

            if (elemento_block_clicked == 1)
            {
                SDL_BlitSurface( jewels.redImagem, &jewels_move.block_click, screen, &block_clicked );
            }else if (elemento_block_clicked == 2)
            {
                SDL_BlitSurface( jewels.blueImagem, &jewels_move.block_click, screen, &block_clicked );
            }else if (elemento_block_clicked == 3)
            {
                SDL_BlitSurface( jewels.greenImagem, &jewels_move.block_click, screen, &block_clicked );
            }else if (elemento_block_clicked == 4)
            {
                SDL_BlitSurface( jewels.pinkImagem, &jewels_move.block_click, screen, &block_clicked );
            }else if (elemento_block_clicked == 5)
            {
                SDL_BlitSurface( jewels.yellowImagem, &jewels_move.block_click, screen, &block_clicked );
            }else if (elemento_block_clicked == 6)
            {
                SDL_BlitSurface( jewels.orangeImagem, &jewels_move.block_click, screen, &block_clicked );
            }           

        }

        // Se acabou possiveis movimentos
        if (game_over_analisar_moves(matriz, vetor_gambiarra) == 1)
        {
            no_more_moves_display();
            criar_matriz(matriz);
            
        }

        SDL_Flip( screen );
       
        int t2 = SDL_GetTicks(); 
        int wait = t2 - t1; 
        wait = (1000/FRAMERATE) - wait; 
       
        if(wait > 0)
        {
            SDL_Delay(wait);
        }

        SDL_FreeSurface(screen);      

        SDL_FreeSurface(message_ponto_int);
        SDL_FreeSurface(message_tempo_int);

        SDL_FreeSurface(jewels.redImagem  );
        SDL_FreeSurface( jewels.blueImagem  );
        SDL_FreeSurface(jewels.greenImagem );
        SDL_FreeSurface(jewels.pinkImagem );
        SDL_FreeSurface(jewels.yellowImagem);
        SDL_FreeSurface(jewels.orangeImagem);  

    }

    SDL_Quit(); 

    return 0;
}

int numero_Rand()
{
    int numero_rando;

    numero_rando = rand()%6 + 1;    

    return numero_rando;     
}

void criar_matriz(int p_matriz[DIM][DIM])
{
    int i,j, x=55,y=55;

    for (i = 0; i < DIM; i++)
    {
        for (j = 0; j < DIM; j++)
        {
            if (i >= 2)
            {
                if (p_matriz[i-1][j] == p_matriz[i-2][j])
                {
                    x = p_matriz[i-1][j];
                }
                
            }

            if (j >= 2)
            {
                if (p_matriz[i][j-1] == p_matriz[i][j-2])
                {
                    y = p_matriz[i][j-1];
                }
            }

            do
            {
                p_matriz[i][j] = numero_Rand();

            } while (p_matriz[i][j] == x || p_matriz[i][j] == y);
            
            
        }
    }
}

//ORIGINAL
void matriz_original(int p_matriz[DIM][DIM], JEWELS *p_jewels , SDL_Surface **p_tab, JEWELS_BLOCK p_vetor_jewels_block[DIM*DIM])
{   
    int i,j;

    VETORES_VERIFICADORES vetores_verificadores;
    vetores_verificadores.contador_verificador = 0;
    vetores_verificadores.contador_apagador = 0;    
    vetores_verificadores.repeticao = 0;

    for (i = 0; i < DIM; i++)
    {
      
        for (j = 0; j < DIM; j++)
        {               
            if (i > 1)
            {
                if (verifica (&vetores_verificadores, i, j) == 1)
                {

                }else {

                     if (p_matriz[i][j] == p_matriz[i-1][j])
                     {
                         if (p_matriz[i][j] == p_matriz[i-2][j])
                          {
                               salva_apagar (&vetores_verificadores, i, j);
                          }
                          
                     }
                }
           }

           if (i < (DIM-1)-1)
           {
                if (verifica (&vetores_verificadores, i, j) == 1)
                {

                }else
                {
                     if (p_matriz[i][j] == p_matriz[i+1][j])
                     {
                          if (p_matriz[i][j] == p_matriz[i+2][j])
                          {
                               salva(&vetores_verificadores, i+2, j);
                               salva_apagar (&vetores_verificadores, i, j);
                               salva_apagar (&vetores_verificadores, i+1, j);
                               salva_apagar (&vetores_verificadores, i+2, j);
 
                          }
                          
                     }
                }
           }
           if (i < (DIM-1) && i > 0) //olha pra cima e baixo
           {
                if (verifica (&vetores_verificadores, i, j) == 1 || verifica (&vetores_verificadores, i+1, j) == 1)
                {

                }else
                {
                     if (p_matriz[i][j] == p_matriz[i+1][j])
                     {
                          if (p_matriz[i][j] == p_matriz[i-1][j])
                          {
                               salva_apagar (&vetores_verificadores, i, j);
                          }
                          
                     }
                }
           }


           if (j < (DIM-1) && j > 0) //olha pra esquerda e direita
           {
                if (verifica (&vetores_verificadores, i, j) == 1 || verifica (&vetores_verificadores, i, j+1) == 1)
                {

                }else
                {
                     if (p_matriz[i][j] == p_matriz[i][j+1])
                     {
                          if (p_matriz[i][j] == p_matriz[i][j-1])
                          {
                               salva_apagar (&vetores_verificadores, i, j); 
                          }
                          
                     }
                }
           } 
           if (j > 1)
           {
                if (verifica (&vetores_verificadores, i, j) == 1)
                {

                }else
                {
                     if (p_matriz[i][j] == p_matriz[i][j-1])
                     {    
                          
                          if (p_matriz[i][j] == p_matriz[i][j-2])
                          {
                            salva_apagar (&vetores_verificadores, i, j);                               
                          }
                          
                          
                     }
                }
           }

           if (j < (DIM-1)-1)
           {    
                if (verifica (&vetores_verificadores, i, j) == 1)
                {

                }else
                {
                     if (p_matriz[i][j] == p_matriz[i][j+1])
                     {
                          if (p_matriz[i][j] == p_matriz[i][j+2])
                          {
                               salva(&vetores_verificadores, i, j+2);
                               salva_apagar (&vetores_verificadores, i, j);
                               salva_apagar (&vetores_verificadores, i, j+1);
                               salva_apagar (&vetores_verificadores, i, j+2);
                          }
                     }
                }
            }  


        }

    }

    if (vetores_verificadores.contador_apagador >= 2)
    {
        animacao_block_descendo(p_matriz, &vetores_verificadores, p_jewels, &*p_tab, p_vetor_jewels_block);
        explode_jewels(p_matriz, &vetores_verificadores);
    }
    
}
void animacao_block_descendo(int p_matriz[DIM][DIM], VETORES_VERIFICADORES *p_vetores, JEWELS *p_jewels, SDL_Surface **p_tab, JEWELS_BLOCK p_vetor_jewels_block[DIM*DIM])
{   

    int i,k,j;
    int rec_tam = 50;    

    int matriz_descer[2][DIM];
    for (i = 0; i < DIM; i++)
    {
        matriz_descer[0][i] = 0;
        matriz_descer[1][i] = 55;
    }

    for (k = 1; k <= 2; k++)
    {
        for (i = 0; i <= p_vetores->contador_apagador; i++)
        {
            p_vetor_jewels_block[((DIM-1)* p_vetores->vetor_apagador[i] ) + p_vetores->vetor_apagador[i] + p_vetores->vetor_apagador[i+1]].block_rec.x = 0 + (k*50);
            p_vetor_jewels_block[((DIM-1)* p_vetores->vetor_apagador[i] ) + p_vetores->vetor_apagador[i] + p_vetores->vetor_apagador[i+1]].block_rec.y = 0;
            
            i += 1;
        }

        toca_efeito_explo(p_matriz[p_vetores->vetor_apagador[0]][p_vetores->vetor_apagador[1]]);

        desenha_matriz(p_jewels, &*p_tab, p_matriz, p_vetor_jewels_block);
        SDL_Flip(screen);
        SDL_Delay(250);

    }


    //Faz Peça Ficar Branca
    for (i = 0; i <= p_vetores->contador_apagador; i++)
    {
        p_vetor_jewels_block[((DIM-1)* p_vetores->vetor_apagador[i] ) + p_vetores->vetor_apagador[i] + p_vetores->vetor_apagador[i+1]].block_rec.x = 100;
        p_vetor_jewels_block[((DIM-1)* p_vetores->vetor_apagador[i] ) + p_vetores->vetor_apagador[i] + p_vetores->vetor_apagador[i+1]].block_rec.y = 50;
        p_vetor_jewels_block[((DIM-1)* p_vetores->vetor_apagador[i] ) + p_vetores->vetor_apagador[i] + p_vetores->vetor_apagador[i+1]].block_rec.w = 50;
        p_vetor_jewels_block[((DIM-1)* p_vetores->vetor_apagador[i] ) + p_vetores->vetor_apagador[i] + p_vetores->vetor_apagador[i+1]].block_rec.h = 50;

        i += 1;
    }
    SDL_FreeSurface(screen);
    desenha_matriz(p_jewels, &*p_tab, p_matriz, p_vetor_jewels_block);
    SDL_Flip(screen);

    for (i = 0; i < DIM; i++)
    {
        for (k = 1; k <= p_vetores->contador_apagador; k++)
        {
            if (p_vetores->vetor_apagador[k] == i)
            {
                matriz_descer[0][i] += 1;

                if (matriz_descer[1][i] > p_vetores->vetor_apagador[k-1])
                {
                    matriz_descer[1][i] = p_vetores->vetor_apagador[k-1];
                }
            }

            
            k += 1;
        }
    }

    for (j = 1; j <= 10; j++)
    {
        for (i = 0; i < DIM; i++)
        {
            if (matriz_descer[0][i] <= 0)
            {

            }else
            {
                for (k = matriz_descer[1][i] - 1; k >= 0; k--)
                {
                    p_vetor_jewels_block[((DIM-1)* k ) + k + i].block_rect.y += (5 * matriz_descer[0][i]);
                }

            }
        }

        desenha_matriz(p_jewels, &*p_tab, p_matriz, p_vetor_jewels_block);
        SDL_Flip(screen);
        SDL_Delay(5);
    }

    defineImg_blocks(p_jewels, rec_tam, p_vetor_jewels_block);

}
void explode_jewels(int p_matriz[DIM][DIM], VETORES_VERIFICADORES *p_vetores)
{   
    int i;
    int contador_especial;

    achvs(p_matriz, &p_vetores);
    
    
    for (i = 0; i < p_vetores->contador_apagador; i++)
    {
        if ((p_vetores->vetor_apagador[i])-1 == 0)
        {
            p_matriz[p_vetores->vetor_apagador[i]][p_vetores->vetor_apagador[i+1]] = p_matriz[0][p_vetores->vetor_apagador[i+1]];
            p_matriz[0][p_vetores->vetor_apagador[i+1]] = numero_Rand();
        
        }else 
        {
        
            for (contador_especial = 0; contador_especial < p_vetores->vetor_apagador[i]; contador_especial++)
            {
                p_matriz[p_vetores->vetor_apagador[i] - contador_especial][p_vetores->vetor_apagador[i+1]] = p_matriz[p_vetores->vetor_apagador[i] - (contador_especial + 1)][p_vetores->vetor_apagador[i+1]];
            }

            p_matriz[0][p_vetores->vetor_apagador[i+1]] = numero_Rand();
        }

        pontuacao += P_BLOCO;

        i += 1;
    }   

    pontuacao = pontuacao + ( P_BLOCO * p_vetores->repeticao );

}



//AUXILIAR
void tranfere_para_aux(int p_matriz[DIM][DIM], int p_matriz_aux[DIM][DIM], int linha1,int coluna1, int linha2, int coluna2, JEWELS_BLOCK p_vetor_jewels_block[DIM*DIM], JEWELS *p_jewels) 
{ 
    int i,j;
    int elemento_salva_troca; 



    for (i = 0; i < DIM; i++)
     {
        for (j = 0; j < DIM; j++)
        {
            p_matriz_aux[i][j] = p_matriz[i][j];
        }
    }

    elemento_salva_troca = p_matriz_aux[linha1][coluna1];
    p_matriz_aux[linha1][coluna1] = p_matriz_aux[linha2][coluna2];
    p_matriz_aux[linha2][coluna2] = elemento_salva_troca;

    matriz_aux(p_matriz_aux, p_matriz, p_vetor_jewels_block, linha1, coluna1, linha2, coluna2, p_jewels);

}
void tranfere_para_original(int p_matriz[DIM][DIM], int p_matriz_aux[DIM][DIM]) 
{ 
    int i,j; 

    for (i = 0; i < DIM; i++)
     {
        for (j = 0; j < DIM; j++)
        {
            p_matriz[i][j] = p_matriz_aux[i][j];
        }
    }

}

void matriz_aux(int p_matriz_aux[DIM][DIM], int p_matriz[DIM][DIM], JEWELS_BLOCK p_vetor_jewels_block[DIM*DIM], int linha1,int coluna1, int linha2, int coluna2, JEWELS *p_jewels)
{   
    int i,j;

    VETORES_VERIFICADORES vetores_verificadores;
    vetores_verificadores.contador_verificador = 0;
    vetores_verificadores.contador_apagador = 0;    

    for (i = 0; i < DIM; i++)
    {
      
        for (j = 0; j < DIM; j++)
        {               
            if (i > 1)
            {
                if (verifica (&vetores_verificadores, i, j) == 1)
                {

                }else {

                     if (p_matriz_aux[i][j] == p_matriz_aux[i-1][j])
                     {
                         if (p_matriz_aux[i][j] == p_matriz_aux[i-2][j])
                          {
                               salva_apagar (&vetores_verificadores, i, j);
                          }
                          
                     }
                }
           }

           if (i < (DIM-1)-1)
           {
                if (verifica (&vetores_verificadores, i, j) == 1)
                {

                }else
                {
                     if (p_matriz_aux[i][j] == p_matriz_aux[i+1][j])
                     {
                          if (p_matriz_aux[i][j] == p_matriz_aux[i+2][j])
                          {
                               salva(&vetores_verificadores, i+2, j);
                               salva_apagar (&vetores_verificadores, i, j);
                               salva_apagar (&vetores_verificadores, i+1, j);
                               salva_apagar (&vetores_verificadores, i+2, j);
                          }
                          
                     }
                }
           }

           

           if (j > 1)
           {
                if (verifica (&vetores_verificadores, i, j) == 1)
                {

                }else
                {
                     if (p_matriz_aux[i][j] == p_matriz_aux[i][j-1])
                     {    
                          
                          if (p_matriz_aux[i][j] == p_matriz_aux[i][j-2])
                          {
                            salva_apagar (&vetores_verificadores, i, j);                               
                          }
                          
                          
                     }
                }
           }

           if (j < (DIM-1)-1)
           {    
                if (verifica (&vetores_verificadores, i, j) == 1)
                {

                }else
                {
                     if (p_matriz_aux[i][j] == p_matriz_aux[i][j+1])
                     {
                          if (p_matriz_aux[i][j] == p_matriz_aux[i][j+2])
                          {
                               salva(&vetores_verificadores, i, j+2);
                               salva_apagar (&vetores_verificadores, i, j);
                               salva_apagar (&vetores_verificadores, i, j+1);
                               salva_apagar (&vetores_verificadores, i, j+2);
                          }
                     }
                }
            }  


        }

    }

    if (vetores_verificadores.contador_apagador >= 2)
    {

        if (coluna1 < coluna2)
        {
            for (i = 1; i <= 5; ++i)
            { 
                p_vetor_jewels_block[((DIM-1)* linha1 ) + linha1 + coluna1].block_rect.x += 10;
                p_vetor_jewels_block[((DIM-1)* linha2 ) + linha2 + coluna2].block_rect.x -= 10;

                desenha_matriz(p_jewels, &screen, p_matriz, p_vetor_jewels_block);
                SDL_Flip(screen);
                SDL_Delay(25);
            }

            p_vetor_jewels_block[((DIM-1)* linha1 ) + linha1 + coluna1].block_rect.x -= 50;
            p_vetor_jewels_block[((DIM-1)* linha2 ) + linha2 + coluna2].block_rect.x += 50;
            

        }else if (coluna1 > coluna2)
        {
            for (i = 1; i <= 5; ++i)
            {
                p_vetor_jewels_block[((DIM-1)* linha1 ) + linha1 + coluna1].block_rect.x -= 10;
                p_vetor_jewels_block[((DIM-1)* linha2 ) + linha2 + coluna2].block_rect.x += 10;

                desenha_matriz(p_jewels, &screen, p_matriz, p_vetor_jewels_block);
                SDL_Flip(screen);
                SDL_Delay(25);
            }

            p_vetor_jewels_block[((DIM-1)* linha1 ) + linha1 + coluna1].block_rect.x += 50;
            p_vetor_jewels_block[((DIM-1)* linha2 ) + linha2 + coluna2].block_rect.x -= 50;

        }else if (linha1 < linha2)
        {
            for (i = 1; i <= 5; ++i)
            {
                p_vetor_jewels_block[((DIM-1)* linha1 ) + linha1 + coluna1].block_rect.y += 10;
                p_vetor_jewels_block[((DIM-1)* linha2 ) + linha2 + coluna2].block_rect.y -= 10;

                desenha_matriz(p_jewels, &screen, p_matriz, p_vetor_jewels_block);
                SDL_Flip(screen);
                SDL_Delay(25);
            }

            p_vetor_jewels_block[((DIM-1)* linha1 ) + linha1 + coluna1].block_rect.y -= 50;
            p_vetor_jewels_block[((DIM-1)* linha2 ) + linha2 + coluna2].block_rect.y += 50;

        }else if (linha1 > linha2)
        {
            for (i = 1; i <= 5; ++i)
            {
                p_vetor_jewels_block[((DIM-1)* linha1 ) + linha1 + coluna1].block_rect.y -= 10;
                p_vetor_jewels_block[((DIM-1)* linha2 ) + linha2 + coluna2].block_rect.y += 10;

                desenha_matriz(p_jewels, &screen, p_matriz, p_vetor_jewels_block);
                SDL_Flip(screen);
                SDL_Delay(25);
            }

            p_vetor_jewels_block[((DIM-1)* linha1 ) + linha1 + coluna1].block_rect.y += 50;
            p_vetor_jewels_block[((DIM-1)* linha2 ) + linha2 + coluna2].block_rect.y -= 50;

        }

        tranfere_para_original(p_matriz, p_matriz_aux);

        desenha_matriz(p_jewels, &screen, p_matriz, p_vetor_jewels_block);
        SDL_Flip(screen);
        SDL_Delay(50);


    }else
    {

    }

}

void salva (VETORES_VERIFICADORES *p_vetores ,int coord_1, int coord_2)
{

    //Adiciona a linha
    p_vetores->vetor_verificador[p_vetores->contador_verificador] = coord_1;

    p_vetores->contador_verificador += 1;

    //Adiciona a coluna
    p_vetores->vetor_verificador[p_vetores->contador_verificador] = coord_2;

    p_vetores->contador_verificador += 1;
}
int verifica (VETORES_VERIFICADORES *p_vetores ,int coord_1, int coord_2)
{
    int i;
    int tem = 0;

    for (i = 0; i < p_vetores->contador_verificador; i++)
    {
      if (p_vetores->vetor_verificador[i] == coord_1)
      {
           if (p_vetores->vetor_verificador[i+1] == coord_2)
           {    
                tem = 1;
                return tem;

           }else
            {
                i += 1;
            }

      }else
      {
        i += 1;
      }
    }

    return tem;
     
}
void salva_apagar (VETORES_VERIFICADORES *p_vetores ,int coord_1, int coord_2)
{
    int i;
    int tem_no_vetor=0;

    if (p_vetores->contador_apagador == 0)
    {

    }else{
      p_vetores->contador_apagador += 1;
    }  

    for (i = 0; i < p_vetores->contador_apagador; i++)
    {        
        if (p_vetores->vetor_apagador[i] == coord_1)
        {
            if (p_vetores->vetor_apagador[i+1] == coord_2)
            {
                tem_no_vetor = 1;
            }
                        
        }

        i += 1;
    }  

    if (tem_no_vetor == 0)
    {
        p_vetores->vetor_apagador[p_vetores->contador_apagador] = coord_1;

        p_vetores->contador_apagador += 1;
        p_vetores->vetor_apagador[p_vetores->contador_apagador] = coord_2;
    }else
    {
        p_vetores->repeticao += 1;
        p_vetores->contador_apagador -= 1;
    }

}


void defineImg_blocks(JEWELS *p_jewels, int rec_tam, JEWELS_BLOCK p_vetor_jewels_block[DIM*DIM])
{
    int i,j;


    p_jewels->redImagem = IMG_Load( "./img/blocks/red.jpg" );
    p_jewels->blueImagem = IMG_Load( "./img/blocks/blue.jpg" );
    p_jewels->greenImagem = IMG_Load( "./img/blocks/green.jpg" );
    p_jewels->pinkImagem = IMG_Load( "./img/blocks/pink.jpg" );
    p_jewels->yellowImagem = IMG_Load( "./img/blocks/gray.jpg" );
    p_jewels->orangeImagem = IMG_Load( "./img/blocks/orange.jpg" );


    for (i = 0; i < (DIM*DIM); i++)
    {
        p_vetor_jewels_block[i].block_rec.x = 0; 
        p_vetor_jewels_block[i].block_rec.y = 0; 
        p_vetor_jewels_block[i].block_rec.w = TAMANHO_BLOCO; 
        p_vetor_jewels_block[i].block_rec.h = TAMANHO_BLOCO;

        p_vetor_jewels_block[i].block_numero = i;
    }

    for (i = DIM-1; i >= 0 ; i--)
    {
        for (j = DIM-1; j >= 0; j--)
        {
            p_vetor_jewels_block[((DIM-1)*i) + i + j].block_rect.x = (rec_tam*j) + 300;
            p_vetor_jewels_block[((DIM-1)*i) + i + j].block_rect.y = (rec_tam*i) + 100;
            p_vetor_jewels_block[((DIM-1)*i) + i + j].block_rect.w = rec_tam;
            p_vetor_jewels_block[((DIM-1)*i) + i + j].block_rect.h = rec_tam;

        }

    }

}

void desenha_matriz(JEWELS *p_jewels, SDL_Surface **p_tab, int p_matriz[DIM][DIM], JEWELS_BLOCK p_vetor_jewels_block[DIM*DIM])
{
    int i,j;

    SDL_FreeSurface(*p_tab);

    SDL_Rect tab_back_matriz;
    tab_back_matriz.x = 300;
    tab_back_matriz.y = 100;
    tab_back_matriz.w = TAMANHO_BLOCO * DIM;
    tab_back_matriz.h = TAMANHO_BLOCO * DIM;

    SDL_FillRect(screen, &tab_back_matriz, SDL_MapRGB(screen->format,255,255,255));

    for (i = DIM-1; i >= 0 ; i--)
    {
        for (j = DIM-1; j >= 0; j--)
        {
            
            if (p_matriz[i][j] == 1)
            {
              SDL_BlitSurface( p_jewels->redImagem, &p_vetor_jewels_block[((DIM-1)*i) + i + j].block_rec, *p_tab, &p_vetor_jewels_block[((DIM-1)*i) + i + j].block_rect );
              continue;

            }else if (p_matriz[i][j] == 2)
            {
              SDL_BlitSurface( p_jewels->blueImagem, &p_vetor_jewels_block[((DIM-1)*i) + i + j].block_rec, *p_tab, &p_vetor_jewels_block[((DIM-1)*i) + i + j].block_rect );
              continue;

            }else if (p_matriz[i][j] == 3)
            {
              SDL_BlitSurface( p_jewels->greenImagem, &p_vetor_jewels_block[((DIM-1)*i) + i + j].block_rec, *p_tab, &p_vetor_jewels_block[((DIM-1)*i) + i + j].block_rect );
              continue;

            }else if (p_matriz[i][j] == 4)
            {
              SDL_BlitSurface( p_jewels->pinkImagem, &p_vetor_jewels_block[((DIM-1)*i) + i + j].block_rec, *p_tab, &p_vetor_jewels_block[((DIM-1)*i) + i + j].block_rect );
              continue;

            }else if (p_matriz[i][j] == 5)
            {
              SDL_BlitSurface( p_jewels->yellowImagem, &p_vetor_jewels_block[((DIM-1)*i) + i + j].block_rec, *p_tab, &p_vetor_jewels_block[((DIM-1)*i) + i + j].block_rect );
              continue;

            }else if (p_matriz[i][j] == 6)
            {
              SDL_BlitSurface( p_jewels->orangeImagem, &p_vetor_jewels_block[((DIM-1)*i) + i + j].block_rec, *p_tab, &p_vetor_jewels_block[((DIM-1)*i) + i + j].block_rect );
              continue;
            }
            
        } 
    }
    
}

void apply_surface( int x, int y, SDL_Surface* message, SDL_Surface* destination )
{
    SDL_Rect* rect_message = NULL;

    //Holds offsets
    SDL_Rect offset;

    //Get offsets
    offset.x = x;
    offset.y = y;

    //Blit
    SDL_BlitSurface( message, rect_message, destination, &offset );
}

int game_over_analisar_moves(int p_matriz[DIM][DIM], int vetor_dica[4])
{
    int i,j;
    int game_over_moves = 1;


    for ( i = 0; i < DIM; i++)
    {
        for (j = 0; j < DIM; j++)
        {
            //verifica pra baixo
            if (i <= (DIM-1) - 3)
            {
                if (p_matriz[i][j] == p_matriz[i+2][j] && p_matriz[i][j] == p_matriz[i+3][j])
                {
                    vetor_dica[0] = i;
                    vetor_dica[1] = j;
                    vetor_dica[2] = i+1;
                    vetor_dica[3] = j;
                    
                    game_over_moves = 0;
                    continue;
                }
            }
            //verifica baixo.direita
            if (i < (DIM-1) && j <= (DIM-1) - 2)
            {
                if (p_matriz[i][j] == p_matriz[i+1][j+1] && p_matriz[i][j] == p_matriz[i+1][j+2])
                {
                    vetor_dica[0] = i;
                    vetor_dica[1] = j;
                    vetor_dica[2] = i+1;
                    vetor_dica[3] = j;

                    game_over_moves = 0;
                    continue;
                }
            }
            //verifica baixo.direita.baixo
            if (i <= (DIM-1) - 2 && j < (DIM-1))
            {
                if (p_matriz[i][j] == p_matriz[i+1][j+1] && p_matriz[i][j] == p_matriz[i+2][j+1])
                {
                    vetor_dica[0] = i;
                    vetor_dica[1] = j;
                    vetor_dica[2] = i+1;
                    vetor_dica[3] = j;

                    game_over_moves = 0;
                    continue;
                }
            }
            //verifica direita
            if (j <= (DIM-1) - 3)
            {
                if (p_matriz[i][j] == p_matriz[i][j+2] && p_matriz[i][j] == p_matriz[i][j+3])
                {
                    vetor_dica[0] = i;
                    vetor_dica[1] = j;
                    vetor_dica[2] = i;
                    vetor_dica[3] = j+1;

                    game_over_moves = 0;
                    continue;
                }
            }
            //verifica esquerda
            if (j >= 3)
            {
                if (p_matriz[i][j] == p_matriz[i][j-2] && p_matriz[i][j] == p_matriz[i][j-3])
                {
                    vetor_dica[0] = i;
                    vetor_dica[1] = j;
                    vetor_dica[2] = i;
                    vetor_dica[3] = j-1;

                    game_over_moves = 0;
                    continue;
                }
            }
            //verifica baixo.esquerda
            if (j >= 2)
            {
                if (p_matriz[i][j] == p_matriz[i+1][j-1] && p_matriz[i][j] == p_matriz[i+1][j-2])
                {
                    vetor_dica[0] = i;
                    vetor_dica[1] = j;
                    vetor_dica[2] = i+1;
                    vetor_dica[3] = j;

                    game_over_moves = 0;
                    continue;
                }
            }
            //verifica baixo.esquerda.baixo
            if (i <= (DIM-1) - 2 && j > 0)
            {
                if (p_matriz[i][j] == p_matriz[i+1][j-1] && p_matriz[i][j] == p_matriz[i+2][j-1])
                {
                    vetor_dica[0] = i;
                    vetor_dica[1] = j;
                    vetor_dica[2] = i;
                    vetor_dica[3] = j-1;

                    game_over_moves = 0;
                    continue;
                }
            }

            //verifica cima
            if (i >= 3)
            {
                if (p_matriz[i][j] == p_matriz[i-2][j] && p_matriz[i][j] == p_matriz[i-3][j])
                {
                    vetor_dica[0] = i;
                    vetor_dica[1] = j;
                    vetor_dica[2] = i-1;
                    vetor_dica[3] = j;

                    game_over_moves = 0;
                    continue;
                }
            }
            //verifica cima.direita
            if (i > 0 && j < (DIM-1) - 1)
            {
                if (p_matriz[i][j] == p_matriz[i-1][j+1] && p_matriz[i][j] == p_matriz[i-1][j+2])
                {
                    vetor_dica[0] = i;
                    vetor_dica[1] = j;
                    vetor_dica[2] = i-1;
                    vetor_dica[3] = j;

                    game_over_moves = 0;
                    continue;
                }
            }
            //verifica cima.direita.cima
            if (i >= 2 && j < (DIM-1))
            {
                if (p_matriz[i][j] == p_matriz[i-1][j+1] && p_matriz[i][j] == p_matriz[i-2][j+1])
                {
                    vetor_dica[0] = i;
                    vetor_dica[1] = j;
                    vetor_dica[2] = i;
                    vetor_dica[3] = j+1;

                    game_over_moves = 0;
                    continue;
                }
            }
            //verifica cima.esquerda
            if (i >= 1 && j >= 2)
            {
                if (p_matriz[i][j] == p_matriz[i-1][j-1] && p_matriz[i][j] == p_matriz[i-1][j-2])
                {
                    vetor_dica[0] = i;
                    vetor_dica[1] = j;
                    vetor_dica[2] = i-1;
                    vetor_dica[3] = j;

                    game_over_moves = 0;
                    continue;
                }
            }
            //verifica cima.esquerda.cima
            if (i >= 2 && j > 0)
            {
                if (p_matriz[i][j] == p_matriz[i-1][j-1] && p_matriz[i][j] == p_matriz[i-2][j-1])
                {
                    vetor_dica[0] = i;
                    vetor_dica[1] = j;
                    vetor_dica[2] = i;
                    vetor_dica[3] = j-1;

                    game_over_moves = 0;
                    continue;
                }
            }

            //verifica especial.baixo
            if (i < (DIM-1) && j > 0 && j < (DIM-1))
            {
                if (p_matriz[i][j] == p_matriz[i+1][j-1] && p_matriz[i][j] == p_matriz[i+1][j+1])
                {
                    vetor_dica[0] = i;
                    vetor_dica[1] = j;
                    vetor_dica[2] = i+1;
                    vetor_dica[3] = j;

                    game_over_moves = 0;
                    continue;
                }
            }
            //verifica especial.cima
            if (i > 0 && j > 0 && j < (DIM-1))
            {
                if (p_matriz[i][j] == p_matriz[i-1][j-1] && p_matriz[i][j] == p_matriz[i-1][j+1])
                {
                    vetor_dica[0] = i;
                    vetor_dica[1] = j;
                    vetor_dica[2] = i-1;
                    vetor_dica[3] = j;

                    game_over_moves = 0;
                    continue;
                }
            }
            //verifica especial.direita
            if (i > 0 && i < (DIM-1) && j < (DIM-1))
            {
                if (p_matriz[i][j] == p_matriz[i-1][j+1] && p_matriz[i][j] == p_matriz[i+1][j+1])
                {
                    vetor_dica[0] = i;
                    vetor_dica[1] = j;
                    vetor_dica[2] = i;
                    vetor_dica[3] = j+1;

                    game_over_moves = 0;
                    continue;
                }
            }
            //verifica especial.esquerda
            if (i > 0 && i < (DIM-1) && j > 0)
            {
                if (p_matriz[i][j] == p_matriz[i-1][j-1] && p_matriz[i][j] == p_matriz[i+1][j-1])
                {
                    vetor_dica[0] = i;
                    vetor_dica[1] = j;
                    vetor_dica[2] = i;
                    vetor_dica[3] = j-1;

                    game_over_moves = 0;
                    continue;
                }
            }


        }
    }

    return game_over_moves;

}
void dica_function(JEWELS_BLOCK p_vetor_jewels_block[DIM*DIM], int p_matriz[DIM][DIM])
{   
    int vetor_dica[4];

    game_over_analisar_moves(p_matriz, vetor_dica);

    SDL_Rect piscar_dica_rec;
    piscar_dica_rec.x = p_vetor_jewels_block[ ((DIM-1) * vetor_dica[0]) + vetor_dica[0] + vetor_dica[1] ].block_rect.x;
    piscar_dica_rec.y = p_vetor_jewels_block[ ((DIM-1) * vetor_dica[0]) + vetor_dica[0] + vetor_dica[1] ].block_rect.y;
    piscar_dica_rec.w = 50;
    piscar_dica_rec.h = 50;

    SDL_Rect piscar_dica_rec2;
    piscar_dica_rec2.x = p_vetor_jewels_block[ ((DIM-1) * vetor_dica[2]) + vetor_dica[2] + vetor_dica[3] ].block_rect.x;
    piscar_dica_rec2.y = p_vetor_jewels_block[ ((DIM-1) * vetor_dica[2]) + vetor_dica[2] + vetor_dica[3] ].block_rect.y;
    piscar_dica_rec2.w = 50;
    piscar_dica_rec2.h = 50;

    
    SDL_FillRect(screen, &piscar_dica_rec, SDL_MapRGBA(screen->format,6,6,6, 255));
    SDL_FillRect(screen, &piscar_dica_rec2, SDL_MapRGBA(screen->format,255,255,255, 255));

    SDL_Flip(screen);

    SDL_Delay(250);


}
void no_more_moves_display()
{
    TTF_Font *font = NULL;
    SDL_Color textColor = { 255, 255, 255 };
    font = TTF_OpenFont( "./fontes/diablo_h.ttf", 28 );
    SDL_Surface *no_moves_txt = NULL;

    char no_moves_txt1[]="Eles escaparam dessa vez!";

    no_moves_txt = TTF_RenderText_Solid( font, no_moves_txt1, textColor );


    SDL_Surface *no_moves;
    no_moves = IMG_Load( "./img/no moves/sem_movimentos.jpg" );
    apply_surface( 0, ((screen->h / 2) - no_moves->h / 2) , no_moves, screen);

    apply_surface( ((screen->w / 2) - no_moves_txt->w / 2), ((screen->w / 2) - no_moves_txt->h / 2) , no_moves_txt, screen);

    SDL_Flip(screen);
    SDL_Delay(1000);

    SDL_FreeSurface(no_moves_txt);
    SDL_FreeSurface(no_moves);
}

int escrever_recorde(int *p_quit)
{
    int flags = 0;
    int quit = 0;


    SDL_Surface* screen2;

    SDL_Event event; 

    SDL_Init(SDL_INIT_VIDEO);
    screen2 = SDL_SetVideoMode(800,600,16,flags);

    //TEXTOS--------------------------------------------------------
    TTF_Font *font = NULL;
    SDL_Color textColor = { 6, 6, 6 };
    font = TTF_OpenFont( "./fontes/mk2.ttf", 35 );


    SDL_Surface *put_name_surface = NULL;
    SDL_Rect put_name_surface_rect;
    put_name_surface_rect.x = 0;
    put_name_surface_rect.y = 0;
    
    char put_name[]="Escreva seu nome (3 letras):";

    put_name_surface = TTF_RenderText_Solid( font, put_name, textColor );

    SDL_Surface *name_surface = NULL;
    SDL_Rect name_surface_rect;
    name_surface_rect.x = 310;
    name_surface_rect.y = 210;
    
    char name[TAMANHO_NOME] = "   ";
    int contador_name = 0;

    //icons ~~~~~~~~~~~~~~~~~~
    SDL_Surface *trofeu;
    trofeu = IMG_Load( "./img/recordes/trophy.png" );

    SDL_Rect trofeu_rec; 
    
    if (pontuacao >= ALTO)
    {
        trofeu_rec.x = 120;

    }else if (pontuacao >= MEDIO)
    {
        trofeu_rec.x = 60;
    }else
    {
        trofeu_rec.x = 0;
    }

    trofeu_rec.y = 0;
    trofeu_rec.w = 60;
    trofeu_rec.h = 60;
    SDL_Rect trofeu_rect; 
    trofeu_rect.x = 240;
    trofeu_rect.y = 200;

    //TEXTOS--------------------------------------------------------


    while (!quit)
    {
        int t1 = SDL_GetTicks();

        // Pega eventos do teclado/mouse.
        while(SDL_PollEvent(&event))
        {
          // Se fechou a janela.
            if (event.type == SDL_QUIT)
            {
                quit = TRUE;
                *p_quit = TRUE;
            } 

        }
     
        SDL_FillRect(screen2, NULL, SDL_MapRGB(screen2->format,255,255,255));       

        //TEXTO e Icons
        SDL_BlitSurface( trofeu, &trofeu_rec, screen2, &trofeu_rect );


        SDL_BlitSurface( put_name_surface, NULL, screen2, &put_name_surface_rect );

        name_surface = TTF_RenderText_Solid( font, name, textColor );

        SDL_BlitSurface( name_surface, NULL, screen2, &name_surface_rect );

        
        if( event.type == SDL_KEYDOWN )
        {
            if ( event.key.keysym.sym == SDLK_ESCAPE )
            {
                return 0;
            }else
            if (event.key.keysym.sym == SDLK_KP_ENTER || event.key.keysym.sym == SDLK_RETURN)
            {
                if (contador_name > 0)
                {
                    //ARQUIVO--------------------------------------------------------------------
                    char *nome_arquivo = "./img/recordes/recordes.dat";
                    FILE *p_arquivo;
                    ARRAY_RECORDES array;
                    array.n = 0;

                    if((p_arquivo = fopen(nome_arquivo, "r")) != NULL)
                    {
                        fread(&array, sizeof(ARRAY_RECORDES), 1, p_arquivo);
                        fclose(p_arquivo);
                    }

                    if((p_arquivo = fopen(nome_arquivo, "w")) == NULL)
                    {
                        perror("fopen:");
                        return 1;
                    }

                    RECORDE rec1;

                    strcpy (rec1.nome_player, name);
                    rec1.pontuacao_player = pontuacao;

                    insere(&array, rec1);

                    fwrite(&array, sizeof(ARRAY_RECORDES), 1, p_arquivo);
                    fclose(p_arquivo);
                    //ARQUIVO--------------------------------------------------------------------

                    return 0 ;
                }else
                {
                    return 0 ;
                }
            }
            
            if (contador_name < 3)
            {
                name[contador_name] = event.key.keysym.sym;
                contador_name += 1;

            }else if (contador_name >= 3)
            {
                name[0] = event.key.keysym.sym;
                contador_name = 1;
            }

            event.type = -1;
        }

        SDL_Flip(screen2);

        int t2 = SDL_GetTicks(); 
        int wait = t2 - t1; 
        wait = (1000/FRAMERATE) - wait; 
       
        if(wait > 0)
        { 
            SDL_Delay(wait);

        }

    }

    return 0;
}
void insere(ARRAY_RECORDES *array, RECORDE recorde)
{
    if(tamanho(array) < MAX_ARRAY_RECORDE)
    {
        array->vetor[tamanho(array)] = recorde;
        array->n += 1;
    }
    else
    {
        RECORDE menor_recorde;
        menor_recorde = elemento(array, MAX_ARRAY_RECORDE - 1);
        
        if(menor_recorde.pontuacao_player >= recorde.pontuacao_player)
        {
            return;
        }
        else
        {
            remove_posicao(array, MAX_ARRAY_RECORDE - 1);
            insere(array, recorde);
            return;
        }
    }
    
    sort(array);
    
}
int tamanho(ARRAY_RECORDES *array)
{
    return array->n;
}
RECORDE elemento(ARRAY_RECORDES *array, int posicao)
{
    return array->vetor[posicao];
}
void remove_posicao(ARRAY_RECORDES *array, int posicao)
{
    if(tamanho(array) == 0)
    {
        return;
    }
    else if(tamanho(array) == 1)
    {
        array->n -= 1;
        return;
    }
    else
    {
        array->n -= 1;
        array->vetor[posicao] = array->vetor[tamanho(array)];
    }
    
    sort(array);
}

void sort(ARRAY_RECORDES *array)
{
    bubble_sort(array->vetor, array->n);
}

void bubble_sort(RECORDE list[], int n)
{
    int i, j;
    RECORDE swap;

    for(i = 0 ; i < ( n - 1 ); i++)
    {
        for(j = 0 ; j < n - i - 1; j++)
        {
            if(list[j].pontuacao_player < list[j+1].pontuacao_player)
            { 
                swap = list[j];
                list[j] = list[j+1];
                list[j+1] = swap;
            }
        }
    }
}

int mostrar_recorde(int *p_quit, int flags)
{
    int i;

    //Abrir recordes------------------------
    char *nome_arquivo = "./img/recordes/recordes.dat";
    FILE *p_arquivo;
    ARRAY_RECORDES array;
    array.n = 0;

    if((p_arquivo = fopen(nome_arquivo, "r")) != NULL)
    {
        fread(&array, sizeof(ARRAY_RECORDES), 1, p_arquivo);
        fclose(p_arquivo);
    }

    //Abrir recordes------------------------

    int quit = 0;

    SDL_Surface* screen2;

    SDL_Event event; 

    SDL_Init(SDL_INIT_VIDEO);
    screen2 = SDL_SetVideoMode(800,600,16,flags);

    //TEXTOS--------------------------------------------------------
    TTF_Font *font = NULL;
    SDL_Color textColor = { 6, 6, 6 };
    font = TTF_OpenFont( "./fontes/mk2.ttf", 35 );

    char put_name[]="RECORDES";

    SDL_Surface *put_name_surface = NULL;
    put_name_surface = TTF_RenderText_Solid( font, put_name, textColor );


    //icons ~~~~~~~~~~~~~~~~~~
    SDL_Surface *icon_menu;
    icon_menu = IMG_Load( "./img/icons_jogo/menu.png" );
    SDL_Rect icon_menu_rec; 
    icon_menu_rec.x = 0;
    icon_menu_rec.y = 0;
    icon_menu_rec.w = 110;
    icon_menu_rec.h = 100;
    SDL_Rect icon_menu_rect; 
    icon_menu_rect.x = 0;
    icon_menu_rect.y = 0;


    SDL_Surface *trofeu;
    trofeu = IMG_Load( "./img/recordes/trophy.png" );

    SDL_Rect trofeu_rec; 

    for (i = 0; i < tamanho(&array); i++)
    {
    
        if (array.vetor[i].pontuacao_player >= ALTO)
        {
            trofeu_rec.x = 120;

        }else if (array.vetor[i].pontuacao_player >= MEDIO)
        {
            trofeu_rec.x = 60;
        }else
        {
            trofeu_rec.x = 0;
        }
    }

    trofeu_rec.y = 0;
    trofeu_rec.w = 60;
    trofeu_rec.h = 60;
    SDL_Rect trofeu_rect; 
    trofeu_rect.x = ((screen2->w / 2) - (trofeu_rec.w/2));
    trofeu_rect.y = 10;

    //TEXTOS--------------------------------------------------------    
    

    while (!quit)
    {
        int t1 = SDL_GetTicks();

        // Pega eventos do teclado/mouse.
        while(SDL_PollEvent(&event))
        {
          // Se fechou a janela.
            if (event.type == SDL_QUIT)
            {
                *p_quit = TRUE;
                return 0;
            } 

        }
     
        SDL_FillRect(screen2, NULL, SDL_MapRGB(screen2->format,255,255,255));       

        //TEXTO e Icons
        SDL_BlitSurface( icon_menu, &icon_menu_rec, screen2, &icon_menu_rect ); 

        SDL_BlitSurface( trofeu, &trofeu_rec, screen2, &trofeu_rect );

        apply_surface( ((screen2->w / 2) - (put_name_surface->w / 2)), 100, put_name_surface, screen2 );


        SDL_Surface *recorde_nome_surface = NULL;
        SDL_Surface *recorde_ponto_surface = NULL;

        for (i = 0; i < tamanho(&array); i++)
        {            
            char recorde_ponto[10];
            itoa(array.vetor[i].pontuacao_player, recorde_ponto);           
            

            recorde_nome_surface = TTF_RenderText_Solid( font, array.vetor[i].nome_player, textColor );
            recorde_ponto_surface = TTF_RenderText_Solid( font, recorde_ponto, textColor );

            apply_surface( 310, (210 + (50*i)), recorde_nome_surface, screen2 );
            apply_surface( (310 + 125), (210 + (50*i)), recorde_ponto_surface, screen2 );
        } 


        if( event.type == SDL_MOUSEBUTTONUP )
        {                

            //If the left mouse button was pressed
            if( event.button.button == SDL_BUTTON_LEFT)
            {

                //Get the mouse offsets
                int mouse_x = 0, mouse_y = 0;
                mouse_x = event.button.x;
                mouse_y = event.button.y;

                //SE clicou no MEUNU
                if( ( mouse_x > icon_menu_rect.x ) && ( mouse_x < (icon_menu_rect.x + icon_menu_rec.w) ) && ( mouse_y > icon_menu_rect.y ) && ( mouse_y < (icon_menu_rect.y + icon_menu_rec.h) ) )
                {
                    toca_efeito_click();

                    SDL_FreeSurface(screen2);
                    SDL_FreeSurface(recorde_nome_surface);
                    SDL_FreeSurface(recorde_ponto_surface);
                    SDL_FreeSurface(trofeu);
                    SDL_FreeSurface(icon_menu);

                    return 0;
                }
                
                 
            }

            event.button.type = -1;
        }

        SDL_FreeSurface(screen2);
        SDL_FreeSurface(recorde_nome_surface);
        SDL_FreeSurface(recorde_ponto_surface);

        SDL_Flip(screen2);

        int t2 = SDL_GetTicks(); 
        int wait = t2 - t1; 

        wait = (1000/FRAMERATE) - wait; 
       
        if(wait > 0)
        { 
            SDL_Delay(wait);

        } 


    }    

    return 0;
}

// ACHVS====================================================
void cria_achvs()
{    
    //Abrir achvs------------------------
    char *nome_arquivo = "./img/recordes/achvs.dat";
    FILE *p_arquivo;    

    if((p_arquivo = fopen(nome_arquivo, "r")) == NULL)
    {

        if((p_arquivo = fopen(nome_arquivo, "w")) != NULL)
        {
            //peças
            g_array_achvs.vetor[0].nome_achv = "Nobody's gonna slow me down";
            g_array_achvs.vetor[1].nome_achv = "They don't really care about us";
            g_array_achvs.vetor[2].nome_achv = "Thriller night";
            g_array_achvs.vetor[3].nome_achv = "You escaped the jaws of the alien this time";
            g_array_achvs.vetor[4].nome_achv = "I've got the moves like 'Jaeger' ";
            g_array_achvs.vetor[5].nome_achv = "You'll not be back";

            //pontos
            g_array_achvs.vetor[6].nome_achv = "--- pontos";
            g_array_achvs.vetor[7].nome_achv = "--- pontos";
            g_array_achvs.vetor[8].nome_achv = "--- pontos";

            int i;
            for (i = 0; i < MAX_ARRAY_ACHV; i++)
            {
                g_array_achvs.vetor[i].have = 0;
            }

            fwrite(&g_array_achvs, sizeof(ARRAY_ACHVS), 1, p_arquivo);
            fclose(p_arquivo);

        }

    }

    if ((p_arquivo = fopen(nome_arquivo, "r")) != NULL)
    {
        fread(&g_array_achvs, sizeof(ARRAY_ACHVS), 1, p_arquivo);
        fclose(p_arquivo);
    }
    

    //Abrir achvs------------------------
}

void achvs (int p_matriz[DIM][DIM], VETORES_VERIFICADORES **p_vetores)
{
    if (g_array_achvs.vetor[0].have == 0)
    {
        demon_achv(p_matriz, &(p_vetores));
    }
    if (g_array_achvs.vetor[1].have == 0)
    {
        anjo_achv(p_matriz, &(p_vetores));
    }
    if (g_array_achvs.vetor[2].have == 0)
    {
        thriller_achv(p_matriz, &(p_vetores));
    }
    if (g_array_achvs.vetor[3].have == 0)
    {
        et_achv(p_matriz, &(p_vetores));
    }
    if (g_array_achvs.vetor[4].have == 0)
    {
        monstro_achv(p_matriz, &(p_vetores));
    }
    if (g_array_achvs.vetor[5].have == 0)
    {
        robo_achv(p_matriz, &(p_vetores));
    }

    
}

void demon_achv (int p_matriz[DIM][DIM], VETORES_VERIFICADORES ***p_vetores)
{

    if ((**p_vetores)->contador_apagador >= 6)
    {
        int i;

        for (i = 0; i <= (**p_vetores)->contador_apagador - 4; i++)
        {

            if (p_matriz[ (**p_vetores)->vetor_apagador[i]][ (**p_vetores)->vetor_apagador[i+1]] == 1)
            {
                if (p_matriz[ (**p_vetores)->vetor_apagador[i+6]][ (**p_vetores)->vetor_apagador[i+7]] == 1)
                {
                    g_array_achvs.vetor[0].have = 1;

                    char *nome_arquivo = "./img/recordes/achvs.dat";
                    FILE *p_arquivo; 

                    if((p_arquivo = fopen(nome_arquivo, "w+")) != NULL)
                    {
                        fwrite(&g_array_achvs, sizeof(ARRAY_ACHVS), 1, p_arquivo);
                        fclose(p_arquivo);

                    } 

                    SDL_Surface *tela_achv;
                    tela_achv = IMG_Load( "./img/achv/0.png" );
                    SDL_BlitSurface( tela_achv, NULL, screen, NULL ); 
                    SDL_Flip(screen);
                    SDL_Delay(2000);
                    SDL_FreeSurface(tela_achv);                   

                    break;
                }

            }else
            {
                i += 4;
            }

            i++;
        }
    }
    

}
void anjo_achv (int p_matriz[DIM][DIM], VETORES_VERIFICADORES ***p_vetores)
{

    if ((**p_vetores)->contador_apagador >= 6)
    {
        int i;

        for (i = 0; i <= (**p_vetores)->contador_apagador - 4; i++)
        {

            if (p_matriz[ (**p_vetores)->vetor_apagador[i]][ (**p_vetores)->vetor_apagador[i+1]] == 2)
            {
                if (p_matriz[ (**p_vetores)->vetor_apagador[i+6]][ (**p_vetores)->vetor_apagador[i+7]] == 2)
                {
                    
                    g_array_achvs.vetor[1].have = 1;

                    char *nome_arquivo = "./img/recordes/achvs.dat";
                    FILE *p_arquivo; 

                    if((p_arquivo = fopen(nome_arquivo, "w+")) != NULL)
                    {                       

                        fwrite(&g_array_achvs, sizeof(ARRAY_ACHVS), 1, p_arquivo);
                        fclose(p_arquivo);                       

                    }

                    SDL_Surface *tela_achv;
                    tela_achv = IMG_Load( "./img/achv/1.png" );
                    SDL_BlitSurface( tela_achv, NULL, screen, NULL ); 
                    SDL_Flip(screen);
                    SDL_Delay(2000);

                    break;

                }

            }else
            {
                i += 4;
            }

            i++;
        }
    }    

}
void thriller_achv (int p_matriz[DIM][DIM], VETORES_VERIFICADORES ***p_vetores)
{
    if ((**p_vetores)->contador_apagador >= 6)
    {
        int i;

        for (i = 0; i <= (**p_vetores)->contador_apagador - 4; i++)
        {

            if (p_matriz[ (**p_vetores)->vetor_apagador[i]][ (**p_vetores)->vetor_apagador[i+1]] == 3)
            {
                if (p_matriz[ (**p_vetores)->vetor_apagador[i+6]][ (**p_vetores)->vetor_apagador[i+7]] == 3)
                {                    
                    g_array_achvs.vetor[2].have = 1;

                    char *nome_arquivo = "./img/recordes/achvs.dat";
                    FILE *p_arquivo; 

                    if((p_arquivo = fopen(nome_arquivo, "w+")) != NULL)
                    {
                        fwrite(&g_array_achvs, sizeof(ARRAY_ACHVS), 1, p_arquivo);
                        fclose(p_arquivo);
                    } 

                    SDL_Surface *tela_achv;
                    tela_achv = IMG_Load( "./img/achv/2.png" );
                    SDL_BlitSurface( tela_achv, NULL, screen, NULL ); 
                    SDL_Flip(screen);
                    SDL_Delay(2000);                   

                    break;

                }

            }else
            {
                i += 4;
            }

            i++;
        }
    }

    

}
void et_achv (int p_matriz[DIM][DIM], VETORES_VERIFICADORES ***p_vetores)
{

    if ((**p_vetores)->contador_apagador >= 6)
    {
        int i;

        for (i = 0; i <= (**p_vetores)->contador_apagador - 4; i++)
        {

            if (p_matriz[ (**p_vetores)->vetor_apagador[i]][ (**p_vetores)->vetor_apagador[i+1]] == 4)
            {
                if (p_matriz[ (**p_vetores)->vetor_apagador[i+6]][ (**p_vetores)->vetor_apagador[i+7]] == 4)
                {
                    
                    g_array_achvs.vetor[3].have = 1;

                    char *nome_arquivo = "./img/recordes/achvs.dat";
                    FILE *p_arquivo; 

                    if((p_arquivo = fopen(nome_arquivo, "w+")) != NULL)
                    {
                        fwrite(&g_array_achvs, sizeof(ARRAY_ACHVS), 1, p_arquivo);
                        fclose(p_arquivo);
                    }

                    SDL_Surface *tela_achv;
                    tela_achv = IMG_Load( "./img/achv/3.png" );
                    
                    SDL_BlitSurface( tela_achv, NULL, screen, NULL ); 
                    SDL_Flip(screen);
                    SDL_Delay(2000);

                    break;

                }

            }else
            {
                i += 4;
            }

            i++;
        }
    }


}
void monstro_achv (int p_matriz[DIM][DIM], VETORES_VERIFICADORES ***p_vetores)
{

    if ((**p_vetores)->contador_apagador >= 6)
    {
        int i;

        for (i = 0; i <= (**p_vetores)->contador_apagador - 4; i++)
        {

            if (p_matriz[ (**p_vetores)->vetor_apagador[i]][ (**p_vetores)->vetor_apagador[i+1]] == 5)
            {
                if (p_matriz[ (**p_vetores)->vetor_apagador[i+6]][ (**p_vetores)->vetor_apagador[i+7]] == 5)
                {
                    
                    g_array_achvs.vetor[4].have = 1;

                    char *nome_arquivo = "./img/recordes/achvs.dat";
                    FILE *p_arquivo; 

                    if((p_arquivo = fopen(nome_arquivo, "w+")) != NULL)
                    {
                        fwrite(&g_array_achvs, sizeof(ARRAY_ACHVS), 1, p_arquivo);
                        fclose(p_arquivo);                        

                    }

                    SDL_Surface *tela_achv;
                    tela_achv = IMG_Load( "./img/achv/4.png" );

                    SDL_BlitSurface( tela_achv, NULL, screen, NULL ); 
                    SDL_Flip(screen);
                    SDL_Delay(2000);

                    break;
                }

            }else
            {
                i += 4;
            }

            i++;
        }
    }

}
void robo_achv (int p_matriz[DIM][DIM], VETORES_VERIFICADORES ***p_vetores)
{
    if ((**p_vetores)->contador_apagador >= 6)
    {
        int i;

        for (i = 0; i <= (**p_vetores)->contador_apagador - 4; i++)
        {

            if (p_matriz[ (**p_vetores)->vetor_apagador[i]][ (**p_vetores)->vetor_apagador[i+1]] == 6)
            {
                if (p_matriz[ (**p_vetores)->vetor_apagador[i+6]][ (**p_vetores)->vetor_apagador[i+7]] == 6)
                {
                    
                    g_array_achvs.vetor[5].have = 1;

                    char *nome_arquivo = "./img/recordes/achvs.dat";
                    FILE *p_arquivo; 

                    if((p_arquivo = fopen(nome_arquivo, "w+")) != NULL)
                    {
                        fwrite(&g_array_achvs, sizeof(ARRAY_ACHVS), 1, p_arquivo);
                        fclose(p_arquivo);                        

                    }

                    SDL_Surface *tela_achv;
                    tela_achv = IMG_Load( "./img/achv/5.png" );

                    SDL_BlitSurface( tela_achv, NULL, screen, NULL ); 
                    SDL_Flip(screen);
                    SDL_Delay(2000);

                    break;
                }

            }else
            {
                i += 4;
            }

            i++;
        }
    }    

}

int mostrar_achvs(int *p_quit, int flags)
{
    char *nome_arquivo = "./img/recordes/achvs.dat";
    FILE *p_arquivo; 

    if ((p_arquivo = fopen(nome_arquivo, "r")) != NULL)
    {
        fread(&g_array_achvs, sizeof(ARRAY_ACHVS), 1, p_arquivo);
        fclose(p_arquivo);
    }

    int i;
    
    int quit = 0;

    SDL_Surface* screen2;

    SDL_Event event; 

    SDL_Init(SDL_INIT_VIDEO);
    screen2 = SDL_SetVideoMode(800,600,16,0);

    //TEXTOS--------------------------------------------------------

    TTF_Font *font = NULL;
    SDL_Color textColor = { 6, 6, 6 };
    font = TTF_OpenFont( "./fontes/diablo_l.ttf", 15 );

    char achv_title[]="Conquistas";


    SDL_Surface *achv_title_surface = NULL;
    achv_title_surface = TTF_RenderText_Solid( font, achv_title, textColor );


    //icons ~~~~~~~~~~~~~~~~~~
    
    SDL_Surface *icon_menu;
    icon_menu = IMG_Load( "./img/icons_jogo/menu.png" );
    SDL_Rect icon_menu_rec; 
    icon_menu_rec.x = 0;
    icon_menu_rec.y = 0;
    icon_menu_rec.w = 110;
    icon_menu_rec.h = 100;
    SDL_Rect icon_menu_rect; 
    icon_menu_rect.x = 0;
    icon_menu_rect.y = 0;

    //TEXTOS--------------------------------------------------------    
    

    while (!quit)
    {
        int t1 = SDL_GetTicks();

        // Pega eventos do teclado/mouse.
        while(SDL_PollEvent(&event))
        {
          // Se fechou a janela.
            if (event.type == SDL_QUIT)
            {
                *p_quit = TRUE;
                return 0;
            } 

        }
     
        SDL_FillRect(screen2, NULL, SDL_MapRGB(screen2->format,255,255,255));       

        //TEXTO e Icons

        SDL_BlitSurface( icon_menu, &icon_menu_rec, screen2, &icon_menu_rect ); 

        apply_surface( ((screen2->w / 2) - (achv_title_surface->w / 2)), 10, achv_title_surface, screen2 );

        SDL_Surface *achv_nome_surface = NULL;
        SDL_Surface *achv_have_surface = NULL;

        for (i = 0; i < MAX_ARRAY_ACHV; i++)
        {            
            char achv_have[3];
            itoa(g_array_achvs.vetor[i].have, achv_have);
            

            achv_nome_surface = TTF_RenderText_Solid( font, g_array_achvs.vetor[i].nome_achv, textColor );
            achv_have_surface = TTF_RenderText_Solid( font, achv_have, textColor );

            apply_surface( 1, (150 + (50*i)), achv_nome_surface, screen2 );
            apply_surface( (600 + 125), (150 + (50*i)), achv_have_surface, screen2 );
        }


        if( event.type == SDL_MOUSEBUTTONUP )
        {                

            if( event.button.button == SDL_BUTTON_LEFT)
            {

                //Get the mouse offsets
                int mouse_x = 0, mouse_y = 0;
                mouse_x = event.button.x;
                mouse_y = event.button.y;

                //SE cliclou MENU
                if( ( mouse_x > icon_menu_rect.x ) && ( mouse_x < (icon_menu_rect.x + icon_menu_rec.w) ) && ( mouse_y > icon_menu_rect.y ) && ( mouse_y < (icon_menu_rect.y + icon_menu_rec.h) ) )
                {
                    toca_efeito_click();

                    SDL_FreeSurface(screen2);
                    SDL_FreeSurface(achv_nome_surface);
                    SDL_FreeSurface(achv_have_surface);
                    SDL_FreeSurface(icon_menu);


                    return 0;
                }                
                 
            }

            event.button.type = -1;
        }

        SDL_FreeSurface(screen2);
        SDL_FreeSurface(achv_nome_surface);
        SDL_FreeSurface(achv_have_surface);

        SDL_Flip(screen2);

        int t2 = SDL_GetTicks(); 
        int wait = t2 - t1;
        wait = (1000/FRAMERATE) - wait; 
       
        if(wait > 0)
        { 
            SDL_Delay(wait);

        } 

    }

    return 0;
}

// ACHVS====================================================

//INT PARA STRING
void itoa(int n, char s[])
{
   int i, sign;

   if ((sign = n) < 0)  /* record sign */
       n = -n;          /* make n positive */
   i = 0;
   do {       /* generate digits in reverse order */
       s[i++] = n % 10 + '0';   /* get next digit */
   } while ((n /= 10) > 0);     /* delete it */
   if (sign < 0)
       s[i++] = '-';
   s[i] = '\0';
   reverse(s);
}

void reverse(char s[])
{
   int i, j;
   char c;

   for (i = 0, j = strlen(s)-1; i<j; i++, j--) {
       c = s[i];
       s[i] = s[j];
       s[j] = c;
   }
}