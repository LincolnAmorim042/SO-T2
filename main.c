#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

enum type {i,j,r};

struct instruction{
    enum type tipo;
    u_int8_t op;
    u_int8_t func;
    int16_t imed;
    int end:26;
    u_int8_t t,d,s,shamp;
};
struct instruction oprt;

//pula as letras iniciais da string
int pulaIni(char* var){
    int i = 0;
    while(i<50) {
        if (isdigit(var[i])) {
            return i;
        } else {
            i++;
        }
    }
    return -1;
}

//remove caracteres especiais da string
void limpaString(char** str){
    int i=0,j=0;
    char aux[32];
    while ((*str)[i]!='\0'){
        if(isdigit((*str)[i])||((*str)[i]>='A'&&(*str)[i]<='Z')||((*str)[i]>='a'&&(*str)[i]<='z')){
            aux[j]=(*str)[i];
            j++;
        }
        i++;
    }
    aux[j] = '\0';
    strcpy((*str),aux);
}

//preenche as variaveis referentes ao tipo r para 3 parametros
void instR3(char *pt){
    int i;
    if((i=pulaIni(pt))>=0)pt=pt+i;
    oprt.d = (u_int8_t) atoi(pt);
    pt = strtok(NULL,",");
    if((i=pulaIni(pt))>=0)pt=pt+i;
    oprt.s = (u_int8_t) atoi(pt);
    pt = strtok(NULL,",");
    if((i=pulaIni(pt))>=0)pt=pt+i;
    oprt.t = (u_int8_t) atoi(pt);
}

//preenche as variaveis referentes ao tipo r para 2 parametros
void instR2(char *pt){
    int i;
    oprt.d = 0;
    if((i=pulaIni(pt))>=0)pt=pt+i;
    oprt.s = (u_int8_t) atoi(pt);
    pt = strtok(NULL,",");
    if((i=pulaIni(pt))>=0)pt=pt+i;
    oprt.t = (u_int8_t) atoi(pt);
}

//preenche as variaveis do tipo i começando pelo reg T
void instI3ts(char *pt){
    int i;
    if((i=pulaIni(pt))>=0)pt=pt+i;
    oprt.t = (u_int8_t) atoi(pt);
    pt = strtok(NULL,",");
    if((i=pulaIni(pt))>=0)pt=pt+i;
    oprt.s = (u_int8_t) atoi(pt);
    pt = strtok(NULL,",");
    oprt.imed = (int16_t)atoi(pt);
}

//preenche as variaveis do tipo i começando pelo reg S
void instI3st(char *pt){
    int i;
    if((i=pulaIni(pt))>=0)pt=pt+i;
    oprt.s = (u_int8_t) atoi(pt);
    pt = strtok(NULL,",");
    if((i=pulaIni(pt))>=0)pt=pt+i;
    oprt.t = (u_int8_t) atoi(pt);
    pt = strtok(NULL,",");
    oprt.imed = (int16_t)atoi(pt);
}

