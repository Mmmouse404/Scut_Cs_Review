%{
#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <math.h>
#define MAXSTR 20 //最大字符串长度
#define MAXMEMBER 100 //最多存放的数字
//INT类型和REAL类型
#define INTTYPE  0
#define REALTYPE 1

extern int yylex();
extern FILE* yyin;
extern char str1[20];//用于存放yytext里面的字符串值

//实现拉链回填和生成四元式的关键函数
int FillType(int First,int type);
int Merge(int,int);
void BackPatch(int,int);
int GenerateQ(char*,int,int,int);
void PrintQ(void);
int yyparse();
void PrintList(void);
int yyerror(char*);
int Entry(char*);
int VarCount=0;
FILE* fp;
int QL=1;  /* 用于记录四元式数*/


struct QUATERLIST{
    char op[6];int arg1,arg2,result;
    } QuaterList[MAXMEMBER];
struct VARLIST{
	char name[20];
	int type;
	} VarList[MAXMEMBER];

%}
%start    ProgDef
%union {
 int Iv; //变量类型
 int CH; //各个表达式的语义值
 int NO; //变量，赋值语句的语义值，无特别用意
 struct { int TC,FC;} _BExpr;//布尔表达式(真出口/假出口)
 struct { int QUAD,CH;} _WBD;//While Do的循环控制结构，QUAD跳转以及CH本身语义值（可存放下一层的出口）
 struct { int type,place;} _Expr;// 算术表达式含类型和名
 char _Rop[5]; //操作字符数组
 int Last;    //以最后一个作为判断类型
 char str[20]; //存放字符串
}

/*常量const  */

%token    	Iden    500
%token    	IntNo	501
%token	  	RealNo  502

/*关键字*/
%token	  	Program 600
%token	  	Begin	601
%token	  	End	    602
%token	  	Var	    603
%token	  	Integer 604
%token	  	Real	605
%token	  	While	606

%token		If	    607
%token		Else	608
%token		For 	609
%token		To 		610
%token 		Down 	611
%token      Repeat  612
%token      Until   613
%token      Do      614
%token      And     615
%token      Or      616
%token      Not     617
%token      Then    618
/*双对象操作数 */
%token		LE	700
%token		GE	701
%token		NE	702
%token      ASSIGN 703
%left  Or
%left  And
%right Not
%nonassoc '<' '>' '=' LE GE NE 
%left '+' '-'
%left '*' '/' '%'
%nonassoc UMINUS

%type <CH> ProgDef
%type <CH> SubProg
%type <CH> VarDef
%type <CH> VarDefList
%type <Iv> Type
%type <Last> VarListS
%type <Last> VarT
%type <CH> Statement
%type <CH> StateList
%type <CH> StateLists
%type <CH> ComplexState
%type <_WBD> AsignState
%type <CH> I_T_E
%type <CH> I_T
%type <_WBD> R_U
%type <_WBD> Wd
%type <_WBD> F_B_A
%type <_Rop> RelationOp
%type <_Expr> Expr 
%type <CH> W
%type <CH> F
%type <CH> R
%type <CH> U
%type <NO> Varable 
%type <NO> Asign_Varable
%type <_Expr> Const 
%type <_BExpr>	BoolExpr



%%
/* <程序> → program <标识符> ；<变量说明> <复合语句> . (SubProg)
   <复合句> → begin <语句表> end */
ProgDef	:	Program Iden ';' SubProg
			{
				$$=0;printf("\n编译成功!\n");
			}
	;
SubProg:   VarDef Begin StateLists End
		{
		 BackPatch($3,QL);
		 GenerateQ("sys",0,0,0);
	    }
	;

/* <变量说明> → var <变量定义>│ε */
VarDef	:Var VarDefList 
	     {$$=0;}
	;
/* <变量定义> → <标识符表> ：<类型> ；<变量定义>│<标识符表> ：<类型> ；
   <标识符表> → <标识符> ，<标识符表>│<标识符> */
