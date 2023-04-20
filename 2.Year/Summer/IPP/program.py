from typing import List, IO, Dict
from models import Symbol, Variable
from enums import Frames, ExitCodes
from helper import app_exist
import instructions as instrs
from sys import stdin



class Program():
    """ Definition of the application to be executed. """

    def __init__(self, instructions: List, data_input: IO):

        self.input = data_input
        self.instruction_pointer = 0
        self.GF: Dict[str, Symbol] = dict()                 
        self.TF: Dict[str, Symbol] = None                   
        self.LF_Stack: List[Dict[str, Symbol]] = list()     
        self.data_stack: List[Symbol] = list()              
        self.call_stack: List[int] = list()                 
        self.exit_code = 0                                  
        self.labels: Dict[str, int] = dict()
        self.instructions: List[instrs.InstructionBase] = list()

        for instruction in instructions:
            if type(instruction) is instrs.Label:
                if instruction.name.name in self.labels: app_exist(ExitCodes.SEMANTIC_ERROR)
                self.labels.update(
                    {instruction.name.name: len(self.instructions)})
            else:
                self.instructions.append(instruction)

    def run(self):
        """ Program execution. """

        while len(self.instructions) > self.instruction_pointer:
            instruction = self.instructions[self.instruction_pointer]
            self.instruction_pointer += 1
            instruction.execute(self)

    def exists_var(self, var: Variable):
        """ Checking for the existence of a variable. """

        if var.frame == Frames.GLOBAL:
            return var.value in self.GF
        elif var.frame == Frames.TEMPORARY:
            if self.TF is None:
                app_exist(ExitCodes.INVALID_FRAME)
            return var.value in self.TF
        elif var.frame == Frames.LOCAL:
            if len(self.LF_Stack) == 0:
                app_exist(ExitCodes.INVALID_FRAME)
            return var.value in self.LF_Stack[-1]

    def set_var(self, var: Variable, value: Symbol, create: bool = False):
        """ Setting the value of a variable. """

        if not create and not self.exists_var(var):
            app_exist(ExitCodes.UNDEFINED_VARIABLE)
        if var.frame == Frames.GLOBAL:
            self.GF[var.value] = value
        elif var.frame == Frames.LOCAL:
            self.LF_Stack[-1][var.value] = value
        elif var.frame == Frames.TEMPORARY:
            self.TF[var.value] = value

    def get_var(self, var: Variable) -> Symbol:
        """ Getting the value of a variable. """

        if not self.exists_var(var):
            app_exist(ExitCodes.UNDEFINED_VARIABLE)
        if var.frame == Frames.TEMPORARY:
            return self.TF[var.value]
        elif var.frame == Frames.GLOBAL:
            return self.GF[var.value]
        elif var.frame == Frames.LOCAL:
            return self.LF_Stack[-1][var.value]

    def get_symb(self, symb: Symbol or Variable, required_value: bool = True):
        result = symb
        if type(symb) is Variable:
            result = self.get_var(symb)
        if required_value and result is None:
            app_exist(ExitCodes.UNDEFINED_VALUE)
        return result

    def exit(self, code: int):
        self.instruction_pointer = len(self.instructions)
        self.exit_code = code

    def pop_stack(self, required_count: int):
        """ Getting N values from stack. """
        
        if len(self.data_stack) < required_count:
            app_exist(ExitCodes.UNDEFINED_VALUE)
        stack_data = list()
        for i in range(required_count):
            stack_data.append(self.data_stack.pop())
        return stack_data
