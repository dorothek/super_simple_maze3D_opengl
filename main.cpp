#include <GL/glut.h>
#include <string>

enum
{
    EXIT // wyjœcie
};

// rozmiary bry³y obcinania
const GLdouble left = -10.0;
const GLdouble right = 10.0;
const GLdouble bottom = -10.0;
const GLdouble top = 10.0;
const GLdouble near = 50.0;
const GLdouble far = 70.0;

// wspolczynnik skalowania
GLfloat scale = 1.0;

// katy obrotu
GLfloat rotatex = -25.0;
GLfloat rotatey = 0.0;
int pom = 0; //pomocnicza do obslugi rotacji w trybie 1

// przesuniecie
GLfloat translatex = 0.0;
GLfloat translatey = 0.0;

// wskaŸnik naciœniêcia lewego przycisku myszki
int button_state = GLUT_UP;

// po³o¿enie kursora myszki
int button_x, button_y;
int torus_rotate = 0;

int moneta = 0; //zdobyte punkty
float angle = 0.0; //kat obrotu pomocnicza
int wyswietlanie = 0; //0 instrukcja, 1 plansza gry, 2 zakonczenie

//mapa labiryntu
char tab[16][16] = {
    "*************$*",
    "* *m      m   *",
    "*m  *   *** ***",
    "* ***** m*  * *",
    "*  ***   * **m*",
    "*m  *   m* ** *",
    "* *   **** ** *",
    "*m  *    m  * *",
    "* * *  ****   *",
    "***** ******* *",
    "*m  *   *m    *",
    "* * * * ***** *",
    "* *   *      m*",
    "* *** **** ** *",
    "* *m          *",
    "*L*************",
};

