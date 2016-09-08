#define _DEBUG_

#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>

#define SSID "DEFINE_ME"                              // insert your SSID
#define PASS "DEFINE_ME"
#define RANCHER_KEY "DEFINE_ME"

// You can set your own credentials in credentials.h to override the default settings here
#include "credentials.h"

int buttonState = 0;         // variable for reading the pushbutton status
#define BUTTON_PIN 2     // the number of the pushbutton pin

void setup() {
                               // used to debug, disable wachdog timer,
  Serial.begin(115200);                           // full speed to monitor
  Serial.println("Setup start");

   // pinMode(LED_BUILTIN, OUTPUT);     // Initialize the LED_BUILTIN pin as an output
  //  digitalWrite(LED_BUILTIN, LOW); 

    pinMode(BUTTON_PIN, INPUT);
       digitalWrite(LED_BUILTIN, HIGH); 
    connectWifi();

  //delay(1000);
  //  makeRequest();
}

void connectWifi(){

 WiFi.begin(SSID, PASS);                         // Connect to WiFi network
  while (WiFi.status() != WL_CONNECTED) {         // Wait for connection
    delay(500);
    Serial.print(".");
  }
  Serial.println();

  Serial.print("Local IP: ");
  Serial.println(WiFi.localIP());
  //digitalWrite(LED_BUILTIN, HIGH);  // Turn the LED off by making the voltage HIGH

}

void loop() {
      //  Serial.println("Loop");

    ESP.wdtDisable();  

    // wait for WiFi connection
    if (WiFi.status() == WL_CONNECTED ){
      //Having no reistors causes the button to be in a random status after being pressed.
      if (digitalRead(BUTTON_PIN) == LOW) {
        makeRequest();
        
        delay(5000);
      }
    } else {
        Serial.print("Client disconnected");
        digitalWrite(LED_BUILTIN, LOW);   // Turn the LED on (Note that LOW is the voltage level
        //connectWifi();
    }
    delay(100);

}


void makeRequest(){
  WiFiClient client;
  if (!client.connect( "rancher.scrip.net", 80)) {
    Serial.println("connection failed");
    return;
  }

  Serial.println("Sending request " );
 /* client.print(String("GET ") + url + " HTTP/1.1\r\n" +
               "Host: 192.168.2.100\r\n" + 
               "Connection: close\r\n\r\n");*/

    //Change the request below to use your own service
   client.print(String() +"POST /v1/projects/1a25/loadbalancerservices/1s3143/?action=deactivate HTTP/1.1\r\n"+
        "Host: rancher.scrip.net\r\n"+
        "Accept: application/json\r\n"+
        "Content-Type: application/json\r\n"+
        "Authorization: Basic "  + RANCHER_KEY +"\r\n"+
        "\r\n"+
        "{}\r\n"+
       "Connection: close\r\n\r\n");

    Serial.println("Awaiting response from host");

  unsigned long timeout = millis();
  while (client.available() == 0) {
    if (millis() - timeout > 5000) {
      Serial.println(">>> Client Timeout !");
      client.stop();
      return;
    }
  }
  while(client.available()){
    String line = client.readStringUntil('\r');
    Serial.print(line);
  }
  Serial.println("<");

  Serial.println("Request finished");
}
