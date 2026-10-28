#include "iostream"
#include "fstream"
using namespace std;

int main(){
	string command;
	string file;
	string data;
	ifstream fin;
	ofstream fout;
	cout << "This programme is for changing data in the game.\n"
	<< "e.g. read <file_name> or write <filename> <data_value>\n"
	<< "close it without typing anything if you don't know what you're doing\n"
	<< "<file_name> can be like \"color/color_seed\" or \"time/fps\" or \"color/rgba/road\".\n";
	while(1){
		data = "";
		cin >> command;
		if(command == "read"){
			cin >> file;
			fin = ifstream("data/"+file+".txt");
			if(!fin){
				cout << "No such file.\n";
				continue;
			}
			getline(fin,data);
			cout << data << "\nDone.\n";
		}
		else if(command == "write"){
			cin >> file;
			fout = ofstream("data/"+file+".txt");
			getline(cin,data);
			bool start = 0;
			for(char character : data){
				if(character==' ' && !start)continue;
				if(!start)start = 1;
				fout << character;
			}
			fout.close();
			cout << "Done.\n";
		}
		else if(command == "quit" || command == "exit" || command == "end"){
			break;
		}
	}
	return 0;
}