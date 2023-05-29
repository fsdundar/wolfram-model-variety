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

#include <map>
#include <algorithm>
#include <string>
#include <vector>

using namespace std;



// Keeps a list of (frequency, {vertices})
typedef class map<int, vector<int> > FrequencyDict;

// Keeps a list of two integers. It is understood that the first will be mapped to the second
// while considering Hypergraph isomorphisms.
typedef class map<int, int> Rule;

Rule create_rule_from_vectors(vector<int> a, vector<int> b){
    int s1 = a.size();
    int s2 = b.size();
    Rule r;
    
    // If the sizes do not match, return an empty Rule
    if (s1 != s2)
        return r;
    
    for (int i = 0; i < s1; i++)
        r.insert(Rule::value_type(a[i],b[i]));
    
    return r;
}

// Adds two rules and returns the total rule.
Rule add_two_rules(Rule r1, Rule r2){
    Rule r = r1;
    
    for (auto i : r2)
        r.insert(i);
    
    return r;
}

void rule_print(Rule r){
    cout << "Rule Print" << endl;
    
    for (auto i : r)
        cout << i.first << " -> " << i.second << endl;
}

// Returns true if f1 and f2 have same number of frequency lists,
// and the number of elements for each frequency are the same.
bool is_of_same_shape(FrequencyDict f1, FrequencyDict f2){
    int s1 = f1.size();
    int s2 = f2.size();
    
    if (s1 != s2)
        return false;
    
    // Let us check if both f1 and f2 has the same set of keys.
    for (auto k: f1)
        if (f2.count(k.first) == 0)
            return false;
    
    for (auto k : f1)
        if (k.second.size() != f2.at(k.first).size())
            return false;
    
    return true;
}

vector<vector <int> > tensor_product(vector<vector <int> > a, vector<vector <int> > b){
    vector<vector <int> > p;
    vector<int> c;
    
    if (b.size() == 0)
        return a;
    
    for(auto i : a)
        for (auto j : b){
            c.empty();
            c = i;
            c.insert(c.end(), j.begin(), j.end());
            p.push_back(c);
        }
    
    return p;
}

vector<vector <int> > tensor_product_of_all(vector<vector <vector <int> > > a){
    int s = a.size();
    
    vector<vector <int> > p;
    
    if (s == 0)
        return p;
    if (s == 1)
        return a[0];
    
    p = tensor_product(a[s-2], a[s-1]);
    
    // We multiplyy from the end.
    for (int i = s-3; i >= 0; i--){
        p = tensor_product(a[i], p);
    }
    
    return p;
    
}

vector<vector <int> > permutations_of_vec(vector<int> v){
    vector< vector<int> > p;
    
    do{
        p.push_back(v);
    }while(next_permutation(v.begin(), v.end()));
    
    return p;
}

// We suppose that the "key" in f exists.
vector<vector <int> > all_permutations_of_dict_with_key(FrequencyDict f, int key){
    return permutations_of_vec(f.at(key));
}

// Combines rs1 and rs2 in every possible way.
vector<Rule> tensor_product_of_rules(vector<Rule> rs1, vector<Rule> rs2){
    vector<Rule> rs;
    
    if (rs2.size() == 0)
        return rs1;

    if (rs1.size() == 0)
        return rs2;
    
    for(auto r1 : rs1)
        for (auto r2 : rs2)
            rs.push_back(add_two_rules(r1,r2));
    
    return rs;
}

// We suppose that the shape of f1 and f2 are the same. This function will be called
// by Hypergraph Isomorphism function which performs such checks. So f1 and f2 have the
// same set of "keys"
vector <Rule> all_permutations_of_two_dict_with_key(FrequencyDict f1, FrequencyDict f2, int key){
    
    vector<vector <int> > vs = all_permutations_of_dict_with_key(f2, key);
    vector<int> v1 = f1.at(key);
    
    vector <Rule> rs;
    
    for (auto v2 : vs)
        rs.push_back(create_rule_from_vectors(v1,v2));
    
    return rs;
}

// Returns the list of keys of f.
vector<int> keys_of_freq_dict(FrequencyDict f){
    vector<int> keys;
    
    for (auto i : f)
        keys.push_back(i.first);
    
    return keys;
}

