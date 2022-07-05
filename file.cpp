#import <iostream>
#import <fstream>
#import <cstring>
#import <cctype>

//[x] create custom string cat func to deal with xyz + \t concat
//[] debug
//[] test multi_cat_string()

typedef void(*FnPtr_get_string)(char*, const int&, std::fstream&);
typedef void(*FnPtr_reset_string)(char*);
typedef void(*FnPtr_multi_string_cat)(char**, const int&);

void get_string(char* string, const int& stringSize, std::fstream& file, 
		FnPtr_reset_string){

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
		FnPtr_get_string, FnPtr_reset_string, FnPtr_multi_string_cat){ 

	char xyz[]{"xyz"};
	const int stringArraySize{3};
	char* stringArray[stringArraySize]; //array of points to char 
	while(openFile.peek() != '\n'){
		get_string(string, stringSize, openFile); //returns current header string
		stringArray[0] = string; //assing current header to char array
		stringArray[2] = '\t'; // assign tab char -> last index
		for(size_t w {0}; w < 3; ++w){
			stringArray[1] = xyz[w]; //assign x/y/z char to middle char array index
			multi_string_cat(stringArray, stringArraySize); //pass entire char array to cat all string
		   	saveFile << string; //pass cat string
		}
	}
}

void multi_string_cat(char** stringArray, const int& stringArraySize){
	
		//concatenates string to 0-INDEXED string in char**
		//does not return new string -> is appending to a currently existing string
	size_t i {1}, j {0};
	for(i ; i < stringArraySize; ++i){ //looping throuhg ptr->ptrs
		while(stringArray[i][j] != '\0'){ //looping through indiviual strings
			stringArray[0][j] = stringArray[i][j];	
			++j;
		}
	}

	stringArray[i-1][j] = '\0'; //append null terminator
	return;
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
					get_string, reset_string, multi_cat_string);
		}		
	}

	openFile.close();
	saveFile.close();

	return 0;
}
