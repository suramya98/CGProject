#include <stdlib.h>
#include <math.h>
#include <GL/glut.h>
#include <iostream>
#include <Windows.h>
#include <mmsystem.h>
using namespace std;
const double PI = 3.141592654;

 static int frameNumber = 0;

static bool isWin, isLost;
static int leftCannibals, leftMissionaries, rightCannibals, rightMissionaries, onBoatCannibals, onBoatMissionaries;

static GLfloat boatPosX, boatDeltaX;
static bool moveBoat, moveBoatReverse, boatCrossed, riverCrossedOnce;
static float moveClouds, cloudDeltaX;

//Character Details

static int leftCharacterPosX, leftCharacterPosY, rightCharacterPosX, rightCharacterPosY, characterWidth, characterHeight;

static bool leftMissionaryVisibility[3], leftCannibalVisibility[3], rightMissionaryVisibility[3], rightCannibalVisibility[3], boatMissionaryVisibility[2], boatCannibalVisibility[2];

static int boatCharacterLeftX, boatCharacterRightX, boatCharacterY;

static int boatLeftCharacter, boatRightCharacter;



inline void resetAll()
{

	isWin = false;
	isLost = false;

	boatLeftCharacter = 0;
	boatRightCharacter = 0;

	boatPosX = 0.0;
	boatDeltaX = 2.0;
	moveBoat = false;
	moveBoatReverse = false;
	boatCrossed = false;
	riverCrossedOnce = false;

	moveClouds = 0.0;
	cloudDeltaX = 1.0;

	leftCharacterPosX = 0;
	leftCharacterPosY = 230;

	rightCharacterPosX = 650;
	rightCharacterPosY = 230;

	characterWidth = 50;
	characterHeight = 80;

	for (int i = 0; i < 3; i++)
	{
	leftMissionaryVisibility[i] = true;
	leftCannibalVisibility[i] = true;

	rightMissionaryVisibility[i] = false;
	rightCannibalVisibility[i] = false;
	}

	for (int i = 0; i < 2; i++)
	{
	boatMissionaryVisibility[i] = false;
	boatCannibalVisibility[i] = false;
	}

	boatCharacterLeftX = 270;
	boatCharacterRightX = 260;
	boatCharacterY = 80;

	system("cls");

}

inline void checkGameStatus()
{
	cout << "Checking game status..." << endl;

	leftCannibals = 0;
	leftMissionaries = 0;
	rightCannibals = 0;
	rightMissionaries = 0;
	onBoatCannibals = 0;
	onBoatMissionaries = 0;



	for (int i = 0; i < 3; i++)
	{
	if (leftCannibalVisibility[i])
	{
	leftCannibals++;
	}

	if (leftMissionaryVisibility[i])
	{
	leftMissionaries++;
	}

	if (rightCannibalVisibility[i])
	{
	rightCannibals++;
	}

	if (rightMissionaryVisibility[i])
	{
	rightMissionaries++;
	}
	}

	if (boatLeftCharacter == 1)
	{
	onBoatCannibals++;
	}
	if (boatRightCharacter == 1)
	{
	onBoatCannibals++;
	}

	if (boatLeftCharacter == 2)
	{
	onBoatMissionaries++;
	}

	if (boatRightCharacter == 2)
	{
	onBoatMissionaries++;
	}

	cout << "Left    : cannibals = " << leftCannibals << ", missionaries = " << leftMissionaries << endl;
	cout << "Right   : cannibals = " << rightCannibals << ", missionaries = " << rightMissionaries << endl;
	cout << "On Boat : cannibals = " << onBoatCannibals << ", missionaries = " << onBoatMissionaries << endl;

	if (!boatCrossed)
	{
	if ((leftCannibals + onBoatCannibals) > (leftMissionaries + onBoatMissionaries) && (leftMissionaries + onBoatMissionaries) > 0)
	{
	isLost = true;
	}
	if ((rightCannibals) > (rightMissionaries) && rightMissionaries > 0)
	{
	isLost = true;
	}
	}
	else if (boatCrossed)
	{
	if ((rightCannibals + onBoatCannibals) > (rightMissionaries + onBoatMissionaries) && (rightMissionaries + onBoatMissionaries) > 0)
	{
	isLost = true;
	}
	if ((leftCannibals) > (leftMissionaries) && leftMissionaries > 0)
	{
	isLost = true;
	}
	}

	if (boatCrossed && (rightCannibals + onBoatCannibals) == 3 && (rightMissionaries + onBoatMissionaries) == 3)
	{
	isWin = true;
	}

	if (isLost)
	{

	int msgRes = MessageBox(NULL, "You Lose...", "Retry", MB_RETRYCANCEL);
	if (msgRes == IDRETRY)
	{
	resetAll();
	}
	else if (msgRes == IDCANCEL)
	{
	exit(1);
	}
	}

	if (isWin)
	{
	MessageBox(NULL, "Congratulation. You Win!!!", "Congratulation", MB_OK);
	int msgRes = MessageBox(NULL, "Do you want to play again?", "Restart?", MB_YESNO);

	if (msgRes == IDYES)
	{
	resetAll();
	}
	else if (msgRes == IDNO)
	{
	exit(1);
	}
	}


}

inline int getCannibalBlankPosition()
{
	if (!boatCrossed)
	{
	for (int i = 0; i < 3; i++)
	{
	if (leftCannibalVisibility[i] == false)
	{
	return i;
	}
	}

	return -1;
	}
	else if (boatCrossed)
	{
	for (int i = 0; i < 3; i++)
	{
	if (rightCannibalVisibility[i] == false)
	{
	return i;
	}
	}

	return -1;
	}
}

inline int getMissinaryBlankPosition()
{
	if (!boatCrossed)
	{
	for (int i = 0; i < 3; i++)
	{
	if (leftMissionaryVisibility[i] == false)
	{
	return i;
	}
	}

	return -1;
	}
	else if (boatCrossed)
	{
	for (int i = 0; i < 3; i++)
	{
	if (rightMissionaryVisibility[i] == false)
	{
	return i;
	}
	}

	return -1;
	}
}

inline void drawPixel(GLint cx, GLint cy)
{

	glBegin(GL_POINTS);
	glVertex2i(cx, cy);
	glEnd();
}

inline void plotPixels(GLint h, GLint k, GLint x, GLint y)
{
	drawPixel(x + h, y + k);
	drawPixel(-x + h, y + k);
	drawPixel(x + h, -y + k);
	drawPixel(-x + h, -y + k);
	drawPixel(y + h, x + k);
	drawPixel(-y + h, x + k);
	drawPixel(y + h, -x + k);
	drawPixel(-y + h, -x + k);
}

inline void drawCircle(GLint h, GLint k, GLint r)
{
	GLint d = 1 - r, x = 0, y = r;
	while (y > x)
	{
	plotPixels(h, k, x, y);
	if (d < 0) d += 2 * x + 3;
	else
	{
	d += 2 * (x - y) + 5;
	--y;
	}
	++x;
	}
	plotPixels(h, k, x, y);
}

inline void drawBackground()
{
	glBegin(GL_POLYGON);
	glColor3f(0.2, 0.6, 1.0);
	glVertex2i(0, 450);
	glVertex2i(0, 1000);
	glVertex2i(1000, 1000);
	glVertex2i(1000, 450);
	glEnd();

	glBegin(GL_POLYGON);
	glColor3f(0.6, 1.0, 0.5);
	glVertex2i(0, 450);
	glVertex2i(0, 0);
	glVertex2i(1000, 0);
	glVertex2i(1000, 450);
	glEnd();
}

inline void drawWater()
{
	glBegin(GL_POLYGON);//river stablepart
	glColor3f(0.2, 0.6, 1.0);
	glVertex2i(0, 0);
	glVertex2i(0, 100);
	glVertex2i(1000, 100);
	glVertex2i(1000, 0);
	glEnd();

	glBegin(GL_POLYGON);//river flowpart
	glColor3f(0.2, 0.6, 1.0);
	glVertex2i(420, 450);
	glVertex2i(290, 100);
	glVertex2i(690, 100);
	glVertex2i(520, 450);
	glEnd();

}

