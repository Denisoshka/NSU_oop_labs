#include <process.h>

bool process::getTasks() {

}
void process::startConversion(std::string &&FilePath) {
  FilePath_ = std::move(FilePath);
  if( FilePath_.find(".wav") == std::string::npos ) {
    throw;// todo make ex
  }

  FileOut_.open(FilePath_, std::ios_base::binary);
  if( FileOut_.fail() ) {
    throw;// todo make ex
  }

  while (!getTasks()){
    converterInterface(task);
  }


}
