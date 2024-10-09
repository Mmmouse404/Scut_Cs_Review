%{
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
int line=1;
int num=0;
char name[100][20];
%}

letter      [A-Za-z] 
digit       [0-9]     

identifier {letter}({letter}|{digit})*  
NUMBER  [1-9][0-9]*|[0]                   
ALPHA  '[A-Za-z0-9+\-*/=<>()\[\]:.,;]*'      

ERRCHAR [^A-Za-z0-9+\-*/=<>()\[\]:.,;']    
BADCHAR '[A-Za-z0-9+\-*/=<>()\[\]:.,;]*   
%%
[ \t\r\a]+     ;
[\n]           line++; 
and          return 1;
array        return 2;
begin        return 3;
bool         return 4;
call         return 5;
case         return 6;
char         return 7;
constant     return 8;
dim          return 9;
do           return 10;
else         return 11;
end          return 12;
false        return 13;
for          return 14;
if           return 15;
input        return 16;
integer      return 17;
not          return 18;
of           return 19;
or           return 20;
output       return 21;
procedure    return 22;
program      return 23;
read         return 24;
real         return 25;
repeat       return 26;
set          return 27;
stop         return 28;
then         return 29;
to           return 30;
true         return 31;
until        return 32;
var          return 33;
while        return 34;
write        return 35;
{identifier} return 36; // 标识符
{NUMBER}        return 37; //整数
{ALPHA}         return 38; //字符常量
"("          return 39;
")"          return 40;
"*"          return 41;
"*/"         return 42;
"+"          return 43;
","          return 44;
"-"          return 45;
"."          return 46;
".."         return 47;
"/"          return 48;
"/*"         return 49;
":"          return 50;
":="         return 51;
";"          return 52;
"<"          return 53;
"<="         return 54;
"<>"         return 55;
"="          return 56;
">"          return 57;
">="         return 58;
"["          return 59;
"]"          return 60;
{ERRCHAR}    return -1;
{BADCHAR}    return -5;


%%

void print(int a,int*b ,char name[100][20])
{
   if(a==36||a==37||a==38)
   {
       int i=0;
       for(;i<*b;i++){ if(strcmp(yytext,name[i])==0) break; }
       if(i==*b) 
       {
          strcpy(name[*b],yytext);//用一个int*存储分别对应的关键字，关键字用name作比较，存在int*b数组里
          (*b)++;
       }
       printf("(%d, %d)\t", a, i+1);
   }
   else printf("(%d,-)\t", a);
}

int main() {
   printf("黄鸿展 计科2班 202230441138\n");
   printf("请输入文件名字：\n");
   char filename[100];
   scanf("%s",&filename);
   FILE* f=fopen(filename, "r");//只读形式打开
   if ( !f) 
   {
      perror("Fail to open file!");
    }
	
	
    yyin=f;
    int type;
    int flag=0;//查看是否处于注释中
    int lnum=0;//注释状态所在行
    int k=0;
	
    while (type = yylex())
    {
        if(type==49 && !flag)
        {
           flag=1;
           lnum=line;
           continue;
        }
        if(flag && line==lnum && type!=42) continue;  //如果未遇到右注释号则无视输入
        else if(flag && line==lnum && type==42) {flag=0; continue;}
        if(flag && line==lnum+1) //上一行注释缺少右注释号
        {
           flag=0;
           printf("第 %d 行缺少右括号\n",lnum);
           print(type,&num,name);
           k++;
           if(k%5==0) printf("\n");//每行输出5个
           continue;
        }
        if(type==-1) 
        {
           printf("第 %d 行出现非法字符\n",line);
           continue;
        }
        if(type==-5)
        {
           printf("第 %d 行缺少右引号 \n",line);
           continue;
        }
        print(type,&num,name);
        k++;
        if(k%5==0) printf("\n");

    }
    system("pause");
	yywrap();
}

int yywrap() {
   return 0;
}
