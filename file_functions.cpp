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

	std::cout << "Enter number of exercises implemented: ";
	std::cin >> num_exercises;
	std::cin.ignore();
	std::cout << "Enter the exercises to be added (separated by white spaces):"
		<< std::endl;
	int a {0}, b {0};
	while(a < num_exercises && std::cin.peek() != '\n'){
		while(std::isspace(std::cin.peek()) == 0){
			std::cin.get(exercise_group[a][b]);
			++b;
		}
		exercise_group[a][b] = '\0'; // null terminate
		++a;
		b = 0; // reset b for next loop
		if(a < (num_exercises - 1)){
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

void get_cur_kinematic_file_name(char* file_name, char* set_string, 
		int sample_num, char exercise_group[][10], const int& num_exercises,
	   	void(*int_to_char)(char*, int), bool& finished){

	static char* exercise_ptr {exercise_group[0]};
	static int exercise_tracker{0};
	static int set_tracker {49}; // 49 -> '1'
	set_string[0] = (char)set_tracker;
	set_string[1] = '\0';
	file_name[0] = 'P'; // all files start with 'P'
	file_name[1] = '\0'; // null terminante
	char under_score[] {"_"};
	char tail_string[] {"_pro.tsv"};

	int_to_char(set_string, sample_num);
	std::strcat(file_name, set_string);
	std::strcat(file_name, under_score);
	std::strcat(file_name, exercise_ptr);
	std::strcat(file_name, under_score);
	std::strcat(file_name, set_string);
	std::strcat(file_name, under_score);
	std::strcat(file_name, tail_string);
	
	// handle static variables
	++set_tracker;
	if(set_tracker > 51){ // if just finished rep 3
		if(exercise_tracker < num_exercises){ 
			++exercise_ptr; // point to next exercise
			++exercise_tracker; // increment tracker
		}else{
			finished = false; // will stop sample while loop in main()
			exercise_ptr - (num_exercises - 1); //reset pointer to first exercise
		}
		set_tracker = 49; // reset set counter to 1	(not matter what)
	}
}

void int_to_char(char* setChar, const int sample_num){
	// will only work if 0 <= sample_num <= 99
	int remainder{0}, base{0};

	remainder = sample_num % 10; //get second digit
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
