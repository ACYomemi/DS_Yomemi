#include "Tree.h"
using namespace std;
int main(void)
{
	FileEditor Fxt;
	string InFileName,OutFileName;
	cout<<"Enter file name here. If filename given is illegal, these filenames will be reset."<<endl;
	do
	{
		cout<<"Enter input file name:";
		cin>>InFileName;
		cout<<"Enter output file name:";
		cin>>OutFileName;
		Fxt.GetFileName(InFileName,OutFileName);
	} while (!Fxt.IsGoodFile());
	cout<<"Reading from input file"<<endl;
	Fxt.ReadFromInFile();
	cout<<"Read complete"<<endl;
	Fxt.GenerateCode();
	cout<<"Code generate complete"<<endl;
	string pass;
	if (Fxt.NeedPassword())
	{
		cout<<"Password needed. Enter password:";
		cin>>pass;
		if (!Fxt.IsRightPassword(pass))
		{
			return 0;
		}
	}
	else if (Fxt.GetFlag())
	{
		cout<<"1)Apply a password for the input file; 0)Go straight to generate output file"<<endl;
		cout<<">>>";
		int need;
		cin>>need;
		Fxt.SetPass(need!=0);
		if (need!=0)
		{
			cout<<"Enter password:";
			cin>>pass;
			Fxt.MakePassword(pass);
		}
	}
	cout<<"Writing to output file"<<endl;
	Fxt.WriteToOutputFile();
	cout<<"Write complete"<<endl;
	return 0;
}
