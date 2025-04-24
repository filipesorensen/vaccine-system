#ifndef PROJECT_H
#define PROJECT_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
 
/* Constants definitions */
#define MAXNAMEVACC 51       /**< Maximum length of vaccine name in bytes */
#define MAXSIZEBATCH 21         /**< Maximum length of batch (lote) string */
#define MAXBATCHES 1000          /**< Maximum number of vaccine batches */
#define BUFMAX 65536           /**< Maximum input line length */
 
/* Error messages in Portuguese */
#define PTE2MANYVAC "demasiadas vacinas"        /**< Too many vaccines */
#define PTEDUPBATCH "número de lote duplicado"  /**< Duplicate batch number */
#define PTEINVBATCH "lote inválido"             /**< Invalid batch */
#define PTEINVNAME "nome inválido"              /**< Invalid name */
#define PTEINVDATE "data inválida"              /**< Invalid date */
#define PTEINVQUANT "quantidade inválida"       /**< Invalid quantity */
#define PTENOVACCINE "vacina inexistente"       /**< No such vaccine */
#define PTENOSTOCK "esgotado"                   /**< Out of stock */
#define PTEALRVACC "já vacinado"                /**< Already vaccinated */
#define PTENOBATCH "lote inexistente"           /**< No such batch */
#define PTENOUSER "utente inexistente"          /**< No such user */
#define PTENOMEMORY "sem memória"               /**< Out of memory */
 
/* Error messages in English */
#define ENGE2MANYVAC "too many vaccines"
#define ENGEDUPBATCH "duplicate batch number"
#define ENGEINVBATCH "invalid batch"
#define ENGEINVNAME "invalid name"
#define ENGEINVDATE "invalid date"
#define ENGEINVQUANT "invalid quantity"
#define ENGENOVACCINE "no such vaccine"
#define ENGENOSTOCK "no stock"
#define ENGEALRVACC "already vaccinated"
#define ENGENOBATCH "no such batch"
#define ENGENOUSER "no such user"
#define ENGENOMEMORY "memory exausted"
 
#define ENG 1
#define PT 0
 
/**
  * @brief Represents a date.
  */
typedef struct date{
    int day;
    int month;
    int year;
}Date;
 
/**
  * @brief Represents a vaccine batch.
  */
typedef struct vaccine{
    char name[MAXNAMEVACC];   /**< Vaccine name */
    char batch[MAXSIZEBATCH];     /**< Batch identifier */
    Date expir;              /**< Expiration date */
    int doses;                /**< Available doses */
    int applys;           /**< Number of inoculations made */
}Vaccine;
 
 /**
  * @brief Represents a vaccine inoculation.
  */
typedef struct inoculation{
    char *userName;          /**< Name of the user (dynamically allocated) */
    char batch[MAXSIZEBATCH];       /**< Batch identifier used */
    char vType[MAXNAMEVACC];   /**< Name of the vaccine applied */
    Date aplication;              /**< Date of inoculation */
}Inoculation;
 
/**
  * @brief Node for the linked list of inoculations.
  */
typedef struct inocNode{
    Inoculation inoc;            /**< Inoculation record */
    struct inocNode *next;      /**< Pointer to the next node */
}InocNode;
 
/**
  * @brief System structure containing vaccine batches and inoculations.
  */
typedef struct sys{
    int cntV;                 /**< Count of vaccine batches */
    Vaccine arr[MAXBATCHES];  /**< Array of vaccine batches */
    InocNode *inocHead;       /**< Head of inoculations list */
    InocNode *inocTail;       /**< Tail of inoculations list */
    int state;               /**< Language state (ENG or PT) */
    Date tcurr;              /**< Current simulated date */
}Sys;

#endif /* PROJECT_H */