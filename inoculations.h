#ifndef INOCULATIONS_H
#define INOCULATIONS_H

#include "project.h"


/* Function prototypes related to inoculations */
void applyVaccine(Sys *sys, char *in);
void freeInocList(InocNode *head);
void listClientHistory(Sys *sys, char *in);
void deleteHistory(Sys *sys, char *in);

#endif /* INOCULATIONS_H */