#include "Net.hpp"
#include "Netlist.hpp"
#ifndef MY_INCLUDES_H
#include "main.hpp"
#endif
#include "Grid_Graph.hpp"
#include "Gamer.hpp"

GAMER::GAMER(int M, int N, int L){
    Sdist.resize(L,std::vector<float>(0));
    Sdir.resize(L,std::vector<char>(0));
    for (int i = 0; i < L; i++) {
        Sdist[i].resize(M * N, 0); // Fill with 0
        Sdir[i].resize(M * N, 'x'); // Fill with space character
    }
}

void GAMER::Init_GAMER(Grid_Graph& G, Point src, Point cornerl, Point cornerh, int first){
    #pragma omp parallel num_threads(num_thread)
    {
        /*std::cout << " size " << Sdir.size() <<" size2 "<<Sdir[0].size()<< std::endl;
        std::cout << " size " << Sdist.size() <<" size2 "<<Sdist[0].size()<< std::endl;
        exit(1);*/
        if (first){
             #pragma omp for collapse(3)
            for (int k = 0; k < Sdist.size(); k += 2){
                for (int i = cornerl.y; i<cornerh.y+1;i++){
                    for (int j = cornerl.x; j<cornerh.x+1;j++){
                        Sdist[k][i*G.M+j] = std::numeric_limits<float>::max()*0.5;
                        Sdir[k][i*G.M+j] = 'x';
                    }
                }
            }
            #pragma omp for collapse(3)
            for (int k = 1; k < Sdist.size(); k += 2){
                for (int i = cornerl.x; i<cornerh.x+1;i++){
                    for (int j = cornerl.y; j<cornerh.y+1;j++){
                        Sdist[k][i*G.N+j] = std::numeric_limits<float>::max()*0.5;
                        Sdir[k][i*G.N+j] = 'x';
                    }
                }
            }
            #pragma omp single
            {
                if (src.l%2==0){
                    Sdist[src.l][src.y*G.M+src.x] = 0;
                }
                else{
                    Sdist[src.l][src.x*G.N+src.y] = 0;
                }
            }
        }
        else{
            #pragma omp for collapse(3)
            for (int k = 0; k < Sdist.size(); k += 2){
                for (int i = cornerl.y; i<cornerh.y+1;i++){
                    for (int j = cornerl.x; j<cornerh.x+1;j++){
                        if (Sdist[k][i*G.M+j]){                             
                            Sdist[k][i*G.M+j] = std::numeric_limits<float>::max()*0.5;
                            Sdir[k][i*G.M+j] = 'x';
                        }
                    }
                }
            }
            #pragma omp for collapse(3)
            for (int k = 1; k < Sdist.size(); k += 2){
                for (int i = cornerl.x; i<cornerh.x+1;i++){
                    for (int j = cornerl.y; j<cornerh.y+1;j++){
                        if (Sdist[k][i*G.N+j]){                          
                            Sdist[k][i*G.N+j] = std::numeric_limits<float>::max()*0.5;
                            Sdir[k][i*G.N+j] = 'x';
                        }
                    }
                }
            }
        }
    }
}

void GAMER::printpath(int L,int M,int N){
    // Printing out Sdist
    //std::cout << "Sdist array:" << std::endl;
    for (int k = 0; k < L; k++) {
        for (int i = 0; i < M; i++) {
            for (int j = 0; j < N; j++) {
                //std::cout << Sdist[k][i * N + j] << " ";
            }
            //std::cout << std::endl;
        }
        //std::cout << std::endl;
    }

    // Printing out Sdir
    //std::cout << "Sdir array:" << std::endl;
    for (int k = 0; k < L; k++) {
        for (int i = 0; i < M; i++) {
            for (int j = 0; j < N; j++) {
                //std::cout << Sdir[k][i * N + j] << " ";
            }
            //std::cout << std::endl;
        }
        //std::cout << std::endl;
    }
}

