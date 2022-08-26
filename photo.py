#!/usr/bin/env python
from sensor_msgs.msg import Image
import cv2 as cv
import rospy
from cv_bridge import CvBridge,CvBridgeError
from geometry_msgs.msg import Twist
import numpy as np
from sensor_msgs.msg import CompressedImage

mv = Twist()
mv.linear.x = 0.1
lrot = Twist()
lrot.angular.z = 0.1
rrot = Twist()
rrot.angular.z = -0.1
stp = Twist()
now = stp
pub = rospy.Publisher("/cmd_vel",Twist,queue_size=1)
cv.namedWindow("img", cv.WINDOW_NORMAL)
t = 1

def findyellow(img):
	global t
	hsv = cv.cvtColor(img,cv.COLOR_BGR2HSV)
	dark_yellow = np.array([50,255,255])
	light_yellow = np.array([25,55,55])
	mask = cv.inRange(hsv,light_yellow,dark_yellow)
	output = cv.bitwise_and(img,img,mask = mask)
	cv.imshow('filtered',output)
	cv.waitKey(1)
	rospy.loginfo(np.sum(output))
	if np.sum(output) != 0:
		rospy.loginfo('found yellow')
		if np.sum(output)>=999999:
			t = 2
		return True
	else:
		rospy.loginfo('no yellow')
		return False

def findwhite(img):
	global t
	hsv = cv.cvtColor(img,cv.COLOR_BGR2HSV)
	dark_white = np.array([255,0,255])
	light_white = np.array([0,0,179])
	mask = cv.inRange(hsv,light_white,dark_white)
	output = cv.bitwise_and(img,img,mask = mask)
	cv.imshow('filtered',output)
	cv.waitKey(1)
	rospy.loginfo(np.sum(output))
	if np.sum(output) != 0:
		rospy.loginfo('found white')
		if np.sum(output)>=999999:
			t = 1
		return True
	else:
		rospy.loginfo('no white')
		return False	

def findcol(img):
	global t
	if t == 1:
		return findyellow(img)
	else:
		return findwhite(img)
def func(inp):
	bridge = CvBridge()
	global now
	global t
	st = rospy.get_time()
	try:
		img = bridge.imgmsg_to_cv2(inp,'bgr8')#img is np.array
		if findcol(img[170:]) == True:
			pub.publish(mv)
		else:
			if t == 1:
				pub.publish(lrot)
			else:
				pub.publish(rrot)
	except CvBridgeError:
		rospy.loginfo('error')
	ed = rospy.get_time()
	rospy.loginfo(ed-st)


if __name__ == "__main__":
	rospy.init_node("image_receiver")
	rate = rospy.Rate(100)
	rospy.Subscriber('/camera/image',Image,func,queue_size = 1)
	while not rospy.is_shutdown():
		rate.sleep()