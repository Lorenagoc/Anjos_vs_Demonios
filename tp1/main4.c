#include <GL/freeglut.h>
#include <stdio.h>
#include <math.h>
#include <time.h>
#include <string.h>
#include <SOIL/SOIL.h>

#define NUMCOISASBOAS 5
#define NUMCOISARUINS 3


//structs

typedef struct vetor{
    double x,y,z;
} Vetor;

typedef struct Objeto{
	double x,y;
	double velocidadeX;
	double velocidadeY;
	double largura,altura;
} objeto;

//------------------------------------------------------ fim das structs --------------------------------------------

//variaveis globais

int tempoAtual;
int tempoAnterior=0;

int texturaFundo=0;
int texturaAnzol=0;
int texturaCoisasBoas=0;
int texturaCoisasRuins =0;

int velocidadeCenario=3;

int larguraJanela, alturaJanela;
int k[256]; // Vetor de teclas, 1 para tecla pressionada e 0 para tecla solta

char Pontuacao[] = {"Score:"}; // guarda o score
int pontuacao = 0; //salva a pontuaçãos

objeto anzol;
objeto coisasRuins[NUMCOISASRUINS];
objeto coisasBoas[NUMCOISASBOAS];
objeto FundoPrincipal[2];

//------------------------------------------------- fim das variáveis globais --------------------------------------

void carregarTextura(int *textura,char *nome)//carregará as texturas para as variáveis
{
	*textura = SOIL_load_OGL_texture(
        nome,
        SOIL_LOAD_AUTO,
        SOIL_CREATE_NEW_ID,
		SOIL_FLAG_INVERT_Y
	);

    if (textura == 0) {
        printf("Erro do SOIL: '%s'\n", SOIL_last_result());
    }
}


void escreveTexto(void * font, char *s, float x, float y, float z) {
    int i;
    glColor3f(0,0,0);
    glRasterPos3f(x, y, z);

    for (i = 0; i < strlen(s); i++) {
        glutBitmapCharacter(font, s[i]);
    }
}