void draw() {  
    //ustawienia oswietlenia
    glEnable(GL_LIGHTING);
    //kolor œwiat³a otoczenia
    float kolorOtoczenie[] = { 0.5, 0.5, 0.5, 1.0 };
    glLightfv(GL_LIGHT0, GL_AMBIENT, kolorOtoczenie);

    glShadeModel(GL_SMOOTH);

    //ustawienia materialu bry³y
    float materialOdbicie[] = { 0.35, 0.35, 0.35, 0.0 };
    glMaterialfv(GL_FRONT, GL_SPECULAR, materialOdbicie);
    glMaterialf(GL_FRONT, GL_SHININESS, 2.0);

    //pozycja zrodla
    GLfloat position[4] = { 400.0, 60.0, 50.0, 0.0 };

    glEnable(GL_DEPTH_TEST); //wazne
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);;
    glEnable(GL_COLOR_MATERIAL); //wazne

    //wlaczanie oswietlenia
    glLightfv(GL_LIGHT0, GL_POSITION, position);
    glEnable(GL_LIGHT0);


    if (wyswietlanie == 1) {
        //rysowanie sceny
        glTranslatef(-8.0, 9.0, 0.0);

        // podstawa labiryntu
        glBegin(GL_POLYGON);
        glColor4f(0.2, 1.0, 1.0, 0.0);
        glVertex3f(-2.0, -17, -0.5); //lewa dolna
        glVertex3f(-2.0, 2.5, -0.5); //lewa gorna
        glVertex3f(16.5, 2.5, -0.5); //prawa gorna
        glVertex3f(16.5, -17.0, -0.5); //prawa dolna
        glEnd();

        //rysowanie planszy
        for (int i = 0; i < 16; i++)
        {
            for (int j = 0; j < 16; j++)
            {
                if (tab[i][j] == '*')
                {
                    //rysowanie œcian labiryntu
                    glColor3f(0.0, 0.0, 1.0);
                    glutSolidCube(1.0);
                    glTranslatef(1.0, 0.0, 0.0);
                }
                else if (tab[i][j] == 'm')
                {
                    glColor3f(1.0, 0.2, 0.2);
                    //rozmieszczanie punktow
                    float a = angle;
                    glPushMatrix();
                    //rotacja punktow w miejscu
                    glRotatef(a, 1.0, 0.0, 0.0);
                    glRotatef(a, 0.0, 1.0, 0.0);
                    glRotatef(a, 0.0, 0.0, 1.0);
                    glutSolidTorus(0.13, 0.21, 13, 20);
                    glPopMatrix();
                    glTranslatef(1.0, 0.0, 0.0);
                }
                else if (tab[i][j] == '$')
                {
                    glColor3f(1.2, 1.0, 0.2);
                    //meta
                    glutSolidTorus(0.23, 0.23, 13, 20);
                    glTranslatef(1.0, 0.0, 0.0);
                }
                else if (tab[i][j] == 'L')
                {
                    glColor3f(0.0, 0.0, 0.0);
                    //ludzik
                    glTranslatef(0.0, 0.0, -0.5);
                    glutSolidCone(0.4, 1.3, 20, 10);
                    glTranslatef(0.0, 0.0, 1.2);
                    glutSolidSphere(0.2, 20, 10);
                    glTranslatef(1.0, 0.0, -0.7);
                }
                else
                    glTranslatef(1.0, 0.0, 0.0);
            }
            glTranslatef(-16.0, -1.0, 0.0);
        }

        glPushMatrix();
        glTranslatef(-1.0, -0.5, -0.4);
        glScalef(0.004, 0.004, 0.0);
        glColor3f(0, 0, 0);
        const char* string = "\'U\': Up   \'D\': Down   \'R\': Right   \'L\': Left   \'PPM\': Wyjscie";

        while (*string) {
            glutStrokeCharacter(GLUT_STROKE_ROMAN, *string++);
        }
        glPopMatrix();
    }
    else if (wyswietlanie== 0) {
        rotatex = 0;
        glPushMatrix();
        glTranslatef(-1.7, 7.0, -0.4);
        glScalef(0.003, 0.004, 0.0);
        glColor3f(0, 0, 0);
        const char string[8][150] = { "WITAJ!",
            "Cel gry to przejscie labiryntu czarnym ludzikiem, od punktu startu do zoltego krazka.",
            "Aby poruszac sie po labiryncie uzywaj nastepujacych klawiszy: U: Up, D: Down, R: Right, L: Left.",
            "Mozesz rotowac plansze gry przy pomocy strzalek, oraz przyblizac i oddalac widok za pomoca + i - .",
            "Zblizajac sie do rozowych torusow mozesz zdobyc dodatkowe punkty.",
            "Aby oposcic gre nacisnij PPM i wybierz opcje 'Wyjscie'.",
            "Aby rozpoczac nacisnij 'T'.",
            "POWODZENIA! :)"};
        int i = 0;
        for(int j=0; j<8; j++){
            while (string[j][i]) {
                glutStrokeCharacter(GLUT_STROKE_ROMAN, string[j][i++]);              
            }
            switch (j)
            {
            case 0: glTranslatef(-2900, -300.0, 0.0); break; 
            case 1: glTranslatef(-6000, -200.0, 0.0); break;  
            case 2: glTranslatef(-6500, -200.0, 0.0); break; 
            case 3: glTranslatef(-5900, -200.0, 0.0); break; 
            case 4: glTranslatef(-3900, -200.0, 0.0); break;
            case 5: glTranslatef(-2800, -200.0, 0.0); break;
            case 6: glTranslatef(-1200, -300.0, 0.0); break;
            }
            i = 0;
        }
        glPopMatrix();
        glColor3f(0.0, 0.0, 0.0);
        //ludzik
        glTranslatef(-0.7, -2.3, -0.5);
        glRotatef(90, 0.0, 0.0, 1.0);
        float a = angle;
        glPushMatrix();
        glRotatef(a, 1.0, 0.0, 0.0);
        glRotatef(a, 0.0, 1.0, 0.0);
        glRotatef(a, 0.0, 0.0, 1.0);
        
        glutSolidCone(0.4, 1.3, 20, 10);
        glTranslatef(0.0, 0.0, 1.2);
        glutSolidSphere(0.2, 20, 10);
        glTranslatef(1.0, 0.0, -0.7);
        glPopMatrix();
    }
    else if (wyswietlanie == 2) {
        rotatex = 0;
        glPushMatrix();
        glTranslatef(-9.0, 3.0, -0.4);
        glScalef(0.0035, 0.004, 0.0);
        glColor3f(0, 0, 0);

        char string[2][250] = { "",
            "Aby opuscic okienko uzyj PPM. Milego dnia!"};
        snprintf(string[0], 250, "Gratulacje, doszedles do konca! :) Liczba zdobytych przez Ciebie punktow to %d.", moneta);

        for (int j = 0; j < 2; j++) {
        int i = 0;
            while (string[j][i]) {
                glutStrokeCharacter(GLUT_STROKE_ROMAN, string[j][i++]);               
            }
            switch (j)
            {
            case 0: glTranslatef(-5200, -200.0, 0.0); break;
            }
        }

        //3 torusy na zakonczenie
        //1
        glPopMatrix();
        glColor3f(1.0, 0.2, 0.2);
        glTranslatef(-0.7, -1.0, -0.5);
        glRotatef(90, 0.0, 0.0, 1.0);
        float a = angle;
        glPushMatrix();
        glRotatef(a, 1.0, 0.0, 0.0);
        glRotatef(a, 0.0, 1.0, 0.0);
        glRotatef(a, 0.0, 0.0, 1.0);
        glutSolidTorus(0.13, 0.21, 13, 20);
        glTranslatef(1.0, 0.0, 0.0);  
        glPopMatrix();
        //2
        glPopMatrix();
        glColor3f(1.0, 0.2, 0.2);
        glTranslatef(0.2, -1.0, 0.0);
        glRotatef(90, 0.0, 0.0, 1.0);
        glPushMatrix();
        glRotatef(a, 1.0, 0.0, 0.0);
        glRotatef(a, 0.0, 1.0, 0.0);
        glRotatef(a, 0.0, 0.0, 1.0);
        glutSolidTorus(0.13, 0.21, 13, 20);
        glPopMatrix();
        //2
        glPopMatrix();
        glColor3f(1.0, 0.2, 0.2);
        glTranslatef(0.0, 1.8, 0.0);
        glPushMatrix();
        glRotatef(a, 1.0, 0.0, 0.0);
        glRotatef(a, 0.0, 1.0, 0.0);
        glRotatef(a, 0.0, 0.0, 1.0);
        glutSolidTorus(0.13, 0.21, 13, 20);
        glPopMatrix();
    }
}

