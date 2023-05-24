#python
import numpy as np
import sim
import simROS2
import pyproj as projection

publisher_cmd_vel = None
# publisher_steering = None
subscriber_latitude = None
subscriber_longitude = None
global latitude, longitude, x, y

def init():
    global publisher_cmd_vel, publisher_steering, subscriber_latitude, subscriber_longitude
    # Prepare the float32 publisher and subscriber
    if simROS2:
        publisher_cmd_vel = simROS2.createPublisher('/cmd_vel','geometry_msgs/msg/Twist')
        # publisher_steering = simROS2.createPublisher('/steering','std_msgs/msg/Float32')
        subscriber_latitude = simROS2.createSubscription('/lat','std_msgs/msg/Float32','subscriber_latitude_callback')
        subscriber_longitude = simROS2.createSubscription('/lon','std_msgs/msg/Float32','subscriber_longitude_callback')

def subscriber_latitude_callback(msg):
    global latitude
    print (msg.keys()) # for debug (to get the keys of the python dict associated to the topic)
    latitude = msg['data']
    print("latitude=",latitude)
    

def subscriber_longitude_callback(msg):
    global longitude
    print (msg.keys()) # for debug (to get the keys of the python dict associated to the topic)
    longitude = msg['data']
    print("longitude=",longitude)


def cmd():
    global publisher_cmd_vel, subscriber_latitude, subscriber_longitude, x , y, latitude, longitude
    p = projection.Proj(proj='utm', zone=10, ellps='WGS84', preserve_units=False) #création de la projection
    x, y = p(longitude, latitude, inverse=True)
    #faire une fonction de contrôle qui permet d'obtenir la vitesse et l'angle de direction souhaités pour suivre la trajectoire
    vel = 0.5
    steering = 0.2
    if simROS2:
        simROS2.publish(publisher_cmd_vel,{'linear':{'x':vel},'angular':{'z':steering}})
        # simROS2.publish(publisher_steering,{'data':steering})

def control():
    #fonction de contrôle

    pass