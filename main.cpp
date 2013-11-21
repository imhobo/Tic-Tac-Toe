#include <iostream>
#include "SDL/SDL.h"
#include "SDL/SDL_image.h"
#include "SDL/SDL_ttf.h"
#include "SDL/SDL_rotozoom.h"
#include <time.h>


using namespace std;


const int SCREEN_WIDTH = 1000;
const int SCREEN_HEIGHT = 640;
const int SCREEN_BPP = 32;
const int sq_size=80;
const int MENU_WIDTH = SCREEN_WIDTH-SCREEN_HEIGHT;
int  players=0,AI_turn=1;                          //no of players and color of AI(for 1 player chess only)

int mouse_x,mouse_y,turn=0;
char game[3][3];
int next_move[2];

bool quit=false;

SDL_Surface *cross = NULL;
SDL_Surface *zero = NULL;
SDL_Surface *board1 = NULL;
SDL_Surface *cover = NULL;
SDL_Surface *back = NULL;
SDL_Surface *ground = NULL;
SDL_Surface *Z = NULL;
SDL_Surface *X = NULL;
SDL_Surface *screen = NULL;
SDL_Surface *message = NULL;
SDL_Surface *restart = NULL;
SDL_Surface *choose = NULL;
SDL_Surface *checkm = NULL;
SDL_Surface *stalem = NULL;
SDL_Surface *thatsdraw = NULL;
SDL_Surface *choice_a = NULL;
SDL_Surface *choice_b = NULL;
SDL_Event event;
TTF_Font *font = NULL;
SDL_Color textWhite = { 255, 255, 255 };
SDL_Color textRed = { 255, 0, 0 };
SDL_Color textGreen = { 0, 128, 0 };

SDL_Surface *load_image(string filename );
void apply_surface( int x, int y, SDL_Surface* source, SDL_Surface* destination );
SDL_Surface *ScaleSurface(SDL_Surface *Surface, Uint16 Width, Uint16 Height);
bool init();
int load_files();
void start_and_initialize();
void welcome();
void menu(int col=0);
void resizeImage( SDL_Surface*& img, const double newwidth, const double newheight );
void board();
void init_game(char game[3][3]);
void AI_func(char game[3][3],int AI_turn,int next_move[2]);
int safal(char game[3][3],char my);
int human_func(char game[3][3],int turn ,int next_move[]);
bool detect(char game[3][3],int x,int y);
void update_game(char game[3][3],int turn,int next_move[2]);
void check_everything(char game[3][3]);
void end_game();
void draw_updated_game (int turn,int next_move[]);
void clean_up();
int check(char opp);
int edge();
int fork(char my);
int corner(char opp);
int def(char my,char opp);


int main(int argc, char* args[])
{
    int temp;
    if( init() == false )return 1;

    if( load_files() != 0 )return 2;

    start_and_initialize();

    while(!quit)
    {
        if(turn == AI_turn)
        {
            AI_func(game,turn,next_move);
        }
        else
        {
            temp = human_func(game,turn,next_move);

            if(temp==0)                                     // 1 otherwise
            {
                start_and_initialize();
                continue;
            }
            else if(temp==-1)quit=true;

            else if(temp==-2)
            {
                quit=true;
            }
        }
        if(!quit)
        {
            update_game(game,turn,next_move);                //logical part
            draw_updated_game(turn,next_move);          //graphical part
            check_everything(game);           //It also changes turn
        }
    }

    //clean_up();

    return 0;
}


SDL_Surface *load_image(string filename )
{
    SDL_Surface* loadedImage = NULL;
    SDL_Surface* optimizedImage = NULL;

    loadedImage = IMG_Load( filename.c_str() );

    if( loadedImage != NULL )
    {
        optimizedImage = SDL_DisplayFormatAlpha( loadedImage );
        SDL_FreeSurface( loadedImage );
    }
    return optimizedImage;
}


void apply_surface( int x, int y, SDL_Surface* source, SDL_Surface* destination )
{
    SDL_Rect offset;

    offset.x = x;
    offset.y = y;

    SDL_BlitSurface( source, NULL, destination, &offset );
}


bool init()
{
    if( SDL_Init( SDL_INIT_EVERYTHING ) == -1 )
        return false;
    if( TTF_Init() == -1 )
        return false;


    screen = SDL_SetVideoMode( SCREEN_WIDTH, SCREEN_HEIGHT, SCREEN_BPP, SDL_SWSURFACE );

    if( screen == NULL )
        return false;

    SDL_WM_SetCaption( "Tic Tac Toe", NULL );

    return true;
}


