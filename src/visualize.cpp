#include <iostream>
#include "opencv2/opencv.hpp"
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <vector>
#include <math.h>
#include <fstream>
#include <algorithm>
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


int main(int argc, const char* argv[]) {
    
    if (argc < 2) {
		std::cerr << "usage: ./?? dirPath\n";
		std::cerr << "the directory must contain all directories of the different resolution\n";
		return -1;
	}
	
	std::string dirPath = argv[1];
	const std::filesystem::path path{dirPath};
	
	std::map<std::string,std::string> files;
	std::map<std::string,std::map<std::string,std::string>> resolutions;
	
	for (auto const& dir_entry : std::filesystem::directory_iterator{path}){
		if (dir_entry.is_directory()){
			//std::cout<<dir_entry.path().filename()<<"\n";
			for (auto const& file : std::filesystem::directory_iterator{dir_entry.path()}){
				if (file.is_regular_file() && file.path().extension() == ".jp2"){
					//std::cout<<file<<"\n";
					std::filesystem::path filePath = file.path();
					files.insert({extract_band_name(filePath.filename()), file.path()});
				}
				else{
					std::cerr<<"not a valid file or not a .jp2 file \n";
				}
			}
			resolutions.insert({dir_entry.path().filename(), files});
			files.clear();
		}
	}
	
	// visualize all images
	
	for(auto &res : resolutions){
		std::map imgs = res.second;
		for(auto &imgPath : imgs){
			std::string image = imgPath.second;
			std::cout<<image<<"\n";
			
			cv::Mat img = cv::imread(image);
			std::cout<<img.size()<<"\n";
			
			cv::resize(img, img, cv::Size(915, 915), cv::INTER_LINEAR);
			
			cv::imshow( res.first + "/" + imgPath.first , img );
			// Press  ESC on keyboard to exit
			char c=(char)cv::waitKey(0);
			if(c==27) cv::destroyAllWindows();
			
		}
	}
	
	/*
	//access one image
	auto paths = resolutions["R60m"];
	auto imgpath = paths["TCI"];
	std::cout<<imgpath<<"\n";
	cv::Mat img = cv::imread(imgpath);
	cv::resize(img, img, cv::Size(915, 915), cv::INTER_LINEAR);
	cv::imshow( "img" , img );
	// Press  ESC on keyboard to exit
	char c=(char)cv::waitKey(0);
	if(c==27) cv::destroyAllWindows();
	*/
	
	
    return 0;
	
}
