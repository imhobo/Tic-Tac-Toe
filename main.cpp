#include <iostream>
#include <cstdlib>
#include <ctime>
#include <conio.h>

using namespace std;

char game[3][3];
int X=0,O=0,dr=0;

int steps[50],st=0;









void func(char ch, int x, int y)
{
    game[x][y]=ch;
    system("cls");
    for(int i=0;i<3;i++)
    {
        for(int j=0;j<3;j++)
            cout<<game[i][j]<<"\t";
        cout<<"\n\n\n";
    }
}







bool result ()
{
    int count=0;
    for(int i=0;i<3;i++)
    {
            if(game[i][0]=='O'&&game[i][1]=='O'&&game[i][2]=='O')
            {
                O++;
                cout<<"\n\n\nO Wins the game..!";
                return true;
            }

            if(game[i][0]=='X'&&game[i][1]=='X'&&game[i][2]=='X')
            {
                X++;
                cout<<"\n\n\nX Wins the game..!";
                return true;
            }
            if(game[0][i]=='O'&&game[1][i]=='O'&&game[2][i]=='O')
            {
                O++;
                cout<<"\n\n\nO Wins the game..!";
                return true;
            }

            if(game[0][i]=='X'&&game[1][i]=='X'&&game[2][i]=='X')
            {
                X++;//
                cout<<"\n\n\nX Wins the game..!";
                return true;
            }

    }

    if((game[0][0]=='O'&&game[1][1]=='O'&&game[2][2]=='O')||(game[0][2]=='O'&&game[1][1]=='O'&&game[2][0]=='O'))
    {
        O++;//
        cout<<"\n\n\nO Wins the game..!";
                return true;
    }
    else if((game[0][0]=='X'&&game[1][1]=='X'&&game[2][2]=='X')||(game[0][2]=='X'&&game[1][1]=='X'&&game[2][0]=='X'))
    {
        X++;//
        cout<<"\n\n\nX Wins the game..!";
                return true;
    }

    return false;
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

int safal (char game[3][3],char my)
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


    bool empty=true;
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

int pulkit(char game[3][3],char my_ch)
{
    char opp_ch;
    int turns=0,temp;
    int pul_check(char game[3][3], char ch);

    srand(time(NULL));

    if(my_ch=='X')
        opp_ch='O';
    else
        opp_ch='X';


    for(int i=0;i<3;i++)
        for(int j=0;j<3;j++)
            if(game[i][j]!='_')
                turns++;
    if(turns==8)
        for(int i=0;i<3;i++)
            for(int j=0;j<3;j++)
                if(game[i][j]=='_')
                    return (((i+1)*10)+j+1);

    temp=pul_check(game,my_ch);
    if(temp>=0)
        return temp;

    temp=pul_check(game,opp_ch);
    if(temp>=0)
        return temp;

    if(turns==0)
    {
        int tem;
        tem=rand()%3+1;
        if(tem==1)
            return 22;
        else
        {
            tem=rand()%3+1;
            if(tem==1)
                return 11;
            else if(tem==2)
                return 13;
            else if(tem==3)
                return 33;
            else
                return 31;
        }
    }
    if(turns==1)
    {
        int tem;
        if(game[1][1]!=opp_ch)
            return 22;
        else
        {
            tem=rand()%3+1;
            if(tem==1)
                return 11;
            else if(tem==2)
                return 13;
            else if(tem==3)
                return 33;
            else
                return 31;
        }
    }

    if(turns==2&&game[1][1]==opp_ch)
    {
        if(game[0][0]==my_ch)
            return 33;
        if(game[2][2]==my_ch)
            return 11;
        if(game[0][2]==my_ch)
            return 31;
        if(game[2][0]==my_ch)
            return 13;
    }

    if(turns==3&&game[1][1]==my_ch)
    {
        int tem;
        if((game[0][0]==opp_ch&&game[2][2]==opp_ch)||(game[0][2]==opp_ch&&game[2][0]==opp_ch))
        {
            tem=rand()%3+1;
            if(tem==1)
                return 12;
            else if(tem==2)
                return 21;
            else if(tem==3)
                return 23;
            else
                return 32;
        }

        if((game[0][0]==opp_ch&&game[1][2]==opp_ch)||(game[0][2]==opp_ch&&game[1][0]==opp_ch))
            return 12;
        if((game[2][0]==opp_ch&&game[0][1]==opp_ch)||(game[0][0]==opp_ch&&game[2][1]==opp_ch))
            return 21;
        if((game[0][2]==opp_ch&&game[2][1]==opp_ch)||(game[2][2]==opp_ch&&game[0][1]==opp_ch))
            return 23;
        if((game[2][2]==opp_ch&&game[1][0]==opp_ch)||(game[2][0]==opp_ch&&game[1][2]==opp_ch))
            return 32;

    }

    if(turns==5&&game[1][1]==my_ch)
    {
        if((game[0][0]==opp_ch&&game[1][2]==opp_ch)||(game[0][2]==opp_ch&&game[1][0]==opp_ch))
        {
            if(game[0][1]=='_')
                return 12;
        }
        if((game[2][0]==opp_ch&&game[0][1]==opp_ch)||(game[0][0]==opp_ch&&game[2][1]==opp_ch))
        {
            if(game[1][0]=='_')
                return 21;
        }
        if((game[0][2]==opp_ch&&game[2][1]==opp_ch)||(game[2][2]==opp_ch&&game[0][1]==opp_ch))
        {
            if(game[1][2]=='_')
                return 23;
        }
        if((game[2][2]==opp_ch&&game[1][0]==opp_ch)||(game[2][0]==opp_ch&&game[1][2]==opp_ch))
        {
            if(game[2][1]=='_')
                return 32;
        }
    }




    int d1=2,d2=2;

    if(game[0][0]=='_'||game[2][0]=='_'||game[0][2]=='_'||game[2][2]=='_')
    {
        int tem;
        while(1)
        {
            tem=rand()%3+1;
            if(tem==1&&game[0][0]=='_')
                return 11;
            else if(tem==2&&game[0][2]=='_')
                return 13;
            else if(tem==3&&game[2][2]=='_')
                return 33;
            else if(game[2][0]=='_')
                return 31;
        }
    }

    while(1)
    {
        d1=rand()%3+1;
        d2=rand()%3+1;
        if(game[d1][d2]=='_')
            break;
    }
    return (((d1+1)*10)+d2+1);
}

int pul_check(char game[3][3], char ch)
{
    int uc1=0,cc1=0,uc2=0,cc2=0,uc3=0,cc3=0,uc4=0,cc4=0,pos1,pos2,pos3,pos4;
    for(int i=0;i<3;i++)
    {
        uc1=0;cc1=0;uc2=0;cc2=0;
        for(int j=0;j<3;j++)
        {
            if(game[i][j]==ch)
                cc1++;
            else if(game[i][j]=='_')
            {
                uc1++;pos1=(((i+1)*10)+j+1);
            }

            if(game[j][i]==ch)
                cc2++;
            else if(game[j][i]=='_')
            {
                uc2++;pos2=(((j+1)*10)+i+1);
            }
        }
        if(cc1==2&&uc1==1)
            return pos1;
        else if(cc2==2&&uc2==1)
            return pos2;

        if(game[i][i]==ch)
            cc3++;
        else if(game[i][i]=='_')
        {
            uc3++;pos3=(((i+1)*10)+i+1);
        }
        if(game[i][2-i]==ch)
            cc4++;
        else if(game[i][2-i]=='_')
        {
            uc4++;pos4=(((i+1)*10)+3-i);
        }
    }
    if(cc3==2&&uc3==1)
        return pos3;
    else if(cc4==2&&uc4==1)
        return pos4;
    return -1;
}

int main()
{

    for(int i=0;i<3;i++)
        for(int j=0;j<3;j++)
            game[i][j]='_';

  /*  for(int i=0;i<3;i++)
    {
        for(int j=0;j<3;j++)
            cout<<game[i][j]<<"\t";
        cout<<"\n\n\n";
    }
*/

    int x,y;
    char ch;
    int temp;
    int count=0;
    bool flag;
    int play=0;
    while(play<5000000)
    {

        for(int aa=0;aa<3;aa++)
            for(int bb=0;bb<3;bb++)
                game[aa][bb]='_';

        st=0;
        count=0;
        while(count<9)
        {
            ch='O';
            cout<<"\n\nPress any key to get position for 'O' - ";
            getch();
          // cin>>x>>y;
           temp=safal(game,ch);
           //cin>>temp;
            y=(temp%10)-1;
            x=(temp/10)-1;
            steps[st++]=temp;

            func(ch,x,y);
            flag=result();
            if(flag)break;
            count++;
            if(!flag&&count>=9)
            {
                dr++;
                //cout<<"\n\nMatch Draw";
                break;
            }
            ch='X';
           // cout<<"\n\nPress any key to get position for 'X' - ";
            //getch();
            //temp=pulkit(game,ch);
            cout<<"Enter pos for X..(e.g. 12 for first row,second column)"<<endl;
           cin>>temp;
            y=(temp%10)-1;
            x=(temp/10)-1;

            steps[st++]=temp;
            //cin>>x>>y;
            func(ch,x,y);
            flag=result();
            if(flag)break;
            count++;
             if(!flag&&count>=9)
            {
                dr++;
                cout<<"\n\nMatch Draw";
                break;
            }
        }

        if(O>0)
            break;

        play++;
        //cout<<"\nGame no. = "<<play;
    }

    system("cls");
    for(int i=0;i<3;i++)
    {
        for(int j=0;j<3;j++)
            cout<<game[i][j]<<"\t";
        cout<<"\n\n\n";
    }


  /*  for(int h=0;h<st;h++)
        cout<<endl<<steps[h];*/

    cout<<"\nDraw = "<<dr;
    cout<<"\nO = "<<O;
    cout<<"\nX = "<<X;
}