// The function "all_permutations_of_two_dict_with_key" is what we will use.
// We need to iterate over all keys, and combine rules.
vector <Rule> all_permutations_of_two_dicts(FrequencyDict f1, FrequencyDict f2){
    vector <Rule> all_rules;
    vector<int> keys = keys_of_freq_dict(f1);
    vector<vector <Rule> > vr;
    
    // Now we reverse the order of keys, that is least frequenct vertices are looped over at first.
    sort(keys.begin(), keys.end());
    reverse(keys.begin(), keys.end());
    
    for (int k : keys)
        vr.push_back(all_permutations_of_two_dict_with_key(f1, f2, k));
    
    // Now vs holds all the information about Rules. We need to perform a tensor product.
    int s = vr.size();
    
    if (s <= 1)
        return vr[0];
    
    // Now s >= 2
    all_rules = tensor_product_of_rules(vr[s-2], vr[s-1]);
    
    if (s == 2)
        return all_rules;
    
    // Now s >= 3
    for (int i = s-3; i >= 0; i--)
        all_rules = tensor_product_of_rules(vr[i], all_rules);
    
    return all_rules;
}

void vec_print(vector<int> v){
  int s = v.size();

  cout << "{";
  for (int i = 0; i < s; i++){
        cout << v[i];
	if (i < s-1)
	  cout << ",";
  }
    cout << "}" << endl;
}

string vec_str(vector<int> v){
  int s = v.size();

  string str = "";
    
  str += "{";
  for (int i = 0; i < s; i++){
        str += to_string(v[i]);
    if (i < s-1)
      str += ",";
  }
    str += "}";
    
    return str;
}

void freq_dict_print(FrequencyDict f){
    for (auto i : f){
        cout << i.first << ": ";
        vec_print(i.second);
        cout << endl;
    }
}

int max_vec_int(const vector<int> v){
    int m = v[0];
    int s = v.size();
    for (int i = 0; i < s; i++)
        if (m < v[i])
            m = v[i];
    return m;
}


// DEFINITIONS OF CLASSES AND RELATED FUNCTIONS
// 1. Hyperedge
// 2. Hypergraph
// 3. Tree

class Hyperedge{
    
private:
    vector<int> vertices;
    
public:
    // vs stands for vertices.
    Hyperedge(vector<int> vs){
        this->vertices = vs;
    };
    
    Hyperedge(string str){
        int s = str.length();
        int pos_pre = 1;
        
        if ((str[0] == '{') && (str[s-1] == '}')){
            for (int pos = 1; pos < s-1; pos++)
                if ((str[pos] == ',') || (pos == s-2)){
                    vertices.push_back(stoi(str.substr(pos_pre, pos-pos_pre+1)));
                    pos_pre = pos + 1;
                }
        }
            
    };
    
    // empty constructor
    Hyperedge(){
    };
    
    // Get vertex at position i.
    int get(int i){
        return vertices[i];
    };
    
    vector<int> get_vertices(){
        return vertices;
    };
    
    // Remove the vertices.
    void clear(){
        vertices.clear();
    };
    
    void append(int v){
        this->vertices.push_back(v);
    };
    
    void append(Hyperedge& he){
        int s = he.size();
        for (int i = 0; i < s; i++)
            this->append(he.get(i));
    };
    
    // Returns true if u is an element of Hyperedge, false otherwise.
    bool is_elem(int u){
        for (int v : vertices)
            if (u == v)
                return true;
        return false;
    };
    
    // Returns how many times the vertex `v` appears in the Hypergraph
    int frequency_of_vertex(int v){
        int c = 0;
        for (int u : vertices)
            if (u == v)
                c++;
        
        return c;
    };
    
    int size(){
        return vertices.size();
    };
    
    // Returns a sorted list of unique elements of Hyperedge.
    Hyperedge unique_vertices(){
        vector<int> he = this->vertices;
        
        // If there is at most one element, return he.
        int s = he.size();
        if (s <= 1)
            return he;
        
        // We sort the vector.
        sort(he.begin(), he.end());
        
        // Remove the duplicate elements
        for(int i = 1; i < s; i++){
            if (he.at(i) == he.at(i-1)){
                he.erase(he.begin() + i);
                i -= 1;
                s -= 1;
            }
        }
        return he;
    };
    
    // Returns true if there is nonzero intersection, false otherwise.
    bool does_intersect_with(Hyperedge& he){
        for (int u : vertices)
            if (he.is_elem(u))
                return true;
        
        return false;
    };
    
    Hyperedge map_via_rule(Rule r){
        Hyperedge he;
        
        for (int i : this->vertices)
            he.append(r.at(i));
        
        return he;
    };
    
