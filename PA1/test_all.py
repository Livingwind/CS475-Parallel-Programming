import sys
sys.path.append('..')
from ..harness.test_harness import *

def parse_output_1D(lines):
    results = lines.split('\n')[-2]
    time = results.split(' ')
    return float(time[-2])

def parse_output_2D(lines):
    results = lines.split('\n')[0]
    time = results.split(' ')
    return float(time[-2])

def parse_output_vec(lines):
    results = lines.split('\n')[-2]
    time = results.split(' ')
    return float(time[-1])

test_and_output_results('stencil_1D', parse_output_1D, '4000', '200000')
test_and_output_results('stencil_2D', parse_output_2D, '800', '2000')
test_and_output_results('mat_vec', parse_output_vec, '25000', '10000')