VarDefList:	VarDefList VarListS ';'{$$=0;}
	|	VarListS ';'{$$=0;}
	;
Type	:	Integer{$$=INTTYPE;}
	|	Real{$$=REALTYPE;}
	;
VarListS	:  
      VarT ':'  Type { $$=$3;$1=$3; FillType($1,$3);}
	/* |  Varable ':' '=' Type Expr {
			$$=$4;
			FillType($4,$1);
			if(VarList[$1].type==$5.type)
			   GenerateQ(":=",$5.place,0,$1);
			 else if($5.type==INTTYPE)
			 {
			   int T=NewTemp();
			   GenerateQ("I->R",$5.place,0,T);
			   GenerateQ(":=",T,0,$1);
			 }
			else
			{
			 int T=NewTemp();
			 GenerateQ("R->I",$5.place,0,T);
			 GenerateQ(":=",T,0,$1);
			}
		} */
	;
VarT   : VarT ',' Varable{FillType($3,$$);}
       | Varable{FillType($1,$$);}
	;

/*<语句表> → <语句> ；<语句表>│<语句>*/
StateLists: StateList ';'{BackPatch($1,QL);$$=0;}//回填BoolExper.FC，QL为跳出while循环的第一个四元式
	|       StateList    {BackPatch($1,QL);$$=0;}//考虑到后面条件控制语句没有分号，且拉链回填方便作左循环
	;
StateList:Statement {$$=$1;}
	|StateLists Statement 
	        {$$=$2;}
	;
/* <语句> → <赋值句>│<if句>│<while句>│<repeat句>│<复合句> */
Statement:	AsignState{$$=0;}
	|	I_T_E ComplexState{$$=Merge($1,$2);}
	|	I_T   ComplexState{$$=Merge($1,$2);}//将if语句中BoorExpr.FC对应的四元式与ComplexState合并，得到四元式(其中跳转位置为ComplexState四元式)。
	|	Wd ComplexState   { GenerateQ("j",0,0,$1.QUAD);
		  BackPatch($2,$1.QUAD);
		  $$=$1.CH;}//产生第一符号的四元式，将第一非终结符尾链（真出口）指向复合句，将while的语义值上传回填
	|	F_B_A	ComplexState	{ GenerateQ("j",0,0,$1.QUAD);
		  BackPatch($2,$1.QUAD);
		  $$=$1.CH;}
	|   R_U                     {  
                            $$ = $1.CH;
							}
	;

/* <赋值句> → <标识符> := <算术表达式> */
AsignState:	Asign_Varable ASSIGN Expr  
            {
            $$.QUAD=$1;
            $$.CH=QL-1;
			 if(VarList[$1].type==$3.type)
			   GenerateQ(":=",$3.place,0,$1);
			 else if($3.type==INTTYPE)//类型不同，追随变量的类型
			 {
			   int T=NewTemp();
			   GenerateQ("I->R",$3.place,0,T);
			   GenerateQ(":=",T,0,$1);
			 }
			else
			{
			 int T=NewTemp();
			 GenerateQ("R->I",$3.place,0,T);
			 GenerateQ(":=",T,0,$1);
			}
	      }
	;
Asign_Varable:  Varable {$$=$1;}
	;

/* <if句>→ if <布尔表达式> then <语句>│if <布尔表达式> then <语句> else <语句> */	
I_T_E	:	I_T ComplexState Else{int q=QL;GenerateQ("j",0,0,0);BackPatch($1,QL);$$=Merge($2,q);}//填加了一句无条件跳转，BoolExpr为真时跳到else下一行的四元式
	;
I_T	:	If  BoolExpr Then {BackPatch($2.TC,QL);$$=$2.FC;}//将QL回填到下一个四元式的结果值，并且将BoolExpr.FC值返回到上一层
	;

/* <while句> → while <布尔表达式> do <语句> */	
Wd	:	W  BoolExpr Do {BackPatch($2.TC,QL);$$.QUAD=$1;$$.CH=$2.FC;}//先对BoolExpr的TC四元式进行回填，再将QL与BoolExpr.FC传到上一层
	;
