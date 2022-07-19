#import <iostream>
#import <fstream>
#import <cstring>
#import <cctype>

void get_string(char* string, const int& stringSize, std::fstream& file, 
		void(*reset_string)(char*)){

	//std::cout << "get string entered." << std::endl;
	reset_string(string);
	size_t index {0};
	while(!std::isspace(file.peek())){
		string[index] = file.get();
		++index;	
	}

	string[index] = '\0';
	//std::cout << string << "\n"; //DEBUGGING
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
	
	//std::cout << "reset string entered.\n";
	for(size_t j {0}; string[j] != '\0'; ++j){
		string[j] = '\0';
	}
	return;
}

void create_new_header(char* string, int stringSize, 
		std::fstream& writeFile, int& num_of_tabs){
	
	char xyz_tab[] {"xyz"};
	for(size_t a {0}; a < 3; ++a){
		string[stringSize] = xyz_tab[a]; //append x OR y OR z
		string[stringSize+1] = '\t'; //append tab char (.tsv file)
		writeFile.write(string, sizeof string);
		++num_of_tabs;
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

void create_grf_headers(std::fstream& saveFile){

	char grf_ar[]{"grf n"}; // 4
	char xyz[]{"xyz"};
	for(size_t i {0}; i < 3; ++i){
		grf_ar[4] = xyz[i];
		saveFile.write(grf_ar, sizeof grf_ar);
		if(i < 2)
			saveFile.put('\t');
	}
	saveFile.put('\n');
}

int main(){
	
	std::fstream openFile, saveFile; // creating fstream objects
	openFile.open("P08_BS_01_pro.tsv", std::ios_base::in);
	saveFile.open("P08_kinematic.tsv", std::ios_base::out);

	if(!openFile || !saveFile){
		if(!openFile){
			std::cerr << "Error opening original kinematic file.\n";
			if(saveFile)
				saveFile.close();
			return 1;
		}else{
			std::cerr << "Error opening original to-write-to file.\n";
			if(openFile)
				openFile.close();
			return 1;
		}
	}
			
	const int stringSize {50};
	char string [stringSize] {'\0'}, comp_string [20] {"MARKER_NAMES"};
	int loop_counter{0};
	int num_of_tabs {0};

	while(!openFile.eof()){
		get_string(string, stringSize, openFile, reset_string); //read string from file
		//std::clog << "String: " << string << "\n";
		clear_spaces(openFile, false);
		if(std::strcmp(string, comp_string) == 0){ //if string matches comp_string
			while(openFile.peek() != '\n'){
				//std::clog << "inner while loop.\n";
				get_string(string, stringSize, openFile, reset_string); // read string
				clear_spaces(openFile, true);
				create_new_header(string, string_size(string), saveFile, num_of_tabs); 
			}	

			std::clog << "Header created.\n";
			saveFile.put(openFile.get()); // put new line char -> new file

			// consume all contents on this row (unnecessary text)
			while(openFile.peek() != '\n')
				openFile.get();
			openFile.get(); // consume new line char

			while(!openFile.eof()){
				openFile.get(*saveFile.rdbuf(), '\t');
				openFile.get(); // consume tab char
				saveFile.put('\t'); // put tab char
			}
			std::clog << "End of file reached.\n";
			}
		}

	openFile.close();
	saveFile.close();
	
	if(!openFile.is_open())
		std::clog << "Read only kinematic file closed.\n";
	if(!saveFile.is_open())
		std::clog << "Write only kinematic file closed.\n";

	// similar approach for ground reaction force files
	
	openFile.open("P08_BS_01_pro_f_1.tsv", std::ios_base::in);
	saveFile.open("P08_grf_south.tsv", std::ios_base::out);

	if(!openFile || !saveFile){
		if(!openFile){
			std::cerr << "Error opening original GRF file.\n";
			if(saveFile)
				saveFile.close();
			return 1;
		}else{
			std::cerr << "Error opening original GRF file.\n";
			if(openFile)
				openFile.close();
			return 1;
		}
	}

	reset_string(comp_string); // reset comp string
	comp_string = "FORCE_PLATE_WIDTH";

	while(!openFile.eof()){
		get_string(string, stringSize, openFile, reset_string); //read string from file
		clear_spaces(openFile, false);
		if(std::strcmp(string, comp_string) == 0){ //if string matches comp_string
			while(openFile.peek() != '\n')
				openFile.get();
			openFile.get(); // consume new line char

			create_grf_header(saveFile); // write grf headers to new file

			int tab_counter {0};
			while(!openFile.eof()){
				while(tab_counter <= 3){
					openFile.get(*saveFile.rdbuf(), '\t');
					openFile.get(); // consume tab char
					saveFile.put('\t'); // put tab char
					++tab_counter;
				}
				while(openFile.peek() != '\n')
					openFile.get();
				saveFile.put(openFile.get()); // put new line char -> new file
			}
			std::clog << "End of file reached.\n";
			}
		}

	openFile.close();
	saveFile.close();
	
	if(!openFile.is_open())
		std::clog << "Read only GRF file closed.\n";
	if(!saveFile.is_open())
		std::clog << "Write only GRF file closed.\n";

	return 0;
}
