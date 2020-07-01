/*************************************************************

Ini adalah koding asal untuk projek: 
1. buka tutup plug kedai ana
2. buka tutup kipas
3. stream data ke favoriot 
4. stream data ke ThinkSpeak 
5. BLYNK

Diubahsuai oleh: 
9W2KEY OJ15dx
30 Jun, 2020
 *************************************************************/

/* Comment this out to disable prints and save space */

#define BLYNK_PRINT Serial

#include <FavoriotHTTPS.h>
#include <ESP8266HTTPClient.h>
#include <SimpleDHT.h>
#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>

// You should get Auth Token in the Blynk App.
// Go to the Project Settings (nut icon).
char auth[] = "AUTH TOKEN BLYNK"; // SILA TUKAR KEPADA AUTH TOKEN ANDA

const char apikey[]   = "Favoriot apikey";  // SILA TUKAR KEPADA FAVORIOT APIKEY ANDA
FavoriotHTTPS favoriot;
unsigned long previousMillis = 0;

// Your WiFi credentials.
// Set password to "" for open networks.
char ssid[] = "SSID ANDA"; // SILA TUKAR KEPADA WIFI ID ANDA
char pass[] = "PASSWORD WIFI ANDA"; // SILA TUKAR KEPADA PASSWORD WIFI ANDA

int pinDHT11 = D6;
SimpleDHT11 dht11(pinDHT11);

void setup()
{
  // Debug console
  Serial.begin(9600);

  Blynk.begin(auth, ssid, pass);

  favoriot.begin(ssid, pass, apikey);

  // You can also specify server:
  //Blynk.begin(auth, ssid, pass, "blynk-cloud.com", 80);
  //Blynk.begin(auth, ssid, pass, IPAddress(192,168,1,100), 8080);
}

void loop()
{
  Blynk.run();

   // start working...
  Serial.println("=================================");
  Serial.println("Sample DHT11...");
  
  // read without samples.
  byte temperature = 0;
  byte humidity = 0;
  int err = SimpleDHTErrSuccess;
  if ((err = dht11.read(&temperature, &humidity, NULL)) != SimpleDHTErrSuccess) {
    Serial.print("Read DHT11 failed, err="); Serial.println(err);delay(1000);
    return;
  }
  
  Serial.print("Sample OK: ");
  Serial.print((int)temperature); Serial.print(" *C, "); 
  Serial.print((int)humidity); Serial.println("% H");
  delay(1500);

  Blynk.virtualWrite(V0, temperature);
  Blynk.virtualWrite(V1, humidity);

//  if(humidity > 80);{
//    Blynk.notify("Amaran, kelembapan melebihi 80%");
//    Blynk.email("mzakiab@gmail.com", "Subjek Email", "Content email");
    
//  }

    if(millis() - previousMillis > 180000){
    previousMillis = millis();

 // Nota: 10,000 = 1 saat, 60,000 = 1 minit

    // STEP 3 - Send data to Favoriot Data Stream
    favoriot.deviceId("DEVICE ID ANDA"); // SILA TUKAR KEPADA DEVICE ID ANDA
    favoriot.dataStream("suhu", String(temperature));
    favoriot.dataStream("kelembapan", String(humidity));
    // add other dataStream as you wish
    favoriot.dataStreamEnd();
//    }


// Tambahan untuk ThingSpeak - MULA
//    if(millis() - previousMillis1 > 60000){
//    previousMillis1 = millis();

  HTTPClient http; 

  http.begin("http://api.thingspeak.com/update?api_key=API KEY THING SPEAK ANDA&field1=" + String(temperature) + "&field2=" + String(humidity));
      // SILA TUKAR API KEY THING SPEAK ANDA

int httpCode = http.GET();

if(httpCode > 0){
  Serial.print("Transfer data ke ThingSpeak BERJAYA!. Nombor siri: ");
  String response = http.getString();
  Serial.println(response);
}
    }
// Tambahan untuk ThingSpeak - TAMMAT
}
