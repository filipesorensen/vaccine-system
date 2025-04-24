/**
 * @file vaccine.c
 * @brief Functions to manage vaccine batches.
 */

#include "vaccine.h"
#include "utils.h"

/**
  * @brief Creates a new vaccine batch and adds it to the system.
  *
  * Reads input parameters from the provided string and inserts
  * a new batch into the system's list. Prints error messages if necessary.
  *
  * @param sys Pointer to the system.
  * @param in Input string containing the batch data.
  */
void createBatch(Sys *sys, char *in){
    Vaccine vacc;
    int i;
    char *tempBatch = (char *)malloc(BUFMAX * sizeof(char));
    char *tempName = (char *)malloc(BUFMAX * sizeof(char));
 
    if(tempBatch == NULL || tempName == NULL){
        puts(sys->state == PT ? PTENOMEMORY : ENGENOMEMORY);
        return;
    }
 
    sscanf(in, "%*s %s %d-%d-%d %d %s", tempBatch,
        &vacc.expir.day, &vacc.expir.month, &vacc.expir.year,
        &vacc.doses, tempName);
    if(sys->cntV >= MAXBATCHES){
        puts(sys->state == PT ? PTE2MANYVAC : ENGE2MANYVAC);
        free(tempBatch);
        free(tempName);
        return;
    }
 
    for(i = 0; i < sys->cntV; i++){
         if(!strcmp(sys->arr[i].batch, tempBatch)){
            puts(sys->state == PT ? PTEDUPBATCH : ENGEDUPBATCH);
            free(tempBatch);
            free(tempName);
            return;
        }
    }
 
    if(strlen(tempBatch) > MAXSIZEBATCH){
        puts(sys->state == PT ? PTEINVBATCH : ENGEINVBATCH);
        free(tempBatch);
        free(tempName);
        return;
    }
 
    strcpy(vacc.batch, tempBatch);
 
    for(i = 0; i < (int)strlen(vacc.batch); i++){
        if(!((vacc.batch[i] >= '0' && vacc.batch[i] <= '9') ||
            (vacc.batch[i] >= 'A' && vacc.batch[i] <= 'F'))){
            puts(sys->state == PT ? PTEINVBATCH : ENGEINVBATCH);
            free(tempBatch);
            free(tempName);
            return;
        }
    }
 
     if(strlen(tempName) > MAXNAMEVACC){
        puts(sys->state == PT ? PTEINVNAME : ENGEINVNAME);
        free(tempBatch);
        free(tempName);
        return;
    }
 
    strcpy(vacc.name, tempName);
 
    if(!verifyDate(vacc.expir) ||
        (compareDates(sys->tcurr, vacc.expir) > 0)){
        puts(sys->state == PT ? PTEINVDATE : ENGEINVDATE);
        free(tempBatch);
        free(tempName);
        return;
    }
 
    if(vacc.doses < 1){
        puts(sys->state == PT ? PTEINVQUANT : ENGEINVQUANT);
        free(tempBatch);
        free(tempName);
        return;
    }
 
    vacc.applys = 0;
    sys->arr[sys->cntV] = vacc;
    sys->cntV += 1;
    printf("%s\n", vacc.batch);
    free(tempBatch);
    free(tempName);
}

/**
  * @brief Merges two sorted subarrays of vaccines.
  *
  * The subarrays are arr[l..m] and arr[m+1..r]. Sorting is done
  * first by expiration date (validade) and then by batch (lote).
  *
  * @param arr Array of vaccines.
  * @param l Left index.
  * @param m Middle index.
  * @param r Right index.
  */
