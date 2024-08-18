#define _CRT_SECURE_NO_WARNINGS	
#include <stdio.h>
#include <stdlib.h>
#include<string.h>
#include<ctype.h>


/// <summary>
///
/// 
/// </summary>
char errors[100][100];
int errorcount = 0;
struct node* programo(int level);
struct node* functiono(int level);
struct node* sp(int level);
struct node* if_statemento(int level);
struct Token
{
	char value[64];
	enum Type type;
};
enum DataType
{
	Int,Float,Char
};
struct Identifier
{
	char id[64];
	char value[64];
	enum DataType type;
};
struct node {
	struct Token token;
	struct node* parent;
	struct node* child[6];
	int numchild;
	int level;
};
struct Identifier identifiers[20];
int idcount = 0;
struct node *start;
struct node* start2;
char parsetree[64][1000] = { "", "", "", "", "", "", "", "", "", "" };
char parsetree2[64][1000] = { "", "", "", "", "", "", "", "", "", "" };

void addchild(struct node* parent, struct node* child)
{
	parent->child[parent->numchild++] = child;
	child->level = parent->level + 1;
	child->parent = parent;
}
struct node* newNode(struct Token token)
{
	struct node* node
		= (struct node*)malloc(sizeof(struct node));
	node->token = token;
	node->level = 0;
	node->parent = NULL; 
	node->numchild = 0;
	for (int i = 0;i < 5;i++)
	{
		node->child[i] = NULL;
	}
	return node;
}

void printtree(struct node* parent,char parsetree[][1000])
{
	char temp[65] = "\t";
	strcat(temp, parent->token.value);
	strcat(parsetree[parent->level], temp);
	for (int i = 0;i < parent->numchild;i++)
	{
		printtree(parent->child[i],parsetree);
	}
}
char reservedwords[][10] = { "printf","if","else","return"};
char datatypes[][10] = {"int","float","char"};
char symbol[][3] = { "#",">","<","<=",">=","==","!=","=","(",")","{","}","\"",".",";","/"};
char number[] = { '0','1','2','3','4','5','6','7','8','9' };
char character[] = { 'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n', 'o', 'p', 'q', 'r', 's', 't', 'u', 'v', 'w', 'x', 'y', 'z','.' };
FILE* sourcefile;
char code[1000];
int fail = 0;
int getDtype(char s[] )
{
	if(s[0]=='\'')
	{
		
		if (isalnum(s[1])) {
			if (s[2] == '\'') {
				if (s[3] == '\0')  return 2;
				else return -1;
			}
		}
	}
	else {
		int numdec = 0;
		for (int i = 0;s[i] != '\0';i++)
		{
			if (s[i] == '.' && numdec < 1)
			{
				numdec++;
				continue;
			}
			else if (!isdigit(s[i])) return -1;
		}

		if (numdec == 0)return 0;
		if (numdec == 1) return 1;
	}
}
void LexicalAnalysis();
int checkfordeclaration(char c[])
{
	for (int i = 0;i < idcount;i++)
	{
		if (strcmp(c, identifiers[i].id) == 0) return 1;
	}
	return 0;
}
struct node* program();
struct node* s();
struct node* if_statement();
enum Type
{
	empty, ReservedWord, SpecialSymbol, Numbers, String, Identifier, Comments, NonTerminal, Datatype, mainfuntion,Chr
};
char* getType(enum Type type)
{
	switch (type)
	{
	case 1:
		return "Reserved Word";
	case 2:
		return "Special Symbol";
	case 3:
		return "Number";
	case 4:
		return "String";
	case 5:
		return "Identifier";
	case 6:
		return "Comments";
	case 7:
		return "Non Terminal";
	case 8:
		return "DataType";
	case 9:
		return "Main Function";
	case 10:
		return "Char";
	}
}
void printnode(struct Token node)
{
	fprintf(sourcefile," %s :: %s\n", node.value, getType(node.type));

}
int isReserved(char s[])
{
	for (int i = 0;i < 4;i++)
	{
		if (strcmp(s, reservedwords[i]) == 0) return 1;
	}
	return 0;
}
int isDataType(char s[])
{
	for (int i = 0;i < 3;i++)
	{
		if (strcmp(s, datatypes[i]) == 0) return 1;
	}
	return 0;
}

