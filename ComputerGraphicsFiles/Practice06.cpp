#include <gl/freeglut.h>
#include <time.h>
#include <stdio.h>
#include <stdbool.h>

#define RECT 1

#define CLOCKWISE		1
#define ANTI_CLOCKWISE	2	

#define RIGHT			1
#define LAST_RIGHT		3
#define UP				1
#define LEFT			2
#define LAST_LEFT		4
#define DOWN			2
#define BIGGER			1
#define SMALLER			-1
#define STOP			0

#define WINDOW_WIDTH  800
#define WINDOW_HEIGHT 600


GLvoid drawScene(GLvoid);
GLvoid Reshape(int w, int h);

void timerfunction(int value);
void mousemovefunction(int button, int state, int x, int y);
void keydownfunction(unsigned char key, int x, int y);
int timeSpeed{ 1 };

bool isPaused{ false };

struct Old {
	float x{ 0 };
	float y{ 0 };
	float size{ 0 };
	float r{ 0 }, g{ 0 }, b{ 0 };
	int speed{ 0 };
	int moveXDirection{ STOP };
	int moveYDirection{ STOP };
	int sizeDirection{ STOP };
};

struct Rect {
	float x;
	float y;
	float maxsize;
	float minsize;
	float currentsize;

	float r;
	float g;
	float b;

	int sizeDirection{ BIGGER };
	int moveXDirection{ RIGHT };
	int modeDirection{ STOP };
	int moveYDirection{ UP };
	int speed{ 0 };
	bool isGenerated{ false };
	bool isModeOn{ false };
	int shape{ RECT };
	Old old;
};

Rect r[9];
int currentIndex{ 0 };


void main(int argc, char *argv[])
{
	srand((unsigned)time(NULL));
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGBA);
	glutInitWindowPosition(100, 100);
	glutInitWindowSize(WINDOW_WIDTH, WINDOW_HEIGHT);
	glutCreateWindow("Practice06");
	glutDisplayFunc(drawScene);
	glutReshapeFunc(Reshape);

	glutTimerFunc(100 - timeSpeed, timerfunction, 1);
	glutMouseFunc(mousemovefunction);
	glutKeyboardFunc(keydownfunction);

	glutMainLoop();
}

