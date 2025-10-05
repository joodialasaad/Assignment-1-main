/**
 *
 * @file interrupts.cpp
 * @author Sasisekhar Govind
 *
 */

#include<interrupts.hpp>

int main(int argc, char** argv) {

    //vectors is a C++ std::vector of strings that contain the address of the ISR
    //delays  is a C++ std::vector of ints that contain the delays of each device
    //the index of these elemens is the device number, starting from 0
    auto [vectors, delays] = parse_args(argc, argv);
    std::ifstream input_file(argv[1]);

    std::string trace;      //!< string to store single line of trace file
    std::string execution;  //!< string to accumulate the execution output

    /******************ADD YOUR VARIABLES HERE*************************/
    int current_time = 0;


    /******************************************************************/

    //parse each line of the input trace file
    while(std::getline(input_file, trace)) {
        auto [activity, duration_intr] = parse_trace(trace);

        /******************ADD YOUR SIMULATION CODE HERE*************************/
        if(activity == "CPU"){
            execution += std::to_string(current_time) + ", " + std::to_string(duration_intr) + ", CPU Burst\n";
            current_time += duration_intr;
        }

        else if (activity == "SYSCALL") {
            auto [boiler, new_time] = intr_boilerplate(current_time, duration_intr, 10, vectors);
            execution += boiler;
            current_time = new_time;

            execution += std::to_string(current_time) + ", 40, SYSCALL: run the ISR (device driver)\n";
            current_time += 40;

            execution += std::to_string(current_time) + ", " + std::to_string(delays[duration_intr]) + ", transfer data from device to memory\n";
            current_time += delays[duration_intr];

            execution += std::to_string(current_time) + ", 376, check for errors\n";
            current_time += 376;
        }



        /************************************************************************/

    }

    input_file.close();

    write_output(execution);

    return 0;
}
