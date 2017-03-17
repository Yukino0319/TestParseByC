//Testparse 语法、语义分析及代码生成
#include "stdio.h"
#include "ctype.h"
#include "conio.h"
#include "string.h"
#include "stdlib.h"

#define maxsymbolIndex 100      //定义符号表的容量

enum Category_symbol {variable, function};      //标识符的类型

int TESTparse();
int program();
int fun_declaration();
int main_declaration();
int function_body();
int compound_stat();
int statement();
int call_stat();
int expression_stat();
int expression();
int bool_expr();
int additive_expr();
int term();
int factor();
int if_stat();
int while_stat();
int for_stat();
int write_stat();
int read_stat();
int do_while_stat();
int switch_stat();
int declaration_stat1();
int declaration_stat();
int declaration_list();
int statement_list();
int compound_stat();
int insert_Symbol(enum Category_symbol category, char *name, char *func_name);
int lookup(char *name, int *pPositon, char *func_name);
int N_bool_expr();

char token[20], token1[40];     //单词类别值、单词自身值
char tokenfile[30] = "out.txt";             //单词流文件的名字
char Codeout[30] = "MidOut.txt";               //要生成的文本形式的中间代码文件的名字
char Codeout1[30] = "BMidOut.txt";               //生成的二进制形式的中间代码文件

FILE *fpTokenin;                //单词流文件指针
FILE *fpCodeBinary;             //中间代码二进制文件指针
FILE *fpCodeText;               //中间代码文本文件指针

char function_name[20];     //当前操作函数名
int flag = 0;
char token_expr[20];

struct{                         //符号表
    char name[20];
    char func_name[20];
    enum Category_symbol kind;
    int address;
} symbol[maxsymbolIndex];

typedef struct Code{
    char opt[10];               //操作码
    int operand;                //操作数
} Code;
Code codes[200];                //存放中间代码的结构体数组

int codesIndex;
//codes数组中第一个空元素的下标，0序（下一条要生成的代码在codes中的位置）
int symbolIndex = 0;
//symbol数组中的第一个空元素的下标，0序（下一个要填入的标识符在符合表中的位置）
int offset;

//局部变量在所定义函数内部的相对地址

//语法、语义分析及代码生成程序
int TESTparse(){
    codesIndex = 0;
    int i;
    int es = 0;
    //printf("请输入单词流文件名（包括路径）：");
    //scanf("%s", tokenfile);
    if(( fpTokenin = fopen(tokenfile, "r")) == NULL){       //tokenfile表示词法分析成功后生成的out.txt，fpTokenin指示文件指针
        printf("\n打开%s错误！\n", tokenfile);
        es = 10;
        return es;
    }
    es = program();             //调用程序语法分析
    fclose(fpTokenin);

    printf("==语法语义分析及代码生成程序结果==\n");
    switch (es){
        case 0:
            printf("语法语义分析成功并抽象机汇编生成代码！\n");
            break;
        case 10:
            printf("\n打开%s错误！\n", tokenfile);
            break;
        case 1:
            printf("缺少\"{\"！\n");
            break;
        case 2:
            printf("缺少\"}\"！\n");
            break;
        case 3:
            printf("缺少标识符！\n");
            break;
        case 4:
            printf("缺少分号！\n");
            break;
        case 5:
            printf("缺少\"(\"！\n");
            break;
        case 6:
            printf("缺少\")\"！\n");
            break;
        case 7:
            printf("缺少操作数！\n");
            break;
        case 11:
            printf("函数开头缺少\"{\"！\n");
            break;
        case 12:
            printf("函数结束缺少\"}\"！\n");
            break;
        case 13:
            printf("最后一个函数的名字应该是main！\n");
            break;
        case 21:
            printf("符号表溢出！\n");
            break;
        case 22:
            printf("变量%s重复定义！\n", token1);
            break;
        case 23:
            printf("变量未声明！\n");
            break;
        case 24:
            printf("程序中main函数结束后，还有其他多余字符！\n");
            break;
        case 32:
            printf("函数名重复定义！\n");
            break;
        case 34:
            printf("call语句后面的标识符%s不是函数名！\n", token1);
            break;
        case 35:
            printf("read语句后面的标识符%s不是变量名！\n", token1);
            break;
        case 36:
            printf("赋值语句的左值%s不是变量名！\n", token1);
            break;
        case 37:
            printf("因子对应的标识符不是变量名！\n");
            break;
        default:
            printf("其他原因错误");
            break;
    }
    //printf("请输入要生成的文本形式的中间代码文件的名字（包括路径）：");
    //scanf("%s", Codeout);
    if((fpCodeText = fopen(Codeout, "w")) == NULL ){                //中间代码生成文件，即汇编语言文件，codeout表示中间代码文件，fpCodeText指示文件指针
        printf("\n创建%s错误！\n", Codeout);
        es = 10;
        return es;
    }
    for(i = 0; i < codesIndex; i++){// codesindex表示codes数组中第一个空元素的下标，0序（下一条要生成的代码在codes中的位置）
        if(strcmp(codes[i].opt, "LOAD") == 0 || strcmp(codes[i].opt, "LOADI") == 0 || strcmp(codes[i].opt, "STO") == 0 ||
           strcmp(codes[i].opt, "BR") == 0 || strcmp(codes[i].opt, "BRF" ) == 0 || strcmp(codes[i].opt, "CAL") == 0 ||
           strcmp(codes[i].opt, "ENTER") == 0 )
            fprintf(fpCodeText, "%3d %-5s %d\n", i, codes[i].opt, codes[i].operand);
        else
            fprintf(fpCodeText, "%3d %-5s\n", i, codes[i].opt);
    }
    fclose(fpCodeText);

    //printf("请输入要生成的二进制形式的中间代码文件的名字（结构体存储）：");
    //scanf("%s", Codeout);
    if((fpCodeBinary = fopen(Codeout1, "wb")) == NULL ){             //二进制生成文件，codeout表示中间代码文件，fpCodeBinary指示文件指针
        printf("\n创建%s错误！\n", Codeout1);
        es = 10;
        return es;
    }
    fwrite(codes, sizeof(Code), codesIndex, fpCodeBinary);
    fclose(fpCodeBinary);
    return es;
}

