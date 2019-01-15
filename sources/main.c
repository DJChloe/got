#include <stdio.h>
#include <lib.h>

#define TITLE "Game of Thrones - Main Theme"
#define ME "by Chlo! Avrillon for Oric Atmos"

#define plot(X,Y,C) poke(0xBBA8+(X)+(Y)*40,C)

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
void plots(char x,char y,const char *msg);


void main()
{
    AYDATA *ptr = AYData;
    unsigned int size = AYData_Size / sizeof(AYDATA);
    unsigned char* status = (unsigned char*)0xBBA8+5+13*40;
    unsigned int i = 0;
    unsigned char j = 0;
    unsigned int progressvalues[31];
    unsigned int* progress=progressvalues;
	unsigned int n = 0;
	setflags(SCREEN+NOKEYCLICK);
	
	//é (!)
	memcpy((unsigned char*)46344,(unsigned char*)46888,8);
	poke(46344,4);
	poke(46345,8);
	
	
	cls(); paper(4); ink(5);
	memset((unsigned char*)0xBBA3, ' ', 5); //hides Caps Lock
	
	plot(1,11,A_FWWHITE);
	plots(20-strlen(TITLE)/2,11,TITLE);
	plot(1,26,A_FWCYAN);
	plots(20-strlen(ME)/2,26,ME);
    *(status-1)=A_ALT;
	memset(status, 160, 30);    
    memset(progressvalues, 0, 31);
    
    for(n=0; n<30; n++)
    {
        progressvalues[n]=(size/30)*n;
    }    
    Install_IRQ_handler();
	progress=progressvalues;
    for(i=0; i<size; i++)
    {
        if (i==*progress) {
			*status=95;
			status++;
			progress++;	
		}
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
}

void plots(char x_pos,char y_pos, const char *msg)
{
    memcpy((unsigned char*)(0xBBA8+x_pos+y_pos*40),msg,strlen(msg));	
}
