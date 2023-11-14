/*
	����͵��������־
	2023.6.12
		1.������Ŀ
		2.�����ز�
		ʵ�ʵĿ�������
		���ڳ�ѧ����˵������Ϸ�û���������

		��Ϸ���ڡ�EasyX��ͼ�ο⿪��
	2023.6.14
		1.������Ϸ����
		2.�����Ϸ����
			1)���ر�����ͬ�ٶ��ƶ�
			2��ѭ��������ʵ��
		3.ʵ����Ϸ����
			1�����ر�����Դ
			2����Ⱦ�����꣺ͼƬ���Ͻ���Դ������Ͻǵ�λ�ã�
			�������⣺����ͼƬ��png��ʽ������ɫ
			�����ʽ�������������̣�����ٶȱ�̣�����ͷ�ļ���Դ�ļ�tools
	2023.6.15
		1.ʵ����ҵı���
		2.ʵ����ҵ���Ծ
		3.�Ż�֡�ȴ�
		4.ʵ��С�ڹ��������
		5.�ṹ���װ
	2023.6.16
		1.ʵ�������ϰ���
		2.ʵ����ײ���
		3.Ѫ������easyX��ͼ
	2023.6.17
		1.PS������Ϸ����ϰ������
		2.�޸�BUG
		3.�Ż�����
*/

/*
	��˼��
	�����Ѷ�ϵͳ����ͬ�Ѷ��ٶ����ϰ���ˢ���ʲ�ͬ
	���Լ��������
	Ѫ������  ��
	��ײ��Ч��������ͬ�ϰ�����Ч��ͬ
	��Ծ��Ч��ͬ�������ϰ���Ҳ����Ч
	��Ϸ�����ı䱳�����ֺ�ʤ���˵�
	����ui��˵��߼�
	������������
	�ؿ������
*/

#define _CRT_SECURE_NO_WARNINGS
#include<stdio.h>
#include<graphics.h> //EasyXͼ�λ�����
#include<conio.h> //�жϰ�������
#include"tools.h" //д�õ�һЩ��Դ����
#include<vector> //C++����
#define WIN_WIDTH  1012
#define WIN_HEIGHT 416
#define OBSTACLE_COUNT 10 
#define WIN_SCORE 300 //ʤ������

using namespace std; //���������ռ�

IMAGE imgBgs[3]; //����ͼƬ����
int bgX[3]; //����ͼƬ�ĳ�ʼx����
int bgSpeed[3] = { 2, 5, 10 }; //�����ƶ��ٶ�
IMAGE imgPlayers[12]; //��ɫ���ܵ��ز�����
int playerX; //��ɫ��x����
int playerY; //��ɫ��y����
int playerIndex; //��ɫ���ܵ�ͼƬ֡���
bool playerJump; //�ж������Ծ
int jumpHeightMax; //��ɫ�����Ծ�߶�
int playerJumpOff; //��������Ծ���ٶ�
int update; //��ʾ�Ƿ�������Ҫˢ�»���
int playerBlood; //���Ѫ��
IMAGE imgBlood; //Ѫ������ͼƬ
IMAGE imgbloodEmp; //��Ѫ������ͼƬ
int score; //��ҵ÷�
IMAGE imgSZP; //"�ܷ�"ͼƬ
IMAGE imgPlayerDown[2]; //��ɫ�¶�ͼƬ����
bool playerDown; //��ʾ����Ƿ����¶�״̬
IMAGE imgSZ[10]; //����ͼƬ����

//����������һ�������ĳ�������һ�������м��ֿ��ܵ�ȡֵʱ�����Խ�������Ϊö�����ͣ�������������
//typedef enum������������һ���������ͣ���ô�����͵ı���ֵֻ����enum����ķ�Χ��ȡ
typedef enum {
	DOG, //���� 0
	ERDOG, //���� 1
	HOOK1,
	HOOK2,
	HOOK3,
	HOOK4,
	OBSTACLE_TYPE_COUNT //�߽磬�ж��м���ö��ֵ 6
} obstacle_type;

