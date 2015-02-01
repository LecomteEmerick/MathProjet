#include <iostream>
#include <GL\glut.h>
#include <GL\gl.h>
#include "Point.h"
#include "Polygon.h"
#include "Fenetrage.h"
#include <vector>
#include <string>


#define dMode_Poly 0
#define dMode_Fenetre 1

void mouseClicks(int button, int state, int x, int y);
void affichage();
void createMenu();
void elementSelected(int i);
void newPolygon(bool isFenetrage);
void colorMenu(int index);
void customColor();
void changeColor(float red, float green, float blue);
void clearAll(int index);
void clearOnePoly(int indexPoly);
void createClearPolySubMenu(int parentMenu);
void init();
Color currentColor;

int drawingMode;
int polySubMenu=-1;
int clearSubMenu;
Polygon* currentPoly;
Polygon* currentFenetrage;
std::vector<Polygon*> polygonArray;
std::vector<Point> pointArray;

int main(int argc, char **argv)
{
	glutInit(&argc, argv);                       // Initialisation
	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE);
	glutInitWindowSize(500, 500);                // dimension fen�tre
	glutInitWindowPosition(100, 100);           // position coin haut gauche
	glutCreateWindow("Math projet");  // nom
	//
	createMenu();
	glutMouseFunc(mouseClicks);
	glutDisplayFunc(affichage);
	//
	init();
	glutMainLoop();
	return 0;
}

void mouseClicks(int button, int state, int x, int y) {
	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
		y = 500 - y;
		Point p = Point(x, y, currentColor);
		if (drawingMode == dMode_Poly)
		{
			currentPoly->addPoint(p);
			pointArray.push_back(p);
		}
		else if (drawingMode == dMode_Fenetre)
		{
			currentFenetrage->addPoint(p);
			pointArray.push_back(p);
		}
		glutPostRedisplay();
	}
}

void affichage(){
	// effacement de l'image avec la couleur de fond
	glClear(GL_COLOR_BUFFER_BIT);

	glPointSize(10);
	//draw point
	glBegin(GL_POINTS);
	for (std::vector<Point>::size_type i = 0; i < pointArray.size(); i++)
	{
		Point p = pointArray[i];
		glColor3f(p.color_get().red_get(), p.color_get().green_get(), p.color_get().blue_get());
		glVertex2f(p.x_get(),p.y_get());
	}
	glEnd();

	//draw poly
	for (std::vector<Polygon>::size_type i = 0; i < polygonArray.size(); i++)
	{
		glBegin(GL_LINE_LOOP);
		std::vector<Point> polygonPoints = polygonArray[i]->points_get();
		for (std::vector<Point>::size_type j = 0; j < polygonPoints.size(); j++)
		{
			Point p = polygonPoints[j];
			glColor3f(p.color_get().red_get(), p.color_get().green_get(), p.color_get().blue_get());
			glVertex2f(p.x_get(), p.y_get());
		}
		glEnd();
	}

	//drawFenetre
	glBegin(GL_LINE_LOOP);
	std::vector<Point> polygonPoints = currentFenetrage->points_get();
	for (std::vector<Point>::size_type j = 0; j < polygonPoints.size(); j++)
	{
		Point p = polygonPoints[j];
		glColor3f(p.color_get().red_get(), p.color_get().green_get(), p.color_get().blue_get());
		glVertex2f(p.x_get(), p.y_get());
	}
	glEnd();
	

	glutSwapBuffers();

	// On foce l'affichage du r�sultat
	glFlush();
}

void createMenu()
{
	int menuIndex = glutCreateMenu(elementSelected);
	//couleur menu
	int colorSubmenu = glutCreateMenu(colorMenu);
	glutAddMenuEntry("Rouge", 0);
	glutAddMenuEntry("Vert", 1);
	glutAddMenuEntry("Bleu", 2);
	glutAddMenuEntry("Personnalise", 3); //a implementer plus tard
	//clear menu
	clearSubMenu = glutCreateMenu(clearAll);
	createClearPolySubMenu(clearSubMenu);
	glutAddMenuEntry("Effacer tout", 1);
	//menu principal
	glutSetMenu(menuIndex);
	glutAddSubMenu("Couleurs", colorSubmenu);
	glutAddMenuEntry("Polygone", 1);
	glutAddMenuEntry("Fenetre", 2);
	glutAddMenuEntry("Fenetrage", 3);
	glutAddMenuEntry("Remplissage", 4);
	glutAddSubMenu("Effacer : ", clearSubMenu);
	glutAttachMenu(GLUT_RIGHT_BUTTON);
}

