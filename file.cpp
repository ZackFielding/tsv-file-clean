#include <iostream>
#include <fstream>
#include <cstring>
#include <cctype>
#include "file_functions.hpp"

int main(){

	int num_exercises {};
	auto exercise_group = new char[num_exercises][10];

	// create 2d array to be used for -> get_cur_kinematic_file_name()
	create_multi_array(exercise_group, num_exercises);

	bool finished {false};
	char openFileString[50]{'\0'}; // to be file string
	char setChar[3]{}; // set num as type char -> required for string cat

	// outter loop for files (19 samples == 38 files to read & write)	
	for(size_t s {0}; s < 20; ++s){
		if(s == 12)
			continue; // skip this sample

		while(!finished){
			std::fstream openFile, saveFile; // creating fstream objects
				//kinetic file names are identical -> works
			openFile.open("P08_BS_01_pro.tsv", std::ios_base::in); 
				// end goal -> unclear if it will work
			reset_string(openFileString); // clear previous file name
			get_cur_kinematic_file_name(openFileString, setChar, s, exercise_group, 
						num_exercises, int_to_char, finished); //create file string
			std::cout << openFileString << "\n"; // DEBUG ONLY 
			openFile.open(openFileString, std::ios_base::in); //pass file string in open()
			
			// new file to be created
			saveFile.open("P0?_kinematic.tsv", std::ios_base::out);

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
			char string [stringSize] {'\0'}, kinematic_comp_string [20] {"MARKER_NAMES"};
			int loop_counter{0};
			int num_of_tabs {0};

			while(!openFile.eof()){
				get_string(string, stringSize, openFile, reset_string); //read string from file
				//std::clog << "String: " << string << "\n";
				clear_spaces(openFile, false);
				if(std::strcmp(string, kinematic_comp_string) == 0){ //if string matches comp_string
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

			char grf_comp_string[] {"FORCE_PLATE_WIDTH"};

			while(!openFile.eof()){
				get_string(string, stringSize, openFile, reset_string); //read string from file
				clear_spaces(openFile, false);
				if(std::strcmp(string, grf_comp_string) == 0){ //if string matches comp_string
					while(openFile.peek() != '\n')
						openFile.get();
					openFile.get(); // consume new line char

					create_grf_headers(saveFile); // write grf headers to new file

					int tab_counter {0};
					while(!openFile.eof()){
						while(tab_counter <= 2){
							openFile.get(*saveFile.rdbuf(), '\t');
							openFile.get(); // consume tab char
							saveFile.put('\t'); // put tab char
							++tab_counter;
						}
						tab_counter = 0;
						// if the current get 'gets' the last byte of file
						// eof stil == false
						// the following get will the change eof -> true
						while(openFile.peek() != '\n' && !openFile.eof()){
							openFile.get();
						}

						// only put if not at end of file
						if(!openFile.eof())
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
		} // finished (exercise) while loop end
	} // outter sample for loop end

	// free heap of 2d char array
	free_multi_array_heap(exercise_group, num_exercises);

	return 0;
}
