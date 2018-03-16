#ifndef LAB2_LIBRARY_H
#define LAB2_LIBRARY_H

#define M_SIZE 100

// Коды ошибок
#define ERR_OVERFLOW 1 // Переполнение при выполнении операции
#define ERR_DIVIDE 2 // Ошибка деления на 0
#define ERR_MEMORY 3 // Выход за границы доступной памяти
#define ERR_INTERRUPT 4 // Игнорирование тактовых импульсов
#define ERR_INVALIDCMD 5 // Указана неверная команда

static int *memory;
int gReg;

int sc_memoryInit();
int sc_memorySet(int address, int value);
int sc_memoryGet(int address, int* value);
int sc_memorySave(char* filename);
int sc_memoryLoad(char* filename);
int sc_regInit(void);
int sc_regSet(int reg, int value);
int sc_regGet(int reg, int* value);
int sc_commandEncode(int command, int operand, int* value);
int sc_commandDecode(int value, int* command, int* operand);

#endif