inline void grass()
{
	glBegin(GL_TRIANGLES);
	glColor3f(0.0, 1.0, 0.0);
	glVertex2i(0, 0); //10
	glVertex2i(5, 230);
	glVertex2i(20, 0);
	glEnd();
	glBegin(GL_LINE_LOOP);
	glColor3f(0.0, 0.0, 0.0);
	glVertex2i(0, 0); //10
	glVertex2i(5, 230);
	glVertex2i(20, 0);
	glEnd();

	glBegin(GL_TRIANGLES);
	glColor3f(0.0, 1.0, 0.0);
	glVertex2i(20, 0); //t1
	glVertex2i(40, 150);
	glVertex2i(60, 0);
	glEnd();
	glBegin(GL_LINE_LOOP);
	glColor3f(0.0, 0.0, 0.0);
	glVertex2i(20, 0); //t1
	glVertex2i(40, 150);
	glVertex2i(60, 0);
	glEnd();

	glBegin(GL_TRIANGLES);
	glColor3f(0.0, 1.0, 0.0);
	glVertex2i(50, 0); //t2
	glVertex2i(80, 170);
	glVertex2i(100, 0);
	glEnd();
	glBegin(GL_LINE_LOOP);
	glColor3f(0.0, 0.0, 0.0);
	glVertex2i(50, 0); //t2
	glVertex2i(80, 170);
	glVertex2i(100, 0);
	glEnd();

	glBegin(GL_TRIANGLES);
	glColor3f(0.0, 1.0, 0.0);
	glVertex2i(95, 0); //t3
	glVertex2i(120,120);
	glVertex2i(140, 0);
	glEnd();
	glBegin(GL_LINE_LOOP);
	glColor3f(0.0, 0.0, 0.0);
	glVertex2i(95, 0); //t3
	glVertex2i(120, 120);
	glVertex2i(140, 0);
	glEnd();

	glBegin(GL_TRIANGLES);
	glColor3f(0.0, 1.0, 0.0);
	glVertex2i(140, 0); //t4
	glVertex2i(160, 180);
	glVertex2i(180, 0);
	glEnd();
	glBegin(GL_LINE_LOOP);
	glColor3f(0.0, 0.0, 0.0);
	glVertex2i(140, 0); //t4
	glVertex2i(160, 180);
	glVertex2i(180, 0);
	glEnd();

	glBegin(GL_TRIANGLES);
	glColor3f(0.0, 1.0, 0.0);
	glVertex2i(175, 0); //t5
	glVertex2i(200, 140);
	glVertex2i(220, 0);
	glEnd();
	glBegin(GL_LINE_LOOP);
	glColor3f(0.0, 0.0, 0.0);
	glVertex2i(175, 0); //t5
	glVertex2i(200, 140);
	glVertex2i(220, 0);
	glEnd();

	glBegin(GL_TRIANGLES);
	glColor3f(0.0, 1.0, 0.0);
	glVertex2i(220, 0); //t6
	glVertex2i(240, 50);
	glVertex2i(260, 0);
	glEnd();
	glBegin(GL_LINE_LOOP);
	glColor3f(0.0, 0.0, 0.0);
	glVertex2i(220, 0); //t6
	glVertex2i(240, 50);
	glVertex2i(260, 0);
	glEnd();

	glBegin(GL_TRIANGLES);
	glColor3f(0.0, 1.0, 0.0);
	glVertex2i(260, 0); //t7
	glVertex2i(290, 130);
	glVertex2i(310, 0);
	glEnd();
	glBegin(GL_LINE_LOOP);
	glColor3f(0.0, 0.0, 0.0);
	glVertex2i(260, 0); //t7
	glVertex2i(290, 130);
	glVertex2i(310, 0);
	glEnd();

	glBegin(GL_TRIANGLES);
	glColor3f(0.0, 1.0, 0.0);
	glVertex2i(300, 0); //t8
	glVertex2i(320, 160);
	glVertex2i(340, 0);
	glEnd();
	glBegin(GL_LINE_LOOP);
	glColor3f(0.0, 0.0, 0.0);
	glVertex2i(300, 0); //t8
	glVertex2i(320, 160);
	glVertex2i(340, 0);
	glEnd();

	glBegin(GL_TRIANGLES);
	glColor3f(0.0, 1.0, 0.0);
	glVertex2i(340, 0); //t9
	glVertex2i(360, 190);
	glVertex2i(390, 0);
	glEnd();
	glBegin(GL_LINE_LOOP);
	glColor3f(0.0, 0.0, 0.0);
	glVertex2i(340, 0); //t9
	glVertex2i(360, 190);
	glVertex2i(390, 0);
	glEnd();

	glBegin(GL_TRIANGLES);
	glColor3f(0.0, 1.0, 0.0);
	glVertex2i(390, 0); //10
	glVertex2i(410, 80);
	glVertex2i(430, 0);
	glEnd();
	glBegin(GL_LINE_LOOP);
	glColor3f(0.0,0.0, 0.0);
	glVertex2i(390, 0); //10
	glVertex2i(410, 80);
	glVertex2i(430, 0);
	glEnd();

	glBegin(GL_TRIANGLES);
	glColor3f(0.0, 1.0, 0.0);
	glVertex2i(420, 0); //11
	glVertex2i(440, 200);
	glVertex2i(460, 0);
	glEnd();
	glBegin(GL_LINE_LOOP);
	glColor3f(0.0, 0.0, 0.0);
	glVertex2i(420, 0); //11
	glVertex2i(440, 200);
	glVertex2i(460, 0);
	glEnd();

	glBegin(GL_TRIANGLES);
	glColor3f(0.0, 1.0, 0.0);
	glVertex2i(460, 0); //12
	glVertex2i(480, 150);
	glVertex2i(500, 0);
	glEnd();
	glBegin(GL_LINE_LOOP);
	glColor3f(0.0, 0.0, 0.0);
	glVertex2i(460, 0); //12
	glVertex2i(480, 150);
	glVertex2i(500, 0);
	glEnd();

	glBegin(GL_TRIANGLES);
	glColor3f(0.0, 1.0, 0.0);
	glVertex2i(495, 0); //t1
	glVertex2i(520, 120);
	glVertex2i(540, 0);
	glEnd();
	glBegin(GL_LINE_LOOP);
	glColor3f(0.0,0.0, 0.0);
	glVertex2i(495, 0); //t1
	glVertex2i(520, 120);
	glVertex2i(540, 0);
	glEnd();

	glBegin(GL_TRIANGLES);
	glColor3f(0.0, 1.0, 0.0);
	glVertex2i(530, 0); //t2
	glVertex2i(550, 170);
	glVertex2i(570, 0);
	glEnd();
	glBegin(GL_LINE_LOOP);
	glColor3f(0.0,0.0, 0.0);
	glVertex2i(530, 0); //t2
	glVertex2i(550, 170);
	glVertex2i(570, 0);
	glEnd();

	glBegin(GL_TRIANGLES);
	glColor3f(0.0, 1.0, 0.0);
	glVertex2i(570, 0); //t3
	glVertex2i(590,30);
	glVertex2i(610, 0);
	glEnd();
	glBegin(GL_LINE_LOOP);
	glColor3f(0.0,0.0, 0.0);
	glVertex2i(570, 0); //t3
	glVertex2i(590, 30);
	glVertex2i(610, 0);
	glEnd();

	glBegin(GL_TRIANGLES);
	glColor3f(0.0, 1.0, 0.0);
	glVertex2i(605, 0); //t4
	glVertex2i(620, 180);
	glVertex2i(640, 0);
	glEnd();
	glBegin(GL_LINE_LOOP);
	glColor3f(0.0, 0.0, 0.0);
	glVertex2i(605, 0); //t4
	glVertex2i(620, 180);
	glVertex2i(640, 0);
	glEnd();

	glBegin(GL_TRIANGLES);
	glColor3f(0.0, 1.0, 0.0);
	glVertex2i(635, 0); //t5
	glVertex2i(650, 140);
	glVertex2i(670, 0);
	glEnd();
	glBegin(GL_LINE_LOOP);
	glColor3f(0.0,0.0, 0.0);
	glVertex2i(635, 0); //t5
	glVertex2i(650, 140);
	glVertex2i(670, 0);
	glEnd();

	glBegin(GL_TRIANGLES);
	glColor3f(0.0, 1.0, 0.0);
	glVertex2i(670, 0); //t6
	glVertex2i(690,90);
	glVertex2i(710, 0);
	glEnd();
	glBegin(GL_LINE_LOOP);
	glColor3f(0.0, 0.0, 0.0);
	glVertex2i(670, 0); //t6
	glVertex2i(690, 90);
	glVertex2i(710, 0);
	glEnd();

	glBegin(GL_TRIANGLES);
	glColor3f(0.0, 1.0, 0.0);
	glVertex2i(705, 0); //t7
	glVertex2i(720, 130);
	glVertex2i(740, 0);
	glEnd();
	glBegin(GL_LINE_LOOP);
	glColor3f(0.0, 0.0, 0.0);
	glVertex2i(705, 0); //t7
	glVertex2i(720, 130);
	glVertex2i(740, 0);
	glEnd();

	glBegin(GL_TRIANGLES);
	glColor3f(0.0, 1.0, 0.0);
	glVertex2i(735, 0); //t8
	glVertex2i(750, 160);
	glVertex2i(790, 0);
	glEnd();
	glBegin(GL_LINE_LOOP);
	glColor3f(0.0,0.0, 0.0);
	glVertex2i(735, 0); //t8
	glVertex2i(750, 160);
	glVertex2i(790, 0);
	glEnd();

	glBegin(GL_TRIANGLES);
	glColor3f(0.0, 1.0, 0.0);
	glVertex2i(785, 0); //t9
	glVertex2i(800, 190);
	glVertex2i(820, 0);
	glEnd();
	glBegin(GL_LINE_LOOP);
	glColor3f(0.0,0.0, 0.0);
	glVertex2i(785, 0); //t9
	glVertex2i(800, 190);
	glVertex2i(820, 0);
	glEnd();

	glBegin(GL_TRIANGLES);
	glColor3f(0.0, 1.0, 0.0);
	glVertex2i(815, 0); //10
	glVertex2i(830, 80);
	glVertex2i(845, 0);
	glEnd();
	glBegin(GL_LINE_LOOP);
	glColor3f(0.0,0.0, 0.0);
	glVertex2i(815, 0); //10
	glVertex2i(830, 80);
	glVertex2i(845, 0);
	glEnd();

	glBegin(GL_TRIANGLES);
	glColor3f(0.0, 1.0, 0.0);
	glVertex2i(845, 0); //t4
	glVertex2i(860, 180);
	glVertex2i(880, 0);
	glEnd();
	glBegin(GL_LINE_LOOP);
	glColor3f(0.0, 0.0, 0.0);
	glVertex2i(845, 0); //t4
	glVertex2i(860, 180);
	glVertex2i(880, 0);
	glEnd();

	glBegin(GL_TRIANGLES);
	glColor3f(0.0, 1.0, 0.0);
	glVertex2i(875, 0); //t5
	glVertex2i(890, 140);
	glVertex2i(910, 0);
	glEnd();
	glBegin(GL_LINE_LOOP);
	glColor3f(0.0,0.0, 0.0);
	glVertex2i(875, 0); //t5
	glVertex2i(890, 140);
	glVertex2i(910, 0);
	glEnd();

	glBegin(GL_TRIANGLES);
	glColor3f(0.0, 1.0, 0.0);
	glVertex2i(905, 0); //t6
	glVertex2i(920, 190);
	glVertex2i(950, 0);
	glEnd();
	glBegin(GL_LINE_LOOP);
	glColor3f(0.0,0.0, 0.0);
	glVertex2i(905, 0); //t6
	glVertex2i(920, 190);
	glVertex2i(950, 0);
	glEnd();

	glBegin(GL_TRIANGLES);
	glColor3f(0.0, 1.0, 0.0);
	glVertex2i(945, 0); //t7
	glVertex2i(970, 200);
	glVertex2i(990, 0);
	glEnd();
	glBegin(GL_LINE_LOOP);
	glColor3f(0.0,0.0, 0.0);
	glVertex2i(945, 0); //t7
	glVertex2i(970, 200);
	glVertex2i(990, 0);
	glEnd();

	glBegin(GL_TRIANGLES);
	glColor3f(0.0, 1.0, 0.0);
	glVertex2i(990, 0); //t8
	glVertex2i(1120, 160);
	glVertex2i(1140, 0);
	glEnd();
	glBegin(GL_LINE_LOOP);
	glColor3f(0.0,0.0, 0.0);
	glVertex2i(990, 0); //t8
	glVertex2i(1120, 160);
	glVertex2i(1140, 0);
	glEnd();

	glBegin(GL_TRIANGLES);
	glColor3f(0.0, 1.0, 0.0);
	glVertex2i(1135, 0); //t9
	glVertex2i(1155, 190);
	glVertex2i(1180, 0);
	glEnd();
	glBegin(GL_LINE_LOOP);
	glColor3f(0.0, 0.0, 0.0);
	glVertex2i(1135, 0); //t9
	glVertex2i(1155, 190);
	glVertex2i(1180, 0);
	glEnd();

	glBegin(GL_TRIANGLES);
	glColor3f(0.0, 1.0, 0.0);
	glVertex2i(1180, 0); //10
	glVertex2i(1200, 220);
	glVertex2i(1220, 0);
	glEnd();
	glBegin(GL_LINE_LOOP);
	glColor3f(0.0, 0.0, 0.0);
	glVertex2i(1180, 0); //10
	glVertex2i(1200, 220);
	glVertex2i(1220, 0);
	glEnd();
	
}
inline void drawHill()
{
	glLineWidth(2.0);
	glColor3f(0.0, 0.0, 1.0);
	glBegin(GL_LINES);
	glVertex2f(0, 450);
	glVertex2f(1000, 450);
	glEnd();

	glBegin(GL_TRIANGLES);
	glColor3f(0.7, 0.3, 0.2);
	glVertex2i(120, 450); //t1
	glVertex2i(180, 650);
	glVertex2i(240, 450);
	glEnd();
	
	glBegin(GL_LINE_LOOP);
	glColor3f(0.0, 0.0, 0.0);
	glVertex2i(120, 450); //t1
	glVertex2i(180, 650);
	glVertex2i(240, 450);
	glEnd();

	glBegin(GL_TRIANGLES);
	glColor3f(0.7, 0.3, 0.2);
	glVertex2i(380, 450); //t4
	glVertex2i(455, 570);
	glVertex2i(520, 450);
	glEnd();
	
	glBegin(GL_LINE_LOOP);
	glColor3f(0.0, 0.0, 0.0);
	glVertex2i(380, 450); //t4
	glVertex2i(455, 570);
	glVertex2i(520, 450);
	glEnd();

	glBegin(GL_TRIANGLES);
	glColor3f(0.7, 0.3, 0.2);
	glVertex2i(460, 450); //t5
	glVertex2i(560, 660);
	glVertex2i(660, 450);
	glEnd();
	
	glBegin(GL_LINE_LOOP);
	glColor3f(0.0, 0.0, 0.0);
	glVertex2i(460, 450); //t5
	glVertex2i(560, 660);
	glVertex2i(660, 450);
	glEnd();

	glBegin(GL_TRIANGLES);
	glColor3f(0.7, 0.3, 0.2);
	glVertex2i(280, 450);//t3
	glVertex2i(360, 620);
	glVertex2i(440, 450);
	glEnd();
	
	glBegin(GL_LINE_LOOP);
	glColor3f(0.0, 0.0, 0.0);
	glVertex2i(280, 450);//t3
	glVertex2i(360, 620);
	glVertex2i(440, 450);
	glEnd();

	glBegin(GL_TRIANGLES);
	glColor3f(0.7, 0.3, 0.2);
	glVertex2i(200, 450); //t2
	glVertex2i(270, 570);
	glVertex2i(350, 450);
	glEnd();
	
	glBegin(GL_LINE_LOOP);
	glColor3f(0.0, 0.0, 0.0);
	glVertex2i(200, 450); //t2
	glVertex2i(270, 570);
	glVertex2i(350, 450);
	glEnd();

	glBegin(GL_TRIANGLES);
	glColor3f(0.7, 0.3, 0.2); //t7
	glVertex2i(660, 450);
	glVertex2i(750, 560);
	glVertex2i(840, 450);
	glEnd();
	
	glBegin(GL_LINE_LOOP);
	glColor3f(0.0, 0.0, 0.0); //t7
	glVertex2i(660, 450);
	glVertex2i(750, 560);
	glVertex2i(840, 450);
	glEnd();

	glBegin(GL_TRIANGLES);
	glColor3f(0.7, 0.3, 0.2); //t6
	glVertex2i(580, 450);
	glVertex2i(660, 630);
	glVertex2i(740, 450);
	glEnd();
	
	glBegin(GL_LINE_LOOP);
	glColor3f(0.0, 0.0, 0.0); //t6
	glVertex2i(580, 450);
	glVertex2i(660, 630);
	glVertex2i(740, 450);
	glEnd();
}

