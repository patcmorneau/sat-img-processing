#ifndef MASS
#define MASS

#include <iostream>
#include "opencv2/opencv.hpp"
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <vector>
#include <filesystem>


std::string extract_band_name(std::string filename){
	std::vector<std::string> bandNames{"B01","B02","B03","B04","B05","B06","B07","B08","B8A","B09","B10","B11","B12","B13","AOT","SCL","TCI","WVP"};
	std::string bandName;
	int checksum = 0;
	for(int i =0; i<bandNames.size();++i){
		if (filename.find(bandNames[i]) != std::string::npos){
			++checksum;
			bandName = bandNames[i];
		}
	}
	if(checksum == 1){
		return bandName;
	}
	else{
		std::cerr<<"found "<< checksum <<" band name in " << filename<<"\n";
		return filename;
	}
	
}



class Mass{
	public:
		Mass(std::string dirPath){
			const std::filesystem::path path{dirPath};
			for (auto const& dir_entry : std::filesystem::directory_iterator{path}){
				if (dir_entry.is_directory()){
					//std::cout<<dir_entry.path().filename()<<"\n";
					for (auto const& file : std::filesystem::directory_iterator{dir_entry.path()}){
						if (file.is_regular_file() && file.path().extension() == ".jp2"){
							//std::cout<<file<<"\n";
							std::filesystem::path filePath = file.path();
							this->files.insert({extract_band_name(filePath.filename()), file.path()});
						}
						else{
							std::cerr<<"not a valid file or not a .jp2 file \n";
						}
					}
					this->resolutions.insert({dir_entry.path().filename(), files});
					this->files.clear();
				}
			}
		
		}
		
		std::map<std::string,std::map<std::string,std::string>> get_all_files(){ return this->resolutions};
		
		
	private:
		std::map<std::string,std::string> files;
		std::map<std::string,std::map<std::string,std::string>> resolutions;
};

#endif
