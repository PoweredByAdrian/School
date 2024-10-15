Implementační dokumentace k 1. úloze do IPP 2023/2024
Jméno a příjmení: Adrian Horvath
Login: xhorva14


# IPPcode24 Source Code Analyzer (parse.py)

This script (`parse.py`) analyzes IPPcode24 source code, providing lexical and syntactic analysis. It parses instructions and generates an XML representation of the program.

Additionally, the script offers the capability to generate statistics related to the analyzed source code.

## Table of Contents

- [Introduction](#Introduction)
  - [Prerequisites](#prerequisites)
  - [Usage](#usage)
- [Options](#options)
- [Exit Codes](#exit-codes)
  - [IPPcode24 Source Code Analysis Exit Codes](#ippcode24-source-code-analysis-exit-codes)
- [Code Structure](#code-structure)
  - [STATP extension classes](#statp-extension-classes)
- [Examples](#examples)

## Introduction
The IPPcode24 Source Code Analyzer processes source code written in the IPPcode24 language. It performs lexical and syntactic analysis, collects various statistics, and generates an XML representation of the program. The script supports command-line options for customizing the output and provides exit codes for error handling.

### Prerequisites

- Python 3.10 or higher

###  Usage

```bash
python3 parse.py [OPTIONS] < input_file.ippcode24
```

## Options

 `--help`: Show the help message and exit.


As part of the script `STATP` extension's functionality, the following statistics can be generated:

- `--stats=FILENAME`: Specify an output file for statistics. This option can be used multiple times. Valid stats flags include:
  - `--loc`: Number of lines of code (excluding comments and empty lines).
  - `--comments`: Number of comment lines.
  - `--jumps`: Total number of jump instructions (conditional and unconditional).
  - `--labels`: Number of unique defined labels.
  - `--fwjumps`: Number of forward jumps.
  - `--backjumps`: Number of backward jumps.
  - `--badjumps`: Number of jumps to non-existent labels (to be implemented).
  - `--eol`: Output a newline character.
  - `--frequent`: The most frequently used command(s), comma-separated if there's a tie.
  - `--print=STRING`: Output a custom string.



## Exit Codes

- 10: Script parameters error or use of a forbidden combination of parameters.
- 12: Error opening output files for writing.

### IPPcode24 Source Code Analysis Exit Codes

- 21: Incorrect or missing header in the source code.
- 22: Unknown or incorrect operation code in the source code.
- 23: Other lexical or syntactic error in the source code.

## Code Structure (NVP Extension)

The script's code is organized and utilizes various classes to manage different aspects of the analysis process:

### Design Overview

The code leverages classes for modularity and improved code organization. The Main class acts as the central coordinator, delegating tasks to other classes responsible for specific functionalities like instruction processing, error handling, and data collection.

- `DataTypes` (Enum): Enumeration of data types used in the IPPcode24 source code, such as `VARIABLE`, `LABEL`, `SYMBOL`, and `TYPE`.
- `ErrorExit` (Class): Handles error exit codes with static methods and exiting the script accordingly.
- `InstructionProcessor` (Class): Class responsible for processing IPPcode24 instructions.
  - Validates instruction names and arguments
  - Generates XML representation of the program using minidom
- `InstructionCollection` (Class): Collection of IPPcode24 instructions and their allowed arguments.
- `Main` (Class): The main entry point of the program.
  - Parses command-line arguments.
  - Processes instructions line by line.
  - Generates XML output and writes statistics to specified files.
- `Instruction` (Class): with its name, ID, arguments, expected parameters...

### STATP extension classes
- `CommandTracker` (Class): Tracks command usage and provides methods for counting and displaying usage statistics.
- `StatCollector` (Class): This class is responsible for gathering and storing various statistics pertaining to the analyzed source code. It encompasses data like lines of code, comments, labels, jumps, and the most frequently used commands.

#### Relationships
- `Main`: utilizes instances of `StatCollector`, `InstructionProcessor`, and `ErrorExit` to perform code analysis.

- `InstructionProcessor`: uses `InstructionCollection` to validate IPPCode24 instructions and `CommandTracker` to track command usage and `StatCollector` to update statistics.

- `Instruction`: relies on `InstructionCollection` to validate IPPCode24 instruction's allowed agruments

## Examples

Analyze IPPcode24 source code and print XML representation to standard output:

```bash
python3 parse.py < input_file.ippcode24
```

Generate statistics and direct output to a file:

```bash
python3 parse.py --stats=stats.txt --loc --comments --jumps < input_file.ippcode24
```
