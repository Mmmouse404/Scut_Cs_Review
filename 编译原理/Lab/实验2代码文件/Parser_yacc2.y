%{
#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <math.h>
#define MAXSTR 20 //����ַ�������
#define MAXMEMBER 100 //����ŵ�����
//INT���ͺ�REAL����
#define INTTYPE  0
#define REALTYPE 1

extern int yylex();
extern FILE* yyin;
extern char str1[20];//���ڴ��yytext������ַ���ֵ

//ʵ�����������������Ԫʽ�Ĺؼ�����
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
int QL=1;  /* ���ڼ�¼��Ԫʽ��*/


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
 int Iv; //��������
 int CH; //�������ʽ������ֵ
 int NO; //��������ֵ��������ֵ�����ر�����
 struct { int TC,FC;} _BExpr;//�������ʽ(�����/�ٳ���)
 struct { int QUAD,CH;} _WBD;//While Do��ѭ�����ƽṹ��QUAD��ת�Լ�CH��������ֵ���ɴ����һ��ĳ��ڣ�
 struct { int type,place;} _Expr;// �������ʽ�����ͺ���
 char _Rop[5]; //�����ַ�����
 int Last;    //�����һ����Ϊ�ж�����
 char str[20]; //����ַ���
}

/*����const  */

%token    	Iden    500
%token    	IntNo	501
%token	  	RealNo  502

/*�ؼ���*/
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
/*˫��������� */
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
/* <����> �� program <��ʶ��> ��<����˵��> <�������> . (SubProg)
   <���Ͼ�> �� begin <����> end */
ProgDef	:	Program Iden ';' SubProg
			{
				$$=0;printf("\n����ɹ�!\n");
			}
	;
SubProg:   VarDef Begin StateLists End
		{
		 BackPatch($3,QL);
		 GenerateQ("sys",0,0,0);
	    }
	;

/* <����˵��> �� var <��������>���� */
VarDef	:Var VarDefList 
	     {$$=0;}
	;
/* <��������> �� <��ʶ����> ��<����> ��<��������>��<��ʶ����> ��<����> ��
   <��ʶ����> �� <��ʶ��> ��<��ʶ����>��<��ʶ��> */
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

/*<����> �� <���> ��<����>��<���>*/
StateLists: StateList ';'{BackPatch($1,QL);$$=0;}//����BoolExper.FC��QLΪ����whileѭ���ĵ�һ����Ԫʽ
	|       StateList    {BackPatch($1,QL);$$=0;}//���ǵ����������������û�зֺţ����������������ѭ��
	;
StateList:Statement {$$=$1;}
	|StateLists Statement 
	        {$$=$2;}
	;
/* <���> �� <��ֵ��>��<if��>��<while��>��<repeat��>��<���Ͼ�> */
Statement:	AsignState{$$=0;}
	|	I_T_E ComplexState{$$=Merge($1,$2);}
	|	I_T   ComplexState{$$=Merge($1,$2);}//��if�����BoorExpr.FC��Ӧ����Ԫʽ��ComplexState�ϲ����õ���Ԫʽ(������תλ��ΪComplexState��Ԫʽ)��
	|	Wd ComplexState   { GenerateQ("j",0,0,$1.QUAD);
		  BackPatch($2,$1.QUAD);
		  $$=$1.CH;}//������һ���ŵ���Ԫʽ������һ���ս��β��������ڣ�ָ�򸴺Ͼ䣬��while������ֵ�ϴ�����
	|	F_B_A	ComplexState	{ GenerateQ("j",0,0,$1.QUAD);
		  BackPatch($2,$1.QUAD);
		  $$=$1.CH;}
	|   R_U                     {  
                            $$ = $1.CH;
							}
	;

/* <��ֵ��> �� <��ʶ��> := <�������ʽ> */
AsignState:	Asign_Varable ASSIGN Expr  
            {
            $$.QUAD=$1;
            $$.CH=QL-1;
			 if(VarList[$1].type==$3.type)
			   GenerateQ(":=",$3.place,0,$1);
			 else if($3.type==INTTYPE)//���Ͳ�ͬ��׷�����������
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

/* <if��>�� if <�������ʽ> then <���>��if <�������ʽ> then <���> else <���> */	
I_T_E	:	I_T ComplexState Else{int q=QL;GenerateQ("j",0,0,0);BackPatch($1,QL);$$=Merge($2,q);}//�����һ����������ת��BoolExprΪ��ʱ����else��һ�е���Ԫʽ
	;
I_T	:	If  BoolExpr Then {BackPatch($2.TC,QL);$$=$2.FC;}//��QL�����һ����Ԫʽ�Ľ��ֵ�����ҽ�BoolExpr.FCֵ���ص���һ��
	;

/* <while��> �� while <�������ʽ> do <���> */	
Wd	:	W  BoolExpr Do {BackPatch($2.TC,QL);$$.QUAD=$1;$$.CH=$2.FC;}//�ȶ�BoolExpr��TC��Ԫʽ���л���ٽ�QL��BoolExpr.FC������һ��
	;
W	:	While{$$=QL;}//��QL������һ��
	;

/*for��� <for> -> for '(' <�������ʽ> ',' <��ֵ���> ')'*/
F_B_A:	F '(' BoolExpr ',' AsignState ')' {BackPatch($3.TC,$5.CH);$$.QUAD=$1;$$.CH=$3.FC;}
	;
F 	:	For{$$=QL;}
	;

/* <repeat��> �� repeat <���> until <�������ʽ> */	
R_U:	
    R ComplexState U BoolExpr {
		$$.QUAD=$3;
		BackPatch($4.FC,$1);//FC�ص�R��QL
		$$.CH=$4.TC;//��Ҫ�������TC
		BackPatch($2,$$.QUAD); //ִ����ص�����жϣ�Until���֣�
        }
    ;
R   :Repeat{$$=QL;}
U   :Until{$$=QL;}  

/*���Ͼ���SIMPLE������û�л����ŷָ�*/
ComplexState:	 Statement    {	$$=$1;	}
	;

/* <�������ʽ> �� <�������ʽ> + <��>��<�������ʽ> - <��>��<��> */
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
		 else if($1.type== INTTYPE)//����������ת�����߼�ͬ��ֵ�����͸�����������
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
		}//���ʵ�ֵ��Ǹ���������������ȼ�

	|	Varable
        {
		 $$.place=$1;
		 $$.type=VarList[$1].type;
		}//����ƽ�ת��Ϊ��ʶ�����õ���������

	|	Const{ $$=$1;}//����
	;

