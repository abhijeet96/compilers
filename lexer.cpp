#include<bits/stdc++.h>
using namespace std;

#define pb push_back

int comment_flag=0;

vector<string> reserved={"if","while","and","or","not","return","elsif","def","for","in","end","then","do","true","false","Array"};

vector<string> symbols={";","=","..","+","-","*","**","/","|","%","^","&",">","<",">=","<=","==","!=","!","&&","||","(",")","*","[","]","$"};

vector<string> split(string line,char ch)
{
	vector<string> ans;
	string temp="";
	for(int i=0;i<line.size();i++)
	{
		if(line[i]==ch && temp!="")
		{
			ans.pb(temp);
			temp="";
		}
		else if(line[i]!=ch)
		temp+=line[i];
	}
	if(temp!="")
	ans.pb(temp);
	return ans;
}

int isReserved(string inp)
{
	for(int i=0;i<reserved.size();i++)
		{
			if(reserved[i]==inp)
				return 1;
		}
		return 0;
}

vector<string>  passToDfa(string inp)
{
	vector<string> temp; // return this

	int state=0;
	int next_state;
	char curr=inp[0];
	char next_char;

	if(inp.length()==1)
	{
		temp.pb(curr+"");
		return temp;
	}
	
	int str_iter=0;
	int length=inp.length();

	inp+=" ";
	
	while(str_iter<length)
	{
		curr=inp[str_iter];
		next_char=inp[str_iter+1];

		while(curr==' ' && str_iter<length)   //skipping whitespaces
		{
			str_iter++;
			curr=inp[str_iter];
			next_char=inp[str_iter+1];
		}

		switch(curr)
		{
			case '*':{
					if(next_char=='*')
					{
						temp.pb("**");
						str_iter++;
					}
					else
					{		
					temp.pb("*");									
					}
					break;
				}				
			case '+':
			case '-':
			case ';':
			case '/':
			case '%':
			case '^':	
			case '(':
			case ')':
			case ']':
			case '[':
			case '$':
			case ',':		
			{
				string y(1,curr);
				temp.pb(y);
				break;
			}
			case '>':
			{
				if(next_char=='=')
				{
					temp.pb(">=");
					str_iter++;
				}
				else
				{
					temp.pb(">");
					break;
				}			
				break;
			}
			case '<':
			{
				if(next_char=='=')
				{
				temp.pb("<=");
					str_iter++;
				}
				else
				{
					temp.pb("<");
				}	
				break;		
			}
			case '!':
			{
				if(next_char=='=')
				{
					temp.pb("!=");
					str_iter++;
				}
				else
				{
					temp.pb("!");
				}				
				break;
			}
			case '&':
			{
				switch(next_char)
				{
					case '&':
					{
						temp.pb("&&");
						str_iter++;
						break;
					}
					default:
					{
						temp.pb("&");
						break;
					}
				}
				break;
			}
			case '=':
			{
				if(next_char=='=')
				{
					temp.pb("==");
					str_iter++;
				}
				else if(next_char=='b')
				{
					string comstart="begin";
					int sto=str_iter;
					int flag=0;
					for(int i=1;i<5;i++)
					{
						str_iter++;
						if(str_iter>=length)
						{
							flag=1;
							break;
						}
						curr=inp[str_iter];
						next_char=inp[str_iter+1];
						if(next_char!=comstart[i])
						{
							flag=1;
							break;
						}
					}
					if(flag==1)
					{
						temp.pb("=");
						str_iter=sto;
					}
					else
					{
						comment_flag=1;
					}
				}
				else if(next_char=='e')
				{
					string comend="end";
					int sto=str_iter;
					int flag=0;

					for(int i=1;i<3;i++)
					{
						str_iter++;
						if(str_iter>=length)
						{
							flag=1;
							break;
						}
						curr=inp[str_iter];
						next_char=inp[str_iter+1];
						if(next_char!=comend[i])
						{
							flag=1;
							break;
						}
					}
					if(flag==1)
					{
						temp.pb("=");
						str_iter=sto;
					}
					else if(comment_flag==0)
					{
						temp.pb("Error comment start not avl");						
					}
					else if(flag==0 && comment_flag==1)
					{
						comment_flag=0;
					}
				}
				else
				{
					temp.pb("=");
				}				
				break;
			}
			case '|':
			{
				if(next_char=='|')
				{
					temp.pb("||");
					str_iter++;
				}
				else
				{
					temp.pb("|");
				}				
				break;
			}
			case 'a' ... 'z':
			case 'A' ... 'Z':
			{
				string tok(1,curr);				
				
				while(isalpha(next_char)||isdigit(next_char)||next_char=='_' && str_iter<length)
				{
					str_iter++;
					curr=next_char;
					tok+=curr;				
					next_char=inp[str_iter+1];
				}		
				

				if(isReserved(tok))
				{
					temp.pb(tok+" KEYWORD");
				}
				else
				{
					temp.pb(tok+" IDENTIFIER");
				}
				break;
			}

			case '1' ... '9':
			{
				string num(1,curr);
				while(isdigit(next_char) && str_iter<length)
				{
					str_iter++;
					curr=next_char;
					num+=curr;
					next_char=inp[str_iter+1];
				}

				if(next_char=='.')
				{
					str_iter++;
					curr=next_char;
					num+=curr;
					next_char=inp[str_iter];

					while(isdigit(next_char) && str_iter<length)
					{
						str_iter++;
						curr=next_char;
						num+=curr;
						next_char=inp[str_iter+1];
					}

					temp.pb(num+" Floating constant");
				}
				else
				{
					temp.pb(num+" Integer constant");
				}
				break;
			}
			case'0':
			{
				string num(1,curr);
				if(next_char=='.')
				{
					num+=next_char;
					str_iter++;
					curr=next_char;
					next_char=inp[str_iter+1];

					while(isdigit(next_char) && str_iter<length)
					{
						str_iter++;
						curr=next_char;
						num+=curr;
						next_char=inp[str_iter+1];
					}

					temp.pb(num+" Floating constant");
				}
				else if(isdigit(next_char))
				{
					int fl=0;
					while(isdigit(next_char))
					{
						str_iter++;
						curr=next_char;
						next_char=inp[str_iter+1];
						fl=1;
					}
					if(fl==1)
					{
						temp.pb("Error Octa, hexa not supported");
					}
				}
				else
				{
					temp.pb(num+" Integer constant");
				}
				break;
			}
			case '\'':
			{
				string tok(1,curr);				
				
				while(next_char!='\'' && str_iter<length)
				{
					str_iter++;
					curr=next_char;
					tok+=curr;				
					next_char=inp[str_iter+1];
				}	

				if(next_char!='\'')
				{
					temp.pb("Error: matching ' not found");
				}
				else
				{
					tok+=next_char;
					str_iter++;
					temp.pb(tok+"single quote string");
				}
				break;
			}
			case '"':
			{
				string tok(1,curr);				
				
				while(next_char!='"' && str_iter<length)
				{
					str_iter++;
					curr=next_char;
					tok+=curr;				
					next_char=inp[str_iter+1];
				}	

				if(next_char!='"')
				{
					temp.pb("Error: matching \" not found");
				}
				else
				{
					tok+=next_char;
					str_iter++;
					temp.pb(tok+"Double quote string");
				}
				break;
			}	
			case '#': //single line comment
			{
					while(str_iter<length) 
					{
						str_iter++;
						curr=inp[str_iter];
						next_char=inp[str_iter+1];
					}
				break;
			}
			case '@':
			{
				temp.pb("@");
				break;
			}
		}
		str_iter++;
	}
}


int main()
{
	//for(int i=0;i<reserved.size();i++)
	//	cout<<reserved[i]<<endl;
	ofstream fout;
  	fout.open ("output.txt");
  	
	
	string line;
	ifstream myfile ("example.txt");
	//ifstream myfile ("small.txt");
	int line_no=1;	
	char curr_char;
	char next_char;	

	if (myfile.is_open())
	{
		while ( getline (myfile,line) )
		{
		 	vector<string> outputstr=passToDfa(line);

		 	if(comment_flag==0)
			for(int i=0;i<outputstr.size();i++)
			{
				fout<<outputstr[i]<<" on line no."<<line_no<<endl;
			}
			line_no++;
		}		
	}
			
		
	myfile.close();
	fout.close();
	return 0;
}
 
