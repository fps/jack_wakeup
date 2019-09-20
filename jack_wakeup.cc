#include <iostream>
#include <sstream>
#include <vector>
#include <algorithm>
#include <cstdlib>
#include <gpiod.h>
#include <unistd.h>
#include <boost/program_options.hpp>
#include <jack/jack.h>
#include <signal.h>
#include <cstdint>
#include <sys/time.h>

namespace po = boost::program_options;

jack_client_t *jack_client;
jack_port_t* jack_port;

uint64_t number_of_samples;
std::vector<timeval> samples;

bool quit = false;

extern "C" 
{
    void signal_handler(int signo)
    {
        quit = true;
    }
}

uint64_t count;

int
process (jack_nframes_t nframes, void *arg)
{
    if (count < number_of_samples)
    {
        gettimeofday(&samples[count], 0);
    }
    else
    {
        quit = true;
    }

    ++count;

    return 0;      
}

int main(int ac, char *av[])
{
    std::string jack_client_name;
    std::string jack_server_name;

    po::options_description desc("Allowed options");
    desc.add_options()
        ("help,h", "produce help message")
        ("jack-client-name,a", po::value<std::string>(&jack_client_name)->default_value("jack_wakeup"), "The jack client name to use")
        ("jack-server-name,e", po::value<std::string>(&jack_server_name)->default_value("default"), "The jack server name to use")
        ("number-of-samples,n", po::value<uint64_t>(&number_of_samples)->default_value(512), "The number of samples to gather")
    ;

    po::variables_map vm;
    po::store(po::parse_command_line(ac, av, desc), vm);
    po::notify(vm);    

    if (vm.count("help")) {
        std::cout << desc << "\n";
        return 1;
    }
    std::cerr << "Starting up..." << std::endl;

    count = 0;
    samples.resize(number_of_samples);

    jack_status_t jack_status;
    jack_client = jack_client_open(jack_client_name.c_str(), JackNullOption, &jack_status, jack_server_name.c_str());

    if (NULL == jack_client) {
        std::cout << "Failed to create jack_client. Exiting." << std::endl;
        return 1;
    }

    jack_set_process_callback(jack_client, process, 0);

    jack_port = jack_port_register(jack_client, "in", JACK_DEFAULT_AUDIO_TYPE, JackPortIsInput, 0);
    if (jack_port == NULL) {
        std::cout << "Failed to register port" << std::endl;
        return 1;
    }

    signal(SIGTERM, signal_handler);
    signal(SIGINT, signal_handler);

    if (jack_activate(jack_client))
    {
        std::cout << "Failed to activate jack_client" << std::endl;
        return 1;
    }


    while(false == quit)
    {
        usleep(1000);
    }

    jack_deactivate(jack_client);

    for (uint64_t index = 0; index < number_of_samples; ++index)
    {
        std::cout << samples[index].tv_sec << " " << samples[index].tv_usec << "\n";
    }

    std::cerr << "Bye." << std::endl;

    return 0;
}
