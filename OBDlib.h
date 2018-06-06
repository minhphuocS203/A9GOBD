#ifndef OBDlib_H
#define OBDlib_H
#include "Arduino.h"
#include "A9G.h"


#define debug 1

class OBD {
public:
      int ReadTemp();
      float ReadVoltage(void);
      int ReadRPM();
      int ReadSpeed();
      int ReadIntemperature();
      int ReadPedalposition (void);
      int ReadTimingadvance (void);
      int ReadFuelinjectiontiming (void);
      int ReadEngineoiltemperature (void);
      int ReadRuntime(void);
      int ReadThrottleposition (void);
      int ReadMAF (void);
      int SetupConnect (void); // 0100
      void settupuart(void);
      void SupportBoard(void) ;// kiem tra ho tro
      int ResetOBDII (void); //atz
      float Readfuelpressure (void);
      float ReadIMAP (void) ;
      float ReadratioAF (void);
      int ReadEGR (void);
      int Readfuellevel (void) ;
      int Readambienttemp (void);
      float Readfuelrate (void);
      //int SetupBoard (void); // atsp0

      void getResponse();

//      int Temp;
      String rxDta = "";
      byte rxData[32]; 
      byte arxDta1[32];
      byte arxDta2[32];
      byte arxDta3[32];
      byte arxDta4[32];
      byte arxDta5[32];
      int m = 31;
      int a;
      int b;
      int c;
      int d;
      byte modedata[160];
};

extern OBD OBD2;
extern int Temp,vspeed,Intemp,vehicleRPM;

#endif
