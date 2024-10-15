# IPPcode24 Source Code Analyzer (parse.py)
# Author: Adrian Horvath (xhorva14)
# Date: 2024-03-12
#
#This script analyzes IPPcode24 source code, offering lexical and syntactic analysis...

from enum import Enum
import sys
from xml.etree.ElementTree import Element
import re
from xml.dom import minidom

# Data types used in the code
class DataTypes(Enum):
    VARIABLE = "var"
    LABEL = "label"
    SYMBOL = "symbol"
    TYPE = "type"

# Class for handling error exit codes
class ErrorExit:
    
    # General exit codes
    INVALID_ARGUMENTS = 10
    FILE_OUTPUT_OPEN_ERROR = 12
    
    # Parse exit codes
    INVALID_OR_MISSING_HEADER = 21
    INVALID_INSTRUCTION = 22
    SYNTAX_ERROR = 23 

    @staticmethod
    def error(code):
        print(code)
        exit(code)

# Class for tracking command usage
class CommandTracker:
    """
    Tracks command usage and provides methods for counting and displaying usage statistics.
    """

    def __init__(self):
        self.commands = {}

    def execute_command(self, command):
        """
        Executes the given command (simulated) and updates its usage count.

        Args:
            command (str): The command to execute (simulated).
        """
        self.commands[command] = self.commands.get(command, 0) + 1

    def get_usage_stats(self):
        """
        Returns a dictionary containing command usage statistics.

        Returns:
            dict: A dictionary where keys are commands and values are their usage counts.
        """
        return self.commands


class StatCollector:
    """
    Collects and stores various statistics related to source code analysis, including command usage.

    Attributes:
        loc (int): Number of lines of code (excluding comments and empty lines).
        comments (int): Number of comment lines.
        labels (int): Number of unique defined labels.
        jumps (int): Total number of jump instructions (conditional and unconditional).
        fwjumps (int): Number of forward jumps.
        backjumps (int): Number of backward jumps.
        badjumps (int): Number of jumps to non-existent labels (to be implemented).
        frequent (str): The most frequently used command(s), comma-separated if there's a tie.
        command_tracker (CommandTracker): An instance of the CommandTracker class.
        defined_labels (set): Set of defined labels.
        all_labels (set): Set of all encountered labels.
    """

    def __init__(self):
        """
        Initializes an instance of the `StatCollector` class.
        """
        self.loc = 0
        self.comments = 0
        self.labels = 0
        self.jumps = 0
        self.fwjumps = 0
        self.backjumps = 0
        self.badjumps = 0
        self.frequent = ""
        self.command_tracker = CommandTracker()
        self.defined_labels = set()
        self.all_labels = set()

    def increment_stat(self, stat_name: str):
        """
        Increments the specified statistic by 1.

        Args:
            stat_name (str): The name of the statistic to increment.
        """

        setattr(self, stat_name, getattr(self, stat_name) + 1)
        
    def execute_command(self, command: str):
        """
        Executes the given command (simulated) and updates both code statistics and command usage.

        Args:
            command (str): The command to execute (simulated).
        """
        self.increment_stat("loc")
        self.command_tracker.execute_command(command)
        self.update_most_frequent_command()

    def update_most_frequent_command(self):
        """
        Updates the `frequent` attribute with the most frequently used command(s) in case of ties.

        - If there's a single most frequent command, sets `frequent` to that command.
        - If there's a tie for the most frequent command, stores all tied commands in a list and sets `frequent` to a comma-separated string of those commands.
        """
        usage_stats = self.command_tracker.get_usage_stats()
        if not usage_stats:
            return

        max_count = max(usage_stats.values())
        tied_commands = [cmd for cmd, count in usage_stats.items() if count == max_count]

        if len(tied_commands) == 1:
            self.frequent = tied_commands[0]
        else:
            self.frequent = ",".join(tied_commands)

    def define_label(self, label: str):
        """
        Registers a new label definition, updating the label count if unique.

        Args:
            label (str): The label name to define.
        """
        if label not in self.defined_labels:
            self.defined_labels.add(label)
            self.labels += 1
    
    def handle_jump(self, label: str):
        """
        Analyzes and classifies the jump based on the defined labels, marking potential forward jumps.

        Args:
            label (str): The label the jump targets.
        """
        self.jumps += 1
        if label not in self.defined_labels:
            self.all_labels.add(label)  # Track all encountered labels
        else:
            self.backjumps += 1

    def finalize_stats(self):
        """
        Classifies jumps as forward, backward, or bad based on all encountered labels.

        This method should be called after processing all code lines.
        """

        for label in self.all_labels:
            if label in self.defined_labels:
                self.fwjumps += 1
            else:
                self.badjumps += 1
    
    def get_stats(self) -> dict:
        """
        Returns a dictionary containing all collected statistics.

        This method should be called after calling `finalize_stats`.

        Returns:
            dict: A dictionary with keys corresponding to statistic names and values
                representing their counts.
        """

        self.finalize_stats()  # Ensure jump classification is complete
        
        stats = {
        "--loc": self.loc,
        "--comments": self.comments,
        "--labels": self.labels,
        "--jumps": self.jumps,
        "--fwjumps": self.fwjumps,
        "--backjumps": self.backjumps,
        "--badjumps": self.badjumps,
        "--frequent": self.frequent,
        }
        return stats
    
