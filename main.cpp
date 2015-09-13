/* Copyright (c) 2012 Cheese and Bacon Games, LLC */
/* This file is licensed under the MIT License. */
/* See the file docs/LICENSE.txt for the full license text. */

#include "main.h"
#include "string_stuff.h"
#include "file_io.h"

#include <iostream>
#include <vector>
#include <fstream>

#include <boost/filesystem.hpp>
#include <boost/algorithm/string.hpp>

using namespace std;

void print_error(string error_message){
    cout<<"Error: "<<error_message<<"\n";
}

int main(int argc,char* args[]){
    update_version_header();

    return 0;
}

void update_version_header(){
    vector<string> file_data;

    String_Stuff string_stuff;

    ifstream file("../version.h");

    if(file.is_open()){
        while(!file.eof()){
            string line="";

            getline(file,line);

            file_data.push_back(line);
        }
    }
    else{
        print_error("Failed to open version.h for updating (input phase)");
    }

    file.close();
    file.clear();

    for(int i=0;i<file_data.size();i++){
        if(boost::algorithm::contains(file_data[i],"static const char") && !boost::algorithm::contains(file_data[i],"STATUS")){
            for(int n=0;n<file_data[i].length();n++){
                if(file_data[i][n]=='='){
                    string date_string="";
                    if(boost::algorithm::contains(file_data[i],"YEAR")){
                        date_string="%Y";
                    }
                    else if(boost::algorithm::contains(file_data[i],"MONTH")){
                        date_string="%m";
                    }
                    else{
                        date_string="%d";
                    }

                    char buff[BUFSIZ];
                    time_t now=time(NULL);
                    struct tm* tm_now=localtime(&now);
                    strftime(buff,sizeof buff,date_string.c_str(),tm_now);

                    string component=buff;

                    file_data[i].erase(file_data[i].begin()+n,file_data[i].end());

                    file_data[i]+="=\""+component+"\";";

                    break;
                }
            }
        }
    }

    ofstream file_save("../version.h");

    if(file_save.is_open()){
        for(int i=0;i<file_data.size();i++){
            file_save<<file_data[i];

            if(i<file_data.size()-1){
                file_save<<"\n";
            }
        }
    }
    else{
        print_error("Failed to open version.h for updating (output phase)");
    }

    file_save.close();
    file_save.clear();
}
