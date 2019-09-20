# jack_wakeup
A small utility to sample wakeup times for a jackd client. Its usefullness is 
mostly limited to being run as the only client in a jack session since only 
in this case it's guaranteed to be run as soon as possible after a
period has started.

It gathers <code>clock_gettime(CLOCK_MONOTONIC_RAW)</code> values and prints 
them to stdout at the end of the run. The format is simply two colums,
the first being the <code>tv_secs</code> and the second the <code>tv_nsesc</code> 
values

<pre>
Allowed options:
  -h [ --help ]                         produce help message
  -a [ --jack-client-name ] arg (=jack_wakeup)
                                        The jack client name to use
  -e [ --jack-server-name ] arg (=default)
                                        The jack server name to use
  -n [ --number-of-samples ] arg (=512) The number of samples to gather
</pre>

# Example usage:

<pre>
$ ./jack_wakeup -n 100000 > log_without_stress.txt
$ octave
octave:1> data1 = dlmread("log_with_stress.txt");               
octave:2> diff1 = mod(diff(data1(:,2)), 10^9) / 10^6;               
octave:3> mean(diff1)                        
ans =  1333.5                                                                              
octave:4> max(diff1)                         
ans =  1699                                                                                
octave:5> min(diff1)                                                                                                                                                                  
ans =  1076                                                                                
octave:7> sqrt(var(diff1))
ans =  62.629
</pre>