class Main:
    """
    Main class responsible for parsing IPPcode24 instructions, performing analysis, and generating output.
    """
    def __init__(self):
        """
        Initializes an instance of the `Main` class.
        """
        self.collector = StatCollector()
        self.haveHeader = False
        self.instructions = []
        self.counter = 1
        self.instructionCollection = {
            "MOVE": [DataTypes.VARIABLE, DataTypes.SYMBOL],
            "CREATEFRAME": [],
            "PUSHFRAME": [],
            "POPFRAME": [],
            "DEFVAR": [DataTypes.VARIABLE],
            "CALL": [DataTypes.LABEL],
            "RETURN": [],
            "PUSHS": [DataTypes.SYMBOL],
            "POPS": [DataTypes.VARIABLE],
            "ADD": [DataTypes.VARIABLE, DataTypes.SYMBOL, DataTypes.SYMBOL],
            "SUB": [DataTypes.VARIABLE, DataTypes.SYMBOL, DataTypes.SYMBOL],
            "MUL": [DataTypes.VARIABLE, DataTypes.SYMBOL, DataTypes.SYMBOL],
            "IDIV": [DataTypes.VARIABLE, DataTypes.SYMBOL, DataTypes.SYMBOL],
            "LT": [DataTypes.VARIABLE, DataTypes.SYMBOL, DataTypes.SYMBOL],
            "GT": [DataTypes.VARIABLE, DataTypes.SYMBOL, DataTypes.SYMBOL],
            "EQ": [DataTypes.VARIABLE, DataTypes.SYMBOL, DataTypes.SYMBOL],
            "AND": [DataTypes.VARIABLE, DataTypes.SYMBOL, DataTypes.SYMBOL],
            "OR": [DataTypes.VARIABLE, DataTypes.SYMBOL, DataTypes.SYMBOL],
            "NOT": [DataTypes.VARIABLE, DataTypes.SYMBOL],
            "INT2CHAR": [DataTypes.VARIABLE, DataTypes.SYMBOL],
            "STRI2INT": [DataTypes.VARIABLE, DataTypes.SYMBOL, DataTypes.SYMBOL],
            "READ": [DataTypes.VARIABLE, DataTypes.TYPE],
            "WRITE": [DataTypes.SYMBOL],
            "CONCAT": [DataTypes.VARIABLE, DataTypes.SYMBOL, DataTypes.SYMBOL],
            "STRLEN": [DataTypes.VARIABLE, DataTypes.SYMBOL],
            "GETCHAR": [DataTypes.VARIABLE, DataTypes.SYMBOL, DataTypes.SYMBOL],
            "SETCHAR": [DataTypes.VARIABLE, DataTypes.SYMBOL, DataTypes.SYMBOL],
            "TYPE": [DataTypes.VARIABLE, DataTypes.SYMBOL],
            "LABEL": [DataTypes.LABEL],
            "JUMP": [DataTypes.LABEL],
            "JUMPIFEQ": [DataTypes.LABEL, DataTypes.SYMBOL, DataTypes.SYMBOL],
            "JUMPIFNEQ": [DataTypes.LABEL, DataTypes.SYMBOL, DataTypes.SYMBOL],
            "EXIT": [DataTypes.SYMBOL],
            "DPRINT": [DataTypes.SYMBOL],
            "BREAK": []
        }

    def process_instruction(self, fields: list):
        """
        Processes a line of IPPcode24 instruction, performing validation and adding it to the list.

        Args:
            fields (list): List of strings representing the instruction and its arguments.
        """
        instructionName = fields[0].upper().strip()
        if instructionName == ".IPPCODE24" and not self.haveHeader:
            self.haveHeader = True
            return
        if not self.haveHeader:
            ErrorExit.error(ErrorExit.INVALID_OR_MISSING_HEADER)
        if instructionName not in self.instructionCollection:
            ErrorExit.error(ErrorExit.INVALID_INSTRUCTION)
        fields.pop(0)
        allowedarguments = self.instructionCollection[instructionName]
        instruction = Instruction(instructionName, self.counter, fields, allowedarguments, self.collector)
        self.instructions.append(instruction)
        self.collector.execute_command(instructionName)
        if instructionName in ("JUMP", "JUMPIFEQ", "JUMPIFNEQ"):
            self.collector.handle_jump(fields[0])
        if instructionName == "LABEL":
            self.collector.define_label(fields[0])
        self.counter += 1

    def generate_XML(self):
        """
        Generates XML representation of the program.
        """
        xml = minidom.Document()
        self.generate_XML_Pr(xml)
        xml.toprettyxml(indent="  ")
        print(xml.toxml())

    def generate_XML_Pr(self, document: minidom.Document):
        """
        Generates XML representation of program elements.

        Args:
            document: XML document object.
        """
        programElem = document.createElement("program")
        programElem.setAttribute('language', 'IPPcode24')

        for instruction in self.instructions:
            instructionElem = document.createElement("instruction")
            instruction.set_instruction_xml(document, instructionElem)

            programElem.appendChild(instructionElem)

        document.appendChild(programElem)

    def print_help(self):
        """
        Prints the help message for the program.
        """
        print("""IPP Projekt 2024
        Author: Adrian Horvath (xhorva14)
        Usage: python3 parse.py [OPTIONS] < input_file.ippcode24

This filter script (parse.py in Python 3.10) reads IPPcode24 source code from standard input, checks for lexical and syntactic correctness, and outputs the XML representation of the program to standard output as per the specification.

Options:
  --stats=FILENAME        Specify an output file for statistics. Can be used multiple times.
                         Valid stats flags: --loc, --comments, --jumps, --labels,
                         --fwjumps, --backjumps, --badjumps, --eol, --frequent, --print=STRING
  --help                  Show this help message and exit.

Common Exit Codes:
  10 - Script parameters error or use of a forbidden combination of parameters.
  12 - Error opening output files for writing.

IPPcode24 Source Code Analysis Exit Codes:
  21 - Incorrect or missing header in the source code.
  22 - Unknown or incorrect operation code in the source code.
  23 - Other lexical or syntactic error in the source code.

        """)
        
    def parse_arguments(self, args: list) -> list:
        """
        Parses command-line arguments and returns the parsed arguments.

        Args:
            args (list): List of command-line arguments.

        Returns:
            list: List of parsed arguments.
        """
        VALID_STATS = ["--loc", "--comments", "--jumps", "--labels", "--fwjumps", "--backjumps", "--badjumps", "--eol", "--frequent"]
        parsed_arguments = []  # List to store parsed arguments for each file
        current_filename = None  # Keeps track of the current filename
        seen_filenames = set()  # Set to track encountered filenames

        for arg in args:
            if arg.startswith("--stats="):
                # Start processing a new file
                if current_filename:
                    
                    parsed_arguments.append({"filename": current_filename, "flags": current_flags})

                current_filename = arg[len("--stats="):]  # Extract filename
                current_flags = []
                
                if current_filename in seen_filenames:
                    # Error: Duplicate filename encountered
                    ErrorExit.error(ErrorExit.FILE_OUTPUT_OPEN_ERROR)

                seen_filenames.add(current_filename)
                
            elif current_filename and arg in VALID_STATS:
                current_flags.append(arg)  # Add valid flag for current file
            elif current_filename and arg.startswith("--print="):
                current_flags.append(arg)
            else:
                ErrorExit.error(ErrorExit.INVALID_ARGUMENTS)

        parsed_arguments.append({"filename": current_filename, "flags": current_flags})

        return parsed_arguments
    
    def write_stats(self, arguments: list):
        """
        Writes the statistics to the specified files.

        Args:
            arguments (list): List of parsed arguments.
        """
        stats = self.collector.get_stats()
        for arg_dict in arguments:
            filename = arg_dict["filename"]
            flags = arg_dict["flags"]
            try:
                with open(filename, "w") as file:
                    for flag in flags:
                        if flag == "--eol":
                            file.write("\n")
                        elif flag.startswith("--print="):
                            file.write(flag[len("--print="):])
                            file.write("\n")
                        else:
                            file.write(str(stats[flag]))
                            file.write("\n")
            except Exception:
                ErrorExit.error(ErrorExit.FILE_OUTPUT_OPEN_ERROR)        
        

    def run(self):
        """
        Main function to run the program.
        """
        if len(sys.argv) > 1:
            arguments = self.parse_arguments(sys.argv[1:])
        
        for line in sys.stdin:
            line = line.strip()
            
            if '#' in line:
                self.collector.increment_stat("comments")
            # Remove comments and whitespaces
            instruction = line.split("#", 1)[0].strip()
            
            # Skip empty lines
            if not instruction:
                continue
            
            # Split into fields and store
            fields = instruction.split()
            self.process_instruction(fields)

        if self.haveHeader:
            self.generate_XML()
        else:
            ErrorExit.error(ErrorExit.INVALID_OR_MISSING_HEADER)
        
        if len(sys.argv) > 1:
            self.write_stats(arguments)


