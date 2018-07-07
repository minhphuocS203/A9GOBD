#include "Arduino.h"
#include "A9G.h"
#include "OBDlib.h"
#include "ArduinoJson.h"

/****************************************************
  Author      : Do Hieu
  Description : Goi command qua A9G tu Arduino
*****************************************************/
void A9G_Module::sendData_A9G(String command) {
  String dulieu = "";
  Serial.println(command); // in ra lenh ra mang hinh
  
#if DEBUG
  while ( Serial.available()) 
  {
    char ch = Serial.read(); // doc tung kí tự của Serial rui gắn vào bien ch
    dulieu += ch;
  }
  Serial1.println(dulieu); // in du lieu phan hoi cua module len man hinh
#endif

}

/********************************************************************
 * Fucntion   : init
 * Author     : Do Hieu
 * Date       : 19/5/2018
 * Description: A9G initialization incldued: 
 ********************************************************************/
void A9G_Module::init(){
  #if DEBUG
    Serial1.println("Initializing A9G module....."); 
  #endif
  sendData_A9G("AT");           delay(1000);
  sendData_A9G("AT+GPS=0");     delay(2000);
  sendData_A9G("AT+CREG=1");    delay(3000);
  sendData_A9G("AT+CGATT=1");   delay(3000);
  sendData_A9G("AT+CGDCONT=1,\"IP\",\"V-INTERNET\""); delay(3000);
  sendData_A9G("AT+CGACT=1,1");   delay(3000);
  sendData_A9G("AT+GPS?");      delay(2000);
  sendData_A9G("AT+GPS=1");     delay(1000);
  sendData_A9G("AT+GPSRD=5");  delay(1000);  // Lay sau 10s
}

/********************************************************************
 * Fucntion   : init
 * Author     : Do Hieu
 * Date       : 19/5/2018
 * Description: get data from A9G via UART.
                @timeGet :  time for waiting transfed data
 ********************************************************************/
void A9G_Module::getData(int timeGet) {  
  char ch;
  while (Serial.available()){
    ch = Serial.read();
    RxData += ch;
  }
  if (millis() - previousMillis > timeGet) {
    previousMillis = millis(); 

    if(check_GPS_Frame()){
      tran_GPS();    // convert GPS data include latitude and longitude 
      
      #if DEBUG
        Serial1.print(RxData);
        
        Serial1.print("latitude :");
        Serial1.println(latitude); 
        Serial1.print("longitude :");
        Serial1.println(longitude); 
      #endif
      }
 
    if (state == DoNothing){ // ???
      state = Idle;
    }
    RxData = ""; // Xoa du lieu
  };
}


/********************************************************************
 * Fucntion   : check_GPS_Frame
 * Author     : Do Hieu
 * Date       : 19/5/2018
 * Description: 
 ********************************************************************/
bool A9G_Module::check_GPS_Frame()
{
  int i=0;
  while (i < RxData.length())
  {
    if ((RxData.substring(i, i + 5) == "+GPSR")) 
    {
        temp_lat    = RxData.substring(25+i, 34+i); 
        temp_long   = RxData.substring(37+i, 47+i);

        if ((temp_lat.toInt()>= 800) && (temp_lat.toInt()<= 2400) && (temp_long.toInt()> 10200) && (temp_long.toInt()< 11000)){ // gioi han toa do trong VN
          return 1; 
        }
        else{
          return 0; 
        } 
    }
    i++;
  }
  return 0; 
}


/********************************************************************
 * Fucntion   : Tran_GPS
 * Author     : Minh Phuoc
 * Date       : 19/5/2018
 * Description: Convert GPS data include latitude and longitude
 ********************************************************************/
void A9G_Module::tran_GPS() {
    String LaDD, LaMM; // Kinh tuyen
    String LoDDD, LoMM; // Vi tuyen
    float XMM; // Kinh tuyen
    float YMM; // Vi tuyen
//**Doi kinh do latitude, format latitude DDMM.MMMM**//
    LaDD = temp_lat.substring(0, 2); // tach DD lay độ
    LaMM = temp_lat.substring(2); // tach MM truoc dau cham
    XMM = LaMM.toFloat() / (60); // doi sang phut (6000000.0 module a7)
    LaDDMM = LaDD.toInt() + XMM; // cong thuc tinh GPS
    latitude = String(LaDDMM, 6); // lay 6 so sau dau phay
//** Doi vi do longitude, format longitude DDDMM.MMMM **//
    LoDDD = temp_long.substring(0, 3); // tach DD tu chuoi ra
    LoMM = temp_long.substring(3); // tach MM truoc dau cham
    YMM = LoMM.toFloat() / (60); // doi sang phut (6000000.0 module a7) 
    LoDDMM = LoDDD.toInt() + YMM; // cong thuc tinh GPS
    longitude = String(LoDDMM, 6); // hien 6 so sau dau phay 
}


/********************************************************************
 * Fucntion   : Send_TCP_data
 * Author     : Minh Phuoc
 * Date       : 19/5/2018
 * Description: Send package data upto the website
 ********************************************************************/
void A9G_Module::Send_TCP_data()
{
 if (millis() - previousTCP > 2000) {
    previousTCP = millis();
    switch(state)
    {
      case Idle :
        sendData_A9G("AT+CIPSTART=\"TCP\",\"159.65.4.55\",1334");
        state = SendedTCPStart;
        break;

      case SendedTCPStart :
        sendData_A9G("AT+CIPSEND");
        state = SendedTCPsend;
        break;

      case SendedTCPsend :
        if ((LaDDMM!=0) && (LoDDMM!=0)){  // bo qua lan lay dư liệu đầu tiên   
        sendData_A9G(Jsonstring);
        }
        state = SendedData;
      break;

      case SendedData :
        Serial.write(0x1A);
        state = DoNothing;
      break;
    }
  } 
}


/********************************************************************
* Fucntion   : JsonWrap
* Author     : Do Hieu
* Date       : 19/5/2018
* Description: Packet data format
********************************************************************/
 void A9G_Module::JsonWrap(int *dataOBD) {                     // đóng gói dữ liệu lại theo chuẩn
    memset(Jsonstring,'\0',200);
    StaticJsonBuffer<400> jsonBuffer;
    JsonObject& root = jsonBuffer.createObject();
    JsonArray& data = root.createNestedArray("GPS");

    data.add(latitude);  // thêm 2 tọa độ vào Data
    data.add(longitude);
    
    int *pOBD = dataOBD; 
    root["a1"] = *(pOBD + 4);       // Throttleposition       // các du lieu dc truyen vao
    root["a2"] = *(pOBD + 2);       // Intemperature
    root["a3"] = *(pOBD + 3);       // Temp
    root["io12"] = *(pOBD + 5);     
    root["io13"] = *(pOBD + 6);
    root["io14"] = digitalRead(14);
    root["io15"] = digitalRead(15);
    root["io16"] = digitalRead(16);
    root["d1"] = *(pOBD + 1);      // RPM
    root["d2"] = random(100);       
    root["d3"] = random(100);
    root["d4"] = random(100);
    root["d5"] = random(100);

    root.printTo(Jsonstring); // lưu chuối Json vừa tạo vào chuỗi Jsonstring 
}





