#include <bits/stdc++.h>
#define ASCII_RANGE 256
using namespace std;
class node
{
	public:
	unsigned char data;
	int weight;
	node* leftChild;
	node* rightChild;
	node* parent;
	node()
	{
		data=(unsigned char)0;
		leftChild=NULL;
		rightChild=NULL;
		parent=NULL;
		weight=-1;
	}
};
class bitExtractor
{
	public:
	string code;
	bitExtractor()
	{
		buffer=(unsigned char)0;
		len=0;
		code="";
	}
	void pushBit(int ch)
	{
		buffer=(buffer<<1)|ch;
		len++;
		if (len==8)
		writeToString();
	}
	void endOfInput()
	{
		code+=buffer;
		buffer=(unsigned char)len;
		code+=buffer;
	}
	private:
	unsigned char buffer;
	int len;
	void writeToString()
	{
		code+=buffer;
		buffer=(unsigned char)0;
		len=0;
	}
};
class HaffmanTree
{
	private:
	node* twig;
	node* head;
	void selectChildren(int counter)
	{
		
		int tag=-1;
		for (int i=0;i<counter;i++)
		{
			if (twig[i].parent==NULL)
			{
				tag=twig[i].weight;
				break;
			}
		}
		for (int i=0;i<counter;i++)
		{
			if (twig[i].parent==NULL&&tag>twig[i].weight)
			tag=twig[i].weight;
		}
		for (int i=0;i<counter;i++)
		{
			if (tag==twig[i].weight&&twig[i].parent==NULL)
			{
				twig[i].parent=&twig[counter];
				twig[counter].leftChild=&twig[i];
				twig[counter].weight=twig[i].weight;
				break;
			}
		}
		tag=-1;
		for (int i=0;i<counter;i++)
		{
			if (twig[i].parent==NULL)
			{
				tag=twig[i].weight;
				break;
			}
		}
		for (int i=0;i<counter;i++)
		{
			if (twig[i].parent==NULL&&tag>twig[i].weight)
			tag=twig[i].weight;
		}
		for (int i=0;i<counter;i++)
		{
			if (tag==twig[i].weight&&twig[i].parent==NULL)
			{
				twig[i].parent=&twig[counter];
				twig[counter].rightChild=&twig[i];
				twig[counter].weight+=twig[i].weight;
				break;
			}
		}
	}
	void init()
	{
		if (twig!=NULL)
		{
			delete []twig;
			twig=NULL;
			twig=new node[2*ASCII_RANGE-1];
		}
	}
	public:
	HaffmanTree()
	{
		twig=new node[2*ASCII_RANGE-1];
		head=NULL;
	}
	~HaffmanTree()
	{
		if (twig!=NULL)
		{
			delete []twig;
			twig=NULL;
		}
	}
	void initHaffmanTreeByArray(int weight[])
	{
		init();
		for (int i=0;i<ASCII_RANGE;i++)
		{
			twig[i].data=(unsigned char)i;
			twig[i].weight=weight[i];
		}
		for (int i=ASCII_RANGE;i<2*ASCII_RANGE-1;i++)
		{
			selectChildren(i);
		}
		for (int i=ASCII_RANGE;i<2*ASCII_RANGE-1;i++)
		{
			if (twig[i].parent==NULL)
			{
				head=&twig[i];
				break;
			}
		}
	}
	void initHaffmanTreeByString(string word)
	{
		int weight[ASCII_RANGE];
		for (int i=0;i<ASCII_RANGE;i++)
		weight[i]=0;
		for (int i=0;i<word.length();i++)
		weight[(unsigned char)word[i]]++;
		initHaffmanTreeByArray(weight);
	}
	string enCodeStringFrom(string word)
	{
		bitExtractor ext;
		for (int i=0;i<word.length();i++)
		{
			node* tmp=&twig[(unsigned char)word[i]];
			node* cur=tmp->parent;
			while (true)
			{
				if (cur==NULL)
				break;
				if (cur->leftChild==tmp)
				{
					ext.pushBit(1);
				}
				else
				{
					ext.pushBit(0);
				}
				tmp=tmp->parent;
				cur=tmp->parent;
			}
		}
		ext.endOfInput();
		return ext.code;
	}
	string deCodeStringFrom(string code)
	{
		string word;
		unsigned char tmp;
		node* cur=head;
		int len=code[code.length()-1];
		if (len!=0)
		{
			tmp=code[code.length()-2];
			for (int j=0;j<len;j++,tmp=tmp>>1)
			{ 
				if (tmp%2==1)
				cur=cur->leftChild;
				else
				cur=cur->rightChild;
				if (cur->leftChild==NULL&&cur->rightChild==NULL)
				{
					word+=cur->data;
					cur=head;
				}
			}
		}
		for (int i=code.length()-3;i>=0;i--)
		{
			tmp=code[i];
			for (int j=0;j<8;j++,tmp=tmp>>1)
			{ 
				if (tmp%2==1)
				cur=cur->leftChild;
				else
				cur=cur->rightChild;
				if (cur->leftChild==NULL&&cur->rightChild==NULL)
				{
					word+=cur->data;
					cur=head;
				}
			}
		}
		string p="";
		for (int i=word.length()-1;i>=0;i--)
		p+=word[i];
		return p;
	}
};
class fileEditor
{
	protected:
	int* weight;
	HaffmanTree ext;
	ifstream inputFile;
	ofstream outputFile;
	string inputText,outputText,password;
	void readTextFromInputFile()
	{
		string tmp="";
		while (inputFile.peek()!=EOF)
		{
			getline(inputFile,tmp);
			if (inputFile.peek()!=EOF)
			tmp+='\n';
			inputText+=tmp;
		}
	}
	void writeTextToOutputFile()
	{
		outputFile<<outputText;
	}
	int getOriginalFileSize()
	{
		inputFile.clear();
		inputFile.seekg(0,ios::end);
		return inputFile.tellg();
	}
	int getNewFileSize()
	{
		return outputFile.tellp();
	}
	public:
	fileEditor()
	{
		weight=new int[ASCII_RANGE];
		for (int i=0;i<ASCII_RANGE;i++)
		weight[i]=0;
		inputText="";
		outputText="";
		password="";
	}
	~fileEditor()
	{
		delete []weight;
		if (inputFile.is_open())
		inputFile.close();
		if (outputFile.is_open())
		outputFile.close();
	}
	bool isEndWithYOMEMI(string name)
	{
		return (name.length()>7&&name.find(".yomemi")==name.length()-7);
	}
	void initFileSource(string inputFileName,string outputFileName)
	{
		inputFile.open(inputFileName.data(),ios::in|ios::binary);
		outputFile.open(outputFileName.data(),ios::out|ios::binary);
	}
	bool isGoodFileSource()
	{
		return inputFile.is_open()&&outputFile.is_open()&&outputFile.tellp()==0;
	}
	void printMessage()
	{
		cout<<"The input file size is "<<getOriginalFileSize();
		cout<<"B, and the output file size is "<<getNewFileSize();
		cout<<"B."<<endl;
		double ratio=getOriginalFileSize()/(double)getNewFileSize();
		if (ratio>1)
		ratio=1/ratio;
		ratio*=100;
		cout<<"Compression ratio is "<<ratio<<"%."<<endl;
	}
};
class fileCompressor:public fileEditor
{
	private:
	void writeMessageToOutputFile()
	{
		for (int i=0;i<ASCII_RANGE;i++)
		outputFile<<weight[i]<<" ";
		if (password!="")
		outputFile<<1<<endl<<password;
		else
		outputFile<<0;
		outputFile<<endl;
	}
	public:
	void readInputFile()
	{
		readTextFromInputFile();
	}
	void writeOutputFile()
	{
		writeMessageToOutputFile();
		writeTextToOutputFile();
	}
	void generateCode()
	{
		for (int i=0;i<inputText.length();i++)
		weight[(unsigned char)inputText[i]]++;
		ext.initHaffmanTreeByString(inputText);
		outputText=ext.enCodeStringFrom(inputText);
	}
	void setPassword(string word)
	{
		password=ext.enCodeStringFrom(word);
	}
};
class fileDecompressor:public fileEditor
{
	private:
	void readMessageFromInputFile()
	{
		int pass;
		for (int i=0;i<ASCII_RANGE;i++)
		inputFile>>weight[i];
		inputFile>>pass;
		string tmp="";
		getline(inputFile,tmp);
		if (pass)
		getline(inputFile,password);
	}
	public:
	void readInputFile()
	{
		readMessageFromInputFile();
		readTextFromInputFile();
	}
	void writeOutputFile()
	{
		writeTextToOutputFile();
	}
	void generateCode()
	{
		ext.initHaffmanTreeByArray(weight);
		outputText=ext.deCodeStringFrom(inputText);
	}
	bool isPasswordNeeded()
	{
		return (password.length()!=0);
	}
	bool isCorrectPassword(string word)
	{
		return (ext.enCodeStringFrom(word)==password);
	}
};
