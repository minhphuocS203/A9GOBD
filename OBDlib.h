#ifndef OBDlib_H
#define OBDlib_H
#include "Arduino.h"
#include "A9G.h"

//Baurate define
#define OBD2_BR_9600    9600  
#define OBD2_BR_14400   14400
#define OBD2_BR_28800   28800
#define OBD_BR_19200    19200
#define OBD22_BR_38400  38400
#define HC05_BR_115200  115200

// Delay define 
#define Delay_OBD2_Init                   200
#define Delay_OBD2_Init_SupportBoard      2000 
#define Delay_sending_data                10
#define Delay_getting_data                200

// Define PIDs
#define PID_SUPPORT00         "0100\r"
#define PID_COOLANT_TEMP      "0105\r"
#define PID_RPM               "010c\r"
#define PID_VEH_SPEED         "010d\r"
#define PID_INT_AIR_TEMP      "010f\r"
#define PID_MAF_AIR_FLOW      "0110\r"
#define PID_THROTTLE_POS      "0111\r"
#define PID_PEDAL_POS         "015a\r"
#define PID_TIMING_ADV        "010e\r"
#define PID_FUEL_INJ_TIME     "015d\r"
#define PID_OIL_ENG_TEMP      "015c\r"
#define PID_RUNTIME_START     "011f\r"

#define debug 0

typedef struct OBD_Buffer
{
      int DTC_count;
      String OBD_VIN_ID[18];
} OBD_Data;

class OBD {
public:
      int ReadTemp();
      float ReadVoltage();
      int ReadRPM();
      int ReadSpeed();
      int ReadIntemperature();
      int ReadMAF();
      int ReadThrottleposition();
      int ReadPedalposition();
      int ReadTimingadvance();
      int ReadFuelinjectiontiming();
      int ReadEngineoiltemperature ();
      int ReadRuntime();
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
      void Mode03_Read();
      //int SetupBoard (void); // atsp0

      int *getOBData();

      void getResponse();  

      /*Translator*/  
      void Mode03_Bit01_Trans(String inStr);
      void Read_VIN();
};

static int dataOBD[11];
static String rxDta; // Uart buffer
static byte modedata[160]; // check mode data

/*Variable hold DTC's mode 3 OBD*/
extern String DTC_temp[4]; 
extern int DTC_count;      

/*Variable hold VIN ID of Vehicle*/
extern String OBD_VIN_ID[18];


#endif