int program(){
    int es = 0, i;
    strcpy(codes[codesIndex++].opt, "BR");
    //codes数组的第0条指令是无条件转移到main函数的入口，入口地址需要返填

    fscanf(fpTokenin, "%s %s\n", token, token1);printf("token = %s, token1 = %s\n",token,token1);


    //拓展6
    while(!strcmp(token, "int")){  //判断是否是”全局变量的定义“
        es = declaration_stat1();
        if(es != 0)
            return es;
    }

    while(!strcmp(token, "function")){  //判断是否是”普通函数的定义“
        fscanf(fpTokenin, "%s %s\n", token, token1);printf("token = %s, token1 = %s\n",token,token1);
        es = fun_declaration();
        if(es != 0)
            return es;
        fscanf(fpTokenin, "%s %s\n", token, token1);printf("token = %s, token1 = %s\n",token,token1);
    }
        if(strcmp(token, "ID")){        //整个程序的最后是main函数的定义，其类别值为”ID”
            es = 1;
            return es;
        }
        if(strcmp(token1, "main")){        //整个程序的最后是main函数的定义，其类别值为”ID”
            es = 13;
            return es;
        }
        fscanf(fpTokenin, "%s %s\n", token, token1);printf("token = %s, token1 = %s\n",token,token1);

        es = main_declaration();

        if(es > 0)
            return es;
        if(!feof(fpTokenin))            //检测源程序中，程序结束后还有语句的情况
            return 24;

        //输出符号表的内容
        printf("符号表\n");
        printf("名字\t \t类型\t地址\t函数\n");
        for(int i = 0; i < symbolIndex; i++)
            printf("%-8s\t%d\t%d\t%s\n", symbol[i].name, symbol[i].kind, symbol[i].address, symbol[i].func_name);
        return es;
}

int fun_declaration(){
    int es = 0;
    if(strcmp(token, "ID")){
        return (es = 2);
    }

    strcpy(function_name, token1);
    insert_Symbol(function, token1, function_name);
    //拓展1  新建char function_name[20]，来储存当前的function名

    fscanf(fpTokenin, "%s %s\n", token, token1);printf("token = %s, token1 = %s\n",token,token1);
    if(strcmp(token, "(")) return (es = 5); //少左括号
    fscanf(fpTokenin, "%s %s\n", token, token1);printf("token = %s, token1 = %s\n",token,token1);
    if(strcmp(token, ")")) return (es = 6); //少右括号
    fscanf(fpTokenin, "%s %s\n", token, token1);printf("token = %s, token1 = %s\n",token,token1);
    es = function_body();
    return es;

}

