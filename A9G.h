#ifndef A9G_H
#define A9G_H
#include "Arduino.h"
#include "OBDlib.h"


#define DEBUG 1


typedef enum {    // khai bao cac trai thai của SendTCP_State
  DoNothing,
  Idle,
  SendedTCPStart,
  SendedTCPsend,
  SendedData
} SendTCP_State;

class A9G_Module
{
  public:

    String RxData;  // du lieu GPS tra ve
    char response[100];
    unsigned long int previousMillis = 0;
    unsigned long int previousTCP = 0;
    unsigned long int previousSend = 0;
    unsigned int state; // tao bien trang thai
    String timehhUTC,timemm,timess,datedd,datemm,dateyy; // time and date
    int timehhVN;
    String altitude; // độ cao mực nước biển 
    String temp_lat, temp_long; 
    float LaDDMM, LoDDMM;
    String latitude, longitude; // toa do kinh , vi do
    char Jsonstring[200];  // Tạo một chuỗi tên là Jsonstring
 
    
    uint8_t SendData_A9G(char* ATcommand, char* ex_answer,unsigned long int timeout);
    void sendData_A9G(String command);
    void init();
    void getData(int timeGet);
    bool check_GPS_Frame();
    void tran_GPS();
    void Send_TCP_data();
    void JsonWrap();
    
  private:
  
  
};
#endif