int isNumb(char s[], int length)
{
	if (length > 0)
	{
		int numdec=0;
		for (int i = 0;i < length;i++)
		{
			if (s[i] == '.'&&numdec<1)
			{
				numdec++;
				continue;
			}
			else if (!isdigit(s[i])) return 0;
		}
		return 1;
	}
	else return 0;

}
int isIdentifier(char s[], int length)
{
	if (length > 0)
	{
		for (int i = 0;i < length;i++)
		{
			if (!isalpha(s[i])) return 0;
		}
		return 1;
	}
	else return 0;

}
int isComparator(char s[])
{
	for (int i = 1;i < 7;i++)
	{
		if (strcmp(s, symbol[i]) == 0) return 1;
	}
	return 0;
}
struct Token tokens[100];
struct Token parsingtokens[100];
int parsingtokensc = 0;
int tokencounter = 0;
int parsercounter = 0;
void prepareforparsing()
{
	for (int i = 0;parsingtokensc < tokencounter;i++) {
		if (strcmp(tokens[i].value, "") == 0) break;
		if (tokens[i].type == 8)
		{
			struct Identifier tempid;
			for (int w = 0;w < 3;w++)
			{
				if (strcmp(tokens[i].value, datatypes[w]) == 0) {
					tempid.type = w;
					break;
				}
			}
			int l = i;
			l++;
			if (tokens[l].type == 5)
			{
				strcpy(tempid.id, tokens[l].value);
				l++;
				if (strcmp(tokens[l].value, "=")==0)
				{
					l++;
					if (isNumb(tokens[l].value, strlen(tokens[l].value)) || tokens[l].type == 4|| tokens[l].type==10)
					{
						strcpy(tempid.value, tokens[l].value);
						identifiers[idcount++] = tempid;
						l++;
						i = l;
					}
				}
			}
			
		}
		else if (tokens[i].type != 6 && tokens[i].type != 8 && tokens[i].type != 9 && tokens[i].type != 0)
		{
			parsingtokens[parsingtokensc++] = tokens[i];
		}
	}
}
int issymbol(char c[])
{
	for (int i = 0;i < 15;i++)
	{
		if (strcmp(c,symbol[i])==0) return 1;
	}
	return 0;
}
void printtokens()
{
	for (int i = 0;i < tokencounter;i++)
	{
		printnode(tokens[i]);
	}
}
void printptokens()
{
	for (int i = 0;i < parsingtokensc;i++)
	{
		printnode(parsingtokens[i]);
	}
}

