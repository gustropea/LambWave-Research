from PIL import Image

img = Image.new("RGB", (960, 720),"white")
img1= Image.open("65.png")
img2=Image.open("7.png")
img.paste(img1,(0,0))
img.paste(img2,(0,360))
img.save("17000.png") 
