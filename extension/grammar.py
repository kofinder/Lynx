import json
import os

# Paths
input_path = "build/grammar.json"
output_path = "textmate/lynx_language.json"

# Load your grammar.json
with open(input_path) as f:
    grammar = json.load(f)

# Map token types to TextMate scopes
scope_mapping = {
    # Keywords
    "KW_AUTO": "keyword.control.lynx",
    "KW_BOOLEAN": "keyword.type.lynx",
    "KW_BREAK": "keyword.control.lynx",
    "KW_BYTE": "keyword.type.lynx",
    "KW_CASE": "keyword.control.lynx",
    "KW_CATCH": "keyword.control.lynx",
    "KW_CHAR": "keyword.type.lynx",
    "KW_CLASS": "keyword.declaration.lynx",
    "KW_CONSOLE": "support.class.lynx",
    "KW_DEFAULT": "keyword.control.lynx",
    "KW_DOUBLE": "keyword.type.lynx",
    "KW_ELSE": "keyword.control.lynx",
    "KW_ENUM": "keyword.declaration.lynx",
    "KW_EXTENDS": "keyword.declaration.lynx",
    "KW_FINALLY": "keyword.control.lynx",
    "KW_FLOAT": "keyword.type.lynx",
    "KW_FOR": "keyword.control.lynx",
    "KW_IF": "keyword.control.lynx",
    "KW_INT": "keyword.type.lynx",
    "KW_INTERFACE": "keyword.declaration.lynx",
    "KW_RETURN": "keyword.control.lynx",
    "KW_STRING": "string.quoted.double.lynx",
    # Constants
    "TOK_INTEGER": "constant.numeric.lynx",
    "TOK_DOUBLE": "constant.numeric.lynx",
    "TOK_FLOAT": "constant.numeric.lynx",
    "TOK_STRING": "string.quoted.double.lynx",
    # Operators
    "TOK_PLUS": "keyword.operator.arithmetic.lynx",
    "TOK_MINUS": "keyword.operator.arithmetic.lynx",
    "TOK_MULTIPLY": "keyword.operator.arithmetic.lynx",
    "TOK_DIVIDE": "keyword.operator.arithmetic.lynx",
    "TOK_ASSIGN": "keyword.operator.assignment.lynx",
    "TOK_EQUALITY": "keyword.operator.comparison.lynx",
    # Add more mappings as needed...
}

patterns = []

for terminal in grammar["terminals"]:
    if terminal in scope_mapping:
        # Use \b for word boundaries for keywords and identifiers
        match_pattern = f"\\b{terminal.lower()}\\b" if terminal.startswith("KW_") else terminal
        pattern = {
            "name": scope_mapping[terminal],
            "match": match_pattern
        }
        patterns.append(pattern)

# Construct the TextMate grammar JSON
tm_json = {
    "scopeName": "source.lynx",
    "name": "LynxLang",
    "patterns": patterns,
    "repository": {}
}

# Ensure output folder exists
os.makedirs(os.path.dirname(output_path), exist_ok=True)

# Write to file
with open(output_path, "w") as f:
    json.dump(tm_json, f, indent=4)

print(f"TextMate grammar generated at {output_path}")
