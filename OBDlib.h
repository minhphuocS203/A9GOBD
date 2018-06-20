#ifndef OBDlib_H
#define OBDlib_H
#include "Arduino.h"
#include "A9G.h"


#define debug 0

class OBD {
public:
      int ReadTemp();
      float ReadVoltage(void);
      int ReadRPM();
      int ReadSpeed();
      int ReadIntemperature();
      int ReadMAF();
      int ReadThrottleposition();
      int ReadPedalposition(void);
      int ReadTimingadvance (void);
      int ReadFuelinjectiontiming (void);
      int ReadEngineoiltemperature (void);
      int ReadRuntime(void);
      int SetupConnect (void); // 0100
      void settupuart(void);
      void SupportBoard() ;// kiem tra ho tro
      int ResetOBDII (void); //atz
      float Readfuelpressure (void);
      float ReadIMAP (void) ;
      float ReadratioAF (void);
      int ReadEGR (void);
      int Readfuellevel (void) ;
      int Readambienttemp (void);
      float Readfuelrate (void);
      //int SetupBoard (void); // atsp0

      int *getOBData();

      void getResponse();
      
};
static int dataOBD[5];
static String rxDta; // Uart buffer
static byte modedata[160]; // check mode data


#endif
