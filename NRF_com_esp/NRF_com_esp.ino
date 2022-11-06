#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

#if defined(ESP32)
#include <WiFi.h>
#include <FirebaseESP32.h>
#elif defined(ESP8266)
#include <ESP8266WiFi.h>
#include <FirebaseESP8266.h>
#endif


#include <addons/TokenHelper.h>

//Provide the RTDB payload printing info and other helper functions.
#include <addons/RTDBHelper.h>

/* 1. Define the WiFi credentials */
#define WIFI_SSID "hello"
#define WIFI_PASSWORD "helloworld"

//For the following credentials, see examples/Authentications/SignInAsUser/EmailPassword/EmailPassword.ino

/* 2. Define the API Key */
#define API_KEY "MMK4mXNJRvB7smpsY1LBAJOnhY5HnaZLfCbzN1cd"

/* 3. Define the RTDB URL */
#define DATABASE_URL "https://shakthi-f143a-default-rtdb.firebaseio.com/" //<databaseName>.firebaseio.com or <databaseName>.<region>.firebasedatabase.app


//Define Firebase Data object
FirebaseData fbdo;

FirebaseAuth auth;
FirebaseConfig config;

float longitude, latitude;



RF24 radio(7, 8); // CE, CSN

const byte address[6] = "00001";

// Max size of this struct is 32 bytes - NRF24L01 buffer limit
struct Data_Package {
  int pulse = 1;
  float longitude = 0;
  float latitude = 0;

};

Data_Package data; //Create a variable with the above structure

void setup() {
  Serial.begin(9600);
  radio.begin();
  radio.openReadingPipe(0, address);
  radio.setPALevel(RF24_PA_MIN);
  radio.startListening();

   Serial.begin(115200);
delay(2000);
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("Connecting to Wi-Fi");
  while (WiFi.status() != WL_CONNECTED)
  {
    Serial.print(".");
    delay(300);
  }
  Serial.println();
  Serial.print("Connected with IP: ");
  Serial.println(WiFi.localIP());
  Serial.println();

  Serial.printf("Firebase Client v%s\n\n", FIREBASE_CLIENT_VERSION);

  /* Assign the api key (required) */
  config.api_key = API_KEY;

  config.database_url = DATABASE_URL;



  //////////////////////////////////////////////////////////////////////////////////////////////
  //Please make sure the device free Heap is not lower than 80 k for ESP32 and 10 k for ESP8266,
  //otherwise the SSL connection will fail.
  //////////////////////////////////////////////////////////////////////////////////////////////

  Firebase.begin(DATABASE_URL, API_KEY);

  //Comment or pass false value when WiFi reconnection will control by your code or third party library
 // Firebase.reconnectWiFi(true);

  Firebase.setDoubleDigits(5);
  
}

void loop() {

}

    longitude = 77.5275351;
    latitude = 12.9300783;
  // Check whether there is data to be received
  if (radio.available()) {
    radio.read(&data, sizeof(Data_Package)); // Read the whole data and store it into the 'data' structure
  }
  Serial.print("Status: ");
  Serial.print(data.pulse);
  Serial.print("Longitude: ");
  Serial.print(data.longitude);
  Serial.print("Latitude: ");
  Serial.print(data.latitude);

  if (Firebase.ready()) 
  {
    
    //Firebase.setInt(fbdo, main, 5);
    Firebase.setInt(fbdo, "/test/logitude", longitude);
    Firebase.setInt(fbdo, "/test/latitude", latitude);
    delay(200);

    Serial.printf("Get int a--  %s\n", Firebase.getInt(fbdo, "/test/longitude") ? String(fbdo.to<int>()).c_str() : fbdo.errorReason().c_str());
     longitude=fbdo.to<int>();
    Serial.printf("Get int b--  %s\n", Firebase.getInt(fbdo, "/test/latitude") ? String(fbdo.to<int>()).c_str() : fbdo.errorReason().c_str());
     latitude=fbdo.to<int>();

//  Serial.println();  
//  Serial.print(" Logitude :");
//  Serial.print(a);
//  Serial.print("  latitude : ");
//  Serial.print(b);
  
  Serial.println();
  Serial.println("------------------");
  Serial.println();
  

  delay(2500);

}
