#include "nemu.h"

/* We use the POSIX regex functions to process regular expressions.
 * Type 'man regex' for more information about POSIX regex functions.
 */
#include <sys/types.h>
#include <regex.h>

enum {
  TK_NOTYPE = 256,
  TK_EQ=0,//==
  TK_HN=1,//0x^
  TK_DN=2,//786
  TK_REG=3,//$reg
  TK_UEQ=4,//!=
  TK_AND=5,//&&
  TK_OR=6,//||
  TK_NOT=7,//!
  DEREF=8,//*
  NEGAT=9,//-
  /* TODO: Add more token types */

};

static struct rule {
  char *regex;
  int token_type;
} rules[] = {

  /* TODO: Add more rules.
   * Pay attention to the precedence level of different rules.
   */

  {" +", TK_NOTYPE},    // spaces
  {"\\+", '+'},         // plus
  {"==", TK_EQ},        // equal
  {"0(x|X)[0-9a-fA-F]{1,8}",TK_HN},//hexadecimal number
  {"([0-9]+)",TK_DN},//Decimal number
  {"\\$(eax|ecx|edx|ebx|esp|ebp|esi|edi|eip)",TK_REG},//register
  {"\\(", '('},
  {"\\)", ')'},
  {"-", '-'},
  {"\\*", '*'},
  {"\\/", '/'},
  {"!=", TK_UEQ},
  {"&&",TK_AND},
  {"\\|\\|",TK_OR},
  {"!",TK_NOT},
  //{""}
};

#define NR_REGEX (sizeof(rules) / sizeof(rules[0]) )

static regex_t re[NR_REGEX];

/* Rules are used for many times.
 * Therefore we compile them only once before any usage.
 */
void init_regex() {
  int i;
  char error_msg[128];
  int ret;

  for (i = 0; i < NR_REGEX; i ++) {
    ret = regcomp(&re[i], rules[i].regex, REG_EXTENDED);
    if (ret != 0) {
      regerror(ret, &re[i], error_msg, 128);
      panic("regex compilation failed: %s\n%s", error_msg, rules[i].regex);
    }
  }
}

typedef struct token {
  int type;
  char str[32];
} Token;

Token tokens[32];
int nr_token;

