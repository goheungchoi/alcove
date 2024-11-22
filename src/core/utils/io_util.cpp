#include "core/utils/io_util.h"

#include <fstream>

/**
 * @brief Read the binary data of a file. outData must be freed after use.
 * 
 * @param filepath 
 * @param outSize 
 * @param outData 
 * @return true 
 * @return false 
 */
bool al::read_file(const char* filepath, unsigned long* outSize, void** outData) {
  // Open the file with the cursor at the end
  std::ifstream file(filepath, std::ios::ate | std::ios::binary);

  if (!file.is_open()) {
    return false;
  }

  // The location of the cursor tells the size of 
  // the file in bytes
  unsigned long filesize = static_cast<unsigned long>(file.tellg());

  // Return the size of the file
  *outSize = filesize;
  
  if (outData) {
    // Allocate memory space for the byte data
    char* data = (char*) malloc(filesize);
    
    // Place the fie cursor at the beginning
    file.seekg(0);

    // Load the entire file into the buffer
    file.read((char*)data, filesize);

    *outData = (void*) data;
  }

  // Close the file
  file.close();

  // Return true
  return true;
}



