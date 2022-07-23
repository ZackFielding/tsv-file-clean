#include <iostream>
#include <fstream>
#include <cstring>
#include <cctype>
#include "file_functions.hpp"

int main(){

	// main variables
	bool finished {false};
	char openFileString[50]{'\0'}; // to be file string
	char saveFileString[50]{'\0'}; // to be new file name
	char sampleChar[3]{'\0'}; // par # as string
	char setChar[2]{'\0'}; // set # as string
	int num_exercises {};
	 // eventually this will be a file read...
	char leg_array[] {"1222222211112222221"}; //19 as no 12 sample
	std::fstream openFile, saveFile, logFile; // creating fstream objects
	char fileOpenErrorString[] {" inccured an error while attempting to open.\n"};

	std::cout << "Enter number of exercises implemented: ";
	std::cin >> num_exercises;
	std::cin.ignore();
	auto exercise_group = new char[num_exercises][10];

	// create 2d array to be used for -> get_cur_kinematic_file_name()
	create_multi_array(exercise_group, num_exercises);

	// outter loop for files (19 samples == 38 files to read & write)	
	size_t starting_sample {1}, sample_stop {1};
	for(size_t s {starting_sample}; s <= sample_stop; ++s){
		if(s == 12)
			continue; // skip this sample

		while(!finished){
				// reset open & save file strings before updating to current file
			reset_string(openFileString); 
			reset_string(saveFileString);
			// compute current file name to be opened + new file name to be created
			get_cur_kinematic_file_name(openFileString, saveFileString, 
					sampleChar, setChar, s, exercise_group, num_exercises, 
					int_to_char, finished, leg_array, false); 

			openFile.open(openFileString, std::ios_base::in); // open read file
		
			if(!openFile || !saveFile){
				logFile.open("error_log.txt" , 
						std::ios_base::out|std::ios_base::app);
				if(!openFile){
					std::cerr << "Error opening original kinematic file.\n";
					logFile.write(openFileString, std::strlen(openFileString));
				}else{
					std::cerr << "Error opening original to-write-to file.\n";
					logFile.write(saveFileString, std::strlen(saveFileString));
				}
				logFile.write(fileOpenErrorString, std::strlen(fileOpenErrorString));
				logFile.close();
				continue; // skip to start of while(!finished) loop and try next file
			}
				// don't open new file until successful open of read only file		
			saveFile.open(saveFileString, std::ios_base::out); // open new file

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
				// reset open & save file strings
			reset_string(openFileString);
			reset_string(saveFileString);
				// generate GRF file string names
			get_cur_kinematic_file_name(openFileString, saveFileString, 
					sampleChar, setChar, s, exercise_group, num_exercises, 
					int_to_char, finished, leg_array, true); 

			openFile.open(openFileString, std::ios_base::in);
			saveFile.open(saveFileString, std::ios_base::out);

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
