/*
*作者：野漫横江
*完成日期：2020.1.11
*项目：《扫雷》
*说明：玩游戏是一件容易的事情，制作游戏实在不是那种简单的活，虽然这已经时第二次开工了，但依旧会在如何实现某一功能上烧脑不断
*			暑假时，这个项目是我的新生作业，为了实现能调整不同难度的功能，由于当时并不会动态申请内存，只能硬生生定义了6个数组变量，
*			用了整整660行代码才完工，而且也并不好管理，经过一个学期的学习，懂得了如何动态申请内存，也明白了如何实现申请二维数组的内存，
*			于是决定重新再来一遍，用了3天时间，才有了这个对于我来说全新的扫雷的出现，这也算是我对这个学期以来所学知识的巩固了。
*/
#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <cstring>
#include <graphics.h>
#include <iostream>
#include <windows.h>
char* tips_init = "请选择需要挑战的难度：\n1.简单\t2.一般\t3.困难\n请输入对应数字：";
char* tips_err = "您输入的格式有误，请重新输入！";
int Tips();//输出提示，返回难度信息level
int Boom;//定义一个雷个数的变量
int Len;//定义长度，将在Init_Map中赋值初始化
int size = 16;//图片大小
int count = 0;//判断胜负依据
int** Init_Map(int level);//初始化地图（设置格子的个数，雷的个数）
void Random_map(int** p_map);//在地图中布雷
void LoadImg_Windows();	//加载图片（加载图形化界面窗口）,窗口句柄（hwnd）在次函数中初始化赋值
void Print_Img_Map(int** p_map);//输出图片地图，刷新图片
int PalyGame(int** p_map);//鼠标事件，返回值为鼠标左单击后图片的数值，雷的数值为-1
void Open_Zero(int** p_map, int x, int y);//使用递归，自动翻开0周围非0的格子，遍历0周围九宫格，在九宫格找到0后递归之前操作
void DeleteArr(int** p_map, int* len);//释放内存
IMAGE img[12];
HWND hwnd;//窗口句柄
int main()
{
	int* len = &Len;//指向等级变量的指针
	int* boom = &Boom;//指向雷变量的指针
	int** p_map;//指向地图的二级指针
	int* p_size = &size;//指向图片大小
	p_map = Init_Map(Tips());//指向地图的地址
	LoadImg_Windows();//加载图片（加载图形化界面窗口）,窗口句柄（hwnd）在此函数中被初始化赋值
	Random_map(p_map);
	while (1)
	{
		Print_Img_Map(p_map);//刷新图片
		if (PalyGame(p_map) == -1)
		{
			Print_Img_Map(p_map);
			if (MessageBox(hwnd, L"你太菜啦！需要重新选择难度重来一局吗？", L"Tips:", MB_RETRYCANCEL) == 4)
			{
				closegraph();
				DeleteArr(p_map, len);
				return main();
			}
			closegraph();
			return 0;
		}
		if ((*len) * (*len) - (*boom) == count)
		{
			if (MessageBox(hwnd, L"恭喜你！获胜了！需要重新选择难度继续挑战吗？", L"Tips:", MB_OKCANCEL) == 1)
			{
				closegraph();
				DeleteArr(p_map, len);
				return main();
			}
			closegraph();
			DeleteArr(p_map, len);
			return 0;
		}
	}
}
int Tips()																//输出提示，并获取难度信息
{
	int level;//声明难度等级变量
	std::cout << tips_init;//输出提示信息
	std::cin >> level;//获取难度信息
	if ((int)level > 3 || (int)level < 1)//判断格式
	{
		std::cout << tips_err;
		return Tips();
	}
	return (int)level;
}
int** Init_Map(int level)//初始化地图
{
	int* len = &Len;
	int* p_Boom = NULL;
	p_Boom = &Boom;
	switch (level)//根据用户所选等级来初始化地图大小以及雷的个数
	{
	case 1:
		*len = 10;
		*p_Boom = 10;
		break;
	case 2:
		*len = 20;
		*p_Boom = 50;
		break;
	case 3:
		*len = 30;
		*p_Boom = 100;
		break;
	}

	int** p_map = new int* [*len + 2];//开辟大小为len的内存空间指向一维指针,+2是为了定义辅助区
	for (int i = 0; i < *len + 2; i++)
	{
		p_map[i] = new int[*len + 2];
	}																		//将每一个一维指针都开辟大小为len的内存空间，实现了动态创建二维数组
	for (int i = 0; i < *len + 2; i++)//将二维数组初始化
	{
		for (int j = 0; j < *len + 2; j++)
		{
			*(p_map[i] + j) = 0;
		}
	}
	return p_map;//返回地图指针的地址，使用new申请的地址若不主动delete则在函数调用结束后不会被释放，即可作为返回值传递
}
void Random_map(int** p_map)
{
	int* p_size = &size;
	srand((unsigned int)time(NULL));
	int x;
	int y;
	int* len = &Len;
	const int* p_boom = &Boom;
	int count = 0;
	//std::cout << p_map;
	while (count < *p_boom)//布雷算法
	{
		x = (int)((rand() % (*len)) + 1);
		y = (int)((rand() % (*len)) + 1);
		if (*(p_map[x] + y) == 0)
		{
			*(p_map[x] + y) = -1;//随机赋雷
			count++;
		}
	}
	for (int i = 1; i <= *len; i++)
	{
		for (int j = 1; j <= *len; j++)
		{
			if (*(p_map[i] + j) == 0)
			{
				for (int m = i - 1; m <= i + 1; m++)
				{
					for (int n = j - 1; n <= j + 1; n++)
					{
						if (*(p_map[m] + n) == -1)
						{
							*(p_map[i] + j) = *(p_map[i] + j) + 1;
						}
					}
				}
			}
		}
	}
	///*
	for (int j = 1; j <= *len; j++)//加密
	{
		for (int i = 1; i <= *len; i++)
		{
			*(p_map[i] + j) = *(p_map[i] + j) + 20;
			//std::cout << *(p_map[i] + j);
		}
		//std::cout << std::endl;
	}
	//*/
}
void Print_Img_Map(int** p_map)
{
	int* len = &Len;
	int* p_size = &size;
	for (int j = 1; j <= *len; j++)//转换x,y轴
	{
		for (int i = 1; i <= *len; i++)
		{
			printf_s("%2d", *(p_map[i] + j));
			if (*(p_map[i] + j) == -1)
			{
				putimage((i - 1) * (*p_size), (j - 1) * (*p_size), &img[9]);
			}
			else if (*(p_map[i] + j) >= 0 && *(p_map[i] + j) <= 8)
			{
				putimage((i - 1) * (*p_size), (j - 1) * (*p_size), &img[*(p_map[i] + j)]);
			}
			else if (*(p_map[i] + j) >= 19 && *(p_map[i] + j) <= 28)
			{
				putimage((i - 1) * (*p_size), (j - 1) * (*p_size), &img[10]);
			}
			else if (*(p_map[i] + j) >= 29)
			{
				putimage((i - 1) * (*p_size), (j - 1) * (*p_size), &img[11]);
			}
			//std::cout << *(p_map[i] + j);
		}
		std::cout << std::endl;
	}
}
void LoadImg_Windows()
{
	int* p_size = &size;														//img_size
	HWND* p_hwnd = &hwnd;
	*p_hwnd = initgraph(Len * size, Len * size, SHOWCONSOLE);
	loadimage(&img[0], L"0.bmp", *p_size, *p_size);//button_onclick
	loadimage(&img[1], L"1.bmp", *p_size, *p_size);
	loadimage(&img[2], L"2.bmp", *p_size, *p_size);
	loadimage(&img[3], L"3.bmp", *p_size, *p_size);
	loadimage(&img[4], L"4.bmp", *p_size, *p_size);
	loadimage(&img[5], L"5.bmp", *p_size, *p_size);
	loadimage(&img[6], L"6.bmp", *p_size, *p_size);
	loadimage(&img[7], L"7.bmp", *p_size, *p_size);
	loadimage(&img[8], L"8.bmp", *p_size, *p_size);
	loadimage(&img[9], L"9.bmp", *p_size, *p_size);//雷
	loadimage(&img[10], L"10.bmp", *p_size, *p_size);//button_normal
	loadimage(&img[11], L"11.bmp", *p_size, *p_size);//flag
}
int PalyGame(int** p_map)
{
	int x;
	int y;
	MOUSEMSG msg = { 0 };
	int* p_size = &size;
	while (1)
	{
		msg = GetMouseMsg();
		switch (msg.uMsg)
		{
		case WM_LBUTTONDOWN://鼠标左键单击
			x = msg.x / (*p_size) + 1;
			y = msg.y / (*p_size) + 1;
			if (*(p_map[x] + y) >= 19 && *(p_map[x] + y) <= 28)
			{
				if (*(p_map[x] + y) == 20)
				{
					Open_Zero(p_map, x, y);
				}
				else {
					*(p_map[x] + y) = *(p_map[x] + y) - 20;
					count++;
				}
				std::cout << *(p_map[x] + y);
			}
			return *(p_map[x] + y);
		case WM_RBUTTONDOWN://鼠标右键单击
			x = msg.x / (*p_size) + 1;
			y = msg.y / (*p_size) + 1;
			if (*(p_map[x] + y) >= 19 && *(p_map[x] + y) <= 28)
			{
				*(p_map[x] + y) = *(p_map[x] + y) + 50;
			}
			else if (*(p_map[x] + y) >= 29)
			{
				*(p_map[x] + y) = *(p_map[x] + y) - 50;
			}
			return *(p_map[x] + y);
		}
	}
}
void Open_Zero(int** p_map, int x, int y)
{
	*(p_map[x] + y) = *(p_map[x] + y) - 20;
	count++;
	int* len = &Len;
	for (int i = x - 1; i <= x + 1; i++)
	{
		for (int j = y - 1; j <= y + 1; j++)
		{
			if (i >= 1 && i <= *len && j >= 1 && j <= *len)
			{
				if (*(p_map[i] + j) > 19 && *(p_map[i] + j) <= 28)
				{
					if (*(p_map[i] + j) != 20)
					{
						*(p_map[i] + j) = *(p_map[i] + j) - 20;
						count++;
					}
					else
					{
						Open_Zero(p_map, i, j);
					}
				}
			}
		}
	}
}
void DeleteArr(int** p_map, int* len)//释放二级指针申请的内存，避免内存泄露
{
	for (int i = 0; i < (*len + 2); i++)
	{
		delete[] p_map[i];
	}
	delete[] p_map;
}
