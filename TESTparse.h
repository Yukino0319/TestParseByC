//Testparse �﷨�������������������
#include "stdio.h"
#include "ctype.h"
#include "conio.h"
#include "string.h"
#include "stdlib.h"

#define maxsymbolIndex 100      //������ű������

enum Category_symbol {variable, function};      //��ʶ��������

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

char token[20], token1[40];     //�������ֵ����������ֵ
char tokenfile[30] = "out.txt";             //�������ļ�������
char Codeout[30] = "MidOut.txt";               //Ҫ���ɵ��ı���ʽ���м�����ļ�������
char Codeout1[30] = "BMidOut.txt";               //���ɵĶ�������ʽ���м�����ļ�

FILE *fpTokenin;                //�������ļ�ָ��
FILE *fpCodeBinary;             //�м����������ļ�ָ��
FILE *fpCodeText;               //�м�����ı��ļ�ָ��

char function_name[20];     //��ǰ����������
int flag = 0;
char token_expr[20];

struct{                         //���ű�
    char name[20];
    char func_name[20];
    enum Category_symbol kind;
    int address;
} symbol[maxsymbolIndex];

typedef struct Code{
    char opt[10];               //������
    int operand;                //������
} Code;
Code codes[200];                //����м����Ľṹ������

int codesIndex;
//codes�����е�һ����Ԫ�ص��±꣬0����һ��Ҫ���ɵĴ�����codes�е�λ�ã�
int symbolIndex = 0;
//symbol�����еĵ�һ����Ԫ�ص��±꣬0����һ��Ҫ����ı�ʶ���ڷ��ϱ��е�λ�ã�
int offset;

//�ֲ������������庯���ڲ�����Ե�ַ

