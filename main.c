#include <stdio.h>
#include <string.h>

int main(int argc, char *argv[]) {  
    if (argc != 3) {
        printf("Uso: nome-arquivo num-thread\n");
        return 0;
    }

    FILE *fw_bin = NULL;
    FILE *fl_bin = NULL;
    char* nread, nwrite;
  
    strcpy(nread,argv[1]);
    strcpy(nwrite,argv[1]);
    strcat(nread,".mips");
    strcat(nwrite,".bin");
  
    fl_bin = fopen(nread, "rb");
    if (fl_bin == NULL) {
        printf("Arquivo nao encontrado\n");
        return 0;
    }
  
  
    printf("Arquivo lido\n");
    fclose(fl_bin);
  
    
    fw_bin = fopen(nwrite, "wb");
    if (fw_bin == NULL) {
        perror("Erro ao criar o arquivo\n");
        return 0;
    }
    fclose(fw_bin);
  
    return 0;
}