void GAMER::algo(Grid_Graph& G, Point cornerl, Point cornerh, int alternations, int lay){
    //std::cout<< "start algo " << std::endl;
    bool flag = 1; // to keep track of if the relaxation step has caused any change to the distances and routes or not
    #pragma omp parallel num_threads(num_thread) shared(flag)
    {
        int iter = 0;
        //std::cout<< "Entered parallel region " << std::endl;
        int l;
        bool loc_flag;
        while (flag&&(iter<alternations)){
            //printpath(3,5,5);
            loc_flag = 0;
            //std::cout<< "step1 " << std::endl;
            // Relaxing Sdir1,3... (rows)
            #pragma omp for collapse(2)
            for (l = 0; l < lay; l += 2){
                //std::cout<< "Layer is " << l <<std::endl;
                for (int k = cornerl.y; k<cornerh.y+1; k++){
                    // left to right
                    for (int j = cornerl.x+1; j<cornerh.x+1; j++){
                        //std::cout<< Sdist[l][G.M*k+j]<<" ";
                        //std::cout<< Sdir[l][G.M*k+j]<<" ";
                        if (Sdist[l][G.M*k+j] > Sdist[l][G.M*k+j-1]+G.weight(l,j,k)){
                            Sdist[l][G.M*k+j] = Sdist[l][G.M*k+j-1]+G.weight(l,j,k);
                            Sdir[l][G.M*k+j] = 'l';
                            loc_flag = 1;
                        }
                    }
                    // right to left
                    for (int j = cornerh.x-1; j>cornerl.x-1; j--){
                        if (Sdist[l][G.M*k+j] > Sdist[l][G.M*k+j+1]+G.weight(l,j,k)){
                            Sdist[l][G.M*k+j] = Sdist[l][G.M*k+j+1]+G.weight(l,j,k);
                            Sdir[l][G.M*k+j] = 'r';
                            loc_flag = 1;
                        }
                    }
                    //std::cout<< std::endl;
                }
            }                //std::cout<<std::endl;
            //std::cout<<std::endl;
            // Relaxing Sdir1,3... (cols)
            #pragma omp for collapse(2)
            for (l = 1; l < lay;l += 2){
                //std::cout<< "layer " << l << std::endl;
                for (int j = cornerl.x; j<cornerh.x+1; j++){
                    // south to  north
                    for (int k = cornerl.y+1; k<cornerh.y+1; k++){
                        //std::cout<< Sdist[l][G.N*j+k] <<" ";
                        //std::cout<< Sdir[l][G.N*j+k] <<" ";
                        if (Sdist[l][G.N*j+k] > Sdist[l][G.N*j+k-1]+G.weight(l,j,k)){
                            Sdist[l][G.N*j+k] = Sdist[l][G.N*j+k-1]+G.weight(l,j,k);
                            Sdir[l][G.N*j+k] = 's';
                            loc_flag = 1;
                        }
                    }
                    // right to left
                    //std:cout<<std::endl;
                    for (int k = cornerh.y-1; k>cornerl.y-1; k--){
                        if (Sdist[l][G.N*j+k] > Sdist[l][G.N*j+k+1]+G.weight(l,j,k)){
                            Sdist[l][G.N*j+k] = Sdist[l][G.N*j+k+1]+G.weight(l,j,k);
                            Sdir[l][G.N*j+k] = 'n';
                            loc_flag = 1;
                        }
                    }
                }
                //std::cout<<std::endl;
            }
            // via sweep
            // down to up
            // Validate Sdir. Print values of SDir[0] where it is 'd' Check code
            #pragma omp for collapse(2)
            for (l=0; l< lay-1; l+=2){
                for (int k = cornerl.y; k<cornerh.y+1; k++){
                    for (int j = cornerl.x; j<cornerh.x+1; j++){
                        if (Sdist[l][G.M*k+j] > Sdist[l+1][G.N*j+k]+G.v){
                            Sdist[l][G.M*k+j] = Sdist[l+1][G.N*j+k]+G.v;
                            Sdir[l][G.M*k+j] =  'u';
                            loc_flag = 1;
                        }
                    }
                }
            }
            #pragma omp for collapse(2)
            for (l=1; l< lay-1; l+=2){
                for (int k = cornerl.y; k<cornerh.y+1; k++){
                    for (int j = cornerl.x; j<cornerh.x+1; j++){
                        if (Sdist[l][G.N*j+k] > Sdist[l+1][G.M*k+j]+G.v){
                            Sdist[l][G.N*j+k] = Sdist[l+1][G.M*k+j]+G.v;
                            Sdir[l][G.N*j+k] =  'u';
                            loc_flag = 1;
                        }
                    }
                }
            }

// Validate Sdir. Print values of SDir[0] where it is 'd' Check code
            // up to down
            // std:cout << "via sweep "<<std::endl;
            #pragma omp for collapse(2)
            for (l=lay-2; l>=0; l--){
                for (int k = cornerl.y; k<cornerh.y+1; k++){
                    for (int j = cornerl.x; j<cornerh.x+1; j++){
                        //std::cout<< Sdist[l][G.M*k+j]<<" ";
                        //std::cout<< Sdir[l][G.M*k+j]<<" ";
                        if (l%2==0){
                            if (Sdist[l][G.M*k+j] + G.v < Sdist[l+1][G.N*j+k]){
                                Sdist[l+1][G.N*j+k] = Sdist[l][G.M*k+j]+G.v;
                                Sdir[l+1][G.N*j+k] =  'd';
                                loc_flag = 1;
                            }
                        }
                        else{
                            if (Sdist[l][G.N*j+k] + G.v < Sdist[l+1][G.M*k+j]){
                                Sdist[l+1][G.M*k+j] = Sdist[l][G.N*j+k]+G.v;
                                Sdir[l+1][G.M*k+j] =  'd';
                                loc_flag = 1;
                            }
                        }
                    }
                    //std::cout<<std::endl;
                }
                //std::cout<<std::endl;
            }

            // Print values of SDir[0] where it is 'd'. #Check2

            //std::cout<<std::endl;
            iter++;
            #pragma omp master
            {
                flag = 0;
            }
            #pragma omp critical
            {
                if (loc_flag){
                    flag = 1;
                }
            }
            #pragma omp barrier
            //std::cout<< "\nIterations alternations flag " << iter <<" "<<alternations<< " "<<flag<<std::endl<<"\n"; 
            //#pragma omp barrier
        }
        //std::cout<< "out of while loop " << std::endl;
    } // While loop ends
    //std::cout<< "Completed algo " << std::endl;
}