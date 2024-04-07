#include <LiquidCrystal.h>
#include<Servo.h>
Servo mot1,mot2, mot3;
const int rs = 8, en = 9, d4 = 10, d5 =11, d6 = 12, d7 = 13;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);
#include <Wire.h>
#include <DFRobot_DHT11.h>
DFRobot_DHT11 DHT;
#define DHT11_PIN 7
#include <Adafruit_INA219.h>
Adafruit_INA219 ina219;



int ld=5;
int rly1=6;

int ldr1=A1;
int ldr2=A0;
int ldr3=A2;
int ldr4=A3;
int bs=1;
int b1_low=0,b2_low=0;

int m1min=70;
int m1max=120;

int m2min=70;
int m2max=120;


int m3min=0;
int m3max=180;


int pos1=m1min;
int pos2=m2min;
int pos3=m3min;
int l1val,l2val,l3val,l4val,lavg,rt;
int b_rating=2000;//mah
long int prv=millis();
void setup() {
  
 Serial.begin(9600);
 delay(200);
 lcd.begin(16,2);
 lcd.print("  WELCOME");
 pinMode(rly1,OUTPUT);
 

  pinMode(ld,OUTPUT);

  mot1.attach(2);
   delay(200);
  mot2.attach(3);
  delay(200);
  mot3.attach(4);

  mot1.write(pos1);
  delay(200);
  mot2.write(pos2);
  delay(200);
    mot3.write(pos3);
  delay(200);
 digitalWrite(rly1,0);
 
  digitalWrite(ld,0);
 
ina219.begin();
 
}

void loop() {


  DHT.read(DHT11_PIN);
  int temp=DHT.temperature;
  int hmd=DHT.humidity;

 
  float busvoltage = 0;
  int current_mA = 0;
 
  busvoltage = ina219.getBusVoltage_V();
  current_mA = int(ina219.getCurrent_mA());
  if(current_mA<0)
  {
    current_mA=0;
  }
  rt= b_rating/(current_mA+1);
  lcd.clear();
  lcd.print("V:"+String(busvoltage,1) + " I:"+String(current_mA) + " T:"+String(rt));


  

  if((busvoltage<10 && digitalRead(rly1)==0) && bs==1)
  {
     b1_low=b1_low+1;

  }

  if(b1_low>2)
  {
    digitalWrite(rly1,1);
  }

  
 if((busvoltage<10 && digitalRead(rly1)==1) && bs==1)
  {
     b2_low=b2_low+1;

  }


if(b2_low>2)
{
  bs=0;
}

if((millis()-prv>20000) && bs==0)
{
lavg=(l1val+l2val+l3val+l4val)/4;
Serial.print("369849,1MQFVO5XSKAUISAW,0,0,SRC 24G,src@internet,"+String(temp) + ","+ String(hmd) + ","+ String(busvoltage)+ "," +String(current_mA) + "," +String(lavg) + "," +String(rt));  
  prv=millis();
  for(int ii=m3min;ii<=m3max;ii++)
  {
    mot3.write(ii);
    delay(15);
  }
  delay(300);

  for(int ii=m3max;ii>=m3min;ii--)
  {
    mot3.write(ii);
    delay(15);
  }
  delay(300);

  
   digitalWrite(rly1,0);
  bs=1;
}

if(bs==1)
{
if(digitalRead(rly1)==0)
{
  lcd.setCursor(0,1);
lcd.print("B1 T:"+String(temp) + " H:"+String(hmd));
}

else
{
    lcd.setCursor(0,1);
lcd.print("B2 T:"+String(temp) + " H:"+String(hmd));
}
}
else
{
   lcd.setCursor(0,1);
lcd.print("LB T:"+String(temp) + " H:"+String(hmd));
}

l1val=analogRead(ldr1);
l2val=analogRead(ldr2);
l3val=analogRead(ldr3);
l4val=analogRead(ldr4);




 if(l1val>600)
 {
  if(pos1==m1min)
  {
    for(int ii=pos1;ii<=m1max;ii++)
    {
      mot1.write(ii);
      delay(15);
    }
    pos1=m1max;
  }
 }


else if(l3val>600)
 {
  if(pos1==m1max)
  {
    for(int ii=pos1;ii>=m1min;ii--)
    {
      mot1.write(ii);
      delay(15);
    }
    pos1=m1min;
  }
 }


if(l2val>600)
 {
  if(pos2==m2min)
  {
    for(int ii=pos2;ii<=m2max;ii++)
    {
      mot2.write(ii);
      delay(15);
    }
    pos2=m2max;
  }
 }


else if(l4val>600)
 {
  if(pos2==m2max)
  {
    for(int ii=pos2;ii>=m2min;ii--)
    {
      mot2.write(ii);
      delay(15);
    }
    pos2=m2min;
  }
 }

 if(Serial.available())
 {
  int x=Serial.read();
  if(x=='1')
  {
    digitalWrite(ld,0);
  }

   if(x=='2')
  {
    digitalWrite(ld,1);
  }

 }
 delay(1000);
}