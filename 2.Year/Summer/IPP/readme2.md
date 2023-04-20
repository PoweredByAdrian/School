Implementation Documentation for Task 2 of IPP 2022/2023
Name and Surname: Adrián Horváth
Login: xhorva14

## Interpretation of IPPCode22 Language (interpret.py)

The interpretation of IPPCode22 language is carried out by taking an XML file as input, which represents the IPPCode22 language. The XML file can be provided either through standard input or from a file using the `--source` parameter. The interpreter performs syntax and lexical analysis of the content and then proceeds with the interpretation of the program.

### Data Loading and Analysis

The `InstructionsParser` class, which contains static methods, is used for loading data for interpretation. After parsing command line parameters, the `InstructionsParser.parse_file` method is called, which reads the XML file and performs content validation. After successful loading of the XML content, the following checks are performed:

* Existence of the `program` element, `language` attribute, and its correctness: The program element, which represents the entire program in the XML file, must exist and contain the language attribute with the correct value indicating the version of the IPPCode language being used.
* Loading of individual elements representing instructions.
  * When loading individual instructions, the following argument checks are performed:
    * Variable names and labels format: The names of variables and labels are checked for correct format according to the IPPCode22 language specifications.
    * `nil` type value check: For instructions that expect a `nil` type argument, the value provided is checked to be only `nil`, as per the language specifications.
    * Data type correctness: For instructions that expect specific data types such as `string`, `bool`, or `int`, the values provided are checked for correctness according to the expected data type.
    * Verification of expected vs actual argument type: The expected type of argument for an instruction is verified against the actual input provided to ensure compatibility and correctness.

The `InstructionsParser.parse_file` function returns an object of type `Dict[int, InstructionBase]`, representing an ordered associative array, where the key is the value of the `order` attribute and the value is an instance of a class representing an instruction.

#### Representation of Instructions

Instructions in the IPPCode22 language are implemented as classes that inherit from the base class InstructionBase. This design approach ensures that all instructions have the same methods implemented, such as `execute()` and `__init__()`. Additionally, it helps to reduce the amount of duplicate code, as common functionalities can be implemented in the base class and reused by the instructions.
For further code simplification, additional base classes, such as a base class for arithmetic operations, were implemented.

### Interpretation

The `Program` class is the core of the interpretation, encapsulating important properties required for interpretation.

After successful loading of instructions and finding all labels, the `Run` method is called, which invokes the `execute` method on individual instructions.

Using classes to represent instructions also allows for easy extensibility and maintainability of the interpreter. New instructions can be added by simply creating a new class that inherits from the `InstructionBase` or relevant base class, and implementing the necessary methods and logic for that instruction. This modular approach makes it convenient to handle different types of instructions and their functionalities, and allows for easy customization and expansion of the interpreter to support new instructions or language features.

Overall, the use of classes and inheritance in the implementation of instructions in the IPPCode22 language interpreter helps to ensure code consistency, maintainability, and extensibility, providing a solid foundation for accurate interpretation of the program instructions.

### Extensions

The following extensions have been implemented in the interpreter:

#### FLOAT - Support for the float data type

The interpreter now supports the `FLOAT` data type, allowing for working with floating-point numbers in instructions.

To support the `FLOAT` data type, new instructions have been added to the interpreter, specifically designed for working with floating-point numbers. These instructions may include operations such as floating-point addition, subtraction, multiplication, and division...

#### STACK - Support for instructions that work with a data stack

The interpreter now includes support for instructions that work with a data stack, referred to as the `STACK`, providing a convenient way to store and retrieve values in a temporary storage area during the execution of the program.

The `STACK` instructions include operations such as pushing values onto the stack, popping values from the stack and performing stack-based arithmetic operations, such as addition, subtraction, multiplication, and division.