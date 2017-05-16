#include "../include/SrtmHgtReader.h"

SrtmHgtReader::SrtmHgtReader(void){
	;
}
SrtmHgtReader::SrtmHgtReader(const std::string &_db_path){
	this->_db_path=_db_path;
}
SrtmHgtReader::~SrtmHgtReader(void){
	this->_tiles.clear();
}
void SrtmHgtReader::loadTile(const int &_latDec,const int &_lonDec){
	if(this->_tiles.count(std::pair<int,int>(_latDec,_lonDec))==0){
		char filename[2048];
      sprintf(filename,"%s/%c%02d%c%03d.hgt",this->_db_path.c_str(),_latDec>0?'N':'S',abs(_latDec),_lonDec>0?'E':'W',abs(_lonDec));
		//this->_tiles[std::pair<int,int>(_latDec,_lonDec)]=make_shared<Tile>(filename);
		this->_tiles[std::pair<int,int>(_latDec,_lonDec)]=Tile(filename);
	}
}
double SrtmHgtReader::getElevation(const double &_lat,const double &_lon){
	int latDec=int(floor(_lat));
   int lonDec=int(floor(_lon));

   float secondsLat=(_lat-latDec)*60*60;
   float secondsLon=(_lon-lonDec)*60*60;

   this->loadTile(latDec,lonDec);

	int y=secondsLat/SECONDSPERPX;
   int x=secondsLon/SECONDSPERPX;

   int height[4];

   /*height[2]=this->readPx(this->_tiles[std::pair<int,int>(latDec,lonDec)]->data(),y,x);
   height[0]=this->readPx(this->_tiles[std::pair<int,int>(latDec,lonDec)]->data(),y+1,x);
   height[3]=this->readPx(this->_tiles[std::pair<int,int>(latDec,lonDec)]->data(),y,x+1);
   height[1]=this->readPx(this->_tiles[std::pair<int,int>(latDec,lonDec)]->data(),y+1,x+1);*/
   height[2]=this->readPx(this->_tiles[std::pair<int,int>(latDec,lonDec)].data(),y,x);
   height[0]=this->readPx(this->_tiles[std::pair<int,int>(latDec,lonDec)].data(),y+1,x);
   height[3]=this->readPx(this->_tiles[std::pair<int,int>(latDec,lonDec)].data(),y,x+1);
   height[1]=this->readPx(this->_tiles[std::pair<int,int>(latDec,lonDec)].data(),y+1,x+1);

   float dy=fmod(secondsLat,SECONDSPERPX)/SECONDSPERPX;
   float dx=fmod(secondsLon,SECONDSPERPX)/SECONDSPERPX;

   return(height[0] * dy * (1 - dx) + height[1] * dy * (dx) + height[2] * (1 - dy) * (1 - dx) + height[3] * (1 - dy) * dx);
}
int SrtmHgtReader::readPx(unsigned char *_data,const int &_y,const int &_x){
	int row=(TOTALPX-1)-_y;
   int col=_x;
   int pos=(row*TOTALPX+col)*2;
	unsigned char *buff=&_data[pos];
   int16_t hgt=0|(buff[0]<<8)|(buff[1]<<0);
   return(int(hgt));
}
