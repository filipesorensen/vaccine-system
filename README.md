# 1. Introduction

This project implements a vaccine management system to track vaccine batches and their inoculations.  The system allows you to register vaccine doses and manage their administration.

Interaction with the program occurs through lines beginning with a single letter (command) followed by a number of arguments that depend on the command.  You may assume all input respects the expected types (e.g. where an integer is expected, no letters will appear).

The available commands are listed in the table below:

| Command | Action |
| ------- | ------ |
|         |        |

| **q** | Quit the program                       |
| ----- | -------------------------------------- |
| **c** | Create a new vaccine batch             |
| **l** | List available vaccine batches         |
| **a** | Apply a vaccine dose to a user         |
| **r** | Remove availability of a vaccine batch |
| **d** | Delete records of vaccine applications |
| **u** | List applications for a user           |
| **t** | Advance the simulated date             |

## 2. Problem Specification

The goal is to build a system that manages vaccine inoculations for users.  The system maintains a collection of vaccine batches and their available doses, and records each inoculation event.

Each vaccine **batch** is described by:

- A vaccine name (no whitespace allowed, max **50 bytes** in UTF‑8)
- A batch identifier (up to **20** hexadecimal digits, only `0`–`9` and `A`–`F`)
- An expiration date (day-month-year)
- A positive integer number of doses

Each **inoculation** record contains:

- A user name (may include spaces or tabs; if so, it must be enclosed in double quotes)
- The batch identifier from which the dose was taken
- The vaccine name
- The date of application

There can be at most **1000** vaccine batches in the system.  There is no fixed limit on the number of inoculations or on the length of a user name (in practice names won’t exceed **200 bytes**).  The program must use memory only as needed and must not use global variables.  If memory is exhausted, the program should exit gracefully, printing `No memory.` and freeing all dynamically allocated memory.

## 3. Input Format

Commands are read from standard input, one per line.  Each line starts with a command letter, followed by required or optional parameters separated by whitespace.  In the specification below, `<...>` indicates a required parameter, `[...]` indicates optional parameters, and `{...}` indicates repetition.  Ellipses (`...`) denote that a parameter may be repeated.

If a command can produce multiple errors, only the first error should be reported.

### Command `q`

- **Input**: `q`
- **Output**: *none*
- **Action**: Terminate the program.

### Command `c`

- **Input**: `c <batch> <day>-<month>-<year> <doses> <vaccine-name>`
- **Output**: `<batch>` on success
- **Errors**:
  - `too many vaccines`
  - `duplicate batch number`
  - `invalid batch`
  - `invalid name`
  - `invalid date`
  - `invalid quantity`

### Command `l`

- **Input**: `l [<vaccine-name> { <vaccine-name> } ]`
- **Output**: For each matching batch:
  ```
  <vaccine-name> <batch> <day>-<month>-<year> <available-doses> <applications>
  ```
  Sorted by expiration date (oldest first), then by batch identifier.  If no names are given, lists all batches.  If names are provided, lists only those batches (in the order given).
- **Errors**:
  - `<vaccine-name>: no such vaccine`

### Command `a`

- **Input**: `a <user-name> <vaccine-name>`
- **Output**: `<batch>` of the applied dose
- **Errors**:
  - `no stock`
  - `already vaccinated`

### Command `r`

- **Input**: `r <batch>`
- **Output**: `<number-of-applied-doses>`
- **Errors**:
  - `<batch>: no such batch`

### Command `d`

- **Input**: `d <user-name> [ <application-date> [ <batch> ]]`
- **Output**: `<number-of-deleted-applications>`
- **Errors**:
  - `<user-name>: no such user`
  - `invalid date`
  - `<batch>: no such batch`

### Command `u`

- **Input**: `u [<user-name>]`
- **Output**: For each application:
  ```
  <user-name> <batch> <day>-<month>-<year>
  ```
  Sorted by application date.
- **Errors**:
  - `<user-name>: no such user`

### Command `t`

- **Input**: `t [<day>-<month>-<year>]`
- **Output**: The current date `day-month-year`
- **Errors**:
  - `invalid date`

---

**Language mode**: If the program is invoked as `./proj pt`, all error messages must be printed in Portuguese; otherwise they appear in English.

**Restrictions**: Only the C standard library headers `<stdio.h>`, `<stdlib.h>`, `<ctype.h>` and `<string.h>` may be used.  The keywords `goto`, `extern`, and the standard `qsort` function are forbidden.

