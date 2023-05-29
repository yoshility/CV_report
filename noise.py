import cv2
import numpy as np

img = cv2.imread('./images/Lenna.bmp') # パスは適宜変える

# add gaussian noise
mean = 0
sigma = 10
noise = np.random.normal(0, sigma, np.shape(img))
img_noised = img + noise
img_noised[img_noised > 255] = 255
img_noised[img_noised < 0] = 0
cv2.imwrite('./images/Lenna_noise.jpg', img_noised)