#!/usr/bin/env python

import rospy
from geometry_msgs.msg import Twist 

def move():
	pub = rospy.Publisher("/cmd_vel",Twist,queue_size=10)
	st = Twist()
	st.linear.x = 0.0
	st.linear.y = 0.0
	st.linear.z = 0.0
	st.angular.z = 3.0
	while not rospy.is_shutdown():
		rospy.loginfo(st)
		pub.publish(st)
if __name__ == '__main__':
	rospy.init_node('tester',anonymous = True)
	move()