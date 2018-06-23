#include "Arduino.h"
#include "A9G.h"
#include "OBDlib.h"


// đọc dữ liệu khi uart gửi về
void OBD::getResponse(){
  while( Serial2.available()>0){
    char c = Serial2.read();
    rxDta += c;
  }

#if debug
  Serial1.println(rxDta);
#endif   
}

// đọc nhiệt độ nước làm mát
int OBD::ReadTemp(){
  if (modedata[5] == 0){return -1;} // kiểm tra PID 05 có được hỗ trợ không?
  Serial2.flush();
  Serial2.write("0105\r"); // 05 là PID của đọc nhiệt độ nước
  delay(200);
  getResponse(); // gọi hàm đọc giá trị từ uart
  int Temp = strtol(&rxDta[10],0,16)-40; // chuyển đổi giá trị hex nhận được sang dec
  rxDta =""; // xóa chuỗi vừa lưu để tiếp tục đọc giá trị khác
  
  return Temp;

#if debug
  Serial1.print("THW: ");
  Serial1.println(Temp);
#endif   
}

// đọc điện áp của accu
float OBD::ReadVoltage(void){
  Serial2.flush();
  Serial2.write("atrv\r");
  delay(200);
  getResponse(); 
  float Voltage = rxDta.substring(4,'V').toFloat();
  rxDta ="";
  
  return Voltage;
}

// đọc tốc độ động cơ
int OBD::ReadRPM(){
  if(modedata[12] == 0){return -1;}
  Serial2.flush();
  Serial2.write("010c\r");
  delay(200);
  getResponse(); 
  int vehicleRPM = ((strtol(&rxDta[10],0,16)*256)+strtol(&rxDta[13],0,16))/4;
  rxDta = ""; 

  return vehicleRPM;

#if debug
  Serial1.print("speedRPM: ");
  Serial1.println(vehicleRPM);
#endif 
}

//đọc tốc độ xe
int OBD::ReadSpeed() {
  if(modedata[13] == 0){return -1;}
  Serial2.flush();
  Serial2.write("010d\r");
  delay(200);
  getResponse();
  int vspeed = strtol(&rxDta[10],0,16);
  rxDta ="";

  return vspeed;
  
#if debug
  Serial1.print("speedvehicle: ");
  Serial1.println(vspeed);
#endif   
}

//reset OBD
int OBD::ResetOBDII (void){
  byte ResetBoard = 0;
  rxDta ="";
  Serial2.flush();
  Serial2.write("atz\r");
  delay(2000);
  getResponse();
  if(rxDta.substring(3,9) == "ELM327"){ResetBoard =1;} else {ResetBoard =0;}
  rxDta ="";
  return ResetBoard;
}

// đọc nhiệt độ khí nạp
int OBD::ReadIntemperature() {
  if(modedata[15] == 0){return -1;}
  Serial2.flush();
  Serial2.write("010f\r");
  delay(200);
  getResponse();
  int Intemp =(strtol(&rxDta[10],0,16)) -40;
  rxDta ="";

  return Intemp;
  
#if debug
  Serial1.print("intaketemp: ");
  Serial1.println(Intemp);
#endif   
}

// đọc khối lượng khí nạp
int OBD::ReadMAF(){
  if(modedata[18] == 0){return -1;}
  Serial2.flush();
  Serial2.write("0110\r");
  delay(200);
  getResponse(); 
  int MAF=((strtol(&rxDta[10],0,16)*256)+strtol(&rxDta[13],0,16))/100;
  rxDta = ""; 

  return MAF;
  
#if debug
  Serial1.print("MAF: ");
  Serial1.println(MAF);
#endif   
}

// đọc vị trí bướm ga
int OBD::ReadThrottleposition() {
//  if(modedata[17] == 0){return -1;}
//  
//  Serial2.flush();
  Serial2.write("0111\r");
  delay(200);
  getResponse();
  int Thro_position = ((strtol(&rxDta[10],0,16))*100)/255;
  rxDta ="";

  return Thro_position;

#if debug
  Serial1.print("VTBuomga: ");
  Serial1.println(Thro_position);
#endif   
}

// đọc vị trí bàn đạp ga
int OBD::ReadPedalposition() {
  int Pposition = 0;
  if(modedata[90] == 0){return -1;}
  Serial2.flush();
  Serial2.write("015a\r");
  delay(200);
  getResponse();
  Pposition = (strtol(&rxDta[10],0,16))*100/255;
  rxDta ="";
  return Pposition;
}

//đọc góc đánh lửa sớm
int OBD::ReadTimingadvance (void) {
  int a = 0;
   if(modedata[14] == 0){return -1;}
  Serial2.flush();
  Serial2.write("010e\r");
  delay(200);
  getResponse();
  a = ((strtol(&rxDta[10],0,16))/2) - 64;
  rxDta ="";
  return a;
}

