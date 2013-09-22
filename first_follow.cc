#include <iostream>
#include <string>

using namespace std; 

void getInput();
void structureInput();
void copyString(int, int, int, int);

string grammar_array[100];
string structured_input[100];

int main () 
{
	getInput();
	structureInput(); 
	return 0;
} 

void getInput()
{
	string line;
	int i=0;
	cout << "Enter grammar speicification"<<endl;
	while(true)
	{
		getline(cin, line);
		if (line.empty())
			break;
		grammar_array[i] = line;
		i++;	
					
	}

	for(i=0;i< 100;i++)
	{	
		if (grammar_array[i].empty())
			break;
		cout << "Grammar specification" << "  " <<grammar_array[i]<<endl;
	}
}

void structureInput()
{
	int i=0,j=0;
	int start_i=0,end_i=0;
	int start_j=0,end_j=0;

	for(i=0;;)
	{
		if (grammar_array[i].empty())
			break; 
		for(j=0;;)
		{
			if (grammar_array[i][j] == '#')
			 	{
					end_i = i;
					end_j = j;
					copyString(start_i, end_i, start_j, end_j);
					start_i = i;
					end_j = j;
				}
			i++;
			j++;
		}

	}
}

void copyString(int start_i, int end_i, int start_j, int end_j)
{
	int count=0, total_char, i, j;
	string new_sentence; 
	total_char = end_i * end_j + start_i * start_j + 1;
	for(i=start_i; ; i++)
	{
		for(j=start_j;;j++)
		{
			if(grammar_array[i][j] == '/0')
				 break;
			if(count <= total_char)
				{
				count ++;
				new_sentence[count] = grammar_array[i][j];
				}
				
		}

	}
}
