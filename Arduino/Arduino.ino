#include <Servo.h>
#include <LiquidCrystal_I2C.h>
#include <Wire.h>
#include <Keypad.h>
#include <SoftwareSerial.h>
SoftwareSerial Uno_serial (10,11);
char c;
String dataIn;
int Position = 0;
bool door = true;
Servo servo;
LiquidCrystal_I2C lcd(0x27 , 16 , 2);


const byte ROWS = 4; //four rows
const byte COLS = 4; //three columns
char keys[ROWS][COLS] = {
  {'1','2','3' , 'A'},
  {'4','5','6', 'B'},
  {'7','8','9', 'C'},
  {'*','0','#', 'D'}
};
byte rowPins[ROWS] = {2,3,4,5}; //connect to the row pinouts of the keypad
byte colPins[COLS] = {6,7,8,9}; //connect to the column pinouts of the keypad
String sifre= ""; 
String girilen_sifre ="";
int basamak = 0;
Keypad keypad = Keypad( makeKeymap(keys), rowPins, colPins, ROWS, COLS );

void setup(){
  Serial.begin(57600); //uno - pc 
  Uno_serial.begin(9600); // uno - nodemcu
  lcd.init();
  lcd.backlight();
  lcd.setCursor(0,0);
lcd.print("Sifre giriniz"); 
  
  
  servo.attach(12);
  ServoClose();
  
}

void loop(){


if(door == 0)//
{
    lcd.setCursor(0,1);
    lcd.print("Kapatmak icin *");
    delay(1500);
    for(int i = 0 ; i<16 ; i++)
        {
          lcd.setCursor(i,1);
          lcd.print(" ");
          
          
        }
        
    char tus_door = keypad.getKey();

    if (tus_door == '*') // when the '' key is pressed
    {
      lcd.setCursor(0,1);
      lcd.print("Kapaniyor..");
      delay(1000);

      for(int i = 0 ; i<16 ; i++)
        {
          lcd.setCursor(i,1);
          lcd.print(" ");
          
          
        }
      ServoClose(); // closes the servo motor
      door = 1;
      delay(1500); // waits 3 seconds
  
    }
}
  
  
  char tus = keypad.getKey();

  if(tus != NO_KEY)
  {
    lcd.setCursor(basamak , 1);
    lcd.print(tus);
    girilen_sifre = girilen_sifre + tus  ;
    basamak++;
      
  }

  if(tus == '#')
  {
         //Serial.print(girilen_sifre.substring(0 , basamak-1)+'\n');

   Uno_serial.print(girilen_sifre.substring(0 , basamak-1)+'\n');
   delay(3000);
  if(Uno_serial.available()==0)
  {Serial.println("data gelmedi");}
  while(Uno_serial.available()>0)
  {
       

    c = Uno_serial.read();
    if(c == '\n') {break;}
    else          {dataIn+= c;}
      
  }
  if(c == '\n')
  {
    Serial.println(dataIn);
    sifre = dataIn;
    c = 0;
    dataIn="";
    
  }
  
   
    
      if(sifre == "true")
      {
        lcd.setCursor(0,1);
        lcd.print("Dogru sifre !");
        ServoOpen();
        door = 0; // open door
        delay(1000);
        for(int i = 0 ; i<16 ; i++)
        {
          lcd.setCursor(i,1);
          lcd.print(" ");
          
          
        }
        girilen_sifre = "";
        //sifre="";
        basamak = 0;
        
          
      }

      else if(sifre == "false")
      {
         
        lcd.setCursor(0,1);
        lcd.print("Yanlis sifre !");
        delay(1000);
        for(int i = 0 ; i<16 ; i++)
        {
          lcd.setCursor(i,1);
          lcd.print(" ");
          
          
        }
        door = 1; //door close
        girilen_sifre = "";
        basamak = 0;
        
          
      }  


       
  }
}

void ServoOpen() // opens the servo
{
  for (Position = 180; Position >= 0; Position -= 5) { // moves from 0 to 180 degrees
    servo.write(Position); // moves to the postion
    delay(15); // waits 15 milliseconds
  }
}

void ServoClose() // closes the servo
{
  for (Position = 0; Position <= 180; Position += 5) { // moves from position 0 to 180 degrees
    servo.write(Position); // moves to the position
    delay(15); // waits 15 milliseconds
  }
}
