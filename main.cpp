#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include <map>
#include <chrono>

#include "BigNum.hpp"

using namespace std;

void printVariable(string input);
void displayTitle();
void displayHelp();

vector<string> splitInput(string input);
bool isValidNumber(string num);
bool isValidVariableName(string var);
bool doesVariableExist(string var);
bool calculate(string operation, string arg1, string arg2);

struct Error {
	static string Command, VariableName, VariableExist, VariableValue;
};
string Error::Command = "\n#### Enter a valid command. Type `help` for help. ####\n\n";
string Error::VariableName = "#### Enter a valid variable name. ####\n";
string Error::VariableExist = "#### Such a variable does not exist. ####\n";
string Error::VariableValue = "#### Enter a valid value. ####\n";

map<string, BigNum> variableMap;
bool timeDisplay = false;

int main()
{
	displayTitle();

	variableMap.insert(make_pair("mem", "0"));

	bool running = true;
	while (running)
	{
		string input;
		cout << "-> ";
		getline(cin, input);

		vector<string> input_args = splitInput(input);
		if (input_args.size() == 1) {
			if (input_args[0] == "help")displayHelp();
			else if (input_args[0] == "time")timeDisplay = !timeDisplay;
			else if (input_args[0] == "exit")running = false;
			else if (input_args[0] == "clear") { variableMap.clear(); displayTitle(); }
			else if (doesVariableExist(input_args[0]))printVariable(input_args[0]);
			else cout << Error::Command;
		}
		else if (input_args.size() == 2) {
			if (isValidVariableName(input_args[0])) {
				if (isValidVariableName(input_args[1])) {
					if (doesVariableExist(input_args[1]))variableMap[input_args[0]] = variableMap[input_args[1]];
					else cout << Error::VariableExist;
				}
				else if (isValidNumber(input_args[1]))variableMap[input_args[0]] = BigNum(input_args[1]);
				else cout << Error::VariableValue;
			}
			else cout << Error::VariableName;
		}
		else if (input_args.size() == 3)
		{
			if (input_args[0] == "sum" || input_args[0] == "sub" || input_args[0] == "mul" || input_args[0] == "div" || input_args[0] == "cmp")
				calculate(input_args[0], input_args[1], input_args[2]);

			else if (input_args[1] == "+" || input_args[1] == "-" || input_args[1] == "*" || input_args[1] == "/" || input_args[1] == "<=>")
				calculate(input_args[1], input_args[0], input_args[2]);

			else cout << Error::Command;
		}
		else if (input_args.size() == 4) {
			if (!isValidVariableName(input_args[0]))cout << Error::VariableName;
			else if (input_args[1] == "sum" || input_args[1] == "sub" || input_args[1] == "mul" || input_args[1] == "div" || input_args[1] == "cmp") {
				if (calculate(input_args[1], input_args[2], input_args[3]))variableMap[input_args[0]] = BigNum(variableMap["mem"]);
			}
			else if (input_args[2] == "+" || input_args[2] == "-" || input_args[2] == "*" || input_args[2] == "/" || input_args[2] == "<=>") {
				if (calculate(input_args[2], input_args[1], input_args[3]))variableMap[input_args[0]] = BigNum(variableMap["mem"]);
			}
			else cout << Error::Command;
		}
		else cout << Error::Command;
	}
}

void printVariable(string input)
{
	cout << "-> " << variableMap[input] << "\n";
}

bool calculate(string operation, string arg1, string arg2)
{
	BigNum a, b;
	if (doesVariableExist(arg1))a = variableMap[arg1];
	else if (isValidNumber(arg1))a = BigNum(arg1);
	else {
		cout << Error::VariableName;
		return false;
	}

	if (doesVariableExist(arg2))b = variableMap[arg2];
	else if (isValidNumber(arg2))b = BigNum(arg2);
	else {
		cout << Error::VariableName;
		return false;
	}

	auto started = chrono::high_resolution_clock::now();

	if (operation == "sum" || operation == "+")variableMap["mem"] = a + b;
	else if (operation == "sub" || operation == "-")variableMap["mem"] = a - b;
	else if (operation == "mul" || operation == "*")variableMap["mem"] = a * b;
	else if (operation == "div" || operation == "/")variableMap["mem"] = a / b;
	else if (operation == "cmp" || operation == "<=>") {
		if (a < b)variableMap["mem"] = "First number is Smaller";
		else if (a == b)variableMap["mem"] = "Both numbers are Equal";
		else if (a > b) variableMap["mem"] = "First number is Greater"; 
	}
	printVariable("mem");
	auto done = chrono::high_resolution_clock::now();
	if (timeDisplay) {
		cout << "Finished in " << chrono::duration_cast<chrono::nanoseconds>(done - started).count() << " nanoseconds\n";
		cout << "Finished in " << chrono::duration_cast<chrono::milliseconds>(done - started).count() << " milliseconds\n";
	}
	return true;
}

bool isValidVariableName(string var)
{
	bool isValid = true;
	for (int i = 0; i < var.size() && isValid; i++) {
		isValid = var[i] == '_' || var[i] >= 'a' && var[i] <= 'z' || var[i] >= 'A' && var[i] <= 'Z';
	}
	return isValid;
}

bool isValidNumber(string num)
{
	if (num.size() == 1)return num[0] >= '0' && num[0] <= '9';
	if (num[0] == '.' || num[0] == '-' && num[1] == '.')return false;
	if (num[num.size() - 1] == '.')false;

	bool isNum = true;
	bool decimal_found = false;

	for (int i = num[0] == '-'; i < num.size() && isNum; i++) {
		isNum = num[i] >= '0' && num[i] <= '9' || !decimal_found && (decimal_found = num[i] == '.');
	}

	return isNum;
}

bool doesVariableExist(string var)
{
	return variableMap.find(var) != variableMap.end();
}

vector<string> splitInput(string input)
{
	stringstream ss(input);
	vector<string> input_args;
	string str;
	while (ss >> str) {
		input_args.push_back(str);
	}
	return input_args;
}

void displayTitle()
{
	cout << "##################  BigInt Mathematics !  ###################\n";
	cout << "########## Type help for a list of useful commands. #########\n";
}

void displayHelp()
{
	cout <<
		"## Commands ####\n"
		"## help - show a list of commands.\n"
		"## use 'mem' to access memory variable (last calculated value).\n"
		"## to declare a variable : [Variable name] [number] ; Eg -> a 1234\n"
		"## to declare a variable and copy its value from memory : [var] mem ; Eg -> a mem\n"
		"## to declare a variable equal to another variable : [var] [var] ; Eg -> a a\n"
		"## to print a variable's value : [var] ; Eg -> a\n"
		"## to add two variables/numbers : sum [var or num] [var or num] || [var or num] + [var or num]\n"
		"## to subtract two variables/numbers : sub [var or num] [var or num] || [var or num] - [var or num]\n"
		"## to multiply two variables/numbers : mul [var or num] [var or num] || [var or num] * [var or num]\n"
		"## to divide two variables/numbers : div [var or num] [var or num] || [var or num] / [var or num]\n"
		"## to copy the result into the variable : [var] command(sum, sub, mul, div, +, -, *, /)\n"
		"## time - display time taken after each solve.\n"
		"## clear - clear the memory.\n"
		"## exit - exit the program.\n\n";
}
