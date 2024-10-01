#include <GameEngineFramework/Serialization/Serialization.h>

#include <fstream>


Serialization::Serialization(void) {
    return;
}

bool Serialization::Serialize(std::string filename, void* buffer, unsigned int size) {
    std::ofstream fStream(filename, std::fstream::out | std::fstream::binary);
    
    if (!fStream.is_open()) 
        return false;
    
    fStream.write( (char*)buffer, size);
    
    fStream.close();
    return true;
}


bool Serialization::Deserialize(std::string filename, void* buffer, unsigned int size) {
    std::ifstream fStream(filename, std::fstream::in | std::fstream::binary);
    
    if (!fStream.is_open()) 
        return false;
    
    fStream.read((char*)buffer, size);
    
    fStream.close();
    return true;
}


int Serialization::GetFileSize(std::string filename) {
    
    std::ifstream fStream(filename, std::ios::binary);
    if (!fStream) 
        return -1;
    
    fStream.seekg( 0, std::ios::end );
    std::streamsize size = fStream.tellg();
    
    fStream.close();
    return size;
}