inline void drawSun(GLfloat x, GLfloat y, GLfloat radius) {
	int i;
	double const PI = 3.1416;
	int triangleAmount = 500; //# of triangles used to draw circle

	  //GLfloat radius = 0.8f; //radius
	GLfloat twicePi = 2.0f * PI;

	glBegin(GL_TRIANGLE_FAN);
	
	glVertex2f(x, y); // center of circle
	for (i = 0; i <= triangleAmount; i++) {
	glVertex2f(
	x + (radius * cos(i *  twicePi / triangleAmount)),
	y + (radius * sin(i * twicePi / triangleAmount))
	);
	}
	glEnd();



}

inline void drawClouds()
{
	// cloud 1

	for (int l = 0; l <= 20; l++)
	{
	glColor3f(1.0, 1.0, 1.0);
	drawCircle(190 + moveClouds, 580, l);

	}


	for (int l = 0; l <= 35; l++)
	{
	glColor3f(1.0, 1.0, 1.0);
	drawCircle(230 + moveClouds, 580, l);
	drawCircle(255 + moveClouds, 580, l);
	}

	for (int l = 0; l <= 20; l++)
	{
	glColor3f(1.0, 1.0, 1.0);
	drawCircle(295 + moveClouds, 580, l);
	}

	//cloud2


	for (int l = 0; l <= 20; l++)
	{
	glColor3f(1.0, 1.0, 1.0);
	drawCircle(770 + moveClouds, 615, l);
	}




	for (int l = 0; l <= 35; l++)
	{

	glColor3f(1.0, 1.0, 1.0);
	drawCircle(810 + moveClouds, 615, l);
	drawCircle(835 + moveClouds, 615, l);
	drawCircle(870 + moveClouds, 615, l);
	}

	for (int l = 0; l <= 20; l++)
	{
	glColor3f(1.0, 1.0, 1.0);
	drawCircle(900 + moveClouds, 615, l);
	}


}

