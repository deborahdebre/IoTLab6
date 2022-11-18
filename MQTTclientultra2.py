import paho.mqtt.client as mqtt
import urllib.request

MQTT_SERVER = "192.168.0.155"
MQTT_PATH = "AshesiInTopic/ultra/2"

#url ="http://172.16.6.138/IoT/data_insert.php?Customer_Name=Kwad&Place=Tema&Water_Level="

# The callback for when the client receives a CONNACK response from the server.
def on_connect(client, userdata, flags, rc):
    print("Connected with result code "+str(rc))
 
    # Subscribing in on_connect() means that if we lose the connection and
    # reconnect then subscriptions will be renewed.
    client.subscribe(MQTT_PATH)
 
# The callback for when a PUBLISH message is received from the server.
def on_message(client, userdata, msg):
    #print(msg.topic+" "+str(msg.payload))
    #kojo="yay"
    #"http://192.168.0.155    /IoT/data_insert.php?Customer_Name=Kwad&Place=Tema&Water_Level=2.2"
    url ="http://192.168.0.155/IoT/data_insert.php?data_insert_ultra.php?deviceID=1&ulttraID=1&ultraVal="+str(float(msg.payload))
    contents = urllib.request.urlopen(url).read()
    print (float(msg.payload))

    # more callbacks, etc
 
client = mqtt.Client()
client.on_connect = on_connect
client.on_message = on_message
 
client.connect(MQTT_SERVER, 1883, 60)
 
# Blocking call that processes network traffic, dispatches callbacks and
# handles reconnecting.
# Other loop*() functions are available that give a threaded interface and a
# manual interface.
client.loop_forever()