int main_declaration(){
    int es = 0;
    strcpy(function_name, "main");
    insert_Symbol(function, "main", function_name);
    if(strcmp(token, "("))
        return (es = 5);
    fscanf(fpTokenin, "%s %s\n", token, token1);printf("token = %s, token1 = %s\n",token,token1);
    if(strcmp(token, ")"))
        return (es = 6);
    codes[0].operand = codesIndex;
    fscanf(fpTokenin, "%s %s\n", token, token1);printf("token = %s, token1 = %s\n",token,token1);
    es = function_body();
    return es;
}

int function_body(){
    int es = 0;
    if(strcmp(token, "{"))
        return (es = 11);
    symbol[symbolIndex-1].address = codesIndex;
    offset = 2+flag;
    fscanf(fpTokenin, "%s %s\n", token, token1);printf("token = %s, token1 = %s\n",token,token1);

    es = declaration_list();
    if(es > 0) return es;
    strcpy(codes[codesIndex].opt, "ENTER");
    codes[codesIndex].operand = offset;
    codesIndex++;

    es = statement_list();

    if(es > 0) return es;
    if(strcmp(token, "}"))
        return (es = 12);
    strcpy(codes[codesIndex++].opt, "RETURN");
    return es;
}

int declaration_list(){
    int es = 0;
    while(strcmp(token, "int") == 0){
        es = declaration_stat1();
        if(es > 0) return es;
    }
    return es;
}

int declaration_stat(){
    int es = 0;

    fscanf(fpTokenin, "%s %s\n", token, token1);printf("token = %s, token1 = %s\n",token,token1);
    if(strcmp(token, "ID")) return (es = 3); //少标识符

    es = insert_Symbol(variable, token1, function_name);

    if(es > 0) return es;
    fscanf(fpTokenin, "%s %s\n", token, token1);printf("token = %s, token1 = %s\n",token,token1);
    if(strcmp(token, ";")) return (es = 4); //少分号
    fscanf(fpTokenin, "%s %s\n", token, token1);printf("token = %s, token1 = %s\n",token,token1);
    return es;
}

int declaration_stat1(){//拓展7
    int es = 0, filladd;
    char token2[20], token3[40];
    int symbolPos;

    multi_int:
        fscanf(fpTokenin, "%s %s\n", token, token1);printf("token = %s, token1 = %s\n",token,token1);
        if(strcmp(token, "ID") == 0){
            es = insert_Symbol(variable, token1, function_name);

            filladd = ftell(fpTokenin);
            fscanf(fpTokenin, "%s %s\n", token2, token3);
            if(strcmp(token2, "=") == 0){
                es = lookup(token1, &symbolPos, function_name);
                if(es > 0) return es;
                if(!symbol[symbolPos].kind == variable) return (es = 36);
                fscanf(fpTokenin, "%s %s\n", token, token1);printf("token = %s, token1 = %s\n",token,token1);
                es = bool_expr();
                if(es > 0) return es;
                strcpy(codes[codesIndex].opt, "STO");
                codes[codesIndex++].operand = symbol[symbolPos].address;
            }
            if(strcmp(token3, ",") == 0 || strcmp(token1, ",") == 0){
                goto multi_int;
            }else if(strcmp(token3, ";") == 0 || strcmp(token1, ";") == 0 ){
                fscanf(fpTokenin, "%s %s\n", token, token1);printf("token = %s, token1 = %s\n",token,token1);
            }else{
                return (es = 233);
            }
        }

    if(es > 0) return es;

    return es;
}

int statement_list(){
    int es = 0;

    while(strcmp(token, "}")){
        es = statement();
        if(es > 0) return es;
    }
}

