#include <SoftwareSerial.h>
#include <ESP8266WiFi.h>
#include <FirebaseArduino.h>

#define FIREBASE_HOST "iot-deneme3-default-rtdb.firebaseio.com"
#define FIREBASE_AUTH "HOy1mp9lpam2Oyq8R7OXyp9WlM600mZ7gK6KHT6Y"
#define WIFI_SSID "a.frkn"
#define WIFI_PASSWORD "sezkente19"


SoftwareSerial Nodemcu_serial (D1,D2);


char c;
String dataIn;

void setup() {
  
  Serial.begin(57600); //uno - pc 
  
  Nodemcu_serial.begin(9600); // uno - nodemcu
  
  delay(10);
  Serial.println();
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print(WIFI_SSID);
  Serial.println(" Kablosuz Agina Baglaniyor");
  
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println();
  Serial.print(WIFI_SSID);
  Serial.println(" Kablosuz Agina Baglandi");
  Serial.print("IP adresi: ");
  Serial.println(WiFi.localIP());

  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);
  
}

String key;

void loop() {
  key = Firebase.getString("users/1/newKey");
  if(Firebase.failed()){
    Serial.print("setting /user failed:");
    Serial.println(Firebase.error());
  } else {
    Serial.println(key);
  }
  delay(1000);  
  
  while(Nodemcu_serial.available()>0)
  {
      c = Nodemcu_serial.read();
      if(c == '\n') {break;}
      else          {dataIn += c;}
      
  }
  if(c == '\n')
  {
    
    if(dataIn == key)
    {
      delay(500);
      Nodemcu_serial.print("true\n");
     }
    else
    {
      delay(500);
      Nodemcu_serial.print("false\n");
      
     }
    Serial.println(dataIn);
    c = 0;
    dataIn = "";
  }
  
}