/* <�������ʽ> �� <�������ʽ> or <������>��<������>���� */	
BoolExpr	: Expr RelationOp Expr
                {
		 $$.TC=QL;$$.FC=QL+1;//����ڼٳ��ڣ���Ӧ��Ԫʽ
		 GenerateQ($2,$1.place,$3.place,0);
		 GenerateQ("j",0,0,0);
		}
	|	BoolExpr And  {BackPatch($1.TC,QL);}//����And �Ĳ������ʽ��ֻ�е�һ������ֵΪTrue����ת���ڶ��������ʽ������ֱ����ת��Falseʱ����Ԫʽ�����ʽ��Ϊ��һ����ڶ����ļ�
        BoolExpr{$$.TC=$4.TC;$$.FC=Merge($1.FC,$4.FC);}

	|	BoolExpr Or {BackPatch($1.FC,QL);}//����Or �Ĳ������ʽ��ֻ���һ������ֵΪTrue��ΪTrue��������ת���ڶ������ʽ�����ʽ��Ϊ��һ����ڶ������棬�ټ�Ϊ�ڶ�����
                BoolExpr{$$.FC=$4.FC;$$.TC=Merge($1.TC,$4.TC);}	

	|	Not BoolExpr
                 {$$.TC=$2.FC;$$.FC=$2.TC;}
	|	'(' BoolExpr ')'{$$=$2;}
	;

//��ʶ��
Varable	: Iden
          { 
		    $$=Entry(str1);
	    	}
	;
//����
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

//˫���������
RelationOp:	'<'{strcpy($$,"j<");}
	|	'>'{strcpy($$,"j>");}
	|	'='{strcpy($$,"j=");}
	|	GE  {strcpy($$,"j>=");}
	|	NE  {strcpy($$,"j!=");}
	|	LE  {strcpy($$,"j<=");}
	;


%%

/*��ӡ��Ԫʽ�Ĺؼ�����*/
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


/*�ҵ��﷨�еĴ���*/
int yyerror(char *errstr)
{
          printf(" %s\n",errstr);
          return 0;
}

/*�½��������洢���������ű�*/
int Entry(char *name)
{int i;
 for(i=1;i<=VarCount;i++) if(!strcmp(name,VarList[i].name)) return i;
 if(++VarCount>MAXMEMBER) {printf("Too many Variables!\n");exit(-1);}
 strcpy(VarList[VarCount].name,name);
 return VarCount;
}

/*��������*/
int FillType(int First,int type)
{ int i;
  for(i=First;i<=VarCount;i++){
    VarList[i].type=type;
    }
  return i-1;
}

/*�ϲ�����β��*/
int Merge(int p,int q)
{
 int r;
 if(!q) return p;
  else
  {
   r=q;
   while(QuaterList[r].result)
    r=QuaterList[r].result;/*�ҵ�Ϊ0(δ��)��β��*/
   QuaterList[r].result=p;
  }
 return q;
}

/*�������t�p*/
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

/*������Ԫʽ�ĺ���*/
int GenerateQ(char* op,int a1,int a2,int re)
{
 strcpy(QuaterList[QL].op,op);
 QuaterList[QL].arg1=a1;
 QuaterList[QL].arg2=a2;
 QuaterList[QL].result=re;
 QL++;
 return QL;
}

/*�½���ʱ����*/
int NewTemp()
{
 static int no=1;
 char Tname[10];
 sprintf(Tname,"T%o",no++);
 return Entry(Tname);
}

/*��ӡ�������ű�*/
void PrintList(void)
{
 int i;
 printf(" \n�����Ǳ������ű�\n");
 printf(" ���\t ������ \t   ����\n");
 for(i=1;i<=VarCount;i++)
  {
   printf("%4d\t%6s\t\t",i,VarList[i].name);
   if(VarList[i].type) printf(" REAL  \n");else printf(" INTEGER\n"); 
  }
 return;
}


int main()
{
printf("z�ƺ�չ �ƿ�2�� 202230441138 ʵ��2����ҵ\n");int p=1;
while(p){
printf("�������ļ�����\n");
char filename[100]; char line[100];char *token;
scanf("%s",&filename);
yyin=NULL;
FILE* f=fopen(filename, "r");//????????
    if (!f) {
        printf(stderr, "Unable to open file\n");
    }
	if (fgets(line, sizeof(line), f) != NULL) {
        // ��ȡ�ڶ����ַ���
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
 printf("�Ƿ������ȡ��(����1����/0����)\n");scanf("%d",&p);
 QL=1;VarCount=0;
 }
 system("pause");
 getchar();
 return;
}