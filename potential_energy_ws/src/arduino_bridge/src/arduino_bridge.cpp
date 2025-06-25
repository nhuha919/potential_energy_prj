#include <rclcpp/rclcpp.hpp>
#include <std_msgs/msg/string.hpp>
#include <boost/asio.hpp>

using boost::asio::ip::tcp;
using namespace std::chrono_literals;

class ArduinoBridgeNode : public rclcpp::Node {
public:
  ArduinoBridgeNode() : Node("arduino_bridge"), io_service_() {
    publisher_ = this->create_publisher<std_msgs::msg::String>("arduino_data", 10);
    timer_ = this->create_wall_timer(1s, std::bind(&ArduinoBridgeNode::read_from_arduino, this));

    try {
      socket_ = std::make_shared<tcp::socket>(io_service_);
      tcp::endpoint endpoint(boost::asio::ip::make_address("172.20.10.2"), 8888);  // replace with Arduino IP
      socket_->connect(endpoint);
      RCLCPP_INFO(this->get_logger(), "Connected to Arduino at 172.20.10.2: 8888"); // replace with Arduino IP
    } catch (std::exception &e) {
      RCLCPP_ERROR(this->get_logger(), "Connection failed: %s", e.what());
    }
  }

private:
  void read_from_arduino() {
    try {
      char buffer[128];
      size_t len = socket_->read_some(boost::asio::buffer(buffer));
      std_msgs::msg::String msg;
      msg.data = std::string(buffer, len);
      publisher_->publish(msg);
    } catch (std::exception &e) {
      RCLCPP_ERROR(this->get_logger(), "Read failed: %s", e.what());
    }
  }

  rclcpp::Publisher<std_msgs::msg::String>::SharedPtr publisher_;
  rclcpp::TimerBase::SharedPtr timer_;
  boost::asio::io_service io_service_;
  std::shared_ptr<tcp::socket> socket_;
};

int main(int argc, char *argv[]) {
  rclcpp::init(argc, argv);
  rclcpp::spin(std::make_shared<ArduinoBridgeNode>());
  rclcpp::shutdown();
  return 0;
}