W	:	While{$$=QL;}//将QL传到上一层
	;

/*for语句 <for> -> for '(' <布尔表达式> ',' <赋值语句> ')'*/
F_B_A:	F '(' BoolExpr ',' AsignState ')' {BackPatch($3.TC,$5.CH);$$.QUAD=$1;$$.CH=$3.FC;}
	;
F 	:	For{$$=QL;}
	;

/* <repeat句> → repeat <语句> until <布尔表达式> */	
R_U:	
    R ComplexState U BoolExpr {
		$$.QUAD=$3;
		BackPatch($4.FC,$1);//FC回到R的QL
		$$.CH=$4.TC;//需要回填的是TC
		BackPatch($2,$$.QUAD); //执行完回到入口判断（Until部分）
        }
    ;
R   :Repeat{$$=QL;}
U   :Until{$$=QL;}  

/*复合句在SIMPLE语言下没有花括号分隔*/
ComplexState:	 Statement    {	$$=$1;	}
	;

/* <算术表达式> → <算术表达式> + <项>│<算术表达式> - <项>│<项> */
Expr	:	Expr'+'Expr
		{
		 int T=NewTemp();
		 if($1.type== INTTYPE && $3.type==INTTYPE)
		 {
		  GenerateQ("+",$1.place,$3.place,T);
		  VarList[T].type=$$.type=INTTYPE;
		 }
		 else if($1.type== REALTYPE && $3.type==REALTYPE)
		 {
		  GenerateQ("+",$1.place,$3.place,T);
		  VarList[T].type=$$.type=REALTYPE;
		 }
		 else if($1.type== INTTYPE)//考虑了类型转化，逻辑同赋值，类型跟随后面的类型
		 {
		  int U=NewTemp();
		  GenerateQ("I->R",$1.place,0,U);
		  GenerateQ("+",U,$3.place,T);
		  VarList[T].type=$$.type=REALTYPE;
		 }
		 else
		 {
		  int U=NewTemp();
		  GenerateQ("I->R",$3.place,0,U);
		  GenerateQ("+",$3.place,U,T);
		  VarList[T].type=$$.type=REALTYPE;
		 }
		 $$.place=T;
		}

	|	Expr'-'Expr
                {
		 int T=NewTemp();
		 if($1.type== INTTYPE && $3.type==INTTYPE)
		 {
		  GenerateQ("-",$1.place,$3.place,T);
		  VarList[T].type=$$.type=INTTYPE;
		 }
		 else if($1.type== REALTYPE && $3.type==REALTYPE)
		 {
		  GenerateQ("-",$1.place,$3.place,T);
		  VarList[T].type=$$.type=REALTYPE;
		 }
		 else if($1.type== INTTYPE)
		 {
		  int U=NewTemp();
		  GenerateQ("I->R",$1.place,0,U);
		  GenerateQ("-",U,$3.place,T);
		  VarList[T].type=$$.type=REALTYPE;
		 }
		 else
		 {
		  int U=NewTemp();
		  GenerateQ("I->R",$3.place,0,U);
		  GenerateQ("-",$3.place,U,T);
		  VarList[T].type=$$.type=REALTYPE;
		 }
		 $$.place=T;
		}

	|	Expr'*'Expr
        {
		 int T=NewTemp();
		 if($1.type== INTTYPE && $3.type==INTTYPE)
		 {
		  GenerateQ("*",$1.place,$3.place,T);
		  VarList[T].type=$$.type=INTTYPE;
		 }
		 else if($1.type== REALTYPE && $3.type==REALTYPE)
		 {
		  GenerateQ("*",$1.place,$3.place,T);
		  VarList[T].type=$$.type=REALTYPE;
		 }
		 else if($1.type== INTTYPE)
		 {
		  int U=NewTemp();
		  GenerateQ("I->R",$1.place,0,U);
		  GenerateQ("*",U,$3.place,T);
		  VarList[T].type=$$.type=REALTYPE;
		 }
		 else
		 {
		  int U=NewTemp();
		  GenerateQ("I->R",$3.place,0,U);
		  GenerateQ("*",$3.place,U,T);
		  VarList[T].type=$$.type=REALTYPE;
		 }
		 $$.place=T;
		}

	|	Expr'/'Expr
        {
		 int T=NewTemp();
		 if($1.type== INTTYPE && $3.type==INTTYPE)
		 {
		  GenerateQ("/",$1.place,$3.place,T);
		  VarList[T].type=$$.type=INTTYPE;
		 }
		 else if($1.type== REALTYPE && $3.type==REALTYPE)
		 {
		  GenerateQ("/",$1.place,$3.place,T);
		  VarList[T].type=$$.type=REALTYPE;
		 }
		 else if($1.type== INTTYPE)
		 {
		  int U=NewTemp();
		  GenerateQ("I->R",$1.place,0,U);
		  GenerateQ("/",U,$3.place,T);
		  VarList[T].type=$$.type=REALTYPE;
		 }
		 else
		 {
		  int U=NewTemp();
		  GenerateQ("I->R",$3.place,0,U);
		  GenerateQ("/",$3.place,U,T);
		  VarList[T].type=$$.type=REALTYPE;
		 }
		 $$.place=T;
		}

	|	Expr'%'Expr
        {
		 int T=NewTemp();
		 if($1.type== INTTYPE && $3.type==INTTYPE)
		 {
		  GenerateQ("%",$1.place,$3.place,T);
		  VarList[T].type=$$.type=INTTYPE;
		  $$.place=T;
                 }
		}

	|	'('Expr')'
        {
		 $$=$2;
		}

	|	'-' Expr %prec UMINUS  
        {
		 int T=NewTemp();
		 $$=$2;
		 GenerateQ("Minus",$2.place,0,T);
		 $$.place=T;VarList[T].type=VarList[$2.place].type;
		}//这句实现的是负数，具有最高优先级

	|	Varable
        {
		 $$.place=$1;
		 $$.type=VarList[$1].type;
		}//最后移进转化为标识符，得到名与类型

	|	Const{ $$=$1;}//常量
	;

