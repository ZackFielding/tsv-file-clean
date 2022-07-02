#import <iostream>
#import <fstream>
#import <cstring>
#import <cctype>

bool get_compare_string(char* string, const int& stringSize, 
		char* compString, std::fstream& file){

	size_t index {0};
	while(std::isspace(file.peek() == 0){
		string[index] = file.get();
		++index;	
	}
	string[index] = '\0';

		// need to re-set array prior to next run
	if(std::strcmp(string, compString) != 0)
		return false
	else
		return true
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
		
	//[] return string as char array
	//[] check to see if returned char[] == MARKER_NAMES	
	//[] clear string array prior to next run
	const int stringSize {50};
	char string [stringSize], comp_string [] {"MARKER_NAMES"};

	while(!openFile.eof()){
		if(get_compare_string(string, stringSize, compString, openFile)){
			// if current read string == comparator string -> enter next stage	
		}		
	}


	openFile.close();
	saveFile.close();
	

	return 0;
}