void merge(Vaccine arr[], int l, int m, int r){
    int i, j, k;
    int n1 = m - l + 1;
    int n2 = r - m;
 
    Vaccine L[n1], R[n2];
 
    for (i = 0; i < n1; i++){
        L[i] = arr[l + i];
    }
    for (j = 0; j < n2; j++){
        R[j] = arr[m + 1 + j];
    }
 
    i = 0, j = 0, k = l;
    while (i < n1 && j < n2){
        if(compareDates(L[i].expir, R[j].expir) < 0 ||
            (compareDates(L[i].expir, R[j].expir) == 0 &&
            strcmp(L[i].batch, R[j].batch) <= 0)){
            arr[k] = L[i];
            i++;
         }else{
            arr[k] = R[j];
            j++;
        }
        k++;
    }
 
    while(i < n1){
        arr[k] = L[i];
        i++;
        k++;
    }
    while(j < n2){
        arr[k] = R[j];
        j++;
        k++;
    }
}

/**
  * @brief Performs merge sort on an array of vaccines.
  *
  * @param arr Array of vaccines.
  * @param l Left index.
  * @param r Right index.
  */
void mergeSort(Vaccine arr[], int l, int r){
    if(l < r){
        int m = l + (r - l) / 2;
        mergeSort(arr, l, m);
        mergeSort(arr, m + 1, r);
        merge(arr, l, m, r);
    }
}

/**
  * @brief Lists vaccine batches.
  *
  * If no vaccine name is provided after the command, lists all batches.
  * If names are provided, lists only the matching batches, in the order given.
  *
  * @param sys Pointer to the system.
  * @param in Input string.
  */
void listVaccines(Sys *sys, char *in){
    int i, j, numVac = 0;
    char *nameV;
    char namesVaccs[MAXBATCHES][MAXNAMEVACC];
 
    if(sys->cntV > 0){
        mergeSort(sys->arr, 0, sys->cntV - 1);
    }
 
    in[strcspn(in, "\n")] = '\0';
 
    nameV = strtok(in, " ");
    while((nameV = strtok(NULL, " ")) != NULL){
        strcpy(namesVaccs[numVac], nameV);
        numVac++;
    }
 
    if(numVac == 0){
        for(i = 0; i < sys->cntV; i++){
            printf("%s %s %.2d-%.2d-%d %d %d\n", sys->arr[i].name,
                sys->arr[i].batch, sys->arr[i].expir.day,
                sys->arr[i].expir.month, sys->arr[i].expir.year,
                sys->arr[i].doses, sys->arr[i].applys);
        }
        return;
    }
 
    for(i = 0; i < numVac; i++){
        int vaccFound = 0;
        for(j = 0; j < sys->cntV; j++){
            if(strcmp(sys->arr[j].name, namesVaccs[i]) == 0){
                printf("%s %s %.2d-%.2d-%d %d %d\n", sys->arr[j].name,
                    sys->arr[j].batch, sys->arr[j].expir.day,
                    sys->arr[j].expir.month, sys->arr[j].expir.year,
                    sys->arr[j].doses, sys->arr[j].applys);
                vaccFound = 1;
            }
        }
        if(!vaccFound){
            printf("%s: ", namesVaccs[i]);
            puts(sys->state == PT ? PTENOVACCINE : ENGENOVACCINE);
        }
    }
}

/**
  * @brief Deletes a vaccine batch.
  *
  * If no inoculations have been made from the batch, the batch is removed.
  * Otherwise, its available doses become zero.
  *
  * @param sys Pointer to the system.
  * @param in Input string containing the batch identifier.
  */
void deleteVaccines(Sys *sys, char *in){
    char *batchToken = strtok(in, " ");
    batchToken = strtok(NULL, " ");
     
    int index = -1;
    for(int i = 0; i < sys->cntV; i++){
        if(strcmp(sys->arr[i].batch, batchToken) == 0){
            index = i;
            break;
        }
    }
     
    if(index == -1){
        printf("%s: ", batchToken);
        puts(sys->state == PT ? PTENOBATCH : ENGENOBATCH);
        return;
    }
     
    if(sys->arr[index].applys == 0){
        for(int i = index; i < sys->cntV - 1; i++){
            sys->arr[i] = sys->arr[i + 1];
        }
        sys->cntV--;
    }else{
        sys->arr[index].doses = 0;
    }
     
    printf("%d\n", sys->arr[index].applys);
}