inline void drawBoat()
{
	int px = 235, py = 200, w = 200, h = 50;
	glBegin(GL_POLYGON);//standop
	glColor3f(0.0, 0.0, 0.0);
	glVertex2i(px + boatPosX +105, py + 90);
	glVertex2i(px + boatPosX + 110, py + 90);
	glVertex2i(px + boatPosX + 110, py + 0);
	glVertex2i(px + boatPosX + 105, py + 0);
	glEnd();

	glBegin(GL_TRIANGLES);
	glColor3f(1.0, 1.0, 1.0);
	glVertex2i(px + boatPosX + 110, py + 90); //t1
	glVertex2i(px + boatPosX + 140, py + 70);
	glVertex2i(px + boatPosX + 110, py + 60);
	glEnd();
	glBegin(GL_POLYGON);
	glColor3f(1.0,1.0, 0.0);
	glVertex2i(px + boatPosX+5, py);
	glVertex2i(px + boatPosX + w+5, py);
	glVertex2i(px + boatPosX + w - (w / 4)+5, py - h);
	glVertex2i(px + boatPosX + (w / 4)+5, py - h);
	glEnd();

	// Under water reflection

	glBegin(GL_POLYGON);
	glColor3f(0.0, 0.0, 0.8);
	glVertex2i(px + boatPosX + (w / 8)+5, py - (h / 2));
	glVertex2i(px + boatPosX + w - (w / 8)+5, py - (h / 2));
	glVertex2i(px + boatPosX + w - (w / 5)+5, py - h);
	glVertex2i(px + boatPosX + (w / 5)+5, py - h);
	glEnd();
}


inline void drawMissionary(int px, int py)
{
	int w = characterWidth, h = characterHeight;

	glBegin(GL_POLYGON);//shoe1
	glColor3f(1.0, 0.7, 0.5);
	glVertex2i(px + 20, py + 70);
	glVertex2i(px + 40, py + 70);
	glVertex2i(px + 35, py + 80);
	glVertex2i(px + 25, py + 80);
	glEnd();

	glBegin(GL_POLYGON);//shoe2
	glColor3f(1.0, 0.7, 0.5);
	glVertex2i(px + 45, py + 70);
	glVertex2i(px + 65, py + 70);
	glVertex2i(px + 60, py + 80);
	glVertex2i(px + 50, py + 80);
	glEnd();

	glBegin(GL_POLYGON);//dress
	glColor3f(0.0, 0.0, 0.0);
	glVertex2i(px + 15, py + 80);
	glVertex2i(px + 70, py + 80);
	glVertex2i(px + 60, py + 135);
	glVertex2i(px + 25, py + 135);
	glEnd();

	glBegin(GL_POLYGON);//face
	glColor3f(1.0, 0.7, 0.5);
	glVertex2i(px + 25, py + 135);
	glVertex2i(px + 20, py + 138);
	glVertex2i(px + 20, py + 155);
	glVertex2i(px + 65, py + 155);
	glVertex2i(px + 65, py + 138);
	glVertex2i(px + 60, py + 135);
	glEnd();

	glBegin(GL_POLYGON);//hat
	glColor3f(0.0, 0.0, 0.0);
	glVertex2i(px + 20, py + 155);
	glVertex2i(px + 18, py + 175);
	glVertex2i(px + 42, py + 190);
	glVertex2i(px + 68, py + 175);
	glVertex2i(px + 65, py + 155);
	glEnd();

	glBegin(GL_POLYGON);//cross1
	glColor3f(1.0, 0.0, 0.0);
	glVertex2i(px + 40, py + 188);
	glVertex2i(px + 42, py + 190);
	glVertex2i(px + 44, py + 188);
	glVertex2i(px + 44, py + 155);
	glVertex2i(px + 40, py + 155);
	glEnd();

	glBegin(GL_POLYGON);//cross2
	glColor3f(1.0, 0.0, 0.0);
	glVertex2i(px + 20, py + 172);
	glVertex2i(px + 18, py + 175);
	glVertex2i(px + 20, py + 178);
	glVertex2i(px + 65, py + 178);
	glVertex2i(px + 68, py + 175);
	glVertex2i(px + 65, py + 172);
	glEnd();

	glBegin(GL_POLYGON);//scarf1
	glColor3f(1.0, 0.0, 0.0);
	glVertex2i(px + 25, py + 135);
	glVertex2i(px + 35, py + 135);
	glVertex2i(px + 35, py + 90);
	glVertex2i(px + 25, py + 90);
	glEnd();
	glBegin(GL_POLYGON);//scarf2
	glColor3f(1.0,0.0, 0.0);
	glVertex2i(px + 50, py + 135);
	glVertex2i(px + 60, py + 135);
	glVertex2i(px + 60, py + 90);
	glVertex2i(px + 50, py + 90);
	glEnd();

	glBegin(GL_POLYGON);//lefthand
	glColor3f(1.0, 0.7, 0.5);
	glVertex2i(px + 25, py + 135);
	glVertex2i(px + 25, py + 125);
	glVertex2i(px + 15, py + 120);
	glVertex2i(px + 15, py + 125);
	glEnd();
	glBegin(GL_POLYGON);//righthand
	glColor3f(1.0, 0.7, 0.5);
	glVertex2i(px + 60, py + 135);
	glVertex2i(px + 60, py + 125);
	glVertex2i(px + 70, py + 120);
	glVertex2i(px + 70, py + 125);
	glEnd();

	glBegin(GL_POLYGON);//lefteye
	glColor3f(0.0, 0.0, 0.0);
	glVertex2i(px + 30, py + 153);
	glVertex2i(px + 38, py + 153);
	glVertex2i(px + 38, py + 148);
	glVertex2i(px + 30, py + 148);
	glEnd();
	glBegin(GL_POLYGON);//righteye
	glColor3f(0.0, 0.0, 0.0);
	glVertex2i(px + 45, py + 153);
	glVertex2i(px + 53, py + 153);
	glVertex2i(px + 53, py + 148);
	glVertex2i(px + 45, py + 148);
	glEnd();

	glBegin(GL_POLYGON);//smile
	glColor3f(1.0, 0.0, 0.0);
	glVertex2i(px + 36, py + 143);
	glVertex2i(px + 47, py + 143);
	glVertex2i(px + 47, py + 139);
	glVertex2i(px + 36, py + 139);
	glEnd();
}

