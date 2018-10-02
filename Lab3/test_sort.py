from harness.test_harness import *

def parse_func(lines):
    output = lines.split(' ')[-2]
    return float(output)

'''
test_and_output_results('Merge_sort_parallel', parse_func)
'''
test_and_output_results('Merge_sortS', parse_func)
