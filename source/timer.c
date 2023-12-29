#ifdef __linux__
#include <unistd.h>
#include <sys/times.h>
#else
#include <time.h>
#endif
#include "openglheader.h"
#include "utilities.h"

double         app_time, tic_time, toc_time;

static clock_t tick0, tick1, tick2;
#ifdef __linux__
static double  ticks_per_sec = 100.0;
#endif

void TimerInit ( void )
{
#ifdef __linux__
  struct tms clk;

  ticks_per_sec = (double)sysconf ( _SC_CLK_TCK );
  tick0 = tick1 = tick2 = times ( &clk );
#else
  tick0 = tick1 = tick2 = clock ();
#endif
  app_time = toc_time = 0.0;
} /*TimerInit*/

double TimerTic ( void )
{
#ifdef __linux__
  struct tms clk;

  tick1 = tick2 = times ( &clk );
  toc_time = 0.0;
  return app_time = tic_time = (double)(tick2-tick0)/ticks_per_sec;
#else
  tick1 = tick2 = clock ();
  toc_time = 0.0;
  return app_time = tic_time = (double)(tick2-tick0)/(double)CLOCKS_PER_SEC;
#endif
} /*TimerTic*/

double TimerToc ( void )
{
#ifdef __linux__
  struct tms clk;

  tick2 = times ( &clk );
  app_time = (double)(tick2-tick0)/ticks_per_sec;
  return toc_time = (double)(tick2-tick1)/ticks_per_sec;
#else
  tick2 = clock ();
  app_time = (double)(tick2-tick0)/(double)CLOCKS_PER_SEC;
  return toc_time = (double)(tick2-tick1)/(double)CLOCKS_PER_SEC;
#endif
} /*TimerToc*/

double TimerTocTic ( void )
{
  clock_t tick;

#ifdef __linux__
  struct tms clk;

  tick = tick1;  tick1 = tick2 = times ( &clk );
  app_time = (double)(tick2-tick0)/ticks_per_sec;
  return toc_time = (double)(tick2-tick)/ticks_per_sec;
#else
  tick = tick1;  tick1 = tick2 = clock ();
  app_time = (double)(tick2-tick0)/(double)CLOCKS_PER_SEC;
  return toc_time = (double)(tick2-tick)/(double)CLOCKS_PER_SEC;
#endif
} /*TimerTocTic*/

