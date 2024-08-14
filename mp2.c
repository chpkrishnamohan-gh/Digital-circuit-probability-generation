#include<stdio.h>
#include<string.h>
#include<stdlib.h>

#define MAX_SIZE 10000

struct GateInfo
{
    char name[10];
    float p;
};

char v[MAX_SIZE][10];
int vcount = 0;

//stack section
typedef struct stack
{
       int top;
       int max_size;
       int count;
       char **instorage;
}stack;

stack* createCharStack(int stackSize)
{
      stack *vstack;
      vstack = (stack*)malloc(sizeof(stack));
      vstack->top = -1;
      vstack->max_size = stackSize;
      vstack->instorage = (char**)malloc(stackSize*sizeof(char*));
      if(vstack ==  NULL)
      { 
         return NULL;
      }
      return vstack;
}
int pushstack(stack *vstack,const char *p)
{
    printf("%s pushed\n",p);
    if(vstack->top + 1 == vstack->max_size)
    {
       printf("Stack is full.\n");
       return 0;
    }
    vstack->top++;
    vstack->instorage[vstack->top] = (char*)malloc(sizeof(p)+1);
    strcpy(vstack->instorage[vstack->top],p);
    vstack->count++;
    return 1;
}
int popstack(stack* vstack,char *dp)
{
    if(vstack->top == -1)
    {
       printf("Stack is empty.\n");
       return 0;
    }
    strcpy(dp,vstack->instorage[vstack->top]);
    vstack->top--;
    vstack->count--;
    return 1;
}
int freestack(stack *s)
{
    for(int i=0;i<s->max_size;i++)
    {
        free(s->instorage[i]);
    }
    free(s->instorage);
    free(s);
    return 1;
}
int isstackfull(stack* vstack)
{
    if(vstack->top + 1 == vstack->max_size)
        {
            return 1;
        }
        return 0;
}
int isstackempty(stack *vstack)
{
    if(vstack->top == -1)
    {
        return 1;
    }
    return 0;
} 
void displayStack(stack *Mystack)
{
    int t = Mystack->top;
    printf("***TOP***\n");
    for(int i=t;i!=-1;i--)
    {
        printf("%s\n",Mystack->instorage[i]);
    }
    printf("**END**\n");
}


char* takein(char *p,char *str)
{
    char *s = str;
    while(*s!='(')
    {
        s++;
    }
    s++;
    int i = 0;
    while(*s!=',' && *s != ')' && *s != ' ')
    {
        p[i] = *s;
        i++;
        s++;
    }
    p[i] = '\0';
    if(*s == ')')
    {
        return NULL;
    }
    return s;
}

char* GateExt(char *s,char *p)
{
    char *str = s;
    if(str == NULL)
    {
        return NULL;
    }
    while(*str==',')
    {
        str++;
    }
    while(*str==' ')
    {
        str++;
    }
    int i = 0;
    p[i] = *str;
    str++;
    i++;
    while (*str != ',' && *str != ')' && *str != ' ') {
        p[i] = *str;
        i++;
        str++;
    }
    p[i] = '\0';
    if(*str == ')')
    {
        return NULL;
    }
    return str;
}


int strequal(char *s1,char *s2)
{
    while(!(*s1 =='\0' && *s2=='\0'))
    {
        if(*s1 !='\0' && *s2=='\0')
        {
            return 0;
        }
        if(*s1 =='\0' && *s2!='\0')
        {
            return 0;
        }
        if(*s1 != *s2)
        {
            return 0;
        }
        s1++;
        s2++;
    }
    return 1;
}