void resize(int width, int height){
    float razaoaspecto = (float) width / (float) height;

    glViewport(0, 0, width, height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glFrustum(-razaoaspecto, razaoaspecto, -1.0, 1.0, 3.5, 1000.0);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity() ;
}

void pressiona(unsigned char key,int x,int y){
	k[key]=1;
	    switch(key){
        case 27:
            exit(0);
            break;
    }
}

void solta(unsigned char key,int x,int y){
	k[key]=0;
}

void redimensiona(int w, int h){
   glViewport(0, 0, w, h);
   glMatrixMode(GL_PROJECTION);
   glLoadIdentity();
   glOrtho(-w/2, w/2, -h/2, h/2, -1, 1); //coordenadas dos plano cartesiano da tela
   alturaJanela = h;
   larguraJanela = w;
   glMatrixMode(GL_MODELVIEW);glBegin(GL_TRIANGLE_STRIP);
   glLoadIdentity();
}

void geradorCoisasBoas(){
	int x=1;
	int geradorX,geradorY;

        for(int i = 0; i<NUMCOISASBOAS ; i++ , x*=-1){ //O x irá variar de positivo para negativo, pois a origem está no meio da tela
	 
	        coisasBoas[i].largura=45;
	        coisasBoas[i].altura = 40;
	        geradorX = (rand()%600);
	        geradorY = ((rand()%250)+110)*-1;
	        
	        if(geradorX == 0 && geradorY == 300){
	            coisasBoas[i].y= ((geradorY));
	        }else
	        
	        coisasBoas[i].y= ((geradorY)); 
	        coisasBoas[i].x = ((geradorX)*x);
	        coisasBoas[i].velocidadeX = 0.0;
	        coisasBoas[i].velocidadeY = velocidadeCenario;
	 }
}
void geradorCoisasRuins(){
	int x=1;
	int geradorX,geradorY;

        for(int i = 0; i<NUMCOISASRUINS ; i++ , x*=-1){ //O x irá variar de positivo para negativo, pois a origem está no meio da tela
	 
	        coisasRuins[i].largura=45;
	        coisasRuins[i].altura = 40;
	        geradorX = (rand()%600);
	        geradorY = ((rand()%250)+110)*-1;
	        
	        if(geradorX == 0 && geradorY == 300){
	            coisasRuins[i].y= ((geradorY));
	        }else
	        
	        coisasRuins[i].y= ((geradorY)); 
	        coisasRuins[i].x = ((geradorX)*x);
	        coisasRuins[i].velocidadeX = 0.0;
	        coisasRuins[i].velocidadeY = velocidadeCenario;
	 }
}
void inicializa(void){

	//habilita mesclagem de cores, para termos suporte a texturas semi-transparentes
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	carregarTextura(&texturaFundo,"BackgroundSky.png");
	carregarTextura(&texturaAnzol, "Anzol.png");
	carregarTextura(&texturaCoisasBoas, "coisasBoas.png");
	carregarTextura(&texturaCoisasRuins, "coisasRuins.png");

	anzol.largura = 8;
	anzol.altura=10;
	anzol.x = 0.0; //a origem,é o centro
	anzol.y = 250.0;
	anzol.velocidadeX = 0.0;
	anzol.velocidadeY = 0.0;
	
	// modificar
	geradorCoisasBoas();
	geradorCoisasRuins();

	//cenário fundo

	FundoPrincipal[0].x = -640.0;
	FundoPrincipal[0].y = 360; // as coordenadas zero estão no meio da tela
	FundoPrincipal[0].largura = 1280.0;
	FundoPrincipal[0].altura = -720.0; 
	FundoPrincipal[1].x = -640.0;     // CENÁRIO
	FundoPrincipal[1].y = -360;
	FundoPrincipal[1].largura = 1280.0;
	FundoPrincipal[1].altura = -720.0;
	
	velocidadeCenario=3;
}

void desenhacoisasBoas(void){ //no im não será usada,pobrezinha
	for(int i =0; i<NUMCOISASBOAS ; i++){
	glPushMatrix(); //coloco minha matriz
		glTranslatef(coisasBoas[i].x,coisasBoas[i].y,0);
		glColor3f(1,1,1);
		glBegin(GL_TRIANGLE_FAN);
			glVertex2f(0,0);   //sentido anti horario
			glVertex2f(0,coisasBoas[i].altura);
			glVertex2f(coisasBoas[i].largura,coisasBoas[i].altura);
			glVertex2f(coisasBoas[i].largura,0);
		glEnd();
	glPopMatrix(); //tiro minha matriz
	        }
}

void desenhaAnzol(void){
	glPushMatrix(); //coloco minha matriz
		glTranslatef(anzol.x,anzol.y,0);
		glColor3f(1,1,1);
		glBegin(GL_TRIANGLE_FAN);
			glVertex2f(0,0);   //sentido anti horario
			glVertex2f(0,anzol.altura);
			glVertex2f(anzol.largura,anzol.altura);
			glVertex2f(anzol.largura,0);
		glEnd();
	glPopMatrix(); //tiro minha matriz
}


int colidiu(double posicaoAnzolX, double posicaoAnzolY ,double larguraAnzol,double alturaAnzol, double posicaoObjetoX , double posicaoObjetoY, double larguraObjeto, double alturaObjeto  ){
	if((posicaoObjetoX<=(posicaoAnzolX+(larguraAnzol)))&&((posicaoObjetoX+(larguraObjeto))>=(posicaoAnzolX+larguraAnzol))&&
	((posicaoObjetoY>=posicaoAnzolY+(alturaAnzol))&&((posicaoObjetoY-alturaObjeto)<=(posicaoAnzolY-(alturaAnzol)))))
	return 1;
}

void detectaColisoes(){
        for(int i =0 ; i<NUMCOISASBOAS ; i++){
                if((colidiu(anzol.x,anzol.y,anzol.largura,anzol.altura,coisasBoas[i].x,coisasBoas[i].y,coisasBoas[i].largura,coisasBoas[i].altura))){
                        pontuacao+=10;  
                        coisasBoas[i].largura = 0.0;
                        coisasBoas[i].altura = 0.0;     
                }
        }
	        for(int i =0 ; i<NUMCOISASRUINS ; i++){
                if((colidiu(anzol.x,anzol.y,anzol.largura,anzol.altura,coisasRuins[i].x,coisasRuins[i].y,coisasRuins[i].largura,coisasRuins[i].altura))){
                        pontuacao-=20;  
                        coisasRuins[i].largura = 0.0;
                        coisasRuins[i].altura = 0.0;     
                }
        }
	
}



void posiciona(){
	 anzol.x+=((anzol.velocidadeX)*(tempoAtual-tempoAnterior));
	 anzol.y+=((anzol.velocidadeY)*(tempoAtual-tempoAnterior));
	 
}

// essa parte faz manipulação de strings para carregar muitas imagens

void substituir(char *nome,int indice) // serve para ajudar a iniciar as texturas do personagem
{
	int i;
	for(i=0;i<strlen(nome);i++)
		if(nome[i]=='!')
			nome[i]=indice+48;
}

void copiarString(char *nome1,char *nome2)// serve para ajudar a iniciar as texturas do personagem
{
	int i;
	for(i=0;i<strlen(nome1);i++)
		nome2[i]=nome1[i];
	nome2[i]='\0';
	
}


void desenhaTextura(objeto R, int textura) // desenha a textura, com funcionamento similar a desenha quadrado
{
	glColor3f(1,1,1);
	glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, textura);
    glBegin(GL_TRIANGLE_FAN);
        // Associamos um canto da textura para cada vértice
        glTexCoord2f(0, 0); glVertex3f(R.x, R.y,  0);
        glTexCoord2f(1, 0); glVertex3f(R.x+R.largura,	R.y,  0);
        glTexCoord2f(1, 1); glVertex3f( R.x+R.largura,  R.y+R.altura,  0);
        glTexCoord2f(0, 1); glVertex3f(	R.x,  R.y+R.altura,  0);
    glEnd();
    glDisable(GL_TEXTURE_2D);
}


