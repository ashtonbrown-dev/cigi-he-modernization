#ifndef _PERFTIMER_H_
#define _PERFTIMER_H_

void InitPerfTimer(void);
__int64 GetCounterFreqSec(void);
__int64 GetCounterFreqMicsec(void);
double GetCounterPeriodSec(void);
double GetCounterPeriodMicsec(void);
void WaitForMicsec(unsigned const long duration);
void WaitUntil(const __int64 tic);
__int64 GetTickAtInterval(unsigned const long microsec);
__int64 GetTicksDuringInterval(unsigned const long microsec);
double GetActualFreq(void);

#endif  // _PERFTIMER_H_