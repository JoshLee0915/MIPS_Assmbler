#include "AssemblerMIPS.h"

const Symbol AssemblerMIPS::REG_LIST[] = {{"$zero", 0}, {"$at", 1}, {"$v0", 2}, {"$v1", 3}, {"$a0", 4},
{"$a1", 5}, {"$a2", 6}, {"$a3", 7}, {"$t0", 8}, {"$t1", 9}, {"$t2", 10}, {"$t3", 11}, {"$t4", 12}, 
{"$t5", 13}, {"$t6", 14}, {"$t7", 15}, {"$s0", 16}, {"$s1", 17}, {"$s2", 18}, {"$s3", 19}, {"$s4", 20}, 
{"$s5", 21}, {"$s6", 22}, {"$s7", 23}, {"$t8", 24}, {"$t9", 25}, {"$k0", 26}, {"$k1", 27}, {"$gp", 28},
{"$sp", 29}, {"$fp", 30}, {"$ra", 31}};

const Instruction AssemblerMIPS::INSTR_LIST[] = {{"add", 0, 0x20, rType}, 
{"sub", 0, 0x22, rType}, {"addi", 0x08, 0, iType},
{"lw", 0x23, 0, iType},  {"sw", 0x2b, 0, iType},
{"slt", 0, 0x2a, rType}, {"slti", 0x0a, 0, iType},
{"beq", 0x04, 0, iType}, {"bne", 0x05, 0, iType},
{"j", 0x02, 0, jType}, {"jal",0x03, 0, jType},
{"jr", 0, 0x08, rType}};

AssemblerMIPS::AssemblerMIPS(string fileContent)
{
	mipsCode = fileContent;
}

AssemblerMIPS::~AssemblerMIPS(void)
{
}

string AssemblerMIPS::assembleCode()
{
	return assembleCode(mipsCode);
}

string AssemblerMIPS::assembleCode(string fileContent)
{
	vector<Symbol> symTable;
	// clean up the code before starting the assembling
	string modFile = removeComments(fileContent);
	symTable = buildSymbolTable(modFile);
	modFile = removeLables(modFile, symTable);
	assemble(modFile, symTable);

	return modFile;
}

string AssemblerMIPS::removeComments(string code)
{
	size_t start = code.find_first_of("#");
	while(start!=string::npos)
	{
		code.erase(start, code.find_first_of('\n', start)-start);
		start = code.find_first_of("#");
	}

	return code;
}

/*string AssemblerMIPS::trimWhiteSpace(string code)
{
	string trimedCode = code.substr(0, code.find_first_of('\n')+1);
	code = code.substr(code.find_first_of('\n')+1);

	while(!code.empty())
	{
		trimedCode += code.substr(0, code.find_first_of('\n')+1);
		code = code.substr(code.find_first_of('\n')+1);
	}

	return trimedCode;
}*/

vector<Symbol> AssemblerMIPS::buildSymbolTable(string code)
{
	vector<Symbol> table;
	stringstream  ss;

	int addr = START_ADDR;
	string line = "";
	// start at the .text
	ss.str(code.substr(code.find(".text")));

	// scan for the lables
	while(getline(ss, line))
	{
		Symbol tmpSymbol;
		size_t start = line.find_first_of(':');

		// check if there is a lable if there is one add it
		if(start!=string.npos)
		{
			tmpSymbol.symbolName = line.substr(0,start);
			tmpSymbol.value = addr;
			table.push_back(tmpSymbol);
		}

		if(!line.empty())
			addr += 4;	//increment the address
	}
	return table;
}

string AssemblerMIPS::removeLables(string code, vector<Symbol> table)
{
	// loop through all the known symbols and remove them from the code
	for(unsigned int index=0; index < table.size(); index++)
	{
		string lable = table[index].symbolName;
		code.erase(code.find(lable.append(":")), lable.length()+1);
	}
	return code;
}
vector<AsmCommand> AssemblerMIPS::assemble(string code, vector<Symbol> table)
{
	string line;
	stringstream codeStream;
	vector<AsmCommand> cmds;

	int addr = START_ADDR+4;

	codeStream.str(code.substr(code.find(".text")));
	getline(codeStream, line); // remove .text

	while(getline(codeStream, line))
	{
		stringstream instrctStream;

		string tmp = "";
		string args = "";
		string instruction = "";
		instrctStream.str(line);

		// get the instruction
		instrctStream >> instruction;

		// get the arguments
		while(instrctStream >> tmp)
			args += tmp;

		for(unsigned int index = 0; index < 12; index++)
		{
			if(instruction == INSTR_LIST[index].instruction)
			{
				AsmCommand mCode;
				mCode.cmd = createMcode(INSTR_LIST[index], args, table);	// store the mCode
				mCode.addr = addr;											// store the address
				cmds.push_back(mCode);				
				addr += 4;
				break;	// stop
			}
		}
	}

	return cmds;
}

command AssemblerMIPS::createMcode(Instruction inst, string args, vector<Symbol> table)
{
	int addr;
	command cmd;
	stringstream argsStream;

	string rs;
	string rt;
	string rd;
	string label;

	argsStream.str(args);

	// set the opcode
	cmd.rType.opcode = inst.opcode;

	//find the instruction type
	switch(inst.type)
	{
	case rType:
		cmd.rType.func = inst.func;	// set the funct code
		cmd.rType.shamt = 0;	// we dont deal with shifts so this will always be 0

		// break up the args
		getline(argsStream, rd, ',');
		getline(argsStream, rs, ',');
		getline(argsStream, rt, ',');

		// store the regs
		cmd.rType.rs = getRegNumber(rs);
		cmd.rType.rt = getRegNumber(rt);
		cmd.rType.rd = getRegNumber(rd);
		break;
	case jType:
		// shift the mem address >> by 2 then store it
		addr = 0;
		// check if a number was entered
		if(!(argsStream >> addr))
			// if it fails it must be a label
			addr = getLables(args, table);

		// store the addr defults to 0 on invalid input
		cmd.jType.address = addr >> 2;
		break;
	case iType:
		break;
	}
	printf("%#010x\n", cmd);

	return cmd;
}

unsigned int AssemblerMIPS::getRegNumber(string reg)
{
	for(unsigned int index = 0; index < 32 && !reg.empty(); index++)
	{
		if(reg == REG_LIST[index].symbolName)
			return REG_LIST[index].value;		// return the reg number
	}

	return 0;	// default to zero for no or invalid
}

unsigned int AssemblerMIPS::getLables(string label, vector<Symbol> table)
{
	for(unsigned int index=0; index < table.size(); index++)
	{
		if(label == table[index].symbolName)
			return table[index].value;
	}

	return 0; // default to zero for invalid
}