import serial
import curses

ser = serial.Serial('/dev/rfcomm10')


screen = curses.initscr()
curses.noecho()
curses.cbreak()


print("Please specify a ch")
a = screen.getch()
print(a)


CHAR = 0;
lastchar = CHAR;
while True:
    CHAR = (screen.getch())
    print(CHAR)
    if lastchar != CHAR:
        if CHAR == -1:
            ser.write("stop\n")
        elif CHAR == ord('w'):
            ser.write('w\n')
        elif CHAR == ord('s'):
            ser.write('s\n')
        elif CHAR == ord('p'):
            ser.write('distance\n')
        elif CHAR == ord('a'):
            ser.write('a\n')
        elif CHAR == ord('d'):
            ser.write('d\n')
        elif CHAR ==ord('q'):
            ser.write('stop\n')
            break
    elif CHAR == -1:
        pass    
    lastchar = CHAR
ser.close()
curses.endwin()
exit()