// đọc thời gian phun nhiên liệu
int OBD::ReadFuelinjectiontiming (void){
  int b = 0;
   if(modedata[93] == 0){return -1;}
  Serial2.flush();
  Serial2.write("015d\r");
  delay(200);
  getResponse(); 
  b= (((strtol(&rxDta[10],0,16)*256)+strtol(&rxDta[13],0,16))/128)-210;
  rxDta = ""; 
 return b;
}

// đọc nhiệt độ dầu động cơ
int OBD::ReadEngineoiltemperature () {
//  if(modedata[92] == 0){return -1;}
//  Serial2.flush();
  Serial2.write("015c\r");
  delay(200);
  getResponse();
  int c = strtol(&rxDta[10],0,16)-40;
  rxDta ="";
  return c;
}

//đọc thời gian chạy của động cơ từ khi bắt đầu khởi động
int OBD::ReadRuntime(void){
  int d = 0;
 if(modedata[31] == 0){return -1;}
  Serial2.flush();
  Serial2.write("011f\r");
  delay(200);
  getResponse(); 
 d= (strtol(&rxDta[10],0,16)*256)+strtol(&rxDta[13],0,16);
  rxDta = ""; 
 return d;
}

//kiểm tra kết nối với board
int OBD::SetupConnect (void){
  int g = 0;
  Serial2.flush();
  Serial2.write("0100\r"); 
  delay(2000);
  getResponse();
  if( rxDta.substring(0,9) == "010041 00"){g =1;} else {g =0;}
  rxDta ="";
  return g;
}

