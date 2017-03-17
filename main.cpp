#include "stdio.h"
#include "ctype.h"
#include "stdlib.h"
#include "string.h"
#include "Lexer.h"
#include "TESTparse.h"
#include "TESTmachine.h"

extern int TESTscan();
extern int TESTparse();
//char Scanin[300] = "F:\\Lexer\\in.txt", Scanout[300] = "F:\\Lexer\\out.txt";
extern char Errorfile[30], Codeout[30];
FILE *fin, *fout;
extern int TESTmachine();

int main(){
    int es = 0;
    printf("词法分析程序启动...\n");
    es = TESTscan();
    if( es > 0 ) printf("词法分析出错，编译停止！错误代码：%d \n", es);
    else printf("词法分析成功！\n\n");

    es = 0;
    printf("语法分析程序启动...\n");
    es = TESTparse();
    if(es == 0)
        printf("语法、语义分析并生成代码成功！\n\n");
    else
        printf("语法、语义分析并生成代码错误！\n\n");

    es = 0;
    printf("虚拟机启动...\n");
    es=TESTmachine();
    return 0;
}