//�﷨������������������ɳ���
int TESTparse(){
    codesIndex = 0;
    int i;
    int es = 0;
    //printf("�����뵥�����ļ���������·������");
    //scanf("%s", tokenfile);
    if(( fpTokenin = fopen(tokenfile, "r")) == NULL){       //tokenfile��ʾ�ʷ������ɹ������ɵ�out.txt��fpTokeninָʾ�ļ�ָ��
        printf("\n��%s����\n", tokenfile);
        es = 10;
        return es;
    }
    es = program();             //���ó����﷨����
    fclose(fpTokenin);

    printf("==�﷨����������������ɳ�����==\n");
    switch (es){
        case 0:
            printf("�﷨��������ɹ��������������ɴ��룡\n");
            break;
        case 10:
            printf("\n��%s����\n", tokenfile);
            break;
        case 1:
            printf("ȱ��\"{\"��\n");
            break;
        case 2:
            printf("ȱ��\"}\"��\n");
            break;
        case 3:
            printf("ȱ�ٱ�ʶ����\n");
            break;
        case 4:
            printf("ȱ�ٷֺţ�\n");
            break;
        case 5:
            printf("ȱ��\"(\"��\n");
            break;
        case 6:
            printf("ȱ��\")\"��\n");
            break;
        case 7:
            printf("ȱ�ٲ�������\n");
            break;
        case 11:
            printf("������ͷȱ��\"{\"��\n");
            break;
        case 12:
            printf("��������ȱ��\"}\"��\n");
            break;
        case 13:
            printf("���һ������������Ӧ����main��\n");
            break;
        case 21:
            printf("���ű������\n");
            break;
        case 22:
            printf("����%s�ظ����壡\n", token1);
            break;
        case 23:
            printf("����δ������\n");
            break;
        case 24:
            printf("������main���������󣬻������������ַ���\n");
            break;
        case 32:
            printf("�������ظ����壡\n");
            break;
        case 34:
            printf("call������ı�ʶ��%s���Ǻ�������\n", token1);
            break;
        case 35:
            printf("read������ı�ʶ��%s���Ǳ�������\n", token1);
            break;
        case 36:
            printf("��ֵ������ֵ%s���Ǳ�������\n", token1);
            break;
        case 37:
            printf("���Ӷ�Ӧ�ı�ʶ�����Ǳ�������\n");
            break;
        default:
            printf("����ԭ�����");
            break;
    }
    //printf("������Ҫ���ɵ��ı���ʽ���м�����ļ������֣�����·������");
    //scanf("%s", Codeout);
    if((fpCodeText = fopen(Codeout, "w")) == NULL ){                //�м���������ļ�������������ļ���codeout��ʾ�м�����ļ���fpCodeTextָʾ�ļ�ָ��
        printf("\n����%s����\n", Codeout);
        es = 10;
        return es;
    }
    for(i = 0; i < codesIndex; i++){// codesindex��ʾcodes�����е�һ����Ԫ�ص��±꣬0����һ��Ҫ���ɵĴ�����codes�е�λ�ã�
        if(strcmp(codes[i].opt, "LOAD") == 0 || strcmp(codes[i].opt, "LOADI") == 0 || strcmp(codes[i].opt, "STO") == 0 ||
           strcmp(codes[i].opt, "BR") == 0 || strcmp(codes[i].opt, "BRF" ) == 0 || strcmp(codes[i].opt, "CAL") == 0 ||
           strcmp(codes[i].opt, "ENTER") == 0 )
            fprintf(fpCodeText, "%3d %-5s %d\n", i, codes[i].opt, codes[i].operand);
        else
            fprintf(fpCodeText, "%3d %-5s\n", i, codes[i].opt);
    }
    fclose(fpCodeText);

    //printf("������Ҫ���ɵĶ�������ʽ���м�����ļ������֣��ṹ��洢����");
    //scanf("%s", Codeout);
    if((fpCodeBinary = fopen(Codeout1, "wb")) == NULL ){             //�����������ļ���codeout��ʾ�м�����ļ���fpCodeBinaryָʾ�ļ�ָ��
        printf("\n����%s����\n", Codeout1);
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
    //codes����ĵ�0��ָ����������ת�Ƶ�main��������ڣ���ڵ�ַ��Ҫ����

    fscanf(fpTokenin, "%s %s\n", token, token1);printf("token = %s, token1 = %s\n",token,token1);


    //��չ6
    while(!strcmp(token, "int")){  //�ж��Ƿ��ǡ�ȫ�ֱ����Ķ��塰
        es = declaration_stat1();
        if(es != 0)
            return es;
    }

    while(!strcmp(token, "function")){  //�ж��Ƿ��ǡ���ͨ�����Ķ��塰
        fscanf(fpTokenin, "%s %s\n", token, token1);printf("token = %s, token1 = %s\n",token,token1);
        es = fun_declaration();
        if(es != 0)
            return es;
        fscanf(fpTokenin, "%s %s\n", token, token1);printf("token = %s, token1 = %s\n",token,token1);
    }
        if(strcmp(token, "ID")){        //��������������main�����Ķ��壬�����ֵΪ��ID��
            es = 1;
            return es;
        }
        if(strcmp(token1, "main")){        //��������������main�����Ķ��壬�����ֵΪ��ID��
            es = 13;
            return es;
        }
        fscanf(fpTokenin, "%s %s\n", token, token1);printf("token = %s, token1 = %s\n",token,token1);

        es = main_declaration();

        if(es > 0)
            return es;
        if(!feof(fpTokenin))            //���Դ�����У�������������������
            return 24;

        //������ű������
        printf("���ű�\n");
        printf("����\t \t����\t��ַ\t����\n");
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
    //��չ1  �½�char function_name[20]�������浱ǰ��function��

    fscanf(fpTokenin, "%s %s\n", token, token1);printf("token = %s, token1 = %s\n",token,token1);
    if(strcmp(token, "(")) return (es = 5); //��������
    fscanf(fpTokenin, "%s %s\n", token, token1);printf("token = %s, token1 = %s\n",token,token1);
    if(strcmp(token, ")")) return (es = 6); //��������
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
    if(strcmp(token, "ID")) return (es = 3); //�ٱ�ʶ��

    es = insert_Symbol(variable, token1, function_name);

    if(es > 0) return es;
    fscanf(fpTokenin, "%s %s\n", token, token1);printf("token = %s, token1 = %s\n",token,token1);
    if(strcmp(token, ";")) return (es = 4); //�ٷֺ�
    fscanf(fpTokenin, "%s %s\n", token, token1);printf("token = %s, token1 = %s\n",token,token1);
    return es;
}

int declaration_stat1(){//��չ7
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

    //��չ4
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
    if(strcmp(token, "(")) return (es = 5); //��������
    fscanf(fpTokenin, "%s %s\n", token, token1);printf("token = %s, token1 = %s\n",token,token1);
    es = expression();
    if(es > 0) return es;
    if(strcmp(token, ")")) return (es = 6); //��������
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
    if(strcmp(token, "(")) return (es = 5); //��������
    fscanf(fpTokenin, "%s %s\n", token, token1);printf("token = %s, token1 = %s\n",token,token1);
    cxEntrance = codesIndex;

    es = expression();
    if(es > 0) return es;
    if(strcmp(token, ")")) return (es = 6); //��������

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
    if(strcmp(token, "(")) return (es = 5); //��������
    fscanf(fpTokenin, "%s %s\n", token, token1);printf("token = %s, token1 = %s\n",token,token1);

    es = expression();
    if(es > 0) return es;
    if(strcmp(token, ";")) return (es = 4); //�ٷֺ�

    cxExp2 = codesIndex;
    fscanf(fpTokenin, "%s %s\n", token, token1);printf("token = %s, token1 = %s\n",token,token1);
    es = expression();
    if(es > 0)
        return es;
    strcpy(codes[codesIndex].opt, "BRF");
    cx1 = codesIndex++;
    strcpy(codes[codesIndex].opt, "BR");
    cx2 = codesIndex++;
    if(strcmp(token, ";")) return (es = 4); //�ٷֺ�

    cxExp3 = codesIndex;
    fscanf(fpTokenin, "%s %s\n", token, token1);printf("token = %s, token1 = %s\n",token,token1);
    es = expression();
    if(es > 0) return es;
    strcpy(codes[codesIndex].opt, "BR");
    codes[codesIndex].operand = cxExp2;
    codesIndex++;
    codes[cx2].operand = codesIndex;
    if(strcmp(token, ")")) return (es = 6); //��������

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
    if(strcmp(token, ";")) return (es = 4); //�ٷֺ�

    strcpy(codes[codesIndex++].opt, "OUT");
    fscanf(fpTokenin, "%s %s\n", token, token1);printf("token = %s, token1 = %s\n",token,token1);
    return es;
}

int read_stat(){
    int es = 0, symbolPos;

    fscanf(fpTokenin, "%s %s\n", token, token1);printf("token = %s, token1 = %s\n",token,token1);
    if(strcmp(token, "ID")) return (es = 3); //�ٱ�ʶ��
    es = lookup(token1, &symbolPos, function_name);
    if(!symbol[symbolPos].kind == variable) return (es = 35);
    strcpy(codes[codesIndex++].opt, "IN");
    strcpy(codes[codesIndex].opt, "STO");
    codes[codesIndex++].operand = symbol[symbolPos].address;
    fscanf(fpTokenin, "%s %s\n", token, token1);printf("token = %s, token1 = %s\n",token,token1);
    if(strcmp(token, ";")) return (es = 4); //�ٷֺ�
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
    if(strcmp(token, "ID")) return (es = 3); //�ٱ�ʶ��

    strcpy(function_name, token1);
    es = lookup(token1, &symbolPos, function_name);
    if(!symbol[symbolPos].kind == function) return (es = 34);

    fscanf(fpTokenin, "%s %s\n", token, token1);printf("token = %s, token1 = %s\n",token,token1);
    if(strcmp(token, "(")) return (es = 5); //��������
    fscanf(fpTokenin, "%s %s\n", token, token1);printf("token = %s, token1 = %s\n",token,token1);
    if(strcmp(token, ")")) return (es = 6); //��������
    fscanf(fpTokenin, "%s %s\n", token, token1);printf("token = %s, token1 = %s\n",token,token1);
    if(strcmp(token, ";")) return (es = 4); //�ٷֺ�
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
        if(strcmp(token, ")")) return (es = 6); //��������
        fscanf(fpTokenin, "%s %s\n", token, token1);printf("token = %s, token1 = %s\n",token,token1);
    }else{
        fscanf(fpTokenin, "%s %s\n", token2, token3);printf("token2 = %s, token3 = %s\n",token2,token3);
        if(strcmp(token2, "(") == 0){ //�Ⱥź��Ǻ���
            int symbolPos;
            es = lookup(token1, &symbolPos, token1);
            if(es > 0) return es;//û����
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
                if(es > 0) return es;//û����
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
            else return (es = 7);//ȱ�ٲ�����
        }



    }
    return es;
}

