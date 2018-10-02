#!/usr/bin/env python3
'''
    Provides an abstracted automated test harness for calculating
    runtime and speedup of parallel experiments.
'''

import subprocess, os, errno
import matplotlib as mpl
mpl.use('Agg')

import matplotlib.pyplot as plt

__author__ = "Christopher William Westerman"
__email__ = "Westerman.Christopher@gmail.com"

def plot_data(program_name, avg_times):
    print('Graphing', avg_times)
    directory = 'graphs/'
    try:
        os.makedirs(directory)
    except OSError as e:
        if e.errno != errno.EEXIST:
            raise

    plt.figure(0)
    plt.title(program_name + ' Averages')
    plt.plot(range(1, len(avg_times)+1), avg_times)
    plt.ylabel('time (seconds)')
    plt.xlabel('cores')
    plt.savefig(directory + program_name + '_avgs_graph.png')
    plt.clf()

    speed_ups = []
    avg_times = avg_times[1:]
    for time in avg_times:
        speed_ups.append(avg_times[0]/time)

    plt.figure(1)
    plt.title(program_name + ' Speedup')
    plt.plot(range(2,len(speed_ups)+2), speed_ups)
    plt.ylabel('speed up (compared to sequential)')
    plt.xlabel('cores')
    plt.savefig(directory + program_name + '_speed_ups.png')
    plt.clf()
    
def decode_wrapper(popen, parse_func):
    output = popen.communicate()
    return float(parse_func(output[0].decode()))

def calculate_avg(program_name, num_threads, parsing_func, *args):
    times = []
    print("Calulating avg for", program_name, "with", num_threads, "threads...")
    for _ in range(0, 7):
        os.environ["OMP_NUM_THREADS"] = str(num_threads)
        p = subprocess.Popen([program_name, *args],
                            stdout=subprocess.PIPE,
                            stderr=subprocess.STDOUT)

        times.append(decode_wrapper(p, parsing_func))
    times = times[1:-1]
    return sum(times)/len(times)

def test_and_output_results(program_name, parsing_func, *args):
    avg_times = []
    for num_threads in range(1, 9):
        result = calculate_avg(program_name, num_threads, parsing_func, *args)
        print(result, 'seconds')
        avg_times.append(result)
    plot_data(program_name, avg_times)

