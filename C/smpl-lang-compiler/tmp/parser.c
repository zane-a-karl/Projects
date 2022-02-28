#include "../hdr/parser.h"

void
parse (Parser *p) {

	char *rv_fgets;
	do {
		rv_fgets = fgets(p->line, MAX_LINE_LEN, p->fin);
		if (rv_fgets == NULL) {
			break;
		}
		if (feof(p->fin) != 0) {
			printf("Reached end of file\n");
			break;
		}
		printf("%s\n", p->line);
		E(p);
	} while(1);
	printf("Made it through parsing\n");
}

Result
E (Parser *p) {
	Result x, y;
	x = T(p);
	while(p->line[p->pos]=='+') {
		next_char(p);
		y = T(p);
		compute(0, &x, &y); // ADD
		//		putF2(op_codes[0], R[29]-1, R[29]-1, R[29]--);//ADD
		//		printf("add\n");
	}
	return x;
}

Result
T (Parser *p) {
	Result x, y;
	x = F(p);
	while (p->line[p->pos]=='*') {
		next_char(p);
		y = F(p);
		compute(2, &x, &y); // MUL
		//		putF2(op_codes[2], R[29]-1, R[29]-1, R[29]--);//MUL
		//		printf("mul\n");
	}
	return x;
}

Result
F (Parser *p) {
	Result x;//, y;
	//	int v;
  char *name = (char *)calloc(MAX_NAME_LEN, sizeof *name);
	if (p->line[p->pos]=='(') {
		next_char(p);
		x = E(p);
		if (p->line[p->pos]==')') {
			next_char(p);
		} else {
			error(NOCLOSINGPAREN);
		}
	} else if ( isdigit(p->line[p->pos]) ) {
		x.kind = CONST;
		x.imm  = val(&p->line[p->pos]);
		//		v = val(&p->line[p->pos]);
		next_char(p);
		while ( isdigit(p->line[p->pos]) ) {
			x.imm = 10*x.imm + val(&p->line[p->pos]);
			//			v = 10*v + val(&p->line[p->pos]);
			next_char(p);
		}
		//		putF1(op_codes[16], ++R[29], 0, v); //ADDI
		//		printf("push %d\n", v);
	} else if ( isalpha(p->line[p->pos]) ) {
		int i = 0;
		x.kind = VARIABLE;
		name[i] = p->line[p->pos];
		next_char(p);
		while ( isalpha(p->line[p->pos]) ) {
			name[++i] = p->line[p->pos];
			next_char(p);
		}
		x.addr = lookup(p->var_table, name); // LDW
		//		putF1(op_codes[32],++R[29],R[30],lookup(p->var_table, name));
		//		printf("load %s\n", name);
	} else {
		error(NOCLOSINGPAREN);
	}
	return x;
}

void
next_char (Parser *p) {
	p->pos++;
}

int
val (char *c) {

	int rv;
	sscanf(c, "%d", &rv);
	return rv;
}

void
error (ParserError e) {

	printf("Parser exited with error code: %d", e);
	exit(0);
}

int
lookup (VAR vt[MAX_NUM_VARS],
				char *name) {
	for (int i = 0; i < MAX_NUM_VARS; ++i) {
		if (strncmp(vt[i].name, name, MAX_NAME_LEN) == 0) {
			return vt[i].addr;
		}
	}
	return -1;
}
