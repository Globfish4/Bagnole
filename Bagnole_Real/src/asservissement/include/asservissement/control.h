//
// Created by camilo on 4/13/23.
// Ici on publie un Twist cmd_vel et on s'abone à
//

#ifndef VOITURE2A_ROS_CONTROL_H
#define VOITURE2A_ROS_CONTROL_H


#include <chrono>
#include <functional>
#include <memory>
#include <string>
#include "rclcpp/rclcpp.hpp"
#include "std_msgs/msg/float32.hpp"
#include "geometry_msgs/msg/twist.hpp"
#include "gpsd_client/msg/gps_fix.hpp"



// Définition des Quaternions
//#include "tf2/LinearMath/Quaternion.h"
// Fonctions pour passer des Quaternions aux messages
//#include "tf2_geometry_msgs/tf2_geometry_msgs.h"

//#include <eigen3/Eigen/Dense>

//using namespace Eigen;
using namespace std::chrono_literals;
using std::placeholders::_1;

/* This example creates a subclass of Node and uses std::bind() to register a
* member function as a callback from the timer. */

class CarControler : public rclcpp::Node{
public:
    CarControler();

private:

    float m_actualHeading;
    float m_desiredHeading;
    float cmdHeading;
    float cmdVelocity;
    float m_K;

    void timer_callback();
    void controler();
    void desiredHeadingReceive(const std_msgs::msg::Float32::SharedPtr msg);
    void actualHeadingReceive(const gpsd_client::msg::GpsFix::SharedPtr msg);

    rclcpp::TimerBase::SharedPtr timer_;
    rclcpp::Publisher<geometry_msgs::msg::Twist>::SharedPtr pubControl_;
    rclcpp::Subscription<std_msgs::msg::Float32>::SharedPtr desiredHeading_;
    rclcpp::Subscription<gpsd_client::msg::GpsFix>::SharedPtr actualHeading_;

};


#endif //VOITURE2A_ROS_CONTROL_H
