#include <GL/glew.h>
#include <GL/freeglut.h>

//desenhar 9 quadrados de cores diferentes

void desenhaQuadrado(int x, int y, float red, float green, float blue){ //função que será chamada para desenhar um quadrado

    glColor3f(red, green, blue);

    // Começa a desenhar um polígono com os vértices especificados
    glBegin(GL_TRIANGLE_FAN);
        glVertex3f(x, y, 0);
        glVertex3f(x+40, y, 0);
        glVertex3f(x+40, y+40, 0);
        glVertex3f(x, y+40, 0);
    glEnd();
}

void desenhaMinhaCena() {
       glClear(GL_COLOR_BUFFER_BIT);
       
    desenhaQuadrado(0,0,0.9,0.9,0.9);
    desenhaQuadrado(40,0,1,0,0);
    desenhaQuadrado(80,0,0,1,0);
    desenhaQuadrado(0,40,0,0,1);
    desenhaQuadrado(40,40,1,1,0);
    desenhaQuadrado(80,40,1,0,1);
    desenhaQuadrado(0,40,0,1,1);
    desenhaQuadrado(40,80,0.6,0.6,0.6);
    desenhaQuadrado(80,80,1,1,1);
    glEnd();

    // diz ao OpenGL para colocar o que desenhamos na tela
    glFlush();
}

void teclaPressionada(unsigned char key, int x, int y) { //se o usuário pressionar esc a janela fecha
    // vê qual tecla foi pressionada
    switch(key) {
    case 27:      // Tecla "ESC"
        exit(0);  // Sai da aplicação
        break;
    default:
        break;
    }
}

void inicializa(void)
{

    glClearColor(0, 0, 0, 0);      
}

void redimensiona(int w, int h)
{
   glViewport(0, 0, w, h);
   glMatrixMode(GL_PROJECTION);
   glLoadIdentity();
   glOrtho(0, 120, 0, 120, -1, 1);
   glMatrixMode(GL_MODELVIEW);
   glLoadIdentity();
}

int main(int argc, char **argv)
{
    // acordando o GLUT
    glutInit(&argc, argv);

    // definindo a versão do OpenGL que vamos usar
    glutInitContextVersion(1, 1);
    glutInitContextProfile(GLUT_COMPATIBILITY_PROFILE);

    // configuração inicial da janela do GLUT
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGBA);
    glutInitWindowSize(500, 500);
    glutInitWindowPosition(120, 120);

    // abre a janela
    glutCreateWindow("Quadrados Coloridos");

    // registra callbacks para alguns eventos
    glutDisplayFunc(desenhaMinhaCena);
    glutReshapeFunc(redimensiona);
    glutKeyboardFunc(teclaPressionada);
    
    inicializa();

    // entra em loop e nunca sai
    glutMainLoop();
    return 0;
}
