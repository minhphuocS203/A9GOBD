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

      int *getOBData(int num);

      void getResponse();
      
};

extern OBD OBD2;
static int Temp,vspeed,Intemp,vehicleRPM,MAF,Thro_position;
extern String rxDta;
extern byte rxData[32],arxDta1[32],arxDta2[32],arxDta3[32],arxDta4[32],arxDta5[32],modedata[160];
extern int m,a,b,c,d;

#endif
