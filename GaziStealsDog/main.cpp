/*
	嘎子偷狗开发日志
	2023.6.12
		1.创建项目
		2.导入素材
		实际的开发流程
		对于初学者来说，从游戏用户界面入手

		游戏基于“EasyX”图形库开发
	2023.6.14
		1.创建游戏窗口
		2.设计游戏背景
			1)三重背景不同速度移动
			2）循环滚动的实现
		3.实现游戏背景
			1）加载背景资源
			2）渲染（坐标：图片左上角相对窗口左上角的位置）
			遇到问题：背景图片的png格式遇到黑色
			解决方式：面向复制黏贴编程，面向百度编程，引入头文件与源文件tools
	2023.6.15
		1.实现玩家的奔跑
		2.实现玩家的跳跃
		3.优化帧等待
		4.实现小乌龟随机出现
		5.结构体封装
	2023.6.16
		1.实现柱子障碍物
		2.实现碰撞检测
		3.血条，用easyX画图
	2023.6.17
		1.PS完善游戏人物，障碍物外观
		2.修改BUG
		3.优化参数
*/

/*
	构思：
	加入难度系统，不同难度速度与障碍物刷新率不同
	尝试加入二段跳
	血条美化  √
	碰撞音效美化，不同障碍物音效不同
	跳跃音效不同，跳过障碍物也有音效
	游戏结束改变背景音乐和胜利菜单
	完善ui与菜单逻辑
	增加死亡动画
	关卡，金币
*/

#define _CRT_SECURE_NO_WARNINGS
#include<stdio.h>
#include<graphics.h> //EasyX图形化界面
#include<conio.h> //判断按键输入
#include"tools.h" //写好的一些开源函数
#include<vector> //C++容器
#define WIN_WIDTH  1012
#define WIN_HEIGHT 416
#define OBSTACLE_COUNT 10 
#define WIN_SCORE 300 //胜利条件

using namespace std; //声明命名空间

IMAGE imgBgs[3]; //背景图片数组
int bgX[3]; //背景图片的初始x坐标
int bgSpeed[3] = { 2, 5, 10 }; //背景移动速度
IMAGE imgPlayers[12]; //角色奔跑的素材数组
int playerX; //角色的x坐标
int playerY; //角色的y坐标
int playerIndex; //角色奔跑的图片帧序号
bool playerJump; //判断玩家跳跃
int jumpHeightMax; //角色最大跳跃高度
int playerJumpOff; //仿重力跳跃初速度
int update; //表示是否马上需要刷新画面
int playerBlood; //玩家血量
IMAGE imgBlood; //血量爱心图片
IMAGE imgbloodEmp; //空血量爱心图片
int score; //玩家得分
IMAGE imgSZP; //"总分"图片
IMAGE imgPlayerDown[2]; //角色下蹲图片数组
bool playerDown; //表示玩家是否处于下蹲状态
IMAGE imgSZ[10]; //数字图片数组

//它用于声明一组命名的常数，当一个变量有几种可能的取值时，可以将它定义为枚举类型，本质上是整数
//typedef enum则是用来定义一个数据类型，那么该类型的变量值只能在enum定义的范围内取
typedef enum {
	DOG, //狗子 0
	ERDOG, //二哈 1
	HOOK1,
	HOOK2,
	HOOK3,
	HOOK4,
	OBSTACLE_TYPE_COUNT //边界，判断有几种枚举值 6
} obstacle_type;

vector<vector<IMAGE>>obstacleImgs; //存放所有障碍物的各个图片

typedef struct obstacle {
	int type; //障碍物的类型
	int imgIndex; //当前显示的图片的序号
	int x, y; //障碍物的坐标
	int speed; //障碍物的速度
	int power; //障碍物的杀伤力
	int grade; //通过后得分
	bool exist; //是否在窗口
	bool hited; //表示是否已经发生碰撞
	bool passed; //表示是否已经被通过
}obstacle_t;

//要在结构体后面声明
obstacle_t obstacles[OBSTACLE_COUNT]; //障碍物池
int lastObsIndex; //上一个障碍物类型

