#!/usr/bin/env python
import rospy
import cv2 as cv
import numpy as np
from matplotlib import pyplot as plt
from sensor_msgs.msg import Image
from cv_bridge import CvBridge,CvBridgeError
from geometry_msgs.msg import Twist
k = 1
pk = False
stp = False
stopimg = cv.imread('stopsign.jpg')
parkimg = cv.imread('park3.jpg')
# cv.imshow('stopsign',stopimg)
# cv.waitKey(0)
pub = rospy.Publisher("parking",Twist,queue_size=1)
parkk = Twist()
gstopimg = cv.cvtColor(stopimg,cv.COLOR_BGR2GRAY)
gparkimg = cv.cvtColor(parkimg,cv.COLOR_BGR2GRAY)
sift = cv.xfeatures2d.SIFT_create()
kpstop,desstop = sift.detectAndCompute(gstopimg,None)
kp1,des1 = sift.detectAndCompute(gparkimg,None)
def get_park_sign(img):
	hsv = cv.cvtColor(img,cv.COLOR_BGR2HSV)
	dark_blue = np.array([115,255,255])
	light_blue = np.array([80,50,55])
	mask = cv.inRange(hsv,light_blue,dark_blue)
	output = cv.bitwise_and(img,img,mask = mask)
	return output

def getsign(inp):
	bridge = CvBridge()
	try:
		global k
		img = bridge.imgmsg_to_cv2(inp,'bgr8')#img is np.array
		filt = get_park_sign(img)
		cv.imshow('whatever',filt)
		cv.waitKey(1)
		if np.sum(filt)>=805053:
			cv.imwrite('parksigns/parkphoto'+str(k)+'.jpg',img)
			k+=1
			rospy.loginfo('saved')
		else:
			rospy.loginfo(np.sum(filt))
	except CvBridgeError:
		rospy.loginfo('error')


def findparksign(inp):
	global sift,kp1,des1,gparkimg,parkk
	# rospy.loginfo('entered')
	bridge = CvBridge()
	try:
		img = bridge.imgmsg_to_cv2(inp,'bgr8')
		gimg = cv.cvtColor(img,cv.COLOR_BGR2GRAY)
		kp2,des2 = sift.detectAndCompute(gimg,None)

		bf = cv.BFMatcher()
		matches = bf.knnMatch(des1,des2,k=2)
		good = []
		for m,n in matches:
			if m.distance<0.75*n.distance:
				good.append([m])

		global pk,parkk,pub
		# rospy.loginfo('park sim:'+str(len(good)))
		if len(good)>=75 or pk == True:
			rospy.loginfo('park')
			parkk.angular.z = 30
			pub.publish(parkk)
			pk = True
		pub.publish(parkk)
	except CvBridgeError:
		rospy.loginfo('error')

def findstopign(inp):
	global sift,kpstop,desstop,stopimg
	# rospy.loginfo('entered')
	bridge = CvBridge()
	try:
		img = bridge.imgmsg_to_cv2(inp,'bgr8')
		gimg = cv.cvtColor(img,cv.COLOR_BGR2GRAY)
		kp2,des2 = sift.detectAndCompute(gimg,None)

		bf = cv.BFMatcher()
		matches = bf.knnMatch(desstop,des2,k=2)
		good = []
		for m,n in matches:
			if m.distance<0.75*n.distance:
				good.append([m])

		global stp
		# rospy.loginfo("stop sim:"+str(len(good)))
		if len(good)>=40 or stp == True:
			rospy.loginfo('stop')
			stp = True
		# if(len(good) == 0):
			# rospy.loginfo('no matches')
		# else:
			# img3 = cv.drawMatchesKnn(gimg,kp1,gparkimg,kp2,good,flags=2)
			# plt.imshow(img3,),plt.show()
	except CvBridgeError:
		rospy.loginfo('error')

def func(inp):
	findparksign(inp)
	findstopign(inp)
if __name__ == "__main__":
	global parkk
	rospy.init_node("parksignnfinder")

	rospy.loginfo(cv.__version__)

	# cv.imshow('hi',gparkimg)
	# cv.waitKey(100)

	rospy.Subscriber('/camera/image',Image,func,queue_size = 1)
	rate = rospy.Rate(100)
	while not rospy.is_shutdown():
		rate.sleep()