#!/usr/bin/env python
from sensor_msgs.msg import Image
import cv2 as cv
import rospy
from cv_bridge import CvBridge,CvBridgeError
from geometry_msgs.msg import Twist
import numpy as np
from sensor_msgs.msg import CompressedImage
import imutils
import time

mv = Twist()
mv.linear.x = 0.2
lrot = Twist()
lrot.angular.z = 0.05
rrot = Twist()
rrot.angular.z = -0.05
stp = Twist()
now = stp
pub = rospy.Publisher("/cmd_vel",Twist,queue_size=1)
t = 1
constantt = 11.5
action = [time.time(),'stop']
globalimg = []
def findyellow(img):
	hsv = cv.cvtColor(img,cv.COLOR_BGR2HSV)
	dark_yellow = np.array([50,255,255])
	light_yellow = np.array([25,55,55])
	mask = cv.inRange(hsv,light_yellow,dark_yellow)
	output = cv.bitwise_and(img,img,mask = mask)
	# rospy.loginfo("yellow:")
	# rospy.loginfo(np.sum(output))
	return output

def findwhite(img):
	hsv = cv.cvtColor(img,cv.COLOR_BGR2HSV)
	dark_white = np.array([255,0,255])
	light_white = np.array([0,0,179])
	mask = cv.inRange(hsv,light_white,dark_white)
	output = cv.bitwise_and(img,img,mask = mask)
	# rospy.loginfo('white:')
	# rospy.loginfo(np.sum(output))
	return output
def get_y_center(img):
	img = findyellow(img)
	gray = cv.cvtColor(img,cv.COLOR_BGR2GRAY)
	blurred = cv.GaussianBlur(gray,(5,5),0)
	thresh = cv.threshold(blurred,60,255,cv.THRESH_BINARY)[1]
	cnts = cv.findContours(thresh.copy(),cv.RETR_EXTERNAL,cv.CHAIN_APPROX_SIMPLE)
	cnts = imutils.grab_contours(cnts)
	for c in cnts:
		M = cv.moments(c)
		cx = int(M['m10']/M['m00'])
		cy = int(M['m01']/M['m00'])
		cv.drawContours(img,[c],-1,(0,255,0),2)

		# if(np.sum(img)<100000):
		# 	break

		return [img,cx+10,cy+10]
	return [img,-1,-1]
	# return [img,0,0]
def get_w_center(img):
	img = findwhite(img)
	gray = cv.cvtColor(img,cv.COLOR_BGR2GRAY)
	blurred = cv.GaussianBlur(gray,(5,5),0)
	thresh = cv.threshold(blurred,60,255,cv.THRESH_BINARY)[1]
	cnts = cv.findContours(thresh.copy(),cv.RETR_EXTERNAL,cv.CHAIN_APPROX_SIMPLE)
	cnts = imutils.grab_contours(cnts)
	for c in cnts:
		M = cv.moments(c)
		cx = int(M['m10']/M['m00'])
		cy = int(M['m01']/M['m00'])
		cv.drawContours(img,[c],-1,(0,255,0),2)
		if cx<50:
			break

		# if(np.sum(img)<100000):
		# 	break

		return [img,cx-10,cy-10]
	return [img,-1,-1]
	# return [img,len(img[0]),50]
def findcol(img):
	cen = 130
	yc = get_y_center(img)
	wc = get_w_center(img)
	cx = 0
	cy = 0
	if(wc[1] == -1):
		cx = yc[1]+cen
		cy = yc[2]
	elif yc[1] == -1:
		cx = wc[1]-cen
		cy = wc[2]
	else:
		cx = (wc[1]+yc[1])/2
		cy = (wc[2]+yc[2])/2
	cv.circle(wc[0],(cx,cy),7,(255,0,255),2)
	return [yc[0]+wc[0],cx,cy]
def mvments(mx,cx):
	global mv
	if cx >= mx-7.5 and cx <= mx+7.5:
		pub.publish(mv)
		return 'move'
	else:
		k = Twist()
		k.angular.z = -0.1
		k.angular.z = (cx-mx+1)/constantt*k.angular.z
		pub.publish(k)
		return str(k.angular.z)
	return 'move'
def mvments2(mx,cx):
	rospy.loginfo(str(mx)+str(cx))
	global mv
	if cx >= mx-20 and cx <= mx+20:
		if action[0]>time.time():
			return 'move'
		pub.publish(mv)
		return 'move'
	else:
		k = Twist()
		k.angular.z = -0.1
		k.angular.z = (cx-mx+1)/constantt*k.angular.z
		# rospy.loginfo(k.angular.z)
		if action[0]>time.time():
			return str(k.angular.z)
		pub.publish(k)
		return str(k.angular.z)
	return 'move'
