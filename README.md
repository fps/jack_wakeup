# jack_wakeup
A small utility to sample wakeup times for a jackd client

It gathers gettimeofday() values and prints them to stdout at the end of the run.

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
octave:2> diff1 = mod(diff(data1(:,2)), 1000000);               
octave:3> mean(diff1)                        
ans =  1333.5                                                                              
octave:4> max(diff1)                         
ans =  1699                                                                                
octave:5> min(diff1)                                                                                                                                                                  
ans =  1076                                                                                
octave:7> sqrt(var(diff1))
ans =  62.629
</pre>

