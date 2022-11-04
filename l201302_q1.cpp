#include<iostream>
#include <sstream>
#include<string>
#include<sys/wait.h>
#include<cstring>
#include<unistd.h>
#include <stdlib.h>
#include<stdio.h>
#include<fcntl.h>
using namespace std;

char* getCharArray(string str) {
	char* arr = new char[str.size() + 1];

	for (int i = 0; i < str.size(); i++)
		arr[i] = str[i];

	arr[str.size()] = '\0';

	return arr;
}

char* returnBackString(string str, int start, int end) {

	if (str[start] == '0' || str[start] == '1')
		start += 2;
	else if (str[start] == '>' || str[start] == '<' || str[start] == ' ')
		start++;


	int size = end - start + 1;
	char* newStr = new char[size + 1];

	int c = 0;
	for (int i = start; i <= end; c++, i++)
		newStr[c] = str[i];

	newStr[c] = '\0';

	return newStr;
}

char* returnForwardString(string str, int index) {
	if (str[index] == '>' || str[index] == '<')
		index++;
	else
		if (str[index] == '0' || str[index] == '1')
			index += 2;

	 // checking for next delimeter
	int end = 0;
	bool flag = true;
	int i = index;
	while (flag == true && i < str.size())
	{
		if (str[i] == '|' || str[i] == '>' || str[i] == '<')
			flag = false;
		else
			end++;
		i++;
	}
	
	char* charArr = new char[end + 1];
	int c = 0;
	for (int a = index; a < end; a++)
		charArr[c] = str[a];

	charArr[c] = '\0';
	return charArr;
}


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
			cout << "Error prone command\n\n";
	}
	else {
		chdir(command[1]); // changing directory
	}
}

bool checkDelim(char* delim, int index) {
	if (delim[index] == '|' || delim[index] == '>' || delim[index] == '<') {
		return true;
	}
	else {
		if (delim[index] == '1')
			if (delim[index + 1] == '>')
				return true;

		if (delim[index] == '0')
			if (delim[index] == '<')
				return true;

		return false;
	}
}

bool containsDelim(string delim){
	for (int index=0; index < delim.size();index++ ){
      if (delim[index] == '|' || delim[index] == '>' || delim[index] == '<' || delim[index] == '1' && delim[index + 1] == '>' || delim[index] == '0' && delim[index] == '<') 
	         return true;
	  
	}
	return false;
}

int main() {

	
	bool flag = false;
	
	while (flag == false){
	
		string buffer; // to store command
		
		char buff[250]; // to store current working directory
		getcwd(buff,250);
		
		cout<< buff <<" Enter Command: ";
		getline(cin, buffer);
			
		if (buffer.compare("exit") != 0) {
		
			pid_t id = fork();
			if (id > 0) { // parent
			//	cout<<"Parent is waiting\n"; // for debugging purposes
				wait(NULL);
			//	cout<<"Parent is executing\n"; // for debugging purposes
			}
			
			if (id == 0) { // child
			    if (!containsDelim(buffer)){
				    int size = 0;
				    char** command = getCommand(buffer, size); // extracting command				
				    runCommand(command); // running command
				}
				else {
						char* str = getCharArray(buffer);
						char** command = nullptr;
						int size = 0;

						int forkFlag = false;
						for (int i = 0; i < buffer.size(); ) {
							int startIndex = i;
							bool flag = true;
							char* tempCommand = nullptr;

							while(flag == true && i < buffer.size()){
								if (checkDelim(str, i) || i == buffer.size() - 1){
									
									if (str[i] == '|' ){
										tempCommand = returnBackString(str, startIndex, i - 2);
									}
									else {
										if (i == buffer.size() - 1){
											tempCommand = returnBackString(str, startIndex + 1, i);
										}
										else{
											if (str[i] == '0' || str[i] == '1' || str[i] == '>' || str[i] == '<') {
												tempCommand = returnBackString(str, startIndex, i - 2);
												//cout << "Command 1:";
												//printCommand(command, size);
												//runCommand(command);
												tempCommand = returnForwardString(str, i + 2);
												//runCommand(command);
											}
										}
									}
									flag = false;
									
								}	
								i++;
							}
							if (forkFlag == false){
								int fd[2];
								int pipeStatus = pipe(fd);
						
								pid_t retVal = fork();

								if (retVal < 0){
									cout << "Error while forking"<< endl;
									perror("fork");
								}
						

								if (retVal > 0 ){
									int tempSize = strlen(tempCommand);
									char cmd[tempSize];
									strcpy(cmd ,tempCommand);
								
									write(fd[1], &tempSize , sizeof(int));
									write(fd[1], &cmd, tempSize);
									wait(NULL);
									close(fd[0]);
									close(fd[1]);
								}

								if (retVal == 0){
									int commandSize = 0;
									read (fd[0], &commandSize, sizeof(int));
//									cout << "Command size: " << commandSize << endl;

									char tempCommand[commandSize + 1];
									read (fd[0], &tempCommand, commandSize);

									tempCommand[commandSize] = '\0';
									//cout << "Command read: "<< tempCommand << endl;
									int s = 0;
									char** command = getCommand(tempCommand,s);
									
									dup2(fd[1],1);
									runCommand(command);
									close(fd[0]);
									close(fd[1]);
								}
							}
						}
						
				}
			}
			
			if (id < 0){ // failed fork 
			
				cout<<"Unsuccessful fork"<<endl;
				perror("fork");
			}
		}
		else {
			 flag = true;	// to stop the program
		}
	}	
	return 0;
}