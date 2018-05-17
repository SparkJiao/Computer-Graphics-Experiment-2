#include <iostream>
#include <GL/glut.h>
#include <GL/freeglut.h>

using namespace std;

double lookX = 0, lookY = 0, lookZ = 15;
double centerX = 0, centerY = 0, centerZ = 0;
double upX = 0, upY = 1, upZ = 0;
//double speed = 0.3;

int gamer[4] = { 4,-14,-20,0 };//x,y,z,朝向(↑0 ←1 ↓2 →3)

int maze[900];

bool inFirstSight = false;

void init() {

	glClearColor(0.8, 0.8, 0.8, 1);//用灰色清屏
	glEnable(GL_DEPTH_TEST);//开启深度测试
	glEnable(GL_TEXTURE_2D);//启用二维纹理
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(50, 1, 0.5, 100);/*其实一开始ZNear设的是1~*/
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	gluLookAt(lookX, lookY, lookZ, centerX, centerY, centerZ, upX, upY, upZ);

	glShadeModel(GL_SMOOTH);     //使用平滑明暗处理  
	glEnable(GL_DEPTH_TEST);     //剔除隐藏面  
	glEnable(GL_CULL_FACE);      //不计算多边形背面  
	glFrontFace(GL_CCW);      //多边形逆时针方向为正面

	{
		//三视图光源
		GLfloat light0_position[] = { 10.0f, 10.0f, 0.0f, 1.0f };
		GLfloat light0_ambient[] = { 0.8f, 0.8f, 0.8f, 1.0f };
		GLfloat light0_diffuse[] = { 1.0f, 1.0f, 1.0f, 1.0f };

		glLightfv(GL_LIGHT0, GL_POSITION, light0_position);
		glLightfv(GL_LIGHT0, GL_AMBIENT, light0_ambient);
		glLightfv(GL_LIGHT0, GL_DIFFUSE, light0_diffuse);

		//一视图光源
		GLfloat light1_position[] = { 10.0f, 10.0f, 0.0f, 1.0f };
		GLfloat light1_ambient[] = { 0.0f, 0.0f, 0.0f, 1.0f };
		GLfloat light1_diffuse[] = { 1.0f, 1.0f, 1.0f, 1.0f };
		GLfloat light1_specular[] = { 1.0f, 1.0f, 1.0f, 1.0f };

		glLightfv(GL_LIGHT1, GL_POSITION, light1_position);
		glEnable(GL_LIGHTING);//开启光照明
		glEnable(GL_DEPTH_TEST);
	}
}

void drawCube(double x, double y, double z) {


	glPushMatrix();/*!划重点!不写后果很严重（当然这个错误不是我犯得哈哈）*/
	glTranslatef(x, y, z);
	glColor3f(1.0f, 1.0f, 1.0f);
	glBegin(GL_QUADS);
	glNormal3f(0.0f, 1.0f, 0.0f);//法向量，光照明要用的
	glVertex3f(0.5f, 0.5f, 0.5f);
	glVertex3f(0.5f, 0.5f, -0.5f);
	glVertex3f(-0.5f, 0.5f, -0.5f);
	glVertex3f(-0.5f, 0.5f, 0.5f);
	glEnd();
	glBegin(GL_QUADS);
	glNormal3f(0.0f, -1.0f, 0.0f);
	glVertex3f(0.5f, -0.5f, 0.5f);
	glVertex3f(-0.5f, -0.5f, 0.5f);
	glVertex3f(-0.5f, -0.5f, -0.5f);
	glVertex3f(0.5f, -0.5f, -0.5f);
	glEnd();
	glBegin(GL_QUADS);
	glNormal3f(0.0f, 0.0f, 1.0f);
	glVertex3f(0.5f, 0.5f, 0.5f);
	glVertex3f(-0.5f, 0.5f, 0.5f);
	glVertex3f(-0.5f, -0.5f, 0.5f);
	glVertex3f(0.5f, -0.5f, 0.5f);
	glEnd();
	glBegin(GL_QUADS);
	glNormal3f(0.0f, 0.0f, -1.0f);
	glVertex3f(0.5f, 0.5f, -0.5f);
	glVertex3f(0.5f, -0.5f, -0.5f);
	glVertex3f(-0.5f, -0.5f, -0.5f);
	glVertex3f(-0.5f, 0.5f, -0.5f);
	glEnd();
	glBegin(GL_QUADS);
	glNormal3f(-1.0f, 0.0f, 0.0f);
	glVertex3f(-0.5f, 0.5f, 0.5f);
	glVertex3f(-0.5f, 0.5f, -0.5f);
	glVertex3f(-0.5f, -0.5f, -0.5f);
	glVertex3f(-0.5f, -0.5f, 0.5f);
	glEnd();
	glBegin(GL_QUADS);
	glNormal3f(1.0f, 0.0f, 0.0f);
	glVertex3f(0.5f, 0.5f, 0.5f);
	glVertex3f(0.5f, -0.5f, 0.5f);
	glVertex3f(0.5f, -0.5f, -0.5f);
	glVertex3f(0.5f, 0.5f, -0.5f);
	glEnd();
	glPopMatrix();/*有Push当然要有Pop*/
}