//ɨ�赽�ı�ʶ���Ķ����Գ���ʱ�����������ű�
int insert_Symbol(enum Category_symbol category, char *name, char *func_name){
    int i, es = 0;
    char temp[20];
    strcpy(temp, func_name);
    if(symbolIndex >= maxsymbolIndex) return 21;
    switch(category){
    case function:
        for(i = symbolIndex-1; i >= 0; i--){//����ű�
            if(strcmp(symbol[i].name, name) == 0 && symbol[i].kind == function){
                es = 32;//��ʾ�������ظ�����
                break;
            }
        }
        symbol[symbolIndex].kind = function;
        break;
    case variable:
        for(i = symbolIndex-1; i >= 0; i--){//����ű�
            if(strcmp(symbol[i].name, name) == 0 && symbol[i].kind == variable && strcmp(symbol[i].func_name, func_name) == 0){
                es = 22;//��ʾ�����ظ�����
                break;
            }
        }
        if(temp[0] == '\0') flag++;
        symbol[symbolIndex].kind = variable;
        symbol[symbolIndex].address = offset;
        offset++;//����һ����Ԫ��������ָ���1
        break;
    }
    if(es > 0) return es;
    strcpy(symbol[symbolIndex].name, name);
    strcpy(symbol[symbolIndex].func_name, func_name);
    symbolIndex++;
    return es;
}