/*
游戏的初始化
*/
void init()
{
	//创建游戏窗口                       命令行窗口
	initgraph(WIN_WIDTH, WIN_HEIGHT, EX_SHOWCONSOLE);

	char name[64]; //多字符数组，用于把图片地址以字符串形式写入，再方便读取

	//加载背景资源
	for (int i = 0; i < 3; i++)
	{
		//"imgs/bg001.png"  "imgs/bg002.png"  "imgs/bg003.png"
		//sprintf 主要功能是把格式化的数据写入某个字符串中，即发送格式化输出到 string 所指向的字符串
		//运行C4996错误，将项目属性 -> C/C++ -> sdl检查调为否
		sprintf(name, "imgs/background/bg%03d.png", i + 1);
		//使用多字节字符集
		//报错 -> 项目属性 -> 配置属性 -> 高级 -> 使用多字节字符集
		loadimage(&imgBgs[i], name);
		bgX[i] = 0;
	}

	//加载Player奔跑的图片帧素材
	for (int i = 0; i < 12; i++)
	{
		//"imgs/chracter/player1.png" ... "imgs/chracter/player12.png" 
		sprintf(name, "imgs/character/player%d.png", i + 1);
		loadimage(&imgPlayers[i], name);
	}

	//设置玩家的初始位置
	playerX = WIN_WIDTH * 0.5 - imgPlayers[0].getwidth() * 0.5;
	playerY = 390 - imgPlayers[0].getheight();
	playerIndex = 0;

	playerJump = false;
	jumpHeightMax = 396 - imgPlayers[0].getheight() - 160;
	playerJumpOff = -10;

	update = true;

	//加载狗子素材
	IMAGE imgDog;
	//                                          对图片等比例放缩
	loadimage(&imgDog, "imgs/obstacle/dog.png", 70, 88, true);
	vector<IMAGE> imgDogArray;
	imgDogArray.push_back(imgDog); //在数组的尾部添加
	obstacleImgs.push_back(imgDogArray); //存入容器

	//加载二哈素材
	IMAGE imgerDog;
	vector<IMAGE> imgerDogArray;
	for (int i = 0; i < 10; i++)
	{
		sprintf(name, "imgs/obstacle/p%d.png", i + 1);
		loadimage(&imgerDog, name, 110, 100);
		imgerDogArray.push_back(imgerDog);
	}
	obstacleImgs.push_back(imgerDogArray);

	//初始化障碍物池
	for (int i = 0; i < OBSTACLE_COUNT; i++)
	{
		obstacles[i].exist = false;
	}

	//加载下蹲素材
	loadimage(&imgPlayerDown[0], "imgs/character/p1.png");
	loadimage(&imgPlayerDown[1], "imgs/character/p2.png");
	playerDown = false;

	//加载柱子素材
	IMAGE imgH;
	//vector<IMAGE>imgPillarArray;错的
	for (int i = 0; i < 4; i++)
	{
		//放在外面会导致柱子图片按帧动，就是二维数组按列储存了，在里面是按行
		vector<IMAGE> imgHookArray;
		sprintf(name, "imgs/obstacle/h%d.png", i + 1);
		loadimage(&imgH, name, 80, 260, true);
		imgHookArray.push_back(imgH);
		obstacleImgs.push_back(imgHookArray);
	}

	//玩家血量
	playerBlood = 100;
	loadimage(&imgBlood, "imgs/blood.png", 20, 20, true);
	loadimage(&imgbloodEmp, "imgs/bloodEmp.png", 20, 20, true);

	//预加载音效
	preLoadSound("imgs/hit.mp3");
	//preLoadSound("imgs/jump.mp3"); //不好听，还没找到合适的

	mciSendString("play imgs/bg.mp3 repeat", 0, 0, 0);

	lastObsIndex = -1; //上一个障碍物类型
	score = 0;

	//加载数字图片
	for (int i = 0; i < 10; i++)
	{
		sprintf(name, "imgs/sz/%d.png", i);
		loadimage(&imgSZ[i], name, 25, 30, true);
	}

	//加载“总分”图片
	sprintf(name, "imgs/sz/p.png");
	loadimage(&imgSZP, name);
}

