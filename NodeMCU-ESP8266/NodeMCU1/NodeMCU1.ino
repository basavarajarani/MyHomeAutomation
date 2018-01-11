#include <ESP8266WiFi.h> 
#include <PubSubClient.h>
#include <string.h>
#include <pins_arduino.h>

const char * ssid = "AMAIRA2";
const char * password = "PeacH@123";

const String nodeName = "NodeMCU1";
const String inMqttTopic= nodeName+ String("/In/Switches");
const String outMqttUpdates = nodeName+String("/Out/Updates");
const String outMqttStatus = nodeName+String("/Out/Status");

const String outMqttMoisture = nodeName+String("/Out/MoistureLevel");
const String mqtt_server = "10.0.0.163";

const int AnalogIn  = A0;
unsigned long currentTime;
const int wateringThreshold = 500;

WiFiClient espClient;
PubSubClient mqttClient(espClient);
void setup() {
pinMode(LED_BUILTIN, INPUT);
  pinMode(D5, OUTPUT);
  pinMode(D6, OUTPUT);
  pinMode(D7, OUTPUT);
  pinMode(D8, OUTPUT);
  
  Serial.begin(9600);
  setup_wifi();
  mqttClient.setServer(mqtt_server.c_str(), 1883);

  mqttClient.setCallback(callback);
  reconnect();
}

void setup_wifi() {

  delay(10);
  // We start by connecting to a WiFi network
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}

void callback(char * topic, byte * payload, unsigned int length) {
  char* switchName;
  char* switchCommand;

  char payLoadChar[500];
  strncpy(payLoadChar, (char*)payload,length);
  payLoadChar[length] = '\0';

  switchName = strtok(payLoadChar,"=");
  switchCommand = strtok(NULL,"=");

  Serial.println(String(switchName)+"="+String(switchCommand));

  
  if (strcmp(switchName,"5") == 0) {
    if (strcmp(switchCommand,"1") == 0) {
          digitalWrite(D5, HIGH); // Turn the LED on (Note that LOW is the voltage level
          Serial.println("DS5--> Turned On");
    } else if (strcmp(switchCommand,"0") == 0 ) {
          digitalWrite(D5,LOW); // Turn the LED off (Note that LOW is the voltage level
          Serial.println("DS5--> Turned off");
    }



  }else if (strcmp(switchName,"6") == 0) {
    if (strcmp(switchCommand,"1") == 0) {
          digitalWrite(D6, HIGH); // Turn the LED on (Note that LOW is the voltage level
          Serial.println("DS1--> Turned On");
    } else if (strcmp(switchCommand,"0") == 0 ) {
          digitalWrite(D6,LOW); // Turn the LED off (Note that LOW is the voltage level
          Serial.println("DS1--> Turned off");
    }    
  }else if (strcmp(switchName,"7") == 0) {
    if (strcmp(switchCommand,"1") == 0) {
          digitalWrite(D7, HIGH); // Turn the LED on (Note that LOW is the voltage level
          Serial.println("DS2--> Turned On");
    } else if (strcmp(switchCommand,"0") == 0 ) {
          digitalWrite(D7,LOW); // Turn the LED off (Note that LOW is the voltage level
          Serial.println("DS2--> Turned off");
    }    
  }else if (strcmp(switchName,"8") == 0) {
    if (strcmp(switchCommand,"1") == 0) {
          digitalWrite(D8, HIGH); // Turn the LED on (Note that LOW is the voltage level
          Serial.println("DS3--> Turned On");
    } else if (strcmp(switchCommand,"0") == 0 ) {
          digitalWrite(D8,LOW); // Turn the LED off (Note that LOW is the voltage level
          Serial.println("DS3--> Turned off");
    }    
  }else if (strcmp(switchName,"100") == 0) {
    if (strcmp(switchCommand,"100") == 0) {
      Serial.println("Received Probe Status");
      mqttClient.publish(outMqttStatus.c_str(), "Online");
    }
  }

  Serial.println();

}

void reconnect() {
  // Loop until we're reconnected
  while (!mqttClient.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Attempt to connect
    if (mqttClient.connect(nodeName.c_str())) {
      Serial.println("connected");
      // Once connected, publish an announcement...
      mqttClient.publish(outMqttUpdates.c_str(), "Connected!");
      // ... and resubscribe
      mqttClient.subscribe(inMqttTopic.c_str());
    } else {
      Serial.print("failed, rc=");
      Serial.print(mqttClient.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}

void loop() {
  static unsigned long startTime = 0;

  if (!mqttClient.connected()) {
    reconnect();
  }
  mqttClient.loop();


  /* Check if the plants need watering */
  currentTime = millis();

  if (startTime == 0)
  {
    readAndSendSoilMoistureLevel();
    startTime = currentTime;
  }

  if (currentTime - startTime >= 120000UL)
  {
    // reste start time; Every hour
    // next iteration of loop a temperature reading will be done because startTime equals 0

    startTime = 0;
  }

}

void readAndSendSoilMoistureLevel () {
  int moistureLevel = 0;
  String moistureLevelString;
  Serial.print("Sending MoistLevel:");
  moistureLevel = analogRead(AnalogIn);
  moistureLevelString = String((int) moistureLevel);
  Serial.println(moistureLevelString);
  mqttClient.publish(outMqttMoisture.c_str(), moistureLevelString.c_str());
}
