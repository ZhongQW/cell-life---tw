#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <conio.h>
#include <time.h>
#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#define random(x) (rand()%x)

//数组的大小 
#define SIZE 40 
//棋盘的大小 
#define MAXWIDTH (40)
#define MAXHEIGHT MAXWIDTH

int cell[SIZE+1][SIZE+1] = {0}; //原始数组
int tempCell[SIZE+1][SIZE+1] = {0}; //临时存放细胞下一个状态数组
void row_col_cell(int i, int j); //此函数是最后一行和最后一列的cell[i][j]，更新tempCell[i][j]
void general(int i, int j); //此函数是cell[i][j]周围有八个细胞，更新tempCell[i][j] 
void copy_cell(); //此函数是一个细胞的周期结束，更新cell的状态
void deadOrlive(); //判断下一个周期细胞的状态 
void initCellArray(); //初始化产生随机的cell[][] 
void init();//初始化棋盘 
void setColor(int color); 


//程序中用到的各种字符，以及它们的颜色和类型（以数字表示）
struct{
	char *ch;
    int color;
    char type;
}
charLive = {"■", 4, 1},  //活细胞 
charDead = {"□", 2, 2},  //死细胞 
charBorder = {"★", 0xe, 3};//边框  

//用一个结构体数组保存地图中的各个点
struct{
    char type;
    int index;
}globalMap[MAXWIDTH][MAXHEIGHT];


HANDLE hStdin;  //控制台句柄

int main()
{
    initCellArray();	
	init();
	int x,y,k;	
	int count = 0; 
    
    while(1){
    	count++; 
    	setColor(charLive.color);
       	printf("\n%s",charLive.ch);
    	printf("代表细胞存活状态\n");
    	setColor(charDead.color);
       	printf("%s",charDead.ch);
    	printf("代表细胞死亡状态\n");
    	printf("\n第%d个生命周期\n",count);
    	for(k = 0; k < MAXHEIGHT + 2; k++){
		setColor(charBorder.color);
       	printf("%s",charBorder.ch);
       }
       printf("\n");
		for(x = 1; x <= MAXHEIGHT; x++){
			for(y = 0; y <= MAXWIDTH; y++){
				if(y == 0){
					setColor(charBorder.color);
       		        printf("%s", charBorder.ch);
			    }
				else{
				   if (cell[x][y] == 0) {
						setColor(charDead.color);
	           		    printf("%s", charDead.ch);
				   } 
				   else {
						setColor(charLive.color);
	            		printf("%s", charLive.ch);
				   }
				}
			   
			}
			setColor(charBorder.color);
       		printf("%s",charBorder.ch);
        	printf("\n");
        }
		for(k = 0; k < MAXHEIGHT + 2; k++){
			setColor(charBorder.color);
	   	 printf("%s", charBorder.ch);
  	 	}
		deadOrlive();
		Sleep(30);
		system("cls");
    }
	return 0;
}

// 设置颜色
void setColor(int color){
    SetConsoleTextAttribute(hStdin, color);
}

//初始化cell
void initCellArray()
{
	int i,j;
	for(i = 0; i <= SIZE; i++)//第0行初始化
	{
		cell[0][i] = -1;
	}
	for(i = 0; i <= SIZE; i++)//第0列初始化 
	{
		cell[i][0] = -1;
	} 
//    srand(time(NULL)); // 让每次产生的随机数都不一样  
//	 
//    for(i = 1; i <= SIZE; i++)//初始化cell[][]，随机产生活细胞和死细胞 
//        for(j = 1; j <= SIZE; j++)
//			cell[i][j] = random(10) % 2;	
     cell[6][2] = 1;
	 cell[6][3] = 1; 
     cell[7][2] = 1;  
     cell[7][3] = 1;  
     cell[6][12] = 1;   
     cell[7][12] = 1;  
     cell[8][12] = 1;  
     cell[5][13] = 1;  
     cell[9][13] = 1;  
     cell[4][14] = 1;  
     cell[10][14] = 1;  
     cell[10][15] = 1;  
     cell[4][15] = 1;  
     cell[7][16] = 1;  
     cell[5][17] = 1;  
     cell[9][17] = 1;  
     cell[6][18] = 1;  
     cell[7][18] = 1;  
     cell[8][18] = 1;  
     cell[7][19] = 1;  
     cell[4][23] = 1;  
     cell[5][23] = 1;  
     cell[6][23] = 1;  
     cell[4][22] = 1; 
     cell[5][22] = 1;  
     cell[6][22] = 1;  
     
	 cell[3][24] = 1;  
     cell[7][24] = 1;  
     
	 cell[2][26] = 1;  
     cell[3][26] = 1;  
     cell[7][26] = 1;  
     cell[8][26] = 1; 
     
     cell[4][36] = 1;  
     cell[5][36] = 1;  
     cell[4][37] = 1;  
     cell[5][37] = 1;  

} 

