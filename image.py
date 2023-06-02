import numpy as np
from PIL import Image

W = 20
H = 20

with open("data2.txt") as f:
    text = f.read()

pixels = [i.split() for i in text.split("\n")]

pixels = [(int(i[0]), int(i[1]), int(i[2])) for i in pixels]

arr = np.array(pixels).reshape(W,H,3)
shape = arr.shape
new_arr = np.empty((shape[0], shape[1], 3), dtype=np.uint8)
for y, row in enumerate(arr):
    for x, color in enumerate(row):
        new_arr[y, x] = np.array(color)

new_arr[0:H:2] = np.flip(new_arr[0:H:2], 1)

Image.frombytes("RGB", (W,H), new_arr).show()