static bool make_token(char *e) {
  int position = 0;
  int i;
  regmatch_t pmatch;

  nr_token = 0;

  while (e[position] != '\0') {
    /* Try all rules one by one. */
    for (i = 0; i < NR_REGEX; i ++) {
      if (regexec(&re[i], e + position, 1, &pmatch, 0) == 0 && pmatch.rm_so == 0) {
        char *substr_start = e + position;
        int substr_len = pmatch.rm_eo;

        Log("match rules[%d] = \"%s\" at position %d with len %d: %.*s",
            i, rules[i].regex, position, substr_len, substr_len, substr_start);
        position += substr_len;

        /* TODO: Now a new token is recognized with rules[i]. Add codes
         * to record the token in the array `tokens'. For certain types
         * of tokens, some extra actions should be performed.
         */

        switch (rules[i].token_type) {
      //    default: TODO();
	    case 0:
		{tokens[nr_token].type=0;
		nr_token++;
		break;
		   }
	    case 1:
		 {tokens[nr_token].type=1;
	          strncpy(tokens[nr_token].str,substr_start,substr_len);
		  nr_token++;
		  break;
		 }
	    case 2:
		 {tokens[nr_token].type=2;
		  strncpy(tokens[nr_token].str,substr_start,substr_len);
	          nr_token++;
		 break;
		 }
	    case 3:
		 {tokens[nr_token].type=3;
		  strncpy(tokens[nr_token].str,substr_start,substr_len);
		  nr_token++;
		 break;
		 }
	    case '+':
		 {tokens[nr_token].type='+';
                   nr_token++;
		 break;
		 }
	    case '-':
		 {tokens[nr_token].type='-';
		     nr_token++;
		     break;
		 }
	    case '*':
	       { tokens[nr_token].type='*';
                  nr_token++;
	         break;
	       }
	    case '/':
	       { tokens[nr_token].type='/';
		   nr_token++;
	       break;
	       }
            case '(':
	       {tokens[nr_token].type='(';
		   nr_token++;
	       break;
	       }
            case ')':
	      {tokens[nr_token].type=')';
		  nr_token++;
	      break;
	      }
            case 256:
              {break;
	      }	  
	   case TK_UEQ:
	     {tokens[nr_token].type=TK_UEQ;
		 nr_token++;
		 break;
	     }     
	   case TK_AND:
	     {tokens[nr_token].type=TK_AND;
		 nr_token++;
		 break;
	     }
	   case TK_OR:
	     {tokens[nr_token].type=TK_OR;
		 nr_token++;
		 break;
	     }
	   case TK_NOT:
	     {tokens[nr_token].type=TK_NOT;
		 nr_token++;
		 break;
	     }
	   default:
	     assert(0);
        }

        break;
      }
    }

    if (i == NR_REGEX) {
      printf("no match at position %d\n%s\n%*.s^\n", position, e, position, "");
      return false;
    }
  }

  return true;
}
bool check_parentheses(int p,int q)
{ int count=0;int i=0;int x=0;
    if((tokens[p].type=='(')&&(tokens[q].type==')'))
   {for(i=p+1;i<q;i++)
       {if(tokens[i].type=='(')
	   count++;
	   else if(tokens[i].type==')')
	       count--;
	   if(count<0) x=1;
	       //printf("THE EXPRESSION IS ILLEGAL!\n");
             if(count<-1)
	     {printf("THE EXPRESSION IS ILLEGAL！\n");
	     assert(0);
	     }
	  //     return false;}
       }
       if(count==0&&x==0) return true;
       else if(count==0&&x==1) return false;
       else if(count!=0) {printf("THE EXPRESSION IS ILLEGAL!\n");assert(0);}
   
   
   }
     return false;
}
int  Precede(int a,int b)
{   //1< 2= 3>
    if(a=='+')
    {if(b=='+'||b=='-'||b==TK_EQ||b==TK_UEQ||b==TK_AND||b==TK_OR) return 3;
    else if(b=='*'||b=='/')return 1;    
    }
    else if(a=='-')
    {if(b=='+'||b=='-'||b==TK_EQ||b==TK_UEQ||b==TK_AND||b==TK_OR)return 3;
    else if(b=='*'||b=='/' )return 1;
    }
    else if(a=='*'||a=='/' ) 
	return 3;
     return 3;
     
}

uint32_t find_dominated_op(int p,int q)
{  //printf("strat find_domination_op\n");
    int Precede(int ,int );
    int number=0;
    uint32_t pos[32]={0};int flag=0;
    int result=0;
    while(p<=q)
    {if((tokens[p].type=='+')||(tokens[p].type=='-')||(tokens[p].type=='*')||(tokens[p].type=='/')||(tokens[p].type==TK_EQ)||(tokens[p].type==TK_UEQ)||(tokens[p].type==TK_AND)||tokens[p].type==TK_OR)
	    {   pos[flag]=p;
              p++;
	        flag++;
	    }
     else if(tokens[p].type=='(')
     {
      number++;
      p++;
       while(number>0)
      {if(tokens[p].type=='(') number++;
       else if(tokens[p].type==')') number--;
       p++;
      }
     }
     else p++;//////////////////////
    }
    //printf("the flag in find_dominated_op is :%d\n",flag);
    for(int i=0;i<flag;i++)
    { if(Precede(tokens[pos[result]].type,tokens[pos[i]].type)==3)
	result=i;
     }
    printf("the result in find_dominated_op is :%d\n",pos[result]);
    return pos[result];
}


