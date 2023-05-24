#include <rclcpp/rclcpp.hpp>
#include <visualization_msgs/msg/marker.hpp>
#include <geometry_msgs/msg/pose_stamped.hpp>
#include "std_msgs/msg/string.hpp"
#include "std_msgs/msg/float64.hpp"

#include "tf2/LinearMath/Quaternion.h"
#include "tf2_geometry_msgs/tf2_geometry_msgs.h"
#include <eigen3/Eigen/Dense>
#include <string>
#include <cstdlib>
#include "math.h"
#include <cmath>

using namespace Eigen;
using namespace std::chrono_literals;

class CommandantdeBord : public rclcpp::Node{
    public:
        CommandantdeBord();
        void PoseCallback(const geometry_msgs::msg::PoseStamped::SharedPtr msg);
        void publishCap();

    private:
        rclcpp::Publisher<std_msgs::msg::Float64>::SharedPtr cap_publisher_;
        rclcpp::Subscription<geometry_msgs::msg::PoseStamped>::SharedPtr position_sub_;
        rclcpp::TimerBase::SharedPtr timer_; // objet timer

        double cap_ = 0.;
        const double L = 42.195;
        const double R = 40.0;
        Matrix<double,2,1>c1_{R , 0.};
        Matrix<double,2,1>c2_{-R , 0.};
        Matrix<double,2,1>x_ = Matrix<double,2,1>::Zero();
        void Commande_Cap();
        void Commande_Cap_Droite();
        void Commande_Cap_Gauche();
        void Commande_Cap_Haut();
        void Commande_Cap_Bas();
};