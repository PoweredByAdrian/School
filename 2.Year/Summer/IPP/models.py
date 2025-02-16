from enums import OperandsTypes, Frames, DataTypes
from typing import Any, List


class InstructionArgument():
    """ Generic model for operands. """
    arg_type: OperandsTypes

class Symbol(InstructionArgument):
    """ Constant or variable. """

    def __init__(self, data_type: DataTypes, value: Any):
        self.arg_type = OperandsTypes.SYMBOL
        self.data_type = data_type
        self.value = value
    def one_of_types(self, types: List[DataTypes]):
        return any(self.equal_type(type) for type in types)
    def equal_type(self, type: DataTypes):
        return self.data_type == type
    def is_nil(self):
        return self.data_type == DataTypes.NIL
    def is_bool(self):
        return self.data_type == DataTypes.BOOL
    def is_float(self):
        return self.data_type == DataTypes.FLOAT
    def is_string(self):
        return self.data_type == DataTypes.STRING
    def is_int(self):
        return self.data_type == DataTypes.INT
    def equals(self, symb: 'Symbol'):
        if self.is_nil():
            return symb.is_nil()
        elif symb.is_nil():
            return False
        return self.equal_type(symb.data_type) and self.equals_value(symb)
    def equals_value(self, symb: 'Symbol'):
        return self.value == symb.value


class Variable(InstructionArgument):
    """ Operand variable. """

    def __init__(self, frame: Frames, value: Any):
        self.arg_type = OperandsTypes.VARIABLE
        self.frame = frame
        self.value = value


class Type(InstructionArgument):
    """ Operand type. """

    def __init__(self, type: Any):
        self.arg_type = OperandsTypes.TYPE
        self.type = type


class Label(InstructionArgument):
    """ Operand label. """

    def __init__(self, name: str):
        self.arg_type = OperandsTypes.LABEL
        self.name = name
