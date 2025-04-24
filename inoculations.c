/**
 * @file inoculations.c
 * @brief Functions to manage inoculations.
 */

#include "inoculations.h"
#include "vaccine.h"
#include "utils.h"

/**
  * @brief Applies a vaccine to a user.
  *
  * Reads the user's name (which may be quoted) and the vaccine name, then
  * updates the corresponding batch and records the inoculation.
  *
  * @param sys Pointer to the system.
  * @param in Input string.
  */
void applyVaccine(Sys *sys, char *in){
    char vaccName[MAXNAMEVACC];
    char tempName[BUFMAX];
    int i, j, exists = 0;
 
    char *ptr = in + 2;
     
    if (*ptr == '"'){
        ptr++;
        char *end = strchr(ptr, '"');
        if(!end){
            puts(sys->state == PT ? PTEINVNAME : ENGEINVNAME);
            return;
        }
        *end = '\0';
        strncpy(tempName, ptr, BUFMAX - 1);
        tempName[BUFMAX - 1] = '\0';
        ptr = end + 1;
    }else{
        sscanf(ptr, "%s", tempName);
        ptr += strlen(tempName);
    }
 
    while(*ptr == ' ' || *ptr == '\t'){
        ptr++;
    }
    sscanf(ptr, "%s", vaccName);
 
    if(sys->cntV > 0){
        mergeSort(sys->arr, 0, sys->cntV - 1);
    }else{
        puts(sys->state == PT ? PTENOSTOCK : ENGENOSTOCK);
        return;
    }
 
    for(i = 0; i < sys->cntV; i++){
        if(strcmp(sys->arr[i].name, vaccName) == 0 &&
            sys->arr[i].doses > 0) {
            j = i;
            exists = 1;
            break;
        }
    }
 
    if(!exists){
        puts(sys->state == PT ? PTENOSTOCK : ENGENOSTOCK);
        return;
    }
 
    InocNode *current = sys->inocHead;
    while(current != NULL){
        if(strcmp(current->inoc.userName, tempName) == 0 &&
        strcmp(current->inoc.vType, vaccName) == 0 &&
        compareDates(sys->tcurr, current->inoc.aplication) == 0){
            puts(sys->state == PT ? PTEALRVACC : ENGEALRVACC);
            return;
        }
        current = current->next;
    }
 
    sys->arr[j].doses -= 1;
    sys->arr[j].applys += 1;
 
    InocNode *newNode = malloc(sizeof(InocNode));
    if(newNode == NULL){
        puts(sys->state == PT ? PTENOMEMORY : ENGENOMEMORY);
        return;
    }
 
    newNode->inoc.aplication = sys->tcurr;
    newNode->inoc.userName = malloc(strlen(tempName) + 1);
    if(newNode->inoc.userName == NULL){
        free(newNode);
        puts(sys->state == PT ? PTENOMEMORY : ENGENOMEMORY);
        return;
    }
    strcpy(newNode->inoc.userName, tempName);
    strcpy(newNode->inoc.batch, sys->arr[j].batch);
    strcpy(newNode->inoc.vType, vaccName);
    newNode->next = NULL;
 
    /* Insert the new node at the end of the inoculations list */
    if(sys->inocHead == NULL){
        sys->inocHead = newNode;
        sys->inocTail = newNode;
    }else{
        sys->inocTail->next = newNode;
        sys->inocTail = newNode;
    }
     
    printf("%s\n", sys->arr[j].batch);
}

/**
  * @brief Frees the linked list of inoculations.
  *
  * @param head Pointer to the head of the list.
  */
void freeInocList(InocNode *head){
    InocNode *current = head;
    while (current != NULL){
        InocNode *temp = current;
        current = current->next;
        free(temp->inoc.userName);
        free(temp);
    }
}

/**
  * @brief Lists the inoculation history for a user.
  *
  * If no user is specified, lists all inoculations.
  *
  * @param sys Pointer to the system.
  * @param in Input string.
  */
