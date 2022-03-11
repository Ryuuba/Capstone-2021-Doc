import paho.mqtt.client as mqtt
from sys import argv

# The callback for when the client receives a CONNACK response from the server.
def on_connect(client, userdata, flags, rc):
    print("Connected with result code "+str(rc))

    # Subscribing in on_connect() means that if we lose the connection and
    # reconnect then subscriptions will be renewed.
    # client.subscribe("$SYS/#")

# The callback for when a PUBLISH message is received from the server.
def on_message(client, userdata, msg):
    print(msg.topic+" "+str(msg.payload))

def main():
    client_id = argv[1]
    host = argv[2]
    topics = argv[3]
    client = mqtt.Client(client_id)
    client.on_connect = on_connect
    client.on_message = on_message
    topic_list = []
    with open(topics, 'r') as f:
        for line in f.readlines():
            topic_list.append(line.strip())
    client.connect(host, 1883, 60)   
    for topic in topic_list:
        client.subscribe(topic)

# Blocking call that processes network traffic, dispatches callbacks and
# handles reconnecting.
# Other loop*() functions are available that give a threaded interface and a
# manual interface.
    client.loop_forever()

if __name__=='__main__':
    main()