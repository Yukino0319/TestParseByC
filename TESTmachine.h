#include<stdio.h>
#include<ctype.h>
#include<stdlib.h>
#include<string>
#include<string.h>
#include<map>
using namespace std;
map<string,int>Map;
int TESTmachine();
//typedef struct codecvt
//{
//    char opt[10];
//    int operand;
//}Code;
//Code codes[1000];
enum enum_Opr{LOAD,LOADI,STO,STI,ADD,SUB,MULT,DIV,BR,BRF,EQ,NOTEQ,GT,LES,GE,LE,AND,OR,NOT,IN,OUT,RETURN,ENTER,CAL};
void init()
{
	Map["LOAD"]=LOAD;
	Map["LOADI"]=LOADI;
	Map["STO"]=STO;
    Map["STI"]=STI;
    Map["ADD"]=ADD;
    Map["SUB"]=SUB;
    Map["MULT"]=MULT;
    Map["DIV"]=DIV;
    Map["BR"]=BR;
    Map["BRF"]=BRF;
    Map["EQ"]=EQ;
    Map["MOTEQ"]=NOTEQ;
    Map["GT"]=GT;
    Map["LES"]=LES;
    Map["GE"]=GE;
    Map["LE"]=LE;
    Map["AND"]=AND;
    Map["OR"]=OR;
    Map["NOT"]=NOT;
    Map["IN"]=IN;
    Map["OUT"]=OUT;
    Map["RETURN"]=RETURN;
    Map["ENTER"]=ENTER;
    Map["CAL"]=CAL;
}

int TESTmachine()
{
    int es=0;

    int numCodes;
    char Codein[30] = "BMidOut.txt";
    FILE *fin;
    int stack[100],top=0,base=0;
    int ip=0;
    Code instruction;
    init();

    //printf("请输入目标文件名（包括路径）：");
    ///scanf("%s",Codein);
    if((fin=fopen(Codein,"rb"))==NULL)
        {
            printf("\n打开%s错误！\n",Codein);
            es=10;
            return(es);
        }
    numCodes=0;
    while(fread(codes+(numCodes++),sizeof(Code),1,fin)==1);
    numCodes--;
    top=0;
    base=0;
    ip=0;
    stack[0]=0;
    stack[1]=0;
    do{
        instruction=codes[ip];
        ip++;
        switch(Map[instruction.opt]){
        case LOAD:
            stack[top]=stack[base+instruction.operand];
            top++;
            break;
        case LOADI:
            stack[top]=instruction.operand;
            top++;
            break;
        case STO:
            top--;
            stack[base+instruction.operand]=stack[top];
            break;
        case ADD:
            top--;
            stack[top-1]=stack[top-1]+stack[top];
            break;
        case SUB:
            top--;
            stack[top-1]=stack[top-1]-stack[top];
            break;
        case MULT:
            top--;
            stack[top-1]=stack[top-1]*stack[top];
            break;
        case DIV:
            top--;
            stack[top-1]=stack[top-1]/stack[top];
            break;
        case BR:
            ip=instruction.operand;
            break;
        case BRF:
            top--;
            if(stack[top]==0)
                ip=instruction.operand;
            break;
        case EQ:
            top--;
            stack[top-1]=(stack[top-1]==stack[top]);
            break;
        case NOTEQ:
            top--;
            stack[top-1]=(stack[top-1]!=stack[top]);
            break;
        case GT:
            top--;
            stack[top-1]=(stack[top-1]>stack[top]);
            break;
        case LES:
            top--;
            stack[top-1]=(stack[top-1]<stack[top]);
            break;
        case GE:
            top--;
            stack[top-1]=(stack[top-1]>=stack[top]);
            break;
        case LE:
            top--;
            stack[top-1]=(stack[top-1]<=stack[top]);
            break;
        case AND:
            top--;
            stack[top-1]=(stack[top-1]&&stack[top]);
            break;
        case OR:
            top--;
            stack[top-1]=(stack[top-1]||stack[top]);
            break;
        case NOT:
            stack[top-1]=!stack[top-1];
            break;
        case IN:
            printf("请输入数据：");
            scanf("%d",&stack[top]);
            top++;
            break;
        case OUT:
            top--;
            printf("程序输出：%d\n",stack[top]);
            break;
        case ENTER:
            top+=instruction.operand;
            break;
        case RETURN:
            top=base;
            ip=stack[top+1];
            base=stack[top];
            break;
        case CAL:
            stack[top]=base;
            stack[top+1]=ip;
            base=top;
            ip=instruction.operand;
            break;
        }
    }while(ip!=0);
    return(es);
}