//preenche as variaveis com os valores passados pelo arquivo
void getop(char* str,FILE *f){
    char* pt;
    char tx2[50];
    memset(tx2,0,50);
    if (strcmp(str,"syscall")==0){
        oprt.op = 0;
        oprt.end = 12;
        oprt.tipo = j;
    }else {
        if (fscanf(f, "%s", tx2) != EOF) {
            pt=strtok(tx2, ",");
            if (strcmp(str, "add") == 0) {
                oprt.tipo = r;
                oprt.op = 0;
                oprt.func = 0x20;
                oprt.shamp = 0;
                instR3(pt);
                return;
            }
            else if (strcmp(str, "addi") == 0) {
                oprt.tipo = i;
                oprt.op = 8;
                instI3ts(pt);
                return;
            }
            else if (strcmp(str, "sub") == 0) {
                oprt.tipo = r;
                oprt.op = 0;
                oprt.func = 0x22;
                oprt.shamp = 0;
                instR3(pt);
                return;
            }
            else if (strcmp(str, "mult") == 0) {
                oprt.tipo = r;
                oprt.op = 0;
                oprt.func = 0x18;
                oprt.shamp = 0;
                instR2(pt);
                return;
            }
            else if(strcmp(str, "div") == 0) {
                oprt.tipo = r;
                oprt.op = 0;
                oprt.func = 0x1a;
                oprt.shamp = 0;
                instR2(pt);
                return;
            }
            else if(strcmp(str, "and") == 0) {
                oprt.tipo = r;
                oprt.op = 0;
                oprt.func = 0x24;
                oprt.shamp = 0;
                instR3(pt);
                return;
            }
            else if(strcmp(str, "andi") == 0) {
                oprt.tipo = i;
                oprt.op = 12;
                instI3ts(pt);
                return;
            }
            else if(strcmp(str, "or") == 0) {
                oprt.tipo = r;
                oprt.op = 0;
                oprt.func = 0x25;
                oprt.shamp = 0;
                instR3(pt);
                return;
            }
            else if(strcmp(str, "xor") == 0) {
                oprt.tipo = r;
                oprt.op = 0;
                oprt.func = 0x26;
                oprt.shamp = 0;
                instR3(pt);
                return;
            }
            else if(strcmp(str, "ori") == 0) {
                oprt.tipo = i;
                oprt.op = 13;
                instI3ts(pt);
                return;
            }
            else if(strcmp(str, "slt") == 0) {
                oprt.tipo = r;
                oprt.op = 0;
                oprt.func = 0x2a;
                oprt.shamp = 0;
                instR3(pt);
                return;
            }
            else if(strcmp(str, "slti") == 0) {
                oprt.tipo = i;
                oprt.op = 10;
                instI3ts(pt);
                return;
            }
            else if(strcmp(str, "sll") == 0) {
                oprt.tipo = r;
                oprt.op = 0;
                oprt.func = 0x00;
                oprt.s = 0;
                int i;
                if((i=pulaIni(pt))>=0)pt=pt+i;
                oprt.d = (u_int8_t) atoi(pt);

                pt = strtok(NULL,",");
                if((i=pulaIni(pt))>=0)pt=pt+i;
                oprt.t = (u_int8_t) atoi(pt);

                pt= strtok(NULL,",");
                if((i=pulaIni(pt))>=0)pt=pt+i;
                oprt.shamp = (u_int8_t) atoi(pt);
                return;
            }
            else if(strcmp(str, "srl") == 0) {
                oprt.tipo = r;
                oprt.op = 0;
                oprt.func = 0x02;
                oprt.s = 0;
                int i;
                if((i=pulaIni(pt))>=0)pt=pt+i;
                oprt.d = (u_int8_t) atoi(pt);

                pt = strtok(NULL,",");
                if((i=pulaIni(pt))>=0)pt=pt+i;
                oprt.t = (u_int8_t) atoi(pt);

                pt = strtok(NULL,",");
                if((i=pulaIni(pt))>=0)pt=pt+i;
                oprt.shamp = (u_int8_t) atoi(pt);
                return;
            }
            else if(strcmp(str, "j") == 0) {
                oprt.tipo = j;
                oprt.op = 2;
                oprt.end = atoi(pt);
                return;
            }
            else if(strcmp(str, "beq") == 0) {
                oprt.tipo = i;
                oprt.op = 4;
                instI3st(pt);
                return;
            }
            else if(strcmp(str, "bne") == 0) {
                oprt.tipo = i;
                oprt.op = 5;
                instI3st(pt);
                return;
            }
            else if(strcmp(str, "bgez") == 0) {
                oprt.tipo = i;
                oprt.op = 1;
                oprt.t = 1;
                int i;
                if((i=pulaIni(pt))>=0)pt=pt+i;
                oprt.s = (u_int8_t) atoi(pt);

                pt = strtok(NULL,",");
                oprt.imed = (u_int8_t) atoi(pt);
                return;
            }
            else if(strcmp(str, "bgtz") == 0) {
                oprt.tipo = i;
                oprt.op = 7;
                oprt.t = 0;
                int i;
                if((i=pulaIni(pt))>=0)pt=pt+i;
                oprt.s = (u_int8_t) atoi(pt);

                pt = strtok(NULL,",");
                oprt.imed = (u_int8_t) atoi(pt);
                return;
            }
            else if(strcmp(str, "lb") == 0) {
                oprt.tipo = i;
                oprt.op = 32;
                int i;
                if((i=pulaIni(pt))>=0)pt=pt+i;
                oprt.t = (u_int8_t) atoi(pt);
                pt = strtok(NULL,",");
                pt = strtok(pt,"(");
                oprt.imed = atoi(pt);
                pt = strtok(NULL,"(");
                limpaString(&pt);
                pt = pt+pulaIni(pt);
                oprt.s = atoi(pt);
                return;
            }
            else if(strcmp(str, "sb") == 0) {
                oprt.tipo = i;
                oprt.op = 40;
                int i;
                if((i=pulaIni(pt))>=0)pt=pt+i;
                oprt.t = (u_int8_t) atoi(pt);
                pt = strtok(NULL,",");
                pt = strtok(pt,"(");
                oprt.imed = atoi(pt);
                pt = strtok(NULL,"(");
                limpaString(&pt);
                pt = pt+pulaIni(pt);
                oprt.s = atoi(pt);
                return;
            }
            else if(strcmp(str, "lw") == 0) {
                oprt.tipo = i;
                oprt.op = 35;
                int i;
                if((i=pulaIni(pt))>=0)pt=pt+i;
                oprt.t = (u_int8_t) atoi(pt);
                pt = strtok(NULL,",");
                pt = strtok(pt,"(");
                oprt.imed = atoi(pt);
                pt = strtok(NULL,"(");
                limpaString(&pt);
                pt = pt+pulaIni(pt);
                oprt.s = atoi(pt);
                return;
            }
            else if(strcmp(str, "sw") == 0) {
                oprt.tipo = i;
                oprt.op = 43;
                int i;
                if((i=pulaIni(pt))>=0)pt=pt+i;
                oprt.t = (u_int8_t) atoi(pt);
                pt = strtok(NULL,",");
                pt = strtok(pt,"(");
                oprt.imed=atoi(pt);
                pt = strtok(NULL,"(");
                limpaString(&pt);
                pt = pt+pulaIni(pt);
                oprt.s = atoi(pt);
                return;
            }
            else if(strcmp(str, "lui") == 0) {
                oprt.tipo = i;
                oprt.op = 15;
                int i;
                if((i=pulaIni(pt))>=0)pt=pt+i;
                oprt.t =(u_int8_t) atoi(pt);
                pt= strtok(NULL,",");
                oprt.imed = atoi(pt);
                oprt.s = 0;
                return;
            }
            else if(strcmp(str, "readb") == 0) {
                oprt.tipo = j;
                oprt.op = 60;
                oprt.end = 0;
                return;
            }
            else if(strcmp(str, "writeb") == 0) {
                oprt.tipo = j;
                oprt.op = 61;
                oprt.end = 0;
            }
        }
    }
}

