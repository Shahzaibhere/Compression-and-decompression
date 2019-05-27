#include<iostream>
#include<string>
#include<map>
#include<queue>
#include<chrono>
#include<bitset>
#include<stdio.h>
#include<fstream>
using namespace std;

int Search(char arrs, char arrn[], int l, int h);
void HuffmanTreeMaker(char arrc[], int arrn[], int size, string org);
string inputss;
map<char, string>codes;
map<string, char>keys;
struct Node  // defines a sructure of the minimum heap data structure used to implement the Huffman's Tree later in this program
{
	Node *left;
	Node *right;
	int frequency;
	char alphabet;
	string in;
	int count;
	void Taker(char *input)
	{
		in = input;
	}
};

struct compare // Compares two characters based on their frequencies and uses them in the Huffman's tree making function accordingly as per requirement
{
	bool operator()(const Node *n1, const Node *n2)const
	{
		if (n2->frequency == n1->frequency)
		{
			return n2->count < n1->count;
		}
		else
		{
			return n2->frequency < n1->frequency;
		}

	}
};
Node *Initializer(Node *head, char alpha, int freq, int pos);
//void file_read();
void storeCodes(Node *root, string str);
void FileHandler(string input, char arrc[]);
void Compressed_File_Reader();
//string Decompressor(Node *root,string input);

int main()
{
	cout << " *************************************************************************" << endl;;
	cout << " Welcome to .txt File compressor and decompressor system " << endl;
	auto start = chrono::high_resolution_clock::now(); // to initialise the time taken for the process to happen
	string temp;
	string s;
	ifstream myfile("Input.txt"); // opens file if it exists 
	if (myfile.is_open())
	{
		while (getline(myfile, temp))
		{
			s = s + temp;
		}
		myfile.close();
	}
	else
	{
		cout << "unable to open file as input file is non-existant. Please make an input file for this process" << endl; // file doesn't exist prompts user to add input text that is required to be compressed 
	}
	cout << "Reading the file now... " << endl;
	int *arr = new int[s.size()];
	char *arrc = new char[s.size()];
	string alphabet = "abcdefghijklmnopqrstuvwxyz ABCDEFGHIJKLMNOPQRSTUVWXYZ.0123456789;:'<>?/(){}()!$_-";
	map<char, int> freq_dict;
	int count = 0, check;
	for (int i = 0; i<71; i++) {
		freq_dict[alphabet[i]] = 0;
	}

	for (int j = 0; j < s.size(); j++) {
		freq_dict[s[j]]++;
	}
	for (int k = 0; k < s.size(); k++)
	{
		check = Search(s[k], arrc, 0, count);
		if (check != 1)
		{
			arrc[count] = s[k];
			arr[count] = freq_dict[s[k]];
			count++;
		}
	}

	/*for (int l = 0; l < count; l++)
	{
	cout << "alphabet\t" << arrc[l] << "\t" << "Frequency\t" << "\t" << arr[l] << endl;

	}*/

	HuffmanTreeMaker(arrc, arr, count, s);
	auto finish = chrono::high_resolution_clock::now();
	chrono::duration<double>elapsed = finish - start; // calculates the total time taken for compression to happen
	cout << "*****************************************************************" << endl;
	cout << "Thank you for using the system, you may find the newly created compressed and decompressed file in the same folder " << endl;
	cout << "Elapsed time:" << elapsed.count() << "s\n" << endl;

	system("pause");
	return 0;

}
//Used for collecting the characters in an array 
int Search(char arrs, char arrn[], int l, int h)
{
	int count = 2; int check = 0;
	for (int i = l; i < h; i++)
	{
		if (arrs != arrn[i])
		{
			count = 2;
			check = 1;
		}
		else
		{
			count = 1;
			check = 1;
			return count;
		}

	}
	if (count != check)
	{
		return count;
	}

}

