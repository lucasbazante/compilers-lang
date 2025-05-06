def split_grammar_rules(rules):
	result = []
	for rule in rules:
		# Split rule into left and right parts
		if '->' not in rule:
			continue
		lhs, rhs = rule.split('->', 1)
		lhs = lhs.strip()
		rhs = rhs.replace('||', 'bb')
		rhs_alternatives = [alt.strip() for alt in rhs.split('|')]
		empty = "''"
		for alt in rhs_alternatives:
			result.append(f"{lhs} -> {alt.replace('bb','||').replace('ϵ', empty)}")
	return result

# Example usage
a = open("rules.txt", 'r', encoding='utf-8', errors='ignore')
rules = a.readlines()

formatted_rules = split_grammar_rules(rules)
for r in formatted_rules:
	print(r)