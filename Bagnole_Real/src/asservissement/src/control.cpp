//
// Created by camilo on 4/13/23.
//

#include "asservissement/control.h"



CarControler::CarControler():Node("control_node"){
    timer_ = this->create_wall_timer(500ms, std::bind(&CarControler::timer_callback, this));
    pubControl_ = this->create_publisher<geometry_msgs::msg::Twist>("cmd_vel", 10);
    desiredHeading_ = this->create_subscription<std_msgs::msg::Float64>("commande_cap", 10, [this](const std_msgs::msg::Float32::SharedPtr msg) {this->desiredHeadingReceive(msg);});
    actualHeading_ = this->create_subscription<gpsd_client::msg::GpsFix>("/fix", 10, [this](const gpsd_client::msg::GpsFix::SharedPtr msg) {this->actualHeadingReceive(msg);});

    //subTarget_ = this->create_subscription<geometry_msgs::msg::PoseStamped>("/target", 10, [this](const geometry_msgs::msg::PoseStamped::SharedPtr msg) {this->targetReceive(msg);});
}



void CarControler::timer_callback(){
    controler();
}


void CarControler::controler(){
    float cmdHeading = m_K*sawtooth(m_actualHeading- m_desiredHeading);
    float cmdVelocity = 5.0;
    geometry_msgs::msg::Twist cmd_vel;
    cmd_vel.linear.x = cmdVelocity;
    cmd_vel.angular.z = cmdHeading;
    pubControl_->publish(cmd_vel);
}


void CarControler::desiredHeadingReceive(const std_msgs::msg::Float32::SharedPtr msg){
    m_desiredHeading = msg->data;
}


void CarControler::actualHeadingReceive(const gpsd_client::msg::GpsFix::SharedPtr msg){
    m_actualHeading = msg->track;
}





float sawtooth(float x) {
    float sawtooth_value = (180 * (x - floor(0.5 + x)));
    return sawtooth_value;
}






int main(int argc, char * argv[])
{
    rclcpp::init(argc, argv);
    rclcpp::spin(std::make_shared<CarControler>());
    rclcpp::shutdown();

    return 0;
}