inline void drawCannibal(int px, int py)
{
	int w = characterWidth, h = characterHeight;
	glColor3f(0.0, 0.0, 0.0); //leg1 
	glBegin(GL_POLYGON);
	glVertex2i(px + 20, py + 70);
	glVertex2i(px + 30, py + 70);
	glVertex2i(px + 30, py + 110);
	glVertex2i(px + 20, py + 110);
	glEnd();

	glColor3f(0.0, 0.0, 0.0); //leg2 
	glBegin(GL_POLYGON);
	glVertex2i(px + 40, py + 70);
	glVertex2i(px + 50, py + 70);
	glVertex2i(px + 50, py + 110);
	glVertex2i(px + 40, py + 110);
	glEnd();

	glColor3f(1.0, 0.0, 0.0);//dress1
	glBegin(GL_POLYGON);
	glVertex2i(px + 30, py + 130);
	glVertex2i(px + 10, py + 120);
	glVertex2i(px + 15, py + 90);
	glVertex2i(px + 20, py + 110);

	glEnd();
	glColor3f(1.0, 1.0, 0.0);//dress2
	glBegin(GL_POLYGON);
	glVertex2i(px + 20, py + 130);
	glVertex2i(px + 20, py + 110);
	glVertex2i(px + 25, py + 90);
	glVertex2i(px + 30, py + 110);
	glVertex2i(px + 30, py + 130);
	glEnd();
	glColor3f(1.0, 0.0, 0.0);//dress3
	glBegin(GL_POLYGON);
	glVertex2i(px + 30, py + 130);
	glVertex2i(px + 30, py + 110);
	glVertex2i(px + 35, py + 90);
	glVertex2i(px + 40, py + 110);
	glVertex2i(px + 40, py + 130);
	glEnd();
	glColor3f(1.0, 1.0, 0.0);//dress4
	glBegin(GL_POLYGON);
	glVertex2i(px + 40, py + 130);
	glVertex2i(px + 40, py + 110);
	glVertex2i(px + 45, py + 90);
	glVertex2i(px + 50, py + 110);
	glVertex2i(px + 50, py + 130);
	glEnd();

	glColor3f(1.0, 0.0, 0.0);//dress5
	glBegin(GL_POLYGON);
	glVertex2i(px + 50, py + 130);
	glVertex2i(px + 50, py + 110);
	glVertex2i(px + 55, py + 90);
	glVertex2i(px + 60, py + 120);
	//glVertex2i(50, 130);
	glEnd();
	glColor3f(0.0, 0.0, 0.0);//body
	glBegin(GL_POLYGON);
	glVertex2i(px + 20, py + 125);
	glVertex2i(px + 20, py + 145);
	glVertex2i(px + 50, py + 145);
	glVertex2i(px + 50, py + 125);
	glEnd();

	glColor3f(0.0, 0.0, 0.0);//face
	glBegin(GL_POLYGON);
	glVertex2i(px + 15, py + 160);
	glVertex2i(px + 15, py + 152);
	glVertex2i(px + 20, py + 147);
	glVertex2i(px + 50, py + 147);
	glVertex2i(px + 55, py + 152);
	glVertex2i(px + 55, py + 160);
	glEnd();

	glColor3f(1.0, 0.0, 0.0);//crown1
	glBegin(GL_POLYGON);
	glVertex2i(px + 15, py + 160);
	glVertex2i(px + 5, py + 175);
	glVertex2i(px + 10, py + 185);
	glVertex2i(px + 15, py + 175);

	glEnd();

	glColor3f(1.0, 1.0, 0.0);//crown2
	glBegin(GL_POLYGON);
	glVertex2i(px + 15, py + 160);
	glVertex2i(px + 15, py + 175);
	glVertex2i(px + 20, py + 185);
	glVertex2i(px + 25, py + 175);
	glVertex2i(px + 25, py + 160);
	glEnd();

	glColor3f(1.0, 0.0, 0.0);//crown3
	glBegin(GL_POLYGON);
	glVertex2i(px + 25, py + 160);
	glVertex2i(px + 25, py + 175);
	glVertex2i(px + 30, py + 185);
	glVertex2i(px + 35, py + 175);
	glVertex2i(px + 35, py + 160);
	glEnd();

	glColor3f(1.0, 1.0, 0.0);//crown4
	glBegin(GL_POLYGON);
	glVertex2i(px + 35, py + 160);
	glVertex2i(px + 35, py + 175);
	glVertex2i(px + 40, py + 185);
	glVertex2i(px + 45, py + 175);
	glVertex2i(px + 45, py + 160);
	glEnd();

	glColor3f(1.0, 0.0, 0.0);//crown5
	glBegin(GL_POLYGON);
	glVertex2i(px + 45, py + 160);
	glVertex2i(px + 45, py + 175);
	glVertex2i(px + 50, py + 185);
	glVertex2i(px + 55, py + 175);
	glVertex2i(px + 55, py + 160);
	glEnd();

	glColor3f(1.0, 1.0, 0.0);//crown6
	glBegin(GL_POLYGON);
	glVertex2i(px + 55, py + 160);
	glVertex2i(px + 55, py + 175);
	glVertex2i(px + 60, py + 185);
	glVertex2i(px + 65, py + 175);
	glEnd();


	glColor3f(0.0, 0.0, 0.0);//lefthand
	glBegin(GL_POLYGON);
	glVertex2i(px + 20, py + 145);
	glVertex2i(px + 5, py + 130);
	glVertex2i(px + 5, py + 125);
	glVertex2i(px + 20, py + 140);
	glEnd();

	glColor3f(0.4, 0.2, 0.1);//sword
	glBegin(GL_POLYGON);
	glVertex2i(px, py + 70);
	glVertex2i(px, py + 185);
	glVertex2i(px + 5, py + 185);
	glVertex2i(px + 5, py + 70);
	glEnd();

	glColor3f(0.0, 0.7, 0.0);//sword-top
	glBegin(GL_TRIANGLES);
	glVertex2i(px, py + 185);
	glVertex2i(px + 2, py + 200);
	glVertex2i(px + 7, py + 185);
	glEnd();
}

inline void drawMissionaryOnBoat()
{
	int px = boatCharacterLeftX, py = boatCharacterY, w = characterWidth, h = characterHeight;

	if (boatMissionaryVisibility[0])
	{
	drawMissionary(px, py);
	}

	px = boatCharacterRightX;

	if (boatMissionaryVisibility[1])
	{
	drawMissionary(px + 80, py);
	}


}

inline void drawCannibalOnBoat()
{
	int px = boatCharacterLeftX, py = boatCharacterY, w = characterWidth, h = characterHeight;

	if (boatCannibalVisibility[0])
	{
	drawCannibal(px, py);
	}

	px = boatCharacterRightX;

	if (boatCannibalVisibility[1])
	{
	drawCannibal(px + 80, py);
	}


}


inline void drawCharacters()
{
	int px, py, w = characterWidth, h = characterHeight;

	px = leftCharacterPosX;
	py = leftCharacterPosY;

	if (leftCannibalVisibility[0])
	{
	drawCannibal(px, py);
	}

	if (leftCannibalVisibility[1])
	{
	drawCannibal(px + w + 5, py);
	}

	if (leftCannibalVisibility[2])
	{
	drawCannibal(px + (2 * w) + (2 * 5), py);
	}

	if (leftMissionaryVisibility[0])
	{
	drawMissionary(px + (3 * w) + (3 * 5), py);
	}

	if (leftMissionaryVisibility[1])
	{
	drawMissionary(px + (4 * w) + (4 * 5), py);
	}

	if (leftMissionaryVisibility[2])
	{
	drawMissionary(px + (5 * w) + (5 * 5), py);
	}

	px = rightCharacterPosX;
	py = rightCharacterPosY;

	if (rightMissionaryVisibility[0])
	{
	drawMissionary(px, py);
	}

	if (rightMissionaryVisibility[1])
	{
	drawMissionary(px + w + 5, py);
	}

	if (rightMissionaryVisibility[2])
	{
	drawMissionary(px + (2 * w) + (2 * 5), py);
	}

	if (rightCannibalVisibility[0])
	{
	drawCannibal(px + (3 * w) + (3 * 5), py);
	}

	if (rightCannibalVisibility[1])
	{
	drawCannibal(px + (4 * w) + (4 * 5), py);
	}

	if (rightCannibalVisibility[2])
	{
	drawCannibal(px + (5 * w) + (5 * 5), py);
	}

}



// Animation Functions

inline void animateClouds()
{
	moveClouds += cloudDeltaX;
	cout << "Cloud Animation" << endl;

	if (moveClouds == 850)
	{
	moveClouds = -900;
	}

}