void HuffmanTreeMaker(char arrc[], int arrn[], int size, string org) // uses the min heap data structure for storage of nodes(characters) based on their frequencies in the input text.
{
	priority_queue <Node*, vector<Node*>, compare> HuffMan;
	int count = 0;

	for (int i = 0; i < size; i++)
	{
		Node *head = NULL;
		Node *temp = NULL;
		temp = Initializer(head, arrc[i], arrn[i], i);
		temp->Taker(arrc);
		HuffMan.push(temp);
	}

	for (int j = 0; j < size - 1; j++) //  takes two least frequent nodes and adds their frequencies to generate a new node until a single node is left to design the Huffman tree
	{
		int Head_Freq;

		Node * temp = new Node;
		Node *First = HuffMan.top();
		HuffMan.pop();
		Node *Second = HuffMan.top();
		HuffMan.pop();
		Head_Freq = First->frequency + Second->frequency;
		temp->frequency = Head_Freq;
		temp->left = First;
		temp->right = Second;
		temp->count = j + size;
		temp->alphabet = '#';
		HuffMan.push(temp);
	}
	storeCodes(HuffMan.top(), "");
	FileHandler(org, arrc);
	Compressed_File_Reader();
	/*while (!HuffMan.empty())
	{
	cout << "The alphabet is\t" << HuffMan.top()->alphabet << "\t" << "The Frequency of this alphabet is\t" << HuffMan.top()->frequency << "\t" << endl;
	HuffMan.pop();
	}*/

}
Node *Initializer(Node *head, char alpha, int freq, int pos)
{
	Node *temp = new Node;
	temp->frequency = freq;
	temp->alphabet = alpha;
	temp->count = pos;
	temp->left = NULL;
	temp->right = NULL;
	return temp;
}
void storeCodes(Node *root, string str) // after development of the Huffman's tree the codes are assigned to each character based on their position from the root node, a left child(has 0 binary value coded) while a right child( has 1 binary value code)
{
	if (root == NULL)
		return;
	if (root->alphabet != '#')
		codes[root->alphabet] = str;
	storeCodes(root->left, str + "0");
	storeCodes(root->right, str + "1");
}
//File Encoder as learnt from internet
void FileHandler(string input, char arrc[])
{
	int count = 0;
	string x;
	cout << sizeof(arrc) << endl;
	map<char, string>::iterator l, j;
	/*for (l = codes.begin(); l != codes.end(); l++)
	{
	cout << l->first << " " << l->second << endl;
	}*/
	for (int i = 0; i < input.size(); i++)
	{
		for (j = codes.begin(); j != codes.end(); j++)
		{
			if (j->first == input[i])
			{
				x = x + codes[j->first];
			}
		}
	}
	//cout << x << endl;
	//inputss = x;
	int pad_count = 0;
	while (x.length() % 32 != 0) {
		x += "0";
		pad_count++;
	}
	//	cout << "new bitstring\n" << bitString << endl;
	fstream outputFile("binary coded.bin", ios::out | ios::binary);
	cout << "File bits\n";
	for (int i = 0; i < x.length(); i += 32) {
		bitset<32> t(x.substr(i, 32));

		//	cout << t.to_string() << endl;
		outputFile.write(reinterpret_cast<const char *>(&t), sizeof(t));
	}
	outputFile.close();
	cout << "file bits end, The file is compressed now. \n";


	ofstream keyFile("keys.txt");
	for (map<char, string>::iterator it = codes.begin(); it != codes.end(); ++it)
	{
		keyFile << (int)it->first;
		keyFile << " " + it->second << endl;
	}
	keyFile << pad_count;
	keyFile.close();


}
/*
string Decompressor(Node *root, string input)
{
string inputs = Compressed_File_Reader();
string ans = "";
Node* curr = root;
for (int i = 0; i<inputs.size(); i++)
{
if (inputs[i] == '0')
curr = curr->left;
else
curr = curr->right;

// reached leaf node
if (curr->left == NULL && curr->right == NULL)
{
ans += curr->alphabet;
curr = root;
}
}
// cout<<ans<<endl;
return ans + '\0';
}
*/
void Compressed_File_Reader() // reverses the compressed file to remake the decompressed file by using the keys.txt file developed from the original text file
{
	ifstream keysFile("Keys.txt");
	if (keysFile.is_open()) {
		string temp;
		while (getline(keysFile, temp)) {
			if (temp.length() > 2)
			{
				int i = 0;
				while (temp[i] != ' '&&i < temp.length())
				{
					i++;
				}
				char c = (char)stoi(temp.substr(0, i));
				string code = temp.substr(i + 1);
				keys[code] = c;
			}
		}
		keys["padd_count"] = stoi(temp);
	}
	keysFile.close();
	/*map<string, char> ::iterator it;
	for (it = keys.begin(); it != keys.end(); it++)
	{
	cout << it->first << " " << it->second << endl;
	}*/


	fstream to_decompress("binary coded.bin", ios::in | ios::binary); // used to decompress the binary coded file as mentioned above ( this part was seen from internet sources due to time constraint)
	bitset<32> t;
	string bitString;
	string temp;
	while (to_decompress.read(reinterpret_cast<char *>(&t), sizeof(t)))
	{
		//		cout << t.to_string()<<endl;
		bitString += t.to_string();
	}
	string ll;
	ofstream file_1("Decompressed.txt"); // recreates the original file  
	int l = keys["padd_count"];
	int j = 0;
	for (int i = 1; i <= bitString.length() - l; i++)
	{
		//ll = ll + bitString[i];
		if (keys.count(bitString.substr(j, i - j)))
		{
			file_1 << keys[bitString.substr(j, i - j)];
			j = i;
		}

	}
}