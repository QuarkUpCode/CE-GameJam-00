from PIL import Image

FILE_SPRITESHEET = "game/assets/spritesheet-2bit.png"

FILE_OUTPUT_DATA = "game/graphics.owo"

img:Image.Image = Image.open(FILE_SPRITESHEET)

data:list[int] = []

TILESIZE = 16
TILES_X = 4
TILES_Y = 4

for sy in range(TILES_Y):
	for sx in range(TILES_X):
		for y in range(TILESIZE):
			for x in range(TILESIZE):
				data.append(int(img.getpixel((int((sx*TILESIZE) + x), int((sy*TILESIZE) + y)))) - 1)

print(data)

pal = [0x00000000, 0xd95763ff, 0x639bffff, 0xffffffff,
	   0x00000000, 0x404040ff, 0x808080ff, 0xffffffff,
	   0x000000ff, 0x404040ff, 0x808080ff, 0xffffffff,
	   0x00000000, 0x808080ff, 0xfbf236ff, 0xffffffff]

with open(FILE_OUTPUT_DATA, 'wb') as f:
	b = b""
	
	for e in pal:
		b += e.to_bytes(4, 'big')

	for e in data:
		b += e.to_bytes(1, 'little')
	
	f.write(b)
