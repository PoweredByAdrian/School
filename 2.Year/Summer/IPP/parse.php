<?php

## Riesenie projektu z predmetu IPP 2022/2023
## Autor: Adrian Horvath (xhorva14)

class Instruction
{
    public $name;
    public $id;
    public $arguments;

    /**
     * Kontrola a spracovanie parametrov instrukcii.
     *
     * @param string $name Opcode.
     * @param int $id Identifikator instrukcie
     * @param string[] $arguments Parametre instrukce.
     * @param string[] $expectedParameters Ocekavane parametry instrukce.
     */
    public function __construct($name, $id, $arguments, $expectedParameters)
    {
        $this->name = $name;
        $this->id = $id;
        $this->arguments = [];

        $this->arguments_check($arguments, $expectedParameters);

        foreach ($arguments as $index => $arg) 
            $this->arguments_insert($arg, $expectedParameters[$index]);
    }

    /**
     * Kontrola parametrov instrukcie.
     *
     * @param string[] $arguments Parametre zadane s instrukciou.
     * @param string[] $expectedParameters Ocakavane parametre instrukcie.
     */
    private function arguments_check($arguments, $expectedParameters)
    {
        $expectedArgumentsCount = count($expectedParameters);
        if (count($arguments) != $expectedArgumentsCount)
            Output::error(Output::syntaxError);
        foreach ($expectedParameters as $index => $expectedType) 
        {
            switch ($expectedType) {
                case DataTypes::label:
                    $this->label_check($arguments[$index]);
                    break;
                case DataTypes::variable:
                    $this->frame_check($arguments[$index]);
                    break;
                case DataTypes::symbol:
                    $this->symbol_check($arguments[$index]);
                    break;
                case DataTypes::type:
                    if (!$this->type_valid($arguments[$index], true))
                        Output::error(Output::syntaxError);
                    break;
                default:
                    throw new Exception("Invalid type\n$expectedType");
            }
        }
    }



    /**
     * Funkcia pre kontrolu navesti.
     *
     * @param string $label Vstupne navesti.
     */
    private function label_check($label)
    {
        if (empty($label))
            Output::error(Output::syntaxError);

        if (preg_match("/^[a-zA-Z_$&%*!?-][a-zA-Z0-9_$&%*!?-]*$/", $label) == 0)
            Output::error(Output::syntaxError);
    }

    /**
     * Funkcia pre kontrolu ramcu.
     *
     * @param string $var Vstupny ramec.
     * @return void
     */
    private function frame_check($var)
    {
        $this->parameter_check($var);
        if (preg_match("/^[LGT]F@[_\-$&%*!?a-zA-Z]\S*$/", $var) == 0)
            Output::error(Output::syntaxError);
    }

    /**
     * Funkcia pre kontrolu symbolov.
     *
     * @param string $symbol Vstupny symbol.
     * @return void
     */
    private function symbol_check($symbol)
    {
        $this->parameter_check($symbol);
        list($type, $value) = explode('@', $symbol, 2);

        if (!$this->frame_valid($type) && !$this->type_valid($type))
            Output::error(Output::syntaxError);

        if ($type == "int" && preg_match("/^int@[-+]?[0-9]+$/", $symbol) ==0)
            Output::error(Output::syntaxError);

        if ($type == "nil" && $symbol != "nil@nil")
            Output::error(Output::syntaxError);

        if ($type == "bool" && !in_array($value, ["true", "false"]))
            Output::error(Output::syntaxError);

        if ($type == "string") {
            if (preg_match("/^string@(\\\\[0-9]{3}|[^#\\\\\s])*$/", $symbol) == 0) {
                Output::error(Output::syntaxError);
            }
        }
    }

    /**
     * Funkcia pre kontrolu datovych typov (string, int, bool, nil).
     *
     * @param string $type Vstupny datovy typ.
     * @param boolean $noNil Volitelny parameter. Ak bude mat hodnotu true, tak sa z kontroly vynecha nil.
     * @return boolean
     */
    private function type_valid($type, $noNil = false)
    {
        $types = ["string", "int", "bool"];

        if (!$noNil)
            array_push($types, "nil");

        return in_array($type, $types);
    }

    /**
     * Funkcia pre kontrolu parametrov (var a symbol).
     *
     * @param string $param Vstupna premenna, alebo symbol.
     */
    private function parameter_check($param)
    {
        if (!string_contains($param, '@'))
            Output::error(Output::syntaxError);
    }

