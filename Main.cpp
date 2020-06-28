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


class hole { //구멍 클래스
public:
	float holecol[3];	// 구멍의 색 명암 배열
	double holeloc[3];	// 6개의 구멍 위치 배열
	hole(float *c, double *l) { // 구멍 생성자
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

class ball {//볼 클래스
public:
	double PIE = atan(1) * 4;
	float ballColor[3];		//공의 색 배열
	double ballLoc[3];	//공의 위치를 나타내는 배열
	double ballR;		//공이 움직이는 각도
	double ballA;			//공의 속도를 0-5로 설정
	double ballG;		//공의 마찰계수

public:
	ball(float *c, double *l, double r) { // 공 생성자
		for (int i = 0; i<3; i++) {
			ballColor[i] = c[i];
			ballLoc[i] = l[i];
		}
		ballR = r;
		ballA = 0;
		ballG = 0.0001;
	}

	void holechk() // 구멍과 접촉 반응 함수
	{
		for (int i = 0; i<HOLE; i++) {
			double x = holeList[i]->holeloc[0] - ballLoc[0];
			double y = holeList[i]->holeloc[1] - ballLoc[1];
			double z = holeList[i]->holeloc[2] - ballLoc[2];
			if ((x*x + y*y + z*z) < BALL_R*BALL_R - 2) {
				if (ballColor[0] == 1.0f && ballColor[1] == 1.0 && ballColor[2] == 1.0)	// 닿는 공이 흰공일 경우 restart
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
						ballColor[j] = 0.0f;	// 공색을 검은색으로
						ballA *= 0.1;	// 들어가기 전 속도 감소 -> 0.1
						ballLoc[j] = 1050.0f;	// 맵에서 공 제거
					}
				}
			}
		}
	}
	bool check(const ball& b) const { //공이 어떠한 공과 서로 충돌했는지 체크
		double move1 = (ballLoc[0] + cos((ballR*PIE) / 180.0)*ballA)//미리 다음 위치로 이동시켜본 다음에 충돌하는지 확인
			- (b.ballLoc[0] + cos((b.ballR*PIE) / 180.0)*b.ballA);
		double move2 = (ballLoc[1] + sin((ballR*PIE) / 180.0)*ballA)
			- (b.ballLoc[1] + sin((b.ballR*PIE) / 180.0)*b.ballA);

		if (pow(move1, 2) + pow(move2, 2) <= pow(BALL_R * 2, 2) + 1)
			return true;
		else
			return false;
	}

	void move() { //현재 공의 각도에 따른 다음위치 계산
		if (ballA>ballG*G) {
			ballA -= ballG*G;
			ballLoc[0] += cos((ballR*PIE) / 180.0)*ballA;
			ballLoc[1] += sin((ballR*PIE) / 180.0)*ballA;
		}
		else
			ballA = 0;
	}
	void collisionWall() { //벽에 충돌했는지 체크
		double move1 = ballLoc[0] + cos((ballR*PIE) / 180.0)*ballA;//미리 다음에 움직일 위치를 계산해본다.
		double move2 = ballLoc[1] + sin((ballR*PIE) / 180.0)*ballA;

		if (move1 - 75 <= BALL_R + 1 || move1 >= X - 75 - BALL_R - 1) {//left, right cheack
			ballR = 180 - ballR;
			if (ballR<0)
				ballR += 360;
			ballA *= 0.70;			//벽에 충돌 시 속도 30퍼센트 감소효과 적용
		}
		else if (move2 - 75 <= BALL_R + 1 || move2 >= Y - 75 - BALL_R - 1) {	//upside,downside cheack
			ballR = 360 - ballR;
			if (ballR<0)
				ballR += 360;
			ballA *= 0.70;
		}
	}

	void collision(ball& b) {//충돌감속계산 함수

		//두공의 각도 측정
		double rect = atan2(this->ballLoc[1] - b.ballLoc[1],this->ballLoc[0] - b.ballLoc[0]);
		rect *= 180;
		rect /= PIE;

		//충돌 속도감소 45퍼센트
		ballA *= 0.55;
		b.ballA *= 0.55;

		double x1,x2,y2,y1;		//벡터사용

		x1 = cos(((ballR - rect)*PIE) / 180.0)*ballA;
		y1 = sin(((ballR - rect)*PIE) / 180.0)*ballA;
		x2 = cos(((b.ballR - rect)*PIE) / 180.0)*b.ballA;
		y2 = sin(((b.ballR - rect)*PIE) / 180.0)*b.ballA;
		//탄성충돌이므로 각 구의 벡터값를 이용하여 공의 속도 재측정
		ballA = sqrt(pow(x2, 2) + pow(y1, 2));
		b.ballA = sqrt(pow(x1, 2) + pow(y2, 2));
		//튕겨진 공의 각도 재측정
		ballR = rect + (atan2(y1, x2) * 180 / PIE);
		b.ballR = rect + (atan2(y2, x1) * 180 / PIE);
	}
};

GLUquadricObj* obj[6];
ball* ballList[BALL];
bool mouseClick = false;
int linexf, lineyf, linexs, lineys;

