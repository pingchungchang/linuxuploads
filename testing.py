import cv2 as cv
import numpy as np
import imutils

def findyellow(img):
	global t
	hsv = cv.cvtColor(img,cv.COLOR_BGR2HSV)
	dark_yellow = np.array([50,255,255])
	light_yellow = np.array([25,55,55])
	mask = cv.inRange(hsv,light_yellow,dark_yellow)
	output = cv.bitwise_and(img,img,mask = mask)
	return output


def get_center(img)
	img = findyellow(img)
	gray = cv.cvtColor(img,cv.COLOR_BGR2GRAY)
	blurred = cv.GaussianBlur(gray,(5,5),0)
	thresh = cv.threshold(blurred,60,255,cv.THRESH_BINARY)[1]
	cnts = cv.findContours(thresh.copy(),cv.RETR_EXTERNAL,cv.CHAIN_APPROX_SIMPLE)
	cnts = imutils.grab_contours(cnts)
	for c in cnts:
		M = cv.moments(c)
		cx = int(M['m01']/M['m00'])
		cy = int(M['m01']/M['m00'])
		cv.drawContours(img,[c],-1,(0,255,0),2)
		cv.circle(img,(cx,cy),7,(255,255,255),-1)
		cv.putText(img,'center',(cx-20,cy-20),cv.FONT_HERSHEY_SIMPLEX,0.5,(255,255,255),2)
	cv.imshow('img',img)
	cv.waitKey(1)