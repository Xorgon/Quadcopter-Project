# -*- coding: utf-8 -*-
"""
Created on Tue Feb 07 14:33:55 2017

@author: Elijah
"""

from matplotlib.pyplot import plot
from time import time, sleep


def run_PID(set_point=100, start_point=0,
            K_p=0.5, K_i=0, K_d=0.1, length=5, force=0):
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
    while (time() - start_time < length):
        sleep(0.1)
        error = set_point - point
        now = time()
        time_dif = now - last_time

        integral = integral + error * time_dif
        derivative = (error - last_error)/time_dif

        out = error*K_p + integral*K_i + derivative*K_d
        # print(out)

        last_error = error
        last_time = now

        deltaP += out

        point += deltaP

        # Some imaginary force acting on the point.
        point += force

        points.append(point)
        times.append(now-start_time)
        target.append(set_point)
        outs.append(out)
        potentials.append(error*K_p)
        integrals.append(integral*K_i)
        derivatives.append(derivative*K_d)
        zero.append(0)
    print(point)
    plot(times, points, times, target)
    plot(times, outs, times, zero)
#    plot(times, potentials, "g", times, integrals, "r", times, derivatives, "blue")