void defaultvalue() {

	//색공의 DEFAULT 위치 값
	double ball1[3] = { 250.0,300.0,0.0 };
	double ball2[3] = { 300.0,330.0,0.0 };
	double ball3[3] = { 300.0,270.0,0.0 };
	double ball4[3] = { 350.0,350.0,0.0 };
	double ball5[3] = { 350.0,300.0,0.0 };
	double ball6[3] = { 350.0,250.0,0.0 };
	double ball7[3] = { 400.0,330.0,0.0 };
	double ball8[3] = { 400.0,270.0,0.0 };
	double ball9[3] = { 450.0,300.0,0.0 };
	//흰공의 DEFAULT 위치 값
	double ball10[3] = { 850.0,300.0,0.0 };

	//색공의 DEFAULT 색 값
	float cball1[3] = { 1.0,0.0,0.0 };
	float cball2[3] = { 0.0,1.0,0.0 };
	float cball3[3] = { 0.0,0.0,1.0 };
	float cball4[3] = { 1.0,0.5,0.0 };
	float cball5[3] = { 1.0,1.0,0.0 };
	float cball6[3] = { 1.0,0.0,1.0 };
	float cball7[3] = { 1.0,0.0,0.5 };
	float cball8[3] = { 1.0,0.5,1.0 };
	float cball9[3] = { 0.5,0.0,1.0 };
	//색공의 DEFAULT 색 값
	float cball10[3] = { 1.0f,1.0f,1.0f };

	//색공 new로 생성
	ballList[0] = new ball(cball1, ball1, 170.0);
	ballList[1] = new ball(cball2, ball2, 10.0);
	ballList[2] = new ball(cball3, ball3, 270.0);
	ballList[3] = new ball(cball4, ball4, 90.0);
	ballList[4] = new ball(cball5, ball5, 180.0);
	ballList[5] = new ball(cball6, ball6, 44.0);
	ballList[6] = new ball(cball7, ball7, 60.0);
	ballList[7] = new ball(cball8, ball8, 120.0);
	ballList[8] = new ball(cball9, ball9, 210.0);
	//흰공 new로 생성
	ballList[9] = new ball(cball10, ball10, 0.0);

	//구명 생성
	holeList[0] = new hole(chole1, hole1);
	holeList[1] = new hole(chole2, hole2);
	holeList[2] = new hole(chole3, hole3);
	holeList[3] = new hole(chole4, hole4);
	holeList[4] = new hole(chole5, hole5);
	holeList[5] = new hole(chole6, hole6);

}

void crush() {

	//7개의 공들이 벽에 충돌하는지 체크
	for (int i = 0; i<BALL; i++)
		ballList[i]->collisionWall();
	//공이 구멍 간 거리가 1 이하일때 들어가도록 체크
	for (int i = 0; i<BALL; i++)
		ballList[i]->holechk();
	//공끼리 충돌 시 충돌계수 체크
	for (int i = 0; i<BALL - 1; i++) {
		for (int j = i + 1; j<BALL; j++) {
			if (ballList[i]->check(*ballList[j]))
				ballList[i]->collision(*ballList[j]);

		}
	}
}
void mouse(int button, int state, int x, int y) {//마우스 이벤트 함수

	y = Y - y;//y좌표 초기화

	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN && ballList[BALL - 1]->ballA == 0) {//흰공 클릭 이벤트
		if (pow(x - ballList[BALL - 1]->ballLoc[0], 2) + pow(y - ballList[BALL - 1]->ballLoc[1], 2) < pow(BALL_R, 2)) {
			mouseClick = true;
			//흰공 어디를 눌러도 중앙에 오도록 처리
			linexf = ballList[BALL - 1]->ballLoc[0];
			lineyf = ballList[BALL - 1]->ballLoc[1];
		}
	}
	else if (button == GLUT_LEFT_BUTTON && state == GLUT_UP && ballList[BALL - 1]->ballA == 0) {//왼쪽 클릭 off and 흰공 속도 0
		double PIE = atan(1) * 4;
		mouseClick = false;
		printf("%d %d\n", linexf - linexs, lineyf - lineys);
		printf("%lf\n", atan2(double(lineyf - lineys), double(linexf - linexs)) * 180 / PIE);
		ballList[BALL - 1]->ballR = atan2(double(lineyf - lineys), double(linexf - linexs)) * 180 / PIE;
		//공의 각도를 예측 및 계산

		ballList[BALL - 1]->ballA = sqrt(pow(linexf - linexs, 2.0) + pow(lineyf - lineys, 2.0)) / 20;
		//흰공의 속도

	}
}

void mousemove(int x, int y) {//흰공의 세기 함수

	if (mouseClick) {//왼쪽 클릭 on
		y = Y - y;
		linexs = x;
		lineys = y;
	}
}

void display() {
	glClearColor(0, 0, 0, 0);	// 배경색 = 검정
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
	for (int i = 0; i<BALL; i++)//공들이 계산값에 따라 이동
		ballList[i]->move();

	crush();
	glutPostRedisplay();			//화면을 초기화
	glutTimerFunc(1, MyTimer, 1);		//연속 초기화
}

int main() {
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
	glutInitWindowSize(X, Y);
	glutCreateWindow("공업수학 프로젝트 - 2019253071 안정수");
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