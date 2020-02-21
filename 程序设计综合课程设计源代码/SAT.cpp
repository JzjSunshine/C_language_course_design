#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <memory.h>
#include <algorithm>
#define ERROR 0
//#define OK 1
#define max 9
#define min 1
//#define ERROR 0
#define OK 1
#define NumClaus 20000
#define MAXLIST 20000
#define INITIALIZATION 9999
#define FLEN 100

typedef int status;
typedef struct Record_Data_status
{
	status Data;//保存的数据
	status positive_text;//正文字
	status negative_text;//负文字
}*Record_Data_status_list;
typedef struct Data_status {
	status Data;
	status Data_situation;
}Data_statusp[MAXLIST];
typedef struct Clause {
	Data_status elem;
	struct Clause *next;
}Clause, *ClauseList;
//子句结点类型定义
typedef struct LnodeClause {
	Clause *firstnode;
	int num_positive_text;//正文字个数
	int num_negative_text;//负文字个数
	int ClauseNum;//记录该字句的单子句数目
	int Number_of_assigned_words;//未赋值字句数量
	int Determine;//判断该字句的真假状态 0 为假 ，1 为真，-1 为不确定
}LnodeClause, LnodeClausep[MAXLIST];
typedef struct MyList {
	LnodeClausep vertices;
	int num, numLine;
}MyList;
typedef struct Association_Clause {
	status num;
	Association_Clause *next;
};
typedef struct Decide_buffer {//文字计数器
	status Data;
	status Data_status;
	int num_positive_text;//正文字个数
	int num_negative_text;//负文字个数
	int total_number;//正负文字总数
	Clause *firstnode;
};
typedef struct Association_Data {
	status Data;
	status mark_num;
	int totalNum;
};
status creatClaus(MyList *NewList, status ID, status key, Decide_buffer *Decide_buf, Association_Data *Record_Data);
status destoryClause(MyList *NewList, status num);//销毁单子句
status DestoryDecide_buf(Decide_buffer *Decide_buf, int i);
status Destory(MyList *NewList, Decide_buffer *Decide_buf, Association_Data *Record_Data);
status isUnitClause(MyList *NewList);//判断是否为单子句*/
status evaluateClause(MyList *NewList, status id);//评估字句的真假状态
status evalute_All_Cluase(MyList *NewList);//判断CNF文件字句的真假状态
										   //status variable_decided();//从子句集中选择一个没有赋值的变量为其赋值，赋值成功
										   //返回 1 ；若所有变量均已被赋值，则说明问题可满足
status bcp_deduce(MyList *NewList, status j_index, status p_n_text, Association_Data *Record_Data, Decide_buffer *Decide_buf);//执行BCP过程
status Conflict(MyList *NewList);
status DPLL(MyList *NewList, int index_j, status temp_p_n_text, Association_Data *Record_Data, Decide_buffer *Decide_buf);
void BackTrack_Initilization(MyList *NewList, status j_index, Association_Data *Record_Data, Decide_buffer *Decide_buf);
status Check_repet_ID(Decide_buffer *Decide_buf, status key, status ID);
void print2(MyList *NewList, Association_Data *Record_Data, Decide_buffer *Decide_buf);
void print3(MyList *NewList, status Data, Decide_buffer *Decide_buf);
void print(MyList *NewList, Association_Data *Record_Data, Decide_buffer *Decide_buf, status index);

void Tranlate(char *filename, MyList *NewList, status Sudoku_buf[][9], status Match_buf[][10][10], Decide_buffer *Decide_buf, Association_Data *Record_Data);
void Ini_sudoku(status Sudoku_buf[][9], status  Match_buf[][10][10]);
status Creat_suduko(status Sudoku_buf[][9]);
status Judge_suduko(status Sudoku_buf[][9], int row, int col, int value);
void Display(status Sudoku_buf[][9]);
status Check_Sudoku_ID(MyList *NewList);
void Creat_CNF_POSITION(MyList *NewList, status Sudoku_buf[][9], status Match_buf[][10][10], Decide_buffer *Decide_buf, Association_Data *Record_Data);
void Creat_CNF_ROW(MyList *NewList, status Sudoku_buf[][9], status Match_buf[][10][10], Decide_buffer *Decide_buf, Association_Data *Record_Data);
void Creat_CNF_COL(MyList *NewList, status Sudoku_buf[][9], status Match_buf[][10][10], Decide_buffer *Decide_buf, Association_Data *Record_Data);
void Creat_CNF_BLOCK(MyList *NewList, status Sudoku_buf[][9], status Match_buf[][10][10], Decide_buffer *Decide_buf, Association_Data *Record_Data);
status num_position(int x, int y, int value);
status creat_sudoku_file(char  *filename, status Sudoku_buf[][9], status find_word_in_sudoku);
status Translate_to_sudoku(MyList *NewList, status Sudoku_buf[][9], status Match_buf[][10][10], Decide_buffer *Decide_buf, Association_Data *Record_Data, status find_word_in_sudoku);
void sudoku_game(MyList *NewList, status Sudoku_buf[][9], status Match_buf[][10][10], Decide_buffer *Decide_buf, Association_Data *Record_Data);
status Judge_hole(char *filename, MyList *NewList, status Sudoku_buf[][9], status Match_buf[][10][10], Decide_buffer *Decide_buf, Association_Data *Record_Data);
status Judge_rule_sudoku(status Sudoku_buf[][9]);
status find_word_in_sudoku_fun(status Sudoku_buf[][9]);

status  ReadFile(char *filename, MyList *NewList, Association_Data *Record_Data, Decide_buffer *Decide_buf);
status SaveFile(char *filename, MyList *NewList, Association_Data *Record_Data, Decide_buffer *Decide_buf, status result, double time);