/* <布尔表达式> → <布尔表达式> or <布尔项>│<布尔项>…… */	
BoolExpr	: Expr RelationOp Expr
                {
		 $$.TC=QL;$$.FC=QL+1;//真出口假出口，相应四元式
		 GenerateQ($2,$1.place,$3.place,0);
		 GenerateQ("j",0,0,0);
		}
	|	BoolExpr And  {BackPatch($1.TC,QL);}//包含And 的布尔表达式中只有第一布尔的值为True才跳转到第二布尔表达式，否则直接跳转到False时的四元式，表达式假为第一个或第二个的假
        BoolExpr{$$.TC=$4.TC;$$.FC=Merge($1.FC,$4.FC);}

	|	BoolExpr Or {BackPatch($1.FC,QL);}//包含Or 的布尔表达式中只需第一布尔的值为True就为True，否则跳转到第二个表达式，表达式真为第一个或第二个的真，假即为第二个假
                BoolExpr{$$.FC=$4.FC;$$.TC=Merge($1.TC,$4.TC);}	

	|	Not BoolExpr
                 {$$.TC=$2.FC;$$.FC=$2.TC;}
	|	'(' BoolExpr ')'{$$=$2;}
	;

//标识符
Varable	: Iden
          { 
		    $$=Entry(str1);
	    	}
	;
//常量
Const	:	IntNo
		{ int i=Entry(str1);
		  $$.type=VarList[i].type=INTTYPE;
		  $$.place=i;
		}
	|	RealNo
		{ int i=Entry(str1);
		  $$.type=VarList[i].type=REALTYPE;
		  $$.place=i;
		}
	;

//双对象操作数
RelationOp:	'<'{strcpy($$,"j<");}
	|	'>'{strcpy($$,"j>");}
	|	'='{strcpy($$,"j=");}
	|	GE  {strcpy($$,"j>=");}
	|	NE  {strcpy($$,"j!=");}
	|	LE  {strcpy($$,"j<=");}
	;


