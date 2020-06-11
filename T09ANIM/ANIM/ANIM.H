#ifndef __ANIM_H_
#define __ANIM_H_

#include "rnd/rnd.h"


#define AC6_MAX_UNITS 30000


typedef struct tagac6UNIT ac6UNIT;

typedef struct tagac6ANIM
{
  HWND hWnd;                     
  HDC hDC;                       
  INT W, H;                      

  ac6UNIT *Units[AC6_MAX_UNITS]; 
  INT NumOfUnits; 

  DBL
    GlobalTime, GlobalDeltaTime, /* Global time and interframe interval */
    Time, DeltaTime,/* Time with pause and interframe interval */
    FPS;/* Frames per second value */
  BOOL
    IsPause;/* Pause flag */

  BYTE Keys[256];      
  BYTE KeysOld[256];   
  BYTE KeysClick[256]; 

  INT Mx, My, Mz, Mdx, Mdy, Mdz;

} ac6ANIM;

#define AC6_UNIT_BASE_FIELDS                  \
VOID (*Init)( ac6UNIT *Uni, ac6ANIM *Ani );     \
VOID (*Close)( ac6UNIT *Uni, ac6ANIM *Ani );    \
VOID (*Response)( ac6UNIT *Uni, ac6ANIM *Ani ); \
VOID (*Render)( ac6UNIT *Uni, ac6ANIM *Ani )

struct tagac6UNIT
{
  AC6_UNIT_BASE_FIELDS;
};

extern ac6ANIM AC6_Anim;
extern INT AC6_MouseWheel;

/*Functions prototypes*/
VOID AC6_AnimInit( HWND hWnd );
VOID AC6_AnimClose( VOID );
VOID AC6_AnimResize( INT W, INT H );
VOID AC6_AnimCopyFrame( VOID );
VOID AC6_AnimRender( VOID );
VOID AC6_AnimAddUnit( ac6UNIT *Uni );

ac6UNIT * AC6_AnimUnitCreate( INT Size );

VOID AC6_AnimInputInit( VOID );
VOID AC6_AnimInputResponse( VOID );

VOID AC6_TimerInit ( VOID );
VOID AC6_TimerResponse( VOID );

#endif 

