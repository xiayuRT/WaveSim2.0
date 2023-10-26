import matplotlib.pyplot as plt
import numpy as np
import csv

if __name__ == "__main__":
    time = []
    speed = []
    waveheight = []

    with open('output.csv', 'r') as csv_file:
        csv_reader = csv.reader(csv_file)
        for row in csv_reader:
            time.append(float(row[0]))
            speed.append(float(row[1]))
            waveheight.append(float(row[2]))

    plt.plot(time, speed, label = 'speed')
    plt.plot(time, waveheight, label = 'waveheight')
    plt.xlabel('time(s)')
    plt.ylabel('Values')
    plt.title('Multiple Lines on One Plot')
    plt.legend()
    plt.grid()
    plt.show()