void initMaze() {
	//初始化迷宫maze[900]数组
	for (int i = 0; i < 900; i++) {
		maze[i] = 1;
	}
	//起点
	maze[19] = 0;
	//终点
	maze[871 + 20] = 0;
	for (int i = 0; i < 28; i++) {
		if (i != 10) {

			maze[i + 31] = 0;
			if (i != 25)
				maze[i + 841] = 0;
		}
	}
	maze[61 + 5] = 0;
	maze[61 + 22] = 0;
	maze[61 + 3] = 0;
	maze[61 + 13] = 0;

	maze[811 + 27] = 0;
	maze[811 + 25] = 0;
	for (int i = 0; i < 28; i++) {
		if (i != 4 && i != 15) {
			maze[i + 91] = 0;
		}
		else {
			maze[i + 811] = 0;
		}
	}
	maze[121 + 1] = 0;
	maze[121 + 18] = 0;
	for (int i = 0; i < 5; i++) {
		maze[i + 781] = 0;
		maze[i + 781 + 10] = 0;
		maze[i + 781 + 15] = 0;
		maze[i + 781 + 23] = 0;
	}
	for (int i = 0; i < 10; i++) {
		if (i != 3) {
			maze[i + 751 + 3] = 0;
			maze[i + 751 + 15] = 0;
		}
	}
	maze[721 + 3] = 0;
	maze[721 + 19] = 0;
	for (int i = 0; i < 28; i++) {
		if (i != 7)
			maze[i + 151] = 0;
	}
	for (int i = 0; i < 28; i++) {
		if (i != 7 && i != 18)
			maze[i + 691] = 0;
	}
	maze[661 + 27] = 0;
	maze[661 + 15] = 0;
	maze[661] = 0;
	maze[631] = 0;
	maze[631 + 27] = 0;
	for (int i = 0; i < 10; i++) {
		maze[631 + i + 15] = 0;
		maze[631 + i + 3] = 0;
	}
	for (int i = 0; i < 4; i++) {
		maze[601 + i] = 0;
		maze[601 + i + 10] = 0;
		maze[601 + i + 24] = 0;
	}
	for (int i = 0; i < 10; i++) {
		if (i != 4)
			maze[571 + i + 3] = 0;
		maze[571 + i + 17] = 0;
	}
	maze[541 + 1] = 0;
	maze[541 + 2] = 0;
	maze[541 + 3] = 0;
	maze[541 + 10] = 0;
	maze[541 + 18] = 0;
	maze[511 + 1] = 0;
	for (int i = 7; i < 28; i++) {
		if (i != 16)
			maze[511 + i] = 0;
	}
	for (int i = 0; i < 7; i++) {
		maze[481 + i] = 0;
		if (i != 3)
			maze[481 + i + 12] = 0;
	}
	maze[451 + 27] = 0;
	maze[481 + 27] = 0;
	maze[451 + 3] = 0;
	maze[451 + 5] = 0;
	maze[451 + 13] = 0;
	for (int i = 0; i < 5; i++) {
		maze[451 + i + 18] = 0;
	}
	for (int i = 0; i < 5; i++) {
		maze[421 + 2 + i] = 0;
		maze[421 + 10 + i] = 0;
		maze[421 + i + 23] = 0;
	}
	maze[421 + 20] = 0;
	maze[391] = 0;
	maze[391 + 1] = 0;
	maze[391 + 2] = 0;
	maze[391 + 13] = 0;
	maze[391 + 27] = 0;
	maze[361 + 27] = 0;
	maze[361 + 13] = 0;
	maze[361] = 0;
	maze[181 + 5] = 0;
	maze[181 + 10] = 0;
	maze[181 + 20] = 0;
	for (int i = 0; i < 28; i++) {
		if (i != 4 && i != 7 && i != 15)
			maze[i + 211] = 0;
	}
	maze[241] = 0;
	maze[241 + 27] = 0;
	for (int i = 3; i < 7; i++) {
		if (i != 5) {
			maze[i + 241] = 0;
			maze[i + 241 + 10] = 0;
		}

	}
	maze[271] = 0;
	maze[271 + 1] = 0;
	for (int i = 4; i < 7; i++) {
		maze[i + 271] = 0;
		maze[i + +271 + 5] = 0;
		maze[i + 271 + 12] = 0;
		maze[i + 271 + 15] = 0;
		maze[i + 271 + 18] = 0;
	}
	maze[301 + 21] = 0;
	maze[301 + 27] = 0;
	for (int i = 1; i < 5; i++) {
		maze[i + 301] = 0;
		maze[i + 301 + 12] = 0;
	}
	maze[301 + 11] = 0;
	maze[331] = 0;
	maze[331 + 1] = 0;
	for (int i = 4; i < 14; i++) {
		if (i != 10)
			maze[i + 331] = 0;
		maze[i + 331 + 14] = 0;
	}

}

