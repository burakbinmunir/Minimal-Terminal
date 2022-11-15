#include<iostream>
#include <sstream>
#include<string>
#include<sys/wait.h>
#include<cstring>
#include<unistd.h>
#include <stdlib.h>
#include<stdio.h>
#include<vector>
using namespace std;

// this function will calculate spaces in order to make rows for char** array or tokenization
int calculateSpace(string str) {

	int index = 0;
	int spaceCount = 0;

	while (index < str.length()) {
		if (str[index] == ' ') { 
			spaceCount++;
		}
		index++;
	}

	return spaceCount + 1; // one extra space for storing NULL
}

char** getCommand(string buffer, int& size) {

	size = calculateSpace(buffer); // calculating rows
	
	char** sent = new char* [size]; // one extra for storing NULL

	stringstream cmd(buffer);
	int index = 0;

	while (index < size) {
	
		string temp;
		getline(cmd, temp, ' '); // extracting till space and storing in temp

		sent[index] = new char[temp.length() + 1]; // extra for storing '\0'

		int count = 0;

		while (count < temp.length()) {
			sent[index][count] = temp[count];
			count++;
		}
		
		sent[index][count] = '\0';
		
		index++;
	}
	sent[size] = NULL;
	return sent;
}	

void printCommand(char** str, int size){ // for printing command ( for debug purposes )
	cout<<"Your entered command is\n";
	
	for (int i = 0; i < size; i++) {
	
		for (int j = 0; str[i][j] != '\0'; j++) 
			cout << str[i][j];
			
		cout<< endl;	
	}
}

void runCommand(char** command){
	if (strcmp(command[0], "cd")){ // in case if the given command is not for directory change
	
		char * temp = new char[strlen(command[0])];
		
		strcpy(temp,"/bin/");
		strcpy(temp,command[0]); // copying command
		
		if (execvp(temp,command) < 0)
			cout << "Command cannot be executed" << endl;
	}
	else {
		chdir(command[1]); // changing directory
	}
}

bool isNumber(char str){
	return (int(str) > 47 && int(str) < 58);
}
	
bool allNum(string buffer)
{
	for (int index = 1; index < buffer.size(); index++)
		if (!isNumber(buffer[index]))
			return false;
	
	return true;
		
}
int historyCommand (string buffer) {
	if (buffer.compare("!") == 0) return 3;
	
	if (buffer.compare("history") == 0)
		return 0;
	
	if (buffer.compare("!!") == 0 )
		return 1;
		
	if (buffer[0] == '!' && buffer[1] != '!'){
		bool check = allNum(buffer);
		if (check)	return 2;
		
		return 3;
	}

	return 4;		
}	

void displayCommands (vector<string> historyCmd) {
	if (historyCmd.size() == 0 ) {
		cout << "No Commands exist in history\n\n";
		return;
	}
	int startIndex = 0, endIndex = 0;
	if (historyCmd.size() >= 10){
		 startIndex = historyCmd.size() - 1;
		 endIndex = startIndex - 10;
	}
	else {
		cout << "Displaying available commands\n\n";
		startIndex = historyCmd.size() - 1;
		endIndex = -1 ;
	}
	for(int i= startIndex ; i > endIndex ; i--)
		cout << i + 1 << ": " << historyCmd[i] << endl;
}

void typeExc(string buffer, vector<string> historyCmd,bool& successExecution) {
	if (historyCmd.size() == 0)
	{
		cout << "No command to execute\n" << endl;
		successExecution = false;
		return;
	}	
	
	int retVal = fork();
	if (retVal > 0 )
	{	
		successExecution = true;
		wait(NULL);
	}
	if (retVal == 0)
	{	//cout << "BACK" << endl; debugging purposes
		int size = 0;
		buffer = historyCmd.back();
		cout << "\t\t\t Command going to execute: " << buffer << endl << endl;

		char** command = getCommand(buffer, size); // extracting command				
		runCommand(command); // running command
		successExecution = false;
	}
	
	if (retVal < 0 )
	{
		cout << "Error in forking" << endl;
		perror("fork");
	}
	
}

void typeExcNum(string buffer, vector<string> historyCmd, bool& successExecution,string& executedCommand) {
	if (historyCmd.size() == 0) 
	{
		cout << "No command in history" << endl;
		successExecution = false;
		return;
	}
	string newCmd = buffer;
	newCmd.erase(newCmd.begin() + 0);
	int index  = stoi (newCmd);
	string cmd;
	if (historyCmd.size() >= index ){
		cmd = historyCmd[index -1];
		executedCommand = cmd;
		cout << "\t\t\t Command going to execute: " << executedCommand << endl;
		int retVal = fork();
		if (retVal > 0 )
		{	
			successExecution = true;
			wait(NULL);
		}
		if (retVal == 0)
		{
			int size = 0;
			char** command = getCommand(cmd, size); // extracting command				
			runCommand(command); // running command
			successExecution = false;
		}
		
		if (retVal < 0 )
		{
			cout << "Error in forking" << endl;
			perror("fork");
		}
		

	}
	else{
		 cout << "Command does not exist" << endl << endl;
		 successExecution = false;
	}
	
	
}

int main() {

	
	bool flag = false;
	vector<string> historyCmd;
	
	while (flag == false){
	
		string buffer; // to store command
		
		char buff[250]; // to store current working directory
		getcwd(buff,250);
		
		cout<< buff <<" Enter Command: ";
		getline(cin, buffer);
		
		bool successExecution = false;
		bool forkFlag = false;
		bool partOne = false;
		if (buffer.compare("exit") != 0) {
			int cmdType = historyCommand(buffer);
			
			if (cmdType <= 3) {
				partOne = true;
				if (cmdType == 0)
					displayCommands(historyCmd);
				if (cmdType == 1){
					typeExc(buffer,	historyCmd,successExecution);
					if (successExecution == true)
						historyCmd.push_back(historyCmd.back());
				}
				if (cmdType == 2){
					string executedCommand;
					typeExcNum(buffer,historyCmd,successExecution,executedCommand);
					if (successExecution == true)
						historyCmd.push_back(executedCommand);
				}
				if (cmdType == 3)
					cout << "Wrong Command" << endl;
			}
			else {
				pid_t id = fork();
				if (!forkFlag){
					if (id > 0) { // parent
					//	cout<<"Parent is waiting\n"; // for debugging purposes
						successExecution = true;
						wait(NULL);
					//	cout<<"Parent is executing\n"; // for debugging purposes
					}
					
					if (id == 0) { // child
					
						int size = 0;
						cout << "\t\t\t Command going to execute: " << buffer << endl << endl;
						char** command = getCommand(buffer, size); // extracting command				
						runCommand(command); // running command
						successExecution = false;
					}	
					
					if (id < 0){ // failed fork 
					
						cout<<"Unsuccessful fork"<<endl;
						perror("fork");
					}
				}
				forkFlag = true;
			}
		}
		else {
			 break;	// to stop the program
		}
		if (successExecution && !partOne)
			historyCmd.push_back(buffer);
	}
	
			
	return 0;
}