GLvoid drawScene(GLvoid)
{
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);

	for (int i = 0; i < 9; ++i) {
		if (r[i].isGenerated) {
			if (!r[i].isModeOn) {
				if (!isPaused) {
					//	기존의 움직임 (반짝이며 튕기기 이동)
					if (r[i].x - r[i].currentsize < 0) {
						r[i].x = r[i].currentsize;
						r[i].moveXDirection = RIGHT;
					}
					else if (r[i].x + r[i].currentsize > 800) {
						r[i].x = 800 - r[i].currentsize;
						r[i].moveXDirection = LEFT;
					}
					else if (r[i].y - r[i].currentsize < 0) {
						r[i].y = r[i].currentsize;
						r[i].moveYDirection = UP;
					}
					else if (r[i].y + r[i].currentsize > 600) {
						r[i].y = 600 - r[i].currentsize;
						r[i].moveYDirection = DOWN;
					}

					if (r[i].moveXDirection == RIGHT) {
						r[i].x += r[i].speed;
					}
					else {
						r[i].x -= r[i].speed;
					}

					if (r[i].moveYDirection == UP) {
						r[i].y += r[i].speed;
					}
					else {
						r[i].y -= r[i].speed;
					}


					if (r[i].sizeDirection == BIGGER) {
						r[i].currentsize += 2;
					}
					else {
						r[i].currentsize -= 2;
					}
					
				}
			}
			else {
				if (!isPaused) {
					//	시계 혹은 반시계 방향 랜덤으로 한 바퀴 이동.
					if (r[i].moveXDirection == LEFT) {
						r[i].x -= 50;
					}
					else if (r[i].moveXDirection == RIGHT) {
						r[i].x += 50;
					}
					else if (r[i].moveYDirection == DOWN) {
						r[i].y -= 50;
					}
					else if (r[i].moveYDirection == UP) {
						r[i].y += 50;
					}
					else if (r[i].moveXDirection == LAST_LEFT) {
						if (r[i].x - r[i].maxsize < r[i].old.x) {
							r[i].isModeOn = false;
							r[i].moveXDirection = r[i].old.moveXDirection;
							r[i].moveYDirection = r[i].old.moveYDirection;
							r[i].sizeDirection = r[i].old.sizeDirection;
							r[i].speed = r[i].old.speed;
							r[i].modeDirection = STOP;
						}
						r[i].x -= 50;
					}
					else if (r[i].moveXDirection == LAST_RIGHT) {
						if (r[i].x + r[i].maxsize > r[i].old.x) {
							r[i].isModeOn = false;
							r[i].moveXDirection = r[i].old.moveXDirection;
							r[i].moveYDirection = r[i].old.moveYDirection;
							r[i].sizeDirection = r[i].old.sizeDirection;
							r[i].speed = r[i].old.speed;
							r[i].modeDirection = STOP;
						}
						r[i].x += 50;
					}


					if (r[i].x - r[i].currentsize < 0) {
						r[i].x = r[i].currentsize;
						if (r[i].modeDirection == CLOCKWISE && r[i].moveXDirection==LEFT) {
							r[i].moveXDirection = STOP;
							r[i].moveYDirection = UP;
						}
						else if (r[i].modeDirection == ANTI_CLOCKWISE && r[i].moveXDirection==LEFT) {
							r[i].moveXDirection = STOP;
							r[i].moveYDirection = DOWN;
						}
						
					}
					else if (r[i].x + r[i].currentsize > 800) {
						r[i].x = 800 - r[i].currentsize;
						if (r[i].modeDirection == CLOCKWISE && r[i].moveXDirection == RIGHT) {
							r[i].moveXDirection = STOP;
							r[i].moveYDirection = DOWN;
						}
						else if (r[i].modeDirection == ANTI_CLOCKWISE && r[i].moveXDirection == RIGHT) {
							r[i].moveXDirection = STOP;
							r[i].moveYDirection = UP;
						}
						
					}
					else if (r[i].y - r[i].currentsize < 0) {
						r[i].y = r[i].currentsize;
						if (r[i].modeDirection == CLOCKWISE && r[i].moveYDirection == DOWN) {
							r[i].moveXDirection = LEFT;
							r[i].moveYDirection = STOP;
						}
						else if (r[i].modeDirection == ANTI_CLOCKWISE && r[i].moveYDirection == DOWN) {
							r[i].moveXDirection = RIGHT;
							r[i].moveYDirection = STOP;
						}
						
					}
					else if (r[i].y + r[i].currentsize + 50 > r[i].old.y) {
						r[i].y = r[i].old.y - r[i].currentsize;
						if (r[i].modeDirection == CLOCKWISE && r[i].moveYDirection==UP) {
							r[i].moveXDirection = LAST_RIGHT;
							r[i].moveYDirection = STOP;
						}
						else if (r[i].modeDirection == ANTI_CLOCKWISE && r[i].moveYDirection == UP) {
							r[i].moveXDirection = LAST_LEFT;
							r[i].moveYDirection = STOP;
						}
					}
				}
				
			}

			// draw
			if (!r[i].isModeOn) {
				glColor3f(r[i].r, r[i].g, r[i].b);
			}
			else {
				glColor3f(r[i].old.r, r[i].old.g, r[i].old.b);
			}
			if (r[i].shape == RECT) {
				glRectf(
					r[i].x - r[i].currentsize, r[i].y - r[i].currentsize,
					r[i].x + r[i].currentsize, r[i].y + r[i].currentsize
				);
			}
		}
	}

	glFlush();
}

GLvoid Reshape(int w, int h)
{
	glViewport(0, 0, w, h);
	glOrtho(0, WINDOW_WIDTH, 0, WINDOW_HEIGHT, -1.0, 1.0);
}

void timerfunction(int value)
{
	for (int i = 0; i < 9; ++i) {
		if (!r[i].isModeOn) {
			if (r[i].currentsize <= r[i].minsize) {
				r[i].currentsize = r[i].minsize;
				r[i].sizeDirection = BIGGER;
			}
			else if (r[i].currentsize >= r[i].maxsize) {
				r[i].currentsize = r[i].maxsize;
				r[i].sizeDirection = SMALLER;
			}
			r[i].r = (double)rand() / RAND_MAX;
			r[i].g = (double)rand() / RAND_MAX;
			r[i].b = (double)rand() / RAND_MAX;
		}
	}
	glutPostRedisplay();
	if (!isPaused) {
		glutTimerFunc(100 - timeSpeed, timerfunction, 1);
	}
}

