# -*- coding: utf-8 -*-
"""
Created on Tue Feb 07 14:33:55 2017

@author: Elijah
"""

import matplotlib.pyplot as plt
from time import time, sleep
import random

def run_PID(set_point=100, start_point=0,
            K_p=0.5, K_i=0, K_d=0.1, length=5, force=0, max_out=20, noise=0):
    start_time = time()
    point = start_point
    points = [start_point]
    times = [0]
    target = [set_point]
    zero = [0]
    outs = [0]
    potentials = [0]
    integrals = [0]
    derivatives = [0]
    integral = 0
    last_error = set_point - start_point
    last_time = start_time
    deltaP = 0
    loops = 0
    while (time() - start_time < length):
        loops += 1        
        sleep(0.1)
        error = set_point - point + random.randint(-noise,noise)
        now = time()
        time_dif = now - last_time

        integral = integral + error * time_dif
        derivative = (error - last_error)/time_dif

        out = error*K_p + integral*K_i + derivative*K_d
        if (out > max_out):
            out = max_out
        elif (out < -max_out):
            out = -max_out
        # print(out)

        last_error = error
        last_time = now

        deltaP += out
        deltaP += force

        point += deltaP

        points.append(point)
        times.append(now-start_time)
        target.append(set_point)
        outs.append(out)
        potentials.append(error*K_p)
        integrals.append(integral*K_i)
        derivatives.append(derivative*K_d)
        zero.append(0)
    print("Point at: %.2f" % point)
    print("Out at: %.2f" % outs[-1])
    print("Total loops: %.0f, average loops per second: %.2f" % (loops, loops/length))
    plt.plot(times, points, times, target)
    plt.plot(times, outs, times, zero)
    plt.xlabel("Time (s)", fontsize=14)
    plt.ylabel("Position/Control response", fontsize=14)
    plt.title("PID Simulation Plot", fontsize=18)
    plt.text(length*0.75, set_point*0.8, r"$K_p = %.1f$" % K_p, fontsize=16)
    plt.text(length*0.75, set_point*0.7, r"$K_i = %.1f$" % K_i, fontsize=16)
    plt.text(length*0.75, set_point*0.6, r"$K_d = %.1f$" % K_d, fontsize=16)
    plt.text(length*0.75, set_point*0.5, r"$force = %.0f$" % force, fontsize=16)
    plt.text(length*0.75, set_point*0.4, r"$noise = %.0f$" % noise, fontsize=16)
    plt.text(length*0.1, set_point*0.7, r"$Position$", fontsize=16, color="b")
    plt.text(length*0.25, set_point*0.2, r"$Control Response$", 
             fontsize=16, color="r")
    cur_axis = list(plt.axis())
    cur_axis[1] = length
    plt.axis(cur_axis)
#    plot(times, potentials, "g", times, integrals, "r", times, derivatives, "blue")