uint32_t eval(int p,int q){
    uint32_t find_dominated_op(int,int);
    uint32_t op=0;
    uint32_t val1;
    uint32_t val2;
    int x=0;
    int num=0;
    //char s[32]={0,};
    //char *buf=s;
    if(p>q){
    assert(0);/*bad expression*/
    }
    else if(p==q)
    {/*for now this token should be a number*/
	int number=0;
     //char *buf;
     if(tokens[p].type==3)
     {if(strcmp(tokens[p].str,"$eax")==0)  number=cpu.gpr[0]._32;
      else if(strcmp(tokens[p].str,"$ecx")==0) number=cpu.gpr[1]._32;
      else if(strcmp(tokens[p].str,"$edx")==0) number=cpu.gpr[2]._32;
      else if(strcmp(tokens[p].str,"$ebx")==0) number=cpu.gpr[3]._32;
      else if(strcmp(tokens[p].str,"$esp")==0) number=cpu.gpr[4]._32;
      else if(strcmp(tokens[p].str,"$ebp")==0) number=cpu.gpr[5]._32;
      else if(strcmp(tokens[p].str,"$esi")==0) number=cpu.gpr[6]._32;
      else if(strcmp(tokens[p].str,"$edi")==0) number=cpu.gpr[7]._32;
      else if(strcmp(tokens[p].str,"$eip")==0) number=cpu.eip;
      
     }
     else if(tokens[p].type==2)
     {sscanf(tokens[p].str,"%d",&number);
     //printf("eval only a number value:%d\n",number);
     }
     else if(tokens[p].type==1)
     {//printf("the hnumber str[1] is:%c\n",tokens[p].str[1]);
       //	 if(tokens[p].str[1]=='x')
         //  char *buf=strtok(tokens[p].str,"x");
	   //printf("the buf is %s\n",buf);
    //else if(tokens[p].str[1]=='X') char *buf=strtok(tokens[p].str,"X");
     sscanf(tokens[p].str,"%x",&number);
     }
     return number;
    }
    else if(check_parentheses(p,q)==true){
    return eval(p+1,q-1);
    }
    else if(tokens[p].type==DEREF)
    {return vaddr_read(eval(p+1,q),4);
    }
    else if(tokens[p].type==NEGAT)
    {//return (0-eval(p+1,q));
     x=p ;
     printf("the Negative x is:%d\n",x);
     //for(int k=p;k<nr_token;k++)
     //{if
        while(tokens[p].type==NEGAT)
	 p++;
     //}
     printf("the p now is :%d\n",p);
     printf("the q now is ：%d\n",q);
     num=eval(p,q);
     printf("the num before:%d\n",num);
     for(int k=x;k<p;k++)
     {num=0-num;
     }
     return num;

    }
    else if(tokens[p].type==TK_NOT)
    { if(eval(p+1,q)==0) return 1;
	else return 0;
    }
    else {
    //printf("start else\n");
    op=find_dominated_op(p,q);
    //printf("the op in eval is:%d\n",op);
    val1=eval(p,op-1);
    //printf("the val1:%d\n",val1);
    val2=eval(op+1,q);
    //printf("the val2:%d\n",val2);
    switch(tokens[op].type){
	case '+':return val1+val2;
	case '-':return val1-val2;
        case '*':return val1*val2;
        case '/':return val1/val2;
	case TK_EQ:{if(val1==val2) return 1;
		       else return 0;
	case TK_UEQ:{if(val1!=val2) return 1;
			else return 0;
		    }
	case TK_OR:{if((val1==0)&&(val2==0))return 0;
		       else return 1;
		   }
	case TK_AND:{if((val1==0)||(val2==0)) return 0;
			else return 1;
		    }
		   
		   }
	default:assert(0);
    }
    }
return 0;
}
uint32_t expr(char *e, bool *success) {
  uint32_t eval(int p,int q);
    if (!make_token(e)) {
    *success = false;
    return 0;
  }
*success=true;
int length=0;
int i=0;
for(i=0;i<nr_token;i++)
 { if(tokens[i].type=='*'&&(i==0||tokens[i-1].type=='+'||tokens[i-1].type=='-'||tokens[i-1].type=='*'||tokens[i-1].type=='/'||tokens[i-1].type==TK_EQ||tokens[i-1].type==TK_UEQ||tokens[i-1].type==TK_AND||tokens[i-1].type==TK_OR))
	 tokens[i].type=DEREF;
}
for(i=0;i<nr_token;i++)
  {if(tokens[i].type=='-'&&(i==0||tokens[i-1].type=='+'||tokens[i-1].type=='-'||tokens[i-1].type=='*'||tokens[i-1].type=='/'||tokens[i-1].type==TK_EQ||tokens[i-1].type==TK_UEQ||tokens[i-1].type==TK_AND||tokens[i-1].type==TK_OR||tokens[i-1].type==NEGAT))
      tokens[i].type=NEGAT;
  }
  /* TODO: Insert codes to evaluate the expression. */
  //TODO();
//int i;
//printf("token")
length=nr_token;
//printf("length in expr is :%d\n",length);
  return eval(0,length-1);
}
