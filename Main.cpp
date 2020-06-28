//ove1 = (ballLoc[0] + cos((ballR*PIE) / 180.0)*ballA)
//ballLoc[0] += cos((ballR*PIE) / 180.0)*ballA;
//ballA = sqrt(pow(x2, 2) + pow(y1, 2));
//ballR = rect + (atan2(y1, x2) * 180 / PIE);

//x1 = cos(((ballR - rect) * PIE) / 180.0) * ballA;
//y1 = sin(((ballR - rect) * PIE) / 180.0) * ballA;
//x2 = cos(((b.ballR - rect) * PIE) / 180.0) * b.ballA;
//y2 = sin(((b.ballR - rect) * PIE) / 180.0) * b.ballA;

#include <iostream>
#include <cmath>
#include "glaux.h"
#include "glut.h"

#define BALL_R	20.0
#define HOLE_R	22.0
#define BALL	10
#define HOLE	6
#define X		1200
#define Y		600
#define G	9.8


class hole { //���� Ŭ����
public:
	float holecol[3];	// ������ �� ��� �迭
	double holeloc[3];	// 6���� ���� ��ġ �迭
	hole(float *c, double *l) { // ���� ������
		for (int i = 0; i<3; i++) {
			holecol[i] = c[i];
			holeloc[i] = l[i];
		}
	}
};

hole *holeList[HOLE];
double hole1[3] = { 110.0,110.0,0.0};
double hole2[3] = { 600.0,110.0,0.0};
double hole3[3] = { 1090.0,110.0,0.0};
double hole4[3] = { 110.0,490.0,0.0};
double hole5[3] = { 600.0,490.0,0.0};
double hole6[3] = { 1090.0,490.0,0.0};


float chole1[3] = { 0.0, 0.5, 0.0};
float chole2[3] = { 0.0, 0.5, 0.0};
float chole3[3] = { 0.0, 0.5, 0.0};
float chole4[3] = { 0.0, 0.5, 0.0};
float chole5[3] = { 0.0, 0.5, 0.0};
float chole6[3] = { 0.0, 0.5, 0.0};

class ball {//�� Ŭ����
public:
	double PIE = atan(1) * 4;
	float ballColor[3];		//���� �� �迭
	double ballLoc[3];	//���� ��ġ�� ��Ÿ���� �迭
	double ballR;		//���� �����̴� ����
	double ballA;			//���� �ӵ��� 0-5�� ����
	double ballG;		//���� �������

public:
	ball(float *c, double *l, double r) { // �� ������
		for (int i = 0; i<3; i++) {
			ballColor[i] = c[i];
			ballLoc[i] = l[i];
		}
		ballR = r;
		ballA = 0;
		ballG = 0.0001;
	}

	void holechk() // ���۰� ���� ���� �Լ�
	{
		for (int i = 0; i<HOLE; i++) {
			double x = holeList[i]->holeloc[0] - ballLoc[0];
			double y = holeList[i]->holeloc[1] - ballLoc[1];
			double z = holeList[i]->holeloc[2] - ballLoc[2];
			if ((x*x + y*y + z*z) < BALL_R*BALL_R - 2) {
				if (ballColor[0] == 1.0f && ballColor[1] == 1.0 && ballColor[2] == 1.0)	// ��� ���� ����� ��� restart
				{
					ballLoc[0] = holeList[i]->holeloc[0];
					ballLoc[1] = holeList[i]->holeloc[1];
					ballLoc[2] = holeList[i]->holeloc[2];
					ballLoc[0] = 650.0;
					ballLoc[1] = 300.0;
					ballLoc[2] = 0.0;
					ballA = 0;
				}
				else {
					for (int j = 0; j<3; j++) {
						ballColor[j] = 0.0f;	// ������ ����������
						ballA *= 0.1;	// ���� �� �ӵ� ���� -> 0.1
						ballLoc[j] = 1050.0f;	// �ʿ��� �� ����
					}
				}
			}
		}
	}
	bool check(const ball& b) const { //���� ��� ���� ���� �浹�ߴ��� üũ
		double move1 = (ballLoc[0] + cos((ballR*PIE) / 180.0)*ballA)//�̸� ���� ��ġ�� �̵����Ѻ� ������ �浹�ϴ��� Ȯ��
			- (b.ballLoc[0] + cos((b.ballR*PIE) / 180.0)*b.ballA);
		double move2 = (ballLoc[1] + sin((ballR*PIE) / 180.0)*ballA)
			- (b.ballLoc[1] + sin((b.ballR*PIE) / 180.0)*b.ballA);

		if (pow(move1, 2) + pow(move2, 2) <= pow(BALL_R * 2, 2) + 1)
			return true;
		else
			return false;
	}

