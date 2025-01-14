DESTDIR ?= /
PREFIX ?= /usr/local

jack_wakeup: jack_wakeup.cc
	g++ -std=c++11 -O3 $(CXXFLAGS) -Wall -o jack_wakeup jack_wakeup.cc -ljack -lboost_program_options

install: jack_wakeup
	mkdir -p $(DESTDIR)$(PREFIX)/bin
	install jack_wakeup $(DESTDIR)$(PREFIX)/bin