int main()
{
	status fun;
	char filename[500];
	char *pfile = NULL;
	status result = 0;
	float time = 0;

	int index_j = 1;
	int DPLL_solve = 0;
	clock_t start, end;

	Association_Data *Record_Data = NULL;
	MyList *NewList = NULL;
	Clause *p = NULL;
	Decide_buffer *Decide_buf = NULL;
	Decide_buf = (Decide_buffer*)malloc(sizeof(Decide_buffer)*MAXLIST);
	memset(Decide_buf, 0, sizeof(Decide_buffer)*MAXLIST);
	NewList = (struct MyList*)malloc(sizeof(MyList));
	NewList->num = 0;
	NewList->numLine = 0;
	memset(NewList->vertices, 0, sizeof(LnodeClause)*MAXLIST);
	Record_Data = (Association_Data*)malloc(sizeof(Association_Data)*MAXLIST);
	memset(Record_Data, -1, sizeof(Association_Data)*MAXLIST);

	status temp_p_n_text;
	status Sudoku_buf[9][9];
	status Match_buf[10][10][10];
	int op = 1;
	while (op)
	{
		//system("cls");
		printf("2019课程综合设计\n\n");
		printf("\n---------------------------------------------\n\n");
		printf("1.数独求解                          2.DPLL求解 \n");
		printf("3.销毁DPLL邻接表                    4.数独游戏 \n");
		printf("	                                0.EXIT（0） \n");
		printf("\n---------------------------------------------\n\n");
		printf("请输入要进行的操作：");
		scanf("%d", &op);
		switch (op)
		{
		case 1:
		{
			Record_Data = NULL;
			NewList = NULL;
			p = NULL;
			Decide_buf = NULL;
			Decide_buf = (Decide_buffer*)malloc(sizeof(Decide_buffer)*MAXLIST);
			memset(Decide_buf, 0, sizeof(Decide_buffer)*MAXLIST);
			NewList = (struct MyList*)malloc(sizeof(MyList));
			NewList->num = 0;
			NewList->numLine = 0;
			memset(NewList->vertices, 0, sizeof(LnodeClause)*MAXLIST);
			Record_Data = (Association_Data*)malloc(sizeof(Association_Data)*MAXLIST);
			memset(Record_Data, -1, sizeof(Association_Data)*MAXLIST);
			Tranlate(filename, NewList, Sudoku_buf, Match_buf, Decide_buf, Record_Data);
		}
		break;
		case 2:
		{
			Record_Data = NULL;
			NewList = NULL;
			p = NULL;
			Decide_buf = NULL;
			Decide_buf = (Decide_buffer*)malloc(sizeof(Decide_buffer)*MAXLIST);
			memset(Decide_buf, 0, sizeof(Decide_buffer)*MAXLIST);
			NewList = (struct MyList*)malloc(sizeof(MyList));
			NewList->num = 0;
			NewList->numLine = 0;
			memset(NewList->vertices, 0, sizeof(LnodeClause)*MAXLIST);
			Record_Data = (Association_Data*)malloc(sizeof(Association_Data)*MAXLIST);
			memset(Record_Data, -1, sizeof(Association_Data)*MAXLIST);
			printf("Please enter the name of the file:");
			scanf("%s", filename);
			fun = ReadFile(filename, NewList, Record_Data, Decide_buf);
			if (fun == ERROR)
				printf("can't open the file!\n");
			else if (fun == OK)
			{
				printf("success read the file!\n");
			}
			else if (fun == -1)
				printf("Array space application failed!\n");
			printf("\n");
			temp_p_n_text = 0;

			start = clock();

			result = DPLL(NewList, index_j, temp_p_n_text, Record_Data, Decide_buf);

			end = clock();
			time = (float)(end - start) * 1000 / CLOCKS_PER_SEC;

			if (result == OK)
				printf("SAT!\n");
			else
			{
				printf("NOT SAT!\n");
			}
			SaveFile(filename, NewList, Record_Data, Decide_buf, result, time);
			printf("输出字句的状态\n");
			for (int i = 1; i < NewList->num + 1; i++)
				printf("%4d %4d\n", Decide_buf[i].Data, Decide_buf[i].Data_status);
			printf("\n");

			printf("running time:%f\n", time);
			//print(NewList, Record_Data, Decide_buf,1);
			//print(NewList, Record_Data, Decide_buf, index_j);
			//free(Decide_buf);
			//free(NewList);
		};
		break;
		case 3:
			Destory(NewList, Decide_buf, Record_Data);
			break;
		case 4:
		{
			Record_Data = NULL;
			NewList = NULL;
			p = NULL;
			Decide_buf = NULL;
			Decide_buf = (Decide_buffer*)malloc(sizeof(Decide_buffer)*MAXLIST);
			memset(Decide_buf, 0, sizeof(Decide_buffer)*MAXLIST);
			NewList = (struct MyList*)malloc(sizeof(MyList));
			NewList->num = 0;
			NewList->numLine = 0;
			memset(NewList->vertices, 0, sizeof(LnodeClause)*MAXLIST);
			Record_Data = (Association_Data*)malloc(sizeof(Association_Data)*MAXLIST);
			memset(Record_Data, -1, sizeof(Association_Data)*MAXLIST);
		}
		sudoku_game(NewList, Sudoku_buf, Match_buf, Decide_buf, Record_Data);
		break;
		case 0:
			exit(0);
			break;
		default:
			break;
		}
	}



	return 0;
}


int Num = 0;
int NumLine = 0;
status  ReadFile(char *filename, MyList *NewList, Association_Data *Record_Data, Decide_buffer *Decide_buf)
{
	FILE *fp = NULL;
	char ch;
	status key = 0;
	char tempFp[FLEN + 1];
	if ((fp = fopen(filename, "r")) == NULL)
		return ERROR;
	ch = fgetc(fp);
	while (ch != 'p')
	{
		fgets(tempFp, FLEN, fp);
		ch = fgetc(fp);
	}
	//printf("%c\n", ch);
	fgets(tempFp, (sizeof(int) / 4) * 5, fp);
	fscanf(fp, "%d %d", &Num, &NumLine);
	printf("%d %d\n", Num, NumLine);
	NewList->num = Num;  NewList->numLine = NumLine;
	for (int i = 0; i < NewList->numLine; i++)
	{
		fscanf(fp, "%d", &key);
		creatClaus(NewList, i, key, Decide_buf, Record_Data);
		//printf("%d ", key);
		while (key != 0)
		{
			fscanf(fp, "%d", &key);
			if (key != 0)
				creatClaus(NewList, i, key, Decide_buf, Record_Data);
		}
	}
	//对计数器的变量数量做排序
	//quickSort(Record_Data,1, NewList->num);
	fclose(fp);
	return OK;
}
status SaveFile(char *filename, MyList *NewList, Association_Data *Record_Data, Decide_buffer *Decide_buf, status result, double time)
{
	FILE *fp = NULL;
	char path[30] = ".res";
	char file[100];
	strncat(filename, path, 50);
	//char filepath[30] = "D:\\SAT测试备选算例\\Result";
	//sprintf(file, "%s/%s", filepath, filename);
	if ((fp = fopen(filename, "w")) == NULL)
		return ERROR;
	if (result == OK)
		fprintf(fp, "%s   %d\n", "s", 1);
	else if (result == ERROR)
		fprintf(fp, "%s   %d\n", "s", 0);
	else
		fprintf(fp, "%s   %d\n", "s", -1);
	fprintf(fp, "%s ", "v");
	for (int i = 1; i < NewList->num + 1; i++)
	{
		if (Decide_buf[i].Data_status == 1)
			fprintf(fp, "%d ", Decide_buf[i].Data);
		else if (Decide_buf[i].Data_status == 0)
			fprintf(fp, "%d ", -Decide_buf[i].Data);
		else
			fprintf(fp, "%d ", -1);
	}
	fprintf(fp, "\n");
	fprintf(fp, "%s  %lf\n", "t", time);
	fclose(fp);
}

