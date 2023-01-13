#include<iostream>
#include<fstream>
#include<dirent.h>
#include<sys/stat.h>

// this function will encrypt a single file
void encryptFile(std::string filePath){
    // File is the file which will be encrypted
    // tempFile is a temporary file which save encrypted data of file
    std::fstream file, tempFile;
    std::string tempFilePath = "temp.txt";


    //Open file streams
    //Open file to read, tempFile to write
    file.open(filePath, std::ios::in);
    tempFile.open(tempFilePath, std::ios::out);

    // Read byte by byte through file
    char byte;
    while(file>>std::noskipws>>byte){
      //encrypt this byte by add 1 to it
      byte += 1;
      // Save this byte into temp file
      tempFile<<byte;
    }

    //close file streams
    file.close();
    tempFile.close();


    // Open file steams
    // But this time we will open file to write and temp file to read
    file.open(filePath, std::ios::out);
    tempFile.open(tempFilePath, std::ios::in);


    //Read through temp file byte by byte
    while(tempFile>>std::noskipws>>byte){
      //Save this byte into file
      file<<byte;
    }

    //close file streams
    file.close();
    tempFile.close();

    //Delete temp file
    remove(tempFilePath.c_str());
}

// this function will read through all directories and files in a directory
// If it find a file, it will encrypt that file
// If it find a directory, it will read through that directory
void encryptDirectory(std::string directoryPath){
    DIR* directory;
    struct dirent* entry;
    struct stat status;
    std::string path;

    //open the directory
    if((directory = opendir(directoryPath.c_str()))!=NULL){
        //Open directory successfully
        // Read through directory
        while((entry = readdir(directory))!=NULL){
            // check if this entry (file or directory) is current directory(".") or parent directory("..")
            if(strcmp(entry->d_name, ".")!=0 && strcmp(entry->d_name,"..")!=0){
                //get enty full path
                path = directoryPath + "\\" + entry->d_name;
                //check if this entry is a directory or file
                stat(path.c_str(), &status);
                if(S_ISDIR(status.st_mode)){
                    // this is a directory , thus we will read through it
                    encryptDirectory(path);
                }else{
                    // this is a file so we'll encypt it
                    encryptFile(path);
                }
            }
        }
    }
}

int main(){

    encryptDirectory("C:\\Users\\sanke\\OneDrive\\Desktop\\coding\\tools\\encryptionTool\\data");
     
    return 0;
}