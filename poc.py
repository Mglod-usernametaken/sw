import os
import time
print("hello.")
moc = input("wybierz moc:")
os.system('clear')
czas = input("wybierz czas:")
os.system('clear')

start = input("Naciśnij start")

while start:
    for i in range(int(czas)):
        counter = int(czas)-i
        os.system('clear')
        print(f"{moc} | {counter}")
        time.sleep(1)


    os.system('clear')
    print("done")
    start = 0