    // Apply rule to "this" Hyperedge and check equality with he2.
    bool is_isomorph_to_via_rule(Rule r, Hyperedge he2){
        return this->map_via_rule(r) == he2;
    };
    
    friend bool operator==(const Hyperedge& he1, const Hyperedge& he2);
    friend bool operator!=(const Hyperedge& he1, const Hyperedge& he2);
    
    // If newline = false, endl is not printed at the end.
    void print(bool newline = true){
        int s = vertices.size();
        
        cout << "{";
        
        for (int i = 0; i < s; i++){
            cout << vertices[i];
            if (i < s-1)
                cout << ",";
        }
        
        cout << "}";
        if (newline)
            cout << endl;
    };
};

bool operator==(const Hyperedge& he1, const Hyperedge& he2){
    return he1.vertices == he2.vertices;
}

bool operator!=(const Hyperedge& he1, const Hyperedge& he2){
    return he1.vertices != he2.vertices;
}

class Hypergraph{
    
private:
    vector<Hyperedge> hg;
    
public:
    // Default value for Hypergraph is an empty list.
    Hypergraph(vector<Hyperedge> hes){
        this->hg = hes;
    };
    
    Hypergraph(string str){
        int s = str.length();
        int pos_pre, pos;
        
        if ((str[0] == '{') && (str[s-1] == '}')){
            for (int pos_pre = 1; pos_pre < s-1; pos_pre++){
                if (str[pos_pre] == '{'){
                    pos = pos_pre;
                    do{
                        pos++;
                    }while(str[pos] != '}');
                    this->hg.push_back(Hyperedge(str.substr(pos_pre, pos - pos_pre + 1)));
                    pos_pre = pos;
                }
            }
        }
        
    };
    
    // empty constructor
    Hypergraph(){
    };
    
    void append(Hyperedge he){
        this->hg.push_back(he);
    };
    
    Hyperedge unique_vertices(){
        int s = this->size();
        
        Hyperedge he;
        
        if (s == 0)
            return he;
        
        he = hg[0];
        
        if (s >= 1)
            for(int i = 1; i < s; i++)
                he.append(hg[i]);
        
        he = he.unique_vertices();
        
        return he;
    };
    
    // Returns how many times the vertex `v` appears in the Hypergraph
    int frequency_of_vertex(int v){
        int c = 0;
        for (Hyperedge he: hg)
            c += he.frequency_of_vertex(v);
        
        return c;
    };
    
    FrequencyDict frequency_of_vertices(){
        Hyperedge he = this->unique_vertices();
        int s = he.size();
        FrequencyDict f;
        int u, freq;
        vector<int> vs;
        
        for (int i = 0; i < s; i++){
            u = he.get(i);
            freq = this->frequency_of_vertex(u);
            
            vs.empty();
            
            if (f.count(freq) > 0){
                f.at(freq).push_back(u);
            }
            else{
                f.insert(FrequencyDict::value_type(freq,vs));
                f.at(freq).push_back(u);
            }
            
        }
        
        return f;
    };
    
    // Returns true if he is an elem of this->hg.
    bool does_include(Hyperedge he2){
        
        for (Hyperedge he : hg)
            if (he == he2)
                return true;
        
        return false;
    };
    
    Hypergraph remove_hyperedge_once(Hyperedge he){
        int s = hg.size();
        int count = 0;
        vector<Hyperedge> hg2;
        
        for (int i = 0; i < s; i++){
            if (he != hg[i]){
                hg2.push_back(hg[i]);
            }
            else{
                if (count > 0)
                    hg2.push_back(hg[i]);
                else
                    count++;
            }
            
        }
        
        return Hypergraph(hg2);
    };
    
    // New implementation: 22 December 2022, 15:43.
    Hypergraph remove_hyperedge(Hyperedge he){
        int s = hg.size();
        bool is_he_elem_of = false;
        vector<Hyperedge> hg2;
        
        for (int i = 0; i < s; i++)
            if (hg[i] != he)
                hg2.push_back(hg[i]);
        
        return Hypergraph(hg2);
    };
    
    int size(){
        return hg.size();
    };
    
    // Returns the list of length of Hyperedges in non-decreasing order.
    vector<int> size_nub(){
        vector<int> list;
        const int s = this->size();
        
        // The length of each Hyperedge is added
        for (int i = 0; i < s; i++)
            list.push_back(hg[i].size());
        
        // We sort the list
        sort(list.begin(), list.end());
        
        return list;
    };
    
