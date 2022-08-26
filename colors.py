import cv2
import numpy as np

def findyellow(arr):
	img = cv2.imread('parksigns/'+arr)
	hsv = cv2.cvtColor(img,cv2.COLOR_BGR2HSV)
	# dark_yellow = np.array([30,255,255])
	# light_yellow = np.array([25,55,55])
	# dark_white = np.array([255,0,255])
	# light_white = np.array([0,0,179])
	dark_blue = np.array([115,255,255])
	light_blue = np.array([80,50,55])
	mask = cv2.inRange(hsv,light_blue,dark_blue)
	output = cv2.bitwise_and(img,img,mask = mask)
	print(type(img))
	cv2.imshow("Color Detected",np.hstack((img,output)))
	cv2.waitKey(0)
	cv2.destroyAllWindows()
if(__name__ == '__main__'):
	findyellow('rainbow.jpg')
# range: h:0 to 180,s,v:0 to 255