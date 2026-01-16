
FILE_GAME = "game/game.uwu"
FILE_GRAPHICS = "game/graphics.uwu"
FILE_OPCODES_H = "emulator/include/opcodes.h"

game = ""
with open(FILE_GAME, 'r') as f:
	game = f.read()

graphics = ""
with open(FILE_GRAPHICS, 'r') as f:
	graphics = f.read()

opcodes = {}
with open(FILE_OPCODES_H, 'r') as f:
	lines = f.readlines()
	lines = [l.strip() for l in lines]
	lines = [l.split(',')[0] for l in lines]
	
	i = 0
	for l in lines:
		if not l.startswith("OC_"):
			continue
		if '=' in l:
			i = int(l.split('=')[1])
			l = l.split('=')[0]

		opcodes[l.lstrip("OC_").lower()] = i
		i+=1

print(opcodes)


game_lines = game.split('\n')
game_lines = [l.split(';')[0] for l in game_lines]
game_lines = [l for l in game_lines if len(l)>0]

macros = {}
for i in range(len(game_lines)):
	l = game_lines[i]
	if l.startswith("macro"):
		m = l.split(' ')[1]
		v = l.split(' ')[2]
		macros[m] = v
		for j in range(i+1, len(game_lines)):
			game_lines[j] = game_lines[j].replace(m, v)

print(macros)