// generacja sceny
void Display()
{
    // kolor tla
    glClearColor(1.0, 0.8, 0.0, 1.0);

    // wybor macierzy modelowania
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    // przesuniecie ukladu wspolrzednych obiektu do srodka bryly odcinania
    glTranslatef(0, 0, -(near + far) / 2);

    // przesuniecie obiektu - ruch myszka
    glTranslatef(translatex, translatey, 0.0);

    // skalowanie obiektu
    glScalef(scale, scale, scale);

    // obroty obiektu - klawisze kursora
    glRotatef(rotatex, 1.0, 0, 0);
    glRotatef(rotatey, 0, 1.0, 0);

    // rysowanie planszy
    glPushMatrix();
    draw();
    glPopMatrix();

    // skierowanie poleceñ do wykonania
    glFlush();
    glutSwapBuffers();
}

// zmiana wielkosci okna
void Reshape(int width, int height)
{
    // obszar renderingu
    glViewport(0, 0, width, height);

    // wybor rzutowania
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    glFrustum(left, right, bottom, top, near, far);

    // generowanie sceny
    Display();
}

//znajdywanie aktualnej pozycji ludka w labiryncie
void findCurrPos(int * x, int * y) {
    for (int i = 0; i < 16; i++) {
        for (int j = 0; j < 16; j++) {
            if (tab[i][j] == 'L') // obecna poycja w labiryncie
            {
                *x = i;
                *y = j;
                return;
            }
        }
    }
}

// obsluga klawiatury
void Keyboard(unsigned char key, int x, int y)
{
    int xpos, ypos;
    findCurrPos(&xpos, &ypos);

	if (tab[xpos][ypos] == 'L') // obecna pozycja w labiryncie
	{
		switch (key)
		{
        case 't':
            wyswietlanie = 1;
            if (pom == 0) {
                rotatex = -25.0;
                pom=1;
            }
            break;
        //case 'p':
		case 'r':  //right
			if (tab[xpos][ypos + 1] != '*') {
				if (tab[xpos][ypos + 1] == 'm') {
					moneta += 1;
				}
				tab[xpos][ypos + 1] = 'L';
				tab[xpos][ypos] = ' ';
			}
			break;
        //case 'q':
		case 'u': //up
			if (tab[xpos - 1][ypos] != '*') {
				if (tab[xpos - 1][ypos] == 'm') {
					moneta += 1;
				}
				if (tab[xpos - 1][ypos] == '$')
				{
					wyswietlanie=2;
					//komunikat o zdobytej ilosci punktow, zakonczenie gry
				}
				tab[xpos - 1][ypos] = 'L';
				tab[xpos][ypos] = ' ';
			}
			break;
        //case 'o':
		case 'l': // left
			if (tab[xpos][ypos - 1] != '*') {
				if (tab[xpos][ypos - 1] == 'm') {
					moneta += 1;
				}
				tab[xpos][ypos - 1] = 'L';
				tab[xpos][ypos] = ' ';
			}
			break;
        //case 'a':
		case 'd': //down 
			if (tab[xpos + 1][ypos] != '*') {
				if (tab[xpos + 1][ypos] == 'm') {
					moneta += 1;
				}
				tab[xpos + 1][ypos] = 'L';
				tab[xpos][ypos] = ' ';
			}
			break;
		}
		glutPostRedisplay();
	}
    // klawisz +
    if (key == '+' && scale < 1.7) {
        scale += 0.1;
    }
    else if (key == '-' && scale > 0.8)
        {
                scale -= 0.1;  
        }
    // odrysowanie okna
    Reshape(glutGet(GLUT_WINDOW_WIDTH), glutGet(GLUT_WINDOW_HEIGHT));
}

