#include<windows.h>
#include <GL/glut.h>
#include<bits/stdc++.h>
using namespace std;



//__________________________________________________________________TRIE CLASS
struct node
{
    int no_prefix,freq;
    bool exist;
    node* ch[26];
};
class trie
{
private:

    node *root;

    node* alloc()
    {
        node *p=(node *)malloc(sizeof(node));
        p->exist=false;
        p->no_prefix=0;
        p->freq=0;
        for(int i=0;i<26;i++)p->ch[i]=0;
        return p;
    }


public:
    trie()
    {
        root=alloc();
    }
    bool insert(char *str)
    {
        node *temp=root;
        int i;
        for(i=0;str[i] && temp->ch[tolower(str[i])-'a'];i++){
            temp->no_prefix++;
            temp=temp->ch[tolower(str[i])-'a'];
        }
        if(!str[i])
            return temp->exist?0:temp->exist=true;
        for(;str[i];i++)
        {
            temp->ch[tolower(str[i])-'a']=alloc();
            temp->no_prefix++;
            temp=temp->ch[tolower(str[i])-'a'];
        }
            return temp->exist=true;
    }
    bool search(char *str)
    {
        int i;
        node *temp=root;
        for(i=0;str[i] && temp->ch[tolower(str[i])-'a'];i++)
            temp=temp->ch[tolower(str[i])-'a'];
        temp->freq++;
        return (!str[i] && temp->exist && temp->freq==1);
    }
    bool search(char *str,int *prefix)
    {
        int i;
        node *temp=root;
        for(i=0;str[i] && temp->ch[tolower(str[i])-'a'];i++)
            temp=temp->ch[tolower(str[i])-'a'];
        *prefix=temp->no_prefix;
        temp->freq++;
        return (!str[i] && temp->exist && temp->freq==1);
    }
};
//__________________________________________________________________TRIE ENDS
struct word
{
    char st[20],pat[20];
    int length,x,y;
};
bool cmpp(word i,word j) { return (i.length>j.length); }
//____________________________________________________________________GLOBAL
#define in(i,j) ((i)>=0 && (j)>=0 && (i)<4 && (j)<4)
int dx[]={1,1,0,-1,-1,-1,0,1};
int dy[]={0,1,1,1,0,-1,-1,-1};
char c[4][4];
bool vis[4][4];
char state[100];
char path[100];
int cou=0,maxx=0,countt=0,i_,j_;

word tot[1000];
trie T;

const int MINLENGTH=5;//CHANGE HERE

//______________________________________________________________________GRAPHICS
void renderBitmapString(float x, float y, void *font,const char *string){
    const char *c;
    glRasterPos2f(x, y);
    for (c=string; *c != '\0'; c++) {
        glutBitmapCharacter(font, *c);
    }
}
void specialKeys( int key, int x, int y )
{

  if (key == GLUT_KEY_LEFT && cou>0){
        cou--;
    }

  else if (key == GLUT_KEY_RIGHT && cou<maxx){
        cou++;
    }
    glutPostRedisplay();
}
void drawbox(float x,float y)
{
    float siz=0.07;
    x+=siz/2;y+=siz/2;
    glBegin(GL_LINE_LOOP);
        glVertex2f(  x+siz,  y+siz );
        glVertex2f(  x+siz, y-siz );
        glVertex2f( x-siz, y-siz );
        glVertex2f( x-siz,  y+siz );
    glEnd();
}
void draw()
{
    int i,j;
    glLoadIdentity();
    glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
    glColor3b(0,0,0);

    char dummy[3]="a";
    for(i=0;i<4;i++)for(j=0;j<4;j++){
        dummy[0]=c[i][j];
        renderBitmapString(((float)j-1.5)/3,(-float(i)*1.0+1.5)/3,GLUT_BITMAP_TIMES_ROMAN_24,dummy);
    }

    renderBitmapString(-0.9,0.7,GLUT_BITMAP_TIMES_ROMAN_24,tot[cou].st);
        //DRAWING LINES AND BOXES
        float curx=(tot[cou].x*1.0-1.5)/3,cury=(tot[cou].y*1.0-1.5)/3;
        drawbox(curx,cury);
        drawbox(curx+0.01,cury+0.01);

        for(int it=0;it<tot[cou].length;it++)
        {
            int var=(int)tot[cou].pat[it]-1;
            glBegin(GL_LINES);
                glVertex2f(curx,cury);
                glVertex2f(curx+(dy[var]*1.0)/3.0,cury-(dx[var]*1.0)/3.0);
            glEnd();
            curx+=(dy[var]*1.0)/3.0;
            cury-=(dx[var]*1.0)/3.0;
            drawbox(curx,cury);
        }
    glutSwapBuffers();
}

//____________________________________________________________LOGIC

void dfs(int x,int y,int pos)
{
    int p;
    state[pos]=c[x][y];

    if(T.search(state,&p) && pos>(MINLENGTH-2)){
            //printf("%s\n",state);

            strcpy(tot[countt].st,state);
            strcpy(tot[countt].pat,path);
            tot[countt].length=pos;
            tot[countt].x=j_;
            tot[countt].y=3-i_;

            countt++;
    }
    if(!p){state[pos]=0;return;}
    vis[x][y]=true;

    for(int i=0;i<8;i++)if(in(x+dx[i],y+dy[i]) && !vis[x+dx[i]][y+dy[i]]){
        path[pos]=i+1;
        dfs(x+dx[i],y+dy[i],pos+1);
        path[pos]=0;
    }

    vis[x][y]=false;
    state[pos]=0;
}
//________________________________________________________MAIN
int main(int argc,char** argv)
{
    int i,j,no_l=0;
    char str[100],cha;

	FILE *fp=fopen("genwords_2.in","r");
	FILE *in=fopen("input.txt","r");

	while(fscanf(fp,"%s",str)>0)
        T.insert(str);

    //SCANNING AND VALIDATING INPUT
    while(fscanf(in,"%c",&cha)>0 && no_l<16)
    {
        if(isalpha(cha)){
            c[no_l/4][no_l%4]=tolower(cha);
            no_l++;
        }
    }
    if(no_l<16){printf("INVALID INPUTz");getchar();exit(0);}


    for(i_=0;i_<4;i_++)for(j_=0;j_<4;j_++)
        dfs(i_,j_,0);

    printf("\n\nTotal %d words\n\n\n",countt);
    maxx=countt-1;
    sort(tot,tot+countt,cmpp);  //SORTING WITH RESPECT TO CMPP FUNCTION

//__________________________________//GLUT
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(480,480);
    glutInitWindowPosition(0,0);
    glutCreateWindow("Ruzzle Solver");
    glEnable(GL_DEPTH_TEST);

    glClearColor(0.7,0.5,0,4);	        //background
//___________________________________

    glutDisplayFunc(draw);          //draw function
    glutSpecialFunc(specialKeys);

/*__________________________________//all irrelevent for the basic code

    glutKeyboardFunc(keyboard);
    glutMotionFunc(mouse_move);
    glutMouseFunc(mouse_click);
    glutTimerFunc(1000,animation,1);
    glutSpecialFunc(specialKeys);
    glutReshapeFunc(ChangeSize);
*/
    glutMainLoop();
    return 0;
}
