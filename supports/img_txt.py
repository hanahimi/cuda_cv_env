#-*-coding:UTF-8
'''
Created on 2018年9月3日
@author: mipapapa
将图片与TXT相互转换
'''
import cv2
import numpy as np


def img2txt(img_path):
    img = cv2.imread(img_path)
    h,w,_ = img.shape
    fpath = img_path[:-3]+"txt"
    
    with open(fpath,'w') as fw:
        # 将图片分开每个通道保存
        for c in range(3):
            img_split = img[:,:,c]
            for row in range(h):
                img_row = img_split[row,:]
                str_row = " ".join([str(x) for x in img_row]) + "\n"
                fw.write(str_row)

def txt2img(txt_path):
    img_split_data = []
    with open(txt_path,'r') as fr:
        for strlog in fr:
            row_data = np.array([int(x) for x in strlog.strip().split(" ")])
            img_split_data.append(row_data)
    w = len(img_split_data[0])
    h3 = len(img_split_data)
    h = int(h3/3)
    img_split_data = np.array(img_split_data)
    img = np.zeros((h,w,3), np.uint8)
    img[:,:,0] = img_split_data[:h,:]
    img[:,:,1] = img_split_data[h:2*h,:]
    img[:,:,2] = img_split_data[2*h:,:]
    return img
 
def main():
#     img_path = "test.jpg"
#     img2txt(img_path)
    
    txt_path = r"D:\test.txt"
    img = txt2img(txt_path)
    cv2.imshow("s", img)
    cv2.waitKey(0)
    
if __name__=="__main__":
    pass
    main()



