#include "nemu.h"
#include <stdio.h>
#include <stdlib.h>
/* We use the POSIX regex functions to process regular expressions.
 * Type 'man regex' for more information about POSIX regex functions.
 */
#include <sys/types.h>
#include <regex.h>
extern uint32_t isa_reg_str2val(const char *s,bool *success);
extern uint32_t paddr_read(paddr_t addr,int len);
enum {
  TK_NOTYPE = 256, TK_EQ=1,TK_NUM=10,TK_UNEQ=0,TK_AND=2,TK_SIXT=16,TK_REG=255,DEREF=254,
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
  {"==", TK_EQ},         // equal
  {"-",'-'},
  {"\\*",'*'},
  {"/",'/'},
  {"\\(",'('},
  {"\\)",')'},
  {"0x[0-9a-f]+",TK_SIXT},
  {"[0-9]+",TK_NUM},
  {"!=",TK_UNEQ},
  {"&&",TK_AND},
  {"\\$[a-ehilpx]{2,3}",TK_REG},
};

#define NR_REGEX (sizeof(rules) / sizeof(rules[0]) )

static regex_t re[NR_REGEX] = {};

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
  char str[64];
} Token;

static Token tokens[100] __attribute__((used)) = {};
static int nr_token __attribute__((used))  = 0;

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
        if (rules[i].token_type==TK_NOTYPE){continue;}
        switch (rules[i].token_type) {
	  case '+':
	  case '-':
	  case '*':
          case '/':
	  case '(':
	  case ')':
	  case TK_EQ:
	  case TK_NUM:
	  case TK_REG:
	  case TK_UNEQ:
	  case TK_SIXT:
	  case TK_AND:{memset(tokens[nr_token].str,'\0',sizeof(tokens[nr_token].str));
			      tokens[nr_token].type=rules[i].token_type;

                       strncpy(tokens[nr_token].str,substr_start,substr_len);break;}
	  default: assert(0);
        }
        nr_token++;
	break;
      }
    }

    if (i == NR_REGEX) {
      printf("no match at position %d\n%s\n%*.s^\n", position, e, position, "");
      return false;
    }
  }
 // for (int j=0;j<nr_token;j++){printf("%s",tokens[j].str);}printf("\n");
  return true;
}
bool check_parentheses(int p,int q){
  int temp=0;
  if (tokens[p].type!='(' || tokens[q].type!=')'){return false;}
  for (int i=p;i<=q;i++){
   /* if (tokens[i].type=='('){
      temp=1;
      i++;
      while(true){
        if (tokens[i].type=='('){temp++;}
        else if(tokens[i].type==')'){temp--;}
        printf("while i:%d",i);
        if (temp==0){printf("i=%d\n",i);break;}
        i++;
      }
      if(i>=q){break;}*/
    if (tokens[i].type=='('){temp++;}
    else if (tokens[i].type==')'){temp--;}
    if (temp<0){assert(0);}
    if (temp==0 && i<q){return false;} 
  }
  if (temp!=0)return false;
  return true;
}

static bool priority(int op1,int op2){
  switch(op1){  //TK_EQ=1,TK_UNEQ=0,TK_AND=i2,DEREF=254,'+','-','*','/'
    case TK_AND:{if (op2!=TK_AND){ return true;}
             else {return false;}
             }
    case TK_EQ:{if (op2!=TK_AND && op2!=TK_EQ && op2!=TK_UNEQ){return true;}
	     else {return false;}
	     }
    case TK_UNEQ:{if (op2!=TK_AND && op2!=TK_EQ && op2!=TK_UNEQ){return true;}
             else {return false;}
             }
    case '+':{if (op2!=TK_AND && op2!=TK_EQ && op2!=TK_UNEQ && op2!='+' && op2!='-'){return true;}
             else {return false;}
             }
    case '-':{if (op2!=TK_AND && op2!=TK_EQ && op2!=TK_UNEQ && op2!='+' && op2!='-'){return true;}
             else {return false;}
             }
    case '*':{if (op2!=TK_AND && op2!=TK_EQ && op2!=TK_UNEQ && op2!='+' && op2!='-' && op2!='*' && op2!='/')
		 {return true;}
             else {return false;}
             }
    case '/':{if (op2!=TK_AND && op2!=TK_EQ && op2!=TK_UNEQ && op2!='+' && op2!='-' && op2!='*' && op2!='/')
                 {return true;}
             else {return false;}
             }
    case DEREF:{if (op2!=TK_AND && op2!=TK_EQ && op2!=TK_UNEQ && op2!='+' && op2!='-' && op2!='*' && op2!='/'&& op2!=DEREF)
                 {return true;}
             else {return false;}
             }
    default:assert(0);
  }
}