void comandos(){
   if(anzol.x <640 && anzol.x >(-640)){
    if(k['A']==1 || k['a']==1){
	anzol.velocidadeX= -0.5;
    }else 

    if(k['D']==1 || k['d']==1){
		anzol.velocidadeX= 0.5;
	} else anzol.velocidadeX=0;
   }

}


void andarCenario() 
{
	FundoPrincipal[0].y += velocidadeCenario;
	FundoPrincipal[1].y += velocidadeCenario;
	if(FundoPrincipal[0].y + FundoPrincipal[0].altura >= 360){
		FundoPrincipal[0].y = 360.0;
		FundoPrincipal[1].y = -360.0;
	}
	if(FundoPrincipal[0].y + FundoPrincipal[0].altura >= 357)
		geradorCoisasBoas();
		geradorCoisasRuins();
		
}

void andarObstaculo()//essa função cuida do deslocamento dos obstáculos(que até agora é apenas um)
{					 //quando todo o obstáculo passar pela tela, ele volta para o início da tela com tamanho diferente
		for(int i=0; i<NUMCOISASBOAS; i++){
		coisasBoas[i].y+=coisasBoas[i].velocidadeY;
		}
	        for(int i=0; i<NUMCOISASRUINS; i++){
		coisasRuins[i].y+=coisasRuins[i].velocidadeY;
		}
		
}



void atualiza(int x){
	//lógica do jogo
	
	tempoAtual = glutGet(GLUT_ELAPSED_TIME); //pega o tempo do teclado	
	posiciona();
	comandos();
	detectaColisoes();
	andarCenario();
	andarObstaculo();
	tempoAnterior = glutGet(GLUT_ELAPSED_TIME);

//parte de física
glutPostRedisplay(); //chama a desenha cena
glutTimerFunc(17,atualiza,0); //tempo de frame
}

void desenhaCena(void){
	//glClearColor(1, 1, 1, 0);
	glClear(GL_COLOR_BUFFER_BIT);
	//desenha as texturas
	desenhaTextura(FundoPrincipal[0],texturaFundo);
	desenhaTextura(FundoPrincipal[1],texturaFundo);
	//desenhaAnzol(); não há necessidade de chamar já q a desenha textura já tem as coordenadas
	desenhaTextura(anzol,texturaAnzol);
	//desenhacoisasBoas(); não há necessidade de chamar já q a desenha textura já tem as coordenadas
	for(int i=0; i<NUMCOISASBOAS; i++){
		desenhaTextura(coisasBoas[i],texturaCoisasBoas);
	}
	for(int i=0; i<NUMCOISASRUINS; i++){
		desenhaTextura(coisasRuins[i],texturaCoisasRuins);
	}
	escreveTexto(GLUT_BITMAP_TIMES_ROMAN_24,Pontuacao,500,300,0);
	char y[8]; sprintf(y, "%i", pontuacao); //transformar inteiro em string
	escreveTexto(GLUT_BITMAP_TIMES_ROMAN_24,y,570,300,0);
	glutSwapBuffers();
	
}


//main

int main(int argc, char **argv){
    srand(time(0));
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);
    larguraJanela = 1280;
    alturaJanela = 720;
    glutInitWindowSize(larguraJanela,alturaJanela);
    glutInitWindowPosition(50, 50);

    glutCreateWindow("JESUS");
    glutReshapeFunc(resize); //janela do tamanho que eu quiser
    glutDisplayFunc(desenhaCena);
    glutReshapeFunc(redimensiona);
    glutKeyboardFunc(pressiona);
    glutKeyboardUpFunc(solta);
    glutTimerFunc(0, atualiza, 0); //ao vivo,a cada framer atualiza
    inicializa();

    glutMainLoop();
    return 0;
}//------------------------------------------------- fim da main --------------------------------------

