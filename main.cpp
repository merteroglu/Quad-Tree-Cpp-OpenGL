#include "QuadTree.h"
#include <algorithm>
#include <cmath>
#include <ctime>
#include <fstream>
#include <vector>
#include <iostream>
#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif
#include <stdlib.h>
using namespace std;

static int quadH = 512; // Quad Genisligi
static int quadW = 512;  // Quad Uzunlugu
static int pencereUzunlugu = 512; // Pencere uzunlugu
static int pencereGenisligi = 712; // Pencere Genisligi
static double ZOOM_P = 16.0; // Zoom Derecesi
static double GEZ_P = 16.0;  // Sag sol hareket derecesi

// Grafik koordinat hesaplama degiskenleri
static float graphXMin = -18; 
static float graphXMax = 18;
float graphXRange = graphXMax - graphXMin;
float pixToXCoord = graphXRange / pencereGenisligi;

static float graphYMin = -18;
static float graphYMax = 18;
float graphYRange = graphYMax - graphYMin;
float pixToYCoord = graphYRange / pencereUzunlugu;

// Mouse Click Degiskenleri
static bool solMouseDown = 0;
static bool sagMouseDown = 0;

vector <point> tumNoktalar; // Agactaki Tum noktalarýn tutuldugu liste
vector <point> bulunanNoktalar;  // Arama islemi sonucunda bulunan noktalarýn tutuldugu liste

// Arama çemberi bilgileri
point kareMerkezi(0, 0);
long double circleR = 64;
point kareGenisligi(64, 64);

point merkez(0, 0); // QuadTree merkezi
point eksen(512, 512); // QuadTree genisligi
static int kumeBoyutu = 1; // Yaprakta tutulcak maks obje sayisi
QuadTree <int>* qtree; // Agac root

long double randomNokta(); // Random nokta oluþturma fonksiyonu prototipi

// Button Yapýsý
struct Button
{
	int   x;							
	int   y;							
	int   w;							
	int   h;											
	char* label;						
};
typedef struct Button Button;

// Buttonlar
Button Button1 = { 600,500, 200,70, "Random"};
Button Button2 = { 600,400, 200,70, "Temizle"};
Button Button3 = { 600,100, 200,70, "Yazdir" };
Button Button4 = { 600,200, 80,80, "+" };
Button Button5 = { 750,200, 80,80, "-" };


/*
Ekrana yazi yazar.
@param font : Yazi tipi
@param text : Yazi
@param x : Yazinin baslangic x koordinatý
@param y : Yazinin baslangic y koordinatý
*/
void ekranaYaz(void *font, char *text, int x, int y)
{
	glRasterPos2i(x, y);

	while (*text != '\0')
	{
		glutBitmapCharacter(font, *text);
		++text;
	}
}

/*
Buttonlarý cizen fonksiyon.
@param *b : Button tipinde yapý
*/
void butonCiz(Button *b)
{
	int fontx;
	int fonty;

	if (b)
	{
		glColor3f(0.6f, 0.6f, 0.6f);

		glBegin(GL_QUADS);
		glVertex2i(b->x, b->y);
		glVertex2i(b->x, b->y + b->h);
		glVertex2i(b->x + b->w, b->y + b->h);
		glVertex2i(b->x + b->w, b->y);
		glEnd();

		glLineWidth(3);
		
		glColor3f(0.8f, 0.8f, 0.8f);

		glBegin(GL_LINE_STRIP);
		glVertex2i(b->x + b->w, b->y);
		glVertex2i(b->x, b->y);
		glVertex2i(b->x, b->y + b->h);
		glEnd();

		glColor3f(0.8f, 0.8f, 0.8f);
		
		glBegin(GL_LINE_STRIP);
		glVertex2i(b->x, b->y + b->h);
		glVertex2i(b->x + b->w, b->y + b->h);
		glVertex2i(b->x + b->w, b->y);
		glEnd();

		glLineWidth(1);


		const unsigned char *text =(unsigned char*) b->label;
		fontx = b->x + 10 + (glutBitmapLength(GLUT_BITMAP_HELVETICA_10, text)/2);
		fonty = b->y - 10 + (b->h + 10) / 2;

		glColor3f(1, 1, 1);
		ekranaYaz(GLUT_BITMAP_HELVETICA_10, b->label, fontx, fonty);
	}
}

