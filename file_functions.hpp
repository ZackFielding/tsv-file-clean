#ifndef FILE_FUNCTIONS_HPP // if file_functions.hpp exists
#define FILE_FUNCTIONS_HPP // define all the code between here -> endif
#include <fstream>

void get_string(char*, const int&, std::fstream&, void(*)(char*));

void clear_spaces(std::fstream&, bool);

void reset_string(char*);

void create_new_header(char*, int, std::fstream&, int&);

int string_size(char*);

void create_grf_headers(std::fstream&);

void create_multi_array(char exercise_group[][10], int&);

void free_multi_array_heap(char exercise_group[][10], const int&);

void get_cur_kinematic_file_name(char*, char*, char*, char*,
	    int, char exercise_group[][10], 
		const int&, void(*int_to_char)(char*, int), bool&, char*, bool);

void int_to_char(char*, const int);

#endif
