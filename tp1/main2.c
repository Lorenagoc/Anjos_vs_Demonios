#include <GL/freeglut.h>
#include <stdio.h>
#include <math.h>
#include <time.h>
#include <string.h>


typedef struct vetor{
    double x,y,z;
} Vetor;

typedef struct Objeto{
	double x,y;
	double velocidadeX;
	double velocidadeY;
	double largura,altura;
} objeto;


//variaveis globais

int tempoAtual;
int tempoAnterior=0;

int larguraJanela, alturaJanela;
int k[256]; // Vetor de teclas, 1 para tecla pressionada e 0 para tecla solta

objeto anzol;
objeto coisasBoas[10];


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

void inicializa(void){
	anzol.largura = 15;
	anzol.altura=15;
	anzol.x = 0.0; //a origem,é o centro
	anzol.y = 300.0;
	anzol.velocidadeX = 0.0;
	anzol.velocidadeY = -0.08;
	
	// modificar
	int x=1;
        for(int i = 0; i<10 ; i++ , x*=-1){ //O x irá variar de positivo para negativo, pois a origem está no meio da tela
	        coisasBoas[i].largura=30;
	        coisasBoas[i].altura = 30;
	        coisasBoas[i].x = ((rand()%500)*x);
	        coisasBoas[i].y= (rand()%300);
	        coisasBoas[i].velocidadeX = 0.0;
	        coisasBoas[i].velocidadeY = 0.0;
	 }
	
}

void desenhacoisasBoas(void){
	for(int i =0; i<10 ; i++){
	glPushMatrix(); //coloco minha matriz
		glTranslatef(coisasBoas[i].x,coisasBoas[i].y,0);
		glColor3f(1,0,0);
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
		glColor3f(1,0,0);
		glBegin(GL_TRIANGLE_FAN);
			glVertex2f(0,0);   //sentido anti horario
			glVertex2f(0,anzol.altura);
			glVertex2f(anzol.largura,anzol.altura);
			glVertex2f(anzol.largura,0);
		glEnd();
	glPopMatrix(); //tiro minha matriz
}


int colidiu(double posicaoAnzolX, double posicaoAnzolY ,double larguraAnzol,double alturaAnzol, double posicaoObjetoX , double posicaoObjetoY, double larguraObjeto, double alturaObjeto  ){
	if((posicaoObjetoX<=posicaoAnzolX)&&((posicaoObjetoX+larguraObjeto)>=(posicaoAnzolX+larguraAnzol))&&
	((posicaoObjetoY>=posicaoAnzolY)&&((posicaoObjetoY-alturaObjeto)<=(posicaoAnzolY-alturaAnzol))))
	return 1;
}

void detectaColisoes(){
        for(int i =0 ; i<10 ; i++){
                if((colidiu(anzol.x,anzol.y,anzol.largura,anzol.altura,coisasBoas[i].x,coisasBoas[i].y,coisasBoas[i].largura,coisasBoas[i].altura))){
                        printf("Opa");   
                        coisasBoas[i].largura = 0.0;
                        coisasBoas[i].altura = 0.0;     
                }
        }
}

tempoAnterior = glutGet(GLUT_ELAPSED_TIME);

void posiciona(){
	 anzol.x+=((anzol.velocidadeX)*(tempoAtual-tempoAnterior));
	 anzol.y+=((anzol.velocidadeY)*(tempoAtual-tempoAnterior));
	 
}

void comandos(){
   
    if(k['A']==1 || k['a']==1){
	anzol.velocidadeX= -0.5;
    }else 

    if(k['D']==1 || k['d']==1){
		anzol.velocidadeX= 0.5;
	} else anzol.velocidadeX=0;

}
void atualiza(int x){
	//lógica do jogo
	
	tempoAtual = glutGet(GLUT_ELAPSED_TIME); //pega o tempo do teclado	
	posiciona();
	comandos();
	detectaColisoes();

//parte de física
glutPostRedisplay(); //chama a desenha cena
glutTimerFunc(17,atualiza,0); //tempo de frame
}

void desenhaCena(void){
	glClearColor(0,0.6, 1,0);
	glClear(GL_COLOR_BUFFER_BIT);
	desenhaAnzol();
	desenhacoisasBoas();
	glutSwapBuffers();

	
}

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
}
