#ifndef _SRTM_HGT_READER_
#define _SRTM_HGT_READER_
#include <stdio.h>
#include <math.h>
#include <map>
#include <string>
#include <memory>
#include <mutex>
#include <fstream>
#include <string.h>
#include <iostream>
#define TOTALPX 		1201
#define SECONDSPERPX 3
using namespace std;
class SrtmHgtReader{

	public:	class Tile{
					private:	unsigned char *_data;
								std::streamsize _size;

					public:	Tile(void){
									this->_data=NULL;
									this->_size=0;
								}
								Tile(const Tile &_tile){
									this->_size=_tile._size;
									this->_data=(unsigned char*)malloc(this->_size);
									memcpy((void*)this->_data,(void*)_tile._data,this->_size);
								}
								Tile(const std::string &_filename){
									std::ifstream file(_filename,std::ios::binary|std::ios::ate);
									this->_size=file.tellg();
									file.seekg(0,std::ios::beg);

									this->_data=(unsigned char*)malloc(this->_size);
									if(!file.read((char*)this->_data,this->_size)){
										cerr << "Error::File " << _filename << " does not exists" << endl;
										exit(EXIT_FAILURE);
									}
									file.close();
								}
								void operator=(const Tile &_tile){ 
									this->_size=_tile._size;
									this->_data=(unsigned char*)malloc(this->_size);
									memcpy((void*)this->_data,(void*)_tile._data,this->_size);
      						}
								unsigned char* data(void){
									return(this->_data);
								}
								~Tile(void){
									if(this->_data!=NULL) free(this->_data);
								}
				};

				std::string _db_path;
				std::mutex  _write_mutex;
				std::map<std::pair<int,int>,Tile> _tiles;
				
			
	public:	SrtmHgtReader(void);	
				SrtmHgtReader(const std::string&);
				~SrtmHgtReader(void);

				void loadTile(const int&,const int&);
				double getElevation(const double&,const double&);
				int readPx(unsigned char*,const int&,const int&);
};
#endif
