#include <ctime>
#include <iostream>

#include "renameosgb.hpp"

int main(int argc, char** argv)
{
  clock_t t = clock();

  if (argc != 3)
  {
    std::cout << "Useage:\n"\
                 "rename.exe path_to_file.osgb output_path\n";
    return -1;
  }

  Renameosgb::read(argv[1],argv[2]);

  t = clock() - t;
  std::cout << "Finish in " << ((float)t)/CLOCKS_PER_SEC << " seconds.\n";

  return 0;
}
