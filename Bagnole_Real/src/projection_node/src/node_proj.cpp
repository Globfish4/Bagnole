#include "projection_node/node_proj.hpp"

using namespace std::chrono_literals;
using namespace std::placeholders;

NodeProj::NodeProj() : Node("projection_node")
{
    init_interfaces();
    RCLCPP_INFO(this->get_logger(), "node_proj initialized");
}

void NodeProj::init_interfaces(){
    pj_ctx = proj_context_create();
    pj = proj_create_crs_to_crs (pj_ctx,
                                "EPSG:4326",
                                "+proj=utm +zone=30  +datum=WGS84", /* or EPSG:32632 */
                                NULL);
    if (0 == pj) {
        fprintf(stderr, "Failed to create transformation object.\n");
        return;
    }
    
    norm = proj_normalize_for_visualization(pj_ctx, pj);
    if (0 == norm) {
        fprintf(stderr, "Failed to normalize transformation object.\n");
        return;
    }
    publisher_ = this->create_publisher<geometry_msgs::msg::PoseStamped>("/position", 10);
    subscription_coord_ = this->create_subscription<gpsd_client::msg::GpsFix>("/fix", 10, std::bind(&NodeProj::coord_callback, this, _1));
    timer_ = this->create_wall_timer(40ms, std::bind(&NodeProj::timer_callback, this));
    
    // PJ_COORD a, b;
    // a = proj_coord(this -> m_y, this -> m_x, 0, 0);
    // b = proj_trans(P, PJ_FWD, a);
    // this -> m_xutm = b.enu.e;
    // this -> m_yutm = b.enu.n;
}

void NodeProj::coord_callback(gpsd_client::msg::GpsFix::SharedPtr msg)
{
    lat_ = msg->latitude;
    lon_ = msg->longitude;
    track_ = msg->track;
}

void NodeProj::timer_callback()
{
    PJ_COORD a, b;
    a = proj_coord(lon_, lat_ , 0, 0);
    b = proj_trans(pj, PJ_FWD, a);
    this -> x_ = b.enu.e;
    this -> y_ = b.enu.n;
    geometry_msgs::msg::PoseStamped msg;
    msg.header.stamp = this->now();
    // msg.header.frame_id = "map";
    msg.pose.position.x = x_;
    msg.pose.position.y = y_;
    msg.pose.position.z = 0;
    msg.pose.orientation.x = 0;
    msg.pose.orientation.y = 0;
    msg.pose.orientation.z = track_;

    publisher_->publish(msg);

}

int main(int argc, char * argv[]){
    // Initialise ROS 2 pour l'executable
    rclcpp::init(argc, argv);
    // Créer le node et se met en attente de messages ou d'évènements du timer
    
    // Attention, cette fonction est bloquante !
    rclcpp::spin(std::make_shared<NodeProj>());
    // Coupe ROS 2 pour l'executable
    rclcpp::shutdown();
    return 0;
}