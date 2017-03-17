#include "stdio.h"
#include "ctype.h"
#include "stdlib.h"
#include "string.h"
//���涨�屣���ֱ�Ϊ�򻯳���ʹ���ַ�ָ�����鱣�����б�����
//��������ӱ����֣��ɼ�����ӣ����޸ı�������ĿkeywordSum
#define keywordSum 13
char *keyword[keywordSum] = { "call", "case", "do", "else", "for", "function", "if", "int", "read", "return", "switch", "while", "write" };
//���涨�崿���ֽ��������Ҫ�����
char singleword[50] = "+-*(){};,:\"\'";
//���涨��˫�ֽ�������ַ�
char doubleword[10] = "><=!";
char Scanin[300] = "in.txt", Scanout[300] = "out.txt";
//���ڽ�����������ļ�������test_main.c�ж���
extern FILE *fin, *fout;                            //����ָ����������ļ���ָ�룬��test_main.c�ж���

int midsearch(char **word_str, char word_temp[]);       //�۰����

int TESTscan(){                                     //�ʷ���������
    char ch, token[40];                             //chΪÿ�ζ�����ַ���token���ڱ���ʶ����ĵ���
    int es = 0, j, n;                               //esΪ������룬 0��ʾ�޴���
                                                    //j��nΪ��ʱ������������ϵ���ʱ���±��
    //printf("������ԭ�����ļ���������·������");
    //scanf("%s", Scanin);
    //printf("������ʷ���������ļ���������·������");
    //scanf("%s",Scanout);
    if( ( fin = fopen( Scanin, "r" ) ) == NULL ){   //�ж������ļ����Ƿ���ȷ
        printf("\n�򿪴ʷ����������ļ�����\n");
        return(1);                                  //�����ļ������ش������1
    }
    if( ( fout = fopen( Scanout, "w" ) ) == NULL ){    //�ж�����ļ����Ƿ���ȷ
        printf("\n�����ʷ���������ļ�����\n");
        return(2);                                  //����ļ������ش������2
    }
    ch = getc(fin);
    while( ch != EOF ){
        while( ch == ' ' || ch == '\n' || ch == '\t' ) ch = getc(fin);
        if( isalpha(ch) ){                          //�������ĸ������б�ʶ������
            token[0] = ch;
            j = 1;
            ch = getc(fin);
            while( isalnum(ch) ){                   //�������ĸ��������ϱ�ʶ��
                token[j++] = ch;                    //��ϵı�ʶ��������token��
                ch = getc(fin);                     //����һ���ַ�
            }
            token[j] = '\0';                        //��ʶ����Ͻ���
            //�鱣����
            n = midsearch( keyword, token );                                           //----------------�۰����--------------------------
            if( n == 1 )                  //���Ǳ����֣������ʶ��
                fprintf( fout, "%s\t%s\n", "ID", token );

            if( n == 0 )                                    //�Ǳ����֣����������
                fprintf( fout, "%s\t%s\n", token,token);
        }else if( isdigit(ch) ){                    //���ִ���
            token[0] = ch;
            j = 1;
            ch = getc(fin);                         //����һ���ַ�
            while( isdigit(ch) ){                   //������������������������������������Ͻ���
                token[j++] = ch;                    //�������������token��
                ch = getc(fin);                     //����һ���ַ�
            }
            token[j] = '\0';                        //������Ͻ���
            fprintf( fout, "%s\t%s\n", "NUM", token );      //�����������
         }else if( strchr( singleword, ch ) > 0 ){          //���ֽ������
             token[0] = ch;
             token[1] = '\0';
             ch = getc(fin);                                //����һ�������Ա�ʶ����һ������
             fprintf( fout, "%s\t%s\n", token, token );     //������ֽ������
         }else if( strchr( doubleword, ch ) > 0 ){          //˫�ֽ������
             token[0] = ch;
             ch = getc(fin);
             if( ch == '=' ){                               //�����=�����˫�ֽ��
                token[1] = ch;
                token[2] = '\0';
                ch = getc(fin);
             }else                                          //�������=����Ϊ���ֽ��
                token[1] = '\0';
             fprintf( fout, "%s\t%s\n", token, token );     //�������˫�ֽ������
         }else if( ch == '/' ){                             //ע�ʹ���
             ch = getc(fin);
             if( ch == '*' ){                               //�����*����ʼ����ע�ʹ���
                char ch1;
                ch1 = getc(fin);
                do{                                         //ɾ��ע������
                    ch = ch1;
                    ch1 = getc(fin);
                }while( ( ch != '*' || ch1 != '/' ) && ch1 != EOF );    //֪������*/�����ļ�β
                ch = getc(fin);
             }else{
                 token[0] = '/';
                 token[1] = '\0';
                 fprintf( fout, "%s\t%s\n", token, token );              //������ֽ��
             }
         }else if ( ch == '&' ){                                        //-----------         &&     start          --------------------------
             token[0] = '&';
             ch = getc(fin);
             if( ch == '&' ){
                token[1] = '&';
                token[2] = '\0';
                fprintf( fout, "%s\t%s\n", token, token );
                ch = getc(fin);
             }else{
                 token[1] = '\0';
                 fprintf( fout, "%s\t%s\n", token, token );
             }
         }else if ( ch == '|' ){                                        //-----------         ||     start          --------------------------
             token[0] = '|';
             ch = getc(fin);
             if( ch == '|' ){
                token[1] = '|';
                token[2] = '\0';
                fprintf( fout, "%s\t%s\n", token, token );
                ch = getc(fin);
             }else{
                 token[1] = '\0';
                 fprintf( fout, "%s\t%s\n", token, token );
             }
         }else{                                             //������//------------------         end          --------------------------
             token[0] = ch;
             token[1] = '\0';
             ch = getc(fin);
             es = 3;
             fprintf( fout, "%s\t%s\n", "ERROR", token );
         }
    }
    fclose(fin);
    fclose(fout);
    return(es);

}

int midsearch(char **word_str, char word_temp[]){
    int low = 0;
    int high = keywordSum;
    int i = ( low + high ) /2;
    while( low <= high ){
        if( strcmp( word_str[i], word_temp ) == 0 ){
            //printf("find it %s.\n", word_temp);
            return 0;
            break;
        }else if ( strcmp(word_str[i], word_temp )>0)
            high = i - 1;
        else
            low = i + 1;
        i = ( low + high )/2;
    }
    if( low > high ) return 1;
}