void listClientHistory(Sys *sys, char *in){
    char *userName = NULL;
    char *ptr = in + 1;
    while(*ptr && isspace(*ptr)){
        ptr++;
    }
    if(*ptr == '"'){
        ptr++;
        char *end = strchr(ptr, '"');
        if(end == NULL){
            puts(sys->state == PT ? PTEINVNAME : ENGEINVNAME);
            return;
        }
        *end = '\0';
        userName = ptr;
    }else{
        userName = strtok(ptr, " ");
    }
 
    if(userName == NULL){
        InocNode *atual = sys->inocHead;
        while(atual != NULL){
            printf("%s %s %.2d-%.2d-%d\n",
                atual->inoc.userName,
                atual->inoc.batch,
                atual->inoc.aplication.day,
                atual->inoc.aplication.month,
                atual->inoc.aplication.year);
                atual = atual->next;
        }
     }else{
        InocNode *current = sys->inocHead;
        int encontrado = 0;
        while(current != NULL){
             if(strcmp(current->inoc.userName, userName) == 0){
                printf("%s %s %.2d-%.2d-%d\n",
                    current->inoc.userName,
                    current->inoc.batch,
                    current->inoc.aplication.day,
                    current->inoc.aplication.month,
                    current->inoc.aplication.year);
                encontrado = 1;
            }
            current = current->next;
        }
        if(!encontrado){
            printf("%s: ", userName);
            puts(sys->state == PT ? PTENOUSER : ENGENOUSER);
        }
    }
}
 
/**
  * @brief Deletes inoculation records for a given user.
  *
  * The deletion criteria may include a specific date and/or batch.
  *
  * @param sys Pointer to the system.
  * @param in Input string containing parameters.
  */
void deleteHistory(Sys *sys, char *in){
    char *userName = NULL;
    char *ptr = in + 1;
    while(*ptr && isspace(*ptr)){
        ptr++;
    }
    if(*ptr == '"'){
        ptr++;
        char *end = strchr(ptr, '"');
        if(end == NULL){
            printf("%s: ", ptr);
            puts(sys->state == PT ? PTEINVNAME : ENGEINVNAME);
            return;
        }
        *end = '\0';
        userName = ptr;
        ptr = end + 1;
    }else{
        userName = strtok(ptr, " ");
    }
     
    int hasDate = 0;
    Date date;
    char *dateToken = strtok(NULL, " ");
    if(dateToken != NULL){
        if(sscanf(dateToken, "%d-%d-%d",
            &date.day, &date.month, &date.year)!=3 ||
            !verifyDate(date) ||
            (compareDates(date, sys->tcurr) > 0)){
            puts(sys->state == PT ? PTEINVDATE : ENGEINVDATE);
            return;
        }
        hasDate = 1;
    }
     
    int hasBatch = 0;
    char *batchToken = strtok(NULL, " ");
    if(batchToken != NULL){
        hasBatch = 1;
        int foundBatch = 0;
        for(int i = 0; i < sys->cntV; i++){
            if(strcmp(sys->arr[i].batch, batchToken) == 0){
                foundBatch = 1;
                break;
            }
        }
        if(!foundBatch){
            printf("%s: ", batchToken);
            puts(sys->state == PT ? PTENOBATCH : ENGENOBATCH);
            return;
        }
    }
     
    InocNode *prev = NULL, *curr = sys->inocHead;
    int deletedCount = 0;
    int userFound = 0;
     
    while(curr != NULL){
        int match = 0;
        if(strcmp(curr->inoc.userName, userName) == 0){
            userFound = 1;
            match = 1;
            if(hasDate){
                if(curr->inoc.aplication.day != date.day ||
                    curr->inoc.aplication.month != date.month ||
                    curr->inoc.aplication.year != date.year){
                    match = 0;
                }
            }
            if(match && hasBatch){
                if(strcmp(curr->inoc.batch, batchToken) != 0){
                    match = 0;
                }
            }
        }
         
        if(match){
            InocNode *temp = curr;
            if(prev == NULL){
                sys->inocHead = curr->next;
            }else{
                prev->next = curr->next;
            }
            if(curr == sys->inocTail){
                sys->inocTail = prev;
            }
            curr = curr->next;
            free(temp->inoc.userName);
            free(temp);
            deletedCount++;
        }else{
            prev = curr;
            curr = curr->next;
        }
    }
     
    if(!userFound){
        printf("%s: ", userName);
        puts(sys->state == PT ? PTENOUSER : ENGENOUSER);
        return;
    }
     
    printf("%d\n", deletedCount);
}