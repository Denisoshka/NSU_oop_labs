#include "cl_parser.h"
#include "converterInterface.h"

#include <iostream>

int main(int argc, char ** argv) {
  clParser cl;
  try{
    if (!cl.getOptions(argc, argv)){
      return 0;
    }
    cl
  }
  return 0;
}