    // Get the Hyperedge that is located at index i.
    Hyperedge get(int i){
        return hg[i];
    };
    
    void union_with(Hypergraph hg2){
        int s = hg2.size();
        
        if (s > 0)
            for (int i = 0; i < s; i++)
                this->append(hg2.get(i));
    };
    
    Hypergraph map_via_rule(Rule r){
        Hypergraph hg2;
        
        for (Hyperedge he : this->hg)
            hg2.append(he.map_via_rule(r));
        
        return hg2;
    };
    
    friend bool operator==(const Hypergraph& hg1, const Hypergraph& hg2);
    friend bool operator!=(const Hypergraph& hg1, const Hypergraph& hg2);
    
    Hypergraph neighborhood_of_vertex(int v){
        Hypergraph hg2(this->hg);
        
        int s = hg.size();
        
        for(int i = 0; i < s; i++){
            if (!hg2.hg.at(i).is_elem(v)){
                hg2.hg.erase(hg2.hg.begin() + i);
                i -= 1;
                s -= 1;
            }
        }
        
        return hg2;
    };
    
    Hypergraph neighborhood_of_hyperedge(Hyperedge he){
        Hypergraph hg2(this->hg);
        
        int s = hg.size();
        
        for(int i = 0; i < s; i++){
            if (!hg2.hg[i].does_intersect_with(he)){
                hg2.hg.erase(hg2.hg.begin() + i);
                i -= 1;
                s -= 1;
            }
        }
        return hg2;
    };
    
    bool is_isomorph_to_via_rule(bool of_same_size, Rule r, Hypergraph hg2){
        // Preliminary checks if of_same_size = false
        if (of_same_size == false){
            vector<int> nub1;
            vector<int> nub2;
            
            // Just to make sure that each have the same number of Hyperedges.
            if (this->size() != hg2.size())
                return false;
            
            vector<int> uv1 = this->unique_vertices().get_vertices();
            vector<int> uv2 = hg2.unique_vertices().get_vertices();
            
            // If two Hypergraphs do not have the same number of vertices,
            // they cannot be isomorphic.
            if (uv1.size() != uv2.size())
                return false;
            
            nub1 = this->size_nub();
            nub2 = hg2.size_nub();
            
            // We make a simple test. If this fails, we need to work more.
            if (nub1 != nub2)
                return false;
            
            FrequencyDict f1 = this->frequency_of_vertices();
            FrequencyDict f2 = hg2.frequency_of_vertices();
            
            if (is_of_same_shape(f1,f2) == false)
                return false;
        }
            
        Hyperedge he3;
        
        for (Hyperedge he : hg){
            he3 = he.map_via_rule(r);
            if (hg2.size() == 0)
                return true;
            else{
                if (hg2.does_include(he3))
                    hg2 = hg2.remove_hyperedge_once(he3);
                else
                    return false;
            }
        }

        return true;
    };
    
    bool is_isomorph_to(Hypergraph hg2){
        vector<int> nub1;
        vector<int> nub2;
        
        // Just to make sure that each have the same number of Hyperedges.
        if (this->size() != hg2.size())
            return false;
        
        vector<int> uv1 = this->unique_vertices().get_vertices();
        vector<int> uv2 = hg2.unique_vertices().get_vertices();
        
        // If two Hypergraphs do not have the same number of vertices,
        // they cannot be isomorphic.
        if (uv1.size() != uv2.size())
            return false;
        
        nub1 = this->size_nub();
        nub2 = hg2.size_nub();
        
        // We make a simple test. If this fails, we need to work more.
        if (nub1 != nub2)
            return false;
        
        FrequencyDict f1 = this->frequency_of_vertices();
        FrequencyDict f2 = hg2.frequency_of_vertices();
        
        if (is_of_same_shape(f1,f2) == false)
            return false;
        
        // Now, if the previous tests are passed we can consider permutation of vertices with the
        // same frequency counts.
        vector<Rule> all_rules = all_permutations_of_two_dicts(f1, f2);
        
	// The number of threads is define in the main file.
	// Maybe there is an overhead in initializing this here.
	//omp_set_num_threads(NUM_THREADS);

	const int num_of_rules = all_rules.size();
	volatile bool is_iso = false;

        #pragma omp parallel for shared(is_iso)
	  for (int i = 0; i < num_of_rules; i++)
	    {
	    if (is_iso == false)
	      if (this->is_isomorph_to_via_rule(true, all_rules[i], hg2))
		is_iso = true;
             }
	
        return is_iso;
    };
    