int load_files()
{
    board1 = load_image("80/board.png");
    cross = load_image("80/cross.png");
    zero = load_image("80/zero.png");
    cover = load_image("80/cover.jpg");
    back = load_image("80/back.png");
    ground=load_image("80/ground.jpeg");

    font = TTF_OpenFont( "arabella.ttf", 32 );
    message = TTF_RenderText_Blended( font, "Turn : ", textWhite );
    Z = TTF_RenderText_Blended( font, "The Great Z", textGreen );
    X = TTF_RenderText_Blended( font, "The Dark X", textGreen );
    restart = TTF_RenderText_Blended( font, "Press 'R' to restart", textWhite );
    thatsdraw = TTF_RenderText_Blended( font, "Thats a Draw", textGreen );
    choose = TTF_RenderText_Blended( font, "Choose your piece :", textWhite );
    checkm = TTF_RenderText_Blended( font, "Emerge Victorious", textWhite );
    choice_a = TTF_RenderText_Blended( font, "A)  Player vs Computer", textWhite );
    choice_b = TTF_RenderText_Blended( font, "B)  Player vs Player", textWhite );

    return 0;
}


void start_and_initialize()
{
    init_game(game);
    welcome();
    board();
    menu(turn);

    if( SDL_Flip( screen ) == -1 )return;
}


void welcome()
{

    bool leave= false;

    resizeImage(cover,SCREEN_WIDTH-MENU_WIDTH,SCREEN_HEIGHT);
    resizeImage(back,MENU_WIDTH,SCREEN_HEIGHT);
    apply_surface(0,0,cover,screen);
    apply_surface( 640, 0, back, screen );
    apply_surface( 640, 250, choice_a, screen );
    apply_surface( 640, 350, choice_b, screen );
    if( SDL_Flip( screen ) == -1 )return ;

     while(!leave)
     {
         if( event.type == SDL_QUIT )break;
         if( SDL_PollEvent( &event ) )
         {
            if( event.type == SDL_KEYDOWN )
            {
                switch( event.key.keysym.sym )
                {
                    case SDLK_a:
                        players=1;
                        //choose_color();
                        leave=true;
                        break;
                    case SDLK_b:
                        players=2;
                        leave=true;

                    default : continue;
                }
            }
         }
     }

}


void menu(int col)
{
    apply_surface( 640, 0, back, screen );
    apply_surface( 650, 100, message, screen );
    apply_surface( 650, 350, restart, screen );

    if(col==1)apply_surface( 650, 150, X, screen );
    else apply_surface( 650, 150, Z, screen );

}


void resizeImage( SDL_Surface*& img, const double newwidth, const double newheight )
{
    double zoomx = newwidth  / (float)img->w;
    double zoomy = newheight / (float)img->h;

    SDL_Surface* sized = zoomSurface( img, zoomx, zoomy, SMOOTHING_OFF );

    SDL_FreeSurface( img );

    img =  sized;
}


void board()
{
    resizeImage(ground,SCREEN_WIDTH-MENU_WIDTH,SCREEN_HEIGHT);
    resizeImage(board1,SCREEN_WIDTH-MENU_WIDTH,SCREEN_HEIGHT);
    apply_surface(0,0,ground,screen);
    apply_surface(0,0,board1,screen);
}


void init_game(char game[3][3])
{
    turn=0;
    for(int i=0;i<3;i++)
        for(int j=0;j<3;j++)
            game[i][j]='_';
}


void AI_func(char game[3][3],int AI_turn,int next_move[2])
{
    int temp;
    char my;
    if(AI_turn==0)
        my='0';
    else
        my='X';


    temp=safal(game,my);

    next_move[0]=(temp/10)-1;
    next_move[1]=(temp%10)-1;
}


int safal(char game[3][3],char my)
{
    int corner (char);
    int edge();
    int check(char);
    int fork (char);
    int def (char,char);
    int turns=0;

    srand(time(NULL));

    for(int i=0;i<3;i++)
        for(int j=0;j<3;j++)
            if(game[i][j]!='_')
                turns++;
    if(turns==0)
    {
        int d1=rand()%3+1,d2=rand()%3+1;
        return ((d1*10)+d2);
        //return 11;
    }



    char oppo;
    if(my=='O')oppo='X';
    else oppo='O';
    int ans=0;

    ans=check(my);
    if(ans!=0)return ans%100;

    ans=check(oppo);
    if(ans!=0)return ans%100;

    ans=fork(my);
    if(ans!=0)return ans;

    if(turns>2)
    {
        ans=def(my,oppo);
        if(ans!=0)return ans;
    }
    ans=fork(oppo);
    if(ans!=0)return ans;

    if(game[1][1]=='_')return 22;

    ans=corner(oppo);if(ans!=-1)return ans;

    ans=edge();return ans;
}


