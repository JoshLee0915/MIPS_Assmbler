#pragma once
#include <iomanip>
#include <string>
#include <cstddef>
#include <sstream>
#include <vector>

#define START_ADDR 0x00400000-4

using namespace std;

enum instructType
{
	rType,
	iType,
	jType,
};

union command
{
	struct 
	{
		unsigned func : 6;
		unsigned shamt : 5;
		unsigned rd : 5;
		unsigned rt : 5;
		unsigned rs : 5;
		unsigned opcode : 6;
	} rType;

	struct
	{
		unsigned imm : 16;
		unsigned rt : 5;
		unsigned rs : 5;
		unsigned opcode : 6;
	} iType;

	struct
	{
		unsigned address : 26;
		unsigned opcode : 6;
	} jType;

	unsigned int cmd;
};

struct Symbol
{
	string symbolName;
	int value;
};

struct AsmCommand
{
	command cmd;
	int addr;
};

struct Instruction
{
	string instruction;
	int opcode;
	int func;
	instructType type;
};

class AssemblerMIPS
{
private:
	string mipsCode;
	static const Symbol REG_LIST[];
	static const Instruction INSTR_LIST[];

public:
	AssemblerMIPS(string fileContent);
	~AssemblerMIPS(void);
	string assembleCode();
	string assembleCode(string fileContent);
private:
	string removeComments(string code);
	string alignLables(string code);
	vector<Symbol> buildSymbolTable(string code);
	string removeLables(string code, vector<Symbol> table);
	vector<AsmCommand> assemble(string code, vector<Symbol> table);
	command createMcode(Instruction inst, string args, vector<Symbol> table, int addr);
	unsigned int getRegNumber(string reg);
	unsigned int getLables(string label, vector<Symbol> table);
};