int statement(){
    int es = 0, symbolPos;

    if(es == 0 && strcmp(token, "if") == 0)
        es = if_stat();
    if(es == 0 && strcmp(token, "while") == 0)
        es = while_stat();
    if(es == 0 && strcmp(token, "for") == 0)
        es = for_stat();
    if(es == 0 && strcmp(token, "read") == 0)
        es = read_stat();
    if(es == 0 && strcmp(token, "write") == 0)
        es = write_stat();
    if(es == 0 && strcmp(token, "{") == 0)
        es = compound_stat();
    if(es == 0 && strcmp(token, "call") == 0)
        es = call_stat();
    if(es == 0 && (strcmp(token, "ID") == 0 || strcmp(token, "NUM") == 0 || strcmp(token, "(") == 0 )){
        es = expression_stat();
    }

    if(es == 0 && strcmp(token, "do") == 0)
        es = do_while_stat();
    if(es == 0 && strcmp(token, "switch") == 0)
        es = switch_stat();

    //拓展4
    if(es == 0 && strcmp(token, "return") == 0){
        fscanf(fpTokenin, "%s %s\n", token, token1);printf("token = %s, token1 = %s\n",token,token1);
        es = bool_expr();
        if(es > 0) return es;

        strcpy(codes[codesIndex].opt, "STO");
        es = lookup(function_name, &symbolPos, function_name);
        if(es > 0) return es;
        codes[codesIndex++].operand = symbol[symbolPos].address;
        fscanf(fpTokenin, "%s %s\n", token, token1);printf("token = %s, token1 = %s\n",token,token1);
    }


    return es;
}

int if_stat(){
    int es = 0, cx1, cx2;

    fscanf(fpTokenin, "%s %s\n", token, token1);printf("token = %s, token1 = %s\n",token,token1);
    if(strcmp(token, "(")) return (es = 5); //少左括号
    fscanf(fpTokenin, "%s %s\n", token, token1);printf("token = %s, token1 = %s\n",token,token1);
    es = expression();
    if(es > 0) return es;
    if(strcmp(token, ")")) return (es = 6); //少右括号
    strcpy(codes[codesIndex].opt, "BRF");
    cx1 = codesIndex++;
    fscanf(fpTokenin, "%s %s\n", token, token1);printf("token = %s, token1 = %s\n",token,token1);
    es = statement();
    if(es > 0) return es;

    strcpy(codes[codesIndex].opt, "BR");
    cx2 = codesIndex++;
    codes[cx1].operand = codesIndex;

    if(strcmp(token, "else") == 0){
        fscanf(fpTokenin, "%s %s\n", token, token1);printf("token = %s, token1 = %s\n",token,token1);
        es = statement();
        if(es > 0) return es;
    }
    codes[cx2].operand = codesIndex;
    return es;
}

int while_stat(){
    int es = 0;
    int cx1, cxEntrance;

    fscanf(fpTokenin, "%s %s\n", token, token1);printf("token = %s, token1 = %s\n",token,token1);
    if(strcmp(token, "(")) return (es = 5); //少左括号
    fscanf(fpTokenin, "%s %s\n", token, token1);printf("token = %s, token1 = %s\n",token,token1);
    cxEntrance = codesIndex;

    es = expression();
    if(es > 0) return es;
    if(strcmp(token, ")")) return (es = 6); //少右括号

    strcpy(codes[codesIndex].opt, "BRF");
    cx1 = codesIndex++;
    fscanf(fpTokenin, "%s %s\n", token, token1);printf("token = %s, token1 = %s\n",token,token1);
    es = statement();
    if(es > 0) return es;

    strcpy(codes[codesIndex].opt, "BR");
    codes[codesIndex].operand = cxEntrance;
    codesIndex++;
    codes[cx1].operand = codesIndex;
    return es;
}

