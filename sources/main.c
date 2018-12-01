#include <stdio.h>

extern void Install_IRQ_handler(void);
extern void Restore_IRQ_handler(void);
extern void Wait_IRQ(void);

typedef struct
{
    unsigned char t;
    unsigned char r;
    unsigned char v;
} AYDATA;

extern AYDATA AYData[];
extern unsigned int AYData_Size;

extern unsigned char AYReg;
extern unsigned char AYVal;
extern void AY_SetReg();
extern void AY_Stop();


int main()
{
    AYDATA *ptr = AYData;
    unsigned int size = AYData_Size / sizeof(AYDATA);
    unsigned char* status = (unsigned char*)0xBB80;
    unsigned int i = 0;
    unsigned char j = 0;

    memset(status, '-', 32);
    status[0] = '[';
    status[31] = ']';

    Install_IRQ_handler();

    for(i=0; i<size; i++)
    {
        status[1+((i*30)/size)] = '*';
        for(j=0; j<ptr->t; j++)
        {
            Wait_IRQ();
        }
        AYReg = ptr->r;
        AYVal = ptr->v;
        AY_SetReg();

        ptr++;
    }
    AY_Stop();

    Restore_IRQ_handler();

    memset(status, ' ', 32);
}
