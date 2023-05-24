#python

import sim
import simROS2
import numpy as np
#import pyproj as projection

#for a gaussian noise
#noise = np.random.normal(0,1,100)
# 0 is the mean of the normal distribution you are choosing from
# 1 is the standard deviation of the normal distribution
# 100 is the number of elements you get in array noise



publisher_longitude = None
publisher_latitude = None
subscriber_steering = None
subscriber_speed = None
subscriber_cmd_vel = None
publisher_cap = None



def sysCall_init():
    global publisher_longitude, publisher_latitude, subscriber_steering, subscriber_speed, subscriber_cmd_vel, publisher_cap
    # Prepare the float32 publisher and subscribers
    if simROS2:
        publisher_longitude = simROS2.createPublisher('/lon','std_msgs/msg/Float32')
        publisher_latitude = simROS2.createPublisher('/lat','std_msgs/msg/Float32')
        publisher_cap = simROS2.createPublisher('cap', 'std_msgs/msg/Float32')
        subscriber_steering = simROS2.createSubscription('/direction','std_msgs/msg/Float32','subscriber_direction_callback')
        subscriber_speed = simROS2.createSubscription('/speed','std_msgs/msg/Float32','subscriber_speed_callback')
        subscriber_cmd_vel = simROS2.createSubscription('/cmd_vel', 'geometry_msgs/msg/Twist', 'subscriber_twist_callback')



def subscriber_speed_callback(msg):
    print (msg.keys()) # for debug (to get the keys of the python dict associated to the topic)
    sim.addLog(sim.verbosity_scriptinfos,'subscriber received speed = ' + str(msg['data']))
    speed = msg['data']
    sim.setJointTargetVelocity(sim.getObject("/jointAx3"), speed)

def subscriber_direction_callback(msg):
    print (msg.keys()) # for debug (to get the keys of the python dict associated to the topic)
    sim.addLog(sim.verbosity_scriptinfos, 'subscriber received direction = ' + str(msg['data']))
    direction = msg['data']
    sim.setJointTargetPosition(sim.getObject("/Pivot_Moteur"), direction)

def subscriber_twist_callback(msg):
    print(msg.keys())
    sim.addLog(sim.verbosity_scriptinfos, 'subscriber received cmdvel = ' + str(msg['data']))
    speed = msg['linear']['x']
    direction = msg['angular']['z']
    #speed = msg.linear.x
    #direction = msg.angular.z
    sim.setJointTargetVelocity(sim.getObject("/jointAx3"), speed)
    sim.setJointTargetPosition(sim.getObject("/Pivot_Moteur"), direction)



def sysCall_actuation():
    #p = projection.Proj(proj='utm', zone=10, ellps='WGS84', preserve_units=False)
    global publisher_longitude, publisher_latitude, subscriber_steering, subscriber_speed

    voiture = sim.getObject(".")
    absolutePosition = sim.getObjectPosition(voiture, sim.handle_world)
    noisedPosition = np.array(absolutePosition) + np.random.normal(0,1,2)
    x, y = noisedPosition[0], noisedPosition[1]
    #lat, lon = p(noisedPosition[0], noisedPosition[1])

    if simROS2:
        simROS2.publish(publisher_longitude, {'data':x})
        simROS2.publish(publisher_latitude, {'data':y})

    voitureEulerAngles = sim.getObjectOrientation(voiture)
    capNoised = voitureEulerAngles[2] + np.random.normal(0, 1, 1)[0]
    simROS2.publish(publisher_cap, {'data':capNoised})




def sysCall_cleanup():
    global publisher_front_sonar, subscriber_steering, subscriber_speed
    # Following not really needed in a simulation script (i.e. automatically shut down at simulation end):
    if simROS2:
        simROS2.shutdownPublisher(publisher_front_sonar)



#pour exécuter le noeud
"""
ros2 topic pub --once /direction std_msgs/msg/Float32 "{'data' : -1.0}"
ros2 topic pub --once /direction std_msgs/msg/Float32 "{'data' : 0.0}"
ros2 topic pub --once /speed std_msgs/msg/Float32 "{'data' : 0.5}"
ros2 topic pub --once /speed std_msgs/msg/Float32 "{'data' : 0.0}"
ros2 topic echo /front_sonar
"""