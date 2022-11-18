//ok
 #include <WiFi.h>  

#include <PubSubClient.h>
#include <WiFiClientSecure.h>

//---- WiFi settings
const char* ssid = "ZTE_F8FA3C_2.4G";
const char* password = "20989296";

//---- MQTT Broker settings
const char* mqtt_server = "192.168.0.155"; // replace with your broker url

const int mqtt_port =1883;


WiFiClient espClient;
PubSubClient client(espClient);
unsigned long lastMsg = 0;

#define MSG_BUFFER_SIZE  (50)
char msg[MSG_BUFFER_SIZE];

int sensor1 = 0;
float sensor2 = 0;
float sensor3=0;
int command1 =0;
int state=0;

const char* sensor2_topic= "AshesiInTopic/LDR/2";
const char*  sensor1_topic="AshesiInTopic/ultra/2";
const char*  sensor3_topic="AshesiIoTTopic";

const char* command1_topic="motorcontrol/esp2";
const char* command2_topic="AshesiIoTTopic/#";

const int LDR_PIN = 34;
int ldrvalue=0; 
 
const int trigPin = 18; 
const int echoPin = 4; 
 
const int Motor_p = 5; 
 
 
#define SOUND_SPEED 0.034 
#define CM_TO_INCH 0.393701 
 
long duration; 
float distanceCm;
float LDR_Voltage; 
 
void ultrasonic(void*parameters); 
void LDR(void*parameters); 
void Motor(void*parameters); 

//==========================================
void setup_wifi() {
  delay(10);
  Serial.print("\nConnecting to ");
  Serial.println(ssid);

  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  Serial.println("");

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  //=======
  Serial.println("");
  Serial.print("Connected to ");
  Serial.println(ssid);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
  //=======
  randomSeed(micros());
  Serial.println("\nWiFi connected\nIP address: ");
  Serial.println(WiFi.localIP());
}


//=====================================
void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    String clientId = "ESP32Client-";   // Create a random client ID
    clientId += String(random(0xffff), HEX);  //you could make this static
    // Attempt to connect
    if (client.connect(clientId.c_str())){//, mqtt_username, mqtt_password)) {
      Serial.println("connected");

      client.subscribe(command1_topic);   // subscribe the topics here
      client.subscribe(command2_topic);   // subscribe the topics here
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");   // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}
 
void setup() { 
  
 Serial.begin(115200);
  //while (!Serial) delay(1);
  Serial.println("Setting up");
  setup_wifi();
  pinMode(BUILTIN_LED, OUTPUT);     // Initialize the BUILTIN_LED pin as an output

//
//  #ifdef ESP8266
//    espClient.setInsecure();
//  #else   // for the ESP32
//    espClient.setCACert(root_ca);      // enable this line and the the "certificate" code for secure connection
//  #endif
  
  client.setServer(mqtt_server, 1883 );//mqtt_port
  client.setCallback(callback);
  analogReadResolution(10); //default is 12. Can be set between 9-12. 
   pinMode(Motor_p, OUTPUT); 
 
  // put your setup code here, to run once: 
  pinMode(trigPin, OUTPUT); 
  pinMode(echoPin, INPUT); 
  
  xTaskCreate( 
    ultrasonic, 
    "This is tasl 1", 
    1024, 
    NULL, 
    1, 
    NULL 
  ); 
 
    xTaskCreate( 
    LDR, 
    "This is tasl 1", 
    1024, 
    NULL, 
    1, 
    NULL 
  ); 
 
    xTaskCreate( 
    Motor, 
    "This is tasl 1", 
    1024, 
    NULL, 
    1, 
    NULL 
  ); 
} 
 


void loop()
{
  if (!client.connected()) reconnect();
  client.loop();

  //---- example: how to publish sensor values every 5 sec
  unsigned long now = millis();
  if (now - lastMsg > 5000) {
    lastMsg = now;
    String sensor1 = String(distanceCm);
    String sensor2 = String(LDR_Voltage);
    publishMessage(sensor1_topic,String(sensor1),true);    
    publishMessage(sensor2_topic,String(sensor2),true);
   
  }
}
//=======================================  
// This void is called every time we have a message from the broker

void callback(char* topic, byte* payload, unsigned int length) {
  String incommingMessage = "";
  for (int i = 0; i < length; i++) incommingMessage+=(char)payload[i];
  
  Serial.println("Message arrived ["+String(topic)+"]"+incommingMessage);
  
  //--- check the incomming message
    if( strcmp(topic,command1_topic) == 0){
     if (incommingMessage.equals("1")) state=1;   // Turn the LED on 
     else state=0;  // Turn the LED off 
  }

   //  check for other commands
 /*  else  if( strcmp(topic,command2_topic) == 0){
     if (incommingMessage.equals("1")) {  } // do something else
  }
  */
}



//======================================= publising as string
void publishMessage(const char* topic, String payload , boolean retained){
  if (client.publish(topic, payload.c_str(), true));
//      Serial.println("Message publised ["+String(topic)+"]: "+payload);
}
 
void ultrasonic(void*parameters){ 
   
for(;;){ 
  digitalWrite(trigPin, LOW); 
  delayMicroseconds(2); 
 
  digitalWrite(trigPin, HIGH); 
  delayMicroseconds(10); 
  digitalWrite(trigPin, LOW); 
 
  duration = pulseIn(echoPin,HIGH); 
  distanceCm = duration* SOUND_SPEED/2; 
 
  Serial.print("Distance (cm): "); 
  Serial.println(distanceCm); 
 
  vTaskDelay(1000); 
} 
 
 
} 
 
void LDR(void*parameters){ 
   
for(;;){ 
  
    ldrvalue=analogRead(LDR_PIN);//Reads the Value of LDR(light).
    LDR_Voltage = 0.4168* ldrvalue - 46.2;
    if (LDR_Voltage < 0) {
      LDR_Voltage=0;
    }
    Serial.print("LDR LUX value is :");//Prints the value of LDR to Serial Monitor.
    Serial.print(LDR_Voltage);
    Serial.println("");

    }
  vTaskDelay(1000); 
} 
 
 
 
 
void Motor(void*parameters){ 
   
for(;;){ 
  digitalWrite(Motor_p, state);   // turn the LED on (HIGH is the voltage level) 
//  delay(10000);                       // wait for a second 
//  digitalWrite(Motor_p, LOW);    // turn the LED off by making the voltage LOW 
//  delay(10000);                       // wait for a second 
// 
  vTaskDelay(1000); 
} 
 
 
}
