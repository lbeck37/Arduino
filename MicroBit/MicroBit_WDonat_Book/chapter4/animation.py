from microbit import *

a = Image('11111:14441:14941:14441:11111:')
b = Image('14441:14941:14441:11111:00000:')
c = Image('14941:14441:11111:00000:00000:')
d = Image('14441:11111:00000:00000:00000:')
e = Image('11111:00000:00000:00000:00000:')
f = Image('00000:00000:00000:00000:00000:')

h = Image('00000:11111:14441:14941:14441:')
i = Image('00000:00000:11111:14441:14941:')
j = Image('00000:00000:00000:11111:14441:')
k = Image('00000:00000:00000:00000:11111:')

pictureList = [a, b, c, d, e, f, e, d, c, b, a, h, i, j, k, f, k, j, i, h]

display.show(pictureList, delay=200, loop=True)