from models import Symbol
from enums import DataTypes, ExitCodes
from typing import List


def app_exist(code: int):
    """ Helper function for terminating the program with an error code.

    Parameters
    ----------
    code: int 
        Error code.
    """
    
    exit(int(code.value))


def valid_mathsymbol(symb1: Symbol, symb2: Symbol):
    """ Helper function for checking mathematical operands. 

    Parameters
    ----------
    symb1, symb2: Symbol
        Input symbols.
    """

    type = [DataTypes.INT, DataTypes.FLOAT]
    if not symb1.one_of_types(type):
        app_exist(ExitCodes.INVALID_DATA_TYPE)
    if not symb2.one_of_types(type):
        app_exist(ExitCodes.INVALID_DATA_TYPE)
    if not symb2.equal_type(symb1.data_type) and not symb2.is_nil() and not symb1.is_nil():
            app_exist(ExitCodes.INVALID_DATA_TYPE)

def valid_comparablesymbol(symb1: Symbol, symb2: Symbol, allowedTypes: List[DataTypes]):
    """ Helper function for checking comparison operands.

    Parameters
    ----------
    symb1, symb2: Symbol
        Input symbols.
    """

    if not symb1.one_of_types(allowedTypes):
        app_exist(ExitCodes.INVALID_DATA_TYPE)
    if not symb2.one_of_types(allowedTypes):
        app_exist(ExitCodes.INVALID_DATA_TYPE)
    if not symb1.equal_type(symb2.data_type) and not symb1.is_nil() and not symb2.is_nil():
        app_exist(ExitCodes.INVALID_DATA_TYPE)
