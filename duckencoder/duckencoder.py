# https://github.com/coopermaa/USBKeyboard/blob/master/hid_keys.h
# http://www.freebsddiary.org/APC/usb_hid_usages.php

# TODO:
# - Variables
# - Repeat

import sys

# Defines
MOD_CTRL = 1
MOD_SHIFT = 2
MOD_ALT = 4
MOD_GUI = 8

KEY_A = 4
KEY_ENTER = 40

# Functions
def keyValue(k):
    k = k.upper()

    if len(k) == 1 and k >= 'A' and k <= 'Z':
        return ord(k) - ord('A') + KEY_A

    elif k == "ENTER":
        return KEY_ENTER

    else:
        print("]]] INFO: Unhandled key '" + k + "'")
        return 0

# Check args
if len(sys.argv) != 2:
    print("Usage: duckencoder.py <script>")
    exit()

# Open duckyscript
inf = open(sys.argv[1], "r")
keys = [] # Alternating ctrl-key and character-key, 

#               CTRL KEY EXCEPTIONS
#           255 - delay
#           254 - release
#           253 - input alt code
#           252 - press normal key then release
#           251 - press key with shift then release

numbers = [] # Used to store ints

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

    # Commands
    if cmd == "DELAY":
        keys.append(255)
        numbers.append(int(arg))

    elif cmd == "REM":
        print(">>> " + arg)

    elif cmd == "STRING":
        for c in arg:
            # Letter key
            if c >= 'a' and c <= 'z':
                keys.append(252)
                keys.append(ord(c) - ord('a') + KEY_A)
            
            # Shifted letter key   
            elif c >= 'A' and c <= 'Z':
                keys.append(251)
                keys.append(ord(c) - ord('A') + KEY_A)
            
            # Anything else
            else:
                # Enter altcode
                keys.append(253)
                keys.append(ord(c))

    elif cmd == "GUI" or cmd == "WINDOWS":
        keys.append(MOD_GUI)
        keys.append(keyValue(arg[0]) if len(arg) > 0 else 0)
        keys.append(254) # Release keys

    elif cmd == "CTRL" or cmd == "CONTROL":
        keys.append(MOD_CTRL)
        keys.append(keyValue(arg[0]) if len(arg) > 0 else 0)
        keys.append(254) # Release keys

    elif cmd == "SHIFT":
        keys.append(MOD_SHIFT)
        keys.append(keyValue(arg[0]) if len(arg) > 0 else 0)
        keys.append(254) # Release keys

    elif cmd == "ALT":
        keys.append(MOD_ALT)
        keys.append(keyValue(arg[0]) if len(arg) > 0 else 0)
        keys.append(254) # Release keys

    # Lastly check if this is a key
    elif keyValue(cmd) > 0:
        keys.append(0)
        keys.append(keyValue(cmd))
        keys.append(254) # Release keys

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
