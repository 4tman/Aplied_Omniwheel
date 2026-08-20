#!/usr/bin/env python3
import socket
import time

import rclpy
from rclpy.node import Node
from geometry_msgs.msg import Twist
import time


class CmdVelToUdp(Node):
    def __init__(self):
        super().__init__('cmdvel_to_udp')

        self.declare_parameter('udp_ip', '192.168.42.1')
        self.declare_parameter('udp_port', 5005)
        self.declare_parameter('linear_threshold', 0.05)
        self.declare_parameter('angular_threshold', 0.05)
        self.declare_parameter('command_timeout', 1.0)
        self.declare_parameter('timer_period', 0.1)

        self.udp_ip = self.get_parameter('udp_ip').value
        self.udp_port = self.get_parameter('udp_port').value
        self.linear_threshold = self.get_parameter('linear_threshold').value
        self.angular_threshold = self.get_parameter('angular_threshold').value
        self.command_timeout = self.get_parameter('command_timeout').value
        self.timer_period = self.get_parameter('timer_period').value

        self.sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)

        self.last_cmd_time = time.monotonic()
        self.last_sent_command = None

        self.subscription = self.create_subscription(
            Twist,
            '/cmd_vel',
            self.cmd_vel_callback,
            5
        )

        self.timer = self.create_timer(self.timer_period, self.timer_callback)

        self.get_logger().info(
            f'Sending UDP commands to {self.udp_ip}:{self.udp_port}'
        )
        self.get_logger().info(
            f'Safety timeout: stop if no new command for {self.command_timeout} s'
        )

    def send_command(self, command: str):
        if command != self.last_sent_command:
            self.sock.sendto(command.encode(), (self.udp_ip, self.udp_port))
            self.last_sent_command = command
            self.get_logger().info(f'Sent: {command}')

    def cmd_vel_callback(self, msg: Twist):
        self.last_cmd_time = time.monotonic()

        lin = msg.linear.x
        ang = msg.angular.z

        if lin == 0.0 and ang == 0.0:
            self.send_command('stop')
        elif lin > self.linear_threshold:
            self.send_command('F')
        elif lin < -self.linear_threshold:
            self.send_command('B')
        elif ang > self.angular_threshold:
            self.send_command('L')
        elif ang < -self.angular_threshold:
            self.send_command('R')


    def timer_callback(self):
        elapsed = time.monotonic() - self.last_cmd_time
        if elapsed > self.command_timeout:
            self.send_command('stop')


def main(args=None):
    rclpy.init(args=args)
    node = CmdVelToUdp()
    try:
        rclpy.spin(node)
    except KeyboardInterrupt:
        pass
    finally:
        node.destroy_node()
        rclpy.shutdown()


if __name__ == '__main__':
    main()