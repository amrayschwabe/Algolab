// ALGOLAB BGL Tutorial 3
// Flow example demonstrating
// - breadth first search (BFS) on the residual graph


// Includes
// ========
// STL includes

#include <iostream>
#include <algorithm>
#include <vector>
#include <queue>
#include <climits>
#include <set>
#include <map>

using namespace std;

struct entry{
    string name;
    vector<entry> direct_offspring;
    vector<string> path_to_luca;
    long age;
    //TODO
    bool operator<(const entry & rhs) const
    {
        return age < rhs.age;
    }
};


void testcase(){
    //read in nr_species and nr_queries
    long nr_species, nr_queries;
    cin >> nr_species >> nr_queries;
    map<string, entry> species_map;
    long curr_max = 0;
    string lucaname;
    for(long i = 0; i < nr_species; ++i){
        string name; long age;
        cin >> name >> age;
        entry* species = new entry;
        species->name = name;
        species->age = age;
        species_map.insert(make_pair(name, *species));
        if(age > curr_max){
            curr_max = age;
            lucaname = name;
        }
    }
    //read in relations
    for(long i = 0; i<nr_species-1; ++i){
        string offspring, anchestor;
        cin >> offspring >> anchestor;
        //cout << "offspring " << offspring << " anchestor " << anchestor << endl;
        species_map[anchestor].direct_offspring.push_back(species_map[offspring]);
        //cout << "size " << species_map[anchestor].direct_offspring.size() << " anchestor " << anchestor << endl;
    }
/*
    for(auto i:species_map){
        cout << "name " << i.second.name << endl;
        cout << "direct_offspring" << i.second.direct_offspring.size() << endl;
        if(i.second.direct_offspring.size() > 0) {
            cout << "child" << i.second.direct_offspring[1].direct_offspring.size() << endl;
           cout << species_map[i.second.direct_offspring[0].name].direct_offspring.size() << endl;
        }

    }*/
    //for every species, add path to luca
    entry luca = species_map[lucaname];
    vector<string> path;
    luca.path_to_luca = path;
    std::queue<entry*> Q2; // BFS queue (from std:: not boost::)
    Q2.push(&luca);
    while (!Q2.empty()) {
        entry u = *Q2.front();
        //cout << "name " << u.name << endl;
        //cout << "age " << u.age << endl;
        Q2.pop();
        vector<string> path = u.path_to_luca;
        path.push_back(u.name);
        //cout << "length" << path.size() << endl;
        //cout << "size " << u.direct_offspring.size() << endl;
        for (auto it : u.direct_offspring) {
           //cout << "it " << it.name << endl;
            //cout << "direct offspring " << it.direct_offspring.size() << endl;
            species_map[it.name].path_to_luca = path;

            Q2.push(&species_map[it.name]);
            //cout << "size " << Q2.size() << endl;
        }
    }
    for(auto i : species_map){
        species_map[i.second.name].path_to_luca.push_back(i.second.name);
    }
    //read in queries
    for(long i = 0; i <nr_queries; ++i){
        string name; long age;
        cin >> name >> age;
        //cout << "name " << name << endl;
        entry tofind = species_map[name];
        vector<string> path = tofind.path_to_luca;
        long length = path.size();
        long middle;
        long upper_limit = length -1;
        long lower_limit = 0;
        //cout << "l " << lower_limit << " u " << upper_limit << endl;
        while(lower_limit != upper_limit){
            //cout << "upper_limit " << upper_limit << " lower_limit " << lower_limit << endl;
            middle = (upper_limit + lower_limit)/2;
            //cout << "l " << lower_limit << " u " << upper_limit << endl;
            if(species_map[path[middle]].age > age){
                //cout << middle<< endl;
                lower_limit = middle + 1;
                //cout << "middle " << middle << endl;
            }else{
                upper_limit = middle;
                //cout << middle << endl;
            }
        }

        string result = (path[lower_limit]);
        cout << result << " ";
    }
    cout << endl;

}
// Main
int main() {
    long nr;
    cin >> nr;
    while(nr--){
        testcase();
    }
}