#include <iostream>
#include "GLFW/glfw3.h"
#include <cmath>
#include <ctime>
#include <vector>
#include <omp.h>
//программа с муравьями, которые ищут еду
//мурашка:
//1. найти еду
//2. отнести ее домой
//3. ->(1.)
//bool useQtree = true;

//на будущее: 
//- сделать с улучшеным алгоритмом дерева квадрантов
//- сделать в 3D
double const PI = 3.1415926535;
int const A_num = 2500;//кол-во мурашек
float xscale, yscale;//для экрана
int const X1_num = 1;//
int const X2_num = 1;//
int const X3_num = 1;
int const X4_num = 1;
int const X5_num = 1;//
int const X6_num = 1;//
struct Target
{
	double x, y/*,z*/, r,dir,s/*,dir2*/;
	Target()
	{
		x = y = 0;// z = 0;
		r = 0.005;
		dir = PI * (double)(rand() % 100) * 0.02;
		//dir2 = PI * (double)(rand() % 100) * 0.02;
		//if (rand() % 2 == 1)
		s = 0.001;
		//else
		//	s = 0.004;
	}
	Target(double X, double Y/*,double Z*/, double R)
	{
		x = X;
		y = Y;
		//z = Z;
		r = R;
		dir = PI * (double)(rand() % 100) * 0.02;
		//dir2 = PI * (double)(rand() % 100) * 0.02;
		
		//if (rand() % 2 == 1)
		s = 0.001;
		//else
		//	s = 0.004;
	}
	void Move()
	{
		//dir = dir < dir + PI ? 0 : dir;
		dir = dir > 2*PI ? dir - 2*PI : dir;
		x += s * cos(dir);
		y += s * sin(dir);
		//x += s * sin(dir);
		//y += s * sin(dir);
		//z += s * cos(dir);
		dir += 0.002 * (double)(rand() % 100) - 0.1;//от -0,1 до 0,1
		if (x >= xscale || x <= -xscale)dir = PI - dir;
		if (y >= yscale || y <= -yscale)dir = -dir;
	}
};
Target x1[X1_num];
Target x2[X2_num];
Target x3[X3_num];
Target x4[X4_num];
Target x5[X5_num];
Target x6[X6_num];
int ID;
//Вариант с деревом квадрантов:
double mysin(double a)
{
	double a2 = a * a;
	double a3 = a2 * a;
	double a5 = a3 * a2;
	double a7 = a5 * a2;
	double a9 = a7 * a2;
	return a - (a3) / 6 + (a5) / 120 - (a7) / 5040 + (a9) / 362880;
}
double mycos(double a)
{
	double a2 = a * a;
	double a4 = a2 * a2;
	double a6 = a4 * a2;
	double a8 = a6 * a2;
	//double a10 = a7 * a2;
	return 1 - (a2) / 2 + (a4) / 24 - (a6) / 720 + (a8) / 40320;
}
struct Ant
{
	//int foodway, homeway;
	int const numOfWays = 6;
	double R, RStandart = 0.09,SpeedStandart = 0.005;
	bool userdata = true;
public:
	int way[6];
	//bool lookFor;//true  - ищет еду
				 //false - ищет путь домой
	int lookFor;//0 - ищет x1
				//1 - ищет x2
				//2 - ищет x3
	double x, y, /*z,*/ dir/*,dir2*/, speed;
	int id;
	//dir є[0,2PI]
	Ant()
	{
		x = y = 0;// z = 0;
		dir = PI * (double)(rand() % 100) * 0.02;
		//dir2 = PI * (double)(rand() % 100) * 0.02;
		lookFor = rand() % numOfWays;
		speed = 0.00001 * (rand() % 1000) + SpeedStandart;
		for (int i = 0; i < numOfWays; i++)
			way[i] = 0;
		//foodway = homeway = 0;
		id = ID;
		ID++;
		R = RStandart;// 0.001 * (rand() % 150) + 0.001;
		userdata = true;
	}
	Ant(double X, double Y/*,double Z*/)
	{
		x = X;
		y = Y;
		//z = Z;
		dir = PI * (double)(rand() % 100) * 0.02;
		//dir2 = PI * (double)(rand() % 100) * 0.02;
		lookFor = 0;// rand() % numOfWays;
		speed = 0.00001 * (rand() % 500) + SpeedStandart;
		for (int i = 0; i < numOfWays; i++)
			way[i] = 0;
		//foodway = homeway = 0;
		id = ID;
		ID++;
		R = RStandart;
		userdata = true;
	}
	Ant(Ant* ant)
	{
		x = ant->x;
		y = ant->y;
		//z = ant->z;
		dir = ant->dir;// PI* (double)(rand() % 100) * 0.02;
		//dir2 = ant->dir2;
		lookFor = ant->lookFor;// rand() % numOfWays;
		speed = ant->speed;// 0.000016 * (rand() % 1000) + 0.004;
		for (int i = 0; i < numOfWays; i++)
			way[i] = ant->way[i];
		id = ant->id;
		R = RStandart;
		userdata = true;
	}
	bool dokrichalsa_li(Ant suka)
	{
		return (suka.x-x)*(suka.x-x)+(suka.y-y)*(suka.y-y)<R*R;
	}
	void Go()
	{
//#pragma omp parallel 
		{
			//dir = dir < dir + PI ? 0 : dir;
			dir = dir > 2 * PI ? dir - 2 * PI : dir;
			x += speed * cos(dir);
			y += speed * sin(dir);
			//x += speed * sin(dir) * cos(dir2);
			//y += speed * sin(dir) * sin(dir2);
			//z += speed * cos(dir);
			for (int i = 0; i < numOfWays; i++)
				way[i]++;
			dir += 0.002 * (double)(rand() % 100) - 0.1;//от -0,1 до 0,1 ~ -+5градусов
			if (x >= xscale || x <= -xscale)dir = PI - dir;
			if (y >= yscale || y <= -yscale)dir = -dir;


			//Say(a);

			//for(int i=0;i<nnnnnnn?;i++)
			//{
			//	for (int j = 0; j < X[i]; j++)
			//		if (HasAnt(x[i][j], *this))
			//		{
			//			way[lookFor] = 0;
			//			lookFor = lookFor + 1 < numOfWays ? lookFor + 1 : 0;
			//			//lookFor = rand() % (numOfWays);
			//			dir += PI;
			//			//dir2 += PI;
			//			break;
			//		}
			//
			//
				

			if (lookFor == 0)
			{//ищем еду x1
				for (int i = 0; i < X1_num; i++)
					if (HasAnt(x1[i], *this))
					{
						way[lookFor] = 0;
						lookFor = lookFor + 1 < numOfWays ? lookFor + 1 : 0;
						//lookFor = rand() % (numOfWays);
						dir += PI;
						//dir2 += PI;
					}
			}
			else if (lookFor == 1)
			{//ищем дом x2
				for (int i = 0; i < X2_num; i++)
					if (HasAnt(x2[i], *this))
					{
						way[lookFor] = 0;
						lookFor = lookFor + 1 < numOfWays ? lookFor + 1 : 0;
						//lookFor = 0;
						//lookFor = rand() % (numOfWays);
						dir += PI;
						//dir2 += PI;
					}
			}
			else if (lookFor == 2)
			{
				for (int i = 0; i < X3_num; i++)
					if (HasAnt(x3[i], *this))
					{
						way[lookFor] = 0;
						lookFor = lookFor + 1 < numOfWays ? lookFor + 1 : 0;
						//lookFor = rand() % (numOfWays);
						dir += PI;
						//dir2 += PI;
					}
			}
			else if (lookFor == 3)
			{
				for (int i = 0; i < X4_num; i++)
					if (HasAnt(x4[i], *this))
					{
						way[lookFor] = 0;
						lookFor = lookFor + 1 < numOfWays ? lookFor + 1 : 0;
						//lookFor = rand() % (numOfWays);
						dir += PI;
						//dir2 += PI;
					}
			}
			else if (lookFor == 4)
			{
				for (int i = 0; i < X5_num; i++)
					if (HasAnt(x5[i], *this))
					{
						way[lookFor] = 0;
						lookFor = lookFor + 1 < numOfWays ? lookFor + 1 : 0;
						//lookFor = rand() % (numOfWays);
						dir += PI;
						//dir2 += PI;
					}
			}
			else if (lookFor == 5)
			{
				for (int i = 0; i < X6_num; i++)
					if (HasAnt(x6[i], *this))
					{
						way[lookFor] = 0;
						//lookFor = lookFor + 1 < numOfWays ? lookFor + 1 : 0;
						lookFor = 0;
						//lookFor = rand() % (numOfWays);
						dir += PI;
						//dir2 += PI;
					}
			}
			/*if (dir2 < 0)
				dir2 += 2 * PI;
			else if (dir2 > 2 * PI)
				dir2 -= 2 * PI;*/
		}
	}
	/*
	void Say(Ant* a[])
	{
		Circle range = Circle(x,y,R);
		std::vector<Ant>ants = qt->query(range);
		for (Ant other : ants)
		{
			if ((x - other.x) * (x - other.x) + (y - other.y) * (y - other.y) < R * R &&
				(x - other.x) * (x - other.x) + (y - other.y) * (y - other.y) != 0)
			{//говорим "тут еда и дом недалеко"
				for (int t = 0; t < numOfWays; t++)
				{
					if (other.way[t] > way[t] + 10 && other.lookFor == t)
					{
						other.way[t] = way[t] + 10;
						//меняем направление чел-а
						if (y > other.y)
							if (x < other.x)
								other.dir = PI + atan(abs(y - other.y) / abs(x - other.x));
							else
								other.dir = 2 * PI + atan(-1 * abs(y - other.y) / abs(x - other.x));
						else
						{
							if (x > other.x)
								other.dir = atan(abs(y - other.y) / abs(x - other.x));
							else
								other.dir = PI + atan(-1 * abs(y - other.y) / abs(x - other.x));
						}
						other.dir = -other.dir;
						glBegin(GL_LINES);
						if (t == 0)
							glColor4d(0, 1, 0, 0.1);
						else if (t == 1)
							glColor4d(1, 0, 0, 0.1);
						else if (t == 2)
							glColor4d(0, 0, 1, 0.1);
						else
							glColor4d(1, 1, 0, 0.1);

						glVertex2d(x, y);
						glVertex2d(other.x, other.y);
						glEnd();
					}
					else if (way[t] > other.way[t] + 10 && lookFor == t)
					{
						way[t] = other.way[t] + 10;
						//меняем направление чел-а
						if (other.y > y)
							if (other.x < x)
								dir = PI + atan(abs(other.y - y) / abs(other.x - x));
							else
								dir = 2 * PI + atan(-1 * abs(other.y - y) / abs(other.x - x));
						else
						{
							if (other.x > x)
								dir = atan(abs(other.y - y) / abs(other.x - x));
							else
								dir = PI + atan(-1 * abs(other.y - y) / abs(other.x - x));
						}
						dir = -dir;
						glBegin(GL_LINES);
						if (t == 0)
							glColor4d(0, 1, 0, 0.1);
						else if (t == 1)
							glColor4d(1, 0, 0, 0.1);
						else if (t == 2)
							glColor4d(0, 0, 1, 0.1);
						else
							glColor4d(1, 1, 0, 0.1);

						glVertex2d(other.x, other.y);
						glVertex2d(x, y);
						glEnd();
					}
				}
			}
		}
	}
	*/
	bool HasAnt(Target f, Ant ant)
	{
		return (f.x - ant.x) * (f.x - ant.x) + (f.y - ant.y) * (f.y - ant.y) /*+ (f.z - ant.z) * (f.z - ant.z)*/ < f.r * f.r;
	}
};
Ant* a[A_num];
struct Square
{
public:
	double a, x, y, ahalf;
	Square()
	{
		x = y = 0;
		a = 1;
		ahalf = 0.5;
	}
	Square(double X, double Y, double A, double height)
	{
		a = A;
		x = X;
		y = Y;
		ahalf = a / 2;
	}
	bool containsPoint(Ant pnt)
	{
		if (pnt.x <= x + a && pnt.x >= x && pnt.y <= y + a && pnt.y >= y)
			return true;
		else
			return false;
	}
	bool intersectsRectangle(Square other)
	{//пересекает ли данный прямоугольник другой
		return (x - other.x < other.a || other.x - x < a) && (y - other.y < other.a || other.y - y < a);
	}
};
struct Circle
{
public:
	double x, y, r;
	Circle()
	{
		x = y = 0;
		r = 1;
	}
	Circle(double X, double Y, double R)
	{
		x = X;
		y = Y;
		r = R;
	}
	bool contains(Ant p)
	{
		return pow((x - p.x),2) + pow((y - p.y),2) <= r * r;
	}
};
bool intersects(Circle c,Square R)
{
	double xDist = abs(R.x+R.ahalf-c.x);
	double yDist = abs(R.y+R.ahalf-c.y);
	double r = c.r;// radius of the circle
	double a = R.ahalf;
	double edges = pow((xDist - a), 2) + pow((yDist - a), 2);
	// no intersection
	if (xDist > (r + a) || yDist > (r + a))
		return false;
	// intersection within the circle
	if (xDist <= a || yDist <= a)
		return true;
	// intersection on the edge of the circle
	return edges <= r*r;
}
//qtree первая версия, работает поиск медленно
class QuadTree
{
public:
	const int capacity = 10;//допустимое количество точек в одном квадранте
	int PNum = 0;
	Square boundary;
	bool divided;
	std::vector<Ant> points;
	//Ant* points[3];//маассив точек, которые способен хранить 1 квадрант
	//но так как у нас всего одна точка, то
	//Ant point;
	//bool has_point = false;
	//int w, h, x, y;
	QuadTree *northWest;
	QuadTree *northEast;
	QuadTree *southWest;
	QuadTree *southEast;
	QuadTree() {}
	QuadTree(Square bndr,int Capacity)
	{
		boundary = bndr;
		//points = new Ant[capacity];
		//capacity = Capacity;
		PNum = 0;
		divided = false;
	}
	QuadTree(Square bndr)
	{
		boundary = bndr;
		//capacity = 20;
		PNum = 0;
		divided = false;
	}
	void refresh()
	{
		PNum = 0;
		divided = false;
	}
	bool insert(Ant pnt)
	{
		if (!boundary.containsPoint(pnt))
			return false;

		if (PNum < capacity)
		{
			points.push_back(pnt);
			PNum++;
			return true;
		}

		if(divided)
			return (northWest->insert(pnt) ||
				northEast->insert(pnt) ||
				southWest->insert(pnt) ||
				southEast->insert(pnt));

		if (!divided)
			subdivide();//передаем точки в дочерние квадранты
		return (northWest->insert(pnt) ||
			northEast->insert(pnt) ||
			southWest->insert(pnt) ||
			southEast->insert(pnt));
	}
	void subdivide()
	{//разделить квадрант на подквадранты
		northWest = new QuadTree(Square(boundary.x, boundary.y, boundary.ahalf, boundary.ahalf));
		northEast = new QuadTree(Square(boundary.x + (boundary.ahalf), boundary.y, boundary.ahalf, boundary.ahalf));
		southWest = new QuadTree(Square(boundary.x, boundary.y + (boundary.ahalf), boundary.ahalf, boundary.ahalf));
		southEast = new QuadTree(Square(boundary.x + (boundary.ahalf), boundary.y + (boundary.ahalf), boundary.ahalf, boundary.ahalf));
		//int n = points.size();
		//for (int i = 0; i < n; i++)
		//{
		//	northWest->insert(points[i]) ||
		//		northEast->insert(points[i]) ||
		//		southWest->insert(points[i]) ||
		//		southEast->insert(points[i]);
		//}
		divided = true;
	}
	void query(Circle range,std::vector <Ant>*found)
	{//поиск точек, входящих в некоторое поле "range"
		if (!intersects(range, boundary))//если круг поиска не входит в квадрант 
			return;//то мы выходим из функции
		
		//если у этого квадранта есть подквадранты
		//то проверяем и их
		for (int i = 0; i < points.size(); i++) //если же входит, то просматриваем,
		{// входят ли точки, которые в квадранте в круг
			if (range.contains(points[i]))
				found->push_back(points[i]);
		}
		
		if(divided)
		{
			northWest->query(range, found);
			northEast->query(range, found);
			southWest->query(range, found);
			southEast->query(range, found);
		}
	}
	void draw()
	{
		if (divided)
		{
			northWest->draw();
			northEast->draw();
			southWest->draw();
			southEast->draw();
		}
		else
		{
			glVertex2d(boundary.x, boundary.y);
			glVertex2d(boundary.x + boundary.a, boundary.y);

			glVertex2d(boundary.x + boundary.a, boundary.y);
			glVertex2d(boundary.x + boundary.a, boundary.y + boundary.a);

			glVertex2d(boundary.x + boundary.a, boundary.y + boundary.a);
			glVertex2d(boundary.x, boundary.y + boundary.a);

			glVertex2d(boundary.x, boundary.y + boundary.a);
			glVertex2d(boundary.x, boundary.y);
		}
	}
};
void SuperFunction(int i)
{
	a[i]->Go();

	//if (a[i]->lookFor == 0)
	//	glColor4d(0, 1, 0, 0.5);
	//else if (a[i]->lookFor == 1)
	//	glColor4d(1, 0, 0, 0.5);
	//else if (a[i]->lookFor == 2)
	//	glColor4d(0, 0, 1, 0.5);
	//else if (a[i]->lookFor == 3)
	//	glColor4d(1, 1, 0, 0.5);
	//else if (a[i]->lookFor == 4)
	//	glColor4d(1, 0, 1, 0.5);
	//else
	//	glColor4d(0, 1, 1, 0.5);
	//
	//glVertex2d(a[i]->x, a[i]->y);
}
int main()
{
	if (!glfwInit())
		return -1;
	int w = 1920, h = 1080;
	const GLFWvidmode* mode = glfwGetVideoMode(glfwGetPrimaryMonitor());
	w = mode->width;
	h = mode->height;

	GLFWwindow* window = glfwCreateWindow(w, h, "", glfwGetPrimaryMonitor() , NULL);
	glfwMakeContextCurrent(window);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE);
	if (w > h)
		glScalef((float)h / (float)w, 1.0f, 1.0f);
	else if (h > w)
		glScalef(1.0f, (float)w / (float)h, 1.0f);
	
	
	glfwGetWindowContentScale(window, &xscale, &yscale);
	srand(time(0));

	QuadTree* qt = new QuadTree(Square(-1, -1, 2, 2));
	ID = 0;
	for (int i = 0; i < A_num; i++)
	{
		a[i] = new Ant(0.002 * (double)(rand() % 100) - 0.1, 0.002 * (double)(rand() % 100) - 0.1/*, 0.002 * (double)(rand() % 100) - 0.1*/);
		//qt->insert(a[i]);
	}
	//Hm = Home(-0.9, -0.9, 0.01);
	//Fd = Target(0.9, 0.9, 0.01);
	for (int i = 0; i < X1_num; i++)
		x1[i] = Target(0.02 * (double)(rand() % 100) - 1, 0.02 * (double)(rand() % 100) - 1/*, 0.02 * (double)(rand() % 100) - 1*/, 0.015);
	for (int i = 0; i < X2_num; i++)
		x2[i] = Target(0.02 * (double)(rand() % 100) - 1, 0.02 * (double)(rand() % 100) - 1/*, 0.02 * (double)(rand() % 100) - 1*/, 0.015);
	for (int i = 0; i < X3_num; i++)
		x3[i] = Target(0.02 * (double)(rand() % 100) - 1, 0.02 * (double)(rand() % 100) - 1/*, 0.02 * (double)(rand() % 100) - 1*/, 0.015);
	for (int i = 0; i < X4_num; i++)
		x4[i] = Target(0.02 * (double)(rand() % 100) - 1, 0.02 * (double)(rand() % 100) - 1/*, 0.02 * (double)(rand() % 100) - 1*/, 0.015);
	for (int i = 0; i < X5_num; i++)
		x5[i] = Target(0.02 * (double)(rand() % 100) - 1, 0.02 * (double)(rand() % 100) - 1/*, 0.02 * (double)(rand() % 100) - 1*/, 0.015);
	for (int i = 0; i < X6_num; i++)
		x6[i] = Target(0.02 * (double)(rand() % 100) - 1, 0.02 * (double)(rand() % 100) - 1/*, 0.02 * (double)(rand() % 100) - 1*/, 0.015);
	std::vector<Ant> xxx = std::vector<Ant>();
	Circle range;
	double msX, msY;
	//float skale = 1;
	//if (w > h)
	//	glScalef((float)h / (float)w, 1.0f, 1.0f);
	//else if (h > w)
	//	glScalef(1.0f, (float)w / (float)h, 1.0f);
	//glScalef(1 / skale, 1 / skale, 1.0f);
	while (!glfwWindowShouldClose(window))
	{
		glClear(GL_COLOR_BUFFER_BIT);
		glClearColor(0, 0, 0, 0);
		//отрисовка и движение целей
		for (int i = 0; i < X1_num; i++)
		{
			x1[i].Move();
			glBegin(GL_POLYGON);
			glColor3d(0, 1, 0);//x1
			for (double j = 0; j < 2 * PI; j += PI / 10)
				glVertex2d(x1[i].x + x1[i].r * cos(j), x1[i].y + x1[i].r * sin(j));
			glEnd();
		}
		/*glfwGetCursorPos(window, &msX, &msY);
		if (w > h)
		{
			msX = (msX * 2 * skale / (double)h) - skale * ((double)w / (double)h);
			msY = ((-msY) * 2 * skale / (double)h + skale);
		}
		else
		{
			msX = (msX * 2 * skale / (double)w) - skale;
			msY = ((-msY) * 2 * skale / (double)w) + skale * (double)h / (double)w;
		}
		x2[0].x = msX;
		x2[0].y = msY;
		glBegin(GL_POLYGON);
		glColor3d(1, 0, 0);//x2
		for (double j = 0; j < 2 * PI; j += PI / 10)
			glVertex2d(x2[0].x + x2[0].r * cos(j), x2[0].y + x2[0].r * sin(j));
		glEnd();*/
		for (int i = 0; i < X2_num; i++)
		{
			x2[i].Move();
			glBegin(GL_POLYGON);
			glColor3d(1, 0, 0);//x2
			for (double j = 0; j < 2 * PI; j += PI / 10)
				glVertex2d(x2[i].x + x2[i].r * cos(j), x2[i].y + x2[i].r * sin(j));
			glEnd();
		}
		for (int i = 0; i < X3_num; i++)
		{
			x3[i].Move();
			glBegin(GL_POLYGON);
			glColor3d(0, 0, 1);//x3
			for (double j = 0; j < 2 * PI; j += PI / 10)
				glVertex2d(x3[i].x + x3[i].r * cos(j), x3[i].y + x3[i].r * sin(j));
			glEnd();
		}
		for (int i = 0; i < X4_num; i++)
		{
			x4[i].Move();
			glBegin(GL_POLYGON);
			glColor3d(1, 1, 0);//x4
			for (double j = 0; j < 2 * PI; j += PI / 10)
				glVertex2d(x4[i].x + x4[i].r * cos(j), x4[i].y + x4[i].r * sin(j));
			glEnd();
		}
		for (int i = 0; i < X5_num; i++)
		{
			x5[i].Move();
			glBegin(GL_POLYGON);
			glColor3d(1, 0, 1);//x5
			for (double j = 0; j < 2 * PI; j += PI / 10)
				glVertex2d(x5[i].x + x5[i].r * cos(j), x5[i].y + x5[i].r * sin(j));
			glEnd();
		}
		for (int i = 0; i < X6_num; i++)
		{
			x6[i].Move();
			glBegin(GL_POLYGON);
			glColor3d(0, 1, 1);//x6
			for (double j = 0; j < 2 * PI; j += PI / 10)
				glVertex2d(x6[i].x + x6[i].r * cos(j), x6[i].y + x6[i].r * sin(j));
			glEnd();
		}
		//qt = new QuadTree(Square(-1, -1, 2, 2));
		//qt->refresh();

		//qt->boundary = Square(-1, -1, 2, 2);
		//qt->divided = false;
		//qt->points = std::vector<Ant>();
		//qt->PNum = 0;
		
		//отрисовка и движение мурашек
		
		//glBegin(GL_POINTS);
//#pragma omp parallel shared(a)
		{
			for (int i = 0; i < A_num; i++)
			{
				//SuperFunction(i);
				
				a[i]->Go();
				//Ant aaa = Ant(a[i]);
				//qt->insert(a[i]);
				
				//if (a[i]->lookFor == 0)
				//	glColor4d(0, 1, 0,0.5);
				//else if (a[i]->lookFor == 1)
				//	glColor4d(1, 0, 0,0.5);
				//else if (a[i]->lookFor == 2)
				//	glColor4d(0, 0, 1,0.5);
				//else if (a[i]->lookFor == 3)
				//	glColor4d(1, 1, 0,0.5);
				//else if (a[i]->lookFor == 4)
				//	glColor4d(1, 0, 1,0.5);
				//else
				//	glColor4d(0, 1, 1,0.5);
				
				//std::cout << a[i]->dir<<"\n";
				//if (a[i].lookFor)
				//	glColor3d(1, 0, 0);
				//else
				//	glColor3d(0, 1, 0);

					//glVertex3d(a[i]->x, a[i]->y,a[i]->z);

					//glVertex2d(a[i]->x, a[i]->y);
					
			}
		}
		//glEnd();

		//glBegin(GL_LINES);
		//glColor4d(1, 1, 1,0.2);
		//qt->draw();
		//glEnd();
		/*for (int i = 0; i < A_num; i++)
			if (a[i]->way[0] == 0 || a[i]->way[1] == 0 || a[i]->way[2] == 0 || a[i]->way[3] == 0)
			{
				for (int j = i + 1; j < A_num; j++)
					for (int t = 0; t < a[i]->numOfWays; t++)
						if (a[j]->way[t] > a[i]->way[t] + 10 && a[j]->lookFor == t)
							if ((a[i]->x - a[j]->x) * (a[i]->x - a[j]->x) + (a[i]->y - a[j]->y) * (a[i]->y - a[j]->y) < a[i]->R * a[i]->R)
							{
								a[j]->way[t] = a[i]->way[t] + 10;
								//меняем направление чел-а
								if (a[i]->y > a[j]->y)
									if (a[i]->x < a[j]->x)
										a[j]->dir = PI + atan(abs(a[i]->y - a[j]->y) / abs(a[i]->x - a[j]->x));
									else
										a[j]->dir = 2 * PI + atan(-1 * abs(a[i]->y - a[j]->y) / abs(a[i]->x - a[j]->x));
								else
								{
									if (a[i]->x > a[j]->x)
										a[j]->dir = atan(abs(a[i]->y - a[j]->y) / abs(a[i]->x - a[j]->x));
									else
										a[j]->dir = PI + atan(-1 * abs(a[i]->y - a[j]->y) / abs(a[i]->x - a[j]->x));
								}
								a[j]->dir = -a[j]->dir;
								glBegin(GL_LINES);
								if (t == 0)
									glColor4d(0, 1, 0, 0.1);
								else if (t == 1)
									glColor4d(1, 0, 0, 0.1);
								else if (t == 2)
									glColor4d(0, 0, 1, 0.1);
								else
									glColor4d(1, 1, 0, 0.1);

								glVertex2d(a[i]->x, a[i]->y);
								glVertex2d(a[j]->x, a[j]->y);
								glEnd();
								//glBegin(GL_LINES);
								//glColor4d(0, 1, 0, 0.1);
								//glVertex2d(a[i]->x, a[i]->y);
								//glVertex2d(a[j]->x, a[j]->y);
								//glEnd();
							}
							else if (a[i]->way[t] > a[j]->way[t] + 10 && a[i]->lookFor == t)
							{
								a[i]->way[t] = a[j]->way[t] + 10;
								//меняем направление чел-а
								if (a[j]->y > a[i]->y)
									if (a[j]->x < a[i]->x)
										a[i]->dir = PI + atan(abs(a[j]->y - a[i]->y) / abs(a[j]->x - a[i]->x));
									else
										a[i]->dir = 2 * PI + atan(-1 * abs(a[j]->y - a[i]->y) / abs(a[j]->x - a[i]->x));
								else
								{
									if (a[j]->x > a[i]->x)
										a[i]->dir = atan(abs(a[j]->y - a[i]->y) / abs(a[j]->x - a[i]->x));
									else
										a[i]->dir = PI + atan(-1 * abs(a[j]->y - a[i]->y) / abs(a[j]->x - a[i]->x));
								}
								a[i]->dir = -a[i]->dir;
								glBegin(GL_LINES);
								if (t == 0)
									glColor4d(0, 1, 0, 0.1);
								else if (t == 1)
									glColor4d(1, 0, 0, 0.1);
								else if (t == 2)
									glColor4d(0, 0, 1, 0.1);
								else
									glColor4d(1, 1, 0, 0.1);

								glVertex2d(a[j]->x, a[j]->y);
								glVertex2d(a[i]->x, a[i]->y);
								glEnd();
								//glBegin(GL_LINES);
								//glColor4d(0, 1, 0, 0.1);
								//glVertex2d(a[i]->x, a[i]->y);
								//glVertex2d(a[j]->x, a[j]->y);
								//glEnd();
							}
			}
		*/
		//поиск ближайших
		if (false)
			for (int i = 0; i < A_num; i++)
			{//qtree
				xxx.clear();
				range = Circle(a[i]->x, a[i]->y, a[i]->R);
				qt->query(range, &xxx);//нашли точки, лежащие в области
				for (int j = 0; j < xxx.size(); j++)
				{//выводим соединение между текущей точкой и точками, лежащими в области
					Ant* other = &xxx[j];
					Ant* thiss = a[i];
					if (other->id != thiss->id)
					{
						//glBegin(GL_LINES);
						//glColor4d(1, 0, 0,1);
						//glVertex2d(a[i]->x, a[i]->y);
						//glVertex2d(xxx[j].x, xxx[j].y);
						//glEnd();
						for (int t = 0; t < thiss->numOfWays; t++)
						{
							if (other->way[t] > thiss->way[t] + 10 && other->lookFor == t)
							{
								xxx[j].way[t] = thiss->way[t] + 10;
								//меняем направление чел-а
								if (thiss->y > other->y)
									if (thiss->x < other->x)
										other->dir = PI + atan(abs(thiss->y - other->y) / abs(thiss->x - other->x));
									else
										other->dir = 2 * PI + atan(-1 * abs(thiss->y - other->y) / abs(thiss->x - other->x));
								else
								{
									if (thiss->x > other->x)
										other->dir = atan(abs(thiss->y - other->y) / abs(thiss->x - other->x));
									else
										other->dir = PI + atan(-1 * abs(thiss->y - other->y) / abs(thiss->x - other->x));
								}
								other->dir = -other->dir;
								glBegin(GL_LINES);
								
								if (t == 0)
									glColor4d(0, 1, 0, 0.2);
								else if (t == 1)
									glColor4d(1, 0, 0, 0.2);
								else if (t == 2)
									glColor4d(0, 0, 1, 0.2);
								else if (t == 3)
									glColor4d(1, 1, 0, 0.2);
								else if (t == 4)
									glColor4d(1, 0, 1, 0.2);
								else
									glColor4d(0, 1, 1, 0.2);

								glVertex2d(thiss->x, thiss->y);
								glVertex2d(other->x, other->y);
								glEnd();
								//glBegin(GL_LINES);
								//glColor4d(0, 1, 0, 0.1);
								//glVertex2d(thiss->x, thiss->y);
								//glVertex2d(xxx[j].x, xxx[j].y);
								//glEnd();
							}
							else if (thiss->way[t] > other->way[t] + 10 && thiss->lookFor == t)
							{
								thiss->way[t] = other->way[t] + 10;
								//меняем направление чел-а
								if (other->y > thiss->y)
									if (other->x < thiss->x)
										thiss->dir = PI + atan(abs(other->y - thiss->y) / abs(other->x - thiss->x));
									else
										thiss->dir = 2 * PI + atan(-1 * abs(other->y - thiss->y) / abs(other->x - thiss->x));
								else
								{
									if (other->x > thiss->x)
										thiss->dir = atan(abs(other->y - thiss->y) / abs(other->x - thiss->x));
									else
										thiss->dir = PI + atan(-1 * abs(other->y - thiss->y) / abs(other->x - thiss->x));
								}
								thiss->dir = -thiss->dir;
								glBegin(GL_LINES);
								
								if (t == 0)
									glColor4d(0, 1, 0, 0.2);
								else if (t == 1)
									glColor4d(1, 0, 0, 0.2);
								else if (t == 2)
									glColor4d(0, 0, 1, 0.2);
								else if (t == 3)
									glColor4d(1, 1, 0, 0.2);
								else if (t == 4)
									glColor4d(1, 0, 1, 0.2);
								else
									glColor4d(0, 1, 1, 0.2);


								glVertex2d(other->x, other->y);
								glVertex2d(thiss->x, thiss->y);
								glEnd();
								//glBegin(GL_LINES);
								//glColor4d(0, 1, 0, 0.1);
								//glVertex2d(a[i]->x, a[i]->y);
								//glVertex2d(xxx[j].x, xxx[j].y);
								//glEnd();
							}
						}
					}
					/*
					if (xxx[j].id != a[i]->id)
					{
						//glBegin(GL_LINES);
						//glColor4d(1, 0, 0,1);
						//glVertex2d(a[i]->x, a[i]->y);
						//glVertex2d(xxx[j].x, xxx[j].y);
						//glEnd();
						for (int t = 0; t < a[i]->numOfWays; t++)
						{
							if (xxx[j].way[t] > a[i]->way[t] + 10 && xxx[j].lookFor == t)
							{
								xxx[j].way[t] = a[i]->way[t] + 10;
								//меняем направление чел-а
								if (a[i]->y > xxx[j].y)
									if (a[i]->x < xxx[j].x)
										xxx[j].dir = PI + atan(abs(a[i]->y - xxx[j].y) / abs(a[i]->x - xxx[j].x));
									else
										xxx[j].dir = 2 * PI + atan(-1 * abs(a[i]->y - xxx[j].y) / abs(a[i]->x - xxx[j].x));
								else
								{
									if (a[i]->x > xxx[j].x)
										xxx[j].dir = atan(abs(a[i]->y - xxx[j].y) / abs(a[i]->x - xxx[j].x));
									else
										xxx[j].dir = PI + atan(-1 * abs(a[i]->y - xxx[j].y) / abs(a[i]->x - xxx[j].x));
								}
								xxx[j].dir = -xxx[j].dir;
								glBegin(GL_LINES);
								if (t == 0)
									glColor4d(0, 1, 0, 0.1);
								else if (t == 1)
									glColor4d(1, 0, 0, 0.1);
								else if (t == 2)
									glColor4d(0, 0, 1, 0.1);
								else
									glColor4d(1, 1, 0, 0.1);
		
								glVertex2d(a[i]->x, a[i]->y);
								glVertex2d(xxx[j].x, xxx[j].y);
								glEnd();
								//glBegin(GL_LINES);
								//glColor4d(0, 1, 0, 0.1);
								//glVertex2d(a[i]->x, a[i]->y);
								//glVertex2d(xxx[j].x, xxx[j].y);
								//glEnd();
							}
							else if (a[i]->way[t] > xxx[j].way[t] + 10 && a[i]->lookFor == t)
							{
								a[i]->way[t] = xxx[j].way[t] + 10;
								//меняем направление чел-а
								if (xxx[j].y > a[i]->y)
									if (xxx[j].x < a[i]->x)
										a[i]->dir = PI + atan(abs(xxx[j].y - a[i]->y) / abs(xxx[j].x - a[i]->x));
									else
										a[i]->dir = 2 * PI + atan(-1 * abs(xxx[j].y - a[i]->y) / abs(xxx[j].x - a[i]->x));
								else
								{
									if (xxx[j].x > a[i]->x)
										a[i]->dir = atan(abs(xxx[j].y - a[i]->y) / abs(xxx[j].x - a[i]->x));
									else
										a[i]->dir = PI + atan(-1 * abs(xxx[j].y - a[i]->y) / abs(xxx[j].x - a[i]->x));
								}
								a[i]->dir = -a[i]->dir;
								glBegin(GL_LINES);
								if (t == 0)
									glColor4d(0, 1, 0, 0.1);
								else if (t == 1)
									glColor4d(1, 0, 0, 0.1);
								else if (t == 2)
									glColor4d(0, 0, 1, 0.1);
								else
									glColor4d(1, 1, 0, 0.1);
		
								glVertex2d(xxx[j].x, xxx[j].y);
								glVertex2d(a[i]->x, a[i]->y);
								glEnd();
								//glBegin(GL_LINES);
								//glColor4d(0, 1, 0, 0.1);
								//glVertex2d(a[i]->x, a[i]->y);
								//glVertex2d(xxx[j].x, xxx[j].y);
								//glEnd();
							}
						}
					}
					*/
				}
			}
		else

//#pragma omp parallel shared(a)

		{
			for (int i = 0; i < A_num; i++)
			{//full
				for (int j = i + 1; j < A_num; j++)
				{
					if ((a[i]->x - a[j]->x) * (a[i]->x - a[j]->x) + (a[i]->y - a[j]->y) * (a[i]->y - a[j]->y)/*+ (a[i]->z - a[j]->z) * (a[i]->z - a[j]->z)*/ < a[i]->R * a[i]->R)
					//if(a[i]->dokrichalsa_li(a[j]))//работает очень медленно(((
					{
						//glBegin(GL_LINES);
						//glColor4d(1, 0, 0,1);
						//glVertex2d(a[i]->x, a[i]->y);
						//glVertex2d(a[j]->x, a[j]->y);
						//glEnd();
						for (int t = 0; t < a[i]->numOfWays; t++)
						{
							if (a[j]->way[t] > a[i]->way[t] + 10 && a[j]->lookFor == t)
							{
								a[j]->way[t] = a[i]->way[t] + 10;
								//меняем направление чел-а
								if (a[i]->y > a[j]->y)//----------------------------------------------------------------------
									if (a[i]->x < a[j]->x)
										a[j]->dir = PI + atan(abs(a[i]->y - a[j]->y) / abs(a[i]->x - a[j]->x));
									else
										a[j]->dir = 2 * PI + atan(-1 * abs(a[i]->y - a[j]->y) / abs(a[i]->x - a[j]->x));
								else
								{
									if (a[i]->x > a[j]->x)
										a[j]->dir = atan(abs(a[i]->y - a[j]->y) / abs(a[i]->x - a[j]->x));
									else
										a[j]->dir = PI + atan(-1 * abs(a[i]->y - a[j]->y) / abs(a[i]->x - a[j]->x));
								}
								a[j]->dir = -a[j]->dir;
								glBegin(GL_LINES);
								if (t == 0)
									glColor4d(0, 1, 0, 0.2);
								else if (t == 1)
									glColor4d(1, 0, 0, 0.2);
								else if (t == 2)
									glColor4d(0, 0, 1, 0.2);
								else if (t == 3)
									glColor4d(1, 1, 0, 0.2);
								else if (t == 4)
									glColor4d(1, 0, 1, 0.2);
								else
									glColor4d(0, 1, 1, 0.2);
								/*if (t == 0)
									glColor4d(1, 0, 0, 0.1);
								else if (t == 1)
									glColor4d(0, 0, 1, 0.1);
								else if (t == 2)
									glColor4d(0, 0, 1, 0.1);
								else if (t == 3)
									glColor4d(0, 0, 1, 0.1);
								else if (t == 4)
									glColor4d(1, 0, 0, 0.1);
								else
									glColor4d(0, 0, 1, 0.1);*/
								glVertex2d(a[i]->x, a[i]->y);
								glVertex2d(a[j]->x, a[j]->y);
								glEnd();
								//glBegin(GL_LINES);
								//glColor4d(1, 0, 0, 0.1);
								//glVertex2d(a[i]->x, a[i]->y);
								//glVertex2d(a[j]->x, a[j]->y);
								//glEnd();
							}
							else if (a[i]->way[t] > a[j]->way[t] + 10 && a[i]->lookFor == t)
							{
								a[i]->way[t] = a[j]->way[t] + 10;
								//меняем направление чел-а
								if (a[j]->y > a[i]->y)
									if (a[j]->x < a[i]->x)
										a[i]->dir = PI + atan(abs(a[j]->y - a[i]->y) / abs(a[j]->x - a[i]->x));
									else
										a[i]->dir = 2 * PI + atan(-1 * abs(a[j]->y - a[i]->y) / abs(a[j]->x - a[i]->x));
								else
								{
									if (a[j]->x > a[i]->x)
										a[i]->dir = atan(abs(a[j]->y - a[i]->y) / abs(a[j]->x - a[i]->x));
									else
										a[i]->dir = PI + atan(-1 * abs(a[j]->y - a[i]->y) / abs(a[j]->x - a[i]->x));
								}
								a[i]->dir = -a[i]->dir;
								glBegin(GL_LINES);
								if (t == 0)
									glColor4d(0, 1, 0, 0.2);
								else if (t == 1)
									glColor4d(1, 0, 0, 0.2);
								else if (t == 2)
									glColor4d(0, 0, 1, 0.2);
								else if (t == 3)
									glColor4d(1, 1, 0, 0.2);
								else if (t == 4)
									glColor4d(1, 0, 1, 0.2);
								else
									glColor4d(0, 1, 1, 0.2);
								/*if (t == 0)
									glColor4d(1, 0, 0, 0.1);
								else if (t == 1)
									glColor4d(0, 0, 1, 0.1);
								else if (t == 2)
									glColor4d(0, 0, 1, 0.1);
								else if (t == 3)
									glColor4d(0, 0, 1, 0.1);
								else if (t == 4)
									glColor4d(1, 0, 0, 0.1);
								else
									glColor4d(0, 0, 1, 0.1);*/
								glVertex2d(a[j]->x, a[j]->y);
								glVertex2d(a[i]->x, a[i]->y);
								glEnd();
								//glBegin(GL_LINES);
								//glColor4d(1, 0, 0, 0.1);
								//glVertex2d(a[i]->x, a[i]->y);
								//glVertex2d(a[j]->x, a[j]->y);
								//glEnd();
							}
						}
					}
				}
			}
		}

			/*for (int i = 0; i < A_num; i++)
			{
				//a[i]->Say(a);
				Circle range = Circle(a[i]->x, a[i]->y, a[i]->R);
				std::vector<Ant>ants = qt->query(range, xxx);
				for(int j=0;j<ants.size();j++)
				//for (Ant other : ants)
				{
					if(a[i]->id != a[ants[j].id]->id)
					if ((a[i]->x - a[ants[j].id]->x) * (a[i]->x - a[ants[j].id]->x) + (a[i]->y - a[ants[j].id]->y) * (a[i]->y - a[ants[j].id]->y) < a[i]->R * a[i]->R &&
						(a[i]->x - a[ants[j].id]->x) * (a[i]->x - a[ants[j].id]->x) + (a[i]->y - a[ants[j].id]->y) * (a[i]->y - a[ants[j].id]->y) != 0)
					{//говорим "тут еда или дом недалеко"

						glBegin(GL_LINES);
						glColor3d(1, 1, 1);
						glVertex2d(a[i]->x, a[i]->y);
						glVertex2d(a[ants[j].id]->x, a[ants[j].id]->y);
						glEnd();
						for (int t = 0; t < a[i]->numOfWays; t++)
						{
							if (a[ants[j].id]->way[t] > a[i]->way[t] + 10 && a[ants[j].id]->lookFor == t)
							{
								a[ants[j].id]->way[t] = a[i]->way[t] + 10;
								//меняем направление чел-а
								if (a[i]->y > a[ants[j].id]->y)
									if (a[i]->x < a[ants[j].id]->x)
										a[ants[j].id]->dir = PI + atan(abs(a[i]->y - a[ants[j].id]->y) / abs(a[i]->x - a[ants[j].id]->x));
									else
										a[ants[j].id]->dir = 2 * PI + atan(-1 * abs(a[i]->y - a[ants[j].id]->y) / abs(a[i]->x - a[ants[j].id]->x));
								else
								{
									if (a[i]->x > a[ants[j].id]->x)
										a[ants[j].id]->dir = atan(abs(a[i]->y - a[ants[j].id]->y) / abs(a[i]->x - a[ants[j].id]->x));
									else
										a[ants[j].id]->dir = PI + atan(-1 * abs(a[i]->y - a[ants[j].id]->y) / abs(a[i]->x - a[ants[j].id]->x));
								}
								a[ants[j].id]->dir = -a[ants[j].id]->dir;
								//glBegin(GL_LINES);
								//if (t == 0)
								//	glColor4d(0, 1, 0, 0.1);
								//else if (t == 1)
								//	glColor4d(1, 0, 0, 0.1);
								//else if (t == 2)
								//	glColor4d(0, 0, 1, 0.1);
								//else
								//	glColor4d(1, 1, 0, 0.1);

								//glVertex2d(a[i]->x, a[i]->y);
								//glVertex2d(a[ants[j].id]->x, a[ants[j].id]->y);
								//glEnd();
								glBegin(GL_LINES);
								glColor3d(1, 1, 1);
								glVertex2d(a[i]->x, a[i]->y);
								glVertex2d(a[ants[j].id]->x, a[ants[j].id]->y);
								glEnd();
							}
							else if (a[i]->way[t] > a[ants[j].id]->way[t] + 10 && a[i]->lookFor == t)
							{
								a[i]->way[t] = a[ants[j].id]->way[t] + 10;
								//меняем направление чел-а
								if (a[ants[j].id]->y > a[i]->y)
									if (a[ants[j].id]->x < a[i]->x)
										a[i]->dir = PI + atan(abs(a[ants[j].id]->y - a[i]->y) / abs(a[ants[j].id]->x - a[i]->x));
									else
										a[i]->dir = 2 * PI + atan(-1 * abs(a[ants[j].id]->y - a[i]->y) / abs(a[ants[j].id]->x - a[i]->x));
								else
								{
									if (a[ants[j].id]->x > a[i]->x)
										a[i]->dir = atan(abs(a[ants[j].id]->y - a[i]->y) / abs(a[ants[j].id]->x - a[i]->x));
									else
										a[i]->dir = PI + atan(-1 * abs(a[ants[j].id]->y - a[i]->y) / abs(a[ants[j].id]->x - a[i]->x));
								}
								a[i]->dir = -a[i]->dir;
								//glBegin(GL_LINES);
								//if (t == 0)
								//	glColor4d(0, 1, 0, 0.1);
								//else if (t == 1)
								//	glColor4d(1, 0, 0, 0.1);
								//else if (t == 2)
								//	glColor4d(0, 0, 1, 0.1);
								//else
								//	glColor4d(1, 1, 0, 0.1);

								//glVertex2d(a[ants[j].id]->x, a[ants[j].id]->y);
								//glVertex2d(a[i]->x, a[i]->y);
								//glEnd();
								glBegin(GL_LINES);
								glColor3d(1, 1, 1);
								glVertex2d(a[i]->x, a[i]->y);
								glVertex2d(a[ants[j].id]->x, a[ants[j].id]->y);
								glEnd();
							}
						}
					}
				}
			}*/
			//glRotated(0.01,1,0,0);
		glfwSwapBuffers(window);
		glfwPollEvents();
	}
}