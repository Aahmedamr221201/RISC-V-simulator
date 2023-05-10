#include <iostream>
#include<fstream>
#include<string>
#include<vector>
#include<map>
#include<cstdlib>
#include <bitset>

using namespace std;
vector<pair<string, int>>registers;
vector<pair<string, int>> labels;
int ra;
int inital_Address;
int programCounter;
//the memory stores the adderess and the content 
map<int, int> memory;
auto itr = memory.begin();

struct instruction
{
	string name;
	bool I_OR_S;

};
vector<string> readFile(string add)
{
	ifstream read1;
	ifstream read2;
	vector<string> AssemblyCode;
	read2.open(add);
	if (read2.is_open())
		cout << " Opened ! " << endl;

	while (!read2.eof())
	{
		string temp;
		getline(read2, temp);
		//read2 >> temp;
		AssemblyCode.push_back(temp);
	}

	return AssemblyCode;
}
vector<instruction> readtheInstruction(vector<string> input)
{
	vector<instruction> inst;
	for (int i = 0; i < input.size(); i++)
	{
		int counter = 0;
		string temp1 = " ";
		while (input[i][counter] != ' ' && counter < input[i].size() && input[i][counter] != ':')
		{
			temp1.push_back(input[i][counter]);
			counter++;
		}
		bool type;
		instruction temp;
		temp1.erase(0, 1);
		if (input[i][counter] == ':')
		{
			pair<string, int> x;
			x.first = temp1;
			x.second = i;
			labels.push_back(x);
		}
		else
		{
			temp.name = temp1;
			if (temp1 != "addi" && temp1 != "lw" && temp1 != "sw")
				temp.I_OR_S = false;
			temp.I_OR_S = true;
			if (temp.name != "")
				inst.push_back(temp);
		}
	}

	return inst;
}
int getlabelindex(string label, vector<pair<string, int>>& labels)
{
	for (int i = 0; i < labels.size(); i++)
	{
		if (labels[i].first == label)
			return labels[i].second;
	}
}
void setprogramcounterOffset(int x)
{
	// x is the line we get to 
	//if label is after the
	if (x >= programCounter)
		programCounter = x;
	else
		programCounter = programCounter - x;
}
vector<string> inputProcessing(vector<string> code)
{

	vector<string>fixed;

	for (int i = 0; i < code.size(); i++)
	{
		vector<int> pos;
		int counter_x = 0;
		for (int j = 0; j < code[i].size(); j++)
		{


			if (code[i][j] == 'x' && counter_x == 0)
			{
				pos.push_back(j);
				counter_x++;
			}
			if (code[i][j] == ',')
			{
				pos.push_back(j);
			}
			if (code[i][j] == '(')
			{
				pos.push_back(j);
			}

		}
		for (int j = 0; j < pos.size(); j++)
		{

			string temp = " ";
			int counter = pos[j];
			if (code[i][counter] == 'x') {

				while (code[i][counter] != ',' && (counter < code[i].size()) && code[i][counter] != ')' && code[i][counter] != ')' && code[i][counter] != ' ')
				{
					temp.push_back(code[i][counter]);
					counter++;
				}
				temp.erase(0, 1);
				fixed.push_back(temp);

			}
			else if (code[i][counter] != 'x')
			{
				counter++;
				while (code[i][counter] != ',' && (counter < code[i].size()) && code[i][counter] != '(' && code[i][counter] != ' ' && code[i][counter] != ')')
				{
					temp.push_back(code[i][counter]);
					counter++;
				}
				temp.erase(0, 1);
				fixed.push_back(temp);
			}
		}
		if (((code[i][0] == 'j') && (code[i][1] == 'a') && (code[i][2] == 'l')) || ((code[i][0] == 'J') && (code[i][1] == 'A') && (code[i][2] == 'L')))
		{
			fixed.push_back("x");
		}
	}


	return fixed;
}
void initializeRegistars()
{

	for (int i = 0; i < 32; i++)
	{
		string x = to_string(i);
		string temp = 'x' + x;
		pair < string, int> y;
		y.first = temp;
		y.second = 0;
		registers.push_back(y);
		/*cout << registers[i].first << "   " << registers[i].second << " ";
		cout << endl;*/
	}
	


}
int getRegisterValue(string x)
{
	for (int i = 0; i < 32; i++)
	{
		if (registers[i].first == x)
			return registers[i].second;
	}
}
int getRegisterindex(string x)
{
	for (int i = 0; i < 32; i++)
	{
		if (registers[i].first == x)
			return i;
	}
}
bool checkforX0(string rd)
{
	if (getRegisterindex(rd) == 0)
		return 1;
	else return 0;
}
void setRegisterValue(string x, int value)
{
	for (int i = 0; i < 32; i++)
	{
		if (registers[i].first == x)
		{
			registers[i].second = value;
			return;
		}
	}
}
string tobinary(long num)
{

	string binary = bitset<32>(num).to_string();
	string x = "0b";
	x = x + binary;
	return x;

}
string tobinary32bits(long num)
{

	string binary = bitset<32>(num).to_string();

	return binary;

}
string tobinary8bits(long num)
{

	string binary = bitset<8>(num).to_string();
	return binary;

}
string tobinary16bits(long num)
{

	string binary = bitset<16>(num).to_string();
	return binary;

}
string toHexadecimal(int value)
{
	long num = value;
	char hex_string[20];

	sprintf_s(hex_string, "%X", num); //convert number to hex
	string x = "0x";
	x = x + hex_string;
	return x;
}
string pad(string& binary, bool sign, string state)
{
	//signed value
	string pad = "";
	if (sign == 0)//unsigned
	{
		if (state == "halfword")
		{
			pad = "0000000000000000";
			binary = pad + binary;
		}
		if (state == "byte")
		{
			pad = "0000000000000000000000000000";
			binary = pad + binary;
		}
	}
	else
	{
		if (state == "halfword")
		{
			int x = binary[0];
			if (x == 0)
			{
				pad = "0000000000000000";
			}
			else if (x == 1)
			{
				pad = "1111111111111111";
			}
			binary = pad + binary;
		}
		if (state == "byte")
		{

			int x = binary[0];
			if (x == 0)
			{
				pad = "000000000000000000000000";
			}
			else if (x == 1)
			{
				pad = "111111111111111111111111";
			}
			binary = pad + binary;

		}
	}
	return binary;

}
long binarytodecimal(string n)
{
	string num = n;
	int decimalvalue = 0;

	// Initializing base value to 1, i.e 2^0
	int base = 1;

	int length = num.length();
	for (int i = length - 1; i >= 0; i--) {
		if (num[i] == '1')
			decimalvalue += base;
		base = base * 2;
	}

	return decimalvalue;
}
int getProgramCounter(int i)
{
	return inital_Address + (i * 4);
}
int getInstrucstionNumber(int program_Counter)
{

	return ((program_Counter - inital_Address) / 4);
}
void updateProgramCounter(int i)
{
	programCounter = inital_Address + (4 * i);
}
//R type
void ADD(string rd, string rs1, string rs2)
{
	if (checkforX0(rd))
	{
		setRegisterValue(rd, 0);
	}
	else
	{

		int temp1 = getRegisterValue(rs1);
		int temp2 = getRegisterValue(rs2);
		setRegisterValue(rd, temp1 + temp2);
	}


}
void SUB(string rd, string rs1, string rs2)
{
	if (checkforX0(rd))
	{
		setRegisterValue(rd, 0);
	}
	else
	{

		int temp1 = getRegisterValue(rs1);
		int temp2 = getRegisterValue(rs2);
		setRegisterValue(rd, temp1 - temp2);
	}

}
void AND(string rd, string rs1, string rs2)
{
	if (checkforX0(rd))
	{
		setRegisterValue(rd, 0);
	}
	else
	{

		int temp1 = getRegisterValue(rs1);
		int temp2 = getRegisterValue(rs2);
		setRegisterValue(rd, temp1 & temp2);
	}

}
void OR(string rd, string rs1, string rs2)
{
	if (checkforX0(rd))
	{
		setRegisterValue(rd, 0);
	}
	else
	{

		int temp1 = getRegisterValue(rs1);
		int temp2 = getRegisterValue(rs2);
		setRegisterValue(rd, temp1 | temp2);
	}

}
void SLT(string rd, string rs1, string rs2)
{
	if (checkforX0(rd))
	{
		setRegisterValue(rd, 0);
	}
	else
	{

		int temp1 = getRegisterValue(rs1);
		int temp2 = getRegisterValue(rs2);
		setRegisterValue(rd, temp1 & temp2);
		if (temp1 < temp2)
			setRegisterValue(rd, 1);
		else
			setRegisterValue(rd, 0);
	}

}
void SLTU(string rd, string rs1, string rs2)
{
	if (checkforX0(rd))
	{
		setRegisterValue(rd, 0);
	}
	else
	{

		int temp1 = getRegisterValue(rs1);
		int temp2 = getRegisterValue(rs2);
		setRegisterValue(rd, temp1 & temp2);

		if (temp1 < temp2)
			setRegisterValue(rd, 1);
		else
			setRegisterValue(rd, 0);

		if (temp1 = 0)
		{
			if (temp2 != 0)
				setRegisterValue(rd, 1);
			else
				setRegisterValue(rd, 0);
		}
		else
		{
			if (abs(temp1) < abs(temp2))
				setRegisterValue(rd, 1);
			else
				setRegisterValue(rd, 0);
		}
	}
}
void SLL(string rd, string rs1, string rs2)
{
	// shift rs1 by rs2
	if (checkforX0(rd))
	{
		setRegisterValue(rd, 0);
	}
	else
	{

		int temp1 = getRegisterValue(rs1);
		int temp2 = getRegisterValue(rs2);
		int temp3;
		temp3 = pow(2.0, temp2);
		setRegisterValue(rd, temp1 * temp3);
	}

}
void SRL(string rd, string rs1, string rs2)
{
	if (checkforX0(rd))
	{
		setRegisterValue(rd, 0);
	}
	else
	{

		int temp1 = getRegisterValue(rs1);
		int temp2 = getRegisterValue(rs2);
		int temp3;
		temp3 = pow(2.0, temp2);
		setRegisterValue(rd, temp1 / temp3);
	}

}
void SRA(string rd, string rs1, string rs2)
{
	if (checkforX0(rd))
	{
		setRegisterValue(rd, 0);
	}
	else
	{
		int temp1 = getRegisterValue(rs1);
		int temp2 = getRegisterValue(rs2);
		setRegisterValue(rd, temp1 >> temp2);
	}

}
//I type:  
void ADDI(string rd, string rs1, string immediate)
{
	if (checkforX0(rd))
	{
		setRegisterValue(rd, 0);
	}
	else
	{
		int temp1 = getRegisterValue(rs1);
		int temp2 = stoi(immediate);
		setRegisterValue(rd, temp1 + temp2);
	}


}
void SLTI(string rd, string rs1, string immediate)
{
	if (checkforX0(rd))
	{
		setRegisterValue(rd, 0);
	}
	else
	{
		int temp1 = getRegisterValue(rs1);
		int temp2 = stoi(immediate);
		if (temp1 < temp2)
			setRegisterValue(rd, 1);
		else
			setRegisterValue(rd, 0);
	}

}
void SLTIU(string rd, string rs1, string immediate)
{
	if (checkforX0(rd))
	{
		setRegisterValue(rd, 0);
	}
	else
	{
		int temp1 = getRegisterValue(rs1);
		int temp2 = stoi(immediate);

		if (temp1 == 0)
		{
			if (temp2 != 0)
				setRegisterValue(rd, 1);
			else
				setRegisterValue(rd, 0);
		}
		else
		{
			if (abs(temp1) < abs(temp2))
				setRegisterValue(rd, 1);
			else
				setRegisterValue(rd, 0);
		}
	}
}
void ANDI(string rd, string rs1, string immediate)
{
	int temp1 = getRegisterValue(rs1);
	int temp2 = stoi(immediate);

	if (checkforX0(rd))
	{
		setRegisterValue(rd, 0);
	}
	else {
		setRegisterValue(rd, temp1 & temp2);
	}
}
void ORI(string rd, string rs1, string immediate)
{
	int temp1 = getRegisterValue(rs1);
	int temp2 = stoi(immediate);
	if (checkforX0(rd))
	{
		setRegisterValue(rd, 0);
	}
	
	else setRegisterValue(rd, temp1 | temp2);
}
void XORI(string rd, string rs1, string immediate)
{
	checkforX0(rd);
	int temp1 = getRegisterValue(rs1);
	int temp2 = stoi(immediate);
	if (checkforX0(rd))
	{
		setRegisterValue(rd, 0);
	}

	else setRegisterValue(rd, temp1 xor temp2);
}
void SLLI(string rd, string rs1, string immediate)
{
	// shift rs1 by rs2
	
	int temp1 = getRegisterValue(rs1);
	int temp2 = stoi(immediate);
	int temp3;
	temp3 = pow(2.0, temp2);
	if (checkforX0(rd))
	{
		setRegisterValue(rd, 0);
	}

	else setRegisterValue(rd, temp1 * temp3);

}
void SRLI(string rd, string rs1, string immediate)
{
	checkforX0(rd);
	int temp1 = getRegisterValue(rs1);
	int temp2 = stoi(immediate);
	int temp3;
	temp3 = pow(2.0, temp2);
	if (checkforX0(rd))
	{
		setRegisterValue(rd, 0);
	}

	else setRegisterValue(rd, temp1 / temp3);

}
void SRAI(string rd, string rs1, string immediate)
{
	checkforX0(rd);
	int temp1 = getRegisterValue(rs1);
	int temp2 = stoi(immediate);
	if (checkforX0(rd))
	{
		setRegisterValue(rd, 0);
	}

	else setRegisterValue(rd, temp1 >> temp2);

}
// S type:
void SW(string rd, string immediate, string rs1)
{
	int temp1 = getRegisterValue(rs1);
	int temp2 = stoi(immediate);
	int temp3 = getRegisterValue(rd);

	string rs1Binary = tobinary(temp3);
	rs1Binary.erase(0, 2);
	string r07;
	string r815;
	string r1623;
	string r2431;
	for (int i = 0; i < 8; i++)
	{
		r07.push_back(rs1Binary[i]);
	}
	for (int i = 8; i < 16; i++)
	{
		r815.push_back(rs1Binary[i]);
	}
	for (int i = 16; i < 24; i++)
	{
		r1623.push_back(rs1Binary[i]);
	}
	for (int i = 24; i < 32; i++)
	{
		r2431.push_back(rs1Binary[i]);
	}
	int address = temp1 + temp2;

	int i = std::stoi(r07, nullptr, 2);
	memory.insert({ (address),i });

	i = std::stoi(r815, nullptr, 2);
	memory.insert({ (address + 1),i });

	i = std::stoi(r1623, nullptr, 2);
	memory.insert({ (address + 2),i });

	i = std::stoi(r2431, nullptr, 2);
	memory.insert({ (address + 3),i });
	//cout << i;

}
void LW(string rd, string immediate, string rs1)
{
	int temp1 = getRegisterValue(rd);
	int temp2 = stoi(immediate);
	int temp3 = getRegisterValue(rs1);// memory address
	temp3 = temp3 + temp2;
	int decimal = 0;
	string binaryvalue;
	for (map<int, int>::iterator it = memory.begin(); it != memory.end(); ++it)
	{
		if (it->first == temp3)
		{


			for (int i = 0; i < 4; i++)
			{
				decimal = +it->second;
				binaryvalue += tobinary8bits(decimal);

				it++;
			}

			decimal = binarytodecimal(binaryvalue);
			break;


		}
	}

	setRegisterValue(rd, decimal);
	programCounter += 4;

}
void LH(string rd, string immediate, string rs1)
{
	int temp1 = getRegisterValue(rd);
	int temp2 = stoi(immediate);
	int temp3 = getRegisterValue(rs1);// memory address
	temp3 = temp3 + temp2;
	int decimal = 0;
	for (map<int, int>::iterator it = memory.begin(); it != memory.end(); ++it)
	{
		if (it->first == temp3)
		{
			string binaryvalue;
			it++;
			it++;
			for (int i = 0; i < 2; i++)
			{
				decimal = it->second;
				binaryvalue += tobinary16bits(decimal);

				it++;
			}
			binaryvalue = pad(binaryvalue, 1, "halfword");
			decimal = binarytodecimal(binaryvalue);
			break;


		}
	}

	setRegisterValue(rd, decimal);
	programCounter += 4;
}
void LB(string rd, string immediate, string rs1)
{
	int temp1 = getRegisterValue(rd);
	int temp2 = stoi(immediate);
	int temp3 = getRegisterValue(rs1);// memory address
	temp3 = temp3 + temp2;
	int decimal = 0;
	string binarystring;
	for (map<int, int>::iterator it = memory.begin(); it != memory.end(); ++it)
	{
		if (it->first == temp3)
		{
			decimal = +it->second;
		}
	}
	binarystring = tobinary8bits(decimal);
	pad(binarystring, 1, "byte");

	setRegisterValue(rd, decimal);
	programCounter += 4;
}
void LHU(string rd, string immediate, string rs1)
{
	int temp1 = getRegisterValue(rd);
	int temp2 = stoi(immediate);
	int temp3 = getRegisterValue(rs1);// memory address
	temp3 = temp3 + temp2;
	int decimal = 0;
	for (map<int, int>::iterator it = memory.begin(); it != memory.end(); ++it)
	{
		if (it->first == temp3)
		{
			string binaryvalue;
			it++;
			it++;
			for (int i = 0; i < 2; i++)
			{
				decimal = it->second;
				binaryvalue += tobinary8bits(decimal);

				it++;
			}
			pad(binaryvalue, 0, "halfword");
			decimal = binarytodecimal(binaryvalue);
			break;


		}
	}

	setRegisterValue(rd, decimal);
	programCounter += 4;
}
void LBU(string rd, string immediate, string rs1)
{
	int temp1 = getRegisterValue(rd);
	int temp2 = stoi(immediate);
	int temp3 = getRegisterValue(rs1);// memory address
	temp3 = temp3 + temp2;
	int decimal = 0;
	string binarystring;
	for (map<int, int>::iterator it = memory.begin(); it != memory.end(); ++it)
	{
		if (it->first == temp3)
		{
			decimal = +it->second;
		}
	}
	binarystring = tobinary8bits(decimal);
	pad(binarystring, 0, "byte");

	setRegisterValue(rd, decimal);
	programCounter += 4;
}

