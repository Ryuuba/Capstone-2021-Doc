import paho.mqtt.client as mqtt
import sqlite3_conn
from time import time

from sys import argv

client_id = ''
host = ''
topics = ''
db_path = ''
db_conn = None

def on_disconnect(client, userdata, rc):
    """
    This callback closes cleanly the database
    """
    if db_conn:
        db_conn.close()

def on_connect(client, userdata, flags, rc):
    """
    This callback prints the number of topics this client subscribes. When this
    client loses the connection, then it renews all subscriptions
    @param rc Result code
    @note client, userdata, and flags parameters are given by the MQTT API
    """
    print('client: {}, userdata: {}, flags {}'.format(client, userdata, flags))
    print('connected with result code ' + str(rc))
    print('Storing reading in database {}'.format(db_path))
 
def on_message(client, userdata, msg):
    """
    The callback for when a PUBLISH message is received from the server.
    This client records a 3-tuple <date, location, reading> where date is the
    current date in UNIX format, location is the place where the remote sensor 
    is located a reading, and the reading is the value captured by the sensor.
    @param msg The message this client receives
    @note client and userdata are given by the MQTT API
    """
    print(msg.topic + ' ' + str(msg.payload, encoding='UTF8'))
    print(msg.topic.split('/'))
    data_tuple = (
        str(int(time())),                  # The current time (UNIX format)
        msg.topic.split('/')[1],      # The sensor location
        int(str(msg.payload, encoding='UTF8')) # The sensor reading
    )
    print('Data tuple {} is saved into {}'.format(data_tuple, db_path))
    sqlite3_conn.insert_tuple(db_conn, data_tuple)

def main():
    client = mqtt.Client(client_id)
    client.on_connect = on_connect
    client.on_message = on_message
    client.on_disconnect = on_disconnect
    topic_list = []
    with open(topics, 'r') as f:
        for line in f.readlines():
            topic_list.append(line.strip())
    client.connect(host, 1883, 60)   
    for topic in topic_list:
        client.subscribe(topic)
    global db_conn
    db_conn = sqlite3_conn.create_connection(db_path)
    client.loop_forever()

if __name__=='__main__':
    client_id = argv[1]
    host = argv[2]
    topics = argv[3]
    db_path = argv[4]
    main()