void elementSelected(int index)
{
	switch (index)
	{
		case 1 :
			newPolygon(false);
			break;
		case 2 :
			newPolygon(true);
			break;
		case 3 :
			Fenetrage().Sutherland_Hodgman();
			//std::cout << "fenetrage" << std::endl;
			break;
		case 4 :
			std::cout << "remplissage" << std::endl;
			break;
		case 5:
			clearAll(0);
			break;
		default:
			std::cout << "index non reconnu" << std::endl;
			break;
	}
}

void newPolygon(bool isFenetrage)
{
	if (!isFenetrage)
	{
		drawingMode = dMode_Poly;
		currentPoly = new Polygon();
		polygonArray.push_back(currentPoly);
		createClearPolySubMenu(clearSubMenu);
	}
	else{
		drawingMode = dMode_Fenetre;
		currentFenetrage = new Polygon();
	}
}

void colorMenu(int index)
{
	switch (index)
	{
		case 0 :
			changeColor(1.0, 0.0, 0.0);
			break;
		case 1:
			changeColor(0.0, 1.0, 0.0);
			break;
		case 2:
			changeColor(0.0, 0.0, 1.0);
			break;
		case 3:
			customColor();
			break;
		default:
			std::cout << "couleur non repertorie." << std::endl;
	}
}

void customColor()
{
	float redValue, greenValue, blueValue;
	std::cout << "Valeur de rouge : (0.0 a 1.0)" << std::endl;
	std::cin >> redValue;
	if (redValue < 0.0 || redValue > 1.0)
	{
		std::cout << "Erreur de valeur" << std::endl;
		return;
	}
	std::cout << "Valeur de vert : (0.0 a 1.0)" << std::endl;
	std::cin >> greenValue;
	if (greenValue < 0.0 || greenValue > 1.0)
	{
		std::cout << "Erreur de valeur" << std::endl;
		return;
	}
	std::cout << "Valeur de blue : (0.0 a 1.0)" << std::endl;
	std::cin >> blueValue;
	if (blueValue < 0.0 || blueValue > 1.0)
	{
		std::cout << "Erreur de valeur" << std::endl;
		return;
	}
	changeColor(redValue, greenValue, blueValue);
}

void changeColor(float red,float green, float blue)
{
	currentColor = Color(red, green, blue);
}

void clearAll(int index)
{
	bool isFenetrage = false;
	pointArray.clear();
	polygonArray.clear();
	glutPostRedisplay();
	if (drawingMode == dMode_Fenetre)
	{
		isFenetrage = true;
	}
	newPolygon(isFenetrage);
}

void clearOnePoly(int indexPoly)
{
	std::cout << "clear poly number : " << indexPoly << std::endl;
	std::vector<Point> polygonPoints = polygonArray[indexPoly]->points_get();
	for (std::vector<Point>::size_type j = 0; j < polygonPoints.size(); j++)
	{
		Point p = polygonPoints[j];
		for (std::vector<Point>::size_type i = 0; i < pointArray.size(); i++)
		{
			if (pointArray[i] == p)
			{
				pointArray.erase(pointArray.begin() + i);
			}
		}
	}
	polygonArray.erase(polygonArray.begin() + indexPoly);
	bool isFenetrage = false;
	if (drawingMode == dMode_Fenetre)
		isFenetrage = true;
	newPolygon(isFenetrage);
	createClearPolySubMenu(clearSubMenu);
	glutPostRedisplay();
}

void createClearPolySubMenu(int parentMenu)
{
	int polygonMenu = polySubMenu;
	std::cout << "enter create Menu" << std::endl;
	if (polygonMenu != -1)
	{
		glutDestroyMenu(polygonMenu);
	}
	polygonMenu = glutCreateMenu(clearOnePoly);

	for (std::vector<Polygon>::size_type i = 0; i < polygonArray.size(); i++)
	{
		glutAddMenuEntry(std::string("Polygone ").append(std::to_string(i)).c_str(),  i);
	}
	glutSetMenu(parentMenu);
	glutChangeToSubMenu(polySubMenu, "Effacer Poly", polygonMenu);
	polySubMenu = polygonMenu;
}

void init()
{
	glClearColor(1.0, 1.0, 1.0, 1.0);
	glClear(GL_COLOR_BUFFER_BIT);

	glViewport(0, 0, 500, 500);
	glMatrixMode(GL_PROJECTION);
	gluOrtho2D(0.0, 500.0, 0.0, 500.0);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	drawingMode = dMode_Poly;
	newPolygon(false);
	currentFenetrage = new Polygon();
	currentColor = Color(1.0, 0.0, 0.0);
}