inline void animateBoat()
{
	if (moveBoat == true)
	{
	cout << "Boat animation" << endl;
	boatPosX += boatDeltaX;
	boatCharacterLeftX += boatDeltaX;
	boatCharacterRightX += boatDeltaX;

	if (boatPosX == 300.0)
	{
	boatCrossed = true;
	moveBoat = false;
	riverCrossedOnce = true;
	checkGameStatus();
	}
	}

	if (moveBoatReverse == true)
	{
	boatPosX -= boatDeltaX;
	boatCharacterLeftX -= boatDeltaX;
	boatCharacterRightX -= boatDeltaX;

	if (boatPosX == 0.0)
	{
	boatCrossed = false;
	moveBoatReverse = false;
	checkGameStatus();
	}
	}

}

inline void skull1()
{
	int px = 0, py = 10;
	glLineWidth(10.0);
	glBegin(GL_LINES);
	glColor3f(1.0, 1.0, 1.0);
	glVertex2i(px + 60, py + 65);
	glVertex2i(px + 155, py + 10);
	glVertex2i(px + 155, py + 65);
	glVertex2i(px + 60, py + 10);
	glEnd();
	glBegin(GL_POLYGON);
	glColor3f(1.0, 1.0, 1.0);
	glVertex2i(px + 85, py + 35);
	glVertex2i(px + 135, py + 35);
	glVertex2i(px + 135, py + 65);
	glVertex2i(px + 85, py + 65);
	glEnd();
	glColor3f(1.0, 1.0, 1.0);
	drawSun(px + 110, py + 90, 40);//skull head

	glBegin(GL_POLYGON);//hat
	glColor3f(0.0, 0.0, 0.0);
	glVertex2i(px + 82, py + 120);
	glVertex2i(px + 80, py + 140);
	glVertex2i(px + 109, py + 155);
	glVertex2i(px + 135, py + 140);
	glVertex2i(px + 132, py + 120);
	glEnd();

	glBegin(GL_POLYGON);//cross1
	glColor3f(1.0, 0.0, 0.0);
	glVertex2i(px + 107, py + 153);
	glVertex2i(px + 109, py + 155);
	glVertex2i(px + 112, py + 153);
	glVertex2i(px + 112, py + 120);
	glVertex2i(px + 107, py + 120);
	glEnd();

	glBegin(GL_POLYGON);//cross2
	glColor3f(1.0, 0.0, 0.0);
	glVertex2i(px + 82, py + 138);
	glVertex2i(px + 80, py + 140);
	glVertex2i(px + 82, py + 142);
	glVertex2i(px + 129, py + 142);
	glVertex2i(px + 132, py + 140);
	glVertex2i(px + 129, py + 138);
	glEnd();

	glColor3f(0.0,0.0,0.0);
	drawSun(px + 95, py + 95, 10);//skull eyel
	drawSun(px + 128, py + 95, 10);//skull eyer

	glBegin(GL_TRIANGLES);
	glColor3f(0.0, 0.0, 0.0);
	glVertex2i(px + 100, py + 65);
	glVertex2i(px + 111, py + 85);
	glVertex2i(px + 121, py + 65);
	glEnd();

	glLineWidth(5.0);
	glBegin(GL_LINES);
	glColor3f(0.0, 0.0, 0.0);
	glVertex2i(px + 92, py + 50);
	glVertex2i(px + 128, py + 50);
	glEnd();
	glLineWidth(2.0);
	glBegin(GL_LINES);
	glColor3f(0.0, 0.0, 0.0);
	glVertex2i(px + 100, py + 40);
	glVertex2i(px + 100, py + 60);
	glVertex2i(px + 110, py + 40);
	glVertex2i(px + 110, py + 60);
	glVertex2i(px + 120, py + 40);
	glVertex2i(px + 120, py + 60);
	glEnd();

}

inline void skull3()
{
	int px = 850, py = 10;
	glLineWidth(10.0);
	glBegin(GL_LINES);
	glColor3f(1.0, 1.0, 1.0);
	glVertex2i(px + 60, py + 65);
	glVertex2i(px + 155, py + 10);
	glVertex2i(px + 155, py + 65);
	glVertex2i(px + 60, py + 10);
	glEnd();
	glBegin(GL_POLYGON);
	glColor3f(1.0, 1.0, 1.0);
	glVertex2i(px + 85, py + 35);
	glVertex2i(px + 135, py + 35);
	glVertex2i(px + 135, py + 65);
	glVertex2i(px + 85, py + 65);
	glEnd();
	glColor3f(1.0, 1.0, 1.0);
	drawSun(px + 110, py + 90, 40);//skull head

	glBegin(GL_POLYGON);//hat
	glColor3f(0.0, 0.0, 0.0);
	glVertex2i(px + 82, py + 120);
	glVertex2i(px + 80, py + 140);
	glVertex2i(px + 109, py + 155);
	glVertex2i(px + 135, py + 140);
	glVertex2i(px + 132, py + 120);
	glEnd();

	glBegin(GL_POLYGON);//cross1
	glColor3f(1.0, 0.0, 0.0);
	glVertex2i(px + 107, py + 153);
	glVertex2i(px + 109, py + 155);
	glVertex2i(px + 112, py + 153);
	glVertex2i(px + 112, py + 120);
	glVertex2i(px + 107, py + 120);
	glEnd();

	glBegin(GL_POLYGON);//cross2
	glColor3f(1.0, 0.0, 0.0);
	glVertex2i(px + 82, py + 138);
	glVertex2i(px + 80, py + 140);
	glVertex2i(px + 82, py + 142);
	glVertex2i(px + 129, py + 142);
	glVertex2i(px + 132, py + 140);
	glVertex2i(px + 129, py + 138);
	glEnd();

	glColor3f(0.0, 0.0, 0.0);
	drawSun(px + 95, py + 95, 10);//skull eyel
	drawSun(px + 128, py + 95, 10);//skull eyer

	glBegin(GL_TRIANGLES);
	glColor3f(0.0, 0.0, 0.0);
	glVertex2i(px + 100, py + 65);
	glVertex2i(px + 111, py + 85);
	glVertex2i(px + 121, py + 65);
	glEnd();

	glLineWidth(5.0);
	glBegin(GL_LINES);
	glColor3f(0.0, 0.0, 0.0);
	glVertex2i(px + 92, py + 50);
	glVertex2i(px + 128, py + 50);
	glEnd();
	glLineWidth(2.0);
	glBegin(GL_LINES);
	glColor3f(0.0, 0.0, 0.0);
	glVertex2i(px + 100, py + 40);
	glVertex2i(px + 100, py + 60);
	glVertex2i(px + 110, py + 40);
	glVertex2i(px + 110, py + 60);
	glVertex2i(px + 120, py + 40);
	glVertex2i(px + 120, py + 60);
	glEnd();

}
inline void skull2()
{

	int px = 150, py = -80;
	glLineWidth(5.0);
	glBegin(GL_LINES);
	glColor3f(1.0, 1.0, 1.0);
	glVertex2i(px + 60, 65);
	glVertex2i(px + 160, 10);
	glVertex2i(px + 160, 65);
	glVertex2i(px + 60, 10);
	glEnd();

	glColor3f(1.0, 1.0, 1.0);
	drawSun(px + 110, py + 90, 40);//skull head

	glBegin(GL_POLYGON);//hat
	glColor3f(0.0, 0.0, 0.0);
	glVertex2i(px + 82, py + 120);
	glVertex2i(px + 80, py + 140);
	glVertex2i(px + 109, py + 155);
	glVertex2i(px + 135, py + 140);
	glVertex2i(px + 132, py + 120);
	glEnd();

	glBegin(GL_POLYGON);//cross1
	glColor3f(1.0, 0.0, 0.0);
	glVertex2i(px + 107, py + 153);
	glVertex2i(px + 109, py + 155);
	glVertex2i(px + 112, py + 153);
	glVertex2i(px + 112, py + 120);
	glVertex2i(px + 107, py + 120);
	glEnd();

	glBegin(GL_POLYGON);//cross2
	glColor3f(1.0, 0.0, 0.0);
	glVertex2i(px + 82, py + 138);
	glVertex2i(px + 80, py + 140);
	glVertex2i(px + 82, py + 142);
	glVertex2i(px + 129, py + 142);
	glVertex2i(px + 132, py + 140);
	glVertex2i(px + 129, py + 138);
	glEnd();

	glColor3f(0.0, 0.0, 0.0);
	drawSun(px + 95, py + 95, 10);//skull eyel
	drawSun(px+128, py+95, 15);//skull eyer

	glEnd();
}

