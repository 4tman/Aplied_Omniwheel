#!/usr/bin/env python3

import os
import threading
import time

import cv2
from cv_bridge import CvBridge
import rclpy
from rclpy.node import Node
from sensor_msgs.msg import Image


class RtspCameraNode(Node):
    def __init__(self):
        super().__init__('trackingcam_camera')

        self.declare_parameter(
            'stream_url',
            'rtsp://192.168.42.1:554/camera720p',
        )
        self.declare_parameter('topic', '/trackingcam/image_raw')
        self.declare_parameter('frame_id', 'trackingcam_optical_frame')
        self.declare_parameter('fps', 15.0)

        self.stream_url = self.get_parameter(
            'stream_url'
        ).get_parameter_value().string_value
        self.topic = self.get_parameter(
            'topic'
        ).get_parameter_value().string_value
        self.frame_id = self.get_parameter(
            'frame_id'
        ).get_parameter_value().string_value
        self.fps = self.get_parameter(
            'fps'
        ).get_parameter_value().double_value

        self.publisher = self.create_publisher(Image, self.topic, 10)
        self.bridge = CvBridge()
        self.frame_lock = threading.Lock()
        self.latest_frame = None
        self.running = True

        os.environ.setdefault(
            'OPENCV_FFMPEG_CAPTURE_OPTIONS',
            'rtsp_transport;tcp'
        )

        self.thread = threading.Thread(
            target=self.capture_loop,
            daemon=True,
        )
        self.thread.start()

        period = 1.0 / max(self.fps, 0.1)
        self.timer = self.create_timer(period, self.publish_frame)

        self.get_logger().info(f'Opening stream: {self.stream_url}')
        self.get_logger().info(f'Publishing: {self.topic}')

    def capture_loop(self):
        while self.running and rclpy.ok():
            cap = cv2.VideoCapture(self.stream_url, cv2.CAP_FFMPEG)

            if not cap.isOpened():
                self.get_logger().warn(
                    'Cannot open stream; retrying in 2 seconds.'
                )
                time.sleep(2.0)
                continue

            self.get_logger().info('Video stream connected.')

            while self.running and rclpy.ok():
                ok, frame = cap.read()

                if not ok or frame is None:
                    self.get_logger().warn(
                        'Video frame read failed; reconnecting.'
                    )
                    break

                with self.frame_lock:
                    self.latest_frame = frame

            cap.release()
            time.sleep(1.0)

    def publish_frame(self):
        with self.frame_lock:
            if self.latest_frame is None:
                return
            frame = self.latest_frame.copy()

        msg = self.bridge.cv2_to_imgmsg(frame, encoding='bgr8')
        msg.header.stamp = self.get_clock().now().to_msg()
        msg.header.frame_id = self.frame_id
        self.publisher.publish(msg)

    def destroy_node(self):
        self.running = False
        if self.thread.is_alive():
            self.thread.join(timeout=3.0)
        super().destroy_node()



def main(args=None):
    rclpy.init(args=args)
    node = RtspCameraNode()

    try:
        rclpy.spin(node)
    except KeyboardInterrupt:
        pass
    finally:
        node.destroy_node()
        rclpy.shutdown()

if __name__ == '__main__':
    main()