import board, busio, time, neopixel, adafruit_ssd1306
from kmk.kmk_keyboard import KMKKeyboard
from kmk.scanners.keypad import MatrixScanner
from kmk.keys import KC
from kmk.modules.layers import Layers
from kmk.modules.encoders import Encoders

# ── KMK base ──────────────────────────────────────────────────────────────
kbd = KMKKeyboard()
kbd.col_pins = (board.GP0, board.GP1, board.GP2)
kbd.row_pins = (board.GP3, board.GP4, board.GP6)
kbd.diode_orientation = MatrixScanner.Column2Row

# ── Layers & colours ──────────────────────────────────────────────────────
layers = Layers(); kbd.modules.append(layers)
BASE, JET, PS, AI, ID, MEDIA = range(6)
LAYER_ORDER = [BASE, JET, PS, AI, ID, MEDIA]
LAYER_NAMES  = ["Base", "Jet", "PS", "AI", "ID", "Media"]
LAYER_COLORS = {
    BASE : (  0, 255, 255),  # Cyan
    JET  : (255, 160,   0),  # Orange
    PS   : (255,  60,  60),  # Red
    AI   : (255, 220,   0),  # Yellow
    ID   : (160,   0, 255),  # Violet
    MEDIA: (  0, 255, 100),  # Green
}

# ── Encoder (A/B only – push wired in matrix) ─────────────────────────────
enc = Encoders(); enc.encoder_pins = ((board.GP26, board.GP27, None),)
kbd.modules.append(enc)

# ── 3×3 NeoPixel panel ───────────────────────────────────────────────────
NUM_PIXELS = 9
pixels = neopixel.NeoPixel(board.GP28, NUM_PIXELS, brightness=0.25, auto_write=False)
MAP = [[0,1,2],[3,4,5],[6,7,8]]
P = lambda r,c,clr: pixels.__setitem__(MAP[r][c], clr)

# ── 128×32 I²C OLED ───────────────────────────────────────────────────────
try:
    i2c  = busio.I2C(board.GP29, board.GP7)  # SCL, SDA
    oled = adafruit_ssd1306.SSD1306_I2C(128, 32, i2c)
    oled.fill(0); oled.show()
    OLED_OK = True
except Exception as e:
    print(f"OLED init failed → continuing headless: {e}")
    OLED_OK = False

oled_line = (lambda txt,y: oled.text(txt,0,y,1)) if OLED_OK else (lambda *a,**k: None)

# ── 16 tiny bit‑maps ───────────────────────────────────────────────────────
OFF=(0,0,0); W=(255,255,255); R=(255,40,40); G=(0,200,40)
B=(0,120,255); Y=(255,180,0)
PATTERNS = [
    [[OFF,R,OFF],[R,R,R],[OFF,R,OFF]],                 # 0 Heart
    [[OFF,B,OFF],[OFF,B,OFF],[B,B,B]],                 # 1 Up
    [[B,B,B],[OFF,B,OFF],[OFF,B,OFF]],                 # 2 Down
    [[OFF,B,OFF],[B,B,B],[OFF,B,OFF]],                 # 3 Left/Right
    [[OFF,G,OFF],[G,G,G],[OFF,G,OFF]],                 # 4 Plus
    [[OFF,OFF,OFF],[Y,Y,Y],[OFF,OFF,OFF]],             # 5 Minus
    [[R,OFF,R],[OFF,R,OFF],[R,OFF,R]],                 # 6 X
    [[W,W,W],[W,OFF,W],[W,W,W]],                       # 7 Square
    [[Y,OFF,Y],[OFF,Y,OFF],[Y,OFF,Y]],                 # 8 Checker
    [[OFF,OFF,R],[OFF,R,OFF],[R,OFF,OFF]],             # 9 / diag
    [[R,OFF,OFF],[OFF,R,OFF],[OFF,OFF,R]],             #10 \ diag
    [[OFF,OFF,OFF],[OFF,W,OFF],[OFF,OFF,OFF]],         #11 Dot
    [[OFF,OFF,OFF],[Y,OFF,Y],[OFF,Y,OFF]],             #12 Smile
    [[OFF,OFF,OFF],[OFF,R,OFF],[R,OFF,R]],             #13 Frown
    [[W,W,W],[W,W,W],[W,W,W]],                         #14 Full
    [[B,G,R],[Y,W,B],[R,G,B]],                         #15 Random
]
pat = 0
edit_mode = False
brightness = 0.25

# ── Draw helpers ───────────────────────────────────────────────────────────
last_oled = 0

def show_pattern(frame=False):
    bmp = PATTERNS[pat]
    for r in range(3):
        for c in range(3):
            P(r,c,bmp[r][c])
    if frame:
        for i in range(3):
            P(0,i,W); P(2,i,W); P(i,0,W); P(i,2,W)
    pixels.show()