inline void drawDisk(double radius) {
	int d;
	glBegin(GL_POLYGON);
	for (d = 0; d < 32; d++) {
	double angle = 2 * PI / 32 * d;
	glVertex2d(radius*cos(angle), radius*sin(angle));
	}
	glEnd();
}
inline void drawSunn() {
	int i;
	glColor3f(1, 1, 0);
	for (i = 0; i < 13; i++) { // Draw 13 rays, with different rotations.
	glRotatef(360 / 13, 0, 0, 1); // Note that the rotations accumulate!
	glBegin(GL_LINES);
	glVertex2i(0, 0);
	glVertex2i(75, 0);
	glEnd();
	}
	drawDisk(40);
	glColor3f(0, 0, 0);
}
inline void sun()
{
	glPushMatrix();
	glTranslated(850, 600, 1);
	glRotated(-frameNumber * 0.7, 00, 0, 1);
	drawSunn();
	glPopMatrix();
	glutSwapBuffers();
}
inline void drawWindmill() {
	int i;
	glColor3f(0.0, 0.0, 0.0);
	glBegin(GL_POLYGON);
	glVertex2i(-195, 1050);
	glVertex2i(-208, 1050);
	glVertex2i(-208, 750);
	glVertex2i(-195, 750);
	glEnd();
	glTranslatef(-200, 1050, 0);//blades
	glRotated(frameNumber * (180.0 / 46), 0, 0, 1);
	glColor3f(0.0, 0.0, 0.8);
	for (i = 0; i < 3; i++) {
	glRotated(120, 0, 0, 1);  // Note: These rotations accumulate.
	glBegin(GL_POLYGON);
	glVertex2i(0, 0);
	glVertex2i(85, 10);
	glVertex2i(105, 0);
	glVertex2i(85, -10);
	glEnd();
	}


}
inline void doFrame(int v) {
	frameNumber++;
	glutPostRedisplay();
	glutTimerFunc(30, doFrame, 0);
}
inline void myDisplay(void)
{
	glClear(GL_COLOR_BUFFER_BIT);

	// Drawings

	drawBackground();
	drawWater();
	drawHill();
	drawClouds();
	grass();
	glPushMatrix();
	glTranslated(-120, 1, 0);
	glScaled(0.6, 0.6, 1);
	skull2();
	glPopMatrix();
	glPushMatrix();
	glTranslated(650, 1, 0);
	glScaled(0.6, 0.6, 1);
	skull2();
	glPopMatrix();
	glPushMatrix();
	glTranslated(20, 3, 0);
	glScaled(0.9, 0.9, 1);
	skull1();
	glPopMatrix();
	glPushMatrix();
	glTranslated(20, 3, 0);
	glScaled(0.9, 0.9, 1);
	skull3();
	glPopMatrix();
	glPushMatrix();
	glTranslated(30, 1, 0);
	glScaled(0.6, 0.6, 1);
	skull2();
	glPopMatrix();
	glPushMatrix();
	glTranslated(800, 1, 0);
	glScaled(0.6, 0.6, 1);
	skull2();
	glPopMatrix();
	drawCharacters();
	drawMissionaryOnBoat();
	drawCannibalOnBoat();

	drawBoat();
	sun();
	glPushMatrix();
	glTranslated(200, 1, 0);
	glScaled(0.6, 0.6, 1);
	drawWindmill();
	glPopMatrix();

	


	glFlush();

	// Animations

	animateBoat();
	animateClouds();





	glutPostRedisplay();

}



inline void keyboardFunc(unsigned char key, int x, int y)
{
	//-------- Boat Move --------
	if (key == 'g' || key == 'G')
	{
	cout << "Keyboard event: G or g" << endl;
	if (boatPosX == 0.0 && (boatLeftCharacter != 0 || boatRightCharacter != 0))
	{
	moveBoat = true;
	}
	glutPostRedisplay();
	}
	else if (key == 'b' || key == 'B')
	{
	cout << "Keyboard event: B or b" << endl;
	if (boatPosX == 300.0 && (boatLeftCharacter != 0 || boatRightCharacter != 0))
	{
	moveBoatReverse = true;
	}
	glutPostRedisplay();
	}
	else if (key == 'r' || key == 'R')
	{
	cout << "Keyboard event: R or r" << endl;
	resetAll();
	glutPostRedisplay();
	}

}

