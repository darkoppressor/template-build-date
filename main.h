/* Copyright (c) 2012 Cheese and Bacon Games, LLC */
/* This file is licensed under the MIT License. */
/* See the file docs/LICENSE.txt for the full license text. */

#ifndef main_h
#define main_h

#include <string>

void print_error(std::string error_message);

int main(int argc,char* args[]);

bool update_version_header(std::string project_directory);

#endif
