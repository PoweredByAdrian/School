import sys
from sys import stdin
from helper import app_exist
from enums import ExitCodes
from argparse import ArgumentParser
from instruction_parser import InstructionsParser
from program import Program


def argument_parse_error():
    app_exist(ExitCodes.INVALID_ARGUMENTS)

if '--help' in sys.argv and len(sys.argv) > 2:
    app_exist(ExitCodes.INVALID_ARGUMENTS)

parser = ArgumentParser()
parser.add_argument('--source', type=str)
parser.add_argument('--input', type=str)
parser.error = argument_parse_error
arguments = parser.parse_args()

if arguments.source is None and arguments.input is None:
    app_exist(ExitCodes.INVALID_ARGUMENTS)

xml_file = stdin
try:
    if arguments.source is not None:
        xml_file = open(arguments.source, 'r')
except Exception:
    app_exist(ExitCodes.CANNOT_READ_FILE)

input_file = stdin
try:
    if arguments.input is not None:
        input_file = open(arguments.input, 'r')
except Exception:
    app_exist(ExitCodes.CANNOT_READ_FILE)
    
instructions = InstructionsParser.parse_file(xml_file)
program = Program(list(instructions.values()), input_file)
program.run()
exit(int(program.exit_code))
