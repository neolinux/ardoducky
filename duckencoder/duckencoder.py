# TODO:
# - Variables
# - Labels/Jumps/Loops

import sys

# Check args
if len(sys.argv) != 2:
    print("Usage: duckencoder.py <script>")
    exit()

# Open duckyscript
inf = open(sys.argv[1], "r")
keys = [] # Key data
                # 0 -> Modifier key input
                # 1 - 250 -> String input
                # 251 -> Number delay

numbers = [] # Used to store ints

# Modifiers
mods = [
    "CONTROL",
    "SHIFT",
    "ALT",
    "GUI",
    "LEFT",
    "DOWN",
    "UP",
    "RIGHT",
    "BACKSPACE",
    "RETURN",
    "ESCAPE",
    "DELETE",
    "TAB"
]

aliases = {
    "CTRL": "CONTROL",
    "WINDOWS": "GUI",
    "ENTER": "RETURN",
    "BACK": "BACKSPACE"
}

# Previous data length
preLen = 0
noReplay = True

# Read lines
for line in inf:
    # Remove newlines
    line = line.replace("\n", "").replace("\r", "")

    # Line too short?
    if len(line) < 1:
        continue

    # Split
    f = line.find(' ')
    if f < 0:
        cmd = line
        arg = ""
    else:
        cmd = line[:f]
        arg = line[f + 1:]

    # Aliases
    if cmd in aliases:
        cmd = aliases[cmd]

    # Resets
    if cmd != "REPLAY":
        noReplay = False
        preLen = len(keys)

    # Commands
    if cmd == "DELAY":
        keys.append(251)
        numbers.append(int(arg))
        noReplay = True

    elif cmd == "REM":
        print(">>> " + arg)

    elif cmd == "STRING":
        while len(arg) > 0:
            sub = arg[:250]
            keys.append(len(sub))
            for c in sub:
                keys.append(ord(c))

            arg = arg[250:]

    elif cmd == "REPLAY":
        if noReplay: 
            print("]]] ERROR: That command cannot be replayed!");

        sub = keys[preLen:]
        for i in range(int(arg)):
            keys.extend(sub)

    elif cmd in mods:
        keys.append(0)
        keys.append(mods.index(cmd))
        keys.append(ord(arg[0]) if len(arg) > 0 else 0)

    else:
        print("]]] WARNING: Unhandled command '" + cmd + "'")


# Close input file
inf.close()

# Create header file
for i in range(0, len(keys)):
    keys[i] = str(keys[i])

for i in range(0, len(numbers)):
    numbers[i] = str(numbers[i])

outsrc =  "#include <avr/pgmspace.h>\n\n";
outsrc += "const unsigned char script_src[] PROGMEM = { " + ", ".join(keys) +  " };\n"
outsrc += "const int script_len = " + str(len(keys)) + ";\n"
outsrc += "const int script_numbers[] = { " + ", ".join(numbers) + " };\n"

# Write file
outf = open("script.h", "w")
outf.write(outsrc)
outf.close()

# Success!
print("Wrote to 'script.h' successfully!")
