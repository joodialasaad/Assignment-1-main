/**
 *
 * @file interrupts.cpp
 * @author Aryan Singh
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
      while (std::getline(inputfile, trace)) {
        // Use parse_trace (not parsetrace)
        auto [activity, duration_intr] = parse_trace(trace);

        if (activity == "CPU") {
            execution += std::to_string(current_time) + ", " +
                         std::to_string(duration_intr) +
                         ", CPU burst of " + std::to_string(duration_intr) + " ms\n";
            current_time += duration_intr;
        }
        else if (activity == "SYSCALL") {
            int devnum = duration_intr;
            // Interrupt boilerplate steps using macros ADDR_BASE and VECTOR_SIZE
            // Step 1: switch to kernel mode (1 ms)
            execution += std::to_string(current_time) + ", 1, switch to kernel mode\n";
            current_time += 1;
            // Step 2: context saved (10 ms)
            execution += std::to_string(current_time) + ", 10, context saved\n";
            current_time += 10;
            // Step 3: find vector in memory (1 ms)
char vectoraddr[10];
std::snprintf(vectoraddr, sizeof(vectoraddr), "0x%04X",
        ADDR_BASE + devnum * VECTOR_SIZE);
execution += std::to_string(current_time) + ", 1, find vector " +
        std::to_string(devnum) + " in memory position " + vectoraddr +
        "\n";
current_time += 1;
// Step 4: load ISR address into the PC (1 ms).  Guard against
// references beyond the provided vector array.
std::string isr = (static_cast<std::size_t>(devnum) < vectors.size())
? vectors.at(devnum)
                : std::string("<unknown>");
execution += std::to_string(current_time) + ", 1, load address " + isr +
        " into the PC\n";
current_time += 1;
// Step 5: execute the ISR (40 ms)
execution += std::to_string(current_time) + ", 40, execute ISR for device " +
        std::to_string(devnum) + "\n";
current_time += 40;
// Step 6: return from interrupt (1 ms)
execution += std::to_string(current_time) + ", 1, IRET - end interrupt service\n";
current_time += 1;






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
            auto [boiler, new_time] = intr_boilerplate(current_time, duration_intr, 10, vectors); // runs the interrupt boilerplate to handle SYSCALL and update current time

            execution += boiler;

            execution += std::to_string(new_time) + ", " + std::to_string(40) + ", SYSCALL: run the ISR (device driver)\n";
            new_time += 40;

            // transfer data from device to memory
            execution += std::to_string(new_time) + ", " + std::to_string(delays[duration_intr]) + ", transfer data from device to memory\n";
            new_time += delays[duration_intr];

            // check for errors
            execution += std::to_string(new_time) + ", " + std::to_string(376) + ", check for errors\n";
            new_time += 376;

            current_time = new_time;
        }


        else if (activity == "END_IO") {
            auto [boiler, new_time] = intr_boilerplate(current_time, duration_intr, 10, vectors);
            execution += boiler;
            execution += std::to_string(new_time) + ", " + std::to_string(40) + ", ENDIO: run the ISR (device driver)\n";
            new_time += 40;

            // simulate checking device status
            execution += std::to_string(new_time) + ", " + std::to_string(delays[duration_intr]) + ", check device status\n";
            new_time += delays[duration_intr];

            current_time = new_time;
        }




        /************************************************************************/

    }

    input_file.close();

    write_output(execution);

    return 0;
}