void SB(string rd, string immediate, string rs1)
{
	int temp1 = getRegisterValue(rs1);

	int temp2 = stoi(immediate);
	int temp3 = getRegisterValue(rd);
	string rs1Binary = tobinary32bits(temp3);
	string r2431;

	for (int i = 24; i < 32; i++)
	{
		r2431.push_back(rs1Binary[i]);
	}
	int address = temp1 + temp2;

	int i = std::stoi(r2431, nullptr, 2);
	memory.insert({ (address + 3),i });
	//cout << i;

}
// B type: 
vector<int> getindex(string label)
{
	//vector<pair<string, int>>
	for (int i = 0; i < labels.size(); i++)
	{
		if (label == labels[i].first)
		{
			vector<int> temp;
			temp.push_back(labels[i].second);
			temp.push_back(i);
			return temp;
		}



	}
	vector<int> temp;
	temp.push_back(-1);
	return temp;
}
int BEQ(string rs1, string rs2, string label)
{
	int temp1 = getRegisterValue(rs1);
	int temp2 = getRegisterValue(rs2);
	if (temp1 == temp2)
	{
		vector<int> x = getindex(label);
		int ret = x[0] - x[1];
		return ret;
	}
	else
		return -1;

}
int BNQ(string rs1, string rs2, string label)
{
	int temp1 = getRegisterValue(rs1);
	int temp2 = getRegisterValue(rs2);
	if (temp1 != temp2)
	{
		vector<int> x = getindex(label);
		int ret = x[0] - x[1];
		return ret;
	}
	else
		return -1;

}
int BLT(string rs1, string rs2, string label)
{
	int temp1 = getRegisterValue(rs1);
	int temp2 = getRegisterValue(rs2);
	if (temp1 < temp2)
	{
		vector<int> x = getindex(label);
		int ret = x[0] - x[1];
		return ret;
	}
	else
		return -1;

}
int BGE(string rs1, string rs2, string label)
{
	int temp1 = getRegisterValue(rs1);
	int temp2 = getRegisterValue(rs2);
	if (temp1 >= temp2)
	{
		vector<int> x = getindex(label);
		int ret = x[0] - x[1];
		return ret;
	}
	else
		return -1;

}
int BLTU(string rs1, string rs2, string label)
{
	int temp1 = abs(getRegisterValue(rs1));
	int temp2 = abs(getRegisterValue(rs2));
	if (temp1 < temp2)
	{
		vector<int> x = getindex(label);
		int ret = x[0] - x[1];
		return ret;
	}
	else
		return -1;

}
int BGEU(string rs1, string rs2, string label)
{
	int temp1 = abs(getRegisterValue(rs1));
	int temp2 = abs(getRegisterValue(rs2));
	if (temp1 >= temp2)
	{
		vector<int> x = getindex(label);
		int ret = x[0] - x[1];
		return ret;
	}
	else
		return -1;

}
int JAL(string R, string imm)
{
	ra = programCounter + 4;
	setRegisterValue(R, ra);
	return getInstrucstionNumber(stoi(imm));
}
int JALR(string rs1, string rs2, string rd)
{
	ra = programCounter + 4;
	setRegisterValue(rs1, ra);
	int temp1 = getRegisterValue(rs2);
	int temp2 = stoi(rd);
	return temp1 + temp2;
}
void AUIPC(string rd, string constant)
{
	string val;
	int value = (((1 << 20) - 1) & (stoi(constant) >> (1 - 1)));
	value = stoi(tobinary32bits(value));
	val = to_string(value) + "000000000000";
	long long temp = binarytodecimal(val);
	setRegisterValue(rd, temp);
}
void LUI(string rd, string constant)
{
	string val;
	int value = (((1 << 20) - 1) & (stoi(constant) >> (1 - 1)));
	value = stoi(tobinary32bits(value));
	val = to_string(value) + "000000000000";
	long long temp = binarytodecimal(val);
	setRegisterValue(rd, temp);
}
void printRegisters()
{
	cout << "  ---------------------------------------------------------------------------------" << endl;
	for (int i = 0; i < 32; i++)
	{
		cout << " | ";
		cout << registers[i].first << "   " << registers[i].second << " ,  " << " Binary :  " << tobinary32bits(registers[i].second) << " ," << "Hexadecimal: " << toHexadecimal(registers[i].second);
		cout << " | ";
		cout << endl;
		cout << "  -----------------------------------------------------------------------------" << endl;
		//cout << endl;
	}
	cout << " " << endl;
}