vector<vector<IMAGE>>obstacleImgs; //��������ϰ���ĸ���ͼƬ

typedef struct obstacle {
	int type; //�ϰ��������
	int imgIndex; //��ǰ��ʾ��ͼƬ�����
	int x, y; //�ϰ��������
	int speed; //�ϰ�����ٶ�
	int power; //�ϰ����ɱ����
	int grade; //ͨ����÷�
	bool exist; //�Ƿ��ڴ���
	bool hited; //��ʾ�Ƿ��Ѿ�������ײ
	bool passed; //��ʾ�Ƿ��Ѿ���ͨ��
}obstacle_t;

//Ҫ�ڽṹ���������
obstacle_t obstacles[OBSTACLE_COUNT]; //�ϰ����
int lastObsIndex; //��һ���ϰ�������

/*
��Ϸ�ĳ�ʼ��
*/
void init()
{
	//������Ϸ����                       �����д���
	initgraph(WIN_WIDTH, WIN_HEIGHT, EX_SHOWCONSOLE);

	char name[64]; //���ַ����飬���ڰ�ͼƬ��ַ���ַ�����ʽд�룬�ٷ����ȡ

	//���ر�����Դ
	for (int i = 0; i < 3; i++)
	{
		//"imgs/bg001.png"  "imgs/bg002.png"  "imgs/bg003.png"
		//sprintf ��Ҫ�����ǰѸ�ʽ��������д��ĳ���ַ����У������͸�ʽ������� string ��ָ����ַ���
		//����C4996���󣬽���Ŀ���� -> C/C++ -> sdl����Ϊ��
		sprintf(name, "imgs/background/bg%03d.png", i + 1);
		//ʹ�ö��ֽ��ַ���
		//���� -> ��Ŀ���� -> �������� -> �߼� -> ʹ�ö��ֽ��ַ���
		loadimage(&imgBgs[i], name);
		bgX[i] = 0;
	}

	//����Player���ܵ�ͼƬ֡�ز�
	for (int i = 0; i < 12; i++)
	{
		//"imgs/chracter/player1.png" ... "imgs/chracter/player12.png" 
		sprintf(name, "imgs/character/player%d.png", i + 1);
		loadimage(&imgPlayers[i], name);
	}

	//������ҵĳ�ʼλ��
	playerX = WIN_WIDTH * 0.5 - imgPlayers[0].getwidth() * 0.5;
	playerY = 390 - imgPlayers[0].getheight();
	playerIndex = 0;

	playerJump = false;
	jumpHeightMax = 396 - imgPlayers[0].getheight() - 160;
	playerJumpOff = -10;

	update = true;

	//���ع����ز�
	IMAGE imgDog;
	//                                          ��ͼƬ�ȱ�������
	loadimage(&imgDog, "imgs/obstacle/dog.png", 70, 88, true);
	vector<IMAGE> imgDogArray;
	imgDogArray.push_back(imgDog); //�������β�����
	obstacleImgs.push_back(imgDogArray); //��������

	//���ض����ز�
	IMAGE imgerDog;
	vector<IMAGE> imgerDogArray;
	for (int i = 0; i < 10; i++)
	{
		sprintf(name, "imgs/obstacle/p%d.png", i + 1);
		loadimage(&imgerDog, name, 110, 100);
		imgerDogArray.push_back(imgerDog);
	}
	obstacleImgs.push_back(imgerDogArray);

	//��ʼ���ϰ����
	for (int i = 0; i < OBSTACLE_COUNT; i++)
	{
		obstacles[i].exist = false;
	}

	//�����¶��ز�
	loadimage(&imgPlayerDown[0], "imgs/character/p1.png");
	loadimage(&imgPlayerDown[1], "imgs/character/p2.png");
	playerDown = false;

	//���������ز�
	IMAGE imgH;
	//vector<IMAGE>imgPillarArray;���
	for (int i = 0; i < 4; i++)
	{
		//��������ᵼ������ͼƬ��֡�������Ƕ�ά���鰴�д����ˣ��������ǰ���
		vector<IMAGE> imgHookArray;
		sprintf(name, "imgs/obstacle/h%d.png", i + 1);
		loadimage(&imgH, name, 80, 260, true);
		imgHookArray.push_back(imgH);
		obstacleImgs.push_back(imgHookArray);
	}

	//���Ѫ��
	playerBlood = 100;
	loadimage(&imgBlood, "imgs/blood.png", 20, 20, true);
	loadimage(&imgbloodEmp, "imgs/bloodEmp.png", 20, 20, true);

	//Ԥ������Ч
	preLoadSound("imgs/hit.mp3");
	//preLoadSound("imgs/jump.mp3"); //����������û�ҵ����ʵ�

	mciSendString("play imgs/bg.mp3 repeat", 0, 0, 0);

	lastObsIndex = -1; //��һ���ϰ�������
	score = 0;

	//��������ͼƬ
	for (int i = 0; i < 10; i++)
	{
		sprintf(name, "imgs/sz/%d.png", i);
		loadimage(&imgSZ[i], name, 25, 30, true);
	}

	//���ء��ܷ֡�ͼƬ
	sprintf(name, "imgs/sz/p.png");
	loadimage(&imgSZP, name);
}

