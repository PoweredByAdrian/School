from typing import List
from enums import OperandsTypes, ExitCodes, DataTypes
from helper import app_exist, valid_mathsymbol, valid_comparablesymbol
from program import Program
from models import InstructionArgument, Symbol, Label as LabelModel
from sys import stdin, stderr


class InstructionBase():
    """ Base class for each instruction. """

    expected_args = []
    def __init__(self, args: List[InstructionArgument], opcode: str):
        if len(self.expected_args) != len(args):
            app_exist(ExitCodes.INVALID_XML_STRUCT)
        self.opcode = opcode
        self.args = args
    def execute(self, program: Program):
        raise NotImplementedError

class MathInstructionBase(InstructionBase):
    """ Base class for all mathematical instructions. """

    expected_args = [OperandsTypes.VARIABLE, OperandsTypes.SYMBOL, OperandsTypes.SYMBOL]
    def compute(self, symb1: Symbol, symb2: Symbol):
        raise NotImplementedError
    def execute(self, program: Program):
        symb1 = program.get_symb(self.args[1])
        symb2 = program.get_symb(self.args[2])
        valid_mathsymbol(symb1, symb2)
        result = self.compute(symb1, symb2)
        program.set_var(self.args[0], result)

class StackMathInstructionBase(InstructionBase):
    """ Extended version using a stack of base class for all mathematical instructions """

    expected_args = []
    def compute(self, symb1: Symbol, symb2: Symbol):
        raise NotImplementedError
    def execute(self, program: Program):
        symbols = program.pop_stack(2)
        valid_mathsymbol(symbols[1], symbols[0])
        result = self.compute(symbols[1], symbols[0])
        program.data_stack.append(result)

class ComparableInstruction(InstructionBase):
    """ Base class for comparison instructions. """

    expected_args = [OperandsTypes.VARIABLE, OperandsTypes.SYMBOL, OperandsTypes.SYMBOL]
    allowedTypes = [DataTypes.INT, DataTypes.BOOL, DataTypes.FLOAT, DataTypes.STRING]
    def compare(self, symb1: Symbol, symb2: Symbol) -> bool:
        raise NotImplementedError
    def execute(self, program: Program):
        symb1 = program.get_symb(self.args[1])
        symb2 = program.get_symb(self.args[2])
        valid_comparablesymbol(symb1, symb2, self.allowedTypes)
        result = Symbol(DataTypes.BOOL, self.compare(symb1, symb2))
        program.set_var(self.args[0], result)

class StackComparableInstruction(InstructionBase):
    """ Base class for comparison instructions. """

    expected_args = []
    allowedTypes = [DataTypes.INT, DataTypes.BOOL, DataTypes.FLOAT, DataTypes.STRING]
    def compare(self, symb1: Symbol, symb2: Symbol) -> bool:
        raise NotImplementedError
    def execute(self, program: Program):
        symbols = program.pop_stack(2)
        valid_comparablesymbol(symbols[1],
                                    symbols[0], self.allowedTypes)

        result = Symbol(DataTypes.BOOL, self.compare(symbols[1], symbols[0]))
        program.data_stack.append(result)

class Move(InstructionBase):
    """ Inserting a constant into a variable. """

    expected_args = [OperandsTypes.VARIABLE, OperandsTypes.SYMBOL]
    def execute(self, program: Program):
        symb = program.get_symb(self.args[1], False)
        program.set_var(self.args[0], symb)

class Createframe(InstructionBase):
    """ Creating a new temporary frame. """

    def execute(self, program: Program):
        program.TF = dict()

class Pushframe(InstructionBase):
    """ Moving the temporary frame to the top of the frame stack. """

    def execute(self, program: Program):
        if program.TF is None:
            app_exist(ExitCodes.INVALID_FRAME)
        program.LF_Stack.append(program.TF)
        program.TF = None

class Popframe(InstructionBase):
    """ Moving a frame from the top of the frame stack to the local frame. """

    def execute(self, program: Program):
        if len(program.LF_Stack) == 0:
            app_exist(ExitCodes.INVALID_FRAME)
        program.TF = program.LF_Stack.pop()

class Defvar(InstructionBase):
    """ Variable definition. """

    expected_args = [OperandsTypes.VARIABLE]
    def execute(self, program: Program):
        arg = self.args[0]
        if program.exists_var(arg):
            app_exist(ExitCodes.SEMANTIC_ERROR)
        program.set_var(arg, None, True)

