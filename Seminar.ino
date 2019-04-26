/*
Kod pokrenuti u Arduino programskom jeziku
Parkiranje Arduino robota između dvije prepreke kretanjem unatrag i korištenjem senzora.
*/

#include <ArduinoRobot.h>
#include <Time.h> 
//#include <Wire.h>     //na verziji 1.0.5-r2 radi error
//#include <SPI.h>      //na verziji 1.0.5-r2 radi error


const int pingPin  = TKD1;   //prednji UZ
const int pingPin4 = TK4;    //lijevi IR
const int pingPin0 = TK0;    //desni IR
const int pingPin6 = TK6;    //zadnji IR

int trueOrFalse = 1; //1-true, 0-false ZA WHILE PETLJU
unsigned long time;  //za vrijeme

long duration;    //za UZ senzor
long brzinaMotora = 100;    //konstantnabrzina motora

void setup()
{
 Robot.begin();
 Serial.begin(9600);
 Robot.beginSpeaker();

 //****************** ZA LIJEVU STRANU ********************** 
 Robot.motorsStop();           // čekati 1 sekundu
 delay(1000);
 if(IcLijevi() < IcDesni()) //true za lijevu stranu
 {  
   if(IcLijevi() >10 && IcLijevi() <30)
   {
     while(trueOrFalse)  //petlja za naprijed
     {
       Robot.motorsWrite(brzinaMotora,brzinaMotora); //naprijed
       delay(100);
       
       if(IcLijevi() >= 30)
       {
         trueOrFalse = 0;    //izaći će iz petlje  WHILE FALSE
         Robot.motorsStop();           // fast stop
         delay(1000);
       }
     }
   }
     trueOrFalse = 1;     //WHILE TRUE
     Robot.motorsStop();   //fast stop
     delay(500);
     time = millis();  //počni brojati vrijeme
     while(trueOrFalse)
     {                
       Robot.motorsWrite(brzinaMotora,brzinaMotora); //idi naprijed
       delay(100);
       if(IcLijevi() >10 && IcLijevi() <30)
       {
         trueOrFalse = 0;   //WHILE FALSE
         time = millis() - time;  //izračunaj vrijeme koje je išao naprijed
         time = time / 2;         //prepolovi vrijeme da zna koliko će ići natrag
       }       
     }
     Robot.motorsStop();           // fast stop
     delay(500);
     Robot.motorsWrite(-brzinaMotora,-brzinaMotora); //idi natrag
     delay(time+200);  //koliko će ići u ms natrag + 200 zbog nesavršenosti motora i dok krene  
     Robot.motorsStop();           // fast stop
     delay(500);
        //rotiraj se pod 90° (gledajući prednji kraj s lijeva na desno) 
     Robot.motorsWrite(-100,100);
     delay(500);
     Robot.motorsStop();           // fast stop
     delay(1000);
     trueOrFalse = 1; //WHILE TRUE
     while(trueOrFalse)
     {
       Robot.motorsWrite(-80,-80);  //idi natrag
       delay(50);
       Robot.beep(BEEP_SIMPLE);    //beep za u natrag
       delay(50);
       if(IcStraznji() < 15)
       {
         trueOrFalse = 0;
         Robot.motorsStop();           // fast stop
         delay(1000);
       }
     }
 }
 //********************ZA DESNU STRANU**************************
 else if(IcLijevi() >= IcDesni()) //true za desnu stranu
 {  
   if(IcDesni() >10 && IcDesni() <30)
   {
     //idi naprijed
     while(trueOrFalse)
     {
       Robot.motorsWrite(brzinaMotora,brzinaMotora); 
       delay(100);
       
       if(IcDesni() >= 30)
       {
         trueOrFalse = 0;    //izaći će iz petlje
         Robot.motorsStop();           // fast stop
         delay(1000);
       }
     }
   }
     trueOrFalse = 1;
     Robot.motorsStop();           // fast stop
     delay(500);
     time = millis();  //počni brojati vrijeme
     while(trueOrFalse)
     {  
       Robot.motorsWrite(brzinaMotora,brzinaMotora); //idi naprijed
       delay(100);
       if(IcDesni() >10 && IcDesni() <30)
       {
         trueOrFalse = 0;
         time = millis() - time;  //izračunaj vrijeme koje je išao naprijed
         time = time / 2;         //prepolovi vrijeme da zna koliko će ići natrag
       }       
     }
     Robot.motorsStop();           // fast stop
     delay(500);
     Robot.motorsWrite(-brzinaMotora,-brzinaMotora); //idi natrag
     delay(time+200);  //koliko će ići u ms natrag
     Robot.motorsStop();           // fast stop
     delay(500);
     //rotiraj se pod 90° (gledajući prednji kraj s desna na lijevo)
     Robot.motorsWrite(100,-100);
     delay(500);   
     trueOrFalse = 1;
     Robot.motorsStop();           // fast stop
     delay(1000);
     while(trueOrFalse)
     {
       Robot.motorsWrite(-80,-80);
       delay(50);
       Robot.beep(BEEP_SIMPLE);
       delay(100);
       if(IcStraznji() < 15)
       {
         trueOrFalse = 0;
         Robot.motorsStop();           // fast stop
         delay(1000);
       }
     }
 }
 else
 {
   Robot.motorsWrite(-255,255);   //error fast rotate
 }
}
void loop()
{  
}

//-----------------------------*-FUNKCIJE-*-------------------------------- ;-)
int UzPrednji()  //vraća udaljenost UZ senzora
{
  // The PING))) is triggered by a HIGH pulse of 2 or more microseconds.
  // Give a short LOW pulse beforehand to ensure a clean HIGH pulse:
  pinMode(pingPin, OUTPUT);
  Robot.digitalWrite(pingPin, LOW);
  delayMicroseconds(2);
  Robot.digitalWrite(pingPin, HIGH);
  delayMicroseconds(5);
  Robot.digitalWrite(pingPin, LOW);
  pinMode(pingPin, INPUT);
  duration = pulseIn(pingPin, HIGH);
  // The same pin is used to read the signal from the PING))): a HIGH
  // pulse whose duration is the time (in microseconds) from the sending
  // of the ping to the reception of its echo off of an object.
  return MicrosecondsToCentimeters(duration);  //očitavanje sa digitalnog ulaza, te pretvaranje u cm
}

int IcDesni()  //udaljenost desnog IC senzora u cm
{
  return KonvertirajAnalognuVrijednostUUdaljenost(Robot.analogRead(TK0));
}

int IcLijevi()  //udaljenost lijevog IC senzora u cm
{
  return KonvertirajAnalognuVrijednostUUdaljenost(Robot.analogRead(TK4));
}

int IcStraznji()  //udaljenost straznjeg IC senzora u cm
{
  return KonvertirajAnalognuVrijednostUUdaljenost(Robot.analogRead(TK6));
}

long MicrosecondsToCentimeters(long microseconds)      //preračunavanje digitalnog signala UZ u cm
{
  return microseconds / 29 / 2;  //cm
}

int KonvertirajAnalognuVrijednostUUdaljenost(int signal)  //preračunavanje analognog signala IC u cm
{
   return (int) pow(3027.4/signal,1.2134); //cm
}