int Is_Input(char* s,char p[][10],int size)
{
    char cgate[100];
    char *check = takein(cgate,s);
    int flag = 0;
    for(int i=0;i<size;i++)
    {
        if(strequal(cgate,p[i]))
        {
            flag = 1;
            break;
        }
    }
    if(flag == 0)
    {
        printf("Not a input\n");
        return 0;
    }
    else if(flag == 1 && check == NULL)
    {
        printf("Yes it is input\n");
        return 1;
    }
    else
    {
        while(check != NULL)
        {
            flag = 0;
            check = GateExt(check,cgate);
            for(int i=0;i<size;i++)
            {
                if(strequal(cgate,p[i]))
                {
                    flag = 1;
                    break;
                }
            }
            if(flag == 0)
            {
                printf("Not a input\n");
                return 0;
            }
        }
    }
    printf("Yes it is input\n");
    return 1;
}



int vis(char* p)
{
    for(int i =0;i<vcount;i++)
    {
        if(strequal(p,v[i]))
        {
            printf("Yes it is visited\n");
            return 1;
        }
    }
    printf("It is not visited\n");
    return 0;
}

void AddToVis(char *p)
{
    printf("%s added to vis\n",p);
    strcpy(v[vcount],p);
    vcount++;
}

void DFS(char* a,FILE *rf,stack *MyStack)
{
    printf("%s undergoing DFS\n",a);
    fseek(rf,0,SEEK_SET);
    char s[50];
    while(fgets(s,40,rf) != NULL)
    {
        int flag = 0;
        char g[100];
        GateExt(s,g);
        printf("Checking on %s\n",g);
        char p[100];
        char *ptr = takein(p,s);
        printf("Comparing %s %s \n",p,a);
        if(strcmp(p,a)==0)
        {
            flag =1;
        }
        else if(ptr != NULL)
        {
            while(ptr != NULL)
            {
                ptr = GateExt(ptr,p);
                printf("Comparing %s %s \n",p,a);
                if(strcmp(p,a) == 0)
                {
                    flag = 1;
                    break;
                }
            }
        }
        if(flag == 1 && (vis(g) != 1))
        {
            DFS(g,rf,MyStack);
            AddToVis(g);
            pushstack(MyStack,g);
            fseek(rf,0,SEEK_SET);
        }
    }
}

//PROBABILITY FUNCTIONS
float pOfAND(float pi1,float pi2)
{
    return pi1 * pi2;
}

float pOfNAND(float pi1, float pi2) {
    return 1.0f - (pi1 * pi2);
}

float pOfNOT(float p1) {
    return 1.0f - p1;
}

float pOfOR(float pi1, float pi2) {
    return ((1.0f - pi1) * (1.0f - pi2));
}
float pOfNOR(float pi1, float pi2) {
    return 1.0f - ((1.0f - pi1) * (1.0f - pi2));
}

float pOfXOR(float pi1, float pi2) {
    return (1.0f - pi1) * (pi2) + (pi1) * (1.0f - pi2);
}

float pOfXNOR(float pi1, float pi2) {
    return 1.0f - ((1.0f - pi1) * (pi2) + (pi1) * (1.0f - pi2));
}

float pOfBUFF(float pi1)
{
    return pi1;
}


float gateDeter(char *s,float f1,float f2)
{
    while(*s != '=')
    {
        s++;
    }
    s = s + 2;
    char g[30];
    int i = 0;
    while(*s != '(')
    {
        g[i] = *s;
        i++;
        s++;
    }
    g[i] = '\0';
    char and[5] = "AND";
    char or[5] = "OR";
    char not[5] = "NOT";
    char nand[5] = "NAND";
    char xor[5] = "XOR";
    char nor[5] = "NOR";
    char xnor[5] = "XNOR";
    char buff[5] = "BUFF";
    if(!strcmp(g,and))
    {
        return pOfAND(f1,f2);
    }
    else if(!strcmp(g,or))
    {
        return pOfOR(f1,f2);
    }
    else if(!strcmp(g,not))
    {
        return pOfNOT(f1);
    }
    else if (!strcmp(g, nand))
    {
    return pOfNAND(f1, f2);
    }
    else if (!strcmp(g, xor))
    {
    return pOfXOR(f1, f2);
    }
    else if (!strcmp(g, nor))
    {
    return pOfNOR(f1, f2);
    }
    else if (!strcmp(g, xnor))
    {
    return pOfXNOR(f1, f2);
    }
    else if(!strcmp(g,buff))
    {
    return pOfBUFF(f1);
    }
}