int for_stat(){
    int es = 0;
    int cx1, cx2, cxExp2, cxExp3;

    fscanf(fpTokenin, "%s %s\n", token, token1);printf("token = %s, token1 = %s\n",token,token1);
    if(strcmp(token, "(")) return (es = 5); //少左括号
    fscanf(fpTokenin, "%s %s\n", token, token1);printf("token = %s, token1 = %s\n",token,token1);

    es = expression();
    if(es > 0) return es;
    if(strcmp(token, ";")) return (es = 4); //少分号

    cxExp2 = codesIndex;
    fscanf(fpTokenin, "%s %s\n", token, token1);printf("token = %s, token1 = %s\n",token,token1);
    es = expression();
    if(es > 0)
        return es;
    strcpy(codes[codesIndex].opt, "BRF");
    cx1 = codesIndex++;
    strcpy(codes[codesIndex].opt, "BR");
    cx2 = codesIndex++;
    if(strcmp(token, ";")) return (es = 4); //少分号

    cxExp3 = codesIndex;
    fscanf(fpTokenin, "%s %s\n", token, token1);printf("token = %s, token1 = %s\n",token,token1);
    es = expression();
    if(es > 0) return es;
    strcpy(codes[codesIndex].opt, "BR");
    codes[codesIndex].operand = cxExp2;
    codesIndex++;
    codes[cx2].operand = codesIndex;
    if(strcmp(token, ")")) return (es = 6); //少右括号

    fscanf(fpTokenin, "%s %s\n", token, token1);printf("token = %s, token1 = %s\n",token,token1);
    es = statement();
    if(es > 0) return es;
    strcpy(codes[codesIndex].opt, "BR");
    codes[codesIndex].operand = cxExp3;
    codesIndex++;
    codes[cx1].operand = codesIndex;
    return es;
}

int write_stat(){
    int es = 0;
    fscanf(fpTokenin, "%s %s\n", token, token1);printf("token = %s, token1 = %s\n",token,token1);
    es = expression();
    if(es > 0) return es;
    if(strcmp(token, ";")) return (es = 4); //少分号

    strcpy(codes[codesIndex++].opt, "OUT");
    fscanf(fpTokenin, "%s %s\n", token, token1);printf("token = %s, token1 = %s\n",token,token1);
    return es;
}

int read_stat(){
    int es = 0, symbolPos;

    fscanf(fpTokenin, "%s %s\n", token, token1);printf("token = %s, token1 = %s\n",token,token1);
    if(strcmp(token, "ID")) return (es = 3); //少标识符
    es = lookup(token1, &symbolPos, function_name);
    if(!symbol[symbolPos].kind == variable) return (es = 35);
    strcpy(codes[codesIndex++].opt, "IN");
    strcpy(codes[codesIndex].opt, "STO");
    codes[codesIndex++].operand = symbol[symbolPos].address;
    fscanf(fpTokenin, "%s %s\n", token, token1);printf("token = %s, token1 = %s\n",token,token1);
    if(strcmp(token, ";")) return (es = 4); //少分号
    fscanf(fpTokenin, "%s %s\n", token, token1);printf("token = %s, token1 = %s\n",token,token1);
    return es;
}

int compound_stat(){
    int es = 0;
    fscanf(fpTokenin, "%s %s\n", token, token1);printf("token = %s, token1 = %s\n",token,token1);
    es = statement_list();
    fscanf(fpTokenin, "%s %s\n", token, token1);printf("token = %s, token1 = %s\n",token,token1);
    return es;
}

int call_stat(){
    int es = 0, symbolPos;

    fscanf(fpTokenin, "%s %s\n", token, token1);printf("token = %s, token1 = %s\n",token,token1);
    if(strcmp(token, "ID")) return (es = 3); //少标识符

    strcpy(function_name, token1);
    es = lookup(token1, &symbolPos, function_name);
    if(!symbol[symbolPos].kind == function) return (es = 34);

    fscanf(fpTokenin, "%s %s\n", token, token1);printf("token = %s, token1 = %s\n",token,token1);
    if(strcmp(token, "(")) return (es = 5); //少左括号
    fscanf(fpTokenin, "%s %s\n", token, token1);printf("token = %s, token1 = %s\n",token,token1);
    if(strcmp(token, ")")) return (es = 6); //少右括号
    fscanf(fpTokenin, "%s %s\n", token, token1);printf("token = %s, token1 = %s\n",token,token1);
    if(strcmp(token, ";")) return (es = 4); //少分号
    fscanf(fpTokenin, "%s %s\n", token, token1);printf("token = %s, token1 = %s\n",token,token1);
    strcpy(codes[codesIndex].opt, "CAL");
    codes[codesIndex++].operand = symbol[symbolPos].address;
    return es;
}

int expression_stat(){
    int es = 0;

    if(strcmp(token, ";") == 0){
        fscanf(fpTokenin, "%s %s\n", token, token1);printf("token = %s, token1 = %s\n",token,token1);
        return es;
    }
    es = expression();
    if(es > 0) return es;
    if(strcmp(token, ";") == 0){
        fscanf(fpTokenin, "%s %s\n", token, token1);printf("token = %s, token1 = %s\n",token,token1);
        return es;
    }else return (es = 4);
}

