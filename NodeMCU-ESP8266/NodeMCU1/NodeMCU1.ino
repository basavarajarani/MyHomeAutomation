#include <ESP8266WiFi.h> 
#include <PubSubClient.h>
#include <string.h>
#include <pins_arduino.h>

const char * ssid = "AMAIRA";
const char * password = "PeacH@123";
const char * inTopic = "NodeMCU1/In/Switches";
const char * outTopic = "NodeMCU1/Out/Updates";

//const char* mqtt_server = "test.mosquitto.org";
//const char* mqtt_server = "iot.eclipse.org";
const char * mqtt_server = "10.0.0.163";

WiFiClient espClient;
PubSubClient client(espClient);
void setup() {
pinMode(LED_BUILTIN, INPUT);
  pinMode(D0, OUTPUT);
  pinMode(D1, OUTPUT);
  pinMode(D2, OUTPUT);
  pinMode(D3, OUTPUT);
  
  Serial.begin(9600);
  setup_wifi();
  client.setServer(mqtt_server, 1883);

  client.setCallback(callback);
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

  
  if (strcmp(switchName,"1") == 0) {
    if (strcmp(switchCommand,"1") == 0) {
          digitalWrite(D0, HIGH); // Turn the LED on (Note that LOW is the voltage level
          Serial.println("DS0--> Turned On");
    } else if (strcmp(switchCommand,"0") == 0 ) {
          digitalWrite(D0,LOW); // Turn the LED off (Note that LOW is the voltage level
          Serial.println("DS0--> Turned off");
    }



  }else if (strcmp(switchName,"2") == 0) {
    if (strcmp(switchCommand,"1") == 0) {
          digitalWrite(D1, HIGH); // Turn the LED on (Note that LOW is the voltage level
          Serial.println("DS1--> Turned On");
    } else if (strcmp(switchCommand,"0") == 0 ) {
          digitalWrite(D1,LOW); // Turn the LED off (Note that LOW is the voltage level
          Serial.println("DS1--> Turned off");
    }    
  }else if (strcmp(switchName,"3") == 0) {
    if (strcmp(switchCommand,"1") == 0) {
          digitalWrite(D2, HIGH); // Turn the LED on (Note that LOW is the voltage level
          Serial.println("DS2--> Turned On");
    } else if (strcmp(switchCommand,"0") == 0 ) {
          digitalWrite(D2,LOW); // Turn the LED off (Note that LOW is the voltage level
          Serial.println("DS2--> Turned off");
    }    
  }else if (strcmp(switchName,"4") == 0) {
    if (strcmp(switchCommand,"1") == 0) {
          digitalWrite(D3, HIGH); // Turn the LED on (Note that LOW is the voltage level
          Serial.println("DS3--> Turned On");
    } else if (strcmp(switchCommand,"0") == 0 ) {
          digitalWrite(D3,LOW); // Turn the LED off (Note that LOW is the voltage level
          Serial.println("DS3--> Turned off");
    }    
  }

  Serial.println();

}

void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Attempt to connect
    if (client.connect("ESP8266Client")) {
      Serial.println("connected");
      // Once connected, publish an announcement...
      client.publish(outTopic, "Connected!");
      // ... and resubscribe
      client.subscribe(inTopic);
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}

void loop() {

  if (!client.connected()) {
    reconnect();
  }
  client.loop();
}
