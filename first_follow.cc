#include <iostream>
#include <string>

using namespace std; 

int getInput();
void cleanInput(int);
void copyToken(int, int, int, int);
void printStructuredInput(int);

string grammar_array[100];
string structured_input[100];

int main () 
{
	int count;
	count = getInput();
	cleanInput(count);
	printStructuredInput(count); 
	return 0;
} 

int getInput()
{
//Get the input.

	string line;
	int i, count =0;
	cout << "Enter grammar specification"<<endl;
	while(true)
	{
		getline(cin, line);
		if (line.empty())
			break;
		grammar_array[count] = line;
		count++;	
	}

	for(i=0;i < count; i++)
	{	
		cout << "Grammar specification" << "  " <<grammar_array[i]<<endl;
	}
	return count;
}

void printStructuredInput(int count)
{
//Just print the array
	int i,j;
	for (i=0;i< count; i++)
	{
		for (j=0;;j++)
		{
			if (structured_input[i][j] == '\0')
				break;
			cout << "Structured Input " << structured_input[i][j]<<endl;
		}
	}

}

void cleanInput(int count)
{
//Inputs can have multiple statements in a single statement separated by # and ## and the end of grammar. So properly put them in an array such that every single rule is separately stored. 

	int rows_count = count;
	int start_col = 0;	
	int end_col = 0;
	int rule_count = 0;
	int i,j;

	for(i=0;i<rows_count;i++)
	{
		for(j=0;;j++)
		{
			if(grammar_array[i][j] == '#')   //If we encounter # it is end of a rule
			{
				end_col = j;
				copyToken(i,start_col, end_col, rule_count);
				rule_count += 1;
				start_col = end_col;
				break;
			}
		
		}
	}
}


void copyToken(int row, int start_col, int end_col, int rule_count)
{
//Utility function to copy string from the given start and end index values
	int count =0;
	int i;
	
	for(i=start_col;i<end_col;i++)
	{
		if (isspace(grammar_array[row][i]))
		{
			continue;
		}
		structured_input[rule_count][count] = grammar_array[row][i];
		count ++;
	}
	
}


//Input getting part need to be revisited for sure. 