int expression(){
    int es = 0, filladd;
    char token2[20], token3[40];
    int symbolPos;

    if(strcmp(token, "ID") == 0){
        filladd = ftell(fpTokenin);
        fscanf(fpTokenin, "%s %s\n", token2, token3);
        if(strcmp(token2, "=") == 0){
            es = lookup(token1, &symbolPos, function_name);
            if(es > 0) return es;
            if(!symbol[symbolPos].kind == variable) return (es = 36);
            fscanf(fpTokenin, "%s %s\n", token, token1);printf("token = %s, token1 = %s\n",token,token1);
            es = bool_expr();
            if(es > 0) return es;
            strcpy(codes[codesIndex].opt, "STO");
            codes[codesIndex++].operand = symbol[symbolPos].address;
        }else{
            fseek(fpTokenin, filladd, 0);
            es = bool_expr();
            if(es > 0) return es;
        }
    }else if(strcmp(token, "case") == 0){
        strcpy(token, "ID");
        strcpy(token1, token_expr);
        es = lookup(token1, &symbolPos, function_name);
        if(es > 0) return es;
        if(!symbol[symbolPos].kind == variable) return (es = 36);
        strcpy(codes[codesIndex].opt, "LOAD");
        codes[codesIndex++].operand = symbol[symbolPos].address;
        strcpy(token, "==");
        strcpy(token1, "==");
        es = N_bool_expr();
        if(es > 0) return es;
    }else es = bool_expr();
    return es;
}

int bool_expr(){
    int es = 0;

    es = additive_expr();
    if(es > 0) return es;
    if(strcmp(token, ">") == 0 || strcmp(token, ">=") == 0 || strcmp(token, "<") == 0 ||
       strcmp(token, "<=") == 0 || strcmp(token, "==") == 0 || strcmp(token, "!=") == 0 ){
        char token2[20];
        strcpy(token2, token);
        fscanf(fpTokenin, "%s %s\n", token, token1);printf("token = %s, token1 = %s\n",token,token1);
        es = additive_expr();
        if(es > 0) return es;

        if(strcmp(token2, ">") == 0)
            strcpy(codes[codesIndex++].opt, "GT");
        if(strcmp(token2, ">=") == 0)
            strcpy(codes[codesIndex++].opt, "GE");
        if(strcmp(token2, "<") == 0)
            strcpy(codes[codesIndex++].opt, "LES");
        if(strcmp(token2, "<=") == 0)
            strcpy(codes[codesIndex++].opt, "LE");
        if(strcmp(token2, "==") == 0)
            strcpy(codes[codesIndex++].opt, "EQ");
        if(strcmp(token2, "!=") == 0)
            strcpy(codes[codesIndex++].opt, "NOTEQ");
    }
    return es;
}

int N_bool_expr(){
    int es = 0;

    if(strcmp(token, "==") == 0 ){
        char token2[20];
        strcpy(token2, token);
        fscanf(fpTokenin, "%s %s\n", token, token1);printf("token = %s, token1 = %s\n",token,token1);
        es = additive_expr();
        if(es > 0) return es;

        if(strcmp(token2, "==") == 0)
            strcpy(codes[codesIndex++].opt, "EQ");
    }
    return es;
}

int additive_expr(){
    int es = 0;
    es = term();
    if(es > 0) return es;
    while(strcmp(token, "+") == 0 || strcmp(token, "-") == 0){
        char token2[20];
        strcpy(token2, token);
        fscanf(fpTokenin, "%s %s\n", token, token1);printf("token = %s, token1 = %s\n",token,token1);
        es = term();
        if(es > 0) return es;
        if(strcmp(token2, "+") == 0)
            strcpy(codes[codesIndex++].opt, "ADD");
        if(strcmp(token2, "-") == 0)
            strcpy(codes[codesIndex++].opt, "SUB");
    }
    return es;
}

