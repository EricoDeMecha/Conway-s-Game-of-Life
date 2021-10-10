#include <cstdio>
#include <cstdlib> // for exit();
#include <getopt.h> // to parse long arguments.
#include <unistd.h> // sleep
#include <vector>
#include <string>
#include <algorithm>

static const char* usage =
        "Usage: %s [OPTIONS]...\n"
        "Text-based version of Conway's game of life.\n\n"
        "   --seed,-s     FILE     read start state from FILE.\n"
        "   --world,-w    FILE     store current world in FILE.\n"
        "   --fast-fw,-f  NUM      evolve system for NUM generations and quit.\n"
        "   --help,-h              show this message and exit.\n";

size_t max_gen = 0; /* if > 0, fast forward to this generation. */
std::vector<std::vector<bool> > world;/* world vector*/
FILE* fw;/*writing*/



/* NOTE: you don't have to write these functions -- this is just how
 * I chose to organize my code. */
size_t nbrCount(size_t i, size_t j, const std::vector<std::vector<bool> >& g);
void update();
int initFromFile(const std::string& fname); /* read initial state into vectors. */
void mainLoop();
void dumpState(FILE* f);

/* NOTE: you can use a *boolean* as an index into the following array
 * to translate from bool to the right characters: */
char text[3] = ".O";

int main(int argc, char *argv[]) {
    // filename
    std::string wfilename =  "../tmp/gol-world-current"; /* write state here */
    std::string initfilename = "../tmp/gol-world-current"; /* read initial state from here. */
    // define long options
    static struct option long_opts[] = {
            {"seed",    required_argument, nullptr, 's'},
            {"world",   required_argument, nullptr, 'w'},
            {"fast-fw", required_argument, nullptr, 'f'},
            {"help",    no_argument,       nullptr, 'h'},
            {nullptr,0,nullptr,0}
    };
    // process options:
    char c;
    int opt_index = 0;
    while ((c = getopt_long(argc, argv, "hs:w:f:", long_opts, &opt_index)) != -1) {
        switch (c) {
            case 'h':
                printf(usage,argv[0]);
                return 0;
            case 's':
                initfilename = optarg;
                break;
            case 'w':
                wfilename = optarg;
                break;
            case 'f':
                max_gen = atoi(optarg);
                break;
            case '?':
                printf(usage,argv[0]);
                return 1;
        }
    }
    /* NOTE: at this point wfilename initfilename and max_gen
     * are all set according to the command line: */
    printf("input file:  %s\n",initfilename.c_str());
    printf("output file: %s\n",wfilename.c_str());
    printf("fast forward to generation: %lu\n",max_gen);
    /* TODO: comment out 3 lines above once you see what's in them. */
    /* NOTE: if wfilename == "-" you should write to stdout, not a file
     * named "-"! */

    /*Process arguments before going into the main loop*/
    if(initFromFile(initfilename) == 1){
        if(wfilename == "-"){
            fw = stdout;/*dump to the terminal*/
            if(!fw){
                printf("\n stdin could not open\n");
                return -1;
            }
            mainLoop();
        }else{
            fw = fopen(wfilename.c_str(), "wb");
            if(!fw){
                printf("%s could not opened\n", wfilename.c_str());
                return -1;
            }else{
                mainLoop();
            }
        }
    }
    fclose(fw);
    return 0;
}

void mainLoop() {
    /* TODO: write this */
    /* update, write, sleep */
    if (max_gen == 0) {
        /* make one generation update per second */
        while(1){
            update();
            dumpState(fw);
            printf("-----------\n");
            sleep(1);/* sleep for a second */
        }
    } else {
        /* go through generations as fast as you can until
         * max_gen is reached... */
        for(size_t i = 0; i < max_gen; i++){
            update();
            dumpState(fw);
            printf("-----------\n");
        }
    }
}
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
void dumpState(FILE* f){
    const char  endLine  = '\n';
    for(const std::vector<bool>& vec: world){
        for(const bool& ch: vec){
            if(ch){
                fwrite(&text[1], sizeof(char), 1, f);/* Write live neighbor to the file*/
                /*printf("%c,",text[1]);*/
            }else{
                /*printf("%c,",text[0]);*/
                fwrite(&text[0], sizeof(char), 1, f);/* Write dead neighbor to the file*/
            }
        }
        /*printf("\n");*/
        fwrite(&endLine,sizeof(char),1, f);/* Write endline to the file*/
    }
}
size_t nbrCount(size_t i, size_t j, const std::vector<std::vector<bool> >& g){
    size_t _count = 0;
    // count the live neighbor
    size_t  R = (g.size() - 1);
    size_t  C = (g[i].size() -1);
    //top
    if (g[std::max<int>(0,(i-1))][j]){   _count++;   }
    //top-right
    if (g[std::max<int>(0, (i-1))][std::min<int>(C,(j+1))]){   _count++;   }
    //top-left
    if (g[std::max<int>(0, (i-1))][std::max<int>(0, (j-1))]){   _count++;   }
    //right
    if (g[i][std::min<int>(C, (j+1))]){    _count++;   }
    //left
    if (g[i][std::max<int>(0, (j-1))]){   _count++;   }
    //bottom
    if (g[std::min<int>(R, (i+1))][j]){    _count++;   }
    //bottom-right
    if (g[std::min<int>(R, (i+1))][std::min<int>(C, (j+1))]){ _count++;   }
    //bottom-left
    if (g[std::min<int>(R,(i+1))][std::max<int>(0, (j-1))]){   _count++;   }
    return _count;
}
void update(){
    for(size_t i = 0; i < world.size(); i++){
        for(size_t j = 0; j < world[i].size(); j++){
            /*Logic*/
            if(world[i][j]){
                size_t live_nbrs  = nbrCount(i,j, world);
                if(live_nbrs < 2 || live_nbrs > 3){
                    world[i][j] = false;
                }
            }else{
                size_t live_nbrs  = nbrCount(i,j, world);
                if(live_nbrs == 3){
                    world[i][j] = true;
                }
            }
        }
    }
}