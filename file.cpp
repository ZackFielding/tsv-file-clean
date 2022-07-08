#import <iostream>
#import <fstream>
#import <cstring>
#import <cctype>

typedef void(*FnPtr_reset_string)(char*);

void get_string(char* string, const int& stringSize, std::fstream& file, 
		FnPtr_reset_string){

	*(FnPtr_reset_string)(string);
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

void create_new_header(char* string, int stringSize){
	
	char xyz_tab[] {"xyz\t"};

	size_t i {0};
	for(i; i < 4; ++i){
		string[stringSize] = xyz_tab[i];
		++stringSize;	
	}
	
	string[stringSize] = '\0';	

	return;
}

int string_size(char* string){

	size_t i {0};
	int stringSize{0};
	while(string[i] != '\0'){
		++stringSize;	
	}
	return stringSize;
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
		if(std::strcmp(string, comp_string) == 0){ //if string matches comp_string
			while(openFile.peek() != '\n'){
				get_string(string, stringSize, openFile, reset_string); // read string
				for(size_t i {0}; i < 3; ++i){
					create_new_header(string, string_size(string)); 
					saveFile << string; // write to new file
				}
			}	
		}		
	}

	openFile.close();
	saveFile.close();

	return 0;
}