// 设置光标位置，x为行，y为列
void setPosition(int x, int y){
    COORD coord;
    coord.X = 2*y;
    coord.Y = x;
    SetConsoleCursorPosition(hStdin, coord);
}

void init(){
    // 设置相关变量 
    int xCenter = MAXHEIGHT%2 == 0 ? MAXHEIGHT / 2 : MAXHEIGHT / 2 + 1;
    int yCenter = MAXWIDTH%2 == 0 ? MAXWIDTH / 2 : MAXWIDTH / 2 + 1;
    CONSOLE_CURSOR_INFO cci;  //控制台光标信息

    //设置窗口大小 
    system("mode con: cols=100 lines=100");

    //隐藏光标
    hStdin = GetStdHandle(STD_OUTPUT_HANDLE);
    GetConsoleCursorInfo(hStdin, &cci);
    cci.bVisible = 0;
    SetConsoleCursorInfo(hStdin, &cci);
}
/*
此函数是一个细胞的周期结束，更新cell的状态
*/
void copy_cell(){
    int i, j;
    for(i = 1; i <= SIZE; i++){
        for(j = 1; j <= SIZE; j++){
            cell[i][j] = tempCell[i][j];
        }
    }
}
/*
此函数是cell[i][j]周围有八个细胞，更新tempCell[i][j]
*/
void general(int i, int j){
    int sum = 0;
    if(cell[i - 1][j] == 1)//上面的cell
        sum++;
    if(cell[i - 1][j + 1] == 1)//右上面的cell
        sum++;
    if(cell[i][j + 1] == 1)//右面的cell
        sum++;
    if(cell[i + 1][j + 1] == 1)//右下面的cell
        sum++;
    if(cell[i + 1][j] == 1)//正下面的cell
        sum++;
    if(cell[i + 1][j - 1] == 1)//左下面的cell
        sum++;
    if(cell[i][j - 1] == 1)//左面的cell
        sum++;
    if(cell[i - 1][j - 1] == 1)//左上面的cell
        sum++;
	//根据sum判断当前cell的下一个周期的状态
	if(sum == 3)
		tempCell[i][j] = 1;
	else if(sum == 2)
		tempCell[i][j] = cell[i][j];
	else
	 	tempCell[i][j] = 0;
}
/*
此函数是最后一行和最后一列的cell[i][j]，更新tempCell[i][j]
*/
void row_col_cell(int i, int j){ //传入当时细胞
	int sum = 0;

	if((SIZE == i) && (j != SIZE)) //最后一行但不是最后一列
	{
		if(cell[i][j - 1] == 1) //左边的细胞
			sum++;
		if(cell[i - 1][j - 1] == 1)//左上角的细胞
			sum++;
		if(cell[i - 1][j] == 1)//正上方的细胞
			sum++;
		if(cell[i + 1][j + 1] == 1)//右上角的细胞
			sum++;
		if(cell[i][j + 1] == 1)//右边的细胞
			sum++;

	 }
	 else if((SIZE == j) && (SIZE != i )) //最后一列但不是最后一行
	{
		if(cell[i - 1][j] == 1) //正上边的细胞
			sum++;
		if(cell[i - 1][j - 1] == 1)//左上角的细胞
			sum++;
		if(cell[i][j - 1] == 1)//左方的细胞
			sum++;
		if(cell[i + 1][j - 1] == 1)//左下角的细胞
			sum++;
		if(cell[i + 1][j] == 1)//正下边的细胞
			sum++;
	}
	else{//最后一个cell
		if(cell[i - 1][j] == 1)//正上方的细胞
			sum++;
		if(cell[i - 1][j - 1] == 1)//左上角的细胞
		    sum++;
		if(cell[i][j - 1] == 1)//左边的细胞
			sum++;
	}
	//根据sum判断当前cell的下一个周期的状态
	if(sum == 3)
		tempCell[i][j] = 1;
	else if(sum == 2)
		tempCell[i][j] = cell[i][j];
	else
	 	tempCell[i][j] = 0;

}

/*
更改细胞的下一个状态 
*/
void deadOrlive()
{
    int i, j;
    for(i = 1; i < SIZE + 1; i++)
    {
        for(j = 1; j < SIZE + 1; j++)
        {
            if(i == SIZE || j == SIZE)
                row_col_cell(i, j);
            else
                general(i, j);
        }
    }
    
    copy_cell();
    
}