	void move() { //���� ���� ������ ���� ������ġ ���
		if (ballA>ballG*G) {
			ballA -= ballG*G;
			ballLoc[0] += cos((ballR*PIE) / 180.0)*ballA;
			ballLoc[1] += sin((ballR*PIE) / 180.0)*ballA;
		}
		else
			ballA = 0;
	}
	void collisionWall() { //���� �浹�ߴ��� üũ
		double move1 = ballLoc[0] + cos((ballR*PIE) / 180.0)*ballA;//�̸� ������ ������ ��ġ�� ����غ���.
		double move2 = ballLoc[1] + sin((ballR*PIE) / 180.0)*ballA;

		if (move1 - 75 <= BALL_R + 1 || move1 >= X - 75 - BALL_R - 1) {//left, right cheack
			ballR = 180 - ballR;
			if (ballR<0)
				ballR += 360;
			ballA *= 0.70;			//���� �浹 �� �ӵ� 30�ۼ�Ʈ ����ȿ�� ����
		}
		else if (move2 - 75 <= BALL_R + 1 || move2 >= Y - 75 - BALL_R - 1) {	//upside,downside cheack
			ballR = 360 - ballR;
			if (ballR<0)
				ballR += 360;
			ballA *= 0.70;
		}
	}

	void collision(ball& b) {//�浹���Ӱ�� �Լ�

		//�ΰ��� ���� ����
		double rect = atan2(this->ballLoc[1] - b.ballLoc[1],this->ballLoc[0] - b.ballLoc[0]);
		rect *= 180;
		rect /= PIE;

		//�浹 �ӵ����� 45�ۼ�Ʈ
		ballA *= 0.55;
		b.ballA *= 0.55;

		double x1,x2,y2,y1;		//���ͻ��

		x1 = cos(((ballR - rect)*PIE) / 180.0)*ballA;
		y1 = sin(((ballR - rect)*PIE) / 180.0)*ballA;
		x2 = cos(((b.ballR - rect)*PIE) / 180.0)*b.ballA;
		y2 = sin(((b.ballR - rect)*PIE) / 180.0)*b.ballA;
		//ź���浹�̹Ƿ� �� ���� ���Ͱ��� �̿��Ͽ� ���� �ӵ� ������
		ballA = sqrt(pow(x2, 2) + pow(y1, 2));
		b.ballA = sqrt(pow(x1, 2) + pow(y2, 2));
		//ƨ���� ���� ���� ������
		ballR = rect + (atan2(y1, x2) * 180 / PIE);
		b.ballR = rect + (atan2(y2, x1) * 180 / PIE);
	}
};

GLUquadricObj* obj[6];
ball* ballList[BALL];
bool mouseClick = false;
int linexf, lineyf, linexs, lineys;

