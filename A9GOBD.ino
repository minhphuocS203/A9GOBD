#include "A9G.h"
#include "OBDlib.h"

A9G_Module A9G;
OBD OBD2;

String rxDta;
byte rxData[32],arxDta1[32],arxDta2[32],arxDta3[32],arxDta4[32],arxDta5[32],modedata[160];
int m,a,b,c,d;
 
void setup() {
  
  Serial.begin (115200); // khoi tao cong serial giao tiep voi module
  Serial1.begin(9600);  // khoi tao cong serial giao tiep voi may tinh
  Serial2.begin(115200);  // khoi tao cong serial giao tiep voi ECU
  SendTCP_State state  = DoNothing;  // gan bien trang thai là DoNothing
  A9G.init();
  OBD2.SupportBoard(); // kiem tra PID có ho tro hay khong?
  Serial1.println("Setup done!"); 
}
void loop() {
  A9G.getData(10000);
  A9G.Send_TCP_data();
}







