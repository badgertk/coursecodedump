#!/usr/bin/env python

from QTClock import QTClock

my_clock = QTClock('QTcF (ms) for patient 9004', color_cycle=['c', 'b'])

my_clock.add_danger_range(500)
my_clock.add_percentile_range('9004.csv', lower=16, upper=84, alpha=0.2, label='healthy', color='g')

my_clock.add_recording('9004.csv', label='unfiltered')

my_clock.add_legend()

my_clock.save('./clock_face.png')