    // If newline = false, endl is not printed at the end.
    void print(bool newline = true){
        int s = hg.size();
        
        cout << "{";
        
        for (int i = 0; i < s; i++){
            hg[i].print(false);
            if (i < s-1)
                cout << ",";
        }
        
        cout << "}";
        if (newline)
            cout << endl;
    };
};

//bool operator==(const Hypergraph& hg1, const Hypergraph& hg2){
//    return hg1.hg == hg2.hg;
//}
//
//bool operator!=(const Hypergraph& hg1, const Hypergraph& hg2){
//    return hg1.hg != hg2.hg;
//}

class Tree{
private:
    Hyperedge node;
    vector<Tree> leaves;
    
public:
    
    // Neighborhood Tree of Hyperedge he.
    Tree(Hypergraph hg, Hyperedge he){
        node = he;
        
        if (hg.size()){
            // hg_neigh is the set of all hyperedges that have nonempty intersection with he.
            Hypergraph hg_neigh = hg.neighborhood_of_hyperedge(he);
            Hypergraph hg_copy = hg;
            // We remove he.
            hg_neigh = hg_neigh.remove_hyperedge(he);
            hg_copy = hg_copy.remove_hyperedge(he);
            int s = hg_neigh.size();
            for (int i = 0; i < s; i++)
                this->leaves.push_back(Tree(hg_copy, hg_neigh.get(i)));
        }
    };
    
    // This is the whole Tree that is associated with the Hypergraph.
    Tree(Hypergraph hg){
        Hyperedge he = hg.unique_vertices();
        Hyperedge he_iter;
        int s = he.size();
        
        for (int i = 0; i < s; i++){
            he_iter.append(he.get(i));
            leaves.push_back(Tree(hg, he_iter));
            he_iter.clear();
        }
    };
    
    // Empty constructor.
    Tree(){
    };
    
    Hyperedge get_node(){
        return node;
    };
    
    Tree get_leave(int i){
        return leaves[i];
    };
    
    int size_of_leaves(){
        return leaves.size();
    };
    
    // Returns the Tree from whole_tree that is a Tree beginning with node {u}
    Tree neighborhood_of_vertex(int u){
        Hyperedge he;
        he.append(u);
        
        for (auto leaf : leaves)
            if (leaf.get_node() == he)
                return leaf;
        
        // If the condition is not met, return an empty Tree
        return Tree();
    };
    
    int depth(){
        int d = 0;
        int s = leaves.size();
        vector<int> leave_depth;
        
        if (s == 0)
            return 0;
        else{
            for (int i = 0; i < s; i++)
                leave_depth.push_back(leaves[i].depth());
        }
        
        int m = max_vec_int(leave_depth);
        return m + 1;
    };
    
    // We suppose the head node in the Tree is just a "vertex" like {1}
    Hypergraph neighborhood_at_depth(int d){
        Hypergraph hg;
        
        if (d < 0){
            cout << "Error: Tree depth cannot be negative!" << endl;
            return hg;
        }
        
        int max_depth = this->depth();
        
        // Just to make sure that we do not go beyond the Tree.
        if (d > max_depth)
            d = max_depth;
        
        if (d == 0){
            hg.append(node);
            return hg;
        }
        else{
            for (Tree t : leaves)
                hg.union_with(t.neighborhood_at_depth(d-1));
        }
        return hg;
    };
    
    // We suppose the head node in the Tree is just a "vertex" like {1}
    Hypergraph neighborhood_down_to_depth(int d){
        Hypergraph hg;
        
        int max_depth = this->depth();
        
        // Just to make sure that we do not go beyond the Tree.
        if (d > max_depth)
            d = max_depth;
        
        for (int i = 1; i <= d; i++)
            hg.union_with(this->neighborhood_at_depth(i));
        
        return hg;
    };
    
    void print(string mode = ""){
        
        int s = leaves.size();
        
        if (mode == ""){
            cout << "Node: ";
            node.print();
            
            for (int i = 0; i < s; i++){
                for (int j = 0; j < s; j++)
                    cout << "---";
                leaves[i].print();
            }
        }
        if (mode == "Mathematica"){
            cout << "Tree[";
            node.print(false);
            if (s == 0)
                cout << ", {}]";
            else{
                cout << ", {";
                for (int i = 0; i < s; i++){
                    if (i > 0)
                        cout << ", ";
                    leaves[i].print("Mathematica");
                }
                cout << "}]";
            }
            
        }
        
        
    };
};

