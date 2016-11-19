#include <iostream>
#include <string>
#include <cstring>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <cstdlib>
#include <climits>
#include <fstream>
#include <stdexcept>
#include <sstream>

using namespace std;

class File_manipulator{
public:
	File_manipulator(string file_name){
		file_path = getRealpath(file_name);
		this->file_name = getFileName(file_name);

		ip_file.open(file_name.c_str(), ios::in | ios::binary);
		if(ip_file.fail())
			throw runtime_error("File open failed");
	}

	bool makeChunks(string path_name);
	bool makeChunks(){return makeChunks(file_path);}

private:
	ifstream ip_file;
	string file_name;
	string file_path;
	static const int fileChunkLen = 64; // in bytes

	string getRealpath(string fileName){
		char realpth[PATH_MAX + 1];
		if(realpath(fileName.c_str(), realpth) == NULL){
			perror("RealPath could not be found ");
			throw runtime_error("RealPath could not be found");
		}
		string s_realpth(realpth);
		
		char sep = '/';
		size_t i = s_realpth.rfind(sep, s_realpth.length());
		if (i != string::npos)
	      return(s_realpth.substr(0, i+1));
	  	throw runtime_error("RealPath could not be extracted");
	}

	string getFileName(string fileName){
		char sep = '/';

	   size_t i = fileName.rfind(sep, fileName.length());
	   if (i != string::npos)
	      return(fileName.substr(i+1, fileName.length() - i));
	   
	   return(fileName);
	}

};

bool File_manipulator::makeChunks(string path_name){
	//ifstream _ip_file;
	//_ip_file.copyfmt(ip_file);

	string path(path_name + file_name + "_dir");
	int ret = mkdir(path.c_str(), S_IRWXU | S_IRWXG | S_IRWXO);
	if(ret < 0){
		perror("Could not make directory ");
		return false;
	}

	struct stat file_stats;
	if(stat((file_path + file_name).c_str(), &file_stats) != 0){
		perror("could not get file stats");
		return false;
	}
	int numChunks = (file_stats.st_size + fileChunkLen-1) / fileChunkLen;

	cout << "path " << path << endl;
	cout << "size: " << file_stats.st_size << " numChunks " << numChunks << endl;
	
	for(int i=0; i< numChunks; i++){

		char data[fileChunkLen + 1];
		memset(data, 0, fileChunkLen + 1);
		ip_file.read(data, fileChunkLen);
		if(!ip_file.good() && !ip_file.eof()){
			throw runtime_error("error reading from file. ");
			return false;
		}
		else if(ip_file.eof()){
			cout << "eof reached " << ip_file.gcount() << " bytes read. " << endl;
		}
		stringstream num; num << i;
		stringstream files(path + "/" + num.str());

		cout << "data len " << strlen(data) <<  endl;
		cout << "file path name " << files.str() << endl;
		if(ip_file.gcount() > 0){
			ofstream op_file(files.str().c_str(), ios::binary | ios::out);
			op_file.write(data, ip_file.gcount());
			op_file.close();
		}
		else{
			cout << "gcount less than 0 " << ip_file.gcount() << endl;
			return false;
		}
		cout << ip_file.gcount() << " bytes written to " << files.str() << endl;
	}
	return true;

}

int main(int argc, char** argv){

	File_manipulator fm("empty");
	fm.makeChunks();
	return 0;
}