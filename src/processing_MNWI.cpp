#include "SENTINEL.hpp"

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

int main(int argc, const char* argv[]) {
    
    if (argc < 2) {
		std::cerr << "usage: ./process dirPath\n";
		std::cerr << "the directory must contain all directories of the different resolution\n";
		return -1;
	}
	
	std::string dirPath = argv[1];
	Sentinel sentinel(dirPath);
	auto resolutions = sentinel.get_all_files();
	
	
	//access one image
	auto paths = resolutions["R60m"];
	auto b03Path = paths["B03"];
	auto b12Path = paths["B12"];
	
	cv::Mat b03 = cv::imread(b03Path, 0);
	cv::Mat b12 = cv::imread(b12Path, 0);
	
	
	
	/*
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
	
	//TODO get size from image
	cv::Mat mask(cv::Size(1830, 1830), CV_8UC1, cv::Scalar(0));
	
	for(int row = 0; row < MNDWI.rows; ++row){
		for(int col = 0; col < MNDWI.cols; ++col){
			float pixel = MNDWI.at<float>(row, col, 0);
			if(pixel > 0){
				mask.at<uchar>(row, col) = 255;
			}
		}
	}
	*/
	
	cv::Mat mask = sentinel.generate_MNDWI_mask(b03, b12);
	
	cv::resize(mask, mask, cv::Size(915, 915), cv::INTER_LINEAR);
	cv::imshow( "mask" , mask );
	// Press  ESC on keyboard to exit
	char c=(char)cv::waitKey(0);
	if(c==27) cv::destroyAllWindows();

    return 0;
	
}