int term(){
    int es = 0;

    es = factor();
    if(es > 0) return es;while(strcmp(token, "+") == 0 || strcmp(token, "-") == 0){
        char token2[20];
        strcpy(token2, token);
        fscanf(fpTokenin, "%s %s\n", token, token1);printf("token = %s, token1 = %s\n",token,token1);
        es = term();
        if(es > 0) return es;
        if(strcmp(token2, "+") == 0)
            strcpy(codes[codesIndex++].opt, "ADD");
        if(strcmp(token2, "-") == 0)
            strcpy(codes[codesIndex++].opt, "SUB");
    }
    while(strcmp(token, "*") == 0 || strcmp(token, "/") == 0){
        char token2[20];
        strcpy(token2, token);
        fscanf(fpTokenin, "%s %s\n", token, token1);printf("token = %s, token1 = %s\n",token,token1);
        es = term();
        if(es > 0) return es;
        if(strcmp(token2, "*") == 0)
            strcpy(codes[codesIndex++].opt, "MULT");
        if(strcmp(token2, "-") == 0)
            strcpy(codes[codesIndex++].opt, "DIV");
    }
    return es;
}

int factor(){
    int es = 0;
    char token2[20], token3[20];

    if(strcmp(token, "(") == 0){
        fscanf(fpTokenin, "%s %s\n", token, token1);printf("token = %s, token1 = %s\n",token,token1);
        es = additive_expr();
        if(es > 0) return es;
        if(strcmp(token, ")")) return (es = 6); //少右括号
        fscanf(fpTokenin, "%s %s\n", token, token1);printf("token = %s, token1 = %s\n",token,token1);
    }else{
        fscanf(fpTokenin, "%s %s\n", token2, token3);printf("token2 = %s, token3 = %s\n",token2,token3);
        if(strcmp(token2, "(") == 0){ //等号后是函数
            int symbolPos;
            es = lookup(token1, &symbolPos, token1);
            if(es > 0) return es;//没定义
            if(!symbol[symbolPos].kind == function) return (es = 37);
            strcpy(codes[codesIndex].opt, "LOAD");
            codes[codesIndex++].operand = symbol[symbolPos].address;
            fscanf(fpTokenin, "%s %s\n", token, token1);printf("token = %s, token1 = %s\n",token, token1);
            fscanf(fpTokenin, "%s %s\n", token, token1);printf("token = %s, token1 = %s\n",token, token1);
            return es;
        }else{
            if(strcmp(token, "ID") == 0){
                int symbolPos;
                es = lookup(token1, &symbolPos, function_name);
                if(es > 0) return es;//没定义
                if(!symbol[symbolPos].kind == variable) return (es = 37);
                strcpy(codes[codesIndex].opt, "LOAD");
                codes[codesIndex++].operand = symbol[symbolPos].address;
                strcpy(token, token2);
                strcpy(token1, token3);
                printf("token = %s, token1 = %s\n",token, token1);
                return es;
            }
            if(strcmp(token, "NUM") == 0){
                strcpy(codes[codesIndex].opt, "LOADI");
                codes[codesIndex++].operand = atoi(token1);
                strcpy(token, token2);
                strcpy(token1, token3);
                printf("token = %s, token1 = %s\n",token, token1);
                return es;
            }
            else return (es = 7);//缺少操作数
        }



    }
    return es;
}

//扫描到的标识符的定义性出现时，将其插入符号表
int insert_Symbol(enum Category_symbol category, char *name, char *func_name){
    int i, es = 0;
    char temp[20];
    strcpy(temp, func_name);
    if(symbolIndex >= maxsymbolIndex) return 21;
    switch(category){
    case function:
        for(i = symbolIndex-1; i >= 0; i--){//查符号表
            if(strcmp(symbol[i].name, name) == 0 && symbol[i].kind == function){
                es = 32;//表示函数名重复定义
                break;
            }
        }
        symbol[symbolIndex].kind = function;
        break;
    case variable:
        for(i = symbolIndex-1; i >= 0; i--){//查符号表
            if(strcmp(symbol[i].name, name) == 0 && symbol[i].kind == variable && strcmp(symbol[i].func_name, func_name) == 0){
                es = 22;//表示变量重复定义
                break;
            }
        }
        if(temp[0] == '\0') flag++;
        symbol[symbolIndex].kind = variable;
        symbol[symbolIndex].address = offset;
        offset++;//分配一个单元，数据区指针加1
        break;
    }
    if(es > 0) return es;
    strcpy(symbol[symbolIndex].name, name);
    strcpy(symbol[symbolIndex].func_name, func_name);
    symbolIndex++;
    return es;
}

