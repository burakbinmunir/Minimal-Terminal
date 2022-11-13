#include<iostream>
#include<string>
#include<cstring>
using namespace std;

char* getCharArray(string str) {
	char* arr = new char[str.size() + 1];

	for (int i = 0; i < str.size(); i++)
		arr[i] = str[i];

	arr[str.size()] = '\0';

	return arr;
}

char* getOperations(char* str, int& operationsCount){
	operationsCount = 0;
	int index = 0;
	while(str[index] != '\0') {
		if (str[index] == '|' || str[index] == '>' || str[index] == '<' || str[index] == '1' && str[index + 1] =='>' || str[index] == '0' && str[index] == '<') 
			operationsCount++;	
		index++;	
	}
	index = 0;
	char* operations = new char[operationsCount + 1];
	int count = 0;
	
	while(str[index] != '\0') {
		if (str[index] == '|' || str[index] == '>' || str[index] == '<' || str[index] == '1' && str[index + 1] =='>' || str[index] == '0' && str[index + 1] == '<') {
			if (str[index] == '0' || str[index] == '1') {
				operations[count] = str[index + 1];				
				index++;
				count++;
			}
			else {	
				operations[count] = str[index];
				count++;
			}
		}

		index++;	
	}
	
	return operations;
}

void trim(char* str)
{

	int size = strlen(str);
	
	int index = size;
	
	bool flag = true;
	int c = 0;
	while(flag)
	{
		if (str[index] == ' ' || str[index] == '\0'){
			index--;
			c++;
		}
		else flag = false;
	}
	
	str[size-c] = '\0';
	
	flag = true;
	
	index = 0;
	c = 0;
	while (flag){
		if (str[index] == ' '){
			c++;
			index++;
		}
		else flag = false;
	}
	 
	cout << "count: " << c << endl;
}


char** getAllCommands(char* str, char* operations, int opCount) {

	if (opCount <=0 ) return nullptr;
	char** command = new char*[opCount];
	
	char* s = new char[1];
	s[0] = operations[0];
	char* tok = strtok(str,s);
	
	command[0] = new char[strlen(tok)+1];
	command[0] = tok;
	command[0][strlen(tok)] = '\0';

	for (int index = 1; index < opCount; index++)
	{
		s[0] = operations[index];
		
		tok = strtok(NULL,s);
		command[index] = new char[strlen(tok)+1];
		command[index] = tok;
		command[index][strlen(tok)] = '\0';
		trim(command[index]);
	}
	return command;
}


int main () {
  string buffer;
  
  getline(cin,buffer);
 
 char* str = getCharArray(buffer);
  int opCount = 0;
  char* operations = getOperations(str,opCount);
  
 char**  commands = getAllCommands(str,operations,opCount);
 
 for (int i = 0; i < opCount; i+=1)
 	cout << commands[i] << endl;
  return 0;
}