flag = 'move'
def parking(inp):
	# rospy.loginfo('in')
	if inp.angular.z == 0:
		return
	elif flag == 'move':
		flag = 'find_line'
	global stp
	global flag
	global mv
	global globalimg
	img = globalimg[110:]
	rrott = Twist()
	rrott.angular.z = -0.4
	lrott = Twist()
	lrott.angular.z = 0.4
	rospy.loginfo(flag)
	# if flag[:3] == 'fff':
	# 	if float(flag[3:])<time.time():
	# 		flag = 'ttt'+str(time.time()+3.75)
	# 	else:
	# 		pub.publish(mv)
	# if flag[:3] == 'ttt':
	# 	if float(flag[3:])<time.time():
	# 		flag = 'forward'
	# 	else:
	# 		pub.publish(lrott)
	if flag == 'forward':
		flag = 'x'+str(time.time()+2)
		pub.publish(mv)
	if flag[0] == 'x':
		if time.time()>float(flag[1:]):
			flag = 'rrot'
		else:
			pub.publish(mv)
	if flag == 'rrot':
		flag  ='y'+str(time.time()+3.5)
		pub.publish(rrot)
	if flag[0] == 'y':
		if time.time()>float(flag[1:]):
			flag = 'in'
		else:
			pub.publish(rrott)
	if flag == 'in':
		flag = 'z'+str(time.time()+1.65)
		pub.publish(mv)
	if flag[0] == 'z':
		if time.time()>float(flag[1:]):
			rospy.loginfo('stopped')
			pub.publish(stp)
		else:
			pub.publish(mv)

	# img = findwhite(img)
	img = findwhite(img[:,len(img[0])/2:len(img[0])*4//5])
	mx = len(img[0])/2
	gray = cv.cvtColor(img,cv.COLOR_BGR2GRAY)
	blurred = cv.GaussianBlur(gray,(5,5),0)
	thresh = cv.threshold(blurred,60,255,cv.THRESH_BINARY)[1]
	cnts = cv.findContours(thresh.copy(),cv.RETR_EXTERNAL,cv.CHAIN_APPROX_SIMPLE)
	cnts = imutils.grab_contours(cnts)
	cv.imshow('hi',img)
	cv.waitKey(1)
	if flag[:3] == 'ttt' or flag[:3] == 'fff' or flag == 'forward':
		return
	arr = []
	for c in cnts:
		M = cv.moments(c)
		if M['m00'] == 0:
			continue
		cx = int(M['m10']/M['m00'])
		cy = int(M['m01']/M['m00'])
		cv.drawContours(img,[c],-1,(0,255,0),2)
		arr.append([cx,cy,cv.contourArea(c),c])
	rospy.loginfo(len(cnts))
	if len(cnts)>=5 and flag == 'find_line':
		# flag = 'change_sight'
		flag = 'forward'
	rospy.loginfo(len(img[0]))
	if flag[0] == 'z' and len(cnts)<=1 and arr[0][1]>100:
		flag = 'stop'
		pub.publish(stp)
	# if flag != 'park' and len(cnts)<3:
	# 	pub.publish(stp)
	# 	'stopped'
	# 	return
	# elif flag == 'stop':
	# 	return
	# elif flag == 'move':
	# 	flag = 'park'
	# arr.sort(key = lambda x:x[2])
	# if len(arr)>1:
	# 	arr.pop()
	# if len(arr)>1:
	# 	arr.pop()
	# nparr = np.array(arr)
	# npx = nparr[:,0]*nparr[:,2]
	# npy = nparr[:,1]*nparr[:,2]
	# centerx = int(np.sum(npx)/np.sum(nparr[:,2]))
	# centery = int(np.sum(npy)/np.sum(nparr[:,2]))
	# # cv.circle(img,(centerx,centery),4,(255,0,0),2)
	# cv.imshow('hi',img)
	# cv.waitKey(1)
	# cv.drawContours(img,[nparr[len(nparr)//2][3]],-1,(255,255,0),2)
	# # rospy.loginfo(nparr[-1,3][0])
	# line = cv.fitLine(nparr[-1,3],cv.DIST_L2,0,0.01,0.01)#1.3 to 1.5
	# cv.circle(img,tuple(nparr[-1,3][0][0]),4,(255,0,0),3)
	# rospy.loginfo(line[0]/line[1])
	# rospy.loginfo(flag)
	# if flag == 'park':
	# 	if line[0]/line[1]>=1 and line[0]/line[1]<=1.5:
	# 		pub.publish(stp)
	# 		flag = 'forward'
	# 		rospy.loginfo('rot done')
	# 	elif line[0]/line[1] >1.5:
	# 		pub.publish(rrott)
	# 		rospy.loginfo('r')
	# 	else:
	# 		pub.publish(lrott)
	# 		rospy.loginfo('l')
	return

def func(inp):
	bridge = CvBridge()
	global now
	global t
	global rrot
	global mv
	global globalimg
	global flag
	st = rospy.get_time()
	try:
		img = bridge.imgmsg_to_cv2(inp,'bgr8')#img is np.array
		# parking(img[110:])
		# return
		globalimg = img
		if flag != 'move' and flag != 'find_line':
			return
		filt = findcol(img[190:])
		cx = filt[1]
		cy = filt[2]
		mx = len(filt[0][0])/2
		cv.circle(filt[0],(mx,cy),4,(0,255,255),2)
		# cv.imshow('whatever',filt[0])
		mvments(mx,cx)
		# cv.waitKey(1)
	except CvBridgeError:
		rospy.loginfo('error')
	# rospy.loginfo("time:")
	ed = rospy.get_time()
	# rospy.loginfo(ed-st)
	# rospy.loginfo(sum(img))


if __name__ == "__main__":
	rospy.loginfo(cv.__version__)
	rospy.init_node("image_receiver")
	rate = rospy.Rate(100)
	rospy.Subscriber('parking',Twist,parking,queue_size = 1)
	rospy.Subscriber('/camera/image',Image,func,queue_size = 1)
	while not rospy.is_shutdown():
		rate.sleep()