status Check_repet_ID(Decide_buffer *Decide_buf, status key, status ID)
{//检查关联链表，防止记录一个文字多次
	Clause *p = NULL;
	if (Decide_buf[key].firstnode == NULL)
		return OK;
	else
		p = Decide_buf[key].firstnode;
	while (p != NULL)
	{
		if (p->elem.Data == ID)
			return ERROR;
		p = p->next;
	}
	return OK;
}
status creatClaus(MyList *NewList, status ID, status key, Decide_buffer *Decide_buf, Association_Data *Record_Data)
{
	Clause *p = NULL;
	NewList->vertices[ID].Determine = -1;
	p = (struct Clause*)malloc(sizeof(Clause));
	p->elem.Data = key;
	p->elem.Data_situation = -1;
	p->next = NULL;

	Clause *q_Record = NULL;
	q_Record = (Clause*)malloc(sizeof(Clause));
	q_Record->elem.Data = ID;
	q_Record->next = NULL;

	if (key > 0)
	{
		if (Decide_buf[key].Data == 0)
			Decide_buf[key].Data = key;
		Decide_buf[key].num_positive_text++;
		NewList->vertices[ID].num_positive_text++;
		Decide_buf[key].total_number++;
		Decide_buf[key].Data_status = -1;
		if (Check_repet_ID(Decide_buf, key, ID) == OK)
		{//记录文字出现的位置
			if (Decide_buf[key].firstnode == NULL)
				Decide_buf[key].firstnode = q_Record;
			else
			{
				q_Record->next = Decide_buf[key].firstnode;
				Decide_buf[key].firstnode = q_Record;
			}
		}
	}
	else if (key < 0)
	{
		if (Decide_buf[-key].Data == 0)
			Decide_buf[-key].Data = -key;
		Decide_buf[-key].num_negative_text++;
		Decide_buf[-key].total_number++;
		NewList->vertices[ID].num_negative_text++;
		Decide_buf[-key].Data_status = -1;
		if (Check_repet_ID(Decide_buf, -key, ID) == OK)
		{
			if (Decide_buf[-key].firstnode == NULL)
			{
				Decide_buf[-key].firstnode = q_Record;
			}
			else
			{
				q_Record->next = Decide_buf[-key].firstnode;
				Decide_buf[-key].firstnode = q_Record;
			}
		}
	}
	NewList->vertices[ID].ClauseNum++;
	NewList->vertices[ID].Number_of_assigned_words = NewList->vertices[ID].ClauseNum;
	if (NewList->vertices[ID].firstnode == NULL)
		NewList->vertices[ID].firstnode = p;
	else
	{
		p->next = NewList->vertices[ID].firstnode;
		NewList->vertices[ID].firstnode = p;
	}
	return OK;
}
status Destory(MyList *NewList, Decide_buffer *Decide_buf, Association_Data *Record_Data)
{
	for (int i = 0; i < NewList->numLine; i++)
	{
		destoryClause(NewList, i);
	}
	for (int i = 1; i < NewList->num + 1; i++)
	{
		DestoryDecide_buf(Decide_buf, i);
	}
	memset(Record_Data, -1, sizeof(Association_Data)*MAXLIST);
	memset(Decide_buf, 0, sizeof(Decide_buffer)*MAXLIST);
	memset(NewList->vertices, 0, sizeof(LnodeClause)*MAXLIST);
	memset(Record_Data, -1, sizeof(Association_Data)*MAXLIST);
	//free(Decide_buf);
	//free(NewList);
	return OK;
}
status DestoryDecide_buf(Decide_buffer *Decide_buf, int i)
{
	Clause *p = Decide_buf[i].firstnode;
	Clause *q = p;
	while (p)
	{
		q = p->next;
		free(p);
		p = q;
	}
	return OK;
}
status destoryClause(MyList *NewList, status num)
{
	Clause *p_New = NewList->vertices[num].firstnode;
	Clause *q = p_New;
	while (p_New)
	{
		q = p_New->next;
		free(p_New);
		p_New = q;
	}
	NewList->vertices[num].firstnode = NULL;
	return OK;
}
status isUnitClause(MyList *NewList)//判断是否为单子句
{   //单子句来源于两部分，一是字句数量为1，二是赋值化简得到的单子句
	//而赋值化简得到的单子句怎么搞
	MyList *p = NULL;
	Clause *q = NULL;

	for (int i = 0; (i < NewList->numLine); i++)
	{
		if ((NewList->vertices[i].ClauseNum == 1) && NewList->vertices[i].Determine == -1)
			return i;
		else if (NewList->vertices[i].Number_of_assigned_words == 1 && NewList->vertices[i].Determine == -1)
			return i;
	}
	return -1;
}
status evaluateClause(MyList *NewList, status id)
{//判断一条的字句的状态,同时给出该字句的三种状态，真(1)，假(0)，无法确定(-1)
 //字句的真假状态赋值
	Clause *p = NULL;
	int temp_Record_Num = NewList->vertices[id].ClauseNum;
	//记录字句个数，如果循环做完，假字句个数没有改变，则该条字句状态为假
	p = NewList->vertices[id].firstnode;
	while (p)
	{
		if (p->elem.Data_situation == 1)
		{
			NewList->vertices[id].Determine = 1;
			return OK;
		}
		else if (p->elem.Data_situation == 0)
			--temp_Record_Num;
		p = p->next;
	}
	if (temp_Record_Num == 0)
	{
		NewList->vertices[id].Determine = 0;
		return ERROR;
	}
	else if (temp_Record_Num != 0)
	{
		NewList->vertices[id].Determine = -1;
		return OK;
	}
	return OK;
}
status evalute_All_Cluase(MyList *NewList)
{
	MyList *p = NULL;
	for (int i = 0; i < NewList->numLine; i++)
	{
		if (NewList->vertices[i].Determine == 0 || NewList->vertices[i].Determine == -1)
			return ERROR;
	}
	return OK;
}