/*
�����ϰ���
*/
void createObstacle()
{
	int i;
	//��һ��û���ù���
	for (i = 0; i < OBSTACLE_COUNT; i++)
	{
		if (obstacles[i].exist == false)
		{
			break;
		}
	}
	//û�ҵ�
	if (i >= OBSTACLE_COUNT)
	{
		return;
	}

	obstacles[i].exist = true;
	obstacles[i].hited = false;
	obstacles[i].imgIndex = 0;

	//�Ż����ӳ���Ƶ�ʣ�ʹ������Ϊһ��
	obstacles[i].type = (obstacle_type)(rand() % 3);
	//�Ż������ϰ�������������
	if (lastObsIndex >= 0 &&
		obstacles[lastObsIndex].type >= HOOK1 &&
		obstacles[lastObsIndex].type <= HOOK4 &&
		obstacles[i].type == ERDOG &&
		obstacles[lastObsIndex].x > (WIN_WIDTH - 500))
	{
		lastObsIndex = i;
	}

	if (obstacles[i].type == HOOK1)
	{
		obstacles[i].type += rand() % 4; // 0..3
	}

	obstacles[i].x = WIN_WIDTH;
	obstacles[i].y = 390 - obstacleImgs[obstacles[i].type][0].getheight();

	if (obstacles[i].type == DOG)
	{
		obstacles[i].speed = 0;
		obstacles[i].power = 10;
		obstacles[i].grade = 5;
	}
	else if (obstacles[i].type == ERDOG)
	{
		obstacles[i].speed = 2;
		obstacles[i].power = 20;
		obstacles[i].grade = 18;
	}
	else if (obstacles[i].type >= HOOK1 && obstacles[i].type <= HOOK4)
	{
		obstacles[i].speed = 0;
		obstacles[i].power = 10;
		obstacles[i].y = 0;
		obstacles[i].grade = 6;
	}

	obstacles[i].passed = false;
}


/*
��ײ���
*/
void checkHit()
{
	for (int i = 0; i < OBSTACLE_COUNT; i++)
	{
		if (obstacles[i].exist && obstacles[i].hited == false)
		{
			//ͼƬ���Ϻ���������
			int a1x, a1y, a2x, a2y;
			//ģ����ײ��ƫ����
			int off = 30;
			//���¶�
			if (!playerDown)
			{
				a1x = playerX + off;
				a1y = playerY ;
				a2x = playerX + imgPlayers[playerIndex].getwidth() - off;
				a2y = playerY + imgPlayers[playerIndex].getheight() - off;
			}
			//�¶�
			else
			{
				a1x = playerX + off;
				a1y = 375 - imgPlayerDown[playerIndex].getheight();
				a2x = playerX + imgPlayerDown[playerIndex].getwidth() - off;
				a2y = 375;
			}

			//�ϰ���
			IMAGE img = obstacleImgs[obstacles[i].type][obstacles[i].imgIndex];
			int b1x = obstacles[i].x + off;
			int b1y = obstacles[i].y + off;
			int b2x = obstacles[i].x + img.getwidth() - off;
			int b2y = obstacles[i].y + img.getheight();

			//���ߺ������жϾ����ཻ
			if (rectIntersect(a1x, a1y, a2x, a2y, b1x, b1y, b2x, b2y))
			{
				playerBlood -= obstacles[i].power;
				//��Ч����
				playSound("imgs/hit.mp3");
				obstacles[i].hited = true;
			}
		}
	}
}