void printerror()
{
	printf("Syntax Error");
	//exit(1);
}
struct node* c(int level)
{
	struct Token c;
	strcpy(c.value, "c");
	c.type = 7;
	struct node* parent = newNode(c);
	parent->level = level;
	if (parsingtokens[parsercounter].type == 5 || parsingtokens[parsercounter].type == 3|| parsingtokens[parsercounter].type == 10)
	{
		if (parsingtokens[parsercounter].type == 5) if (!checkfordeclaration(parsingtokens[parsercounter].value)) 
		{
			strcpy(errors[errorcount++], "Identifier not Declared");
			fail = 1;
		}
		struct Token first;
		strcpy(first.value, getType(parsingtokens[parsercounter].type));
		struct node* firstnode = newNode(first);
		addchild(parent,firstnode);

		strcat(parsingtokens[parsercounter].value, "\t\t");

		addchild(firstnode, newNode(parsingtokens[parsercounter++]));
		

		if (isComparator(parsingtokens[parsercounter].value)) {
			addchild(parent, newNode(parsingtokens[parsercounter++]));
			if (parsingtokens[parsercounter].type == 5 || parsingtokens[parsercounter].type == 3 || parsingtokens[parsercounter].type == 10)
			{
				if(parsingtokens[parsercounter].type == 5)
				if (!checkfordeclaration(parsingtokens[parsercounter].value)) fail = 1;
				struct Token second;
				strcpy(second.value, getType(parsingtokens[parsercounter].type));
				struct node* secondnode = newNode(second);
				addchild(parent, secondnode);
				addchild(secondnode, newNode(parsingtokens[parsercounter++]));
			}
			else {
				strcpy(errors[errorcount++], "Expected value or identifier");
				fail = 1;
			}
		}
		else {
			strcpy(errors[errorcount++], "Expected Comparator");
			fail = 1;
		}
	}
	else {
		strcpy(errors[errorcount++], "Expected value or identifier");
		fail = 1;
	}
	return parent;
}
struct node* s(int level)
{
	struct Token s;
	strcpy(s.value, "S");
	s.type = 7;
	struct node* parent = newNode(s);
	parent->level = level;
	if (strcmp(parsingtokens[parsercounter].value, "else") == 0)
	{
		addchild(parent, newNode(parsingtokens[parsercounter++]));
		if (strcmp(parsingtokens[parsercounter].value, "if") == 0)
		{
			addchild(parent, if_statement(parent->level+1));
		}
		else if (strcmp(parsingtokens[parsercounter].value, "{") == 0)
		{

			addchild(parent, program(parent->level+1));
			
		}
		else {
			strcpy(errors[errorcount++], "Expected if or {");
			fail = 1;
		}

	}
	else {
		strcpy(errors[errorcount++], "Expected else ");
		fail = 1;
	}
	return parent;
}
struct node* if_statement(int level)
{	
	struct Token if_state;
	strcpy(if_state.value, "if_statement");
	if_state.type = 7;
	struct node* parent = newNode(if_state);
	parent->level = level;
	addchild(parent, newNode(parsingtokens[parsercounter++]));
	if (strcmp(parsingtokens[parsercounter].value, "(") == 0)
	{
		addchild(parent, newNode(parsingtokens[parsercounter]));
		parsercounter++;
		addchild(parent,c(parent->level+1));
		if (strcmp(parsingtokens[parsercounter].value, ")") == 0)
		{
			addchild(parent, newNode(parsingtokens[parsercounter++]));
			if (strcmp(parsingtokens[parsercounter].value, "{") == 0) {
				addchild(parent,program(parent->level+1));
			}
			else 
			{
				strcpy(errors[errorcount++], "Expected {");
				fail = 1;
			}
			if (strcmp(parsingtokens[parsercounter].value, "else") == 0) addchild(parent, s(parent->level + 1));
			else if (strcmp(parsingtokens[parsercounter].value, "}") == 0) return parent;
			else {
				strcpy(errors[errorcount++], "Expected else or } ");
				fail = 1;
			}
		}
		else 
		{
			strcpy(errors[errorcount++], "Expected )");
			fail = 1;
		}
	}
	else {
		strcpy(errors[errorcount++], "Expected ( ");
		fail = 1;
	}
	return parent;
}
struct node* function(int level)
{
	struct Token func;
	strcpy(func.value, "function");
	func.type = 7;
	struct node* parent = newNode(func);
	parent->level = level;
	addchild(parent, newNode(parsingtokens[parsercounter++]));
	if (strcmp(parsingtokens[parsercounter].value, "(") == 0)
	{
		addchild(parent, newNode(parsingtokens[parsercounter++]));
		if (parsingtokens[parsercounter].type==4)
		{
			struct Token string;
			strcpy(string.value, getType(parsingtokens[parsercounter].type));
			struct node* stringnode = newNode(string);
			addchild(parent, stringnode);
			int j = 1;
			while (parsingtokens[parsercounter].value[j] != '\"')
			{
				j++;
				if (parsercounter == parsingtokensc) {
					strcpy(errors[errorcount++], "Expected \"");
					fail = 1;
					break;
				}
			}
			if(fail!=1)
			addchild(stringnode, newNode(parsingtokens[parsercounter++]));
			if (strcmp(parsingtokens[parsercounter].value, ")") == 0)
			{
				addchild(parent, newNode(parsingtokens[parsercounter++]));
			}
			else 
			{
				strcpy(errors[errorcount++], "Expected )");
				fail = 1;
			}
			if (strcmp(parsingtokens[parsercounter].value, ";") == 0)
			{
				addchild(parent, newNode(parsingtokens[parsercounter++]));
			}
			else 
			{ 
				strcpy(errors[errorcount++], "Expected ;");
				fail = 1;
			}
			if (strcmp(parsingtokens[parsercounter].value, "printf") == 0)
			{
				addchild(parent, function(parent->level + 1));
			}
		}
	}
	return parent;

}
struct node* program(int level)
{
	struct Token prog;
	strcpy(prog.value, "program");
	prog.type = 7;