void defaultvalue() {

	//������ DEFAULT ��ġ ��
	double ball1[3] = { 250.0,300.0,0.0 };
	double ball2[3] = { 300.0,330.0,0.0 };
	double ball3[3] = { 300.0,270.0,0.0 };
	double ball4[3] = { 350.0,350.0,0.0 };
	double ball5[3] = { 350.0,300.0,0.0 };
	double ball6[3] = { 350.0,250.0,0.0 };
	double ball7[3] = { 400.0,330.0,0.0 };
	double ball8[3] = { 400.0,270.0,0.0 };
	double ball9[3] = { 450.0,300.0,0.0 };
	//����� DEFAULT ��ġ ��
	double ball10[3] = { 850.0,300.0,0.0 };

	//������ DEFAULT �� ��
	float cball1[3] = { 1.0,0.0,0.0 };
	float cball2[3] = { 0.0,1.0,0.0 };
	float cball3[3] = { 0.0,0.0,1.0 };
	float cball4[3] = { 1.0,0.5,0.0 };
	float cball5[3] = { 1.0,1.0,0.0 };
	float cball6[3] = { 1.0,0.0,1.0 };
	float cball7[3] = { 1.0,0.0,0.5 };
	float cball8[3] = { 1.0,0.5,1.0 };
	float cball9[3] = { 0.5,0.0,1.0 };
	//������ DEFAULT �� ��
	float cball10[3] = { 1.0f,1.0f,1.0f };

	//���� new�� ����
	ballList[0] = new ball(cball1, ball1, 170.0);
	ballList[1] = new ball(cball2, ball2, 10.0);
	ballList[2] = new ball(cball3, ball3, 270.0);
	ballList[3] = new ball(cball4, ball4, 90.0);
	ballList[4] = new ball(cball5, ball5, 180.0);
	ballList[5] = new ball(cball6, ball6, 44.0);
	ballList[6] = new ball(cball7, ball7, 60.0);
	ballList[7] = new ball(cball8, ball8, 120.0);
	ballList[8] = new ball(cball9, ball9, 210.0);
	//��� new�� ����
	ballList[9] = new ball(cball10, ball10, 0.0);

	//���� ����
	holeList[0] = new hole(chole1, hole1);
	holeList[1] = new hole(chole2, hole2);
	holeList[2] = new hole(chole3, hole3);
	holeList[3] = new hole(chole4, hole4);
	holeList[4] = new hole(chole5, hole5);
	holeList[5] = new hole(chole6, hole6);

}

void crush() {

	//7���� ������ ���� �浹�ϴ��� üũ
	for (int i = 0; i<BALL; i++)
		ballList[i]->collisionWall();
	//���� ���� �� �Ÿ��� 1 �����϶� ������ üũ
	for (int i = 0; i<BALL; i++)
		ballList[i]->holechk();
	//������ �浹 �� �浹��� üũ
	for (int i = 0; i<BALL - 1; i++) {
		for (int j = i + 1; j<BALL; j++) {
			if (ballList[i]->check(*ballList[j]))
				ballList[i]->collision(*ballList[j]);

		}
	}
}
void mouse(int button, int state, int x, int y) {//���콺 �̺�Ʈ �Լ�

	y = Y - y;//y��ǥ �ʱ�ȭ

	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN && ballList[BALL - 1]->ballA == 0) {//��� Ŭ�� �̺�Ʈ
		if (pow(x - ballList[BALL - 1]->ballLoc[0], 2) + pow(y - ballList[BALL - 1]->ballLoc[1], 2) < pow(BALL_R, 2)) {
			mouseClick = true;
			//��� ��� ������ �߾ӿ� ������ ó��
			linexf = ballList[BALL - 1]->ballLoc[0];
			lineyf = ballList[BALL - 1]->ballLoc[1];
		}
	}
	else if (button == GLUT_LEFT_BUTTON && state == GLUT_UP && ballList[BALL - 1]->ballA == 0) {//���� Ŭ�� off and ��� �ӵ� 0
		double PIE = atan(1) * 4;
		mouseClick = false;
		printf("%d %d\n", linexf - linexs, lineyf - lineys);
		printf("%lf\n", atan2(double(lineyf - lineys), double(linexf - linexs)) * 180 / PIE);
		ballList[BALL - 1]->ballR = atan2(double(lineyf - lineys), double(linexf - linexs)) * 180 / PIE;
		//���� ������ ���� �� ���

		ballList[BALL - 1]->ballA = sqrt(pow(linexf - linexs, 2.0) + pow(lineyf - lineys, 2.0)) / 20;
		//����� �ӵ�

	}
}

void mousemove(int x, int y) {//����� ���� �Լ�

	if (mouseClick) {//���� Ŭ�� on
		y = Y - y;
		linexs = x;
		lineys = y;
	}
}

