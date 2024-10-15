from typing import IO, Dict, List
import instructions
from xml.etree.ElementTree import parse as parse_xml, ParseError, Element
from helper import app_exist
from enums import ExitCodes, OperandsTypes, Frames, DataTypes
import re
from models import (InstructionArgument, Symbol, Variable, Type, Label)


class InstructionsParser():
    """ Class for reading input XML data into internal structures. """

    @staticmethod
    def parse_file(file: IO) -> Dict[int, instructions.InstructionBase]:
        """ Loading XML data from a data stream and processing. 

        Perameters
        ----------
        file: IO
            Input data stream.

        Returns
        -------
        Dict[int, instructions.InstructionBase]
            Sorted dictionary where the key will be the content of the XML attribute "order".
        """
        try:
            xml_data = parse_xml(file).getroot()
            return InstructionsParser.parse(xml_data)
        except ParseError:
            app_exist(ExitCodes.INVALID_XML_FORMAT)

    @staticmethod
    def parse(xml_data: Element) -> Dict[int, instructions.InstructionBase]:
        """ Loading XML data and parsing it into structures.

        Parameters
        ----------
        xml_data: Element
            Root XML element.
        Returns
        -------
        Dict[int, instructions.InstructionBase]
            Sorted dictionary where the key will be the content of the XML attribute "order".
        """

        if xml_data.tag != 'program':
            app_exist(ExitCodes.INVALID_XML_STRUCT)
        if 'language' not in xml_data.attrib.get('language') != 'IPPcode23':
            app_exist(ExitCodes.INVALID_XML_STRUCT)
        result: Dict[int, instructions.InstructionBase] = dict()
        for element in list(xml_data):
            if element.tag != 'instruction':
                app_exist(ExitCodes.INVALID_XML_STRUCT)
            try:
                order = int(element.attrib.get('order'))
                if order <= 0:
                    app_exist(ExitCodes.INVALID_XML_STRUCT)
            except ValueError:
                app_exist(ExitCodes.INVALID_XML_STRUCT)
            except TypeError:
                app_exist(ExitCodes.INVALID_XML_STRUCT)
            if order in result:
                app_exist(ExitCodes.INVALID_XML_STRUCT)
            result[order] = InstructionsParser.instruction_parse(
                element)
        return dict(sorted(result.items()))

    @staticmethod
    def instruction_parse(element: Element) -> instructions.InstructionBase:
        """ Loading and processing of a single instruction.

        Parameters
        ----------
        element: Element
            XML element containing instruction data.
        Returns
        -------
        instructions.InstructionBase
            Instance of a class representing a specific instruction.
        """
        opcode = element.get('opcode')
        if opcode is None or len(opcode) == 0:
            app_exist(ExitCodes.INVALID_XML_STRUCT)
        opcode = opcode.upper()
        if opcode not in instructions.OPCODE_TO_CLASS_MAP:
            app_exist(ExitCodes.INVALID_XML_STRUCT)
        args = InstructionsParser.arguments_parse(element)
        instruction = instructions.OPCODE_TO_CLASS_MAP[opcode](args, opcode)
        for i in range(0, len(args)):
            expected = instruction.expected_args[i]
            real = args[i].arg_type
            is_invalid = False
            if expected == OperandsTypes.SYMBOL:
                if real != OperandsTypes.VARIABLE and real != OperandsTypes.SYMBOL:
                    is_invalid = True
            elif expected == OperandsTypes.VARIABLE and real != OperandsTypes.VARIABLE:
                is_invalid = True
            elif expected == OperandsTypes.LABEL and real != OperandsTypes.LABEL:
                is_invalid = True
            elif expected == OperandsTypes.TYPE and real != OperandsTypes.TYPE:
                is_invalid = True
            if is_invalid:
                app_exist(ExitCodes.INVALID_XML_STRUCT)
        return instruction

    @staticmethod
    def arguments_parse(element: Element) -> List[InstructionArgument]:
        """ Loading instruction parameters.

        Parameters
        ----------
        element: Element
            XML element containing instruction data.
        Returns
        -------
        List[InstructionArgument]
            Array of instruction parameter instances.
        """
        arg1 = element.findall('arg1')
        arg2 = element.findall('arg2')
        arg3 = element.findall('arg3')
        if len(arg1) > 1:
            app_exist(ExitCodes.INVALID_XML_STRUCT)
        elif len(arg2) > 1:
            app_exist(ExitCodes.INVALID_XML_STRUCT)
        elif len(arg3) > 1:
            app_exist(ExitCodes.INVALID_XML_STRUCT)

        if len(arg3) > 0 and (len(arg1) == 0 or len(arg2) == 0):
            app_exist(ExitCodes.INVALID_XML_STRUCT)
        if len(arg2) > 0 and len(arg1) == 0:
            app_exist(ExitCodes.INVALID_XML_STRUCT)
        args = list()
        if len(arg1) > 0:
            args.append(InstructionsParser.argument_parse(arg1[0]))
        if len(arg2) > 0:
            args.append(InstructionsParser.argument_parse(arg2[0]))
        if len(arg3) > 0:
            args.append(InstructionsParser.argument_parse(arg3[0]))
        return args

    @staticmethod
    def argument_parse(arg: Element) -> InstructionArgument:
        """ Processing of instruction parameters.

        Parameters
        ----------
        arg: Element
            XML element of a parameter.
        Returns
        -------
        InstructionArgument
            Processed parameter.
        """

        if len(list(arg)) > 0:
            app_exist(ExitCodes.INVALID_XML_STRUCT)
        arg_type = arg.attrib.get('type')
        arg_value = arg.text if arg.text is not None else ''
        if arg_type == OperandsTypes.LABEL.value:
            InstructionsParser.variable_validate(arg_value)
            return Label(arg_value)
        elif arg_type == OperandsTypes.VARIABLE.value:
            variable_parts = arg_value.split('@', 1)
            if len(variable_parts) == 2:
                InstructionsParser.scope_validate(variable_parts[0])
                InstructionsParser.variable_validate(variable_parts[1])
                if variable_parts[0] == 'GF':
                    return Variable(Frames.GLOBAL, variable_parts[1])
                elif variable_parts[0] == 'TF':
                    return Variable(Frames.TEMPORARY, variable_parts[1])
                elif variable_parts[0] == 'LF':
                    return Variable(Frames.LOCAL, variable_parts[1])
            else:
                app_exist(ExitCodes.INVALID_XML_STRUCT)
        elif arg_type == 'nil':
            if arg_value != 'nil':
                app_exist(ExitCodes.INVALID_XML_STRUCT)
            return Symbol(DataTypes.NIL, None)
        elif arg_type == 'int':
            try:
                return Symbol(DataTypes.INT, int(arg_value))
            except ValueError:
                app_exist(ExitCodes.INVALID_XML_STRUCT)
        elif arg_type == 'bool':
            if arg_value == 'true':
                return Symbol(DataTypes.BOOL, True)
            elif arg_value == 'false':
                return Symbol(DataTypes.BOOL, False)
            else:
                app_exist(ExitCodes.INVALID_XML_STRUCT)
        elif arg_type == 'string':
            if re.compile('.*#.*').match(arg_value):
                app_exist(ExitCodes.INVALID_XML_STRUCT)
            fixed_string = InstructionsParser.string_fix(arg_value)
            return Symbol(DataTypes.STRING, fixed_string)
        elif arg_type == 'type':
            if arg_value == 'int':
                return Type(int)
            elif arg_value == 'string':
                return Type(str)
            elif arg_value == 'float':
                return Type(float)
            elif arg_value == 'bool':
                return Type(bool)
            else:
                app_exist(ExitCodes.INVALID_XML_STRUCT)
        elif arg_type == 'float':
                try:
                    return Symbol(DataTypes.FLOAT, float.fromhex(arg_value))
                except Exception:
                    app_exist(ExitCodes.INVALID_XML_STRUCT)
        else:
            app_exist(ExitCodes.INVALID_XML_STRUCT)

    @staticmethod
    def scope_validate(scope: str):
        """ Validity check of a frame. """

        if scope != Frames.GLOBAL.value and scope != Frames.LOCAL.value and scope != Frames.TEMPORARY.value:
            app_exist(ExitCodes.INVALID_XML_STRUCT)

    @staticmethod
    def variable_validate(name: str):
        """ Validity check of variable or label names. """

        if re.compile(r"^[_\-$&%*!?a-zA-Z][_\-$&%*!?a-zA-Z0-9]*$").match(name) is None:
            app_exist(ExitCodes.INVALID_XML_STRUCT)

    @staticmethod
    def string_fix(value: str) -> str:
        """ String handling. Processing of escape sequences. """

        result: List[str] = list()
        splitedParts = value.split('\\')
        result.append(splitedParts[0])
        number_regex = re.compile("\\d{3}")
        for item in splitedParts[1:]:
            number_matched = number_regex.match(item[:3])
            if not number_matched:
                app_exist(ExitCodes.INVALID_XML_STRUCT)
            result.append(chr(int(item[:3])))
            result.append(item[3:])
        return str().join(result)
