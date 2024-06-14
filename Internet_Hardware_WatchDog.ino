#include <ESP8266WiFi.h>             // NodeMCU 0.09
#include <WiFiClient.h>              // Wifi Library
#include <ESP8266Ping.h>             // https://github.com/dancol90/ESP8266Ping/


const char* WIFI_SSID   = "SSID";        // Your SSID
const char* WIFI_PWD    = "PASS";        // Your WiFi Password

#define RELAY D0
#define LED1  D1
#define LED2  D2
#define LED3  D3
#define LED4  D5
#define LED5  D7
int random_n    = 1;    
int ctr_main    = 1;
int fail_count  = 0;  // Increments if ping fails
int reset_timer = 90; 


void setup() {
  Serial.begin(115200);
  delay(10);

  /** Define the pin modes **/
  pinMode(RELAY, OUTPUT);
  pinMode(LED1, OUTPUT);
  pinMode(LED2, OUTPUT);
  pinMode(LED3, OUTPUT);
  pinMode(LED4, OUTPUT);
  pinMode(LED5, OUTPUT);

  /** We start by connecting to a WiFi network **/  
  IPAddress local_IP(192, 168, 0, 5);     // Static IP Address for ESP8266
  IPAddress subnet(255, 255, 255, 0);       // Subnet Mask
  IPAddress gateway(192, 168, 0, 1);        // Default Gateway

  /*********** Setup a WiFi connection ***********/
  if (WiFi.config(local_IP, gateway, subnet)) { Serial.println("Static IP Configured"); }
  else                                        { Serial.println("Static IP Configuration Failed"); };

  WiFi.mode(WIFI_STA);
  WiFi.begin(WIFI_SSID, WIFI_PWD);
  Serial.println(); Serial.println("Connecting to WiFi");
  digitalWrite(LED2, HIGH); digitalWrite(LED4, HIGH); // Turn on all the red LEDs (first boot)
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  };
  Serial.println("\nWiFi connected");
  Serial.print("IP address: "); Serial.println(WiFi.localIP());
};


void loop() {
  /** Successful Pings **/
  if ( pingTest() && (fail_count < 5) ) {             // If a successful ping is received
    fail_count = 0;                                   // Reset the fail count
    digitalWrite(LED2, LOW); digitalWrite(LED4, LOW); // Turn off all red LEDs
    random_n = random(1, 4);                          // Generate a random number
    switch(random_n){                                 // Pick a patter to display
      case 1: pattern_blue1(); break;
      case 2: pattern_blue2(); break;
      case 3: pattern_blue3(); break;
    };
  } else {
    if ( fail_count < 5 ){
      fail_count++;                                   // If ping test fails - increment the fail_count
      Serial.print("Fail Count: ");Serial.println(fail_count);
      pattern_red();                                  // Flash the RED LED
    };

    /** Ping Completely Failed **/
    if ( fail_count >= 5 ) {                          // Reset the router when more than 5 continus pings fail 
      if ( ctr_main <= reset_timer ) { digitalWrite(RELAY, HIGH); Serial.println("Disconnected.."); pattern_red(); ctr_main++;    }; // Disconnect the circuit for few seconds
      if ( ctr_main >  reset_timer ) { digitalWrite(RELAY, LOW);  Serial.println("Reconnected..."); ctr_main = 1; fail_count = 0; }; // Reconnect after the time is passed
    };
  };
};


/** Function that performs the ping test **/
bool pingTest(){
  IPAddress ip (8, 8, 8, 8); // The remote ip to ping
  Serial.print("Pinging host : ");Serial.println(ip);//bool x =Ping.ping(ip);
  if(Ping.ping(ip, 2)) { // Ping the remote host with two ping requests
    Serial.println("Success!!");
    return true;
  } else {
    Serial.println("Error :(");
    return false;
  };
};


/** Function that is called in an event of loss of ping **/
void pattern_red(){  
  digitalWrite(LED1, LOW); digitalWrite(LED3, LOW); digitalWrite(LED5, LOW);
  digitalWrite(LED2, LOW); digitalWrite(LED4, LOW);
  delay(400);
  digitalWrite(LED2, HIGH); digitalWrite(LED4, HIGH);
  delay(400);
};


/** Blue Patterns **/
void pattern_blue1(){
  int i = 0;
  do{
    digitalWrite(LED1, HIGH); digitalWrite(LED3, HIGH); digitalWrite(LED5, HIGH);
    if ( i == 5 ) break;
    delay(400);
    digitalWrite(LED1, LOW); digitalWrite(LED3, LOW); digitalWrite(LED5, LOW);
    delay(400);
    i++;
  } while( true );
};


void pattern_blue2(){
  int i = 0;
  do{
    digitalWrite(LED1, HIGH); digitalWrite(LED3, LOW); digitalWrite(LED5, HIGH);
    delay(400);
    digitalWrite(LED1, LOW); digitalWrite(LED3, HIGH); digitalWrite(LED5, LOW);
    if ( i == 5 ) break;
    delay(400);
    i++;
  } while( true );
};


void pattern_blue3(){
  int i = 0;
  do{
    digitalWrite(LED1, HIGH); digitalWrite(LED3, LOW); digitalWrite(LED5, LOW);
    delay(200);
    digitalWrite(LED1, LOW); digitalWrite(LED3, HIGH); digitalWrite(LED5, LOW);
    delay(200);
    digitalWrite(LED1, LOW); digitalWrite(LED3, LOW); digitalWrite(LED5, HIGH);
    delay(200);
    digitalWrite(LED1, LOW); digitalWrite(LED3, HIGH); digitalWrite(LED5, LOW);
    if ( i == 5 ) break;
    delay(200);
    i++;
  } while( true );
};
