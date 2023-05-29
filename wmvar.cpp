/*
 # LICENSE
 This program is free software: you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by
 the Free Software Foundation, either version 3 of the License, or
 (at your option) any later version.

 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.

 You should have received a copy of the GNU General Public License
 along with this program.  If not, see <https://www.gnu.org/licenses/>.

 Copyright 2023, Furkan Semih DÜNDAR
 Email: f.semih.dundar@yandex.com
*/

#include "omp.h"
#include <iostream>
#include <fstream>
#include "Variety.h"

using namespace std;

int main(int argc, char ** argv){

  if (argc < 2){
    cout << "Hypergraph is not specified!" << endl;
    return 0;
  }
  
    string argv1 = argv[1];

    vector<string> hg_str;

    string file_name;
    
    if (argv1 == "-f"){
      string line;
      file_name = argv[2];
      ifstream file(file_name);
      
      while(getline(file,line))
	hg_str.push_back(line); //We add HG strings to the vector.
    }
    else{
      hg_str.push_back(argv1); //We suppose that if argv != "-f" then it is an HG string
    }

    //Here we print the list of ais for each HG.
    //The lists will then be processed by another (Mathematica) program.
    //This gives flexibility about which variety function to use.
    //The canonical choice is sum_i 1/ai;

    string output_file_name = file_name + "_hg_and_ais.txt";
    ofstream output_file(output_file_name);
    
    omp_set_num_threads(8);

    for(string str : hg_str){
        output_file << str << ";";
        output_file << variety_omp_str(str) << endl;
    }
    
    output_file.close();
    
    return 0;
}
