#include "comp.h"
#include "y.tab.h"

int MARK_NUM = 0;

int
main(int argc, const char *argv[])
{
  char tmp_file_name[257];
  char asm_file_name[257];
  FILE *asm_file;

  if (argc > 1)
  {
    FILE *f = fopen(argv[1], "r");
    if (f == NULL)
    {
      printf("No such file\n");
      return -1;
    }

    int len = strlen(argv[1]);
    strcpy(tmp_file_name, argv[1]);

    if (strcmp(tmp_file_name + len - 2, ".z"))
    {
      printf("incorrect file extension\n");
      return -1;
    }
    else
    {
      tmp_file_name[len - 2] = '\0';
      strcat(tmp_file_name + len - 2, ".asm");
      printf("%s\n", tmp_file_name);
    }

    yyset_in (f);
  }
  else strcpy(tmp_file_name, "o.asm");

  strcpy(asm_file_name, tmp_file_name);
  strcat(tmp_file_name, ".tmp");
  tmp_file = fopen(tmp_file_name, "w");

  MARK_FRST = stack_new();
  MARK_SCND = stack_new();
  VARS = str_stack_new();

  int Res = yyparse();
  fclose(tmp_file);

  tmp_file = fopen(tmp_file_name, "r");
  asm_file = fopen(asm_file_name, "w");
  fprintf(asm_file, ".DATA\n"); 
  print_vars(asm_file);
  fprintf(asm_file, ".CODE\n");

  char buf[100];
  while (fwrite(buf, 1, fread(buf, 1, 100, tmp_file), asm_file));

  fclose(tmp_file);
  fclose(asm_file);
  remove(tmp_file_name);

  if (Res == 0) 
  {
      printf("OK \n");
  }
  else 
  {
      printf("HE OK\n");
      remove(asm_file_name);
  }

  stack_delete(&MARK_FRST);
  stack_delete(&MARK_SCND);
  str_stack_delete(&VARS);

  return(Res);
}

void
yyerror(char *s)
{
  fprintf(stderr, "%s\n" , s);
}

int
yywrap()
{
  return(1);
}


int FREE = 2;
int IN_MEM = 0;

const char *reg[2] = { "eax", "ebx" };
int CUR_REG_NUM = 0;

void
LOAD_VAR(const char *str)
{
  if (str_stack_fnd(&VARS, str) < 0) 
  {
    str_stack_push(&VARS, str);
  }
}

void
print_vars(FILE *f)
{
  for (int i = 0; i < VARS.count; ++i)
  {
    fprintf(f, "    %s: .dw 0x00\n", VARS.list[i].name);
  }
  fprintf(f, "    .mem:\n");
}

int BASE = 0;
int OFFSET = 0;

void
PUSH(const char *arg)
{
  fprintf(tmp_file, "    mov  [.mem+%02Xh], %s\n", BASE + OFFSET, arg);
  OFFSET += 4;
}

void
POP(const char *arg)
{
  OFFSET -= 4;
  fprintf(tmp_file, "    mov  %s, [.mem+%02Xh]\n", arg, BASE + OFFSET);
}

void
MOV(const char *str, TERM_TYPE type)
{
    const char *
    format = (type == t_num) ? ("    mov  %s, %s\n")
                             : ("    mov  %s, [%s]\n");

    if (!FREE)
    {
        CUR_REG_NUM = IN_MEM%2;
        PUSH(reg[CUR_REG_NUM]);
        IN_MEM++;
    }
    else
    {
        CUR_REG_NUM = (IN_MEM + FREE)%2;
        FREE--;
    }

    if (type == t_num)
    {
      fprintf(tmp_file, "    mov  %s, %s\n", reg[CUR_REG_NUM], str);
    }
    else
    {
      fprintf(tmp_file, "    mov  %s, [%s]\n", reg[CUR_REG_NUM], str);

      LOAD_VAR(str);
    }
}

void
PUSH_MARK(STACK *st)
{
  stack_push(st, MARK_NUM);
  MARK_NUM++;
}

int
POP_MARK(STACK *st)
{
  return stack_pop(st);
}

int  
LAST_MARK()
{
  return MARK_NUM - 1;
}

