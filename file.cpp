#import <iostream>
#import <fstream>
#import <cstring>
#import <cctype>

//[] create custom string cat func to deal with xyz + \t concat

void get_string(char* string, const int& stringSize, std::fstream& file, 
		void*(reset_string)(char*)){

	reset_string(string); 
	size_t index {0};
	while(std::isspace(file.peek() == 0)){
		string[index] = file.get();
		++index;	
	}
	string[index] = '\0';
	return; 
}

void reset_string(char* string){
	//reset string for future use
	size_t i {0};
	while(string[i] != '\0')
		string[i] = '\0';
	return;
}

void extract_modify_string(char* string, const int& stringSize, 
		std::fstream& openFile, std::fstream &saveFile, 
		void*(get_string)(char*, const int&, std::fstream&)){ //*func missing *reset_string

	char xyz[]{"xyz"};
	while(openFile.peek() != '\n'){
		get_string(string, stringSize, openFile);	
		for(size_t w {0}; w < 3; ++w){
			string = std::strcat(string,xyz[w]);
			string = std::strcat(string, '\t');
		   	saveFile << string; 	
		}
	}
}



int main(){
	
	std::fstream openFile; // original file
	openFile.open("P08_BS_01_pro.tsv", std::ios_base::in);
	
	std::fstream saveFile; // new (cleaned) file
	saveFile.open("P08_kinematic.tsv", std::ios_base::out);

	if(!openFile || !saveFile){
		if(!openFile)
			std::cerr << "Error opening original file.\n";
		else
			std::cerr << "Error opening to-write-to file.\n";
		return 1;
	}
		
	const int stringSize {50};
	char string [stringSize] {'\0'}, comp_string [] {"MARKER_NAMES"};

	while(!openFile.eof()){
		get_string(string, stringSize, openFile, reset_string); //read string from file
		if(std::strcmp(string, comp_string) == 0) //if string matches comp_string
			extract_modify_string(string, stringSize, openFile, saveFile,
					get_string);
		}		
	}

	openFile.close();
	saveFile.close();

	return 0;
}
