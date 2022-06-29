#include <iostream>
#include "opencv2/opencv.hpp"
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <vector>
#include <math.h>
#include <fstream>
#include <algorithm>
#include <filesystem>

std::string type2str(int type) {
  std::string r;

  uchar depth = type & CV_MAT_DEPTH_MASK;
  uchar chans = 1 + (type >> CV_CN_SHIFT);

  switch ( depth ) {
    case CV_8U:  r = "8U"; break;
    case CV_8S:  r = "8S"; break;
    case CV_16U: r = "16U"; break;
    case CV_16S: r = "16S"; break;
    case CV_32S: r = "32S"; break;
    case CV_32F: r = "32F"; break;
    case CV_64F: r = "64F"; break;
    default:     r = "User"; break;
  }

  r += "C";
  r += (chans+'0');

  return r;
}


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

/*
void to_same_resolution(std::vector<std::string>> paths){
//TODO
}
*/



int main(int argc, const char* argv[]) {
    
    if (argc < 2) {
		std::cerr << "usage: ./process dirPath\n";
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
	
	
	//access one image
	auto paths = resolutions["R60m"];
	auto b03Path = paths["B03"];
	auto b12Path = paths["B12"];
	
	cv::Mat b03T = cv::imread(b03Path, 0);
	cv::Mat b12T = cv::imread(b12Path, 0);
	
	cv::Mat b03;
	cv::Mat b12;
	
	b03T.convertTo(b03, CV_32FC1);
	b12T.convertTo(b12, CV_32FC1);
	
	std::cout<<"B03 : " << b03.dims<<"\n";
	std::cout<<type2str(b03.type())<<"\n";
	double min, max;
	cv::minMaxLoc(b03, &min, &max);
	std::cout<<"min max "<< min<<"  "<<max<<"\n";
	
	std::cout<<"B12 : " << b12.dims<<"\n";
	std::cout<<type2str(b12.type())<<"\n";
	cv::minMaxLoc(b12, &min, &max);
	std::cout<<"min max "<< min<<"  "<<max<<"\n";
	

	cv::Mat MNDWI = (b03 - b12) / (b03 + b12);
	std::cout<<"MNDWI : " << MNDWI.dims<<"\n";
	cv::minMaxLoc(MNDWI, &min, &max);
	std::cout<<"min max "<< min<<"  "<<max<<"\n";
	
	cv::Mat mask(cv::Size(1830, 1830), CV_8UC1, cv::Scalar(0));
	
	for(int row = 0; row < MNDWI.rows; ++row){
		for(int col = 0; col < MNDWI.cols; ++col){
			float pixel = MNDWI.at<float>(row, col, 0);
			if(pixel > 0){
				mask.at<uchar>(row, col) = 255;
			}
		}
	}
	
	
	
	cv::resize(mask, mask, cv::Size(915, 915), cv::INTER_LINEAR);
	cv::imshow( "mask" , mask );
	// Press  ESC on keyboard to exit
	char c=(char)cv::waitKey(0);
	if(c==27) cv::destroyAllWindows();

    return 0;
	
}
