# Wolfram Model Variety
Author: [Furkan Semih Dündar](mailto:f.semih.dundar@yandex.com)

Website: [Personal website](https://www.fsemih.org)

# About
This repository includes free softwares (released under the GPLv3) that can be used to perform various calculations about Leibnizian ideas on Wolfram Model, most important being the notion of _variety_. However it should be noted that you may need to modify the source codes to do what you want to do with the codes. We provide two programs, a Mathematica library and a C++ program.

## The Mathematica Library (`Variety.wl`)
It is named as “Variety.wl” and depends on [SetReplace](https://github.com/maxitg/SetReplace) package in that it uses  _IsomorphicHypergraphQ_ function from that package. Variety.wl library is useful for experimenting with small hypergraphs and to produce the data (such as variety) to label multiway systems.

## The C++ Program (`wmvar.cpp`)
A C++ program named “wmvar.cpp” is provided which uses OpenMP to parallelize the tasks. It depends on two libraries, 1) Structures.h and 2) Variety.h.

Structures.h defines the basic objects such as hypergraphs, whereas the Variety.h includes high level functions such as variety, absolute indifference and so on.

On GNU/Linux you can compile the code via:

	`g++ wmvar.cpp -o wmvar -w -fopenmp`

or just simply type `make`. (Apple Silicon users, see below.)

The default use of `wmvar` is through `./wmvar -f file` where `file` contains one hypergraph (a list of lists) at each line. Then the output file is `file_hg_and_ais.txt` where at each line one hypergraph appears, a semicolon is placed, then comes a list of absolute indifference values of vertices. Due to race conditions on the OpenMP part, the order of absolute indifference values may change. The reason to display only the absolute indifference values is that, one may use this data even if one chooses a different function for /variety/. Hence there is no need to re-run the program when one wants to use another definition for _variety_.

Moreover, you should change the line `omp_set_num_threads(8);` in `wmvar.cpp` to suit the number of cores you want to use.

### Note for Apple Silicon Users
In order to use OpenMP on Apple Silicon, you may refer to [this guide](https://stackoverflow.com/questions/71061894/how-to-install-openmp-on-mac-m1) . According to a test on M1Max, the following line successfully compiled the code:

	`clang++ -w -Xclang -fopenmp -L/opt/homebrew/opt/libomp/lib -I/opt/homebrew/opt/libomp/include -lomp wmvar.cpp -o wmvar`

# Manuscript

Furkan Semih Dündar. "A Case Study for Leibnizian Ideas in Wolfram Model" Foundations of Physics. 54, 43 (2024). [https://doi.org/10.1007/s10701-024-00777-3](https://doi.org/10.1007/s10701-024-00777-3)

# Citation

If you use the code in your studies, please acknowledge it by citing the article published in Foundations of Physics as well as the url of this repo.

# Acknowledgements
The codes that we share in this repo are products of a study supported by a TÜBİTAK 1002-A project under the grant number 122F297.
