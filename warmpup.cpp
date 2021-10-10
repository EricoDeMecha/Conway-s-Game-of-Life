#include <vector>
using std::vector;
#include <iostream>
using std::cout;


std::vector<std::vector<bool> > world;/* world vector*/
char text[3] = ".O";
/* read data from  file into vector of vector*/
int initFromFile(const std::string& fname){
    FILE* fseed =fopen(fname.c_str(), "rb");// open the file for reading
    if(!fseed){
        perror("Seed file could not be open\n");
        return -1;/* File read failed*/
    }
    char c;// read a 1 byte character from the file
    size_t nrows = 0; // rows  from the seed file
    world.emplace_back(std::vector<bool>());/*Push one before loop*/
    while(fread(&c,1,1,fseed)){
        if(c == '\n'){
            // found a new line
            world.emplace_back(std::vector<bool>());
            nrows++;
        }else if(c == text[0]){/*.*/
            world[nrows].emplace_back(false);/*Dead neighbor*/
        }else if(c == text[1]){/*O*/
            world[nrows].emplace_back(true);/* Live neighbors*/
        }
    }
    fclose(fseed);/* Close the file*/
    return  1;/* File read successful */
}

//r = row
//columns
//totalR = total Rows
//totalC = total Columns

int countN(int r, int c, int totalR, int totalC)
{
    int n = 0;

    //Counting Neighbors

    //top
    if (world[(r - 1 + totalR) % totalR][c])
    {
        n++;
    }

    //top-right
    if (world[(r - 1 + totalR) % totalR][(c + 1 + totalC) % totalC])
    {
        n++;
    }

    //top-left
    if (world[(r - 1 + totalR) % totalR][(c - 1 + totalC) % totalC])
    {
        n++;
    }

    //right
    if (world[r][(c + 1) % totalC])
    {
        n++;
    }

    //left
    if (world[r][(c - 1 + totalC) % totalC])
    {
        n++;
    }

    //bottom
    if (world[(r + 1) % totalR][c])
    {
        n++;
    }

    //bottom-right
    if (world[(r + 1) % totalR][(c + 1) % totalC])
    {
        n++;
    }

    //bottom-left
    if (world[(r + 1 + totalR) % totalR][(c - 1 + totalC) % totalC])
    {
        n++;
    }

    return n;
}

void update () {
    int totalR = world.size();
    int totalC = world [0].size();

    vector<vector<bool>> future(world);

    for (int r = 0; r < totalR; r++) {
        for (int c = 0; c < totalC; c++) {

//neighbors

            int n = countN(r, c, totalR, totalC);

//if alive
            if(world[r][c]) {

//die when
                if (n < 2 || n > 3) {
                    future[r][c] = 0;
                }
            }

//if dead
            else if (n == 3) {
                future[r][c] = 1;
            }
        }
    }
    world = future;
}

int main(int argc, char *argv[])
{
    int p = 1;
    if(argc > 1)
    {
        //used atoi to convert the char to int
        p = atoi(argv[1]);
    }

    /*Load  the world here*/
    const char* filename  = "../tmp/go_current_world.txt";
    initFromFile(filename);

    for (int i = 0; i < p; i++)
    {
        printf("Reached here\n");
        update();
    }

    int totalR = world.size();
    int totalC = world [0].size();

    for (int r = 0; r < totalR; r++)
    {
        for (int c = 0; c < totalC; c++)
        {
            if(world[r][c])
            {
                cout << "O";
            }
            else
            {
                cout << ".";
            }
        }
        cout << "\n";
    }

    return 0;
}


