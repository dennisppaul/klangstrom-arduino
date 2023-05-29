#include "Rakarrack.h"

#ifndef KLANG_AUDIO_RATE
#error KLANG_AUDIO_RATE not defined! consider importing 'Klangstrom.h'
#endif

#ifndef KLANG_SAMPLES_PER_AUDIO_BLOCK
#error KLANG_SAMPLES_PER_AUDIO_BLOCK not defined! consider importing 'Klangstrom.h'
#endif

unsigned int SAMPLE_RATE  = KLANG_AUDIO_RATE;
float        fSAMPLE_RATE = (float)KLANG_AUDIO_RATE;
int          PERIOD       = KLANG_SAMPLES_PER_AUDIO_BLOCK;
float        fPERIOD      = (float)KLANG_SAMPLES_PER_AUDIO_BLOCK;

extern int   Pexitprogram, preset;
extern int   commandline, gui;
extern int   exitwithhelp, nojack;
extern int   maxx_len;
extern int   error_num;
extern int   reconota;
extern int   stecla;
extern int   looper_lqua;
extern int   note_active[POLY];
extern int   rnote[POLY];
extern int   gate[POLY];
extern int   pdata[50];
extern float val_sum;
extern float cSAMPLE_RATE;
extern float r__ratio[12];
extern int   Wave_res_amount;
extern int   Wave_up_q;
extern int   Wave_down_q;