int check(char opp)
{
    int count=0,ans=0;
    for(int i=0;i<3;i++)
    {
        if((game[i][0]==opp&&game[i][1]==opp)||(game[i][0]==opp&&game[i][2]==opp)||(game[i][1]==opp&&game[i][2]==opp))
            {
                for(int j=0;j<3;j++)
                    if(game[i][j]=='_')
                        {
                            count++;
                            ans=(i+1)*10+(j+1);
                        }
            }

        if((game[0][i]==opp&&game[1][i]==opp)||(game[0][i]==opp&&game[2][i]==opp)||(game[1][i]==opp&&game[2][i]==opp))
            {
                for(int j=0;j<3;j++)
                    if(game[j][i]=='_')
                        {
                            count++;
                            ans=(j+1)*10+(i+1);
                        }
            }
    }

    if((game[0][0]==opp&&game[1][1]==opp)||(game[0][0]==opp&&game[2][2]==opp)||(game[2][2]==opp&&game[1][1]==opp))
    {
        for(int i=0;i<3;i++)
            if(game[i][i]=='_')
                {
                    count++;
                    ans=(i+1)*10+(i+1);
                }
    }

    if((game[0][2]==opp&&game[1][1]==opp)||(game[0][2]==opp&&game[2][0]==opp)||(game[2][0]==opp&&game[1][1]==opp))
    {
        for(int i=0,j=2;i<3;i++,j--)
            if(game[i][j]=='_')
                {
                    count++;
                    ans=(i+1)*10+(j+1);
                }
    }
    return count*100+ans;
}


int edge()
{
    if(game[0][1]=='_')return 12;
    if(game[1][2]=='_')return 23;
    if(game[2][1]=='_')return 32;
    if(game[1][0]=='_')return 21;

    return -1;
}


int corner(char opp)
{
    if(game[0][0]=='_'&&game[2][2]==opp)return 11;
    if(game[0][2]=='_'&&game[2][0]==opp)return 13;
    if(game[2][0]=='_'&&game[0][2]==opp)return 31;
    if(game[2][2]=='_'&&game[0][0]==opp)return 33;

    if(game[0][0]=='_')return 11;
    if(game[0][2]=='_')return 13;
    if(game[2][0]=='_')return 31;
    if(game[2][2]=='_')return 33;

    return -1;
}


int fork(char my)
{

    for(int i=0;i<3;i++)
    {
        for(int j=0;j<3;j++)
        {
            if(game[i][j]=='_')
            {
                game[i][j]=my;

                if((check(my)/100)>1)
                    {
                        game[i][j]='_';
                        return (i+1)*10+(j+1);
                    }
                game[i][j]='_';
            }
        }
    }
    return 0;
}


int def(char my,char opp)
{
    for(int i=0;i<3;i++)
    {
        for(int j=0;j<3;j++)
        {
            if(game[i][j]==my)
            {
                int r=0,c=0,tr=-1,tc=-1,ttr=-1,ttc=-1;
                for(int k=0;k<3;k++)
                {
                    if(game[i][k]=='_')
                    {
                        r++;
                        if(r==1){tr=i;tc=k;}

                        if(r==2)
                        {
                            game[i][k]=my;
                            game[tr][tc]=opp;

                            if(check(opp)/100<2)
                            {
                                game[i][k]='_';
                                game[tr][tc]='_';
                                return (i+1)*10+(k+1);
                            }
                            game[i][k]=opp;
                            game[tr][tc]=my;

                            if(check(opp)/100<2)
                            {
                                game[i][k]='_';
                                game[tr][tc]='_';
                                return (tr+1)*10+(tc+1);
                            }
                                game[i][k]='_';
                                game[tr][tc]='_';
                        }
                    }


                    if(game[k][j]=='_')
                    {
                        c++;
                        if(c==1)
                        {
                            ttr=k;ttc=j;
                        }

                        if(c==2)
                        {
                            game[k][j]=my;
                            game[ttr][ttc]=opp;
                            if(check(opp)/100<2)
                            {
                                game[k][j]='_';
                                game[ttr][ttc]='_';

                                return (k+1)*10+(j+1);
                            }

                            game[k][j]=opp;
                            game[ttr][ttc]=my;
                            if(check(opp)/100<2)
                            {
                                game[k][j]='_';
                                game[ttr][ttc]='_';
                                return (ttc+1)*10+(ttr+1);
                            }

                            game[k][j]='_';
                            game[ttr][ttc]='_';
                        }
                    }
                }
            }
        }
    }
    return 0;
}


