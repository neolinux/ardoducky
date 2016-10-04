# https://github.com/coopermaa/USBKeyboard/blob/master/hid_keys.h
# http://www.freebsddiary.org/APC/usb_hid_usages.php

# TODO:
# - Variables
# - Repeat

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

# Read lines
for line in inf:
    # Remove newlines
    line = line.replace("\n", "").replace("\r", "")

    # Line too short?
    if len(line) < 3:
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

    # Commands
    if cmd == "DELAY":
        keys.append(251)
        numbers.append(int(arg))

    elif cmd == "REM":
        print(">>> " + arg)

    elif cmd == "STRING":
        while len(arg) > 0:
            sub = arg[:250]
            keys.append(len(sub))
            for c in sub:
                keys.append(ord(c))

            arg = arg[250:]

    elif cmd in mods:
        keys.append(0)
        keys.append(mods.index(cmd))
        keys.append(ord(arg[0]) if len(arg) > 0 else 0)

    # Lastly check if this is a key
    #elif len(cmd) == 1 and ord(cmd[0]) > 0:
    #    keys.append(1)
    #    keys.append(ord(cmd))

    else:
        print("]]] WARNING: Unhandled command '" + cmd + "'")


# Close file
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
