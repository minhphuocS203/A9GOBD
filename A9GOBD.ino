#include "A9G.h"
#include "OBDlib.h"

A9G_Module A9G;
OBD OBD2;
int Temp = 0;
int vspeed,Intemp,vehicleRPM;

void setup() {
  
  Serial.begin (115200); // khoi tao cong serial giao tiep voi module
  Serial1.begin(9600);  // khoi tao cong serial giao tiep voi may tinh
  Serial2.begin(115200);  // khoi tao cong serial giao tiep voi ECU
  SendTCP_State state  = DoNothing;  // gan bien trang thai là DoNothing
  A9G.init();
  Serial1.println("Setup adone!"); 

}
void loop() {
  A9G.getData(10000);
  A9G.Send_TCP_data();


}







