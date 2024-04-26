#include <iostream>
#include <sstream>
#include <vector>
#include <algorithm>
#include <cstdlib>
#include <unistd.h>
#include <boost/program_options.hpp>
#include <jack/jack.h>
#include <signal.h>
#include <cstdint>
#include <time.h>
#include <cmath>
#include <iomanip>
#include <stdio.h>

namespace po = boost::program_options;

jack_client_t *jack_client;
jack_port_t* jack_port;

size_t number_of_samples;
std::vector<timespec> samples;

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
        clock_gettime(CLOCK_MONOTONIC_RAW, &samples[count]);
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

    bool report_samples = false;
    bool report_statistics = false;
    bool report_differences = false;

    po::options_description desc("Allowed options");
    desc.add_options()
        ("help,h", "produce help message")
        ("jack-client-name,a", po::value<std::string>(&jack_client_name)->default_value("jack_wakeup"), "The jack client name to use")
        ("jack-server-name,e", po::value<std::string>(&jack_server_name)->default_value("default"), "The jack server name to use")
        ("number-of-samples,n", po::value<size_t>(&number_of_samples)->default_value(512), "The number of samples to gather")
        ("report-differences,d", po::value<bool>(&report_differences)->default_value(false), "Whether to report the differences between raw sample data")
        ("report-samples,s", po::value<bool>(&report_samples)->default_value(false), "Whether to report the raw sample data")
        ("report-statistics,t", po::value<bool>(&report_statistics)->default_value(true), "Whether to report statistics about differences between samples")
    ;

    po::variables_map vm;
    po::store(po::parse_command_line(ac, av, desc), vm);
    po::notify(vm);    

    if (vm.count("help")) {
        std::cout << desc << "\n";
        return 1;
    }
    // std::cerr << "Starting up..." << std::endl;

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

    if (true == report_samples)
    {
        for (size_t index = 0; index < number_of_samples; ++index)
        {
            std::cout << samples[index].tv_sec << " " << samples[index].tv_nsec << "\n";
        }
    }

    if (true == report_differences) 
    {
        for (size_t index = 1; index < number_of_samples; ++index)
        {
            double diff = (((double)samples[index].tv_nsec + 1000000000.0 * (double)samples[index].tv_sec) - ((double)samples[index-1].tv_nsec + 1000000000.0 * (double)samples[index-1].tv_sec)) / 1000000.0f;
            std::cout << diff << "\n";
        }
    }

    if (true == report_statistics)
    {
        std::vector<float> diffs;
        diffs.resize(samples.size() - 1);
        for (size_t index = 0; index < diffs.size(); ++index)
        {
            diffs[index] = remainder((samples[index + 1].tv_nsec - samples[index].tv_nsec), 1000000000.0);
        }

        double mean_diff = 0;
        float min_diff = diffs[0];
        float max_diff = 0;

        for (size_t index = 0; index < diffs.size(); ++index)
        {
            mean_diff += diffs[index] / (float)(samples.size());
            if (diffs[index] < min_diff) min_diff = diffs[index];
            if (diffs[index] > max_diff) max_diff = diffs[index];
        }

        std::cout << "min: " << (min_diff/1e6) << " ms; mean: " << (mean_diff/1e6) << "ms;  max: " << (max_diff/1e6) << " ms\n";
    }

    // std::cerr << "Bye." << std::endl;

    return 0;
}
