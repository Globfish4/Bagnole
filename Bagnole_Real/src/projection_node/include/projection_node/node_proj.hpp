#include <proj.h>
#include <rclcpp/rclcpp.hpp>
#include <gpsd_client/msg/gps_fix.hpp>
#include <geometry_msgs/msg/pose_stamped.hpp>

class NodeProj : public rclcpp::Node{
    public:
        NodeProj();
    private:
        void init_interfaces();
        void timer_callback();
        void coord_callback(gpsd_client::msg::GpsFix::SharedPtr msg); //topic '/fix'
        double x_ = 0;
        double y_ = 0;
        // double theta_ = 0;
        double lat_ = 0;
        double lon_ = 0;
        double track_ = 0;
        rclcpp::TimerBase::SharedPtr timer_;
        rclcpp::Subscription<gpsd_client::msg::GpsFix>::SharedPtr subscription_coord_;
        rclcpp::Publisher<geometry_msgs::msg::PoseStamped>::SharedPtr publisher_;
        PJ_CONTEXT *pj_ctx;
        PJ *pj;

        PJ *norm;
        

};