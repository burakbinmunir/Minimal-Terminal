#include<iostream>
#include <sstream>
#include<string>
#include<sys/wait.h>
#include<cstring>
#include<unistd.h>
#include <stdlib.h>
#include<stdio.h>
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
		
		execvp(temp,command);
	}
	else {
		chdir(command[1]); // changing directory
	}
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
			
				int size = 0;
				char** command = getCommand(buffer, size); // extracting command				
				runCommand(command); // running command
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
