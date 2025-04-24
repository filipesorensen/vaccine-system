/**
 * @file main.c
 * @brief Main function of the vaccine management system.
 *
 * Reads commands from standard input and processes them.
 */

#include "project.h"
#include "utils.h"
#include "vaccine.h"
#include "inoculations.h"
#include "time.h"

/**
  * @brief Main function.
  *
  * Reads commands from standard input and processes them.
  *
  * @param argc Argument count.
  * @param argv Argument vector.
  * @return 0 on success.
  */
int main(int argc, char *argv[]){
    char buf[BUFMAX];
    Sys sys = {0, {}, NULL, NULL, ENG, {1, 1, 2025}};
     
    if (argc > 1) {
        if(strcmp(argv[1], "pt") == 0){
            sys.state = PT;
        }
    }
 
    while(fgets(buf, BUFMAX, stdin)){
        buf[strcspn(buf, "\n")] = 0;
        switch(buf[0]){
            case 'q':{
                 freeInocList(sys.inocHead);
                 return 0;
            }
            case 'c': createBatch(&sys, buf); break;
            case 'l': listVaccines(&sys, buf); break;
            case 'a': applyVaccine(&sys, buf); break;
            case 'r': deleteVaccines(&sys, buf); break;
            case 'd': deleteHistory(&sys, buf); break;
            case 'u': listClientHistory(&sys, buf); break;
            case 't': timeControl(&sys, buf); break;
        }
    }
     
    freeInocList(sys.inocHead);
    return 0;
}