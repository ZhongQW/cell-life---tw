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

//����Ĵ�С 
#define SIZE 40 
//���̵Ĵ�С 
#define MAXWIDTH (40)
#define MAXHEIGHT MAXWIDTH

int cell[SIZE+1][SIZE+1] = {0}; //ԭʼ����
int tempCell[SIZE+1][SIZE+1] = {0}; //��ʱ���ϸ����һ��״̬����
void row_col_cell(int i, int j); //�˺��������һ�к����һ�е�cell[i][j]������tempCell[i][j]
void general(int i, int j); //�˺�����cell[i][j]��Χ�а˸�ϸ��������tempCell[i][j] 
void copy_cell(); //�˺�����һ��ϸ�������ڽ���������cell��״̬
void deadOrlive(); //�ж���һ������ϸ����״̬ 
void initCellArray(); //��ʼ�����������cell[][] 
void init();//��ʼ������ 
void setColor(int color); 


//�������õ��ĸ����ַ����Լ����ǵ���ɫ�����ͣ������ֱ�ʾ��
struct{
	char *ch;
    int color;
    char type;
}
charLive = {"��", 4, 1},  //��ϸ�� 
charDead = {"��", 2, 2},  //��ϸ�� 
charBorder = {"��", 0xe, 3};//�߿�  

//��һ���ṹ�����鱣���ͼ�еĸ�����
struct{
    char type;
    int index;
}globalMap[MAXWIDTH][MAXHEIGHT];


HANDLE hStdin;  //����̨���

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
    	printf("����ϸ�����״̬\n");
    	setColor(charDead.color);
       	printf("%s",charDead.ch);
    	printf("����ϸ������״̬\n");
    	printf("\n��%d����������\n",count);
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

// ������ɫ
void setColor(int color){
    SetConsoleTextAttribute(hStdin, color);
}

//��ʼ��cell
void initCellArray()
{
	int i,j;
	for(i = 0; i <= SIZE; i++)//��0�г�ʼ��
	{
		cell[0][i] = -1;
	}
	for(i = 0; i <= SIZE; i++)//��0�г�ʼ�� 
	{
		cell[i][0] = -1;
	} 
//    srand(time(NULL)); // ��ÿ�β��������������һ��  
//	 
//    for(i = 1; i <= SIZE; i++)//��ʼ��cell[][]�����������ϸ������ϸ�� 
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

// ���ù��λ�ã�xΪ�У�yΪ��
void setPosition(int x, int y){
    COORD coord;
    coord.X = 2*y;
    coord.Y = x;
    SetConsoleCursorPosition(hStdin, coord);
}

void init(){
    // ������ر��� 
    int xCenter = MAXHEIGHT%2 == 0 ? MAXHEIGHT / 2 : MAXHEIGHT / 2 + 1;
    int yCenter = MAXWIDTH%2 == 0 ? MAXWIDTH / 2 : MAXWIDTH / 2 + 1;
    CONSOLE_CURSOR_INFO cci;  //����̨�����Ϣ

    //���ô��ڴ�С 
    system("mode con: cols=100 lines=100");

    //���ع��
    hStdin = GetStdHandle(STD_OUTPUT_HANDLE);
    GetConsoleCursorInfo(hStdin, &cci);
    cci.bVisible = 0;
    SetConsoleCursorInfo(hStdin, &cci);
}
/*
�˺�����һ��ϸ�������ڽ���������cell��״̬
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
�˺�����cell[i][j]��Χ�а˸�ϸ��������tempCell[i][j]
*/
void general(int i, int j){
    int sum = 0;
    if(cell[i - 1][j] == 1)//�����cell
        sum++;
    if(cell[i - 1][j + 1] == 1)//�������cell
        sum++;
    if(cell[i][j + 1] == 1)//�����cell
        sum++;
    if(cell[i + 1][j + 1] == 1)//�������cell
        sum++;
    if(cell[i + 1][j] == 1)//�������cell
        sum++;
    if(cell[i + 1][j - 1] == 1)//�������cell
        sum++;
    if(cell[i][j - 1] == 1)//�����cell
        sum++;
    if(cell[i - 1][j - 1] == 1)//�������cell
        sum++;
	//����sum�жϵ�ǰcell����һ�����ڵ�״̬
	if(sum == 3)
		tempCell[i][j] = 1;
	else if(sum == 2)
		tempCell[i][j] = cell[i][j];
	else
	 	tempCell[i][j] = 0;
}
/*
�˺��������һ�к����һ�е�cell[i][j]������tempCell[i][j]
*/
void row_col_cell(int i, int j){ //���뵱ʱϸ��
	int sum = 0;

	if((SIZE == i) && (j != SIZE)) //���һ�е��������һ��
	{
		if(cell[i][j - 1] == 1) //��ߵ�ϸ��
			sum++;
		if(cell[i - 1][j - 1] == 1)//���Ͻǵ�ϸ��
			sum++;
		if(cell[i - 1][j] == 1)//���Ϸ���ϸ��
			sum++;
		if(cell[i + 1][j + 1] == 1)//���Ͻǵ�ϸ��
			sum++;
		if(cell[i][j + 1] == 1)//�ұߵ�ϸ��
			sum++;

	 }
	 else if((SIZE == j) && (SIZE != i )) //���һ�е��������һ��
	{
		if(cell[i - 1][j] == 1) //���ϱߵ�ϸ��
			sum++;
		if(cell[i - 1][j - 1] == 1)//���Ͻǵ�ϸ��
			sum++;
		if(cell[i][j - 1] == 1)//�󷽵�ϸ��
			sum++;
		if(cell[i + 1][j - 1] == 1)//���½ǵ�ϸ��
			sum++;
		if(cell[i + 1][j] == 1)//���±ߵ�ϸ��
			sum++;
	}
	else{//���һ��cell
		if(cell[i - 1][j] == 1)//���Ϸ���ϸ��
			sum++;
		if(cell[i - 1][j - 1] == 1)//���Ͻǵ�ϸ��
		    sum++;
		if(cell[i][j - 1] == 1)//��ߵ�ϸ��
			sum++;
	}
	//����sum�жϵ�ǰcell����һ�����ڵ�״̬
	if(sum == 3)
		tempCell[i][j] = 1;
	else if(sum == 2)
		tempCell[i][j] = cell[i][j];
	else
	 	tempCell[i][j] = 0;

}

/*
����ϸ������һ��״̬ 
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
