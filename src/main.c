#include <stdio.h>
#include <stdlib.h>

int main() {
    FILE* asfile = fopen("umlcc.s", "w");
    fprintf(asfile, "    .text\n");
    fprintf(asfile, "    .global _main\n");
    fprintf(asfile, "_main:\n");
    fprintf(asfile, "    pushq  %%rbp\n");
    fprintf(asfile, "    movq  %%rsp, %%rbp\n");
    fprintf(asfile, "    movl  $0, %%eax\n");
    fprintf(asfile, "    popq  %%rbp\n");
    fprintf(asfile, "    ret\n");
    fclose(asfile);
    return 0;
}
