#include <iostream>
#include <boost/filesystem.hpp>
#include <vector>
#include <string>
#include <set>
#include <algorithm>
#include <map>
#include <boost/algorithm/string/classification.hpp> // Include boost::for is_any_of
#include <boost/algorithm/string/split.hpp> // Include for boost::split

using namespace boost::filesystem;
using namespace std;


int makeDir(string dir_name){

    path dir(dir_name);
    if(create_directory(dir)){
        cout<< "Success\n" << endl;
        return 1;
    }
    else{
        cout << "Directory already exists\n" << endl;
        return 0;
    }
}


int files(string str_path_mask){
    string str_path = ".";
    string mask;
    int i = str_path_mask.find("/");
    if(i != -1 || str_path_mask == "."){
        str_path = str_path_mask;
    }
    else{
        mask = str_path_mask;
    }
    try{
       path p(str_path);
       int len_of_path = str_path.length();
       directory_iterator end_itr;

       if(mask.length()==0){
       // cycle through the directory
           for (directory_iterator itr(p); itr != end_itr; ++itr)
           {
               // If it's not a directory
               if (is_regular_file(itr->path())) {
                   // assign current file name to current_file and echo it out to the console.
                   string current_file = itr->path().string();
                   cout << current_file.substr(len_of_path+1,current_file.length()) << endl;
               }
               else{
                   string current_file = itr->path().string();
                   cout << current_file.substr(len_of_path,current_file.length()) << endl;
               }
           }
       }
       else{
           if(mask.find("*") != -1){
               int ind = mask.find("*");
               for (directory_iterator itr(p); itr != end_itr; ++itr){
                   string current_file = itr->path().string().substr(len_of_path+1,current_file.length());
                   //cout<<current_file << endl;
                   //cout << current_file.substr(current_file.length()-(mask.length() - ind)+1,current_file.length()) << endl;
                   //cout << mask.substr(ind+1) << endl;
                   if (current_file.substr(0,ind) == mask.substr(0,ind)
                           && mask.substr(ind+1) ==
                           current_file.substr(current_file.length()-(mask.length() - ind)+1,current_file.length())){
                       cout<<current_file<<endl;
                   }
               }
           }
           else {
               for (directory_iterator itr(p); itr != end_itr; ++itr){
                   string current_file = itr->path().string().substr(len_of_path+1,current_file.length());
                   if( mask == current_file){
                       cout<<current_file<<endl;
                   }
               }

           }
           
       }
               return 1;
    }
    catch (const filesystem_error& e){
        cout << "No such file or directory" << endl;
        return 0;
    }
}

void file_details(string f_name){
    if(exists(f_name)){
        cout << f_name << " size:"<< file_size(f_name)<< " last modified: " << last_write_time(f_name) << endl;
    }
    else{
        cout<< "No such file or directory" << endl;
    }
}

void change_path(string p){
    try{
        if(p.find("/home") != -1){
            current_path(p);
        }
        else{
            string p1 = current_path().string() + "/" + p;
            current_path(p1);
        }
    }
    catch (const filesystem_error& e){
        cout << "No such file or directory" << endl;
    }
}

void cp(string src_path, string dst_path){
    if (exists(dst_path)){
        boost::filesystem::remove(dst_path);
        boost::filesystem::copy_file(src_path, dst_path);
        cout<< "Success\n" << endl;
    }
    else{
            cout << "Error :(" << endl;
    }
}

void cp(vector<string> files, string dir){
    for( auto &i : files){
        boost::filesystem::copy_file(i, dir);
        }
}

void rname(string old, string newn){
    if(exists(old)){
        rename(old,newn);
        cout<<"renamed"<<endl;
      }
    else{
        cout << "file: "<< old << " does not exist" << endl;
    }
}
void rm(const vector<string> rem){
    for(auto& i : rem){
        wpath file(i);
        if(exists(file)){
            remove(file);
            cout << "file: " << i << " removed" << endl;
        }
        else{
            cout << "file: " << i << " does not exist" << endl;
        }
    }
}


int main(){

    map<string, string> help_data;

    help_data["ls -h"] = "Information about files in directory.";
    help_data["ls --help"] = "Information about files in directory.";
    help_data["pwd -h"] = "Information about current path.";
    help_data["pwd --help"] = "Information about current path";
    help_data["cd -h"] = "Go to path.";
    help_data["cd --help"] = "Go to path.";
    help_data["exit -h"] = "Exit from shell.";
    help_data["exit --help"] = "Exit from shell.";
    help_data["cp -h"] = "Copy from one file to another.";
    help_data["cp --help"] = "Copy from one file to another.";
    help_data["mv -h"] = "Rename or remove.";
    help_data["mv --help"] = "Rename or remove.";
    help_data["rm -h"] = "Delete file.";
    help_data["rm --help"] = "Delete file.";
    help_data["mkdir -h"] = "Create new directory.";

    while(1){
        cout << "cat@ ";
        string command;
        getline(cin,command);

        if(help_data.find(command) != help_data.end()){
           cout << help_data.find(command) -> second << endl;
        }
        else if (command == "pwd") {
            cout << current_path() << endl;
        }
        else if (command == "ls"){
            files(".");
        }
        else if(command.find("mkdir") != -1){
            string name  = command.substr(6,command.length());
            makeDir(name);
        }
        else if (command.length() > 2 && command.find("ls") != -1){
            string path  = command.substr(3,command.length());
            files(path);
        }
        else if (command.length() > 5 && command.find("ls -l") != -1){
            string path  = command.substr(6,command.length());
            file_details(path);
        }
        else if (command.length() > 4 && command.find("mkdir") != -1){
            string path  = command.substr(6,command.length());
            makeDir(path);
        }
        else if (command.find("rm") != -1 && command.find("-R") == -1){
            cout << "Can not delete without '-R'" << endl;
        }
        else if (command.find("rm") != -1 && command.find("-f") != -1 && command.find("-R") != -1){
            string path  = command.substr(9,command.length());
            vector<string> words;
            boost::split(words, path, boost::is_any_of(" "), boost::token_compress_on);
            rm(words);
        }
        else if (command.find("rm") != -1 && command.find("-f") == -1 && command.find("-R") != -1){
            cout << "Are you sure? [Y/N]" << endl;
            string answer;
            cin >> answer;
            if (answer == "Y"){
                string path  = command.substr(7,command.length());
                vector<string> words;
                boost::split(words, path, boost::is_any_of(" "), boost::token_compress_on);
                rm(words);
            }
        }
        else if (command.find("mv") != -1){
            string path  = command.substr(3,command.length());
            vector<string> words;
            boost::split(words, path, boost::is_any_of(" "), boost::token_compress_on);
            rname(words[0], words[1]);
        }
        else if(command.find("cd") != -1){
            string path  = command.substr(3,command.length());
            change_path(path);
        }
        else if(command.find("cp") != -1){
            string files = command.substr(3, command.length());
            vector<string> dir;
            boost::split(dir, files, boost::is_any_of(" "), boost::token_compress_on);
            cp(dir[0], dir[1]);
        }
        else if(command == "exit"){
            break;
            return 0;
        }
        else{
            cout << "No such command"<<endl;
        }
    }
    return 0;
}