status bcp_deduce(MyList *NewList, status j_index, status p_n_text, Association_Data *Record_Data, Decide_buffer *Decide_buf)
{   //执行BCP推导过程，利用化简策略对字句集化简
	//BCP就是反复应用单元字句规则对合取范式的子句集进行化简，直到子句集中不再存在
	//单元字句的过程单元字句规则：    对于子句w = (x1 || -x2 || x3 || -x4）
	//当在x1 = 0，x2 = 1，x3 = 0的条件下，该子句就变为单元子句，很显然，
	//为使该子句可满足，子句w中的文字-x4必需取值为 0，
	status temp_p_n_text = 0;
	status temp_Data = 0;
	status Data = 0;
	int flag = 1;
	status fun = -1;
	Data = Record_Data[j_index].Data;
	Decide_buf[Data].Data_status = p_n_text;
	Clause *p_NewList = NULL;
	Clause *p_Association = NULL;
	p_Association = Decide_buf[Record_Data[j_index].Data].firstnode;
	while (p_Association)
	{
		if (NewList->vertices[p_Association->elem.Data].Determine == -1)
		{//对字句当前的状态有判断
			p_NewList = NewList->vertices[p_Association->elem.Data].firstnode;
			flag = 1;
			while (p_NewList)
			{
				if (p_NewList->elem.Data == Data || p_NewList->elem.Data == -Data)
				{
					if (p_NewList->elem.Data > 0)
					{
						p_NewList->elem.Data_situation = Decide_buf[Data].Data_status;
						NewList->vertices[p_Association->elem.Data].num_positive_text--;
					}
					else
					{
						p_NewList->elem.Data_situation = (Decide_buf[Data].Data_status == 1 ? 0 : 1);
						NewList->vertices[p_Association->elem.Data].num_negative_text--;
					}
					NewList->vertices[p_Association->elem.Data].Number_of_assigned_words--;
				}
				p_NewList = p_NewList->next;
			}
			evaluateClause(NewList, p_Association->elem.Data);
		}
		p_Association = p_Association->next;
	}
	//system("cls");
	//print3(NewList, Data, Decide_buf);
	return OK;
}
status Conflict(MyList *NewList)
{//判断是否存在冲突
	for (int i = 0; i < NewList->numLine; i++)
	{
		if (NewList->vertices[i].Determine == 0)
			return ERROR;
	}
	return OK;
}
void print(MyList *NewList, Association_Data *Record_Data, Decide_buffer *Decide_buf, status index)
{
	printf("当前在栈里的元素\n");
	printf("序号      变元     真值     化简方式 \n");
	for (int i = 1; i<index + 2; i++)
		printf("%5d    %5d   %5d   %5d\n", i, Record_Data[i].Data, Decide_buf[Record_Data[i].Data].Data_status, Record_Data[i].mark_num);
	printf("\n\n");
	printf("当前所有  字句  的真假状态\n");
	for (int i = 0; i < NewList->numLine; i++)
	{
		Clause *p = NewList->vertices[i].firstnode;
		printf("%-5d    %-5d\n", i, NewList->vertices[i].Determine);
	}
	printf("\n\n");
	printf("当前CNF文件中变元的的真值情况\n");
	for (int i = 0; i < NewList->numLine; i++)
	{
		printf("%-2d  %-2d      ", i, NewList->vertices[i].Determine);
		Clause *p = NewList->vertices[i].firstnode;
		while (p)
		{
			printf("%-2d    ", p->elem.Data_situation);
			p = p->next;
		}
		printf("\n");
	}
	printf("\n\n");
	/*printf("当前元素所在位置:%d\n", Record_Data[index].Data);
	/*Clause *p_Ass = Decide_buf[Record_Data[index].Data].firstnode;
	while (p_Ass)
	{
	printf("%d   ", p_Ass->elem.Data);
	p_Ass = p_Ass->next;
	}*/
}
status variable_decide(MyList *NewList, Association_Data *Record_Data, Decide_buffer *Decide_buf)
{//最短正字句规则,返回当前最短的正字句序号
	status MaxNum = -1;
	status Cluase_Num = 100;
	status ID = -1;
	for (int i = 0; i < NewList->numLine; i++)
	{
		if (NewList->vertices[i].num_positive_text == NewList->vertices[i].Number_of_assigned_words && NewList->vertices[i].ClauseNum <= Cluase_Num)
		{//在每次回溯之后，字句长度都会变短
			if (NewList->vertices[i].Determine == -1)
			{
				Cluase_Num = NewList->vertices[i].ClauseNum;
				ID = i;
			}
		}
	}
	return ID;
}
void print2(MyList *NewList, Association_Data *Record_Data, Decide_buffer *Decide_buf)
{//输出当前所有变元的赋值情况
	printf("变元当前的真值：\n");
	for (int i = 1; i < NewList->num + 1; i++)
		printf("%2d   %2d\n", i, Decide_buf[i].Data_status);
}
status DPLL(MyList *NewList, int index_j, status temp_p_n_text, Association_Data *Record_Data, Decide_buffer *Decide_buf)
{
	status fun = 0;
	int flag_while = 1;
	int flag = 1;
	int j_index = 0;
	status fun_single_flag = -1;
	status temp_Data = -1;
	status back_flag = 0;
	status isUnitClause_flag = 0;
	status bcp_flag = 1;
	status postitive_num = 0;
	status negative_num = 0;
	status Decide_p_n_text = 0;
	status fun_Decide = -1;
	status fun_conflict_one = -1;
	status fun_conflict_two = -1;

	/*for (int i = 1; i < NewList->num+1; i++)
	{
	postitive_num += Decide_buf[i].num_positive_text;
	negative_num += Decide_buf[i].num_negative_text;
	}
	if (postitive_num > negative_num)
	Decide_p_n_text = 1;
	else*/
	Decide_p_n_text = 0;
	//第一次给最短正字句赋值，并入栈
	fun_Decide = variable_decide(NewList, Record_Data, Decide_buf);
	if (fun_Decide == -1)
	{//刚开始没有满足条件的最短正字句
		if (Decide_buf[1].Data_status == -1)
			Record_Data[1].Data = Decide_buf[1].Data;
	}
	else {//有满足条件的正字句
		Clause *p = NewList->vertices[fun_Decide].firstnode;
		while (p)
		{
			if (p->elem.Data_situation == -1)
			{
				Record_Data[1].Data = p->elem.Data;
				break;
			}
			p = p->next;
		}
	}
	bcp_deduce(NewList, index_j, Decide_p_n_text, Record_Data, Decide_buf);

	while (flag_while == 1)
	{
		status back_flag = 1;
		fun_single_flag = isUnitClause(NewList);
		if (fun_conflict_two != ERROR)
		{//这里是为了防止翻转后有矛盾继续执行
			while (fun_single_flag != -1 && back_flag == 1)
			{//当前单子句没有化简完的时候,循环化简当前单子句
				status temp_Data = 0;
				Clause *p = NewList->vertices[fun_single_flag].firstnode;
				while (p)
				{//找到单子句
					if (p->elem.Data_situation == -1)
					{
						temp_Data = p->elem.Data;
						break;
					}
					p = p->next;
				}
				while (Record_Data[index_j].Data != -1)
					index_j++;
				if (Record_Data[index_j].Data == -1)
				{//把单子句文字入栈
					if (temp_Data > 0)
					{
						Record_Data[index_j].Data = temp_Data;
						Record_Data[index_j].mark_num = 0;
						Decide_buf[Record_Data[index_j].Data].Data_status = 1;
					}
					else if (temp_Data < 0)
					{
						Record_Data[index_j].Data = -temp_Data;
						Record_Data[index_j].mark_num = 0;
						Decide_buf[Record_Data[index_j].Data].Data_status = 0;
					}
					bcp_deduce(NewList, index_j, Decide_buf[Record_Data[index_j].Data].Data_status, Record_Data, Decide_buf);

				}
				fun_conflict_one = Conflict(NewList);//是否有矛盾
				if (fun_conflict_one == ERROR)
				{
					BackTrack_Initilization(NewList, index_j, Record_Data, Decide_buf);
					--index_j;
					if (Record_Data[index_j].mark_num == -1)
						back_flag = 0;//遇到最短正字句规则化简的文字，跳出循环
					else
					{//全部恢复由于单子句规则化简的文字
						while (Record_Data[index_j].mark_num != -1)
						{
							BackTrack_Initilization(NewList, index_j, Record_Data, Decide_buf);
							--index_j;
							back_flag = 0;
						}
					}
				}
				fun_single_flag = isUnitClause(NewList);
			}
		}
		//system("cls");
		//print(NewList, Record_Data, Decide_buf, index_j);
		fun = evalute_All_Cluase(NewList);
		if (fun == OK)
		{
			flag_while = 0;
			return OK;
		}
		if (back_flag == 1 && fun_conflict_one != ERROR)
		{//上一次单子句化简没有矛盾，新元素继续入栈
			while (Record_Data[index_j].Data != -1)
				index_j++;
			if (Record_Data[index_j].Data == -1)
			{
				fun_Decide = variable_decide(NewList, Record_Data, Decide_buf);//寻找当前最短正字句
				if (fun_Decide != -1)
				{//有满足条件的最短正字句，该正字句的首个变元入栈
					Clause  *p_temp = NewList->vertices[fun_Decide].firstnode;
					while (p_temp)
					{
						if (p_temp->elem.Data_situation == -1)
						{//因为是最短正字句，所以，元素一定是正值
							Record_Data[index_j].Data = p_temp->elem.Data;
							temp_p_n_text = Decide_p_n_text;
							break;
						}
						p_temp = p_temp->next;
					}
				}
				else
				{//没有满足条件的正字句,就按照变元顺序选择一个没有赋值的变元
					for (int i = 1; i < NewList->num + 1; i++)
					{
						if (Decide_buf[i].Data_status == -1)
						{
							Record_Data[index_j].Data = Decide_buf[i].Data;
							temp_p_n_text = Decide_p_n_text;
							break;
						}
					}
				}
				//system("cls");
				//print2(NewList, index_j, temp_p_n_text,Record_Data, Decide_buf);
				//print(NewList, Record_Data, Decide_buf, j_index);
				bcp_deduce(NewList, index_j, temp_p_n_text, Record_Data, Decide_buf);

			}
		}
		if (fun_conflict_two != ERROR)//还是为了防止在翻转后没法判断是不是有矛盾
		{
			fun_conflict_one = Conflict(NewList);
		}
		if ((fun_conflict_one != ERROR) && (index_j < NewList->num + 1))
		{//没有冲突
			fun = evalute_All_Cluase(NewList);
			if (fun == OK)
			{
				flag_while = 0;
				return OK;
			}
		}
		if (fun_conflict_one == ERROR || back_flag == 0)
		{//发生冲突   发生冲突的值是在最短正字句赋值过程中产生的，不是单子句化简产生
			if (Decide_buf[Record_Data[index_j].Data].Data_status == Decide_p_n_text)
			{//就近翻转, 还停留在index_j这一层，所以不需要考虑其他情形
				temp_Data = Record_Data[index_j].Data;
				BackTrack_Initilization(NewList, index_j, Record_Data, Decide_buf);
				Record_Data[index_j].Data = temp_Data;
				temp_p_n_text = (Decide_p_n_text == 1 ? 0 : 1);
				bcp_deduce(NewList, index_j, temp_p_n_text, Record_Data, Decide_buf);
			}
			else if (Decide_buf[Record_Data[index_j].Data].Data_status == (Decide_p_n_text == 0 ? 1 : 0))
			{//从右边回溯上来
				if (index_j == 1 && Decide_buf[Record_Data[index_j].Data].Data_status == (Decide_p_n_text == 0 ? 1 : 0))
				{//从右子树回溯到根，说明整个二叉树已经遍历完成
					flag_while = 0;
					return ERROR;
				}
				BackTrack_Initilization(NewList, index_j, Record_Data, Decide_buf);
				--index_j;
				if (index_j == 1 && Decide_buf[Record_Data[index_j].Data].Data_status == (Decide_p_n_text == 0 ? 1 : 0))
				{//从右子树回溯到根，说明整个二叉树已经遍历完成
					flag_while = 0;
					return ERROR;
				}
				while (((Record_Data[index_j].mark_num == 0) && index_j > 1) || (Decide_buf[Record_Data[index_j].Data].Data_status == 1) && index_j > 1)
				{//当栈里面的值赋值是 1 或者是单子句化简得到的值（做了标记，即mark_num的值是 0），都需要恢复
					BackTrack_Initilization(NewList, index_j, Record_Data, Decide_buf);
					--index_j;
					if (Decide_buf[Record_Data[index_j].Data].Data_status == 0 && Record_Data[index_j].mark_num == -1)
						break;
				}
				if (index_j == 1 && Decide_buf[Record_Data[index_j].Data].Data_status == (Decide_p_n_text == 0 ? 1 : 0))
				{//从右子树回溯到根，说明整个二叉树已经遍历完成
					flag_while = 0;
					return ERROR;
				}
				temp_Data = Record_Data[index_j].Data;
				BackTrack_Initilization(NewList, index_j, Record_Data, Decide_buf);
				Record_Data[index_j].Data = temp_Data;
				temp_p_n_text = (Decide_p_n_text == 0 ? 1 : 0);
				bcp_deduce(NewList, index_j, temp_p_n_text, Record_Data, Decide_buf);

			}
		}
		//再次判断是不是有矛盾
		fun_conflict_two = Conflict(NewList);
		//system("cls");
		//print(NewList, Record_Data, Decide_buf, index_j);
	}
}
void BackTrack_Initilization(MyList *NewList, status j_index, Association_Data *Record_Data, Decide_buffer *Decide_buf)
{//回溯过程中恢复特定变量的真值
	Clause *p_NewList = NULL;
	Clause *p_Association = NULL;
	status Data = Record_Data[j_index].Data;
	status temp_Data = 0;
	status Data_flag = 1;

	//system("cls");
	//print(NewList, Record_Data, Decide_buf, j_index);

	p_Association = Decide_buf[Record_Data[j_index].Data].firstnode;
	Record_Data[j_index].Data = -1;
	Record_Data[j_index].mark_num = -1;
	Decide_buf[Data].Data_status = -1;

	while (p_Association)
	{
		Data_flag = 1;
		if (NewList->vertices[p_Association->elem.Data].Determine == -1)
		{//字句真假状态没有确定
			p_NewList = NewList->vertices[p_Association->elem.Data].firstnode;
			while (p_NewList)
			{
				if (p_NewList->elem.Data == Data || p_NewList->elem.Data == -Data)
				{
					p_NewList->elem.Data_situation = -1;
					if (p_NewList->elem.Data > 0)
						++NewList->vertices[p_Association->elem.Data].num_positive_text;
					else
						++NewList->vertices[p_Association->elem.Data].num_negative_text;
					++NewList->vertices[p_Association->elem.Data].Number_of_assigned_words;
				}
				p_NewList = p_NewList->next;
			}
		}
		else if (NewList->vertices[p_Association->elem.Data].Determine == 0)
		{// 只能是  最后一个文字的赋值导致该字句为假，只恢复该文字的状态，再判断字句的真假状态
			p_NewList = NewList->vertices[p_Association->elem.Data].firstnode;
			while (p_NewList)
			{
				if (p_NewList->elem.Data == Data || p_NewList->elem.Data == -Data)
				{
					p_NewList->elem.Data_situation = -1;
					if (p_NewList->elem.Data > 0)
						++NewList->vertices[p_Association->elem.Data].num_positive_text;
					else
						++NewList->vertices[p_Association->elem.Data].num_negative_text;
					++NewList->vertices[p_Association->elem.Data].Number_of_assigned_words;
				}
				p_NewList = p_NewList->next;
			}
			evaluateClause(NewList, p_Association->elem.Data);
		}
		else if (NewList->vertices[p_Association->elem.Data].Determine == 1)
		{//
			status temp_flag = 1;
			Data_flag = 1;
			p_NewList = NewList->vertices[p_Association->elem.Data].firstnode;
			while (p_NewList && temp_flag == 1)
			{
				//判断是 当前变量 赋值使得该字句为真
				//如果是前面变量赋值导致该字句为真，则后面的变量在该字句中的值一直是 -1
				//如果不是该变量赋值导致该字句为真，则前面变量的值根本不需要做变动
				if (p_NewList->elem.Data == Data || p_NewList->elem.Data == -Data)
				{
					if (p_NewList->elem.Data_situation == 1)
					{
						Data_flag = 0;
						temp_flag = 0;
					}
				}
				if (Data_flag == 1)
					p_NewList = p_NewList->next;
			}
			if (Data_flag == 0)
			{//如果是由当前文字的赋值导致该字句为真
				p_NewList = NewList->vertices[p_Association->elem.Data].firstnode;
				while (p_NewList)
				{
					if (p_NewList->elem.Data == Data || p_NewList->elem.Data == -Data)
					{
						p_NewList->elem.Data_situation = -1;
						if (p_NewList->elem.Data > 0)
							++NewList->vertices[p_Association->elem.Data].num_positive_text;
						else
							++NewList->vertices[p_Association->elem.Data].num_negative_text;
						++NewList->vertices[p_Association->elem.Data].Number_of_assigned_words;
					}
					p_NewList = p_NewList->next;
				}
				evaluateClause(NewList, p_Association->elem.Data);
			}
		}
		p_Association = p_Association->next;
	}
	//system("cls");
	//print3(NewList, Data, Decide_buf);
}
void print3(MyList *NewList, status Data, Decide_buffer *Decide_buf)
{
	Clause *p_ass = Decide_buf[Data].firstnode;
	printf("当前文字的值: %2d       ", Data);
	printf("文字状态: %2d\n", Decide_buf[Data].Data_status);
	while (p_ass)
	{

		printf("字句序号 ：%2d      文字所在字句的状态 ：%2d ", p_ass->elem.Data, NewList->vertices[p_ass->elem.Data].Determine);
		Clause *p_N = NewList->vertices[p_ass->elem.Data].firstnode;
		while (p_N)
		{
			if (p_N->elem.Data == Data || p_N->elem.Data == -Data)
			{
				printf("文字：%2d   真值：%2d\n", p_N->elem.Data, p_N->elem.Data_situation);
			}
			p_N = p_N->next;
		}
		p_ass = p_ass->next;
	}
}