void drawGamer() {

	glPushMatrix();
	glDisable(GL_TEXTURE_2D);
	glTranslatef(gamer[0], gamer[1], gamer[2]);
	glutSolidSphere(0.2, 20, 20);
	glEnable(GL_TEXTURE_2D);
	glPopMatrix();
}

void drawMaze() {
	glEnable(GL_SCISSOR_TEST);
	glScissor(0, 0, 700, 700);

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glDisable(GL_SCISSOR_TEST);

	glViewport(0, 0, 700, 700);

	glEnable(GL_LIGHT0);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(50, 1, 0.5, 100);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	gluLookAt(lookX, lookY, lookZ, centerX, centerY, centerZ, upX, upY, upZ);

	drawGamer();
	//画三维地图
	glPushMatrix();
	glTranslatef(-15, -14, 0);
	for (int i = 0; i < 900; i++) {
		if (maze[i] == 1 || maze[i] == -1) {

			drawCube(i - i / 30 * 30, i / 30, -20);
		}

	}
	glPopMatrix();
	//画地板

	glPushMatrix();
	glColor3f(1.0f, 1.0f, 1.0f);
	glBegin(GL_QUADS);
	glNormal3f(0.0f, 0.0f, 1.0f);
	glVertex3f(14, 15, -20.5);
	glVertex3f(-15, 15, -20.5);
	glVertex3f(-15, -14.9, -20.5);
	glVertex3f(14, -14.9, -20.5);
	glEnd();
	glPopMatrix();
}

void display() {

	drawMaze();
	glutSwapBuffers();

}

void changeToFirstSight() {
	lookX = gamer[0];
	lookY = gamer[1];
	lookZ = gamer[2];

	switch (gamer[3])
	{
	case 0://up
		centerX = lookX;
		centerY = lookY + 1;

		break;
	case 1://left
		centerX = lookX - 1;
		centerY = lookY;
		break;
	case 2://down
		centerX = lookX;
		centerY = lookY - 1;
		break;
	case 3://right
		centerX = lookX + 1;
		centerY = lookY;
		break;
	default:
		break;
	}
	centerZ = lookZ;

	upX = 0;
	upY = 0;
	upZ = 1;
}