inline void mouseFunc(int b, int s, int x, int y) {

	if (s == GLUT_DOWN && b == GLUT_LEFT_BUTTON) {

	int px, py, w = characterWidth, h = characterHeight;

	if (!boatCrossed)
	{
	px = leftCharacterPosX;
	py = leftCharacterPosY + 150;

	if (x >= px && x <= (px + w) && y >= py && y <= (py + h) && leftCannibalVisibility[0])
	{

	cout << "Left: Cannibal 0" << endl;


	if (boatLeftCharacter == 0)
	{
	leftCannibalVisibility[0] = false;
	boatCannibalVisibility[0] = true;
	boatLeftCharacter = 1;
	}
	else if (boatRightCharacter == 0)
	{
	leftCannibalVisibility[0] = false;
	boatCannibalVisibility[1] = true;
	boatRightCharacter = 1;
	}

	cout << px << ',' << px + w << ',' << py << ',' << py + h << endl;
	cout << x << '\t' << y << endl;
	}
	else if (x >= (px + w + 5) && x <= (px + w + 5 + w) && y >= py && y <= (py + h) && leftCannibalVisibility[1])
	{
	cout << "Left: Cannibal 1" << endl;

	if (boatLeftCharacter == 0)
	{
	leftCannibalVisibility[1] = false;
	boatCannibalVisibility[0] = true;
	boatLeftCharacter = 1;
	}
	else if (boatRightCharacter == 0)
	{
	leftCannibalVisibility[1] = false;
	boatCannibalVisibility[1] = true;
	boatRightCharacter = 1;
	}
	}
	else if (x >= (px + (2 * w) + (2 * 5)) && x <= (px + (2 * w) + (2 * 5) + w) && y >= py && y <= (py + h) && leftCannibalVisibility[2])
	{
	cout << "Left: Cannibal 2" << endl;


	if (boatLeftCharacter == 0)
	{
	leftCannibalVisibility[2] = false;
	boatCannibalVisibility[0] = true;
	boatLeftCharacter = 1;
	}
	else if (boatRightCharacter == 0)
	{
	leftCannibalVisibility[2] = false;
	boatCannibalVisibility[1] = true;
	boatRightCharacter = 1;
	}
	}
	else if (x >= (px + (3 * w) + (3 * 5)) && x <= (px + (3 * w) + (3 * 5) + w) && y >= py && y <= (py + h) && leftMissionaryVisibility[0])
	{
	cout << "Left: Missionary 0" << endl;
	if (boatLeftCharacter == 0)
	{
	leftMissionaryVisibility[0] = false;
	boatMissionaryVisibility[0] = true;
	boatLeftCharacter = 2;
	leftMissionaries--;
	}
	else if (boatRightCharacter == 0)
	{
	leftMissionaryVisibility[0] = false;
	boatMissionaryVisibility[1] = true;
	boatRightCharacter = 2;
	}
	}
	else if (x >= (px + (4 * w) + (4 * 5)) && x <= (px + (4 * w) + (4 * 5) + w) && y >= py && y <= (py + h) && leftMissionaryVisibility[1])
	{
	cout << "Left: Missionary 1" << endl;

	if (boatLeftCharacter == 0)
	{
	leftMissionaryVisibility[1] = false;
	boatMissionaryVisibility[0] = true;
	boatLeftCharacter = 2;
	}
	else if (boatRightCharacter == 0)
	{
	leftMissionaryVisibility[1] = false;
	boatMissionaryVisibility[1] = true;
	boatRightCharacter = 2;
	}
	}
	else if (x >= (px + (5 * w) + (5 * 5)) && x <= (px + (5 * w) + (5 * 5) + w) && y >= py && y <= (py + h) && leftMissionaryVisibility[2])
	{
	cout << "Left: Missionary 2" << endl;

	if (boatLeftCharacter == 0)
	{
	leftMissionaryVisibility[2] = false;
	boatMissionaryVisibility[0] = true;
	boatLeftCharacter = 2;
	}
	else if (boatRightCharacter == 0)
	{
	leftMissionaryVisibility[2] = false;
	boatMissionaryVisibility[1] = true;
	boatRightCharacter = 2;
	}
	}

	if (boatLeftCharacter != 0 || boatRightCharacter != 0)
	{
	w = characterWidth, h = characterHeight;


	if (x >= boatCharacterLeftX && x <= (boatCharacterLeftX + w) && y >= (boatCharacterY + 290) && y <= (boatCharacterY + 290 + h))
	{
	cout << boatCharacterLeftX << ',' << boatCharacterLeftX + w << ',' << (boatCharacterY + 290) << ',' << (boatCharacterY + 290) + h << endl;
	cout << x << '\t' << y << endl;
	cout << "Left: Left Character On boat" << endl;

	if (boatLeftCharacter == 1)
	{
	leftCannibalVisibility[getCannibalBlankPosition()] = true;
	boatCannibalVisibility[0] = false;
	boatLeftCharacter = 0;


	}
	else if (boatLeftCharacter == 2)
	{
	leftMissionaryVisibility[getMissinaryBlankPosition()] = true;
	boatMissionaryVisibility[0] = false;
	boatLeftCharacter = 0;
	}

	}
	else if (x >= (boatCharacterRightX + 80) && x <= (boatCharacterRightX + 80 + w) && y >= (boatCharacterY + 290) && y <= (boatCharacterY + 290 + h))
	{
	cout << "Left: Right Character On boat" << endl;

	if (boatRightCharacter == 1)
	{
	leftCannibalVisibility[getCannibalBlankPosition()] = true;
	boatCannibalVisibility[1] = false;
	boatRightCharacter = 0;
	}
	else if (boatRightCharacter == 2)
	{
	leftMissionaryVisibility[getMissinaryBlankPosition()] = true;
	boatMissionaryVisibility[1] = false;
	boatRightCharacter = 0;
	}
	}

	}

	}
	else if (boatCrossed)
	{
	px = rightCharacterPosX;
	py = rightCharacterPosY + 150;

	if (x >= px && x <= (px + w) && y >= py && y <= (py + h) && rightMissionaryVisibility[0])
	{
	cout << px << ',' << px + w << ',' << py << ',' << py + h << endl;
	cout << x << '\t' << y << endl;

	cout << "Right: Missionary 0" << endl;

	if (boatLeftCharacter == 0)
	{
	rightMissionaryVisibility[0] = false;
	boatMissionaryVisibility[0] = true;
	boatLeftCharacter = 2;
	}
	else if (boatRightCharacter == 0)
	{
	rightMissionaryVisibility[0] = false;
	boatMissionaryVisibility[1] = true;
	boatRightCharacter = 2;
	}
	}
	else if (x >= (px + w + 5) && x <= (px + w + 5 + w) && y >= py && y <= (py + h) && rightMissionaryVisibility[1])
	{
	cout << "Right: Missionary 1" << endl;
	if (boatLeftCharacter == 0)
	{
	rightMissionaryVisibility[1] = false;
	boatMissionaryVisibility[0] = true;
	boatLeftCharacter = 2;
	}
	else if (boatRightCharacter == 0)
	{
	rightMissionaryVisibility[1] = false;
	boatMissionaryVisibility[1] = true;
	boatRightCharacter = 2;
	}
	}
	else if (x >= (px + (2 * w) + (2 * 5)) && x <= (px + (2 * w) + (2 * 5) + w) && y >= py && y <= (py + h) && rightMissionaryVisibility[2])
	{
	cout << "Right: Missionary 2" << endl;

	if (boatLeftCharacter == 0)
	{
	rightMissionaryVisibility[2] = false;
	boatMissionaryVisibility[0] = true;
	boatLeftCharacter = 2;
	}
	else if (boatRightCharacter == 0)
	{
	rightMissionaryVisibility[2] = false;
	boatMissionaryVisibility[1] = true;
	boatRightCharacter = 2;
	}
	}
	else if (x >= (px + (3 * w) + (3 * 5)) && x <= (px + (3 * w) + (3 * 5) + w) && y >= py && y <= (py + h) && rightCannibalVisibility[0])
	{
	cout << "Right: Cannibal 0" << endl;

	if (boatLeftCharacter == 0)
	{
	rightCannibalVisibility[0] = false;
	boatCannibalVisibility[0] = true;
	boatLeftCharacter = 1;
	}
	else if (boatRightCharacter == 0)
	{
	rightCannibalVisibility[0] = false;
	boatCannibalVisibility[1] = true;
	boatRightCharacter = 1;
	}
	}
	else if (x >= (px + (4 * w) + (4 * 5)) && x <= (px + (4 * w) + (4 * 5) + w) && y >= py && y <= (py + h) && rightCannibalVisibility[1])
	{
	cout << "Right: Cannibal 1" << endl;

	if (boatLeftCharacter == 0)
	{
	rightCannibalVisibility[1] = false;
	boatCannibalVisibility[0] = true;
	boatLeftCharacter = 1;
	}
	else if (boatRightCharacter == 0)
	{
	rightCannibalVisibility[1] = false;
	boatCannibalVisibility[1] = true;
	boatRightCharacter = 1;
	}
	}
	else if (x >= (px + (5 * w) + (5 * 5)) && x <= (px + (5 * w) + (5 * 5) + w) && y >= py && y <= (py + h) && rightCannibalVisibility[2])
	{
	cout << "Right: Cannibal 2" << endl;

	if (boatLeftCharacter == 0)
	{
	rightCannibalVisibility[2] = false;
	boatCannibalVisibility[0] = true;
	boatLeftCharacter = 1;
	}
	else if (boatRightCharacter == 0)
	{
	rightCannibalVisibility[2] = false;
	boatCannibalVisibility[1] = true;
	boatRightCharacter = 1;
	}
	}

	if (boatLeftCharacter != 0 || boatRightCharacter != 0)
	{
	w = characterWidth, h = characterHeight;


	if (x >= boatCharacterLeftX && x <= (boatCharacterLeftX + w) && y >= (boatCharacterY + 290) && y <= (boatCharacterY + 290 + h))
	{
	cout << boatCharacterLeftX << ',' << boatCharacterLeftX + w << ',' << (boatCharacterY + 290) << ',' << (boatCharacterY + 290) + h << endl;
	cout << x << '\t' << y << endl;

	cout << "Right: Left Character On boat" << endl;

	if (boatLeftCharacter == 1)
	{
	rightCannibalVisibility[getCannibalBlankPosition()] = true;
	boatCannibalVisibility[0] = false;
	boatLeftCharacter = 0;
	}
	else if (boatLeftCharacter == 2)
	{
	rightMissionaryVisibility[getMissinaryBlankPosition()] = true;
	boatMissionaryVisibility[0] = false;
	boatLeftCharacter = 0;
	}


	}
	else if (x >= (boatCharacterRightX + 80) && x <= (boatCharacterRightX + 80 + w) && y >= (boatCharacterY + 290) && y <= (boatCharacterY + 290 + h))
	{
	cout << "Right: Right Character On boat" << endl;

	if (boatRightCharacter == 1)
	{
	rightCannibalVisibility[getCannibalBlankPosition()] = true;
	boatCannibalVisibility[1] = false;
	boatRightCharacter = 0;
	}
	else if (boatRightCharacter == 2)
	{
	rightMissionaryVisibility[getMissinaryBlankPosition()] = true;
	boatMissionaryVisibility[1] = false;
	boatRightCharacter = 0;

	}
	}

	}
	}


	}

}

inline void myInit(void)
{
	glClearColor(1.0, 1.0, 1.0, 1.0);
	glColor3f(0.0f, 0.0f, 0.0f);
	glPointSize(4.0);
	glDisable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(0.0, 1000.0, 0.0, 700.0);
	glMatrixMode(GL_MODELVIEW);

}

 inline int function()
{

	PlaySound("audio.wav", NULL, SND_LOOP | SND_ASYNC);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
	glutInitWindowSize(1000, 700);
	glutInitWindowPosition((glutGet(GLUT_SCREEN_WIDTH) - 1000) / 2, (glutGet(GLUT_SCREEN_HEIGHT) - 700) / 2);
	glutCreateWindow("Missionaries and Cannibals");
	glutDisplayFunc(myDisplay);
	glutKeyboardFunc(keyboardFunc);
	glutMouseFunc(mouseFunc);
	resetAll();
	myInit();
	glutTimerFunc(200, doFrame, 0);
	glutMainLoop();
	return 0;
}