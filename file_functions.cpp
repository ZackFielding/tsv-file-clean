#include "file_functions.hpp"
#include <iostream>
#include <fstream>
#include <cstring>
#include <cctype>

void get_string(char* string, const int& stringSize, std::fstream& file, 
		void(*reset_string)(char*)){

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

void create_multi_array(char exercise_group[][10], int& num_exercises){

	std::cout << "Enter the exercises to be added (separated by white spaces):"
		<< std::endl;
	int a {0}, b {0};
	while(a < num_exercises){
		while(std::isspace(std::cin.peek()) == 0){
			std::cin.get(exercise_group[a][b]);
			++b;
		}
		exercise_group[a][b] = '\0'; // null terminate
		++a;
		b = 0; // reset b for next loop
		if(a < num_exercises){
			while(std::isspace(std::cin.peek()) != 0)
				std::cin.get(); // consume space
		}
	}
}

void free_multi_array_heap(char exercise_group[][10], const int& num_exercises){
	for(size_t i {0}; i < num_exercises; ++i)
		delete [] exercise_group[i];
	
	delete [] exercise_group;
}

void get_cur_kinematic_file_name(char* open_file_name, char* save_file_name, char* sampleChar, char* setChar, 
		int sample_num, char exercise_group[][10], const int& num_exercises,
	   	void(*int_to_char)(char*, int), bool& finished, char* leg_array, bool grf_bool){

	static int exercise_tracker{0};
	static int sample_tracker{0};
	static int set_tracker {49}; // 49 -> '1'
	setChar[0] = (char)set_tracker;
	setChar[1] = '\0';
	open_file_name[0] = 'P'; // all files start with 'P'
	open_file_name[1] = '\0'; // null terminante
	static char under_score[] {"_"};
	static char tail_string_kinematic[] {"_pro.tsv"};
	static char tail_string_grf[] {"_pro_f_?.tsv"};
	static char tail_save_kinematic[] {"_kinematic.tsv"};
	static char tail_save_grf[] {"_grf.tsv"};

	int_to_char(sampleChar, sample_num); // pass in -> char [] & current par #
	std::strcat(open_file_name, sampleChar); // append sample char with file name == "P01"
	std::strcat(open_file_name, under_score); // append '_' == "P01_"
	std::strcat(open_file_name, exercise_group[exercise_tracker]); // append exercise string == "P01_BS"
	std::strcat(open_file_name, under_score); // '_' == "P01_BS_"
	std::strcat(open_file_name, setChar); // == "P01_BS_1"
	std::strcpy(save_file_name, open_file_name); // "P01_BS_1"

	if(grf_bool){
		std::strcat(save_file_name, tail_save_grf); // "P01_BS_1_grf.tsv"
		std::strcat(open_file_name, tail_string_grf); // == "P01_BS_1_pro_f_?.tsv"
		open_file_name[15] = leg_array[sample_tracker]; // == "P01_BS_1_pro_f_1/2.tsv"
	}else{
		std::strcat(save_file_name, tail_save_kinematic); // "P01_BS_1_kinematic.tsv
		std::strcat(open_file_name, tail_string_kinematic); // == "P01_BS_1_pro.tsv"
	}
	
	// handle static variables
	++set_tracker;
	if(set_tracker > 51 && exercise_tracker < num_exercises){
		++exercise_tracker;
		set_tracker = 49;
	}else if(set_tracker > 51 && exercise_tracker >= num_exercises){
		exercise_tracker = 0;
		finished = true;
		set_tracker = 49;
		++sample_tracker;
	}
}

void int_to_char(char* setChar, const int sample_num){

	// will only work if 0 <= sample_num <= 99
	int remainder{sample_num % 10},  base{0};

	if(sample_num >=10 && sample_num <100){
		base = sample_num / 10;
	}else if(sample_num < 10){
		base = 0;
	}else{
		std::cout << "Sample num is >100 - file name is incorrect\n";
	}
	
	setChar[1] = (char)(remainder + 48);
	setChar[0] = (char)(base + 48);
	setChar[2] = '\0';
}