def show_layer():
    ly = layers.active_layers[0] if layers.active_layers else BASE
    col = LAYER_COLORS[ly]
    for i in range(NUM_PIXELS): pixels[i]=col
    pixels.show()

def refresh_oled(force=False):
    global last_oled
    if not OLED_OK: return
    now = time.monotonic()
    if not force and now - last_oled < .1: return     # 100 ms throttle
    last_oled = now
    oled.fill(0)
    if edit_mode:
        oled_line("PX Mode",0)
        oled_line(f"{pat+1}/{len(PATTERNS)}  B{int(pixels.brightness*100)}%",10)
    else:
        ly = layers.active_layers[0] if layers.active_layers else BASE
        oled_line("Layer",0); oled_line(LAYER_NAMES[ly],10)
    oled.show()

# ── Encoder rotation ───────────────────────────────────────────────────────
last_rot = 0

def rot(step:int):
    global pat, last_rot
    now=time.monotonic()
    if now-last_rot<.05: return    # 50 ms debounce
    last_rot=now
    if edit_mode:
        pat=(pat+step)%len(PATTERNS); show_pattern(True); refresh_oled(True)
    else:
        cur=layers.active_layers[0] if layers.active_layers else BASE
        idx=LAYER_ORDER.index(cur); new=LAYER_ORDER[(idx+step)%len(LAYER_ORDER)]
        if new!=cur:
            layers.active_layers[:]=[new]; show_layer(); refresh_oled(True)

enc.encoder_callbacks=[[lambda:rot(+1),lambda:rot(-1)]]

# ── Matrix‑handled buttons ─────────────────────────────────────────────────
STATE = {'push':False,'up':False,'dn':False}
LAST  = {'push':0,'up':0,'dn':0}
DB=0.02; BR_STEP=0.05

def after_scan():
    global edit_mode, brightness
    now=time.monotonic()
    # encoder push row2 col0
    pushed=kbd.matrix.is_key_pressed(2,0)
    if pushed!=STATE['push'] and now-LAST['push']>DB:
        if pushed:
            edit_mode=not edit_mode
            (show_pattern(True) if edit_mode else show_layer())
            refresh_oled(True)
        STATE['push']=pushed; LAST['push']=now
    if edit_mode:
        # brightness up   row2 col1
        up=kbd.matrix.is_key_pressed(2,1)
        if up!=STATE['up'] and now-LAST['up']>DB:
            if up:
                brightness=min(brightness+BR_STEP,1)
                pixels.brightness=brightness; pixels.show(); refresh_oled(True)
            STATE['up']=up; LAST['up']=now
        # brightness down row2 col2
        dn=kbd.matrix.is_key_pressed(2,2)
        if dn!=STATE['dn'] and now-LAST['dn']>DB:
            if dn:
                brightness=max(brightness-BR_STEP,.05)
                pixels.brightness=brightness; pixels.show(); refresh_oled(True)
            STATE['dn']=dn; LAST['dn']=now

kbd.after_matrix_scan_callbacks.append(after_scan)

# ── Shortcut helpers ───────────────────────────────────────────────────────
C=lambda *k: KC.LCTL(*k)
A=lambda *k: KC.LALT(*k)
S=lambda *k: KC.LSFT(*k)

JET=[C(S(KC.A)), C(S(KC.N)), C(KC.E),
     C(A(KC.L)), S(KC.F10), S(KC.F9),
     S(KC.F6),   A(KC.ENTER), C(KC.P)]
PS =[KC.V, KC.B, KC.L,
     KC.M, KC.E, C(KC.D),
     C(A(KC.I)), C(A(KC.C)), C(A(S(KC.S)))]
AI =[KC.V, KC.A, KC.P,
     KC.T, KC.R, C(KC.G),
     C(S(KC.G)), C(KC.Z), C(S(KC.Z))]
ID =[KC.V, KC.T, KC.P,
     C(KC.D), C(KC.E), KC.W,
     C(KC.SEMICOLON), C(KC.N0), C(A(KC.I))]
MEDIA=[KC.MPLY,KC.MPRV,KC.MNXT,
       KC.VOLD,KC.VOLU,KC.MUTE,
       KC.BRIGHTNESS_DOWN,KC.BRIGHTNESS_UP,KC.NO]
BASE=[KC.ESC,KC.TAB,KC.MUTE,
      KC.LEFT,KC.DOWN,KC.RIGHT,
      KC.SPC,KC.UP,KC.ENT]

kbd.keymap=[BASE,JET,PS,AI,ID,MEDIA]

# ── Startup splash ─────────────────────────────────────────────────────────
print("Booting Arya‑Pad…")
pixels.brightness = brightness
show_layer(); refresh_oled(True)

if __name__=='__main__':
    kbd.go()
