#!/usr/bin/python

#run like this:
#python writeToLCD "your message" 10
#where 10 is in seconds how long you want the message to stay on the screen

import sys
import socket
import fcntl
import struct
import time
import pyupm_i2clcd as lcd

def writeToLCD(message, timeW=2, r =0,g =0,b=255):
    # Initialize Jhd1313m1 at 0x3E (LCD_ADDRESS) and 0x62 (RGB_ADDRESS)
    myLcd = lcd.Jhd1313m1(0, 0x3E, 0x62)

    # Clear
    myLcd.clear()

    # Green
    #red,green 
    myLcd.setColor(r, g, b)
    #RED
    #myLcd.setColor(255,0,0)

    # Zero the cursor
    myLcd.setCursor(0,0)

    # myLcd.clear()
    myLcd.write(message)
    time.sleep(timeW)

if sys.argv[2]:
    writeToLCD(sys.argv[1], int(sys.argv[2]))
else:
    writeToLCD(sys.argv[1])
