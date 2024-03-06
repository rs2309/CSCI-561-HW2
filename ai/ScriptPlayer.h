//
// Created by Rutvik Shah on 2/26/24.
//

#ifndef CSCI_561_HW2_SCRIPTPLAYER_H
#define CSCI_561_HW2_SCRIPTPLAYER_H
#include "Player.h"
#include "BoardHelper.h"


class ScriptPlayer: public Player {
public:
    string name;
    ScriptPlayer(int p,string n):Player(p),name(n){
    }
    int  run(){
        int compile_status = system(("g++ -std=c++11 -o ./ScriptPlayer/"+name+"/homework ./ScriptPlayer/"+name+"/homework.cpp").c_str());

//         Check if compilation was successful
        if (compile_status != 0) {
            // Handle compilation error
            return -1; // Or another error handling approach
        }

        // Step 2: Run the compiled program
        // Use the "./" prefix to run an executable in the current directory on Unix-like systems
        int run_status = system(("./ScriptPlayer/"+name+"/homework").c_str());

        if (run_status != 0) {
            // Handle runtime error
            return -1; // Or another error handling approach
        }

        return 0;
    }

    Move playMove(Reversi& board,double t){
        ofstream outputFile("./ScriptPlayer/"+name+"/input.txt");
        outputFile<<(current_player==0?'O':'X')<<endl;
        outputFile<<t<<" "<<t<<endl;
        for(int i=0;i<N;i++){
            for(int j=0;j<N;j++){
                string s=board[i][j]==1?"X":(board[i][j]==0?"O":".");
                outputFile<<s;
            }
            outputFile<<endl;
        }
//        outputFile<<(char)(97+result.y)<<result.x+1<<endl;
        outputFile.close();

        int res=run();

        ifstream inputFile("./ScriptPlayer/"+name+"/output.txt");
        string opMove;
        inputFile>>opMove;
        inputFile.close();
        cout<<opMove<<endl;
        Move m{stoi(opMove.substr(1,2))-1,(int)opMove[0]-97};
        return m;
    }
};


#endif //CSCI_561_HW2_SCRIPTPLAYER_H
