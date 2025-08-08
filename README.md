# Arya's Hackpad Macropad!

This 9-key macropad also includes a 3x3 led aray, rotary encoder, and an OLED display. It uses KMK Firmware (learning the python library was fun!) This is my first time working with PCB so it was a fun expeirment!

I designed this to help me with photoshop, illustrator, and indesign! I also saw some cool bitmap-pattern led machines at Ikea and wanted to emulate them using a LED array. 

# Features:
- top and bottom parts for the case
- XIAO‑RP2040 as the processor
- 128x32 OLED display
- 3x3 LED array
- 3x3 switch array
- Rotary encoder to switch between key-map layers and bitmap patterns
- 9 Diodes to help with having a lot of inputs with a limited number of GPIO ports
- KMK code!
- made with my sweat and tears :D

# CAD Model

Everything fits together using 4 M3 × 16 mm pan-head screws; it will go through the case and pcb.

This has 2 seperate printed pieces, sandwiching the PCB. 

This was made using Autodesk Fusion!

<img width="1433" alt="Screenshot 2025-06-12 at 12 26 53 AM" src="https://github.com/user-attachments/assets/d73ee25c-7734-4533-b3ad-d5efd2bba7f3" />
_screenshot of your overall hackpad and of your case and how it'll fit together_

Fusion wasn't importing the screws, switches/keycaps, rotary, or oled, or LEDs :( but the schematics down below will better illustrate where they are supposed to be :)

# Schematic

Here is a picture of the Schematic! This was my first interaction with anything hardware so it dosn't look exactly like my final board but it was so so fun to make this! 

<img width="997" alt="Screenshot 2025-06-12 at 12 30 19 AM" src="https://github.com/user-attachments/assets/1260159e-466b-4ae3-bb2c-9da14434cadd" />
_screenshot of your schematic_

# PCB

Now the real thing! It was definelty more challenging to do this than the schematic because this was for real! It was definetly a processes learning that how I made my schematic wouldn't nessicarily be how it would look in the end product, but fiddeling around with it was fun. I was super into graphic design before and this was a fun application of those skills, merging art with hardware!

<img width="475" alt="Screenshot 2025-06-12 at 12 32 29 AM" src="https://github.com/user-attachments/assets/bce588f4-4461-4b1b-b351-50b1fadb303f" />
_screenshot of your PCB_

# Firmware

This hackpad uses KMK firmware!

Some features:

- theres a different map for each application which the encoder can rotate between!
- there is 2 modes: layer mode and bitmap mode-- the layer mode allows it to work like a macropad while in bitmap mode the enocder rotates between different pattenrs for the LED array

There will be more features later! This was just an intro to get my hackpad running. very cool stuff indeed

# BOM
Here's the official Arya-pad partlist!

- 9x MX-Style Switches (preferably brown :3 )
- 9x DSA Blank White Keycaps
- 1x EC11 rotary encoder w/ push switch
- 10x 1N4148 signal diodes
- 9x SK6812 MINI-E RGB LEDs
- 1x 0.91 in 128 × 32 OLED Pannel
- 1x Seeed XIAO RP2040
- 1x Case (2 printed parts)
- 4x M3 × 16 mm pan-head screws

# Fun Facts
This was a really fun build! I spent 3ish weeks working on it, spreading it out so I can learn the most from it! As I said before, this was my first experience with hardware design but this project has made me really excited about what I can build on my own. 


# Build Journal 6ish hours

This was my first hardware design project and it was a ton of fun learning how to solder! Bellow, I have attached some snapshots through my build journey. 

![IMG_5028](https://github.com/user-attachments/assets/a89b004a-74da-410a-9cb6-66f71fbf4c70)

![IMG_5018](https://github.com/user-attachments/assets/91a2a374-d064-42a6-bb38-2736b0300f4f)

![IMG_5029](https://github.com/user-attachments/assets/cca5f109-3c17-4af8-832f-f62dfd37cd1b)


![IMG_5012](https://github.com/user-attachments/assets/a1fff8b3-28f7-4f0a-9221-e8ebec3fd485)

![IMG_5013](https://github.com/user-attachments/assets/8a6bfd2b-48ce-402c-![IMG_5058](https://github.com/user-attachments/assets/dd8e9a42-585f-498f-82bb-7e9cf0de07d7)
8bdf-4c33d4b0655e)
![IMG_5061](https://github.com/user-attachments/assets/b97e60c6-6bf8-45cc-a67a-0ea558268207)

