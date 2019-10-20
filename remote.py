import serial
import curses


screen = curses.initscr()
curses.echo()
curses.cbreak()


print("Please specify a ch")
a = screen.getch()
print(a)

curses.endwin()
exit()
#ser = serial.Serial(port)
#print(ser.name)
