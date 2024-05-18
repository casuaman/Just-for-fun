#include <graphics.h>
#include <conio.h>
#include <mmsystem.h>
#include <time.h>
//#pragma comment(lib,"Winmm.lib")
typedef struct q {
	int lei = 0;
	int num = 0;
	int discovered = 0;
	int flag = 0;
}q;
void Output_initial_interface();//生成初始界面
void add(int x, int y, q game[14][16]);//计算3*3区域内的雷的个数
int output(int x, int y, q game[14][16]);//用于点击后的输出
boolean game();//游戏开始
MOUSEMSG m;
IMAGE img;
int main() {
	initgraph(350, 450);
	while (1)
	{
		game();
	}
	closegraph();
	return 0;
}
void Output_initial_interface()
{

	setbkcolor(RGB(204, 204, 204));
	cleardevice();
	loadimage(&img, "playing.png", 25, 25, true);
	putimage(163, 12, &img);
	loadimage(&img, "cover.png", 25, 25, true);
	for (int i = 0; i < 14; i++)
		for (int j = 0; j < 16; j++)
		{
			putimage(25 * i, 25 * j + 50, &img);
		}
}
boolean game()
{
	Output_initial_interface();//生成初始界面
	q game1[14][16];//创建扫雷数组
	srand((unsigned int)time(NULL));//用于随机生成位置
	int discovered = 0;//已扫格数
	int flag = 0;//插旗数
	int status = 0;//扫雷状态：0为进行中；-1为失败；1为成功
	while ((m = GetMouseMsg()).uMsg != WM_LBUTTONDOWN);
	
	int x = m.x / 25;
	int y = (m.y - 50) / 25;
	for (int i = 0, x1, y1; i < 20;)//随机放置20个雷
	{

		x1 = rand() % 14;
		y1 = rand() % 16;
		if (x1 - x >= -1 && x1 - x <= 1 && y1 - y >= -1 && y1 - y <= 1)
			continue;
		if (game1[x1][y1].lei)
			continue;
		else
			game1[x1][y1].lei = 1;
		i++;
	}
	for (int i = 0; i < 14; i++)//计算3*3区域内的雷的个数
		for (int j = 0; j < 16; j++)
		{
			add(i, j, game1);
		}
	int t = 0;
	while (1) {

		if (t == 0)
			t++;
		else
			while ((m = GetMouseMsg()).uMsg != WM_LBUTTONUP && m.uMsg != WM_RBUTTONUP && m.uMsg != WM_RBUTTONDOWN && m.uMsg != WM_LBUTTONDOWN);//获取点击信息
		if (m.y < 50)//点击状态栏区域
		{
			if (m.x >= 163 && m.x <= 188 && m.y >= 12 && m.y <= 37)
			{
				if (m.uMsg == WM_LBUTTONDOWN || m.uMsg == WM_RBUTTONDOWN)
				{
					switch (status)
					{
					case 0:loadimage(&img, "playing_down.png", 25, 25, true); break;
					case 1:loadimage(&img, "win_down.png", 25, 25, true); break;
					case -1:loadimage(&img, "lose_down.png", 25, 25, true); break;
					}
					putimage(163, 12, &img);
				}
				if (m.uMsg == WM_LBUTTONUP || m.uMsg == WM_RBUTTONUP)
					return true;
			}	
		}
		else//点击雷区
		{
			x = m.x / 25;
			y = (m.y - 50) / 25;
			if (game1[x][y].discovered == 1)//点击已发现部分
				continue;
			//点击未发现部分
			if (m.uMsg == WM_LBUTTONDOWN)//左键按下
			{
				if (game1[x][y].flag == 1)//已标记点击无效
					continue;
				if (game1[x][y].lei == 0)//无雷点击
				{
					;
					discovered += output(x, y, game1);
				}

				else//暴雷
				{
					output(x, y, game1);
					status = -1;//失败
					loadimage(&img, "lose.png", 25, 25, true);
					putimage(163, 12, &img);
				}
			}
			if (m.uMsg == WM_RBUTTONDOWN)//右键按下插旗和撤销插旗
			{
				if (game1[x][y].flag == 0)//插旗
				{
					if (flag < 20)
					{
						game1[x][y].flag = 1;
						flag++;
						loadimage(&img, "flag.png", 25, 25, true);
						putimage(25 * x, 25 * y + 50, &img);
					}
				}
				else//撤销插旗
				{
					game1[x][y].flag = 0;
					flag--;
					loadimage(&img, "cover.png", 25, 25, true);
					putimage(25 * x, 25 * y + 50, &img);
				}
			}
			if (discovered == 14 * 16 - 20)//成功
			{
				status = 1;
				loadimage(&img, "win.png", 25, 25, true);
				putimage(163, 12, &img);
				for (int i = 0; i < 14; i++)
					for (int j = 0; j < 16; j++)
					{
						if (game1[i][j].lei == 1)
						{
							loadimage(&img, "flag.png", 25, 25, true);
							putimage(25 * i, 25 * j + 50, &img);
						}
					}
			}
		}
	}
}
void add(int x, int y, q game[14][16])
{
	for (int i = x - 1; i <= x + 1; i++)
		for (int j = y - 1; j <= y + 1; j++)
		{
			if (i >= 0 && i < 14 && j >= 0 && j < 16)
			{
				game[x][y].num += game[i][j].lei;
			}
		}
}
int output(int x, int y, q game[14][16])
{
	int discovered = 0;
	if (game[x][y].lei == 0)
	{
		if (game[x][y].num == 0)
		{
			loadimage(&img, "0.png", 25, 25, true);
			putimage(25 * x, 25 * y + 50, &img);
			game[x][y].discovered = 1;
			discovered++;
			for (int i = x - 1; i <= x + 1; i++)
				for (int j = y - 1; j <= y + 1; j++)
				{
					if (i >= 0 && i < 14 && j >= 0 && j < 16 && (!(i == x && j == y)) && game[i][j].discovered == 0)
						discovered += output(i, j, game);
				}
		}
		else
		{
			switch (game[x][y].num) {
			case 1:loadimage(&img, "1.png", 25, 25, true); break;
			case 2:loadimage(&img, "2.png", 25, 25, true); break;
			case 3:loadimage(&img, "3.png", 25, 25, true); break;
			case 4:loadimage(&img, "4.png", 25, 25, true); break;
			case 5:loadimage(&img, "5.png", 25, 25, true); break;
			case 6:loadimage(&img, "6.png", 25, 25, true); break;
			case 7:loadimage(&img, "7.png", 25, 25, true); break;
			case 8:loadimage(&img, "8.png", 25, 25, true); break;
			}
			putimage(25 * x, 25 * y + 50, &img);
			game[x][y].discovered = 1;
			discovered++;
		}
	}
	else
	{
		loadimage(&img, "10.png", 25, 25, true);
		for (int i = 0; i < 14; i++)
			for (int j = 0; j < 16; j++)
			{
				game[i][j].discovered = 1;
				if (game[i][j].lei == 1)
				{
					putimage(25 * i, 25 * j + 50, &img);
				}
			}
		loadimage(&img, "9.png", 25, 25, true);
		putimage(25 * x, 25 * y + 50, &img);
		game[x][y].discovered = 1;
	}
	return discovered;
}



