#include<SoftwareSerial.h>

#define rxpin 3
#define txpin 4
SoftwareSerial gsm (rxpin,txpin);

const int button1 = 5;
const int button2 = 6;
const int button3 = 7;
const int button4 = A0;

int state1 = 0;
int state2 = 0;
int state3 = 0;
int state4 = 0;

void setup(){
  Serial.begin(9600);
  gsm.begin(9600);

  pinMode(button1,INPUT);
  pinMode(button2,INPUT);
  pinMode(button3,INPUT);
  pinMode(button4,INPUT);

  Serial.println("Setting up");
  delay(1000);
  gsm.println("AT");
}
void loop(){
  state1 = digitalRead(button1);
  state2 = digitalRead(button2);
  state3 = digitalRead(button3);
  state4 = digitalRead(button4);

  if (state1 == HIGH){
    sendmessage1();
    Serial.println("Button1 pressed");
  }
  else if (state2 == HIGH){
    sendmessage2();
    Serial.println("Button 2 pressed");
  }
  else if (state3 == HIGH){
    sendmessage3();
    Serial.println("button 3 pressed");
  }
  else if (state4 == HIGH){
    makecall();
    Serial.println("button 4 pressed");
  }
}
void updateSerial(){
  delay(500);
  while (Serial.available()) 
  {
    gsm.write(Serial.read());//Forward what Serial received to Software Serial Port
  }
  while(gsm.available()) 
  {
    Serial.write(gsm.read());//Forward what Software Serial received to Serial Port
  }
}
void sendmessage1(){
  updateSerial();
  gsm.println("AT+CMGF=1");
  updateSerial();
  gsm.println("AT+CMGS=\"+8801556360443\"");
  updateSerial();
  gsm.print("I need water!");
  updateSerial();
  gsm.write(26);
}
void sendmessage2(){
  updateSerial();
  gsm.println("AT+CMGF=1");
  updateSerial();
  gsm.println("AT+CMGS=\"+8801556360443\"");
  updateSerial();
  gsm.print("I need food!");
  updateSerial();
  gsm.write(26);
}
void sendmessage3(){
  updateSerial();
  gsm.println("AT+CMGF=1");
  updateSerial();
  gsm.println("AT+CMGS=\"+8801556360443\"");
  updateSerial();
  gsm.print("I need to use the washroom!");
  updateSerial();
  gsm.write(26);
}
void makecall(){
  updateSerial();
  gsm.println("ATD+ +8801556360443;"); //  change ZZ with country code and xxxxxxxxxxx with phone number to dial
  updateSerial();
  delay(10000); // wait for 20 seconds...
  gsm.println("ATH"); //hang up
  updateSerial();
}