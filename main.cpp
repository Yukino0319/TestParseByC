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
    printf("�ʷ�������������...\n");
    es = TESTscan();
    if( es > 0 ) printf("�ʷ�������������ֹͣ��������룺%d \n", es);
    else printf("�ʷ������ɹ���\n\n");

    es = 0;
    printf("�﷨������������...\n");
    es = TESTparse();
    if(es == 0)
        printf("�﷨��������������ɴ���ɹ���\n\n");
    else
        printf("�﷨��������������ɴ������\n\n");

    es = 0;
    printf("���������...\n");
    es=TESTmachine();
    return 0;
}
