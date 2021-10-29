#include <stdarg.h>
#include <stdio.h>
#include <cstring>

//#define __arm__
//#define ARM9
#ifdef __arm__

extern "C"
{
    #include "doomdef.h"
    #include "doomtype.h"
    #include "d_main.h"
    #include "d_event.h"

    #include "global_data.h"

    #include "tables.h"
}

#include "i_system_e32.h"

#include "lprintf.h"

//#include <nds.h>

#include "nds/ndstypes.h"
#include "nds/dma.h"
#include "nds/interrupts.h"
//#include "nds/memory.h"
//#include "nds/system.h"
#include "nds/timers.h"
#include "nds/input.h"

#include "nds/arm9/dynamicArray.h"
#include "nds/arm9/console.h"
#include "nds/arm9/exceptions.h"
#include "nds/arm9/image.h"
#include "nds/arm9/input.h"
#include "nds/arm9/math.h"
#include "nds/arm9/video.h"

#include <nds/input.h>
#include <nds/timers.h>
#include <nds/registers_alt.h>

#include <maxmod9.h>

#define DCNT_PAGE 0x0010

#define VID_PAGE1 VRAM
#define VID_PAGE2 0x600A000

#define TM_FREQ_1024 0x0003
#define TM_ENABLE 0x0080
#define TM_CASCADE 0x0004
#define TM_FREQ_1024 0x0003
#define TM_FREQ_256 0x0002

//#define REG_WAITCNT	*((vu16 *)(0x4000204))  //Unneeded on NDS?
//BUS_CLOCK 


//**************************************************************************************


//*******************************************************************************
//VBlank handler.
//*******************************************************************************

void VBlankCallback()
{
    //mmVBlank();
    //swiWaitForVBlank();
    //mmFrame(); //Fixme
    //int count = 60; //Figure out how long to waste cycles.
    //while(count--) swiWaitForVBlank(); //Replaces mmFrame()
}


void I_InitScreen_e32()
{
    irqInit();

    irqSet( IRQ_VBLANK, VBlankCallback );
    irqEnable(IRQ_VBLANK);


    //Set gamepak wait states and prefetch.
    //REG_WAITCNT = 0x46DA; //Unneeded on NDS?

    consoleDemoInit();

    //REG_TM2CNT_L= 65535-1872;     // 1872 ticks = 1/35 secs
    //REG_TM2CNT_H = TM_FREQ_256 | TM_ENABLE;       // we're using the 256 cycle timer
    TIMER2_DATA = 65535-1872; //NDS
    TIMER2_CR = TIMER_FREQ(256) | TIMER_ENABLE; //NDS

    // cascade into tm3
    //REG_TM3CNT_H = TM_CASCADE | TM_ENABLE;
    TIMER3_CR = TIMER_CASCADE | TIMER_ENABLE; //NDS
}

//**************************************************************************************

void I_BlitScreenBmp_e32()
{

}

//**************************************************************************************

void I_StartWServEvents_e32()
{

}

//**************************************************************************************

void I_PollWServEvents_e32()
{
    scanKeys();

    uint key_down = keysDown();

    event_t ev;

    if(key_down)
    {
        ev.type = ev_keydown;

        if(key_down & KEY_SELECT)
        {
            ev.data1 = KEYD_SELECT;
            D_PostEvent(&ev);
        }

        if(key_down & KEY_UP)
        {
            ev.data1 = KEYD_UP;
            D_PostEvent(&ev);
        }

        if(key_down & KEY_DOWN)
        {
            ev.data1 = KEYD_DOWN;
            D_PostEvent(&ev);
        }

        if(key_down & KEY_LEFT)
        {
            ev.data1 = KEYD_LEFT;
            D_PostEvent(&ev);
        }

        if(key_down & KEY_RIGHT)
        {
            ev.data1 = KEYD_RIGHT;
            D_PostEvent(&ev);
        }

        if(key_down & KEY_START)
        {
            ev.data1 = KEYD_START;
            D_PostEvent(&ev);
        }

        if(key_down & KEY_A)
        {
            ev.data1 = KEYD_A;
            D_PostEvent(&ev);
        }

        if(key_down & KEY_B)
        {
            ev.data1 = KEYD_B;
            D_PostEvent(&ev);
        }

        if(key_down & KEY_L)
        {
            ev.data1 = KEYD_L;
            D_PostEvent(&ev);
        }

        if(key_down & KEY_R)
        {
            ev.data1 = KEYD_R;
            D_PostEvent(&ev);
        }
    }

    uint key_up = keysUp();

    if(key_up)
    {
        ev.type = ev_keyup;

        if(key_up & KEY_SELECT)
        {
            ev.data1 = KEYD_SELECT;
            D_PostEvent(&ev);
        }

        if(key_up & KEY_UP)
        {
            ev.data1 = KEYD_UP;
            D_PostEvent(&ev);
        }

        if(key_up & KEY_DOWN)
        {
            ev.data1 = KEYD_DOWN;
            D_PostEvent(&ev);
        }

        if(key_up & KEY_LEFT)
        {
            ev.data1 = KEYD_LEFT;
            D_PostEvent(&ev);
        }

        if(key_up & KEY_RIGHT)
        {
            ev.data1 = KEYD_RIGHT;
            D_PostEvent(&ev);
        }

        if(key_up & KEY_START)
        {
            ev.data1 = KEYD_START;
            D_PostEvent(&ev);
        }

        if(key_up & KEY_A)
        {
            ev.data1 = KEYD_A;
            D_PostEvent(&ev);
        }

        if(key_up & KEY_B)
        {
            ev.data1 = KEYD_B;
            D_PostEvent(&ev);
        }

        if(key_up & KEY_L)
        {
            ev.data1 = KEYD_L;
            D_PostEvent(&ev);
        }

        if(key_up & KEY_R)
        {
            ev.data1 = KEYD_R;
            D_PostEvent(&ev);
        }
    }
}