/*
��Ϸ���ϸ���ͼƬ
*/
void run()
{
	//����
	for (int i = 0; i < 3; i++)
	{
		bgX[i] -= bgSpeed[i];
		if (bgX[i] < -WIN_WIDTH)
		{
			bgX[i] = 0;
		}
	}

	//��ɫ��Ծ
	if (playerJump)
	{
		//playSound("imgs/jump.mp3");
		playerIndex = 0;
		if (playerY < jumpHeightMax)
		{
			playerJumpOff = 10;
		}

		playerY += playerJumpOff;

		if (playerY > 386 - imgPlayers[0].getheight())
		{
			playerJump = false;
			playerJumpOff = -10; //�ع��ʼ̬
		}
	}
	else if (playerDown)
	{
		static int count = 0;
		int delays[2] = { 1, 18 };
		count++;
		if (count >= delays[playerIndex])
		{
			count = 0;
			playerIndex++;
			if (playerIndex >= 2)
			{
				playerIndex = 0;
				playerDown = false;
			}
		}
	}
	else
	{	//�������ײű���
		playerIndex = (playerIndex + 1) % 12;
	}

	//��̬����������������֮�󲻻ᱻ���٣��´ε���ֵ����
	static int frameCount = 0;
	//�����ϰ���Ƶ��
	static int enemyFre = 100;
	frameCount++;
	if (frameCount > enemyFre)
	{
		frameCount = 0;
		enemyFre = 20 + rand() % 50;
		createObstacle();
	}

	//���������ϰ��������
	for (int i = 0; i < OBSTACLE_COUNT; i++)
	{
		if (obstacles[i].exist)
		{
			obstacles[i].x -= obstacles[i].speed + bgSpeed[2];
			if (obstacles[i].x < -obstacleImgs[obstacles[i].type][0].getwidth() * 2)
			{
				obstacles[i].exist = false;
			}

			int len = obstacleImgs[obstacles[i].type].size();
			obstacles[i].imgIndex = (obstacles[i].imgIndex + 1) % len;
		}
	}

	//��ײ���
	checkHit();
}

/*
��Ⱦ����Ϸ������
*/
void updateBg()
{
	putimagePNG2(bgX[0], 0, &imgBgs[0]);
	putimagePNG2(bgX[1], 139, &imgBgs[1]);
	putimagePNG2(bgX[2], 20, &imgBgs[2]);
}

/*
��ɫ��Ծ
*/
void jump()
{
	playerDown = false; //ʹ�¶׹��̿���������Ծ
	playerJump = true;
	update = true;
}
/*
��ɫ�¶�
*/
void down()
{	//������Ծ���������¶ף���ɫ���껹û�лָ�
	playerDown = true;
	playerIndex = 0;
	update = true;
}

/*
�����û�����������
*/
void keyEvent()
{
	//scanf�����������ִ��
	char ch;
	if (kbhit())	//����а������£�_kbhit()����  true
	{
		ch = getch();	//_getch()����Ҫ���»س�����ֱ�Ӷ�ȡ
		if (ch == 'w')
		{
			jump();
		}
		else if (ch == 's')
		{
			down();
		}
	}

}

/*
��Ⱦ���ϰ��
*/
void updateEnemy()
{
	for (int i = 0; i < OBSTACLE_COUNT; i++)
	{
		if (obstacles[i].exist)
		{
			putimagePNG2(obstacles[i].x, obstacles[i].y, WIN_WIDTH,
				&obstacleImgs[obstacles[i].type][obstacles[i].imgIndex]);
		}
	}
}