//ȡ�ñ�ʶ���ڷ��ű��е�λ��
int lookup(char *name, int *pPosition, char *func_name){
    int i, es = 0;

    for(i = 0; i < symbolIndex; i++){
        if(strcmp(symbol[i].name, name) == 0 && (strcmp(symbol[i].func_name, func_name) == 0 || symbol[i].func_name[0] == '\0')){
            *pPosition = i;
            return es;
        }
    }
    return (es = 23);   //��ʶ��û����
}

int do_while_stat(){//��չ3
    int es = 0;
    int cx1, cxEntrance;

    cxEntrance = codesIndex;
    fscanf(fpTokenin, "%s %s\n", token, token1);printf("token = %s, token1 = %s\n",token,token1);
    es = statement();
    if(es > 0) return es;
    printf("test is loading....token = %s, token1 = %s\n",token,token1);

    if(strcmp(token, "while") == 0){
        fscanf(fpTokenin, "%s %s\n", token, token1);printf("token = %s, token1 = %s\n",token,token1);
        if(strcmp(token, "(")) return (es = 5); //��������
        fscanf(fpTokenin, "%s %s\n", token, token1);printf("token = %s, token1 = %s\n",token,token1);
        es = expression();
        if(es > 0) return es;
        if(strcmp(token, ")")) return (es = 6); //��������
        strcpy(codes[codesIndex].opt, "BRF");
        cx1 = codesIndex++;
        fscanf(fpTokenin, "%s %s\n", token, token1);printf("token = %s, token1 = %s\n",token,token1);
        if(strcmp(token, ";")) return (es = 4); //�ٷֺ�
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

int switch_stat(){//��չswitch
    int es = 0, cx0;

    fscanf(fpTokenin, "%s %s\n", token, token1);printf("token = %s, token1 = %s\n",token,token1);
    if(strcmp(token, "(")) return (es = 5); //��������
    fscanf(fpTokenin, "%s %s\n", token, token1);printf("token = %s, token1 = %s\n",token,token1);
    strcpy(token_expr, token1);
    es = expression();
    if(es > 0) return es;
    if(strcmp(token, ")")) return (es = 6); //��������

    fscanf(fpTokenin, "%s %s\n", token, token1);printf("token = %s, token1 = %s\n",token,token1);
    if(strcmp(token, "{")) return (es = 11);

    int i = 0, cx[10], k;
    fscanf(fpTokenin, "%s %s\n", token, token1);printf("token = %s, token1 = %s\n",token,token1);
    while(strcmp(token, "case") == 0){
        //fscanf(fpTokenin, "%s %s\n", token, token1);printf("token = %s, token1 = %s\n",token,token1);
        es = expression();
        if(es > 0) return es;

        if(strcmp(token, ":")) {printf("switch case ȱ��ð�ţ�\n");return (es = 233);} //��ð��
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