%%

/*打印四元式的关键函数*/
void PrintQ(void)
{int i;
 for(i=1;i<QL;i++)
 {
  printf("(%2d) ( %5s, ",i,QuaterList[i].op);
  if(QuaterList[i].arg1)
	printf("%3s, ",VarList[QuaterList[i].arg1].name);
	else printf("  -, ");
  if(QuaterList[i].arg2)
	printf("%3s, ",VarList[QuaterList[i].arg2].name);
	else printf("  -, ");
  if((QuaterList[i].op[0]=='j')||(QuaterList[i].op[0]=='S')) 
		printf("%3d )\n",QuaterList[i].result);
   else if(QuaterList[i].result)
	printf("%3s )\n",VarList[QuaterList[i].result].name);
	else printf("  - )\n");
 }
 return;
}


/*找到语法中的错误*/
int yyerror(char *errstr)
{
          printf(" %s\n",errstr);
          return 0;
}

/*新建变量并存储到变量符号表*/
int Entry(char *name)
{int i;
 for(i=1;i<=VarCount;i++) if(!strcmp(name,VarList[i].name)) return i;
 if(++VarCount>MAXMEMBER) {printf("Too many Variables!\n");exit(-1);}
 strcpy(VarList[VarCount].name,name);
 return VarCount;
}

/*回填类型*/
int FillType(int First,int type)
{ int i;
  for(i=First;i<=VarCount;i++){
    VarList[i].type=type;
    }
  return i-1;
}

/*合并两条尾链*/
int Merge(int p,int q)
{
 int r;
 if(!q) return p;
  else
  {
   r=q;
   while(QuaterList[r].result)
    r=QuaterList[r].result;/*找到为0(未填)的尾链*/
   QuaterList[r].result=p;
  }
 return q;
}

/*拉链回填，t填到p*/
void BackPatch(int p,int t)
{
 int q=p;
 while(q)
 { int q1=QuaterList[q].result;
   QuaterList[q].result=t;
   q=q1;
 } 
 return;
}

/*产生四元式的函数*/
int GenerateQ(char* op,int a1,int a2,int re)
{
 strcpy(QuaterList[QL].op,op);
 QuaterList[QL].arg1=a1;
 QuaterList[QL].arg2=a2;
 QuaterList[QL].result=re;
 QL++;
 return QL;
}

/*新建临时变量*/
int NewTemp()
{
 static int no=1;
 char Tname[10];
 sprintf(Tname,"T%o",no++);
 return Entry(Tname);
}

/*打印变量符号表*/
void PrintList(void)
{
 int i;
 printf(" \n以下是变量符号表：\n");
 printf(" 序号\t 变量名 \t   类型\n");
 for(i=1;i<=VarCount;i++)
  {
   printf("%4d\t%6s\t\t",i,VarList[i].name);
   if(VarList[i].type) printf(" REAL  \n");else printf(" INTEGER\n"); 
  }
 return;
}


int main()
{
printf("z黄鸿展 计科2班 202230441138 实验2大作业\n");int p=1;
while(p){
printf("请输入文件名字\n");
char filename[100]; char line[100];char *token;
scanf("%s",&filename);
yyin=NULL;
FILE* f=fopen(filename, "r");//????????
    if (!f) {
        printf(stderr, "Unable to open file\n");
    }
	if (fgets(line, sizeof(line), f) != NULL) {
        // 获取第二个字符串
        token = strtok(line, " ");
        for (int i = 0; i < 1; i++) {
            token = strtok(NULL, " ");
        }
		fclose(f);
    }
	f=fopen(filename, "r");
	yyin = f;
    yyparse();	
 printf("\n");
 printf("( 0) (Program, %9s ,- ,- )\n",token);
 PrintQ();
 PrintList();
 fclose(f);
 printf("是否继续读取？(输入1继续/0结束)\n");scanf("%d",&p);
 QL=1;VarCount=0;
 }
 system("pause");
 getchar();
 return;
}