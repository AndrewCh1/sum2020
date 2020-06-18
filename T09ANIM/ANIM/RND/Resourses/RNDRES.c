#include <stdio.h>
#include <string.h>


#include "../rnd.h"


VOID AC6_RndResInit( VOID )
{
  AC6_RndShdInit();
  AC6_RndTexInit();
  AC6_RndMtlInit();
}
VOID AC6_RndResClose( VOID )
{
  AC6_RndShdClose();
  AC6_RndTexClose();
  AC6_RndMtlClose();
}
