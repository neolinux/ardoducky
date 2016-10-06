import sys

# Functions
keyCodes = {
    "enter":    10,
    "space":    32,

    "ctrl":     0x80,
    "shift":    0x81,
    "alt":      0x82,
    "gui":      0x83,

    "right":    0xd7,
    "left":     0xd8,
    "down":     0xd9,
    "up":       0xda,

    "esc":      0xb1,
    "back":     0xb2,
    "tab":      0xb3,
    
    "ins":      0xd1,
    "home":     0xd2,
    "pgup":     0xd3,
    "del":      0xd4,
    "end":      0xd5,
    "pgdn":     0xd6,

    "caps":     0xc1,
    
    "f1":       0xc2,
    "f2":       0xc3,
    "f3":       0xc4,
    "f4":       0xc5,
    "f5":       0xc6,
    "f6":       0xc7,
    "f7":       0xc8,
    "f8":       0xc9,
    "f9":       0xca,
    "f10":      0xcb,
    "f11":      0xcc,
    "f12":      0xcd
}

def keyCode(key):
    if key in keyCodes:
        return keyCodes[key]

    elif (key >= 'A' and key <= 'Z') or\
        (key >= 'a' and key <= 'z') or\
        (key >= '0' and key <= '0'):
            return ord(key)

    else:
        print("]]] WARNING: Unknown key '" + key + "'")
        return 0

# Check args
if len(sys.argv) != 2:
    print("Usage: adscompiler.py <script>")
    exit()

# Open duckyscript
inf = open(sys.argv[1], "r")

# Script vars
ads_data = []
ads_consts = []
ads_vars = []

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

    # Write string
    if cmd == "write":
        while len(arg) > 0:
            sub = arg[:255]
            ads_data.append(1) # OP
            ads_data.append(len(sub)) # Len
            for c in sub:
                ads_data.append(ord(c)) # Char

            arg = arg[255:]

    # Wait
    elif cmd == "wait":
        # Push number into constants if it doesn't exist already
        num = int(arg)
        if not num in ads_consts:
            ads_consts.append(num)

        idx = ads_consts.index(num)
        if idx > 255:
            printf("]]] ERROR: Too many constants!")

        # Push command
        ads_data.append(2) # OP
        ads_data.append(idx) # Const index

    # Press key
    elif cmd == "press":
        keys = arg.split('+')
        for key in keys:
            ads_data.append(3) # Press key OP
            ads_data.append(keyCode(key)) # Keycode

        for key in reversed(keys):
            ads_data.append(4) # Release key OP
            ads_data.append(keyCode(key)) # Keycode

    # Unknown
    else:
        print("]]] WARNING: Unhandled command '" + cmd + "'")

# Close input file
inf.close()

# Turn data into strings
for i in range(len(ads_data)):
    ads_data[i] = str(ads_data[i])

for i in range(len(ads_consts)):
    ads_consts[i] = str(ads_consts[i])

for i in range(len(ads_vars)):
    ads_vars[i] = str(ads_vars[i])

# Create header
outsrc =  "#include <avr/pgmspace.h>\n\n";
outsrc += "const unsigned char ads_data[] PROGMEM = { " + ", ".join(ads_data) +  " };\n"
outsrc += "const int ads_len = " + str(len(ads_data)) + ";\n"
outsrc += "const int ads_consts[] = { " + ", ".join(ads_consts) + " };\n"
outsrc += "const int ads_vars[] = { " + ", ".join(ads_vars) + " };\n"

# Write file
outf = open("script.h", "w")
outf.write(outsrc)
outf.close()

# Success!
print("Wrote to 'script.h' successfully!")