void START(vector<instruction>Instrctions, vector<string>fixedInput)
{
	int offset = 0;
	int counter = 1;
	for (int i = 0; i < Instrctions.size(); i++)
	{

		cout << "PROGRAM COUNTER : " << programCounter << endl;
		cout << "THE REGISTERS AFTER INSTURCTION NUMBER " << counter << " : " << endl;

		//cout << i << "   " << Instrctions[i].name << endl;
		string temp = Instrctions[i].name;
		////R-type instructions
		if (temp == "add" || temp == "ADD")
		{

			string x1 = fixedInput[offset];
			string x2 = fixedInput[offset + 1];
			string x3 = fixedInput[offset + 2];
			offset = i * 3;
			ADD(x1, x2, x3);

		}
		else if (temp == "sub" || temp == "SUB")
		{
			string x1 = fixedInput[offset];
			string x2 = fixedInput[offset + 1];
			string x3 = fixedInput[offset + 2];
			offset = offset + 3;
			SUB(x1, x2, x3);
		}
		else if (temp == "and" || temp == "AND")
		{
			string x1 = fixedInput[offset];
			string x2 = fixedInput[offset + 1];
			string x3 = fixedInput[offset + 2];
			offset = offset + 3;
			ADD(x1, x2, x3);
		}
		else if (temp == "OR" || temp == "or")
		{
			string x1 = fixedInput[offset];
			string x2 = fixedInput[offset + 1];
			string x3 = fixedInput[offset + 2];
			offset = offset + 3;
			OR(x1, x2, x3);
		}
		else if (temp == "SLT" || temp == "slt")
		{
			string x1 = fixedInput[offset];
			string x2 = fixedInput[offset + 1];
			string x3 = fixedInput[offset + 2];
			offset = offset + 3;
			SLT(x1, x2, x3);
		}
		else if (temp == "sltu" || temp == "SLTU")
		{
			string x1 = fixedInput[offset];
			string x2 = fixedInput[offset + 1];
			string x3 = fixedInput[offset + 2];
			offset = offset + 3;
			SLTU(x1, x2, x3);
		}
		else if (temp == "SLL" || temp == "sll")
		{
			string x1 = fixedInput[offset];
			string x2 = fixedInput[offset + 1];
			string x3 = fixedInput[offset + 2];
			offset = offset + 3;
			SLL(x1, x2, x3);
		}
		else if (temp == "SRL" || temp == "srl")
		{
			string x1 = fixedInput[offset];
			string x2 = fixedInput[offset + 1];
			string x3 = fixedInput[offset + 2];
			offset = offset + 3;
			SRL(x1, x2, x3);
		}
		else if (temp == "SRA" || temp == "sra")
		{
			string x1 = fixedInput[offset];
			string x2 = fixedInput[offset + 1];
			string x3 = fixedInput[offset + 2];
			offset = offset + 3;
			SRA(x1, x2, x3);

		} // I  Type instructions
		else if (temp == "ADDI" || temp == "addi")
		{
			string x1 = fixedInput[offset];
			string x2 = fixedInput[offset + 1];
			string x3 = fixedInput[offset + 2];
			offset = (i + 1) * 3;
			ADDI(x1, x2, x3);
		}
		else if (temp == "SLTI" || temp == "slti")
		{
			string x1 = fixedInput[offset];
			string x2 = fixedInput[offset + 1];
			string x3 = fixedInput[offset + 2];
			offset = (i + 1) * 3;
			SLTI(x1, x2, x3);
		}
		else if (temp == "SLTIU" || temp == "sltiu")
		{
			string x1 = fixedInput[offset];
			string x2 = fixedInput[offset + 1];
			string x3 = fixedInput[offset + 2];
			offset = (i + 1) * 3;
			SLTIU(x1, x2, x3);
		}
		else if (temp == "ANDI" || temp == "andi")
		{
			string x1 = fixedInput[offset];
			string x2 = fixedInput[offset + 1];
			string x3 = fixedInput[offset + 2];
			offset = (i + 1) * 3;
			ANDI(x1, x2, x3);
		}
		else if (temp == "ORI" || temp == "ori")
		{
			string x1 = fixedInput[offset];
			string x2 = fixedInput[offset + 1];
			string x3 = fixedInput[offset + 2];
			offset = (i + 1) * 3;
			ORI(x1, x2, x3);
		}
		else if (temp == "SLLI" || temp == "slli")
		{
			string x1 = fixedInput[offset];
			string x2 = fixedInput[offset + 1];
			string x3 = fixedInput[offset + 2];
			offset = (i + 1) * 3;
			SLLI(x1, x2, x3);
		}
		else if (temp == "SRLI" || temp == "srli")
		{
			string x1 = fixedInput[offset];
			string x2 = fixedInput[offset + 1];
			string x3 = fixedInput[offset + 2];
			offset = (i + 1) * 3;
			SRLI(x1, x2, x3);
		}
		else if (temp == "SRAI" || temp == "srai")
		{
			string x1 = fixedInput[offset];
			string x2 = fixedInput[offset + 1];
			string x3 = fixedInput[offset + 2];
			offset = (i + 1) * 3;
			SRAI(x1, x2, x3);
		}
		else if (temp == "AUIPC" || temp == "auipc")
		{
		string x1 = fixedInput[offset];
		string x2 = fixedInput[offset + 1];
		offset = (i + 1) * 3;
		AUIPC(x1, x2);
		}
		else if (temp == "LUI" || temp == "lui")
		{
		string x1 = fixedInput[offset];
		string x2 = fixedInput[offset + 1];
		offset = (i + 1) * 3;
		LUI(x1, x2);
		}// S type
		else if (temp == "SW" || temp == "sw")
		{
			string x1 = fixedInput[offset];
			string x2 = fixedInput[offset + 1];
			string x3 = fixedInput[offset + 2];
			offset = (i + 1) * 3;
			SW(x1, x2, x3);
		}
		else if (temp == "SB" || temp == "sb")
		{
		string x1 = fixedInput[offset];
		string x2 = fixedInput[offset + 1];
		string x3 = fixedInput[offset + 2];
		offset = (i + 1) * 3;
		SB(x1, x2, x3);
		}
		else if (temp == "LH" || temp == "lh")
		{
			string x1 = fixedInput[offset];
			string x2 = fixedInput[offset + 1];
			string x3 = fixedInput[offset + 2];
			offset = (i + 1) * 3;
			LH(x1, x2, x3);
		}
		else if (temp == "LW" || temp == "lw")
		{
			string x1 = fixedInput[offset];
			string x2 = fixedInput[offset + 1];
			string x3 = fixedInput[offset + 2];
			offset = (i + 1) * 3;
			LW(x1, x2, x3);
		}
		else if (temp == "LB" || temp == "lb")
		{
		string x1 = fixedInput[offset];
		string x2 = fixedInput[offset + 1];
		string x3 = fixedInput[offset + 2];
		offset = (i + 1) * 3;
		LB(x1, x2, x3);
		}
		else if (temp == "LBU" || temp == "lbu")
		{
		string x1 = fixedInput[offset];
		string x2 = fixedInput[offset + 1];
		string x3 = fixedInput[offset + 2];
		offset = (i + 1) * 3;
		LBU(x1, x2, x3);
		}
		else if (temp == "LHU" || temp == "lhu")
		{
			string x1 = fixedInput[offset];
			string x2 = fixedInput[offset + 1];
			string x3 = fixedInput[offset + 2];
			offset = (i + 1) * 3;
			LHU(x1, x2, x3);
		}

		else if (temp == "BEQ" || temp == "beq")
		{
			string x1 = fixedInput[offset];
			string x2 = fixedInput[offset + 1];
			string x3 = fixedInput[offset + 2];
			int jump = BEQ(x1, x2, x3);
			if (jump >= 0)
				i = jump - 1;
			offset = (i + 1) * 3;
		}
		else if (temp == "BNQ" || temp == "bnq")
		{
			string x1 = fixedInput[offset];
			string x2 = fixedInput[offset + 1];
			string x3 = fixedInput[offset + 2];
			int jump = BNQ(x1, x2, x3);
			if (jump >= 0)
				i = jump - 1;
			offset = (i + 1) * 3;
		}
		else if (temp == "BLT" || temp == "blt")
		{
			string x1 = fixedInput[offset];
			string x2 = fixedInput[offset + 1];
			string x3 = fixedInput[offset + 2];
			int jump = BLT(x1, x2, x3);
			if (jump >= 0)
				i = jump - 1;
			offset = (i + 1) * 3;
		}
		else if (temp == "BGE" || temp == "bge")
		{
			string x1 = fixedInput[offset];
			string x2 = fixedInput[offset + 1];
			string x3 = fixedInput[offset + 2];
			int jump = BGE(x1, x2, x3);
			if (jump >= 0)
				i = jump - 1;
			offset = (i + 1) * 3;
		}
		else if (temp == "BLTU" || temp == "bltu")
		{
			string x1 = fixedInput[offset];
			string x2 = fixedInput[offset + 1];
			string x3 = fixedInput[offset + 2];
			int jump = BLTU(x1, x2, x3);
			if (jump >= 0)
				i = jump - 1;
			offset = (i + 1) * 3;
		}
		else if (temp == "BGEU" || temp == "bgeu")
		{
			string x1 = fixedInput[offset];
			string x2 = fixedInput[offset + 1];
			string x3 = fixedInput[offset + 2];
			int jump = BGEU(x1, x2, x3);
			if (jump >= 0)
				i = jump - 1;
			offset = (i + 1) * 3;
		}
		else if (temp == "JAL" || temp == "jal")
		{
			string x1 = fixedInput[offset];
			string x2 = fixedInput[offset + 1];
			updateProgramCounter(i);
			int jump = JAL(x1, x2);
			i = jump - 1;
			offset = (i + 1) * 3;
		}
		else if (temp == "JALR" || temp == "jalr")
		{
			string x1 = fixedInput[offset];
			string x2 = fixedInput[offset + 1];
			string x3 = fixedInput[offset + 2];
			int jump = JALR(x1, x2, x3);
			offset = offset + 3;
			if (jump >= 0)
				i = jump - 1;
			offset = (i + 1) * 3;
		}
		else if (temp == "ECALL" || temp == "ecall" || temp == "EBREAK" || temp == "ebreak" || temp == "FENCE" || temp == "fence")
		{
		return;
		}
		updateProgramCounter(i + 1);
		printRegisters();
		counter++;
	}
}
// Print functions: 
void printmemory()
{
	for (map<int, int>::iterator it = memory.begin(); it != memory.end(); ++it)
	{
		cout << it->first << " : " << it->second << endl;
	}
}