int human_func(char game[3][3],int turn ,int next_move[])
{
    int x=0,y=0;

    while(1)
    {

        if( event.type == SDL_QUIT )return -1; // Exit the game by returning -1

        if( SDL_PollEvent( &event ) )
        {
            if( event.type == SDL_KEYDOWN )
            {
                switch( event.key.keysym.sym )
                {
                    case SDLK_r:

                        return 0;
                        break;  // Restart the game by returning 0

                    default : break;
                }
            }

            if( event.type == SDL_MOUSEBUTTONDOWN )
            {
                if( event.button.button == SDL_BUTTON_LEFT )
                {
                    mouse_x = event.button.x;
                    mouse_y = event.button.y;

                    if(mouse_y>0&&mouse_y<200)
                        x=0;
                    else if(mouse_y>220&&mouse_y<420)
                        x=1;
                    else if(mouse_y>440&&mouse_y<640)
                        x=2;
                    else
                        x=3;

                    if(mouse_x>0&&mouse_x<200)
                        y=0;
                    else if(mouse_x>220&&mouse_x<420)
                        y=1;
                    else if(mouse_x>440&&mouse_x<640)
                        y=2;
                    else
                        y=3;



                    if(x>2||y>2)continue; //Ensuring clicks on the chess board



                    if(detect(game,x,y)) // If the piece clicked is valid
                    {
                        continue;
                    }
                    else
                    {
                        next_move[0]=x;
                        next_move[1]=y;
                        return 1;
                    }
                }
            }
        }
    }
}


bool detect(char game[3][3],int x,int y)
{
    if(y>2||y<0)return false;
    if(x>2||x<0)return false;

    return (game[x][y]!='_');
}


void update_game(char game[3][3],int turn,int next_move[2])
{
    char ch;
    if(turn==0)
        ch='O';
    else
        ch='X';
    game[next_move[0]][next_move[1]]=ch;
}


void check_everything(char game[3][3])
{
    turn=1-turn;   //Change the turn
    menu(turn);

    bool O=false,X=false,draw=true;

    for(int i=0;i<3;i++)
    {
            if(game[i][0]=='O'&&game[i][1]=='O'&&game[i][2]=='O')
            {
                O=true;
                break;
            }

            if(game[i][0]=='X'&&game[i][1]=='X'&&game[i][2]=='X')
            {
                X=true;
                break;
            }
            if(game[0][i]=='O'&&game[1][i]=='O'&&game[2][i]=='O')
            {
                O=true;
                break;
            }

            if(game[0][i]=='X'&&game[1][i]=='X'&&game[2][i]=='X')
            {
                X=true;
                break;
            }

    }

    if(!(O||X))
    {
        if((game[0][0]=='O'&&game[1][1]=='O'&&game[2][2]=='O')||(game[0][2]=='O'&&game[1][1]=='O'&&game[2][0]=='O'))
        {
            O=true;
        }
        else if((game[0][0]=='X'&&game[1][1]=='X'&&game[2][2]=='X')||(game[0][2]=='X'&&game[1][1]=='X'&&game[2][0]=='X'))
        {
            X=true;
        }
    }

    if(O||X)
    {
        menu(1-turn);
        apply_surface( 650, 480, checkm, screen );
        if( SDL_Flip( screen ) == -1 )return;
        end_game();
    }

    for(int i=0;i<3;i++)
        for(int j=0;j<3;j++)
            if(game[i][j]=='_')
                draw=false;

    if(draw)
    {
        menu(1-turn);
        apply_surface( 650, 480, thatsdraw, screen );
        if( SDL_Flip( screen ) == -1 )return;
        end_game();
    }
    if( SDL_Flip( screen ) == -1 )return;


}


void end_game()
{
    bool leave= false;
    while(!leave)
    {
         if( event.type == SDL_QUIT )
         {
             quit=true;
             break;
         }
         if( SDL_PollEvent( &event ) )
         {
            if( event.type == SDL_KEYDOWN )
            {
                switch( event.key.keysym.sym )
                {
                    case SDLK_r:
                        start_and_initialize();
                        leave=true;
                        break;
                    default : continue;

                }
            }
         }
     }
}


void draw_updated_game (int turn,int next_move[])
{
    int draw_x,draw_y;
    if(next_move[0]==0)
        draw_y=0;
    else if(next_move[0]==1)
        draw_y=220;
    else if(next_move[0]==2)
        draw_y=440;

    if(next_move[1]==0)
        draw_x=0;
    else if(next_move[1]==1)
        draw_x=220;
    else if(next_move[1]==2)
        draw_x=440;

    if(turn==0)
    {
        resizeImage(zero,200,200);
        apply_surface(draw_x,draw_y,zero,screen);
    }
    else
    {
        resizeImage(cross,200,200);
        apply_surface(draw_x,draw_y,cross,screen);
    }

    if( SDL_Flip( screen ) == -1 )return;

}


void clean_up()
{
    //Free the image






    TTF_CloseFont( font );
    TTF_Quit();

    SDL_Quit();
}

