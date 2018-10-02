from harness.test_harness import *

def parse_output(lines):
    results = lines.split('\n')[-2]
    time = results.split(' ')
    return time[-2]


print(calculate_avg('sieve3', 1, parse_output, '500000000', '1000000'))
print(calculate_avg('sieve3', 1, parse_output, '1000000000', '1000000'))  
print(calculate_avg('sieve3', 1, parse_output, '1500000000', '1000000'))

for i in range(2, 9):
    print(calculate_avg('sieve4', i, parse_output, '500000000', '1000000'))
    print(calculate_avg('sieve4', i, parse_output, '1000000000', '1000000'))
    print(calculate_avg('sieve4', i, parse_output, '1500000000', '1000000'))