void Ini_sudoku(status Sudoku_buf[][9], status  Match_buf[][10][10])
{
	for (int i = 0; i < 9; i++)
	{
		for (int j = 0; j < 9; j++)
		{
			Sudoku_buf[i][j] = 0;
		}
	}
	for (int i = 0; i <= 9; i++)
	{
		for (int j = 0; j <= 9; j++)
		{
			for (int k = 0; k <= 9; k++)
			{
				Match_buf[i][j][k] = 0;
			}
		}
	}
}
status Creat_suduko(status Sudoku_buf[][9])
{//随机生成15个数填进三位数组
	status  i = 0, j = 0, v = 0;
	time_t t = time(NULL);
	srand(t);
	int count = 0;
	while (count < 11)
	{
		i = (rand() % 9);
		j = (rand() % 9);
		v = (rand() % 9) + 1;
		if (Judge_suduko(Sudoku_buf, i, j, v) == OK)
		{
			Sudoku_buf[i][j] = v;
			count++;
		}
	}
	return OK;
}
status Judge_suduko(status Sudoku_buf[][9], int row, int col, int value)
{
	for (int i = 0; i < 9; i++)
	{//判断行
		if (Sudoku_buf[row][i] == value)
			return ERROR;
	}
	for (int j = 0; j < 9; j++)
	{//判断列
		if (Sudoku_buf[j][col] == value)
			return ERROR;
	}
	//判断块
	int x = (row / 3) * 3;
	int y = (col / 3) * 3;
	for (int i = x; i < x + 3; i++)
	{
		for (int j = y; j < y + 3; j++)
			if (Sudoku_buf[i][j] == value)
				return ERROR;
	}
	return OK;
}
void Display(status Sudoku_buf[][9])
{
	status flag = 0;
	for (int i = 0; i < 9; i++)
	{
		if (i == 3 || i == 6)
			printf("- - - + - - - + - - - \n");
		for (int j = 0; j < 9; j++)
		{
			if (j == 3 || j == 6)
				printf("|");
			printf("%2d", Sudoku_buf[i][j]);
		}
		printf("\n");
	}
}
void Match(status Sudoku_buf[][9], status  Match_buf[][10][10])
{//匹配函数
	for (int i = 0; i<9; i++)
		for (int j = 0; j < 9; j++)
		{
			if (Match_buf[i][j] == 0)
			{//如果该位置为空
				for (int value = 1; value <= 9; value++)
				{
					if (Judge_suduko(Sudoku_buf, i, j, value) == OK)
					{//可以填值的位置标志位 1
						++Match_buf[i + 1][j + 1][0];
						Match_buf[i + 1][j + 1][value] = 1;
					}
				}
			}
		}
}
status Check_Sudoku_ID(MyList *NewList)
{//检查邻接表里面还没有生成字句的序号
	int i = 0;
	MyList *p = NewList;
	while (p)
	{
		if (p->vertices[i].firstnode == NULL)
			return i;
		i++;
	}
}
status num_position(int x, int y, int value)
{
	return (x * 100 + y * 10 + value);
}
void Creat_CNF_POSITION(MyList *NewList, status Sudoku_buf[][9], status Match_buf[][10][10], Decide_buffer *Decide_buf, Association_Data *Record_Data)
{//创建每个位置的约束条件
	int ID = 0;
	for (int i = 1; i <= 9; i++)
	{
		for (int j = 1; j <= 9; j++)
		{
			ID = Check_Sudoku_ID(NewList);
			for (int k = 1; k <= 9; k++)
			{//一个位置生成 9 条字句
				creatClaus(NewList, ID, num_position(i, j, k), Decide_buf, Record_Data);
			}
			for (int m = 1; m <= 9; m++)
			{//每个位置的字句两两组合
				ID = Check_Sudoku_ID(NewList);
				creatClaus(NewList, ID, -num_position(i, j, m), Decide_buf, Record_Data);
				for (int n = m; n <= 9; n++)
				{
					creatClaus(NewList, ID, -num_position(i, j, n), Decide_buf, Record_Data);
					ID = Check_Sudoku_ID(NewList);
					creatClaus(NewList, ID, -num_position(i, j, m), Decide_buf, Record_Data);
				}
			}
		}
	}
}
void Creat_CNF_ROW(MyList *NewList, status Sudoku_buf[][9], status Match_buf[][10][10], Decide_buffer *Decide_buf, Association_Data *Record_Data)
{//创建行约束条件
	int ID = 0;
	int value = 1;
	int flag_value = 0;
	for (int i = 1; i <= 9; i++)
	{
		ID = Check_Sudoku_ID(NewList);
		for (int j = 1; j <= 9; j++)
		{//创建字句的约束
			creatClaus(NewList, ID, num_position(i, j, value), Decide_buf, Record_Data);
		}
		value++;
	}
	int temp_i = 0;
	for (int k = 1; k <= 9; k++)
	{//值
		for (int i = 1; i <= 9; i++)
		{//行
			for (int j = 1; j <= 9; j++)
			{//列
				for (int m = j + 1; m <= 9; m++)
				{//下一列
					creatClaus(NewList, ID, -num_position(i, j, k), Decide_buf, Record_Data);
					if (temp_i % 2 == 0)
						ID = Check_Sudoku_ID(NewList);
				}
			}
		}
	}
}
void Creat_CNF_COL(MyList *NewList, status Sudoku_buf[][9], status Match_buf[][10][10], Decide_buffer *Decide_buf, Association_Data *Record_Data)
{//创建列约束条件
	int ID = 0;
	int sign[10] = { 0 };
	for (int j = 0; j < 9; j++)
	{
		for (int i = 0; i < 9; i++)//第 i + 1 列
		{//遍历寻找该列已经出现的数字
			if (Sudoku_buf[i][j] != 0)
				sign[Sudoku_buf[i][j]] = j + 1;//sign[该位置的值]=位置横坐标 j
		}
		for (int m = 1; m <= 9; m++)
		{//创建单子句
			if (sign[m] != 0)
			{
				ID = Check_Sudoku_ID(NewList);
				creatClaus(NewList, ID, num_position(sign[m], j, m), Decide_buf, Record_Data);
			}
		}
		for (int k = 1; k <= 9; k++)
		{//创建 111 121 131 141 151 161 171 181 191 形式的约束规则
			if (sign[k] == 0)
			{
				ID = Check_Sudoku_ID(NewList);
				for (int m = 1; m <= 9; m++)
				{
					creatClaus(NewList, ID, num_position(k, sign[m], m), Decide_buf, Record_Data);
				}
			}
		}
		for (int k = 1; k <= 9; k++)
		{//创建两两约束的规则

		}
	}

}
void Creat_CNF_BLOCK(MyList *NewList, status Sudoku_buf[][9], status Match_buf[][10][10], Decide_buffer *Decide_buf, Association_Data *Record_Data)
{//创建块约束条件
	int ID = Check_Sudoku_ID(NewList);
	int x = 0, y = 0;
	for (int block = 1; block <= 9; block++)
	{
		switch (block)
		{
		case 1:
			ID = Check_Sudoku_ID(NewList);
			for (int i = 1; i <= 3; i++)
			{
				for (int j = 1; j <= 3; j++)
				{
					for (int k = 1; k <= 9; k++)
					{

					}
				}
			}
			break;
		case 2:
			break;
		case 3:
			break;
		case 4:
			break;
		case 5:
			break;
		case 6:
			break;
		case 7:
			break;
		case 8:
			break;
		case 9:
			break;

		}
	}
}
void Creat_single_clause(MyList *NewList, status Sudoku_buf[][9], status Match_buf[][10][10], Decide_buffer *Decide_buf, Association_Data *Record_Data)
{//创建单子句
	int ID = 0;
	for (int i = 0; i < 9; i++)
	{
		for (int j = 0; j < 9; j++)
		{
			if (Sudoku_buf[i][j] != 0)
			{
				ID = Check_Sudoku_ID(NewList);
				creatClaus(NewList, ID, num_position(i + 1, j + 1, Sudoku_buf[i][j]), Decide_buf, Record_Data);
			}
		}
	}
}
//将数独转换成DPLL函数
void Tranlate(char *filename, MyList *NewList, status Sudoku_buf[][9], status Match_buf[][10][10], Decide_buffer *Decide_buf, Association_Data *Record_Data)
{
	Match_buf[10][10][10];
	for (int i = 0; i < 10; i++)
		for (int j = 0; j < 10; j++)
			for (int k = 0; k < 10; k++)
				Match_buf[i][j][k] = 0;
	Ini_sudoku(Sudoku_buf, Match_buf);//初始化
	Creat_suduko(Sudoku_buf);//随机填一些数创建数独
	status find_word_in_sudoku = 0;
	find_word_in_sudoku = find_word_in_sudoku_fun(Sudoku_buf);
	Display(Sudoku_buf);
	printf("\n\n");
	printf("创建CNF文件\n\n");
	creat_sudoku_file(filename, Sudoku_buf, find_word_in_sudoku);
	printf("将CNF读进DPLL\n\n");
	ReadFile(filename, NewList, Record_Data, Decide_buf);
	int index_j = 1;
	int temp_p_n_text = 0;
	printf("DPLL求解过程\n\n");
	DPLL(NewList, index_j, temp_p_n_text, Record_Data, Decide_buf);
	status result = DPLL(NewList, index_j, temp_p_n_text, Record_Data, Decide_buf);
	if (result == OK)
		printf("求解成功！\n\n");
	else if (result == ERROR)
		printf("求解失败！\n");
	printf("将DPLL求解结果转换为数独游戏！\n\n");
	Translate_to_sudoku(NewList, Sudoku_buf, Match_buf, Decide_buf, Record_Data, find_word_in_sudoku);
	Display(Sudoku_buf);
}
status creat_sudoku_file(char *filename, status Sudoku_buf[][9], status find_word_in_sudoku)
{
	int i = 1;
	int j = 1;
	int n = 1;
	int r = 0;
	int s = 0;
	int m = 0;
	int a = 0;
	int b = 0;
	int c = 0;
	int d = 0;
	int count = 0;
	int numline = 11988 + find_word_in_sudoku;
	int num = 889;
	FILE *fp;
	//char filename[50];
	char filepath[50];
	char file[100];
	printf("input filename: ");
	scanf("%s", filename);
	//printf("input filepath: "); scanf("%s", filepath);
	//sprintf(file, "%s\%s", filepath, filename);
	if ((fp = fopen(filename, "w")) == NULL)
		printf("读取失败！\n");
	fprintf(fp, "%s %d %d\n", "p cnf", num, numline);
	for (i = 1; i<10; i++)
		for (n = 1; n < 10; n++)
		{
			for (j = 1; j < 10; j++)
				fprintf(fp, " %d%d%d ", i, j, n);
			fprintf(fp, "0 \n");
			count++;
		}
	for (j = 1; j<10; j++)
		for (n = 1; n < 10; n++)
		{
			for (i = 1; i < 10; i++)
				fprintf(fp, " %d%d%d ", i, j, n);
			fprintf(fp, "0 \n");
			count++;
		}
	for (r = 0; r<3; r++)
		for (s = 0; s < 3; s++)
			for (n = 1; n < 10; n++)
			{
				for (i = 1; i < 4; i++)
					for (j = 1; j < 4; j++)
						fprintf(fp, " %d%d%d ", 3 * r + i, 3 * s + j, n);
				fprintf(fp, "0 \n");
				count++;
			}
	for (i = 1; i < 10; i++)
		for (j = 1; j < 10; j++)
			for (n = 1; n < 9; n++)
				for (m = n + 1; m < 10; m++)
				{
					fprintf(fp, "-%d%d%d -%d%d%d 0 \n", i, j, n, i, j, m);
					count++;
				}
	for (i = 1; i < 10; i++)
		for (j = 1; j < 10; j++)
		{
			for (n = 1; n < 10; n++)
				fprintf(fp, " %d%d%d ", i, j, n);
			fprintf(fp, "0 \n");
			count++;
		}
	for (i = 1; i<10; i++)
		for (n = 1; n < 9; n++)
			for (m = n + 1; m<10; m++)
				for (j = 1; j < 10; j++)
				{
					fprintf(fp, " -%d%d%d -%d%d%d 0 \n", i, n, j, i, m, j);
					count++;
				}
	for (i = 1; i<10; i++)
		for (n = 1; n < 9; n++)
			for (m = n + 1; m<10; m++)
				for (j = 1; j < 10; j++)
				{
					fprintf(fp, " -%d%d%d -%d%d%d 0 \n", n, i, j, m, i, j);
					count++;
				}
	for (r = 0; r<3; r++)//块约束
		for (s = 0; s < 3; s++)
			for (n = 1; n < 10; n++)
				for (i = 0; i < 8; i++)
					for (j = i + 1; j < 9; j++)
					{
						fprintf(fp, " -%d%d%d -%d%d%d 0 \n", 3 * r + i % 3 + 1, 3 * s + i / 3 + 1, n, 3 * r + j % 3 + 1, 3 * s + j / 3 + 1, n);
						count++;
					}
	//创建单子句
	for (int i = 0; i < 9; i++)
	{
		for (int j = 0; j < 9; j++)
		{
			if (Sudoku_buf[i][j] != 0)
			{
				fprintf(fp, " %d%d%d 0\n", i + 1, j + 1, Sudoku_buf[i][j]);
			}
		}
	}
	//printf("%d\n", count);
	fclose(fp);
	return OK;
}
status Translate_to_sudoku(MyList *NewList, status Sudoku_buf[][9], status Match_buf[][10][10], Decide_buffer *Decide_buf, Association_Data *Record_Data, status find_word_in_sudoku)
{
	int x = 0, y = 0, value = 0;
	status Data = 0;
	for (int i = 111; i < NewList->num + 1 + find_word_in_sudoku * 10; i++)
	{
		if (Decide_buf[i].Data_status == 1)
		{
			Data = Decide_buf[i].Data;
			value = Data % 10;
			Data = Data / 10;
			y = Data % 10;
			Data = Data / 10;
			x = Data % 10;
			Sudoku_buf[x - 1][y - 1] = value;
		}
	}
	return OK;
}
void sudoku_game(MyList *NewList, status Sudoku_buf[][9], status Match_buf[][10][10], Decide_buffer *Decide_buf, Association_Data *Record_Data)
{
	int hole = 0;
	int x = 0, y = 0, value = 0;
	status Data = 0;
	char filename[50];
	printf("生成数独终盘\n\n");
	Tranlate(filename, NewList, Sudoku_buf, Match_buf, Decide_buf, Record_Data);
	printf("\n\n");
	printf("请输入要挖的洞的个数:");
	scanf("%d", &hole);
	printf("\n\n");
	for (int k = 0; k < hole; k++)
	{//洞的个数
		x = (rand() % 9);
		y = (rand() % 9);
		Sudoku_buf[x][y] = 0;
	}
	printf("挖洞后的棋盘！\n\n");
	Display(Sudoku_buf);
	printf("数独游戏求解!\n\n");
	Destory(NewList, Decide_buf, Record_Data);
	{
		Record_Data = NULL;
		NewList = NULL;
		Decide_buf = NULL;
		Decide_buf = (Decide_buffer*)malloc(sizeof(Decide_buffer)*MAXLIST);
		memset(Decide_buf, 0, sizeof(Decide_buffer)*MAXLIST);
		NewList = (struct MyList*)malloc(sizeof(MyList));
		NewList->num = 0;
		NewList->numLine = 0;
		memset(NewList->vertices, 0, sizeof(LnodeClause)*MAXLIST);
		Record_Data = (Association_Data*)malloc(sizeof(Association_Data)*MAXLIST);
		memset(Record_Data, -1, sizeof(Association_Data)*MAXLIST);
	}
	Tranlate(filename, NewList, Sudoku_buf, Match_buf, Decide_buf, Record_Data);
	printf("求解结果!\n\n");
	Display(Sudoku_buf);
}
status Judge_hole(char *filename, MyList *NewList, status Sudoku_buf[][9], status Match_buf[][10][10], Decide_buffer *Decide_buf, Association_Data *Record_Data)
{
	Display(Sudoku_buf);
	status find_word_in_sudoku = 0;
	find_word_in_sudoku = find_word_in_sudoku_fun(Sudoku_buf);
	printf("\n\n");
	printf("创建CNF文件\n\n");
	creat_sudoku_file(filename, Sudoku_buf, find_word_in_sudoku);
	printf("将CNF读进DPLL\n\n");
	ReadFile(filename, NewList, Record_Data, Decide_buf);
	int index_j = 1;
	int temp_p_n_text = 0;
	printf("DPLL求解过程\n\n");
	DPLL(NewList, index_j, temp_p_n_text, Record_Data, Decide_buf);
	status result = DPLL(NewList, index_j, temp_p_n_text, Record_Data, Decide_buf);
	if (result == OK)
		printf("求解成功！\n\n");
	else if (result == ERROR)
		printf("求解失败！\n");
	printf("将DPLL求解结果转换为数独游戏！\n\n");
	Translate_to_sudoku(NewList, Sudoku_buf, Match_buf, Decide_buf, Record_Data, find_word_in_sudoku);
	if (Judge_rule_sudoku(Sudoku_buf) == OK)
		return OK;
	else
		return ERROR;
}
status Judge_rule_sudoku(status Sudoku_buf[][9])
{
	for (int i = 0; i < 9; i++)
	{
		for (int j = 0; j < 9; j++)
		{
			if (Sudoku_buf[i][j] == 0)
				return ERROR;
		}
	}
	return OK;
}
status find_word_in_sudoku_fun(status Sudoku_buf[][9])
{
	int count = 0;
	for (int i = 0; i < 9; i++)
	{
		for (int j = 0; j < 9; j++)
		{
			if (Sudoku_buf[i][j] != 0)
				count++;
		}
	}
	return count;
}