void display() {
	glClearColor(0, 0, 0, 0);	// ���� = ����
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glEnable(GL_DEPTH_TEST);
	glDisable(GL_LIGHTING);
	glColor3f(0.1f, 0.2f, 0.1f);
	glBegin(GL_LINE_STRIP);
	glVertex3f(50.0f, 50.0f, 0.0f);
	glVertex3f(1150.0f, 50.0f, 0.0f);
	glVertex3f(1150.0f, 550.0f, 0.0f);
	glVertex3f(50.0f, 550.0f, 0.0f);
	glVertex3f(50.0f, 50.0f, 0.0f);
	glVertex3f(75.0f, 75.0f, 0.0f);
	glVertex3f(1125.0f, 75.0f, 0.0f);
	glVertex3f(1150.0f, 50.0f, 0.0f);
	glVertex3f(1125.0f, 75.0f, 0.0f);
	glVertex3f(1125.0f, 525.0f, 0.0f);
	glVertex3f(1150.0f, 550.0f, 0.0f);
	glVertex3f(1125.0f, 525.0f, 0.0f);
	glVertex3f(75.0f, 525.0f, 0.0f);
	glVertex3f(50.0f, 550.0f, 0.0f);
	glVertex3f(75.0f, 525.0f, 0.0f);
	glVertex3f(75.0f, 75.0f, 0.0f);
	glEnd();

	for (int i = 0; i<BALL; i++) {
		GLfloat light_position[] = { -0.5f, 0.5f, 0.5f, 0.0f }; 
		glMaterialfv(GL_FRONT, GL_DIFFUSE, ballList[i]->ballColor);
		glLightfv(GL_LIGHT0, GL_POSITION, light_position);
		glEnable(GL_LIGHTING);
		glPushMatrix();
		glTranslatef(ballList[i]->ballLoc[0],
			ballList[i]->ballLoc[1], ballList[i]->ballLoc[2]);
		glutSolidSphere(BALL_R, 24, 24);
		glPopMatrix();
	}
	for (int i = 0; i<HOLE; i++) {
		glEnable(GL_LIGHT0);
		glDisable(GL_LIGHTING);
		glPushMatrix();
		glTranslatef(holeList[i]->holeloc[0], holeList[i]->holeloc[1], holeList[i]->holeloc[2]);
		glutSolidSphere(HOLE_R, 24, 24);
		glPopMatrix();
	}
	glDisable(GL_LIGHTING);
	glPushMatrix();
	glEnable(GL_TEXTURE_2D);
	glColor3f(0.0f, 0.5f, 0.0f);
	glBegin(GL_QUADS);
	glNormal3f(0.0f, 0.0f, 1.0f);
	glBegin(GL_QUADS);
	glTexCoord2f(0.0f, 0.0f);glVertex3f(75.0f, 75.0f, 0.0f);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(75.0f, 525.0f, 0.0f);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(1125.0f, 525.0f, 0.0f);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(1125.0f, 75.0f, 0.0f);
	glEnd();
	glDisable(GL_TEXTURE_2D);
	glPopMatrix();

	glColor3f(0.45f, 0.2f, 0.1f); 
	glRectf(50.0f, 50.0f, 1150.0f, 550.0f);
	glutSwapBuffers();
}

void MyTimer(int value) {
	for (int i = 0; i<BALL; i++)//������ ��갪�� ���� �̵�
		ballList[i]->move();

	crush();
	glutPostRedisplay();			//ȭ���� �ʱ�ȭ
	glutTimerFunc(1, MyTimer, 1);		//���� �ʱ�ȭ
}

int main() {
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
	glutInitWindowSize(X, Y);
	glutCreateWindow("�������� ������Ʈ - 2019253071 ������");
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glViewport(0, 0, X, Y);	
	glOrtho(0.0, X, 0.0, Y, -100.0, 100.0);
	defaultvalue();
	glutMouseFunc(mouse);			
	glutMotionFunc(mousemove);		
	glutTimerFunc(1, MyTimer, 1);		
	glutDisplayFunc(display);
	glutMainLoop();
	return 0;
}