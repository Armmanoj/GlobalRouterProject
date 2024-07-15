g++ ../evaluator.cpp -o evaluator
g++ -fopenmp -O3 -Wall -fopt-info-all ../main.cpp ../Netlist.cpp ../Net.cpp ../Gamer.cpp ../Grid_Graph.cpp 2>../err.txt
./a.out tst_5.net tst_5.cap tst_5.PR_output 1 3 1>../debug.txt
python3 ../visual_tst.py tst_5.cap tst_5.net tst_5.PR_output
./evaluator tst_5.cap tst_5.net tst_5.PR_outpu
rm evaluator