//**************************************************************************************

void I_ClearWindow_e32()
{

}

//**************************************************************************************

unsigned short* I_GetBackBuffer()
{
    if(SUB_DISPLAY_CR & DCNT_PAGE)
        return (unsigned short*)VID_PAGE1;

    return (unsigned short*)VID_PAGE2;
}

//**************************************************************************************

void I_CreateWindow_e32()
{

    //Bit5 = unlocked vram at h-blank.
    //SetMode(MODE_4 | BG2_ENABLE | BIT(5));
    //videoSetMode(MODE_4_2D | DISPLAY_BG2_ACTIVE | BIT(5));
    videoSetMode(MODE_4_2D);//Fixme?
    videoBgEnable(2);//NDS Unneeded?

    unsigned short* bb = I_GetBackBuffer();

    memset(bb, 0, 240*160);

    I_FinishUpdate_e32(NULL, NULL, 0, 0);

    bb = I_GetBackBuffer();

    memset(bb, 0, 240*160);

    I_FinishUpdate_e32(NULL, NULL, 0, 0);

}

//**************************************************************************************

void I_CreateBackBuffer_e32()
{
    I_CreateWindow_e32();
}

//**************************************************************************************

void I_FinishUpdate_e32(const byte* srcBuffer, const byte* pallete, const unsigned int width, const unsigned int height)
{
    SUB_DISPLAY_CR ^= DCNT_PAGE;
}

//**************************************************************************************

void I_SetPallete_e32(const byte* pallete)
{
    unsigned short* pal_ram = (unsigned short*)0x5000000;

    for(int i = 0; i< 256; i++)
    {
        unsigned int r = *pallete++;
        unsigned int g = *pallete++;
        unsigned int b = *pallete++;

        pal_ram[i] = RGB5(r >> 3, g >> 3, b >> 3);
    }
}

//**************************************************************************************

int I_GetVideoWidth_e32()
{
    return 120;
}

//**************************************************************************************

int I_GetVideoHeight_e32()
{
    return 160;
}



//**************************************************************************************

void I_ProcessKeyEvents()
{
    I_PollWServEvents_e32();
}

//**************************************************************************************

#if	defined	( __thumb__ )
#define	SystemCall(Number)	 __asm ("SWI	  "#Number"\n" :::  "r0", "r1", "r2", "r3")
#else
#define	SystemCall(Number)	 __asm ("SWI	  "#Number"	<< 16\n" :::"r0", "r1", "r2", "r3")
#endif

#define MAX_MESSAGE_SIZE 1024

void I_Error (const char *error, ...)
{
    consoleDemoInit();

    char msg[MAX_MESSAGE_SIZE];

    va_list v;
    va_start(v, error);

    vsprintf(msg, error, v);

    va_end(v);

    printf("%s", msg);

    while(true)
    {
        //VBlankIntrWait();
        //SystemCall(5); //Fixme?
        swiWaitForVBlank();
    }
}

//**************************************************************************************

void I_Quit_e32()
{

}

//**************************************************************************************

#endif
