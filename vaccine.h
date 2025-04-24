#ifndef VACCINE_H
#define VACCINE_H

#include "project.h"

/* Function prototypes related to vaccine batches */
void createBatch(Sys *sys, char *in);
void merge(Vaccine arr[], int l, int m, int r);
void mergeSort(Vaccine arr[], int l, int r);
void listVaccines(Sys *sys, char *in);
void deleteVaccines(Sys *sys, char *in);

#endif /* VACCINE_H */