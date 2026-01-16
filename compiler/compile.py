
FILE_GAME = "game/game.uwu"
FILE_GRAPHICS = "game/graphics.uwu"
FILE_OPCODES_H = "emulator/include/opcodes.h"

game:str = ""
with open(FILE_GAME, 'r') as f:
	game = f.read()


# not sure yet if its gonna be string or binary data
graphics = ""
with open(FILE_GRAPHICS, 'r') as f:
	graphics = f.read()

opcodes:dict[str, int] = {}
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

		opcodes[l[3:].lower()] = i
		i+=1

print(opcodes)

withoperands:dict[str, int] = {"save":2, "restore":2, "ldd":2, "ldx":1, "leap":1, "call":2}
for e in withoperands.keys():
	if not (e in opcodes):
		print(f"!!! A 'withoperands' opcode was not found in opcodes : {e} !!!")
		exit(0)

# withoperands = [(opcodes[e[0]], e[1]) for e in withoperands]


game_lines:list[str] = game.split('\n')
game_lines = [l.split(';')[0] for l in game_lines]
game_lines = [l for l in game_lines if len(l)>0]

macros:dict[str, str] = {}
for i in range(len(game_lines)):
	l = game_lines[i]
	if l.startswith("macro"):
		m = l.split(' ')[1]
		v = l.split(' ')[2]
		macros[m] = v
		for j in range(i+1, len(game_lines)):
			game_lines[j] = game_lines[j].replace(m, v)

print(macros)



labels:dict[str, int] = {}
labeluses:dict[str, list[int]] = {}
datasectors:dict[str, list[int]] = {}	#like #graphics or #hw

code:list[int] = []

index = 0

for i in range(len(game_lines)):
	l = game_lines[i]
	op = l.split(' ')[0]
	if not (op in opcodes.keys()):
		if(op.startswith('.')): labels[op[1:]] = index
		print(f"Couldn't match opcode for lines {i} : {l}")
		continue
	
	oc = opcodes[op]
	code.append(oc)
	index += 1
	if(op in withoperands.keys()):
		operand = l.split(' ')[1]
		
		if operand.startswith("#"):
			if operand in datasectors.keys():
				datasectors[operand].append(index)
			else:
				datasectors[operand] = [index]
			for j in range(withoperands[op]):
				code.append(42)
		
		elif operand.startswith("@"):
			if operand[1:] in labeluses.keys():
				labeluses[operand[1:]].append(index)
			else:
				labeluses[operand[1:]] = [index]
			
			code.append(69)
			code.append(69)

		else:
			n = 0x00
			if operand.startswith("$"):
				n = int(operand[1:], 16) & (~0b1100_0000_0000_0000)
			
			if(withoperands[op] == 2):
				code.append(n&0xFF)
				code.append(n>>8)
			else:
				code.append(n&0x7F)

		index += withoperands[op]


for k in labeluses:
	for i in labeluses[k]:
		code[i] = labels[k]&0xFF
		code[i+1] = (labels[k]&(~0b1100_0000_0000_0000))>>8


print(code)
