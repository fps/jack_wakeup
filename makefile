jack_wakeup: jack_wakeup.cc
	g++ -std=c++11 -O3 $(CXXFLAGS) -Wall -o jack_wakeup jack_wakeup.cc -lgpiod -ljack -lboost_program_options