class Return(InstructionBase):
    """ Instruction for jumping to a position defined on the top of the call stack. """

    def execute(self, program: Program):
        if len(program.call_stack) == 0:
            app_exist(ExitCodes.UNDEFINED_VALUE)
        program.instruction_pointer = program.call_stack.pop()

class PushS(InstructionBase):
    """ Storing a value on top of the data stack. """

    expected_args = [OperandsTypes.SYMBOL]
    def execute(self, program: Program):
        symb = program.get_symb(self.args[0])
        program.data_stack.append(symb)

class PopS(InstructionBase):
    """ Getting a value from the top of the data stack and storing it in a variable. """

    expected_args = [OperandsTypes.VARIABLE]
    def execute(self, program: Program):
        if len(program.data_stack) == 0:
            app_exist(ExitCodes.UNDEFINED_VALUE)
        program.set_var(self.args[0], program.pop_stack(1)[0])

class Add(MathInstructionBase):
    """ Addition """

    def compute(self, symb1: Symbol, symb2: Symbol):
        return Symbol(symb1.data_type, symb1.value + symb2.value)

class Sub(MathInstructionBase):
    """ Subtraction """

    def compute(self, symb1: Symbol, symb2: Symbol):
        return Symbol(symb1.data_type, symb1.value - symb2.value)

class Mul(MathInstructionBase):
    """ Multiplication """

    def compute(self, symb1: Symbol, symb2: Symbol):
        return Symbol(symb1.data_type, symb1.value * symb2.value)