	struct node* parent = newNode(prog);
	parent->level = level;
	if (strcmp(parsingtokens[parsercounter].value, "{") == 0) {
		addchild(parent, newNode(parsingtokens[parsercounter]));
		parsercounter++;
		if (strcmp(parsingtokens[parsercounter].value, "if") == 0) {
			addchild(parent,if_statement(parent->level+1));
		}
		else if (strcmp(parsingtokens[parsercounter].value, "printf") == 0) {
			addchild(parent,function(parent->level+1));
		}
		else {
			strcpy(errors[errorcount++], "Expected printf or if");
			fail = 1;
		}
		if (strcmp(parsingtokens[parsercounter].value, "}") != 0) 
		{
			strcpy(errors[errorcount++], "Expected }");
			fail = 1;
		}
		else {
			addchild(parent, newNode(parsingtokens[parsercounter++]));
		}
			
	}
	return parent; // Return the created node
}
void SyntaxAnalysis()
{
	start=program(0);
	if (fail == 1)
	{
		printerror();
	}
	else if (fail == 0 && parsercounter == parsingtokensc) printf("Syntax Successful");
}
struct node* cp(int level)
{
	struct Token c;
	strcpy(c.value, "c");
	c.type = 7;
	struct node* parent = newNode(c);
	parent->level = level;
	int firstype = -1;
	int secondtype = -1;
	if (parsingtokens[parsercounter].type == 5 || parsingtokens[parsercounter].type == 3 ||parsingtokens[parsercounter].type==10)
	{
		if (parsingtokens[parsercounter].type == 5)
		{
			for (int i = 0;i < idcount;i++)
			{
				if (strstr(parsingtokens[parsercounter].value, identifiers[i].id) != NULL) firstype = identifiers[i].type;
			}
		}
		else if (parsingtokens[parsercounter].type == 3)
		{
			firstype = getDtype(parsingtokens[parsercounter].value);
		}
		else 
		{
			firstype = 2;
		}
		struct Token first;
		strcpy(first.value, getType(parsingtokens[parsercounter].type));
		strcat(first.value, ".value");
		struct node* firstnode = newNode(first);
		addchild(parent, firstnode);

		addchild(firstnode, newNode(parsingtokens[parsercounter++]));


		if (isComparator(parsingtokens[parsercounter].value)) {
			addchild(parent, newNode(parsingtokens[parsercounter++]));
			if (parsingtokens[parsercounter].type == 5 || parsingtokens[parsercounter].type == 3 || parsingtokens[parsercounter].type == 10)
			{
				if (parsingtokens[parsercounter].type == 5)
				{
					for (int i = 0;i < idcount;i++)
					{
						if (strstr(parsingtokens[parsercounter].value, identifiers[i].value) != NULL) secondtype = identifiers[i].type;
					}
				}
				else if (parsingtokens[parsercounter].type == 3)
				{
					secondtype = getDtype(parsingtokens[parsercounter].value);
				}
				else
				{
					secondtype = 2;
				}
				struct Token second;
				strcpy(second.value, getType(parsingtokens[parsercounter].type));
				strcat(second.value, ".value");
				second.type = 7;
				struct node* secondnode = newNode(second);
				addchild(parent, secondnode);
				addchild(secondnode, newNode(parsingtokens[parsercounter++]));
			}
			else fail = 1;
		}
		else fail = 1;
	}
	else fail = 1;
	if (firstype != secondtype) {
		fail = 1;
		strcpy(errors[errorcount++],"Type Mismatch");
	}
	return parent;
}
struct node* sp(int level)
{
	struct Token s;
	strcpy(s.value, "S");
	s.type = 7;
	struct node* parent = newNode(s);
	parent->level = level;
	if (strcmp(parsingtokens[parsercounter].value, "else") == 0)
	{
		addchild(parent, newNode(parsingtokens[parsercounter++]));
		if (strcmp(parsingtokens[parsercounter].value, "if") == 0)
		{
			addchild(parent, if_statemento(parent->level + 1));
		}
		else if (strcmp(parsingtokens[parsercounter].value, "{") == 0)
		{

			addchild(parent, programo(parent->level + 1));

		}
		else fail = 1;

	}
	else fail = 1;
	return parent;
}
struct node* if_statemento(int level)
{
	struct Token if_state;
	strcpy(if_state.value, "if_statement");
	if_state.type = 7;
	struct node* parent = newNode(if_state);
	parent->level = level;
	addchild(parent, newNode(parsingtokens[parsercounter++]));
	if (strcmp(parsingtokens[parsercounter].value, "(") == 0)
	{
		addchild(parent, newNode(parsingtokens[parsercounter]));
		parsercounter++;
		addchild(parent, cp(parent->level + 1));
		if (strcmp(parsingtokens[parsercounter].value, ")") == 0)
		{
			addchild(parent, newNode(parsingtokens[parsercounter++]));
			if (strcmp(parsingtokens[parsercounter].value, "{") == 0) {
				addchild(parent, program(parent->level + 1));
			}
			else fail = 1;
			if (strcmp(parsingtokens[parsercounter].value, "else") == 0) addchild(parent, sp(parent->level + 1));
			else if (parsercounter = parsingtokensc) return parent;
			else fail = 1;
		}
		else fail = 1;
	}
	else fail = 1;
	return parent;
}
struct node* functiono(int level)
{
	struct Token func;
	strcpy(func.value, "function");
	func.type = 7;
	struct node* parent = newNode(func);
	parent->level = level;
	addchild(parent, newNode(parsingtokens[parsercounter++]));
	if (strcmp(parsingtokens[parsercounter].value, "(") == 0)
	{
		addchild(parent, newNode(parsingtokens[parsercounter++]));
		if (parsingtokens[parsercounter].type == 4)
		{
			struct Token string;
			strcpy(string.value, getType(parsingtokens[parsercounter].type));
			strcat(string.value, ".value");
			struct node* stringnode = newNode(string);
			addchild(parent, stringnode);
			int j = 1;
			while (parsingtokens[parsercounter].value[j] != '\"')
			{
				j++;
				if (parsercounter == parsingtokensc) {
					fail = 1;
					break;
				}
			}
			if (fail != 1)
				addchild(stringnode, newNode(parsingtokens[parsercounter++]));
			if (strcmp(parsingtokens[parsercounter].value, ")") == 0)
			{
				addchild(parent, newNode(parsingtokens[parsercounter++]));
			}
			else fail = 1;
			if (strcmp(parsingtokens[parsercounter].value, ";") == 0)
			{
				addchild(parent, newNode(parsingtokens[parsercounter++]));
			}
			else fail = 1;
			if (strcmp(parsingtokens[parsercounter].value, "printf") == 0)
			{
				addchild(parent, functiono(parent->level + 1));
			}
		}
	}
	return parent;

}
struct node* programo(int level)
{
	struct Token prog;
	strcpy(prog.value, "program");
	prog.type = 7;

