"""

NOT PERFECT. BUT GOOD ENOUGH.

"""

import re

def transform_table_entries(cpp_code: str) -> str:

    def replacer_for_epsilon(match):
        return f'{{EMPTY}}'

    def replacer_for_single_words(match):
        name = match.group(1)
        if name == None:
            return f'{{}}'
        else:
            return f'{{{name}}}'

    def replacer_for_multiple_words(match):
        match_as_list = match.group(0).replace('::= ', '').split(' ')
        match_as_string = str(match_as_list[1:len(match_as_list)])
        match_as_string = match_as_string.replace('[', '').replace(']', '').replace('\'', "").replace('\"', "")

        match_as_string = (match_as_string.
                           replace(":=", "ATTRIBUTION").
                           replace(":", "COLON").
                           replace(";", "SEMICOLON").
                           replace(".", "DOT").
                           replace(",,", "COMMA,").
                           replace("$", "DOLAR"))
        match_as_string = (match_as_string.
                           replace("(", "OPENPARENTHESES").
                           replace(")", "CLOSEPARENTHESES").
                           replace("{", "OPENBRACKETS").
                           replace("}", "CLOSEBRACKETS"))
        match_as_string = (match_as_string.
                           replace("<>", "DIFFERENTTHAN").
                           replace("<", "LESSTHAN").
                           replace(">", "GREATERTHAN").
                           replace("=", "EQUALS").
                           replace("&&", "AND").
                           replace("||", "OR").
                           replace("not", "NOT"))
        match_as_string = (match_as_string.
                           replace("+", "PLUS").
                           replace("-", "MINUS").
                           replace("/", "DIV").
                           replace("*", "MULT").
                           replace("^", "POW"))
        match_as_string = (match_as_string.
                           replace("if", "IF").
                           replace("for", "FOR").
                           replace("do", "DO").
                           replace("else", "ELSE").
                           replace("return", "RETURN").
                           replace("while", "WHILE").
                           replace("fi", "FI").
                           replace("od", "OD").
                           replace("step", "STEP").
                           replace("to", "TO").
                           replace("until", "UNTIL").
                           replace("then", "THEN"))
        match_as_string = (match_as_string.
                           replace("int", "INT").
                           replace("float", "FLOAT").
                           replace("bool", "BOOL").
                           replace("string", "STRING"))
        match_as_string = (match_as_string.
                           replace("name", "NAME").
                           replace("begin", "BEGIN").
                           replace("end", "END").
                           replace("struct", "STRUCT").
                           replace("procedure", "PROCEDURE").
                           replace("program", "PROGRAM").
                           replace("in", "IN").
                           replace("new", "NEW").
                           replace("var", "VAR").
                           replace("deref", "DEREF").
                           replace("ref", "REF").
                           replace("null", "NULL"))

        return f'{{{match_as_string}}}'
    
    def namespace_inserter(match):
        name = match.group(1)
        return "TableTokens::" + name

    word_pattern = r'([A-Z_]+)\s*'
    multiple_word_rule_pattern = r'"([a-zA-Z_]+)\s*::=\s*(([a-zA-Z_;.,:$&|><\+\-\*/\^\{\}\=\)\(]+)(\s*))*"'
    single_word_pattern = r'"([A-Z_]+)*"'
    espsilon_pattern = r'"([A-Z_\{\}]+)\s*::=\s*ε"'

    # Apply the substitution
    transformed_code = re.sub(word_pattern, namespace_inserter,
                              re.sub(single_word_pattern, replacer_for_single_words, 
                              re.sub(multiple_word_rule_pattern, replacer_for_multiple_words,
                              re.sub(espsilon_pattern, replacer_for_epsilon, cpp_code))))

    return transformed_code


# Example usage
if __name__ == "__main__":
    with open("table_as_string.cpp", "r", encoding="utf-8") as file:
        cpp_input = file.read()

    cpp_output = transform_table_entries(cpp_input)

    with open("syntactic_table.cpp", "w", encoding="utf-8") as file:
        file.write(cpp_output)

    print("Transformation complete. Output written to final_table.cpp.")