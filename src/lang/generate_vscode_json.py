#!/usr/bin/env python3
import sys
import json
import re

def parse_bison_output(file_path):
    nonterminals = set()
    terminals = set()
    rules = []
    start_symbol = None

    with open(file_path, 'r') as f:
        lines = f.readlines()

    rule_section = False

    for line in lines:
        line = line.strip()
        if not line:
            continue
        if line.startswith("Grammar"):
            rule_section = True
            continue
        if rule_section:
            # Match rules like: lhs : rhs
            match = re.match(r'^\d*\s*(\S+)\s*:\s*(.*)', line)
            if match:
                lhs = match.group(1).strip()
                rhs_text = match.group(2).strip()

                # Initialize start symbol
                if not start_symbol:
                    start_symbol = lhs

                # Split RHS tokens
                rhs_tokens = rhs_text.split() if rhs_text != "." else []

                # Clean tokens
                rhs = []
                for sym in rhs_tokens:
                    sym = sym.strip(",[]")  # remove brackets and commas
                    if sym == "%empty":
                        continue
                    rhs.append(sym)
                    if sym.startswith("TOK_") or sym.startswith("KW_"):
                        terminals.add(sym)
                    else:
                        nonterminals.add(sym)

                # Add LHS to nonterminals
                if lhs != "%empty":
                    nonterminals.add(lhs)

                # Store the rule
                rules.append({"lhs": lhs, "rhs": rhs})

    return {
        "start": start_symbol,
        "nonterminals": sorted(list(nonterminals)),
        "terminals": sorted(list(terminals)),
        "rules": rules
    }

if __name__ == "__main__":
    if len(sys.argv) != 3:
        print(f"Usage: {sys.argv[0]} parser.output grammar.json")
        sys.exit(1)

    parser_output = sys.argv[1]
    output_json = sys.argv[2]

    grammar = parse_bison_output(parser_output)

    with open(output_json, 'w') as f:
        json.dump(grammar, f, indent=4)

    print(f"Grammar JSON saved to {output_json}")