void mousemovefunction(int button, int state, int x, int y)
{
	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
		if (currentIndex == 9) {
			currentIndex = 0;
		}
		r[currentIndex].x = x;
		r[currentIndex].y = WINDOW_HEIGHT - y;
		r[currentIndex].maxsize = rand() % 25 + 10;
		r[currentIndex].minsize = rand() % 5 + 10;
		r[currentIndex].currentsize = rand() % 25 + 1;
		r[currentIndex].isGenerated = true;
		r[currentIndex].speed = rand() % 10 + 1;
		r[currentIndex].moveXDirection = rand() % 2 + 1;
		r[currentIndex].moveYDirection = rand() % 2 + 1;
		r[currentIndex].shape = RECT;
		r[currentIndex].isModeOn = false;
		currentIndex++;
	}
}

void keydownfunction(unsigned char key, int x, int y)
{
	switch (key)
	{
	case 'a':
	case 'A':
		timeSpeed += 1;
		printf("%d\n", timeSpeed);
		break;
	case 'd':
	case 'D':
		timeSpeed -= 1;
		printf("%d\n", timeSpeed);
		break;

	case '1':
	case '2':
	case '3':
	case '4':
	case '5':
	case '6':
	case '7':
	case '8':
	case '9':
		if (!r[key - 49].isModeOn) {
			r[key - 49].old.x = r[key - 49].x;
			r[key - 49].old.y = r[key - 49].y;
			r[key - 49].old.size = r[key - 49].currentsize;
			r[key - 49].old.sizeDirection = r[key - 49].sizeDirection;
			r[key - 49].old.r = r[key - 49].r;
			r[key - 49].old.g = r[key - 49].g;
			r[key - 49].old.b = r[key - 49].b;
			r[key - 49].isModeOn = true;
			r[key - 49].old.moveXDirection = r[key - 49].moveXDirection;
			r[key - 49].old.moveYDirection = r[key - 49].moveYDirection;
			r[key - 49].old.speed = r[key - 49].speed;
			r[key - 49].moveXDirection = STOP;
			r[key - 49].moveYDirection = STOP;
			r[key - 49].speed = 0;
			r[key - 49].modeDirection = rand() % 2 + 1;
			if (r[key - 49].modeDirection == CLOCKWISE) {
				r[key - 49].moveXDirection = RIGHT;
				r[key - 49].moveYDirection = STOP;
			}
			else if (r[key - 49].modeDirection == ANTI_CLOCKWISE) {
				r[key - 49].moveXDirection = LEFT;
				r[key - 49].moveYDirection = STOP;
			}
			r[key - 49].y = r[key - 49].old.y;
		}
		break;

	case '0':
		for (int i = 0; i < 9; ++i) {
			if (!r[i].isModeOn) {
				r[i].old.x = r[i].x;
				r[i].old.y = r[i].y;
				r[i].old.size = r[i].currentsize;
				r[i].old.sizeDirection = r[i].sizeDirection;
				r[i].old.r = r[i].r;
				r[i].old.b = r[i].b;
				r[i].old.g = r[i].g;
				r[i].isModeOn = true;
				r[i].old.moveXDirection = r[i].moveXDirection;
				r[i].old.moveYDirection = r[i].moveYDirection;
				r[i].old.speed = r[i].speed;
				r[i].moveXDirection = STOP;
				r[i].moveYDirection = STOP;
				r[i].speed = 0;
				r[i].modeDirection = rand() % 2 + 1;
				if (r[i].modeDirection == CLOCKWISE) {
					r[i].moveXDirection = RIGHT;
					r[i].moveYDirection = STOP;
				}
				else if (r[i].modeDirection == ANTI_CLOCKWISE) {
					r[i].moveXDirection = LEFT;
					r[i].moveYDirection = STOP;
				}
				r[i].y = r[i].old.y;
			}
		}
		break;

	case 'p':
	case 'P':
		for (int i = 0; i < 9; ++i) {
			r[i].isModeOn = false;
			r[i].speed = r[i].old.speed;
			r[i].moveXDirection = r[i].old.moveXDirection;
			r[i].moveYDirection = r[i].old.moveYDirection;
			r[i].modeDirection = STOP;
			r[i].sizeDirection = r[i].old.sizeDirection;
		}
		break;

	case 's':
	case 'S':
		if (isPaused) {
			isPaused = false;
			glutTimerFunc(100 - timeSpeed, timerfunction, 1);
		}
		else {
			isPaused = true;
		}
		break;

	case 'q':
	case 'Q':
		glutLeaveMainLoop();
	default:
		break;
	}
}