	struct node* parent = newNode(prog);
	parent->level = level;
	if (strcmp(parsingtokens[parsercounter].value, "{") == 0) {
		addchild(parent, newNode(parsingtokens[parsercounter]));
		parsercounter++;
		if (strcmp(parsingtokens[parsercounter].value, "if") == 0) {
			addchild(parent, if_statemento(parent->level + 1));
		}
		else if (strcmp(parsingtokens[parsercounter].value, "printf") == 0) {
			addchild(parent, functiono(parent->level + 1));
		}
		else {
			fail = 1;
		}
		if (strcmp(parsingtokens[parsercounter].value, "}") != 0) fail = 1;
		else {
			addchild(parent, newNode(parsingtokens[parsercounter++]));
		}

	}

	return parent; // Return the created node
}
void SemanticsAnalysis()
{
	parsercounter = 0;
	start2 = programo(0);
	
	if (fail == 0 && parsercounter == parsingtokensc) printf("Semantic Successful");
	else printf("Semantic Unsuccessful");
}
void main()
{
	sourcefile = fopen("test.c", "r");
	for (int i = 0; (code[i] = fgetc(sourcefile)) != EOF;i++);
	fclose(sourcefile);
	sourcefile = fopen("Lexical Output.txt", "w");
	fprintf(sourcefile,"%s", code);
	fprintf(sourcefile,"\nFirstly Lexical Analyzer Phase : \n");
	LexicalAnalysis();
	fclose(sourcefile);
	prepareforparsing();
	sourcefile = fopen("Syntax Output.txt", "w");
	fprintf(sourcefile,"\n Secondly Syntax Analysis : \n");
	SyntaxAnalysis();
	printf("\n");
	printtree(start,parsetree);
	fprintf(sourcefile, "Input Tockens :\n");
	printptokens();
	for (int i = 0;i < 20;i++) {
		fputs(parsetree[i], sourcefile);
		fprintf(sourcefile, "\n");
	}
	fputs(errors[0], sourcefile);
	fclose(sourcefile);
	sourcefile = fopen("Semantic Output.txt", "w");
	SemanticsAnalysis();
	printtree(start2, parsetree2);
	for (int i = 0;i < 20;i++) {
		fputs(parsetree2[i], sourcefile);
		fprintf(sourcefile, "\n");
	}
	fputs(errors[0], sourcefile);
	fclose(sourcefile);

}
void LexicalAnalysis()
{
	for (int i = 0;code[i] != '\0';i++)
	{
		struct Token temptoken;
		
			if (code[i] == '\"') {   // string detection
				int j = 0;
				char temp[64];
				temp[j++] = code[i++];
				while (code[i] != '\"')
				{
					temp[j++] = code[i++];

				}
				temp[j++] = code[i];
				temp[j] = '\0';
				strcpy(temptoken.value, temp);
				temptoken.type = 4;
				tokens[tokencounter++] = temptoken;
			}
			else if (code[i] == '#') // preprocessor detection
			{
				char temp[64];
				int j = 0;
				while (code[i] != '>' && code[i] != '\n')
				{
					temp[j++] = code[i++];
				}
				if (code[i] == '>') temp[j++] = code[i++];
				temp[j] = '\0';
				strcpy(temptoken.value, temp);
				temptoken.type = 6;
				tokens[tokencounter++] = temptoken;
			}
			else if (code[i] == '/') //comment detection
			{
				temptoken.value[0] = '/';
				i++;
				if (code[i] == '/')
				{
					int j = 1;
					while (code[i] != '\n')
					{
						temptoken.value[j++] = code[i++];
					}
					temptoken.value[j] = '\0';
					temptoken.type = 6;
					tokens[tokencounter++] = temptoken;
				}
				else i--;
			}
			else if (code[i] == ';'||code[i]=='('|| code[i]==')')
			{
				temptoken.value[0]= code[i];
				temptoken.value[1] = '\0';
				temptoken.type = 2;
				tokens[tokencounter++] = temptoken;
			}
		else {
			char temp[64] = {""};
			int j = 0;
			while (!isspace(code[i]) && code[i] != '\0')
			{
				if (code[i] == ';' || code[i] == '(' || code[i] == ')') 
				{
					i--;
					break;
				}
				temp[j++] = code[i++];
			}
			temp[j] = '\0';
			if (j != 0 && code[i] != '\0') {
				if (strcmp(temp, "main")==0)
				{
					temp[j++] = code[i++];
					temp[j++] = code[i++];
					strcpy(temptoken.value, temp);
					temptoken.type = 9;
					tokens[tokencounter++] = temptoken;
				}
				else if (isReserved(temp)) {
					strcpy(temptoken.value, temp);
					temptoken.type = 1;
					tokens[tokencounter++] = temptoken;
				}
				else if (issymbol(temp))
				{
					strcpy(temptoken.value, temp);
					temptoken.type = 2;
					tokens[tokencounter++] = temptoken;
				}
				else if (isNumb(temp, j))
				{
					strcpy(temptoken.value, temp);
					temptoken.type = 3;
					tokens[tokencounter++] = temptoken;
				}
				else if (isDataType(temp))
				{
					strcpy(temptoken.value, temp);
					temptoken.type = 8;
					tokens[tokencounter++] = temptoken;
				}
				else if (getDtype(temp) == 2) {

					strcpy(temptoken.value, temp);
					temptoken.type = 10;
					tokens[tokencounter++] = temptoken;
				}

				else if (isIdentifier(temp, j))
				{
					strcpy(temptoken.value, temp);
					temptoken.type = 5;
					tokens[tokencounter++] = temptoken;
				}
			}
		}
	}
	printtokens();
}


