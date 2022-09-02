#include "HaffmanTree.h"
using namespace std;
int main(void)
{
	int flag;
	cout<<"Please enter the mode."<<endl;
	cout<<"0)Exit 1)Compress 2)Decompress"<<endl<<"Choice>>>";
	cin>>flag;
	if (flag==0)
	return 0;
	else if (flag==1)
	{
		fileCompressor fc;
		string inputFileName,outputFileName;
		do 
		{
			cout<<"Please enter input file name."<<endl<<"Input file name>>>";
			cin>>inputFileName;
			cout<<"Please enter output file name."<<endl;
			cout<<"Output file name is excepted to end with a '.yomemi'. ";
			cout<<"If given filename doesn't contain it, ";
			cout<<"the program will automatically add it at the end."<<endl<<"Output file name>>>";
			cin>>outputFileName;
			if (!fc.isEndWithYOMEMI(outputFileName))
			outputFileName+=".yomemi";
			cout<<"Searching and load target file..."<<endl; 
			fc.initFileSource(inputFileName,outputFileName);
			if (!fc.isGoodFileSource())
			cout<<"Fail to load file, please retry!"<<endl;
		} while (!fc.isGoodFileSource());
		cout<<"Reading file..."<<endl;
		fc.readInputFile();
		fc.generateCode();
		cout<<"Reading complete!"<<endl;
		int sel;
		cout<<"Do you want to apply a password for output file?"<<endl;
		cout<<"1)Apply a password  2)Do not apply"<<endl<<"choice>>>";
		cin>>sel;
		if (sel==1)
		{
			string word;
			getline(cin,word);
			cout<<"Please enter the password below."<<endl<<"password>>>";
			getline(cin,word);
			fc.setPassword(word);
		}
		cout<<"Writing file..."<<endl;
		fc.writeOutputFile();
		cout<<"Writing complete!"<<endl; 
		fc.printMessage();
		return 0;
	}
	else if (flag==2)
	{
		fileDecompressor fd;
		string inputFileName,outputFileName;
		do 
		{
			cout<<"Please enter input file name."<<endl;
			cout<<"Input file name is excepted to end with a '.yomemi'. ";
			cout<<"If given filename doesn't contain it, ";
			cout<<"the program will automatically add it at the end."<<endl<<"Input file name>>>";
			cin>>inputFileName;
			if (!fd.isEndWithYOMEMI(inputFileName))
			inputFileName+=".yomemi";
			cout<<"Please enter output file name."<<endl<<"Output file name>>>";
			cin>>outputFileName;
			cout<<"Searching and load target file..."<<endl; 
			fd.initFileSource(inputFileName,outputFileName);
			if (!fd.isGoodFileSource())
			cout<<"Fail to load file, please retry!"<<endl;
		} while (!fd.isGoodFileSource());
		cout<<"Reading file..."<<endl;
		fd.readInputFile();
		fd.generateCode();
		cout<<"Reading complete!"<<endl;
		bool unslove=false;
		if (fd.isPasswordNeeded())
		{
			string word;
			getline(cin,word);
			cout<<"Password needed. Please enter the password below."<<endl<<"Password>>>";
			getline(cin,word);
			
			if (!fd.isCorrectPassword(word))
			{
				unslove=true;
				cout<<"Wrong password. Exit."<<endl;
			}
		}
		if (unslove)
		return 0;
		cout<<"Writing file..."<<endl;
		fd.writeOutputFile();
		cout<<"Writing complete!"<<endl; 
		fd.printMessage();
		return 0;
	}
	return 0;
}
