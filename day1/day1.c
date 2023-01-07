// https://adventofcode.com/2022/day/1

#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <errno.h>
#include <string.h>

#define INPUT_FILE "M:\\Repos\\personal\\Advent of Code\\AOC-2022\\day1\\day1_input.txt"

void main(void) {
    // input stream character buffers
    char c_curr = '\0';
    char c_prev = '\0';

    // elf variables
    uint32_t cal_temp = 0; // temp value for calculation
    uint32_t cal_curr = 0; // current elf carrying
    uint32_t cal_max  = 0; // most calories carried
    uint32_t elf_max  = 0; // elf # carrying the most
    uint32_t elf_curr = 1; // current elf #, text friendly start

    // loop logic to comply with MISRA R15.4
    bool   error     = false;
    fpos_t error_pos = 0;

    // attempt to open values file
    FILE *stream = fopen(INPUT_FILE, "r"); 

    if(stream != NULL) {

        // keep looping unless we encounter
        // an out of range input value
        while(!error) {
            c_prev = c_curr;
            c_curr = fgetc(stream);
            if(feof(stream)) {
                // end of file
                cal_curr += cal_temp;
                if(cal_curr > cal_max) {
                    cal_max = cal_curr;
                    elf_max = elf_curr;
                }
                break;
            }

            // ignore any carriage returns
            // probably not needed, but to be safe
            if(c_curr == '\r') {
                continue;
            }

            if(c_curr == '\n') {
                // new line = new value 
                
                // add our temp value to the current
                // elf total and reset the temp value
                cal_curr += cal_temp;
                cal_temp = 0;

                // check for new elf
                if(c_prev == '\n') {
                    // check if current elf is the winner
                    if(cal_curr > cal_max) {
                        cal_max = cal_curr;
                        elf_max = elf_curr;
                    }
                    
                    // reset the current elf calories
                    cal_curr = 0;

                    // increment current elf
                    elf_curr++;
                }
                continue;
            }

            // verify bounds of input
            if(c_curr >= '0' && c_curr <= '9') {
                // calculate temp value
                cal_temp = (cal_temp * 10) 
                         + (uint8_t)(c_curr - '0');
            } else {
                // input file has an invalid character
                error = true;
                fgetpos(stream, &error_pos);
            }
        }

        // close file
        fclose(stream);

        if(!error) {
            printf("Elf #%u is carrying the most at %u calories!\n", 
                   elf_max, cal_max);
        } else {
            printf("Error in input stream. char '%c' @ position %lld\n", 
                   c_curr, (int64_t)error_pos);
        }
    } else {
        printf("fopen(): %s\n", strerror(errno));
    }
}