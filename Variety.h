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

#include "Structures.h"

// whole_tree is the tree of everypossible neighborhood in the Hypergraph.
// This is a type memoization.
// u and v are two vertices such that u != v
// If zero is returned the Hypergraphs is non-Leibnizian
int relative_indifference(Tree &whole_tree, int u, int v){
    Tree tu = whole_tree.neighborhood_of_vertex(u);
    Tree tv = whole_tree.neighborhood_of_vertex(v);
    
    int ud = tu.depth();
    int vd = tv.depth();
    int d = min(ud, vd);
    
    Hypergraph hg1, hg2;

    // Let us create the threads for OpenMP here. 7 Jan 23, 20:00
    // NUM_THREADS is defined in Structures.h
    //omp_set_num_threads(NUM_THREADS);

    // Since all vertices are created equal, we begin with depth = 1
    for (int i = 1; i <= d; i++){
        hg1 = tu.neighborhood_down_to_depth(i);
        hg2 = tv.neighborhood_down_to_depth(i);
        if (!hg1.is_isomorph_to(hg2))
            return i;
    }
    
    return 0;
}

// vs is a list of vertices of the Hypergraph
// whole_tree is a pointer
int absolute_indifference(Tree &whole_tree, vector<int> unique_vertices, int u){
    int ri = 0;
    int ri_pre = 0;
        
    for (int v : unique_vertices)
      if (v != u){
	ri_pre = relative_indifference(whole_tree, u, v);
	if (ri_pre == 0) // Meaning that the Hypergraph is non-Leibnizian
	  return 0;
	
	ri = max(ri, ri_pre);
      }
    
    return ri;
}

// Convert this function to return a rational number
double variety(Hypergraph hg){
    Tree whole_tree(hg);
    
    vector<int> unique_elements = hg.unique_vertices().get_vertices();
    int s = unique_elements.size();
    
    double var = 0;
    double ai;
    
    for (int i : unique_elements){
        ai = absolute_indifference(whole_tree, unique_elements, i);
	cout << "vertex: " << i << ", ai: " << ai << endl;
        if (ai == 0){ // Meaning that the Hypergraph is non-Leibnizian
	  return 0;
	}
        
        var += 1/ai;
    }
    
    return var;
}

int variety_omp_print(string str){

    Hypergraph hg(str);    
    Tree whole_tree(hg);
    
    vector<int> unique_elements = hg.unique_vertices().get_vertices();
    int s = unique_elements.size();

    vector<int> ai;
    int ai_tmp;
  
    #pragma omp parallel for
    for (int i = 0; i < s; i++){
      ai_tmp = absolute_indifference(whole_tree, unique_elements, unique_elements[i]);
      ai.push_back(ai_tmp);
    }

    vec_print(ai);


  return 0;
}

string variety_omp_str(string str){

    Hypergraph hg(str);
    Tree whole_tree(hg);
    
    vector<int> unique_elements = hg.unique_vertices().get_vertices();
    int s = unique_elements.size();

    vector<int> ai;
    int ai_tmp;
  
    #pragma omp parallel for
    for (int i = 0; i < s; i++){
      ai_tmp = absolute_indifference(whole_tree, unique_elements, unique_elements[i]);
      ai.push_back(ai_tmp);
    }

    return vec_str(ai);
}
