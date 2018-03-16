#include "lib.h"

#include <stdio.h>
#include <stdlib.h>

int gCmds[] = {
        0x10, 0x11, 0x20, 0x21,
        0x30, 0x31, 0x32, 0x33,
        0x40, 0x41, 0x42, 0x43,
        0x51, 0x52, 0x53, 0x54,
        0x55, 0x56, 0x57, 0x58,
        0x59, 0x60, 0x61, 0x62,
        0x63, 0x64, 0x65, 0x66,
        0x67, 0x68, 0x69, 0x70,
        0x71, 0x72, 0x73, 0x74,
        0x75, 0x76
};

int sc_memoryInit() {
    free(memory);
    memory = (int*)malloc(M_SIZE * sizeof(int));

    if (memory == NULL)
        return -1;

    for(int i = 0; i < M_SIZE; i++)
        memory[i] = 0x0;
    return 0;
}

int sc_memorySet(int address, int value) {
    if (address >= 0 && address < M_SIZE) {
        memory[address] = value;
        return 0;
    } else {
        sc_regSet(ERR_MEMORY, 1);
        return -1;
    }
}

int sc_memoryGet(int address, int* value) {
    if (address >= 0 && address < M_SIZE && value != NULL) {
        *value = memory[address];
        return 0;
    } else {
        sc_regSet(ERR_MEMORY, 1);
        return -1;
    }
}

int sc_memorySave(char* filename) {
    FILE *RAMDUMP;
    RAMDUMP = fopen(filename, "wb");

    if (RAMDUMP == NULL)
        return -1;
    else {
        if (fwrite(memory, sizeof(*memory) * M_SIZE, 1, RAMDUMP)) {
            fclose(RAMDUMP);
        }
    }
    return 0;
}

int sc_memoryLoad(char* filename) {
    FILE *RAMDUMP;
    RAMDUMP = fopen(filename, "rb");

    if (RAMDUMP == NULL)
        return -1;
    else {
        if (fread(memory, sizeof(*memory) * M_SIZE, 1, RAMDUMP)) {
            for(int i = 0; i < M_SIZE; i++)
                memory[i] &= 0x7FFF;
            fclose(RAMDUMP);
        }
    }
    return 0;
}

int sc_regInit(void) {
    gReg = 0x0;
    return 0;
}

int sc_regSet(int reg, int value) {
    if (reg > 0 && reg <= 5) {
          if (value == 1) {
                gReg = gReg | (1 << (reg - 1));
          } else if(value == 0)  {
                gReg = gReg & (~(1 << (reg - 1)));
          } else
              return -1;
    } else
        return -1;

    return 0;
}

int sc_regGet(int reg, int* value) {
    if (reg > 0 && reg <= 5) {
        *value = (gReg >> (reg -1)) & 1;
    } else
        return -1;

    return 0;
}

int sc_commandEncode(int command, int operand, int* value) {
    int* cmd;
    for (int i = 0; i < 38; i++) {
        if (gCmds[i] == command) {
            cmd = &gCmds[i];
            break;
        }
    }

    if (cmd == NULL) {
        sc_regSet(ERR_INVALIDCMD, 1);
        return -1;
    } else {
        if (operand > 0x7F) {
            sc_regSet(ERR_OVERFLOW, 1);
            return -1;
        }
        if (value != NULL) {
            *value = (command << 7) | operand;
            sc_regSet(ERR_INVALIDCMD, 0);
        }
    }
    return 0;
}

int sc_commandDecode(int value, int* command, int* operand) {
    int isCmd = (value >> 14) & 1,
        tempCmd, tempOper;
    int *ptr;

    if (command != NULL && operand != NULL) {
        if (isCmd == 0) {
            tempCmd = (value >> 7) & 0x7F;
            tempOper = value & 0x7F;
            for (int i = 0; i < 38; i++) {
                if (gCmds[i] == tempCmd) {
                    ptr = &gCmds[i];
                    break;
                }
            }
        }
        if (ptr != NULL) {
            *command = tempCmd;
            *operand = tempOper;
        } else {
            sc_regSet(ERR_INVALIDCMD, 1);
            return -1;
        }
    }
    return 0;
}