float probability(char *p,struct GateInfo GI[],int count)
{
    for(int i=0;i<count;i++)
    {
        if(strcmp(p,GI[i].name) == 0)
        {
            return GI[i].p;
        }
    }
    return 0;
}

float Dprobab(char *s,int n,struct GateInfo GI[])
{
    char *check;
    char g1[100];
    char g2[100];
    check = takein(g1,s);
    if(check != NULL)
    {
        float f = probability(g1,GI,n);
        while(check != NULL)
        {
            check = GateExt(check,g2);
            f = gateDeter(s,f,probability(g2,GI,n));
        }
        return f;
    }
    else
    {
        return gateDeter(s,probability(g1,GI,n),0);
    }
}



int main()
{
    stack *MyStack = createCharStack(MAX_SIZE);

    struct GateInfo GI[MAX_SIZE];

    int count = 0;

    FILE *ptr = fopen("c880.bench","r");
    char s[50];
    fgets(s,40,ptr);
    int max = 0;
    while(strstr(s,"OUTPUT") == NULL && strstr(s,"INPUT") != NULL)
    {
        max++;
        fgets(s,40,ptr);
    }
    fseek(ptr,0,SEEK_SET);
    int In_Size = max;
    char pIn[max][10];
    fgets(s,40,ptr);
    int i = 0;
    while(strstr(s,"OUTPUT") == NULL && strstr(s,"INPUT") != NULL)
    {
        takein(pIn[i],s);
        i++;
        fgets(s,40,ptr);
    }

    fseek(ptr,0,SEEK_SET);
    fgets(s,40,ptr);
    while(strstr(s,"INPUT") != NULL)
    {
        fgets(s,40,ptr);
    }
    fgets(s,40,ptr);
    while(strstr(s,"OUTPUT") != NULL)
    {
        fgets(s,40,ptr);
    } 
    FILE * rf = fopen("relationfile.txt","w");
    while(fgets(s,40,ptr) != NULL)
    {
        fputs(s,rf);
    }
    fclose(rf);
    fclose(ptr);

    

    rf = fopen("relationfile.txt","r");
    while(fgets(s,40,rf) != NULL)
    {
        char p[100];
        GateExt(s,p);
        printf("%s has been recieved\n",p);
        if(Is_Input(s,pIn,In_Size) && !vis(p))
        {
            DFS(p,rf,MyStack);
            AddToVis(p);
            pushstack(MyStack,p);
            fseek(rf,0,SEEK_SET);
        }
    }


    FILE *result = fopen("result.txt","w");
    fprintf(result,"{");
    int k;
    for(k=0;k<In_Size;k++)
    {
        strcpy(GI[k].name,pIn[k]);
        count++;
        GI[k].p = 0.5;
        fprintf(result,"'%s': ",pIn[k]);
        fprintf(result,"[0.5, 0.5], ");
    }
    fseek(rf,0,SEEK_SET);
    char data[100];
    while(!isstackempty(MyStack))
    {
        char *check;
        char p1[100];
        popstack(MyStack,data);
        strcpy(GI[k].name,data);
        count++;
        check = fgets(s,40,rf);
        GateExt(s,p1);
        while(!strequal(p1,data) &&  check!= NULL)
        {
            check = fgets(s,40,rf);
            GateExt(s,p1);
        }
        GI[k].p = Dprobab(s,k,GI);
        fprintf(result,"'%s': ",GI[k].name);
        if(isstackempty(MyStack))
        {
            fprintf(result,"[%f, %f]",1.0-GI[k].p,GI[k].p);
        }
        else
        {
            fprintf(result,"[%f, %f],",1.0-GI[k].p,GI[k].p);
        }
        k++;
        fseek(rf,0,SEEK_SET);
    }
    fprintf(result,"}");
    fclose(result);
    fclose(rf);
    freestack(MyStack);
    return 0;
}