//取得标识符在符号表中的位置
int lookup(char *name, int *pPosition, char *func_name){
    int i, es = 0;

    for(i = 0; i < symbolIndex; i++){
        if(strcmp(symbol[i].name, name) == 0 && (strcmp(symbol[i].func_name, func_name) == 0 || symbol[i].func_name[0] == '\0')){
            *pPosition = i;
            return es;
        }
    }
    return (es = 23);   //标识符没定义
}

int do_while_stat(){//拓展3
    int es = 0;
    int cx1, cxEntrance;

    cxEntrance = codesIndex;
    fscanf(fpTokenin, "%s %s\n", token, token1);printf("token = %s, token1 = %s\n",token,token1);
    es = statement();
    if(es > 0) return es;
    printf("test is loading....token = %s, token1 = %s\n",token,token1);

    if(strcmp(token, "while") == 0){
        fscanf(fpTokenin, "%s %s\n", token, token1);printf("token = %s, token1 = %s\n",token,token1);
        if(strcmp(token, "(")) return (es = 5); //少左括号
        fscanf(fpTokenin, "%s %s\n", token, token1);printf("token = %s, token1 = %s\n",token,token1);
        es = expression();
        if(es > 0) return es;
        if(strcmp(token, ")")) return (es = 6); //少右括号
        strcpy(codes[codesIndex].opt, "BRF");
        cx1 = codesIndex++;
        fscanf(fpTokenin, "%s %s\n", token, token1);printf("token = %s, token1 = %s\n",token,token1);
        if(strcmp(token, ";")) return (es = 4); //少分号
    }else{
        return (es = 233);
    }

    strcpy(codes[codesIndex].opt, "BR");
    codes[codesIndex].operand = cxEntrance;
    codesIndex++;
    codes[cx1].operand = codesIndex;
    fscanf(fpTokenin, "%s %s\n", token, token1);printf("token = %s, token1 = %s\n",token,token1);
    return es;
}

int switch_stat(){//拓展switch
    int es = 0, cx0;

    fscanf(fpTokenin, "%s %s\n", token, token1);printf("token = %s, token1 = %s\n",token,token1);
    if(strcmp(token, "(")) return (es = 5); //少左括号
    fscanf(fpTokenin, "%s %s\n", token, token1);printf("token = %s, token1 = %s\n",token,token1);
    strcpy(token_expr, token1);
    es = expression();
    if(es > 0) return es;
    if(strcmp(token, ")")) return (es = 6); //少右括号

    fscanf(fpTokenin, "%s %s\n", token, token1);printf("token = %s, token1 = %s\n",token,token1);
    if(strcmp(token, "{")) return (es = 11);

    int i = 0, cx[10], k;
    fscanf(fpTokenin, "%s %s\n", token, token1);printf("token = %s, token1 = %s\n",token,token1);
    while(strcmp(token, "case") == 0){
        //fscanf(fpTokenin, "%s %s\n", token, token1);printf("token = %s, token1 = %s\n",token,token1);
        es = expression();
        if(es > 0) return es;

        if(strcmp(token, ":")) {printf("switch case 缺少冒号！\n");return (es = 233);} //少冒号
        fscanf(fpTokenin, "%s %s\n", token, token1);printf("token = %s, token1 = %s\n",token,token1);

        strcpy(codes[codesIndex].opt, "BRF");
        cx[i++] = codesIndex++;

        fscanf(fpTokenin, "%s %s\n", token, token1);printf("token = %s, token1 = %s\n",token,token1);
        es = statement();
        if(es > 0) return es;

        strcpy(codes[codesIndex].opt, "BR");
        cx[i++] = codesIndex++;
        codes[(cx[i-2])].operand = codesIndex;
        fscanf(fpTokenin, "%s %s\n", token, token1);printf("token = %s, token1 = %s\n",token,token1);
    }
    while(i>=1){
        if(i%2)
            codes[(cx[i])].operand = codesIndex;
        i--;
    }
    fscanf(fpTokenin, "%s %s\n", token, token1);printf("token = %s, token1 = %s\n",token,token1);
    return es;
}

