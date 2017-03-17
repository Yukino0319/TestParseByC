#include "stdio.h"
#include "ctype.h"
#include "stdlib.h"
#include "string.h"
//下面定义保留字表，为简化程序，使用字符指针数组保存所有保留字
//如果想增加保留字，可继续添加，并修改保留字数目keywordSum
#define keywordSum 13
char *keyword[keywordSum] = { "call", "case", "do", "else", "for", "function", "if", "int", "read", "return", "switch", "while", "write" };
//下面定义纯单分界符，如需要可添加
char singleword[50] = "+-*(){};,:\"\'";
//下面定义双分界符的首字符
char doubleword[10] = "><=!";
char Scanin[300] = "in.txt", Scanout[300] = "out.txt";
//用于接收输入输出文件名，在test_main.c中定义
extern FILE *fin, *fout;                            //用于指向输入输出文件的指针，在test_main.c中定义

int midsearch(char **word_str, char word_temp[]);       //折半查找

int TESTscan(){                                     //词法分析函数
    char ch, token[40];                             //ch为每次读入的字符，token用于保存识别出的单词
    int es = 0, j, n;                               //es为错误代码， 0表示无错误
                                                    //j，n为临时变量，控制组合单词时的下标等
    //printf("请输入原程序文件名（包括路径）：");
    //scanf("%s", Scanin);
    //printf("请输入词法分析输出文件名（包括路径）：");
    //scanf("%s",Scanout);
    if( ( fin = fopen( Scanin, "r" ) ) == NULL ){   //判断输入文件名是否正确
        printf("\n打开词法分析输入文件出错！\n");
        return(1);                                  //输入文件出错返回错误代码1
    }
    if( ( fout = fopen( Scanout, "w" ) ) == NULL ){    //判断输出文件名是否正确
        printf("\n创建词法分析输出文件出错！\n");
        return(2);                                  //输出文件出错返回错误代码2
    }
    ch = getc(fin);
    while( ch != EOF ){
        while( ch == ' ' || ch == '\n' || ch == '\t' ) ch = getc(fin);
        if( isalpha(ch) ){                          //如果是字母，则进行标识符处理
            token[0] = ch;
            j = 1;
            ch = getc(fin);
            while( isalnum(ch) ){                   //如果是字母数字则组合标识符
                token[j++] = ch;                    //组合的标识符保存在token中
                ch = getc(fin);                     //读下一个字符
            }
            token[j] = '\0';                        //标识符组合结束
            //查保留字
            n = midsearch( keyword, token );                                           //----------------折半查找--------------------------
            if( n == 1 )                  //不是保留字，输出标识符
                fprintf( fout, "%s\t%s\n", "ID", token );

            if( n == 0 )                                    //是保留字，输出保留字
                fprintf( fout, "%s\t%s\n", token,token);
        }else if( isdigit(ch) ){                    //数字处理
            token[0] = ch;
            j = 1;
            ch = getc(fin);                         //读下一个字符
            while( isdigit(ch) ){                   //如果是数字则整合整数，如果不是则整数组合结束
                token[j++] = ch;                    //组合整数保存在token中
                ch = getc(fin);                     //读下一个字符
            }
            token[j] = '\0';                        //整数组合结束
            fprintf( fout, "%s\t%s\n", "NUM", token );      //输出整数符号
         }else if( strchr( singleword, ch ) > 0 ){          //单分界符处理
             token[0] = ch;
             token[1] = '\0';
             ch = getc(fin);                                //读下一个符号以便识别下一个单词
             fprintf( fout, "%s\t%s\n", token, token );     //输出单分界符符号
         }else if( strchr( doubleword, ch ) > 0 ){          //双分界符处理
             token[0] = ch;
             ch = getc(fin);
             if( ch == '=' ){                               //如果是=，组合双分界符
                token[1] = ch;
                token[2] = '\0';
                ch = getc(fin);
             }else                                          //如果不是=，则为单分界符
                token[1] = '\0';
             fprintf( fout, "%s\t%s\n", token, token );     //输出单、双分界符符号
         }else if( ch == '/' ){                             //注释处理
             ch = getc(fin);
             if( ch == '*' ){                               //如果是*，开始进行注释处理
                char ch1;
                ch1 = getc(fin);
                do{                                         //删除注释内容
                    ch = ch1;
                    ch1 = getc(fin);
                }while( ( ch != '*' || ch1 != '/' ) && ch1 != EOF );    //知道遇到*/或者文件尾
                ch = getc(fin);
             }else{
                 token[0] = '/';
                 token[1] = '\0';
                 fprintf( fout, "%s\t%s\n", token, token );              //输出单分界符
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
         }else{                                             //错误处理//------------------         end          --------------------------
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