class IDiv(MathInstructionBase):
    """ Integer division """

    def compute(self, symb1: Symbol, symb2: Symbol):
        if symb2.value == 0:
            app_exist(ExitCodes.INVALID_OPERAND_VALUE)
        return Symbol(DataTypes.INT, symb1.value // symb2.value)

class Lt(ComparableInstruction):
    """ Less """

    def compare(self, symb1: Symbol, symb2: Symbol) -> bool:
        return symb1.value < symb2.value

class Gt(ComparableInstruction):
    """ Greather """

    def compare(self, symb1: Symbol, symb2: Symbol) -> bool:
        return symb1.value > symb2.value

class Eq(ComparableInstruction):
    """ Equal """

    allowedTypes = [DataTypes.INT, DataTypes.BOOL, DataTypes.STRING, DataTypes.NIL]
    def compare(self, symb1: Symbol, symb2: Symbol) -> bool:
        return symb1.equals(symb2)

class And(ComparableInstruction):
    """ And """

    allowedTypes = [DataTypes.BOOL]
    def compare(self, symb1: Symbol, symb2: Symbol) -> bool:
        return symb1.value and symb2.value

class Or(ComparableInstruction):
    """ Or """

    allowedTypes = [DataTypes.BOOL]
    def compare(self, symb1: Symbol, symb2: Symbol) -> bool:
        return symb1.value or symb2.value

class Not(InstructionBase):
    """ Negation """

    expected_args = [OperandsTypes.VARIABLE, OperandsTypes.SYMBOL]
    def execute(self, program: Program):
        symb = program.get_symb(self.args[1])
        if not symb.is_bool():
            app_exist(ExitCodes.INVALID_DATA_TYPE)
        result = Symbol(DataTypes.BOOL, not symb.value)
        program.set_var(self.args[0], result)

class Int2Char(InstructionBase):
    """ Conversion of a number to a character. """

    expected_args = [OperandsTypes.VARIABLE, OperandsTypes.SYMBOL]
    def execute(self, program: Program):
        var = self.args[0]
        symb = program.get_symb(self.args[1])
        if not symb.is_int():
            app_exist(ExitCodes.INVALID_DATA_TYPE)
        try:
            char = chr(symb.value)
            program.set_var(var, Symbol(DataTypes.STRING, char))
        except Exception:
            app_exist(ExitCodes.INVALID_STRING_OPERATION)

class Stri2Int(InstructionBase):
    """ Conversion of a character at a certain position to a number. """

    expected_args = [OperandsTypes.VARIABLE, OperandsTypes.SYMBOL, OperandsTypes.SYMBOL]
    def execute(self, program: Program):
        string = program.get_symb(self.args[1])
        index = program.get_symb(self.args[2])
        if string is None or not string.is_string() or index is None or not index.is_int():
            app_exist(ExitCodes.INVALID_DATA_TYPE)
        try:
            ordinary = ord(string.value[index.value])
            program.set_var(self.args[0], Symbol(DataTypes.INT, ordinary))
        except IndexError:
            app_exist(ExitCodes.INVALID_STRING_OPERATION)

class Read(InstructionBase):
    """ Reading data from standard input or from a file specified using the --input parameter. """

    expected_args = [OperandsTypes.VARIABLE, OperandsTypes.TYPE]
    def execute(self, program: Program):
        try:
            if program.input == stdin:
                line = input().rstrip()
            else:
                line = program.input.readline().rstrip()
        except Exception:
            line = None
        arg_type = self.args[1]
        if line is None:
            program.set_var(self.args[0], Symbol(DataTypes.NIL, None))
        elif arg_type.type is bool:
            program.set_var(self.args[0], Symbol(DataTypes.BOOL, line.lower() == 'true'))
        elif arg_type.type is str:
            program.set_var(self.args[0], Symbol(DataTypes.STRING, line))
        elif arg_type.type is int:
            try:
                temp_val = int(line)
                if str(temp_val) != line:
                    program.set_var(self.args[0], Symbol(DataTypes.NIL, None))
                else:
                    program.set_var(self.args[0], Symbol(
                        DataTypes.INT, temp_val))
            except ValueError:
                program.set_var(self.args[0], Symbol(DataTypes.NIL, None))
        elif arg_type.type is float:
            try:
                try:
                    float_value = float(line)
                except ValueError:
                    float_value = float.fromhex(line)
            except Exception:
                program.set_var(self.args[0], Symbol(DataTypes.NIL, None))
            else:
                program.set_var(self.args[0], Symbol(DataTypes.FLOAT, float_value))

class Write(InstructionBase):
    """ Instruction for printing a symbol to standard output. """

    expected_args = [OperandsTypes.SYMBOL]
    def execute(self, program: Program):
        symb = program.get_symb(self.args[0])
        if symb.is_nil():
            print('', end='')
        elif symb.is_bool():
            print(str(symb.value).lower(), end='')
        elif symb.is_float():
            print(symb.value.hex(), end='')
        else:
            print(symb.value, end='')

class Concat(InstructionBase):
    """ Concatenation of two strings. """

    expected_args = [OperandsTypes.VARIABLE, OperandsTypes.SYMBOL, OperandsTypes.SYMBOL]
    def execute(self, program: Program):
        symb1 = program.get_symb(self.args[1])
        if not symb1.is_string():
            app_exist(ExitCodes.INVALID_DATA_TYPE)
        symb2 = program.get_symb(self.args[2])
        if not symb2.is_string():
            app_exist(ExitCodes.INVALID_DATA_TYPE)
        result = Symbol(DataTypes.STRING, symb1.value + symb2.value)
        program.set_var(self.args[0], result)

class Strlen(InstructionBase):
    """ Getting the length of a string. """

    expected_args = [OperandsTypes.VARIABLE, OperandsTypes.SYMBOL]
    def execute(self, program: Program):
        string = program.get_symb(self.args[1])
        if not string.is_string():
            app_exist(ExitCodes.INVALID_DATA_TYPE)
        string_length = Symbol(DataTypes.INT, len(string.value))
        program.set_var(self.args[0], string_length)

class Getchar(InstructionBase):
    """ Getting a character from a string at a certain position. """

    expected_args = [OperandsTypes.VARIABLE, OperandsTypes.SYMBOL, OperandsTypes.SYMBOL]
    def execute(self, program: Program):
        string = program.get_symb(self.args[1])
        index = program.get_symb(self.args[2])
        if not string.is_string() or not index.is_int():
            app_exist(ExitCodes.INVALID_DATA_TYPE)
        try:
            result = Symbol(DataTypes.STRING, string.value[index.value])
            program.set_var(self.args[0], result)
        except IndexError:
            app_exist(ExitCodes.INVALID_STRING_OPERATION)

class Setchar(InstructionBase):
    """ Modifying a character at a certain position. """

    expected_args = [OperandsTypes.VARIABLE, OperandsTypes.SYMBOL, OperandsTypes.SYMBOL]
    def execute(self, program: Program):
        variable = program.get_var(self.args[0])
        index = program.get_symb(self.args[1])
        toModify = program.get_symb(self.args[2])
        if variable is None:
            app_exist(ExitCodes.UNDEFINED_VALUE)
        if not index.is_int() or not variable.is_string() or\
                not toModify.is_string():
            app_exist(ExitCodes.INVALID_DATA_TYPE)
        if len(toModify.value) == 0 or index.value >= len(variable.value):
            app_exist(ExitCodes.INVALID_STRING_OPERATION)
        try:
            result = "{}{}{}".format(variable.value[:index.value], toModify.value[0], variable.value[index.value + 1:])
            program.set_var(self.args[0],
                            Symbol(DataTypes.STRING, result))
        except IndexError:
            app_exist(ExitCodes.INVALID_STRING_OPERATION)

class Type(InstructionBase):
    """ Determining the data type. """

    expected_args = [OperandsTypes.VARIABLE, OperandsTypes.SYMBOL]
    def execute(self, program: Program):
        symb = program.get_symb(self.args[1], False)
        if symb is None:
            program.set_var(self.args[0], Symbol(DataTypes.STRING, ''))
        else:
            program.set_var(self.args[0], Symbol(
                DataTypes.STRING, symb.data_type.value))

class Label(InstructionBase):
    """ Label. """

    expected_args = [OperandsTypes.LABEL]
    def __init__(self, args: List, opcode: str):
        InstructionBase.__init__(self, args, opcode)
        self.name = args[0]

class Jump(InstructionBase):
    """ Jump to a label. """

    expected_args = [OperandsTypes.LABEL]
    def execute(self, program: Program):
        label: LabelModel = self.args[0]
        if label.name not in program.labels:
            app_exist(ExitCodes.SEMANTIC_ERROR)
        program.instruction_pointer = program.labels[label.name]

class Call(Jump):
    """ Save the current position to the call stack and jump to a label. """

    def execute(self, program: Program):
        program.call_stack.append(program.instruction_pointer)
        Jump.execute(self, program)

class Jumpifeq(Jump):
    """ Jump to a label if two values are equal. """

    expected_args = [OperandsTypes.LABEL, OperandsTypes.SYMBOL, OperandsTypes.SYMBOL]
    def execute(self, program: Program):
        symb1 = program.get_symb(self.args[1])
        symb2 = program.get_symb(self.args[2])
        if symb2.equal_type(symb1.data_type) or symb1.is_nil() or\
                symb2.is_nil():
            if symb2.equals_value(symb1):
                Jump.execute(self, program)
        else:
            app_exist(ExitCodes.INVALID_DATA_TYPE)

class Jumpifneq(Jump):
    """ Jump to a label if two values are not equal. """

    expected_args = [OperandsTypes.LABEL, OperandsTypes.SYMBOL, OperandsTypes.SYMBOL]
    def execute(self, program: Program):
        symb1 = program.get_symb(self.args[1])
        symb2 = program.get_symb(self.args[2])
        if symb2.equal_type(symb1.data_type) or symb1.is_nil() or\
                symb2.is_nil():
            if not symb2.equals_value(symb1):
                Jump.execute(self, program)
        else:
            app_exist(ExitCodes.INVALID_DATA_TYPE)

class Exit(InstructionBase):
    """ Premature termination of the program with a return code. """

    expected_args = [OperandsTypes.SYMBOL]
    def execute(self, program: Program):
        symb = program.get_symb(self.args[0], True)
        if not symb.is_int():
            app_exist(ExitCodes.INVALID_DATA_TYPE)
        elif symb.value < 0 or symb.value > 49:
            app_exist(ExitCodes.INVALID_OPERAND_VALUE)
        else:
            program.exit(symb.value)

class DPrint(InstructionBase):
    """ Printing a symbol to standard error output. """

    expected_args = [OperandsTypes.SYMBOL]
    def execute(self, program: Program):
        symb = program.get_symb(self.args[0])
        print(symb.value, file=stderr)

class Break(InstructionBase):
    """ Printing the current state of the program. """

    def execute(self, program: Program):
        print(program.get_state(), file=stderr)

class Int2Float(InstructionBase):
    """ Converting an integer value to a float. """

    expected_args = [OperandsTypes.VARIABLE, OperandsTypes.SYMBOL]
    def execute(self, program: Program):
        symb = program.get_symb(self.args[1])
        if not symb.is_int():
            app_exist(ExitCodes.INVALID_DATA_TYPE)
        symbol = Symbol(DataTypes.FLOAT, float(symb.value))
        program.set_var(self.args[0], symbol)

class Float2Int(InstructionBase):
    """ Converting a float value to an integer. """

    expected_args = [OperandsTypes.VARIABLE, OperandsTypes.SYMBOL]
    def execute(self, program: Program):
        symb = program.get_symb(self.args[1])
        if not symb.is_float():
            app_exist(ExitCodes.INVALID_DATA_TYPE)
        symbol = Symbol(DataTypes.INT, int(symb.value))
        program.set_var(self.args[0], symbol)

class Div(MathInstructionBase):
    """ Division using float. """

    def compute(self, symb1: Symbol, symb2: Symbol):
        if symb2.value == 0.0:
            app_exist(ExitCodes.INVALID_OPERAND_VALUE)
        return Symbol(DataTypes.FLOAT, symb1.value / symb2.value)

class Clears(InstructionBase):
    """ Clearing all data in the data stack. """

    expected_args = []
    def execute(self, program: Program):
        program.data_stack = list()

class Adds(StackMathInstructionBase):
    """ Extended version using a stack of addition """

    def compute(self, symb1: Symbol, symb2: Symbol):
        return Symbol(symb1.data_type, symb1.value + symb2.value)

class Subs(StackMathInstructionBase):
    """ Extended version using a stack of subtraction """

    def compute(self, symb1: Symbol, symb2: Symbol):
        return Symbol(symb1.data_type, symb1.value - symb2.value)

class Muls(StackMathInstructionBase):
    """ Extended version using a stack of Multiplication """

    def compute(self, symb1: Symbol, symb2: Symbol):
        return Symbol(symb1.data_type, symb1.value * symb2.value)

class IDivs(StackMathInstructionBase):
    """ Extended version using a stack of integer division """

    def compute(self, symb1: Symbol, symb2: Symbol):
        return Symbol(symb1.data_type, symb1.value // symb2.value)

class Divs(StackMathInstructionBase):
    """ Extended version using a stack of division using float. """

    def compute(self, symb1: Symbol, symb2: Symbol):
        return Symbol(symb1.data_type, symb1.value / symb2.value)

class Lts(StackComparableInstruction):
    """ Extended version using a stack of less """

    def compare(self, symb1: Symbol, symb2: Symbol) -> bool:
        return symb1.value < symb2.value

class Gts(StackComparableInstruction):
    """ Extended version using a stack of ereater """

    def compare(self, symb1: Symbol, symb2: Symbol) -> bool:
        return symb1.value > symb2.value

class Eqs(StackComparableInstruction):
    """ Extended version using a stack of equal """

    allowedTypes = [DataTypes.INT, DataTypes.BOOL, DataTypes.STRING, DataTypes.NIL]
    def compare(self, symb1: Symbol, symb2: Symbol) -> bool:
        return symb1.equals(symb2)

class Ands(StackComparableInstruction):
    """ Extended version using a stack of and """

    allowedTypes = [DataTypes.BOOL]
    def compare(self, symb1: Symbol, symb2: Symbol) -> bool:
        return symb1.value and symb2.value

class Ors(StackComparableInstruction):
    """ Extended version using a stack of or """

    allowedTypes = [DataTypes.BOOL]
    def compare(self, symb1: Symbol, symb2: Symbol) -> bool:
        return symb1.value or symb2.value

class Nots(InstructionBase):
    """ Extended version using a stack of negation """

    expected_args = []
    def execute(self, program: Program):
        symb = program.pop_stack(1)[0]
        if not symb.is_bool():
            app_exist(ExitCodes.INVALID_DATA_TYPE)
        result = Symbol(DataTypes.BOOL, not symb.value)
        program.data_stack.append(result)

class Int2Chars(InstructionBase):
    """ Extended version using a stack of conversion of a number to a character """

    expected_args = []
    def execute(self, program: Program):
        symb = program.pop_stack(1)[0]
        if not symb.is_int():
            app_exist(ExitCodes.INVALID_DATA_TYPE)
        try:
            char = chr(symb.value)
        except Exception:
            app_exist(ExitCodes.INVALID_STRING_OPERATION)
        else:
            program.data_stack.append(Symbol(DataTypes.STRING, char))

class Stri2Ints(InstructionBase):
    """ Extended version using a stack of conversion of a character at a certain position to a number.  """

    expected_args = []
    def execute(self, program: Program):
        symbols = program.pop_stack(2)
        index = symbols[0]
        string = symbols[1]
        if not string.is_string() or not index.is_int():
            app_exist(ExitCodes.INVALID_DATA_TYPE)
        try:
            ordinary = ord(string.value[index.value])
        except IndexError:
            app_exist(ExitCodes.INVALID_STRING_OPERATION)
        else:
            program.data_stack.append(Symbol(DataTypes.INT, ordinary))

class Jumpifeqs(Jump):
    """ Extended version using a stack of Jump to a label or tag if two values are equal. """

    expected_args = [OperandsTypes.LABEL]
    def execute(self, program: Program):
        symbols = program.pop_stack(2)
        symb2 = symbols[0]
        symb1 = symbols[1]
        if symb2.equal_type(symb1.data_type) or symb1.is_nil() or\
                symb2.is_nil():
            if symb2.equals_value(symb1):
                Jump.execute(self, program)
        else:
            app_exist(ExitCodes.INVALID_DATA_TYPE)

class Jumpifneqs(Jump):
    """ Extended version using a stack of Jump to a label or tag if two values are not equal. """

    expected_args = [OperandsTypes.LABEL]
    def execute(self, program: Program):
        symbols = program.pop_stack(2)
        symb2 = symbols[0]
        symb1 = symbols[1]
        if symb2.equal_type(symb1.data_type) or symb1.is_nil() or\
                symb2.is_nil():
            if not symb2.equals_value(symb1):
                Jump.execute(self, program)
        else:
            app_exist(ExitCodes.INVALID_DATA_TYPE)

class Int2Floats(InstructionBase):
    """ Extended version using a stack of Converting an integer value to a float. """

    expected_args = []
    def execute(self, program: Program):
        symb = program.pop_stack(1)[0]
        if not symb.is_int():
            app_exist(ExitCodes.INVALID_DATA_TYPE)
        symbol = Symbol(DataTypes.FLOAT, float(symb.value))
        program.data_stack.append(symbol)

class Float2Ints(InstructionBase):
    """ Extended version using a stack of Converting a float value to an integer. """

    expected_args = []
    def execute(self, program: Program):
        symb = program.pop_stack(1)[0]
        if not symb.is_float():
            app_exist(ExitCodes.INVALID_DATA_TYPE)
        symbol = Symbol(DataTypes.INT, int(symb.value))
        program.data_stack.append(symbol)


OPCODE_TO_CLASS_MAP = {
    "MOVE": Move,
    "CREATEFRAME": Createframe,
    "PUSHFRAME": Pushframe,
    "POPFRAME": Popframe,
    "DEFVAR": Defvar,
    "CALL": Call,
    "RETURN": Return,
    "PUSHS": PushS,
    "POPS": PopS,
    "ADD": Add,
    "SUB": Sub,
    "MUL": Mul,
    "IDIV": IDiv,
    "LT": Lt,
    "GT": Gt,
    "EQ": Eq,
    "AND": And,
    "OR": Or,
    "NOT": Not,
    "INT2CHAR": Int2Char,
    "STRI2INT": Stri2Int,
    "READ": Read,
    "WRITE": Write,
    "CONCAT": Concat,
    "STRLEN": Strlen,
    "GETCHAR": Getchar,
    "SETCHAR": Setchar,
    "TYPE": Type,
    "LABEL": Label,
    "JUMP": Jump,
    "JUMPIFEQ": Jumpifeq,
    "JUMPIFNEQ": Jumpifneq,
    "EXIT": Exit,
    "DPRINT": DPrint,
    "BREAK": Break,
    "INT2FLOAT": Int2Float,
    "FLOAT2INT": Float2Int,
    "DIV": Div,
    "CLEARS": Clears,
    "ADDS": Adds,
    "SUBS": Subs,
    "MULS": Muls,
    "IDIVS": IDivs,
    "DIVS": Divs,
    "LTS": Lts,
    "GTS": Gts,
    "EQS": Eqs,
    "ANDS": Ands,
    "ORS": Ors,
    "NOTS": Nots,
    "INT2CHARS": Int2Chars,
    "STRI2INTS": Stri2Ints,
    "JUMPIFNEQS": Jumpifneqs,
    "JUMPIFEQS": Jumpifeqs,
    "INT2FLOATS": Int2Floats,
    "FLOAT2INTS": Float2Ints
}