    /**
     * Funkcia pre kontrolu ramcov premennych.
     *
     * @param string $frame Identifikator ramca.
     * @return boolean Vracia true, ak je ramec {GF, TF, LF}.
     */
    private function frame_valid($frame)
    {
        return in_array($frame, ["LF", "TF", "GF"]);
    }

    /**
     * Funkcia pre vlozenie zkontrolovanych parametrov do pola pred jeho vlozenim do XML.
     *
     * @param string $arg Vstupny parameter.
     * @param string $type Typ parametru. (var, symbol, type)
     * @return void
     */
    private function arguments_insert($arg, $type)
    {
        $arg = @htmlspecialchars($arg);
        switch ($type) {
            case DataTypes::label:
                array_push($this->arguments, ["type" => $type, "value" => $arg]);
                break;
            case DataTypes::variable:
            case DataTypes::type:
                array_push($this->arguments, ["type" => $type, "value" => $arg]);
                break;
            case DataTypes::symbol:
                list($dataType, $value) = explode('@', $arg, 2);
                if ($this->frame_valid($dataType)) {
                    $this->arguments_insert($arg, DataTypes::variable);
                    return;
                }
                array_push($this->arguments, ["type" => $dataType, "value" => $value]);
                break;
            default:
                throw new Exception("Invalid argument\n$arg\n$type");
        }
    }

    /**
     * Funkcia na vlozenie potrebnych dat instrukcii do XML.
     *
     * @param DOMDocument $document XML document, do ktoreho sa vkladaju DOM elementy. 
     * @param DOMElement $xmlElem Element, do ktereho sa budu vkladat data.
     */
    public function set_instruction_xml($document, $xmlElem)
    {
        $xmlElem->set_attribute("order", $this->id);
        $xmlElem->set_attribute("opcode", strtoupper($this->name));

        for ($i = 0; $i < count($this->arguments); $i++) {
            $argument = $this->arguments[$i];

            $argElem = $document->create_element("arg" . ($i + 1), trim($argument["value"]));
            $argElem->set_attribute("type", $argument["type"]);

            $xmlElem->append_child($argElem);
        }
    }
}

class DataTypes
{
    public const variable = "var";
    public const label = "label";
    public const symbol = "symbol";
    public const type = "type";
}

class Output
{
    public const invalidarguments = 10;
    public const fileOutputOpenError = 12;
    public const invalidOrMissingHeader = 21;
    public const invalidInstruction = 22;
    public const syntaxError = 23;

    /**
     *Ukonci program s chybovym kodem.
     *
     * @param int $code Navratovy kod.
     * @return void
     */
    public static function error($code)
    {
        exit($code);
    }
}

$program = new Main();

class Main
{
    private $haveHeader = false;
    private $instructions;
    private $counter;
    private $instructionCollection;

    public function __construct()
    {
        $this->instructions = [];
        $this->counter = 1;
        $this->instructionCollection = [

            "MOVE" => [DataTypes::variable, DataTypes::symbol],
            "CREATEFRAME" => [],
            "PUSHFRAME" => [],
            "POPFRAME" => [],
            "DEFVAR" => [DataTypes::variable],
            "CALL" => [DataTypes::label],
            "RETURN" => [],

            "PUSHS" => [DataTypes::symbol],
            "POPS" => [DataTypes::variable],

            "ADD" => [DataTypes::variable, DataTypes::symbol, DataTypes::symbol],
            "SUB" => [DataTypes::variable, DataTypes::symbol, DataTypes::symbol],
            "MUL" => [DataTypes::variable, DataTypes::symbol, DataTypes::symbol],
            "IDIV" => [DataTypes::variable, DataTypes::symbol, DataTypes::symbol],
            "LT" => [DataTypes::variable, DataTypes::symbol, DataTypes::symbol],
            "GT" => [DataTypes::variable, DataTypes::symbol, DataTypes::symbol],
            "EQ" => [DataTypes::variable, DataTypes::symbol, DataTypes::symbol],
            "AND" => [DataTypes::variable, DataTypes::symbol, DataTypes::symbol],
            "OR" => [DataTypes::variable, DataTypes::symbol, DataTypes::symbol],
            "NOT" => [DataTypes::variable, DataTypes::symbol],
            "INT2CHAR" => [DataTypes::variable, DataTypes::symbol],
            "STRI2INT" => [DataTypes::variable, DataTypes::symbol, DataTypes::symbol],

            "READ" => [DataTypes::variable, DataTypes::type],
            "WRITE" => [DataTypes::symbol],

            "CONCAT" => [DataTypes::variable, DataTypes::symbol, DataTypes::symbol],
            "STRLEN" => [DataTypes::variable, DataTypes::symbol],
            "GETCHAR" => [DataTypes::variable, DataTypes::symbol, DataTypes::symbol],
            "SETCHAR" => [DataTypes::variable, DataTypes::symbol, DataTypes::symbol],

            "TYPE" => [DataTypes::variable, DataTypes::symbol],

            "LABEL" => [DataTypes::label],
            "JUMP" => [DataTypes::label],
            "JUMPIFEQ" => [DataTypes::label, DataTypes::symbol, DataTypes::symbol],
            "JUMPIFNEQ" => [DataTypes::label, DataTypes::symbol, DataTypes::symbol],
            "EXIT" => [DataTypes::symbol],

            "DPRINT" => [DataTypes::symbol],
            "BREAK" => []
        ];
    }