/*
创造障碍物
*/
void createObstacle()
{
	int i;
	//找一个没有用过的
	for (i = 0; i < OBSTACLE_COUNT; i++)
	{
		if (obstacles[i].exist == false)
		{
			break;
		}
	}
	//没找到
	if (i >= OBSTACLE_COUNT)
	{
		return;
	}

	obstacles[i].exist = true;
	obstacles[i].hited = false;
	obstacles[i].imgIndex = 0;

	//优化柱子出现频率，使柱子自为一类
	obstacles[i].type = (obstacle_type)(rand() % 3);
	//优化必死障碍：二哈＋柱子
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
碰撞检测
*/
void checkHit()
{
	for (int i = 0; i < OBSTACLE_COUNT; i++)
	{
		if (obstacles[i].exist && obstacles[i].hited == false)
		{
			//图片左上和右下坐标
			int a1x, a1y, a2x, a2y;
			//模糊碰撞，偏移量
			int off = 30;
			//非下蹲
			if (!playerDown)
			{
				a1x = playerX + off;
				a1y = playerY ;
				a2x = playerX + imgPlayers[playerIndex].getwidth() - off;
				a2y = playerY + imgPlayers[playerIndex].getheight() - off;
			}
			//下蹲
			else
			{
				a1x = playerX + off;
				a1y = 375 - imgPlayerDown[playerIndex].getheight();
				a2x = playerX + imgPlayerDown[playerIndex].getwidth() - off;
				a2y = 375;
			}

			//障碍物
			IMAGE img = obstacleImgs[obstacles[i].type][obstacles[i].imgIndex];
			int b1x = obstacles[i].x + off;
			int b1y = obstacles[i].y + off;
			int b2x = obstacles[i].x + img.getwidth() - off;
			int b2y = obstacles[i].y + img.getheight();

			//工具函数，判断矩形相交
			if (rectIntersect(a1x, a1y, a2x, a2y, b1x, b1y, b2x, b2y))
			{
				playerBlood -= obstacles[i].power;
				//音效工具
				playSound("imgs/hit.mp3");
				obstacles[i].hited = true;
			}
		}
	}
}

/*
游戏不断更新图片
*/
void run()
{
	//背景
	for (int i = 0; i < 3; i++)
	{
		bgX[i] -= bgSpeed[i];
		if (bgX[i] < -WIN_WIDTH)
		{
			bgX[i] = 0;
		}
	}

	//角色跳跃
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
			playerJumpOff = -10; //回归初始态
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
	{	//不跳不蹲才奔跑
		playerIndex = (playerIndex + 1) % 12;
	}

	//静态变量，函数调用完之后不会被销毁，下次调用值不变
	static int frameCount = 0;
	//生成障碍物频率
	static int enemyFre = 100;
	frameCount++;
	if (frameCount > enemyFre)
	{
		frameCount = 0;
		enemyFre = 20 + rand() % 50;
		createObstacle();
	}

	//更新所有障碍物的坐标
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

	//碰撞检测
	checkHit();
}

/*
渲染“游戏背景”
*/
void updateBg()
{
	putimagePNG2(bgX[0], 0, &imgBgs[0]);
	putimagePNG2(bgX[1], 139, &imgBgs[1]);
	putimagePNG2(bgX[2], 20, &imgBgs[2]);
}

/*
角色跳跃
*/
void jump()
{
	playerDown = false; //使下蹲过程可以立即跳跃
	playerJump = true;
	update = true;
}
/*
角色下蹲
*/
void down()
{	//不能跳跃过程立即下蹲，角色坐标还没有恢复
	playerDown = true;
	playerIndex = 0;
	update = true;
}

/*
处理用户按键的输入
*/
void keyEvent()
{
	//scanf会阻塞程序的执行
	char ch;
	if (kbhit())	//如果有按键按下，_kbhit()返回  true
	{
		ch = getch();	//_getch()不需要按下回车即可直接读取
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
渲染“障碍物”
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
渲染“角色”
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
渲染“血条”
*/
void updateBloodBar()
{
	//位置，长宽，边框，覆盖，填充，比例
	int i;
	for (i = 0; i < (playerBlood / 10); i++) {
		//打印血条
		putimagePNG(20 + i * imgBlood.getwidth(), 10, &imgBlood);
	}
	for (; i < 10; i++) {
		//打印空血条
		putimagePNG(20 + i * imgbloodEmp.getwidth(), 10, &imgbloodEmp);
	}
}

/*
检查游戏是否结束
*/
void checkOver()
{
	if (playerBlood <= 0)
	{
		loadimage(0, "imgs/background/lose.jpg");
		mciSendString("stop imgs/bg.mp3", 0, 0, 0);
		//刷新屏幕
		FlushBatchDraw();
		system("pause");

		//暂停，或者直接开始下一局
		init();
	}
}

/*
检查得分
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
更新分数
*/
void updateScore()
{
	//将得分改为字符串，更为方便 50 => "50"   '5'     '5'-'0' == 5
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
检查胜利
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
		//getDelay工具函数，返回距离上一次调用间隔的时间（ms），第一次调用时返回为0
		timer += getDelay();
		//到达一定数值才刷新画面，相当于减速
		if (timer > 20)
		{
			timer = 0;
			update = true;
		}

		if (update)
		{
			update = false;
			//解决屏幕闪烁问题
			//这个函数用于开始批量绘图。
			//执行后，任何绘图操作都将暂时不输出到绘图窗口上。
			BeginBatchDraw();
			updateBg();
			updatePlayer();
			updateEnemy();
			updateBloodBar();
			updateScore();
			//直到执行 FlushBatchDraw 或 EndBatchDraw 才将之前的绘图输出。
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