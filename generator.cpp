#include <iostream>
#include <random>
#include  <fstream>
#include <algorithm>


int main(){
    std::vector<char> row {'O','.','O','.','O','.','O','.','O','.','O','.','O','.','O','.','O'};
    /* shuffle a vector  and write it to a file */
    std::ofstream output_file("../tmp/go_current_world.txt");
    auto rng = std::default_random_engine {};
    for(int i = 0; i < 10; i++){
        std::shuffle(row.begin(), row.end(), rng);
        for (const auto &e : row) output_file << e;

        output_file << '\n';
    }
    output_file.close();
    return 0;
}