void Memory_initialization(vector<string>memoryx)
{
	vector<int> pos;
	int temp_add;
	int temp_value;
	int temp = stoi(memoryx[0]);
	inital_Address = temp;
	for (int i = 1; i < memoryx.size(); i++)
	{
		for (int j = 0; j < memoryx[i].size(); j++)
		{
			if (memoryx[i][j] == ':')
			{
				pos.push_back(j);

			}
		}
		int j = 0;
		string temp = " ";
		while (memoryx[i][j] != ':')
		{
			temp.push_back(memoryx[i][j]);
			j++;
		}
		temp.erase(0, 1);
		temp_add = stoi(temp);
		j = pos[0];
		string temp1 = " ";
		while (memoryx[i][j] != ' ' && j < memoryx[i].size())
		{
			temp1.push_back(memoryx[i][j]);
			j++;
		}
		temp1.erase(0, 1);
		temp1.erase(0, 1);
		temp_value = stoi(temp1);
		if (temp_value > 256)
			temp_value = 0;
		memory.insert({ temp_add,temp_value });
	}

}

int main()
{
	initializeRegistars();
	vector<string>Input = readFile("Project.txt");
	//vector<string>Memory_input = readFile("D://Assebmly Project//Memory.txt");
	//Memory_initialization(Memory_input);
	updateProgramCounter(0);
	vector<string>fixedInput = inputProcessing(Input);
	vector<instruction> Instrctions = readtheInstruction(Input);
	START(Instrctions, fixedInput);
	printmemory();

}