class Instruction:
    """
    Represents an instruction in the program.
    """
    def __init__(self, name: str, id: int, arguments: list, expectedParameters: list, collector: StatCollector):
        """
        Initializes an Instruction instance.

        Args:
            name (str): Name of the instruction.
            id (int): Identifier of the instruction.
            arguments (list): List of arguments for the instruction.
            expectedParameters (list): List of expected parameter types.
            collector (StatCollector): Instance of StatCollector for tracking statistics.
        """
        self.name = name
        self.id = id
        self.arguments = []
        self.collector = collector

        self.arguments_check(arguments, expectedParameters)

        for index, arg in enumerate(arguments):
            self.insert_argument(arg, expectedParameters[index])
    
    def arguments_check(self, arguments: list, expectedParameters: list):
        """
        Checks if the given arguments match the expected parameters.

        Args:
            arguments (list): List of arguments.
            expectedParameters (list): List of expected parameter types.
        """
        expectedArgumentsCount = len(expectedParameters)
        if len(arguments) != expectedArgumentsCount:
            ErrorExit.error(ErrorExit.SYNTAX_ERROR)

        for index, expectedType in enumerate(expectedParameters):
            if expectedType == DataTypes.LABEL:
                self.label_check(arguments[index])
            elif expectedType == DataTypes.VARIABLE:
                self.frame_check(arguments[index])
            elif expectedType == DataTypes.SYMBOL:
                self.symbol_check(arguments[index])
            elif expectedType == DataTypes.TYPE:
                if not self.type_valid(arguments[index], True):
                    ErrorExit.error(ErrorExit.SYNTAX_ERROR)
            else:
                raise Exception(f"Invalid type\n{expectedType}")
    
    def label_check(self, label: str):
        """
        Checks if the label is valid.

        Args:
            label (str): Label to check.
        """
        if not label:
            ErrorExit.error(ErrorExit.SYNTAX_ERROR)

        if not re.match(r"^[a-zA-Z_$&%*!?-][a-zA-Z0-9_$&%*!?-]*$", label):
            ErrorExit.error(ErrorExit.SYNTAX_ERROR)
    def frame_check(self, var: str):
        """
        Checks if the variable frame is valid.

        Args:
            var (str): Variable to check.
        """
        self.parameter_check(var)
        if not re.match(r"^[LGT]F@[_\-$&%*!?a-zA-Z]\S*$", var):
            ErrorExit.error(ErrorExit.SYNTAX_ERROR)

    def symbol_check(self, symbol: str):
        """
        Checks if the symbol is valid.

        Args:
            symbol (str): Symbol to check.
        """
        self.parameter_check(symbol)
        type, value = symbol.split('@', 1)

        if not (self.frame_valid(type) or self.type_valid(type)):
            ErrorExit.error(ErrorExit.SYNTAX_ERROR)

        if type == "int" and not re.match(r"^int@[-+]?(?:0(?:o[0-7]+|x[0-9A-Fa-f]+)|0|[0-9]*)$", symbol):
            ErrorExit.error(ErrorExit.SYNTAX_ERROR)

        if type == "nil" and symbol != "nil@nil":
            ErrorExit.error(ErrorExit.SYNTAX_ERROR)

        if type == "bool" and value not in ["true", "false"]:
            ErrorExit.error(ErrorExit.SYNTAX_ERROR)

        if type == "string" and not re.match(r"^string@(\\[0-9]{3}|[^#\\\\\s])*$", symbol):
            ErrorExit.error(ErrorExit.SYNTAX_ERROR)

    
    
    def type_valid(self, type: str, noNil=False) -> bool:
        """
        Checks if the given type is valid.

        Args:
            type (str): Type to check.
            noNil (bool): Flag to exclude 'nil' type.

        Returns:
            bool: True if the type is valid, False otherwise.
        """
        types = ["string", "int", "bool"]

        if not noNil:
            types.append("nil")

        return type in types

    def parameter_check(self, param: str):
        """
        Checks if the parameter contains an '@' symbol.

        Args:
            param (str): Parameter to check.
        """
        if '@' not in param:
            ErrorExit.error(ErrorExit.SYNTAX_ERROR)

    def frame_valid(self, frame: str) -> bool:
        """
        Checks if the frame is valid.

        Args:
            frame (str): Frame to check.

        Returns:
            bool: True if the frame is valid, False otherwise.
        """
        return frame in ["LF", "TF", "GF"]

    def insert_argument(self, arg: str, type: DataTypes):
        """
        Inserts an argument into the list.

        Args:
            arg (str): Argument value.
            type (DataTypes): Type of the argument.
        """
        arg = arg if arg is not None else ""

        if type == DataTypes.LABEL:
            self.arguments.append({"type": type.value, "value": arg})
        elif type == DataTypes.VARIABLE or type == DataTypes.TYPE:
            self.arguments.append({"type": type.value, "value": arg})
        elif type == DataTypes.SYMBOL:
            dataType, value = arg.split('@', 1)
            if self.frame_valid(dataType):
                self.insert_argument(arg, DataTypes.VARIABLE)
                return
            self.arguments.append({"type": dataType, "value": value})
        else:
            raise Exception(f"Invalid argument\n{arg}\n{type}")

    def set_instruction_xml(self, document: minidom.Document , xmlElem: Element):
        """
        Sets the XML representation of the instruction.

        Args:
            document: XML document object.
            xmlElem: XML element to set attributes and values.
        """
        xmlElem.setAttribute("order", str(self.id))
        xmlElem.setAttribute("opcode", self.name.upper())

        for i, argument in enumerate(self.arguments):
            argElem = document.createElement("arg" + str(i + 1))
            argElem.setAttribute("type", argument["type"])
            argElem.appendChild(document.createTextNode(argument["value"]))

            xmlElem.appendChild(argElem)

if __name__ == "__main__":
    program = Main()

    if "--help" in sys.argv:
        if len(sys.argv) > 2:
            ErrorExit.error(ErrorExit.INVALID_ARGUMENTS)

        program.print_help()
    else:
        program.run()