/*
��Ⱦ����ɫ��
*/
void updatePlayer()
{
	if (!playerDown)
	{
		putimagePNG2(playerX, playerY, &imgPlayers[playerIndex]);
	}
	else
	{
		int y = 416 - imgPlayerDown[playerIndex].getheight();
		putimagePNG2(playerX, y, &imgPlayerDown[playerIndex]);
	}
}

/*
��Ⱦ��Ѫ����
*/
void updateBloodBar()
{
	//λ�ã������߿򣬸��ǣ���䣬����
	int i;
	for (i = 0; i < (playerBlood / 10); i++) {
		//��ӡѪ��
		putimagePNG(20 + i * imgBlood.getwidth(), 10, &imgBlood);
	}
	for (; i < 10; i++) {
		//��ӡ��Ѫ��
		putimagePNG(20 + i * imgbloodEmp.getwidth(), 10, &imgbloodEmp);
	}
}

/*
�����Ϸ�Ƿ����
*/
void checkOver()
{
	if (playerBlood <= 0)
	{
		loadimage(0, "imgs/background/lose.jpg");
		mciSendString("stop imgs/bg.mp3", 0, 0, 0);
		//ˢ����Ļ
		FlushBatchDraw();
		system("pause");

		//��ͣ������ֱ�ӿ�ʼ��һ��
		init();
	}
}

/*
���÷�
*/
void checkScore()
{
	for (int i = 0; i < OBSTACLE_COUNT; i++)
	{
		if (obstacles[i].exist &&
			obstacles[i].passed == false && obstacles[i].hited == false &&
			obstacles[i].x + obstacleImgs[obstacles[i].type][0].getwidth() < playerX)
		{
			score += obstacles[i].grade;
			obstacles[i].passed = true;
		}
	}
}

/*
���·���
*/
void updateScore()
{
	//���÷ָ�Ϊ�ַ�������Ϊ���� 50 => "50"   '5'     '5'-'0' == 5
	char str[8];
	sprintf(str, "%d", score);

	putimagePNG(850, 15, &imgSZP);

	int x = 920;
	int y = 15;
	for (int i = 0; str[i]; i++)
	{
		int sz = str[i] - '0';
		putimagePNG(x, y, &imgSZ[sz]);
		x += imgSZ[sz].getwidth() + 5;
	}
}

/*
���ʤ��
*/
void checkWin()
{
	if (score >= WIN_SCORE)
	{
		loadimage(0, "imgs/background/win.jpg");
		mciSendString("play imgs/win.mp3", 0, 0, 0);
		FlushBatchDraw();
		Sleep(1000);
		FlushBatchDraw();
		system("pause");

		init();
		/*playerBlood = 100;
		score = 0;
		mciSendString("play imgs/bg.mp3 repeat", 0, 0, 0);*/
	}
}

int main(void)
{
	init();

	loadimage(0, "imgs/background/startbg.jpg");
	system("pause");

	int timer = 0;
	while (1)
	{
		keyEvent();
		//getDelay���ߺ��������ؾ�����һ�ε��ü����ʱ�䣨ms������һ�ε���ʱ����Ϊ0
		timer += getDelay();
		//����һ����ֵ��ˢ�»��棬�൱�ڼ���
		if (timer > 20)
		{
			timer = 0;
			update = true;
		}

		if (update)
		{
			update = false;
			//�����Ļ��˸����
			//����������ڿ�ʼ������ͼ��
			//ִ�к��κλ�ͼ����������ʱ���������ͼ�����ϡ�
			BeginBatchDraw();
			updateBg();
			updatePlayer();
			updateEnemy();
			updateBloodBar();
			updateScore();
			//ֱ��ִ�� FlushBatchDraw �� EndBatchDraw �Ž�֮ǰ�Ļ�ͼ�����
			EndBatchDraw();

			checkOver();
			checkScore();
			checkWin();
			run();
		}
	}
	system("pause");
	return 0;
}