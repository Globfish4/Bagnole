from launch import LaunchDescription
from launch_ros.actions import Node

def generate_launch_description():
    control = Node(
        package='asservissement',
        namespace='',
        executable='control',
        name= 'node_control')
    
    gpsd = Node(
        package='gpsd_client',
        namespace='',
        executable='gpsd_client',
        name= 'node_gpsd_client')
    
    projection = Node(
        package='projection_node',
        namespace='',
        executable='projection_node',
        name= 'node_projection')
    
    motor = Node(
        package='voiture2a_motors_driver',
        namespace='',
        executable='voiture2a_motors_driver',
        name= 'node_motors')
    commande = Node(
        package='commandant',
        namespace='',
        executable='commandant_node',
        name= 'commandant_node')
    
    return LaunchDescription([control, gpsd, projection, motor ,commande])
