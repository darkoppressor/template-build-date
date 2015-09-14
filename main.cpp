/* Copyright (c) 2012 Cheese and Bacon Games, LLC */
/* This file is licensed under the MIT License. */
/* See the file docs/LICENSE.txt for the full license text. */

#include "main.h"
#include "string_stuff.h"

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
    //Can this even happen?
    if(argc<=0){
        print_error("Did not receive the program name");

        return 1;
    }
    else if(argc!=2){
        string program_name=args[0];

        cout<<program_name<<" - refresh the build date for a Cheese Engine project\n";
        cout<<"Usage: "<<program_name<<" PROJECT-DIRECTORY\n";

        return 0;
    }

    string project_directory=args[1];

    if(project_directory.length()==0){
        print_error("The project-directory argument has a length of 0");

        return 1;
    }

    if(boost::algorithm::ends_with(project_directory,"/") || boost::algorithm::ends_with(project_directory,"\\")){
        project_directory.erase(project_directory.begin()+project_directory.length()-1);
    }

    if(!boost::filesystem::is_directory(project_directory)){
        print_error("No such directory: "+project_directory);

        return 1;
    }

    if(!update_version_header(project_directory)){
        return 1;
    }

    return 0;
}

bool update_version_header(string project_directory){
    if(!boost::filesystem::exists(project_directory+"/version.h")){
        print_error("No such file: "+project_directory+"/version.h");

        return false;
    }

    cout<<"Refreshing the build date for the project in directory "<<project_directory<<"\n";

    vector<string> file_data;

    String_Stuff string_stuff;

    ifstream file(project_directory+"/version.h");

    if(file.is_open()){
        while(!file.eof()){
            string line="";

            getline(file,line);

            file_data.push_back(line);
        }
    }
    else{
        print_error("Failed to open version.h for updating (input phase)");

        file.close();
        file.clear();

        return false;
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

    ofstream file_save(project_directory+"/version.h");

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

        file_save.close();
        file_save.clear();

        return false;
    }

    file_save.close();
    file_save.clear();

    return true;
}
