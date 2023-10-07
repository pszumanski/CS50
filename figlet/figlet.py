import sys
import random
from pyfiglet import Figlet

figlet = Figlet()
if len(sys.argv) == 1:
    figlet.setFont(font=str(random.choice(figlet.getFonts())))
elif len(sys.argv) == 3 and sys.argv[1] in ["-f", "-font"] and sys.argv[2] in figlet.getFonts():
    figlet.setFont(font=sys.argv[2])
else:
    print("Usage: python figlet.py -f font")
    sys.exit(1)
s = str(input("Input: "))
print("Output: ")
print(figlet.renderText(s))