/*
Butona týklanýp týklanmadýðýný kontrol eder
@param b : buton
@param x : týklanan noktanýn x koordinati
@param y : týklanan noktanýn y koordinati
*/
int ButtonClickTest(Button* b, int x, int y)
{
	if (b)
	{
		if (x > b->x      &&
			x < b->x + b->w &&
			y > b->y      &&
			y < b->y + b->h) {
			return 1;
		}
	}

	return 0;
}


/*
Zoom iþlemleri sonrasýnda koordinat pixel ayarlarý yapan fonksiyon
*/
void  initializeViewMatrix()
{
	graphXRange = graphXMax - graphXMin;
	graphYRange = graphYMax - graphYMin;
	pixToXCoord = graphXRange / quadW;
	pixToYCoord = graphYRange / quadH;
}

/*
Pencerenin boyutlarýný ayarlar
@param w : genislik
@param h : yukseklik
*/
static void resize(int w, int h)
{
	pencereGenisligi = w;
	pencereUzunlugu = h;
	initializeViewMatrix();
	glViewport(0, 0, (GLsizei)quadW, (GLsizei)quadH);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(graphXMin, graphXMax, graphYMin, graphYMax);
	glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

/*
Sað,Sol,Yukarý,Aþaðý hareket ettirir
@param xAmount : X deki hareket miktarý
@param yAmount : Y deki hareket miktarý
*/
void gez(double xAmount, double yAmount)
{
	graphXMin += xAmount;
	graphXMax += xAmount;
	graphYMin += yAmount;
	graphYMax += yAmount;

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(graphXMin, graphXMax, graphYMin, graphYMax);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}
/*
Zoom iþlemi yapan fonksiyon
@param xAmount : X deki artýþ-azalýþ
@param yAmount : Y deki artýþ-azalýþ
*/
void zoom(double xAmount, double yAmount)
{
	graphXMin -= xAmount;
	graphXMax += xAmount;
	graphYMin -= yAmount;
	graphYMax += yAmount;

	initializeViewMatrix();
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(graphXMin, graphXMax, graphYMin, graphYMax);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

/*
Random deðer oluþturan fonksiyon
*/
long double randomNokta() {
	long double x = ((long double)rand() / (long double)RAND_MAX);
	return x;
}

/*
Çemberi içerisine alan en kucuk kare içerisinde ki noktalarý bulan fonksiyon
*/
static void noktalariBul()
{
	vector <pair <point, int> > bulunanlar;
	
	bulunanlar = qtree->bolgedekiObjeleriBul(
	{ kareMerkezi.x - kareGenisligi.x, kareMerkezi.y - kareGenisligi.y },
	{ kareMerkezi.x + kareGenisligi.x, kareMerkezi.y + kareGenisligi.y } );
	
	
	bulunanNoktalar.clear();
	bulunanNoktalar.resize(bulunanlar.size());

	for (int i = 0; i < bulunanlar.size(); ++i) {
		bulunanNoktalar[i] = bulunanlar[i].first;
	}
}

/*
Opengl renklerini dðeiþtiren fonksiyon.Bulunan noktalarý farklý renklerde basmak için
@param index : Nokta sýrasýna göre renklendirme
*/
void renkDegistir(int index) {
	index %= 10;

	switch (index)
	{
	case 0:
		glColor3f(1, 0, 0);
		break;
	case 1:
		glColor3f(1, 1, 0);
		break;
	case 2:
		glColor3f(0, 1, 0);
		break;
	case 3:
		glColor3f(0, 1, 1);
		break;
	case 4:
		glColor3f(0, 0, 1);
		break;
	case 5:
		glColor3f(1, 0, 1);
		break;
	case 6:
		glColor3f(0.8, 0.2, 1);
		break;
	case 7:
		glColor3f(0.5, 0.5, 0.5);
		break;
	case 8:
		glColor3f(1, 0.5, 1);
		break;
	case 9:
		glColor3f(0.3, 0.2, 0.6);
		break;
	default:
		break;
	}


}

/*
Ekrana basma iþlemlerinin yapýldýðý fonksiyon
*/
static void display(void)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glColor3f(1, 1, 1);

	qtree->ciz();

	
	glColor3f (1, 1, 1);
	glPointSize (5.0);
	glBegin (GL_POINTS);
	for (unsigned i=0; i<tumNoktalar.size(); ++i){
	glVertex2f (tumNoktalar[i].x, tumNoktalar[i].y);
	}
	glEnd();
	
	
	glColor3f(0, 1, 0);
	glBegin(GL_LINE_LOOP);
	
	for (int ii = 0; ii < 100; ii++) {
		float theta = 2.0f * 3.1415926f * float(ii) / float(100);//get the current angle 
		float x = circleR * cosf(theta);//calculate the x component 
		float y = circleR * sinf(theta);//calculate the y component 
		glVertex2f(x + kareMerkezi.x, y + kareMerkezi.y);//output vertex 
	}
	glEnd();

	glPointSize(5.0);
	glBegin(GL_POINTS);
	for (unsigned i = 0; i<bulunanNoktalar.size(); ++i) {
		renkDegistir(i);
		glVertex2f(bulunanNoktalar[i].x, bulunanNoktalar[i].y);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}
	glEnd();

	butonCiz(&Button1);
	butonCiz(&Button2);
	butonCiz(&Button3);
	ekranaYaz(GLUT_BITMAP_HELVETICA_10,"Arama Cemberi Yari Capi", 600, 300);
	butonCiz(&Button4);
	butonCiz(&Button5);

	glFlush();
	glutSwapBuffers();
}

/*
Bulunan noktalarý sýralayýp ekrana yazar
@param vector<vertex> : Bulunan noktalarýn tutulduðu verctor
*/
void selectionSort(vector<point>  toSort)
{
	for (vector<point>::size_type pos = 0; pos < toSort.size(); ++pos)
	{
		vector<point>::size_type min = pos;

		for (vector<point>::size_type i = pos + 1; i < toSort.size(); ++i)
		{
			if (toSort[i].x < toSort[min].x) {
				min = i;
			}else if (toSort[i].x == toSort[min].x) {
				if (toSort[i].y < toSort[min].y)
					min = i;
			}
		}

		if (min != pos)
		{
			swap(toSort[min].x, toSort[pos].x);
			swap(toSort[min].y, toSort[pos].y);
		}
	}

	cout << "Bulunan noktalar (" << toSort.size() << " adet) :\n";
	for (int i = 0; i < toSort.size(); i++) {
		cout << "x:" << toSort[i].x << " y:" << toSort[i].y << endl;
	}
	printf("\n");

}

/*
Arama çemberi içerisinde ki noktalarýn koordinatlarýný console a yazan fonksiyon
*/
void bulunanNoktalariYaz() {
	selectionSort(bulunanNoktalar);
}

/*
OpenGl kütüphanesinin key olaylarýný kontrol etmek için istediði fonksiyon. Basýlan tuþlar sonrasý iþlemler yapýlýr
@param key : Basýlan tuþ
@param x,y : X ve Y kooordinatlarý
*/
static void key(unsigned char key, int x, int y)
{
	switch (key) {

	case 'h':
		if (circleR < 256) {
			circleR += 16;
			kareGenisligi = { circleR,circleR };
			noktalariBul();
			}
		break;
	case 'j':
		if (circleR > 32) {
			circleR -= 16;
			kareGenisligi = { circleR,circleR };
			noktalariBul();
			}
		break;
	case 'p':
	case 'P':
		bulunanNoktalariYaz();
		break;

	case 'a':
	case 'A':
		gez(-graphXRange / GEZ_P, 0);
		break;

	case 'd':
	case 'D':
		gez(graphXRange / GEZ_P, 0);
		break;

	case 'w':
	case 'W':
		gez(0, graphYRange / GEZ_P);
		break;

	case 's':
	case 'S':
		gez(0, -graphYRange / GEZ_P);
		break;

	case 'r':
	case 'R':
		for (int i = 0; i < 20; ++i) {
			point yeniNokta(eksen.x - (2 * eksen.x * randomNokta()),
				eksen.y - (2 * eksen.y * randomNokta()));
			tumNoktalar.push_back(yeniNokta);
			qtree->ekle(yeniNokta, 1);
		}
		break;
		
	case 'b':
		delete qtree;
		qtree = new QuadTree <int>(merkez, eksen, kumeBoyutu);
		tumNoktalar.clear();
		bulunanNoktalar.clear();
		break;

	case '+':
	case '=':
		zoom(graphXRange / ZOOM_P, graphYRange / ZOOM_P);
		break;

	case '-':
	case '_':
		zoom(-graphXRange / ZOOM_P, -graphYRange / ZOOM_P);
		break;

	case 'f':
		noktalariBul();
		break;
	}

	glutPostRedisplay();
}

/*
OpenGl kütüphanesinin mouse eventlarýný kontrol etmek için istediði fonksiyon. Mouse olaylarý sonrasý yapýlacak iþlemler
@param button : Basýlan mouse tuþu
@param state : Tuþun durumu up-down
@param x,y : X ve Y koordinatlarý
*/
static void mouse(int button, int state, int x, int y)
{
	point yeniNokta(x*pixToXCoord + graphXMin, -y*pixToYCoord + graphYMax);

	switch (button) {
	case GLUT_LEFT_BUTTON:
		switch (state) {
		case GLUT_DOWN:
			if (yeniNokta.x > -quadW && yeniNokta.x < quadW && yeniNokta.y < quadH && yeniNokta.y>-quadH) {
				solMouseDown = 1;
				tumNoktalar.push_back(yeniNokta);
				qtree->ekle(yeniNokta, 1);
			}
			else {

				if (ButtonClickTest(&Button1, yeniNokta.x, yeniNokta.y)) {
					for (int i = 0; i < 20; ++i) {
						point yeniNokta(eksen.x - (2 * eksen.x * randomNokta()),
							eksen.y - (2 * eksen.y * randomNokta()));
						tumNoktalar.push_back(yeniNokta);
						qtree->ekle(yeniNokta, 1);
					}
				}

				if (ButtonClickTest(&Button2, yeniNokta.x, yeniNokta.y)) {
					delete qtree;
					qtree = new QuadTree <int>(merkez, eksen, kumeBoyutu);
					tumNoktalar.clear();
					bulunanNoktalar.clear();
				}

				if (ButtonClickTest(&Button3, yeniNokta.x, yeniNokta.y)) {
					bulunanNoktalariYaz();
				}

				if (ButtonClickTest(&Button4, yeniNokta.x, yeniNokta.y)) {
					if (circleR < 256) {
						circleR += 16;
						kareGenisligi = { circleR,circleR };
						noktalariBul();
					}
				}
			
				if (ButtonClickTest(&Button5, yeniNokta.x, yeniNokta.y)) {
					if (circleR > 32) {
						circleR -= 16;
						kareGenisligi = { circleR,circleR };
						noktalariBul();
					}
				}
			}
			break;

		case GLUT_UP:
			solMouseDown = 0;
			break;
		}
		break;
	case GLUT_RIGHT_BUTTON:
		switch (state) {
		case GLUT_DOWN:
			if (yeniNokta.x > -quadW && yeniNokta.x < quadW && yeniNokta.y < quadH && yeniNokta.y>-quadH){
				sagMouseDown = 1;
				kareMerkezi = yeniNokta;
				noktalariBul();
			}
			break;

		case GLUT_UP:
			sagMouseDown = 0;
			break;
		}
		break;
	}
	glutPostRedisplay();
}

/*
OpenGl hareket fonksiyonu. 
@param x : X koordinatý
@param y : Y koordinatý
*/
static void motion(int x, int y)
{
	point yeniNokta(x*pixToXCoord + graphXMin,
		-y*pixToYCoord + graphYMax);

	if (solMouseDown) {
		if (yeniNokta.x > -quadW && yeniNokta.x < quadW && yeniNokta.y < quadH && yeniNokta.y>-quadH) {
			tumNoktalar.push_back(yeniNokta);
			qtree->ekle(yeniNokta, 1);
		}
	}
	else if (sagMouseDown) {
		if (yeniNokta.x > -quadW && yeniNokta.x < quadW && yeniNokta.y < quadH && yeniNokta.y>-quadH) {
			kareMerkezi = yeniNokta;
			noktalariBul();
	     }
	}

	glutPostRedisplay();
}

/*
Menu'de basýlan butonlarý tespit edip iþlem yapar
@param num : Basýlan buton
*/
void menu(int num) {
	switch (num)
	{
	case 0:
		for (int i = 0; i < 20; ++i) {
			point yeniNokta(eksen.x - (2 * eksen.x * randomNokta()),
				eksen.y - (2 * eksen.y * randomNokta()));
			tumNoktalar.push_back(yeniNokta);
			qtree->ekle(yeniNokta, 1);
		}
		break;
	case 1:
		delete qtree;
		qtree = new QuadTree <int>(merkez, eksen, kumeBoyutu);
		tumNoktalar.clear();
		bulunanNoktalar.clear();
		break;
	case 2:
		if (circleR < 256) {
			circleR += 16;
			kareGenisligi = { circleR,circleR };
			noktalariBul();
		}
		break;
	case 3:
		if (circleR > 32) {
			circleR -= 16;
			kareGenisligi = { circleR,circleR };
			noktalariBul();
		}
		break;
	case 4:
		bulunanNoktalariYaz();
		break;
	default:
		break;
	}
	glutPostRedisplay();
}

/*
Menu oluþturur
*/
void menuOlustur() {
	glutCreateMenu(menu);
	glutAddMenuEntry("Random Deger Ata", 0);
	glutAddMenuEntry("Ekrani Temizle", 1);
	glutAddMenuEntry("Arama çemberi yaricapi arttir", 2);
	glutAddMenuEntry("Arama çemberi yaricapi azalt", 3);
	glutAddMenuEntry("Bulunan noktalari yaz", 4);
	glutAttachMenu(GLUT_MIDDLE_BUTTON);
}


int main(int argc, char *argv[])
{
	srand(time(0));
	qtree = new QuadTree <int>(merkez, eksen, 1);
	glutInit(&argc, argv);
	glutInitWindowSize(pencereGenisligi, pencereUzunlugu);
	glutInitWindowPosition(10, 10);
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
	glutCreateWindow("QuadTree ProLab Mert Eroglu");
	glutReshapeFunc(resize);
	menuOlustur();
	glutDisplayFunc(display);
	glutKeyboardFunc(key);
	glutMouseFunc(mouse);
	glutMotionFunc(motion);
	glClearColor(0, 0, 0, 0);
	for (int i = 0; i < 33; i++) {
		zoom(graphXRange / ZOOM_P, graphYRange / ZOOM_P);
	}
	glutMainLoop();

	
	
	return EXIT_SUCCESS;
}