%{
#include "y.tab.h"
#include <stdio.h>
#include <ctype.h>
#include <string.h>
#define    	Iden    	500
#define    	IntNo		501
#define	  	RealNo  	502
/*关键字:*/
#define	  	Program 600
#define	  	Begin	601
#define	  	End	    602
#define	  	Var	    603
#define	  	Integer 604
#define	  	Real	605
#define	  	While	606
#define		If	    607
#define		Else	608
#define  	For 	609
#define 	To 		610
#define 	Down	611
#define     Repeat  612
#define     Until   613
#define     Do      614
#define     And     615
#define     Or      616
#define     Not     617
#define     Then    618
/*双对象操作*/
#define		LE	    700
#define		GE	    701
#define		NE	    702
#define     DOT     704
#define     ASSIGN  703
#define		ERRORCHAR	800

#define MAXSTR 20


char str1[20]; /*存放yytext的字符串?*/
%}
letter      [A-Za-z]
alnum		[A-Za-z0-9]
d			[0-9]
white		[\t\n\040]
%start		COMMENT
%%
\{			{ECHO;BEGIN COMMENT;} /*ECHO:写yyout*/
<COMMENT>\}		{ECHO;BEGIN 0;}
<COMMENT>\n		{ECHO;}
<COMMENT>[^\}]+ 	{ECHO;} /*去注释化*/
[Bb][eE][Gg][iI][Nn]	{ECHO;return Begin;}
[eE][Nn][Dd]\.		{ECHO;return End;}
[Pp][Rr][oO][Gg][Rr][Aa][Mm]	{ECHO;return Program;}
[Vv][Aa][Rr]		{ECHO;return Var;}
[Ii][nN][Tt][eE][Gg][eE][Rr]	{ECHO;return Integer;}
[Rr][eE][Aa][lL]	{ECHO;return Real;}
[Ww][Hh][Ii][Ll][Ee]	{ECHO;return While;}
[iI][Ff]		{ECHO;return If;}
[Ee][Ll][Ss][Ee]	{ECHO;return Else;}
[Ff][Oo][Rr]		{ECHO;return For;}
[Tt][Oo]			{ECHO;return To;}
[Dd][Oo][Ww][Nn]	{ECHO;return Down;}
[Rr][Ee][Pp][Ee][Aa][Tt] {ECHO;return Repeat;}
[Uu][Nn][Tt][Ii][Ll] {ECHO;return Until;}
[Dd][Oo]             {ECHO;return Do;}
[Aa][Nn][Dd]         {ECHO;return And;}
[Oo][Rr]             {ECHO;return Or;}
[Nn][Oo][Tt]         {ECHO;return Not;}
[Tt][Hh][Ee][Nn]     {ECHO;return Then;}
({d}+(\.{d}*)?[Ee]([+-])?{d}+)|({d}*\.{d}+[Ee]([+-])?{d}+)	{ECHO;strcpy(str1,yytext);return RealNo;}		/*科学计数法*/
{d}+\.{d}*|{d}*\.{d}+		{ECHO;strcpy(str1,yytext);return RealNo;}
{d}+			{ECHO;strcpy(str1,yytext);return IntNo;}

"("			{ECHO;return '(';}
")"			{ECHO;return ')';}
"["			{ECHO;return '[';}
"]"			{ECHO;return ']';}
">"			{ECHO;return '>';}
"<"			{ECHO;return '<';}
"="			{ECHO;return '=';}
"<="		{ECHO;return LE;}
">="	    {ECHO;return GE;}
"!="		{ECHO;return NE;}
":="        {ECHO;return ASSIGN;}
":"			{ECHO;return ':';}
";"			{ECHO;return ';';}
"\."		{ECHO;return '.';}
"*"			{ECHO;return '*';}
"+"			{ECHO;return '+';}
"-"			{ECHO;return '-';}
"/"			{ECHO;return '/';}
"%"			{ECHO;return '%';}
","			{ECHO;return ',';} 
"&"         {ECHO;return '&';}
"|"         {ECHO;return '|';}
"!"         {ECHO;return '!';} 

{letter}{alnum}*	{ ECHO;strcpy(str1,yytext);return Iden;} /*标识符*/
{white}+		    {ECHO;} /*空格换行*/
.			        {printf("Invalid char %s\n",yytext);
			        return ERRORCHAR;
			        }		
%%


#ifndef yywrap
 int yywrap(){return 1;}
#endif