// obs³uga klawiszy funkcyjnych i klawiszy kursora
void SpecialKeys(int key, int x, int y)
{
    switch (key)
    {
    case GLUT_KEY_LEFT: //w lewo
        if (rotatey > -80) {
            rotatey -= 1;
        }
        break;
    case GLUT_KEY_UP: //w gore
        if (rotatex > -80) {
            rotatex -= 1;
        }
        break;
    case GLUT_KEY_RIGHT: //w prawo
        if (rotatey < 80) {
            rotatey += 1;
        }
        break;
    case GLUT_KEY_DOWN: //w dol
        if (rotatex < 80)
        {
            rotatex += 1;
        }
        break;
    }
    Reshape(glutGet(GLUT_WINDOW_WIDTH), glutGet(GLUT_WINDOW_HEIGHT));
}

// obsluga przyciskow myszki
void MouseButton(int button, int state, int x, int y)
{
    if (button == GLUT_LEFT_BUTTON)
    {
        // zapamietanie stanu lewego przycisku myszki
        button_state = state;
        // zapamietanie polozenia kursora
        if (state == GLUT_DOWN)
        {
            button_x = x;
            button_y = y;
        }
    }
}

// obsluga ruchu myszka
void MouseMotion(int x, int y)
{
    if (button_state == GLUT_DOWN)
    {
        // pobranie macierz modelowania
        GLdouble model[16];
        glGetDoublev(GL_MODELVIEW_MATRIX, model);

        // pobranie macierzy rzutowania
        GLdouble proj[16];
        glGetDoublev(GL_PROJECTION_MATRIX, proj);
        GLint view[4];
        glGetIntegerv(GL_VIEWPORT, view);

        // tablice wspolrzednych przestrzeni widoku
        GLdouble prev[3], curr[3];
        gluUnProject(button_x, button_y, 0.0, model, proj, view, prev + 0, prev + 1, prev + 2);
        gluUnProject(x, y, 0.0, model, proj, view, curr + 0, curr + 1, curr + 2);

        // obliczenie wspó³rzêdnych wektora przesuniêcia obiektu
        translatex += curr[0] - prev[0];
        translatey += prev[1] - curr[1];

        button_x = x;
        button_y = y;

        glutPostRedisplay();
    }
}

// obsluga menu PPM
void Menu(int value)
{
    switch (value)
    {
    case EXIT:
        exit(0);
    }
}

//obsluga czasu
void timer(int)
{
    glutPostRedisplay();
    glutTimerFunc(1000 / 60, timer, 0);

    angle += 0.8 * 3;
    if (angle > 360)
        angle = angle - 360.0;
}

int main(int argc, char* argv[])
{
    // inicjalizacja biblioteki GLUT
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    glutInitWindowSize(1000, 1000);
    glutCreateWindow("Labirynt");

    // generowanie sceny
    glutDisplayFunc(Display);

    // zmiana rozmiaru okna
    glutReshapeFunc(Reshape);

    // obsluga klawiatury
    glutKeyboardFunc(Keyboard);
    glutSpecialFunc(SpecialKeys);

    // obsluga myszki
    glutMouseFunc(MouseButton);
    glutMotionFunc(MouseMotion);
    glutCreateMenu(Menu);
#ifdef WIN32
    glutAddMenuEntry("Wyjscie", EXIT);
#endif
    glutAttachMenu(GLUT_RIGHT_BUTTON);

    glutTimerFunc(0, timer, 0);
    // obsluga petli komunikatow
    glutMainLoop();
    return 0;
}