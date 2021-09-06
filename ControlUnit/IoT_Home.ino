#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
//#include <WiFiClient.h>
#include <ArduinoJson.h>

 
/* smarthome wi-Fi wi-Fi informations */
const char* ssid = "";	 /* REPLACE_WITH_YOUR_SSID     */
const char* passwd = ""; /* REPLACE_WITH_YOUR_PASSWORD */

const char* server_IP = "http://192.168.1.24:5000/api/post_temperature";

/*
unsigned long lastTime = 0;
unsigned long timerDelay = 5000;
*/


/* NTC Thermistor Initialization */
String command;
const double VCC = 3.3;                /* NodeMCU VCC              */
const double R2 = 10000;                /* 10K ohm series resistor  */
const double adc_resolution = 1023;    /* 10-bit adc (2^10) reader */

const double A = 0.001129148;    /* thermistor equation parameters */
const double B = 0.000234125; 
const double C = 0.0000000876741; 

void setup()
{
  Serial.begin(115200);  /* Define baud rate for serial communication */
  
  WiFi.begin(ssid, passwd); /* Start WiFi Connection*/
  Serial.println("Connecting");
  
  /* Attempt to connect wi-Fi network */
  while(WiFi.status() != WL_CONNECTED) {  
    delay(500);
    Serial.print("Waiting for connection.");
  }

  Serial.println("");
  Serial.print("Connected to WiFi network with IP Address: ");
  Serial.println(WiFi.localIP());
  //WiFi.setAutoReconnect(true); /* Force to reconnect             */
  //WiFi.persistent(true);       /* Force to keep connection alive */
}


void loop()
{
  double Vout, /* ADC Read Voltage    */
          Rth, /* */
  temperature, /* Temperature Value   */
    adc_value; /* ADC value 0 to 1023 */
    
  adc_value = analogRead(A0);
  Vout = (adc_value * VCC) / adc_resolution;  /* adc_value 0 to 1023 */
  Rth = (VCC * R2 / Vout) - R2;               /* Vout 0 to 3.3V      */
 
  /***
     Steinhart-Hart Thermistor Formula:
     T in Kelvin = 1 / (A + B[ln(R)] + C[ln(R)]^3)
  ***/
  temperature = (1 / (A + (B * log(Rth)) + (C * pow((log(Rth)),3))));
  temperature = temperature - 273.15;  /* Convert Temperature Degree to Celsius */
  
  if(WiFi.status() == WL_CONNECTED) { /* Check Connection Status */
    StaticJsonBuffer<300> JSONbuffer; /* Static JSON Buffer */
    JsonObject& JSONencoder = JSONbuffer.createObject();

    /* Serialize JSON Object */ 
    JSONencoder["token"] = "0x5AE98c80";
    JSONencoder["Temperature"] = temperature;

    char JSONmessageBuffer[300];
    JSONencoder.prettyPrintTo(JSONmessageBuffer, sizeof(JSONmessageBuffer));
    Serial.print("JSONmessageBuffer:");
    Serial.println(JSONmessageBuffer);

    /* Initialize HTTP Client */
    HTTPClient http;
    http.begin(server_IP); /* Specify request destination */
    http.addHeader("Content-Type", "application/json");          /* Content-Type header         */

    /* Post JSON Object via HTTP */
    int httpCode = http.POST(JSONmessageBuffer);
    String payload = http.getString();

    Serial.print("httpCode:");   //Print HTTP return code
    Serial.println(httpCode);
    Serial.print("Payload:");
    Serial.println(payload);    //Print request response payload

    http.end();
  }
  else {
    Serial.println("WiFi Connection Error");
  }
  delay(4000);

}