    /** Vlozenie instrukcie nacitanie zo STDIN. */
    public function add_instruction($fields)
    {
        $instructionName = trim(strtoupper($fields[0]));
        if ($instructionName == ".IPPCODE23" && !$this->haveHeader) {
            $this->haveHeader = true;
            return;
        }
        if (!$this->haveHeader)
            Output::error(Output::invalidOrMissingHeader);
        if (!array_key_exists($instructionName, $this->instructionCollection))
            Output::error(Output::invalidInstruction);
        array_shift($fields);
        $allowedarguments = $this->instructionCollection[$instructionName];
        $instruction = new Instruction($instructionName, $this->counter++, $fields, $allowedarguments);
        array_push($this->instructions, $instruction);
    }

    /** Vygenerovanie vytupneho XML. */
    private function generate_XML()
    {
        $xml = new DOMDocument('1.0', 'UTF-8');
        $this->generate_XML_Pr($xml);
        $xml->formatOutput = true;
        echo $xml->saveXML();
    }

    /**
     * Funkcia pre vygenerovanie XML dat do dokumentu.
     *
     * @param DOMDocument $document
     * @return void
     */
    private function generate_XML_Pr($document)
    {
        $programElem = $document->create_element("program");
        $programElem->set_attribute('language', 'IPPcode23');

        foreach ($this->instructions as $instruction) {
            $instructionElem = $document->create_element("instruction");
            $instruction->set_instruction_xml($document, $instructionElem);

            $programElem->append_child($instructionElem);
        }

        $document->append_child($programElem);
    }

    /**
     * Vypis napovedy.
     *
     * @return void
     */
    public function print_help()
    {
        echo "IPP Projekt 2023
        Autor: Adrian Horvath (xhorva14)
        Skript ocakava na standardnom vstupe instrukcie jazyka IPPcode23.
        Skript vykona lexikalnu a syntakticku kontrolu a nasledne vypise na standardny vystup XML programu jazyka IPPcode23.
        Priklady spustenia:

        Spustenie zpracovania:
        php parse.php <input.ippcode23

        Napoveda:
        php parse.php --help
        ";
    }

    /**
     * Spustenie hlavneho programu.
     *
     * @param string[] $arguments Parametry na prikazove radce.
     * @return void
     */
    public function run($arguments)
    {   
        $input = file('php://stdin');

        foreach ($input as $line => $instruction) {
            $instruction = preg_replace("/\s+/", " ", $instruction);
            $commentFields = explode('#', $instruction, 2);
            $instruction = trim($commentFields[0]);

            if (empty($instruction))
                continue;

            $fields = explode(' ', trim($instruction));
            $this->add_instruction($fields);
        }
        if ($this->haveHeader) {
            $this->generate_XML();
        } else
            Output::error(Output::invalidOrMissingHeader);
    }
}

function string_contains($str, $searched)
{
    return strpos($str, $searched) !== false;
}


if (in_array("--help", $argv)) {
    if ($argc > 2)
        Output::error(Output::invalidarguments);

    $program->print_help();

} else {
    $program->run($argv);
}