void move(int key, int x, int y) {

	int gamerX = gamer[0] + 15;//19
	int gamerY = gamer[1] + 14;//0

							   //判断地图是否走过
	{
		//right
		if (maze[gamerY * 30 + (gamerX + 1)] == 1) {
			maze[gamerY * 30 + (gamerX + 1)] = -1;
		}
		//left
		if (maze[gamerY * 30 + (gamerX - 1)] == 1) {
			maze[gamerY * 30 + (gamerX - 1)] = -1;
		}
		//up
		if (maze[(gamerY + 1) * 30 + gamerX] == 1) {
			maze[(gamerY + 1) * 30 + gamerX] = -1;
		}
		//down
		if (maze[(gamerY - 1) * 30 + gamerX] == 1) {
			maze[(gamerY - 1) * 30 + gamerX] = -1;
		}
		//upleft
		if (maze[(gamerY + 1) * 30 + gamerX - 1] == 1) {
			maze[(gamerY + 1) * 30 + gamerX - 1] = -1;
		}
		//upright
		if (maze[(gamerY + 1) * 30 + gamerX + 1] == 1) {
			maze[(gamerY + 1) * 30 + gamerX + 1] = -1;
		}
		//downleft
		if (maze[(gamerY - 1) * 30 + gamerX - 1] == 1) {
			maze[(gamerY - 1) * 30 + gamerX - 1] = -1;
		}

		//downright
		if (maze[(gamerY - 1) * 30 + gamerX + 1] == 1) {
			maze[(gamerY - 1) * 30 + gamerX + 1] = -1;
		}
	}
	//第三视图下的移动
	if (!inFirstSight) {
		if (key == GLUT_KEY_UP) {
			if (gamer[3] == 0) {
				if (maze[(gamerY + 1) * 30 + gamerX] == 0)
					gamer[1]++;
			}
			else {
				gamer[3] = 0;
			}
		}
		if (key == GLUT_KEY_DOWN) {
			if (gamer[3] == 2) {
				if (maze[(gamerY - 1) * 30 + gamerX] == 0 && gamerY>0)
					gamer[1]--;
			}
			else {
				gamer[3] = 2;
			}
		}
		if (key == GLUT_KEY_LEFT) {
			if (gamer[3] == 1) {
				if (maze[gamerY * 30 + gamerX - 1] == 0)
					gamer[0]--;
			}
			else {
				gamer[3] = 1;
			}
		}
		if (key == GLUT_KEY_RIGHT) {
			if (gamer[3] == 3) {
				if (maze[gamerY * 30 + gamerX + 1] == 0)
					gamer[0]++;
			}
			else {
				gamer[3] = 3;
			}
		}
	}
	else {
		//第一视图下的移动

		if (key == GLUT_KEY_UP) {
			switch (gamer[3])
			{
			case 0:
				if (maze[(gamerY + 1) * 30 + gamerX] == 0)
					gamer[1]++;
				break;
			case 1:
				if (maze[gamerY * 30 + (gamerX - 1)] == 0)
					gamer[0]--;
				break;
			case 2:
				if (maze[(gamerY - 1) * 30 + gamerX] == 0 && gamerY>0)
					gamer[1]--;
				break;
			case 3:
				if (maze[gamerY * 30 + (gamerX + 1)] == 0)
					gamer[0]++;
				break;
			default:
				break;
			}

		}
		if (key == GLUT_KEY_DOWN) {
			switch (gamer[3])
			{
			case 0:
				gamer[3] = 2;
				break;
			case 1:
				gamer[3] = 3;
				break;
			case 2:
				gamer[3] = 0;
				break;
			case 3:
				gamer[3] = 1;
				break;
			default:
				break;
			}

		}
		if (key == GLUT_KEY_LEFT) {
			gamer[3] = (gamer[3] + 1) % 4;
		}
		if (key == GLUT_KEY_RIGHT) {
			gamer[3] = (gamer[3] + 3) % 4;
		}
		changeToFirstSight();
	}


	init();
	display();
}
void changeSight(int btn, int state, int x, int y) {

	//cout << x << "," << y << endl;

	if (btn == GLUT_MIDDLE_BUTTON&&state == GLUT_UP) {
		//右键切换一三视角

		if (inFirstSight) {
			lookX = 0;
			lookY = 0;
			lookZ = 15;

			centerX = 0;
			centerY = 0;
			centerZ = 0;

			upX = 0;
			upY = 1;
			upZ = 0;

			inFirstSight = false;

		}
		else {
			changeToFirstSight();
			inFirstSight = true;
		}

		init();
		display();
	}
}

int main(int argc, char **argv) {
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);

	glutInitWindowPosition(300, 50);
	glutInitWindowSize(700, 700);
	glutCreateWindow("迷宫");

	initMaze();
	init();

	glutDisplayFunc(display);
	glutSpecialFunc(move);
	glutMouseFunc(changeSight);
	glutMainLoop();

	return 0;
}