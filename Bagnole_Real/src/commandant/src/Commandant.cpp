#include <commandant/Commandant.hpp>

CommandantdeBord::CommandantdeBord() : Node("command_node")
{
  cap_publisher_ = this->create_publisher<std_msgs::msg::Float64>("commande_cap",10);
  position_sub_ = this->create_subscription<geometry_msgs::msg::PoseStamped>("position",10,
                                                                           std::bind(&CommandantdeBord::PoseCallback,this,std::placeholders::_1));
  timer_ = this->create_wall_timer(40ms, std::bind(&CommandantdeBord::publishCap, this));
}

void CommandantdeBord::publishCap(){
    auto message = std_msgs::msg::Float64();
    this->Commande_Cap();
    message.data = cap_;
    cap_publisher_->publish(message);
}
void CommandantdeBord::Commande_Cap(){
    if(x_(0) > L){
        this->Commande_Cap_Droite();
    }
    else if(x_(0) < -L)
    {
        this->Commande_Cap_Gauche();
    }
    else{
        if(x_(1) > 0.){
            this->Commande_Cap_Haut();
        }
        else{
            this->Commande_Cap_Bas();
        }
    }
}
void CommandantdeBord::Commande_Cap_Droite(){
    double p1,p2,q1,q2;
    q1 = (x_(0) - c1_(0))/R;
    q2 = (x_(1) - c1_(1))/R;
    p1 = -(q1*q1*q1 + q2*q2*q1 - q1 + q2);
    p2 = -(q2*q2*q2 + q1*q1*q2 - q1 - q2);
    cap_ = atan2(p2,p1);

}

void CommandantdeBord::Commande_Cap_Gauche(){
    double p1,p2,q1,q2;
    q1 = (x_(0) - c2_(0))/R;
    q2 = (x_(1) - c2_(1))/R;
    p1 = -(q1*q1*q1 + q2*q2*q1 - q1 + q2);
    p2 = -(q2*q2*q2 + q1*q1*q2 - q1 - q2);
    cap_ = atan2(p2,p1);
}

void CommandantdeBord::Commande_Cap_Haut(){
    double p1,p2;
    p1 = -x_(1);
    p2 = - atan(x_(1) - R);
    cap_ = atan2(p2,p1);
}

void CommandantdeBord::Commande_Cap_Bas(){
    double p1,p2;
    p1 = -x_(1);
    p2 = - atan(x_(1) + R);
    cap_ = atan2(p2,p1);
}

void CommandantdeBord::PoseCallback(const geometry_msgs::msg::PoseStamped::SharedPtr msg){
    x_(0) = msg->pose.position.x;
    x_(1) = msg->pose.position.y;
}

int main(int argc , char ** argv){
    rclcpp::init(argc,argv);
    rclcpp::spin(std::make_shared<CommandantdeBord>());
    rclcpp::shutdown();
}