int main(int argv, char **argc) {
    int32_t inst;
    FILE *fr, *fw;

    if(argv != 2){
        perror("uso: nomearquivo\n");
        return 1;
    }

    fr = fopen(argc[1], "r");

    if (fr == NULL) {
        perror("Arquivo nao encontrado\n");
        return 1;
    }
  
    char txt[10], aux[50];
    int temp=0,temp2=0;
    while (argc[1][temp] != '.'){
        aux[temp2++] = argc[1][temp++];
    }
    aux[temp2] = '\0';
    strcat(aux,".bin");
    fw = fopen(aux,"w");
    while (fscanf(fr, "%s", txt) != EOF) {
        getop(txt, fr);
        inst = 0;
        switch (oprt.tipo) {
            case i:
                inst = oprt.op;
                inst = (inst << 5) | oprt.s;
                inst = (inst << 5) | oprt.t;
                inst = (inst << 16);
                unsigned int aux;
                aux = oprt.imed << 16;
                aux = aux >> 16;
                inst = inst| aux;
                break;
            case j:
                inst = oprt.op << 26;
                inst = inst|oprt.end;
                break;
            case r:
                inst = oprt.op;
                inst = (inst << 5) | oprt.s;
                inst = (inst << 5) | oprt.t;
                inst = (inst << 5) | oprt.d;
                inst = (inst << 5) | oprt.shamp;
                inst = (inst << 6) | oprt.func;
                break;
            default:
                return 1;
                break;
        }
        fwrite(&inst, sizeof(int32_t),1,fw);
    }

    fclose(fw);
    fclose(fr);
    return 0;
}