int main_operator(int p,int q){
  int op=tokens[p].type;
  int op_position=p;
  bool init=false;
  for (int i=p;i<=q;i++){
    if (tokens[i].type=='('){
      int temp=1;
      i++;
      while(true){
        if (tokens[i].type=='('){temp++;}
	else if(tokens[i].type==')'){temp--;}
        if (temp==0){break;}
	i++;
      }
      if(i>=q){break;}
    }//TK_NOTYPE = 256, TK_EQ=1,TK_NUM=10,TK_UNEQ=0,TK_AND=2,TK_SIXT=16,TK_REG=255,DEREF=254,
    else if (tokens[i].type==DEREF||tokens[i].type==TK_NUM||tokens[i].type==TK_SIXT||
		    tokens[i].type==TK_REG){continue;}
    else {
      if (init==false){op=tokens[i].type;op_position=i;init=true;
	  continue;}	    
      else if(priority(op,tokens[i].type)==false) {op=tokens[i].type;op_position=i;}
    }
  }
  return op_position;
}

uint32_t eval(int p,int q){
  if (p>q){return -1;}
  else if (p==q){
    uint32_t num=0;
    if(tokens[p].type==TK_NUM){
      sscanf(tokens[p].str,"%d",&num);
      return num;}
    else if(tokens[p].type==TK_SIXT){
      sscanf(tokens[p].str,"%x",&num);
      return num;
    }
    else if (tokens[p].type==TK_REG){
      bool success=true;
      num=isa_reg_str2val(strtok(tokens[p].str,"$"),&success);
      return num;
    }
  }
  else if (check_parentheses(p,q)) {return eval(p+1,q-1);}
  else {
    int op=main_operator(p,q);//printf("eval %s,%d\n",tokens[op].str,op);
    //if(op==p && tokens[op].type==DEREF){return paddr_read(eval(p+1,q),4);}
    uint32_t val1=eval(p,op-1);
    uint32_t val2=eval(op+1,q);
    switch(tokens[op].type){
      case '+':return val1+val2;
      case '-':return val1-val2;
      case '*':return val1*val2;
      case '/':return val1/val2;
      case TK_EQ:return val1==val2;
      case TK_UNEQ:return val1!=val2;
      case TK_AND:return val1 && val2;
      case DEREF:return paddr_read(val2,4);
      default:assert(0);	       
    }
  }
  return 0;
}


uint32_t expr(char *e, bool *success) {
  if (!make_token(e)) {
    *success = false;
    printf("not make_token\n");
    return 0;
  }
  for(int i=0;i<nr_token;i++){
    if (tokens[i].type=='*' &&(i==0 || tokens[i-1].type=='+' || tokens[i-1].type=='-' || \
        tokens[i-1].type=='*' || tokens[i-1].type=='/'))
        {tokens[i].type=DEREF;}
  }
  /* TODO: Insert codes to evaluate the expression. */
  return eval(0,nr_token-1);
}

void clear_tokens(){
  while(nr_token!=0){
  memset(tokens[nr_token].str,'\0',sizeof(tokens[nr_token].str));
  nr_token--;
  }
}



