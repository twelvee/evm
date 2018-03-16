#include "lib.c"

#include <stdio.h>

int main() {
    int value, command, operand, reg, address;

    sc_memoryInit();
    sc_regInit();

    sc_regGet(ERR_DIVIDE, &value);
    printf("%d\n", value);
    sc_regSet(ERR_DIVIDE, 1);
    sc_regGet(ERR_DIVIDE, &value);
    printf("%d\n", value);
    sc_regSet(ERR_DIVIDE, 0);
    sc_regGet(ERR_DIVIDE, &value);
    printf("%d\n", value);

    sc_memorySet(0, 0xF);
    sc_memoryGet(0, &value);
    printf("%d\n",value);

    sc_memorySave("dump");
    sc_memorySet(0, 0);
    sc_memoryGet(0, &value);
    printf("%d\n",value);
    sc_memoryLoad("dump");
    sc_memoryGet(0, &value);
    printf("%d\n",value);

    sc_commandEncode(0x10, 0x10, &value);
    printf("%x\n",value);
    sc_commandDecode(value, &command, &operand);
    printf("0x%x 0x%x\n", command, operand);
}