void
OP(int op_id)
{
    if (FREE)
    {
        FREE--;
        IN_MEM--;
        POP(reg[IN_MEM%2]);

    }

    CUR_REG_NUM = IN_MEM%2;
    const char * reg_lft = reg[CUR_REG_NUM];
    const char * reg_rgt = reg[(CUR_REG_NUM + 1)%2];

    switch(op_id)
    {
      case Mvadd:
      case Inc:
      case Add:
      {
        fprintf(tmp_file, "    add  %s, %s\n", reg_lft, reg_rgt);
      } break;

      case Mvsub:
      case Dec:
      case Sub:
      {
        fprintf(tmp_file, "    sub  %s, %s\n", reg_lft, reg_rgt);
      } break;

      case Mvmlt:
      case Mlt:
      {
        fprintf(tmp_file, "    mlt  %s, %s\n", reg_lft, reg_rgt);
      } break;

      case Mvdiv:
      case Div:
      {
        fprintf(tmp_file, "    div  %s, %s\n", reg_lft, reg_rgt);
      } break;

      case Mvmod:
      case Mod:
      {
        fprintf(tmp_file, "    div  %s, %s\n", reg_lft, reg_rgt);
        fprintf(tmp_file, "    mov  %s, %s\n", reg_lft, reg_rgt);
      } break;

      case Mvlsh:
      case Lsh:
      {
        fprintf(tmp_file, "    shl  %s, %s\n", reg_lft, reg_rgt);
      } break;

      case Mvrsh:
      case Rsh:
      {
        fprintf(tmp_file, "    shr  %s, %s\n", reg_lft, reg_rgt);
      } break;

      case Moreq:
      {
        fprintf(tmp_file, "    cmp  %s, %s\n", reg_lft, reg_rgt);
        fprintf(tmp_file, "    jl   .+0Ah\n");
        fprintf(tmp_file, "    mov  %s, 1\n", reg_lft);
        fprintf(tmp_file, "    jmp  .+08h\n");
        fprintf(tmp_file, "    mov  %s, 0\n", reg_lft);
      } break;

      case Leseq:
      {
        fprintf(tmp_file, "    cmp  %s, %s\n", reg_lft, reg_rgt);
        fprintf(tmp_file, "    jg   .+0Ah\n");
        fprintf(tmp_file, "    mov  %s, 1\n", reg_lft);
        fprintf(tmp_file, "    jmp  .+08h\n");
        fprintf(tmp_file, "    mov  %s, 0\n", reg_lft);
      } break;

      case Equal:
      {
        fprintf(tmp_file, "    cmp  %s, %s\n", reg_lft, reg_rgt);
        fprintf(tmp_file, "    jne  .+0Ah\n");
        fprintf(tmp_file, "    mov  %s, 1\n", reg_lft);
        fprintf(tmp_file, "    jmp  .+08h\n");
        fprintf(tmp_file, "    mov  %s, 0\n", reg_lft);
      } break;

      case Noteq:
      {
        fprintf(tmp_file, "    cmp  %s, %s\n", reg_lft, reg_rgt);
        fprintf(tmp_file, "    je   .+0Ah\n");
        fprintf(tmp_file, "    mov  %s, 1\n", reg_lft);
        fprintf(tmp_file, "    jmp  .+08h\n");
        fprintf(tmp_file, "    mov  %s, 0\n", reg_lft);
      } break;

      case More:
      {
        fprintf(tmp_file, "    cmp  %s, %s\n", reg_lft, reg_rgt);
        fprintf(tmp_file, "    jle  .+0Ah\n");
        fprintf(tmp_file, "    mov  %s, 1\n", reg_lft);
        fprintf(tmp_file, "    jmp  .+08h\n");
        fprintf(tmp_file, "    mov  %s, 0\n", reg_lft);
      } break;

      case Less:
      {
        fprintf(tmp_file, "    cmp  %s, %s\n", reg_lft, reg_rgt);
        fprintf(tmp_file, "    jge  .+0Ah\n");
        fprintf(tmp_file, "    mov  %s, 1\n", reg_lft);
        fprintf(tmp_file, "    jmp  .+08h\n");
        fprintf(tmp_file, "    mov  %s, 0\n", reg_lft);
      } break;

      case Or:
      {
        fprintf(tmp_file, "    tst  %s\n", reg_lft);
        fprintf(tmp_file, "    jne  .+0Eh\n");
        fprintf(tmp_file, "    tst  %s\n", reg_rgt);
        fprintf(tmp_file, "    jne  .+0Ah\n");
        fprintf(tmp_file, "    mov  %s, 0\n", reg_lft);
        fprintf(tmp_file, "    jmp  .+08h\n");
        fprintf(tmp_file, "    mov  %s, 1\n", reg_lft);
      } break;

      case And:
      {
        fprintf(tmp_file, "    tst  %s\n", reg_lft);
        fprintf(tmp_file, "    je   .+0Eh\n");
        fprintf(tmp_file, "    tst  %s\n", reg_rgt);
        fprintf(tmp_file, "    je   .+0Ah\n");
        fprintf(tmp_file, "    mov  %s, 1\n", reg_lft);
        fprintf(tmp_file, "    jmp  .+08h\n");
        fprintf(tmp_file, "    mov  %s, 0\n", reg_lft);
      } break;

      case Mvor:
      case Bitor:
      {
        fprintf(tmp_file, "    or   %s, %s\n", reg_lft, reg_rgt);
      } break;

      case Mvand:
      case Bitand:
      {
        fprintf(tmp_file, "    and  %s, %s\n", reg_lft, reg_rgt);
      } break;

      case Mvxor:
      case Bitxor:
      {
        fprintf(tmp_file, "    xor  %s, %s\n", reg_lft, reg_rgt);
      } break;
    }

    FREE++;
} 

void
UNOOP(int op_id)
{
  switch(op_id)
  {
    case Sub:
    {
      fprintf(tmp_file, "    neg  %s\n", CUR_REG());
    } break;

    case Bitnot:
    {
      fprintf(tmp_file, "    not  %s\n", CUR_REG());
    } break;

    case Not:
    {
      fprintf(tmp_file, "    tst  %s\n", CUR_REG());
      fprintf(tmp_file, "    jne  .+0Ah\n");
      fprintf(tmp_file, "    mov  %s, 1\n", CUR_REG());
      fprintf(tmp_file, "    jmp  .+08h\n");
      fprintf(tmp_file, "    mov  %s, 0\n", CUR_REG());
    } break;
    
    case Add:
    default: break;
  }
}

const char *
CUR_REG()
{
    return reg[CUR_REG_NUM];
}

void
RESET_EXP()
{
    IN_MEM = 0; 
    FREE = 2;
}

void
PREF(int op_id, const char *var)
{
    MOV("1", t_num);
    OP(op_id);
    fprintf(tmp_file, "    mov  [%s], %s\n", var, CUR_REG());
}

void
POST(int op_id, const char *var)
{
    PUSH(CUR_REG());
    MOV("1", t_num);
    OP(op_id);
    fprintf(tmp_file, "    mov  [%s], %s\n", var, CUR_REG());
    POP(CUR_REG());
}