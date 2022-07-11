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

bool row_position_test(){

	/*
	 *Function purpose -> to determine if length of rows are the same
	 * .tellg() returns fpos obj of type pos_type
	 * thus need to determine if each row are the same total bytes in size
	 * .seekg() takes int value but needs to correspond to bytes (char ==1, int ==4, ...)
	 * Steps:
	 * 	1. loop through first ~3 rows and determine their length
	 * 	2. compare lengths from each row -> return true if equal
	 * 		-> return false if not equal
	 * */


	std::fpos<int> filePos;
	std::streamoff filePosInc {}; // long long int (can convert fpos -> streamoff)
	filePos = file.tellg(); // returns current file position
	int holdPeek {0};

	// BLOCK NEEDS LOGIC CHECK && 'DOES IT WORK?' CHECK
	for(size_t t {0}; t < 3; ++t){
		size_t inner {0};
		while(file.peek() != '\n'){
			filePos = filePos + filePosInc; //++ current file pos
		}

		holdPeek = (int)file.peek(); //convert to int
		while(holdPeek < 48 || holdPeek > 57){ //if current char is NOT a bnum
			filePos = filePos + filePosInc; //++ current file pos
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
	int loop_counter{0};
	std::stream

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
			//break;
			openFile.get(); // consume new line char
			// ready for row_position_test()
			if(row_position_test()){
				// do something ...	
			}
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
