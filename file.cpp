#import <iostream>
#import <fstream>
#import <cstring>
#import <cctype>

void get_string(char* string, const int& stringSize, std::fstream& file, 
		void(*reset_string)(char*)){

	std::cout << "get string entered." << std::endl;
	reset_string(string);
	size_t index {0};
	while(!std::isspace(file.peek())){
		string[index] = file.get();
		++index;	
	}

	string[index] = '\0';
	std::cout << string << "\n"; //DEBUGGING


	return; 
}

void clear_spaces(std::fstream& file, bool headerRowFound){
	
	if(!headerRowFound){
		do{
			file.get();
		}while(std::isspace(file.peek()));	
	}else{
		while(std::isspace(file.peek()) && file.peek() != '\n')
			file.get();
	}
}

void reset_string(char* string){
	
	std::cout << "reset string entered.\n";
	for(size_t j {0}; string[j] != '\0'; ++j){
		string[j] = '\0';
	}
	return;
}

void create_new_header(char* string, int stringSize, 
		std::fstream& writeFile){
	
	char xyz_tab[] {"xyz"};
	for(size_t a {0}; a < 3; ++a){
		string[stringSize] = xyz_tab[a]; //append x OR y OR z
		string[stringSize+1] = '\t'; //append tab char (.tsv file)
		writeFile << string; // write to current position in new file
	}	
}

int string_size(char* string){

	size_t i {0};
	int stringSize{0};
	while(string[i] != '\0'){
		++stringSize;	
		++i;
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
	int loop_counter{0};

	while(!openFile.eof()){
		get_string(string, stringSize, openFile, reset_string); //read string from file
		std::clog << "String: " << string << "\n";
		clear_spaces(openFile, false);
		if(std::strcmp(string, comp_string) == 0){ //if string matches comp_string
			while(openFile.peek() != '\n'){
				std::clog << "inner while loop.\n";
				get_string(string, stringSize, openFile, reset_string); // read string
				clear_spaces(openFile, true);
				create_new_header(string, string_size(string), saveFile); 
			}	
			break;
		}
	}

	openFile.close();
	saveFile.close();
	
	if(!openFile.is_open())
		std::clog << "Read only file closed.\n";
	if(!saveFile.is_open())
		std::clog << "Write only file closed.\n";

	return 0;
}