//kiểm tra những PID nào được hỗ trợ
void OBD::SupportBoard(){

  int m,a,b,c,d; 
  byte rxData[32],arxDta1[32],arxDta2[32],arxDta3[32],arxDta4[32],arxDta5[32];
  
 // mang 0100
  rxDta = ""; 
  Serial2.flush();
  Serial2.write("0100\r");
  delay(1000);
  getResponse();
  
 if(( rxDta.substring(4,6) == "NO") || (rxDta.substring(4,6) == "SE")) {
  for ( int i = 0; i < 32; i++) {
  arxDta1[i] = 0;
  }
  }else{

 a = strtol(&rxDta[10],0,16);
 b = strtol(&rxDta[13],0,16); 
 c = strtol(&rxDta[16],0,16); 
 d = strtol(&rxDta[19],0,16);  
   rxDta = "";

 //chuyen a
 if (d > 0) {
 for(int i = 0; d>0;i++){
 rxData [i]= d%2;
 d=d/2; 
 }
 }
 
  //chuyen b
 if (c > 0) {
 for(int i = 8; c>0;i++){
 rxData [i]= c%2;
 c=c/2;  
 }
 }
 
  //chuyen c
 if (b > 0) {
 for(int i = 16; b>0;i++){
 rxData [i]= b%2;
 b=b/2;  
 }
 }
  //chuyen d
 if (a > 0) {
 for(int i = 24; a>0;i++){
 rxData [i]= a%2;
 a=a/2;  
 }
 }
 
 m =31;
  for ( int i = 0; i < 32; i++) {
  arxDta1[m] = rxData[i];
  m = m -1;}
  }
  
// mang 0120
  Serial2.flush();
  Serial2.write("0120\r");
  delay(1000);
  getResponse();
 if(( rxDta.substring(4,6) == "NO") ||( rxDta.substring(4,6) == "SE")) {
    for ( int i = 0; i < 32; i++) {
    arxDta2[i] = 0;}
  } else{ 
 a = strtol(&rxDta[10],0,16);
 b = strtol(&rxDta[12],0,16); 
 c = strtol(&rxDta[15],0,16); 
 d = strtol(&rxDta[18],0,16);
   rxDta = "";
 //chuyen a
 if (d > 0) {
 for(int i = 0; d>0;i++){
 rxData [i]= d%2;
 d=d/2; 
 }
 }
 
  //chuyen b
 if (c > 0) {
 for(int i = 8; c>0;i++){
 rxData [i]= c%2;
 c=c/2;  
 }
 }
 
  //chuyen c
 if (b > 0) {
 for(int i = 16; b>0;i++){
 rxData [i]= b%2;
 b=b/2;  
 }
 }
 
  //chuyen d
 if (a > 0) {
 for(int i = 24; a>0;i++){
 rxData [i]= a%2;
 a=a/2;  
 }
 }
 
 m =31;
  for ( int i = 0; i < 32; i++) {
  arxDta2[m] = rxData[i];
  m = m -1;}
  } 


  // mang 0140

  Serial2.flush();
  Serial2.write("0140\r");
  delay(1000);
  getResponse();
 if(( rxDta.substring(4,6) == "NO") || (rxDta.substring(4,6) == "SE")) {
     for ( int i = 0; i < 32; i++) {
     arxDta3[i] = 0;}
  }else{ 
 a = strtol(&rxDta[10],0,16);
 b = strtol(&rxDta[12],0,16); 
 c = strtol(&rxDta[15],0,16); 
 d = strtol(&rxDta[18],0,16);
   rxDta = "";
 
 //chuyen a
 if (d > 0) {
 for(int i = 0; d>0;i++){
 rxData [i]= d%2;
 d=d/2; 
 }
 }
 
  //chuyen b
 if (c > 0) {
 for(int i = 8; c>0;i++){
 rxData [i]= c%2;
 c=c/2;  
 }
 }
 
  //chuyen c
 if (b > 0) {
 for(int i = 16; b>0;i++){
 rxData [i]= b%2;
 b=b/2;  
 }
 }
 
  //chuyen d
 if (a > 0) {
 for(int i = 24; a>0;i++){
 rxData [i]= a%2;
 a=a/2;  
 }
 }
 
 m =31;
  for ( int i = 0; i < 32; i++) {
  arxDta3[m] = rxData[i];
  m = m -1;}
 
} 

  // mang 0160

  Serial2.flush();
  Serial2.write("0160\r");
  delay(1000);
  getResponse();
 if(( rxDta.substring(4,6) == "NO") || (rxDta.substring(4,6) == "SE")) {
   for ( int i = 0; i < 32; i++) {
   arxDta4[i] = 0;}
   }else{
 a = strtol(&rxDta[10],0,16);
 b = strtol(&rxDta[12],0,16); 
 c = strtol(&rxDta[15],0,16); 
 d = strtol(&rxDta[18],0,16);
   rxDta = "";
 //chuyen a
 if (d > 0) {
 for(int i = 0; d>0;i++){
 rxData [i]= d%2;
 d=d/2; 
 }
 }
 
  //chuyen b
 if (c > 0) {
 for(int i = 8; c>0;i++){
 rxData [i]= c%2;
 c=c/2;  
 }
 }
 
  //chuyen c
 if (b > 0) {
 for(int i = 16; b>0;i++){
 rxData [i]= b%2;
 b=b/2;  
 }
 }
 
  //chuyen d
 if (a > 0) {
 for(int i = 24; a>0;i++){
 rxData [i]= a%2;
 a=a/2;  
 }
 }
 
 m =31;
  for ( int i = 0; i < 32; i++) {
  arxDta4[m] = rxData[i];
  m = m -1;}
 } 
 
  // mang 0180

  Serial2.flush();
  Serial2.write("0180\r");
  delay(1000);
  getResponse();
 if(( rxDta.substring(4,6) == "NO") || (rxDta.substring(4,6) == "SE")) {
  for ( int i = 0; i < 32; i++) {
  arxDta5[i] = 0;}
  } else {  
 a = strtol(&rxDta[10],0,16);
 b = strtol(&rxDta[12],0,16); 
 c = strtol(&rxDta[15],0,16); 
 d = strtol(&rxDta[18],0,16);
   rxDta = "";
 
 //chuyen a
 if (d > 0) {
 for(int i = 0; d>0;i++){
 rxData [i]= d%2;
 d=d/2; 
 }
 }

  //chuyen b
 if (c > 0) {
 for(int i = 8; c>0;i++){
 rxData [i]= c%2;
 c=c/2;  
 }
 }
 
  //chuyen c
 if (b > 0) {
 for(int i = 16; b>0;i++){
 rxData [i]= b%2;
 b=b/2;  
 }
 }
 
  //chuyen d
 if (a > 0) {
 for(int i = 24; a>0;i++){
 rxData [i]= a%2;
 a=a/2;  
 }
 }
 
 m =31;
  for ( int i = 0; i < 32; i++) {
  arxDta5[m] = rxData[i];
  m = m -1;}
}

  for ( int i = 0; i<= 31; i++){
    modedata[i] = arxDta1[i];
    }
  for ( int i = 32; i<= 63; i++){
    modedata[i] = arxDta2[i-32];
    }
  for ( int i = 64; i<= 95; i++){
    modedata[64] = arxDta3[i-64];
    }
  for ( int i = 96; i<= 127; i++){
    modedata[96] = arxDta4[i-96];
    }
  for ( int i = 128; i<= 159; i++){
    modedata[i] = arxDta5[i-128];
    }
}

int *OBD::getOBData(){
  int *pOBD = dataOBD;
  *pOBD = 4;
  *(pOBD + 1) = ReadRPM();
  *(pOBD + 2) = ReadIntemperature();
  *(pOBD + 3) = ReadTemp();
  *(pOBD + 4) = ReadThrottleposition();
  *(pOBD + 5) = ReadMAF();
  *(pOBD + 6) = ReadEngineoiltemperature();
  return pOBD;
}

