# jack_wakeup
A small utility to sample wakeup times for a jackd client. Its usefullness is 
mostly limited to being run as the only client in a jack session since only 
in this case it's guaranteed to be run as soon as possible after a
period has started.

It gathers <code>clock_gettime(CLOCK_MONOTONIC_RAW)</code> values and prints 
them to stdout at the end of the run per default. The format is simply two colums,
the first being the <code>tv_secs</code> and the second the <code>tv_nsesc</code> 
values.

Optionally it can print a little bit of statistics. See the example usage
below.

<pre>
Allowed options:
  -h [ --help ]                         produce help message
  -a [ --jack-client-name ] arg (=jack_wakeup)
                                        The jack client name to use
  -e [ --jack-server-name ] arg (=default)
                                        The jack server name to use
  -n [ --number-of-samples ] arg (=512) The number of samples to gather
  -s [ --report-samples ] arg (=1)      Whether to report the raw sample data
  -t [ --report-statistics ] arg (=0)   Whether to report statistics about 
                                        differences between samples
</pre>

# Example usage:

<pre>
[alarm@alarmpi jack_wakeup]$ for n in `seq 1 10`; do ./jack_wakeup -t 1 -s 0; done
mean: 2.66157e+06 min: 2.55376e+06 max: 2.7847e+06
mean: 2.66122e+06 min: 2.38767e+06 max: 2.78672e+06
mean: 2.66161e+06 min: 2.58015e+06 max: 2.77909e+06
mean: 2.66165e+06 min: 2.58152e+06 max: 2.78143e+06
mean: 2.66166e+06 min: 2.59293e+06 max: 2.77958e+06
mean: 2.66178e+06 min: 2.58165e+06 max: 2.78341e+06
mean: 2.66149e+06 min: 2.5447e+06 max: 2.78161e+06
mean: 2.66155e+06 min: 2.54759e+06 max: 2.7778e+06
mean: 2.66116e+06 min: 2.34372e+06 max: 2.78307e+06
mean: 2.66135e+06 min: 2.45059e+06 max: 2.78507e+06
[alarm@alarmpi jack_wakeup]$ ./jack_wakeup -n 30
118165 788141772
118165 790593032
118165 793297846
118165 795922069
118165 798551865
118165 801295161
118165 803924995
118165 806550291
118165 809300883
118165 811925032
118165 814553865
118165 817302606
118165 819928846
118165 822552995
118165 825298439
118165 827929735
118165 830552550
118165 833302569
118165 835903994
118165 838555661
118165 841305476
118165 843926809
118165 846555457
118165 849305735
118165 851931069
118165 854558976
118165 857303494
118165 859933087
118165 862559735
118165 865309346

</pre>


