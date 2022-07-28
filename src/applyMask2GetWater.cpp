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

void get_pixel_value(int action, int x, int y, int flags, void *userdata){
	
	if( action == cv::EVENT_LBUTTONDOWN ){
		cv::Mat*img = (cv::Mat*)userdata;
		std::cout<< x << " " << y <<"\n";
		int val = img->at<uchar>(y, x);
		std::cout<<val<<"\n";
		
	}
	
}


int main(int argc, const char* argv[]) {
    
    if (argc < 2) {
		std::cerr << "usage: ./getWater dirPath\n";
		std::cerr << "the directory must contain all directories of the different resolution\n";
		return -1;
	}
	
	std::string dirPath = argv[1];
	Sentinel sentinel(dirPath);
	auto resolutions = sentinel.get_all_files();
	
	
	//access images
	auto paths = resolutions["R60m"];
	std::string sclPath = paths["SCL"];
	std::string b04Path = paths["B04"];
	
	cv::Mat b04 = cv::imread(b04Path, cv::IMREAD_LOAD_GDAL);

	
	cv::Mat scl = cv::imread(sclPath, cv::IMREAD_LOAD_GDAL);
	double min, max;
	cv::minMaxLoc(scl, &min, &max);
	
	/*
	std::cout<<"scl min max "<< min<<"  "<<max<<"\n"; // each pixel is classified between 0 and 11 
	std::cout<<"scl type : "<<type2str(scl.type())<<"\n";
	std::cout<<"b04 type : "<<type2str(b04.type())<<"\n";
	double b04Max, b04Min;
	cv::minMaxLoc(b04, &b04Min, &b04Max);
	std::cout<<"b04 min max "<< b04Min<<"  "<<b04Max<<"\n";
	*/
	
	//TODO get size from image and make sure they are the same
	cv::Mat mask(cv::Size(1830, 1830), CV_8UC1, cv::Scalar(0));
	for(int row = 0; row < scl.rows; ++row){
		for(int col = 0; col < scl.cols; ++col){
			if(scl.at<uchar>(row, col, 0) == 6){ // 
				mask.at<uchar>(row, col) = 255;
			}
			else{
				mask.at<uchar>(row, col) = 0;
			}
		}
	}
	mask.convertTo(mask, CV_16UC1);
	
	cv::Mat result(cv::Size(1830, 1830), CV_16UC1, cv::Scalar(0));
	
	cv::bitwise_and(b04, mask, result);
	
	result.convertTo(result, CV_8UC1);
	
	//cv::namedWindow("mask");
	cv::resize(result, result, cv::Size(915, 915), cv::INTER_LINEAR);
	cv::imshow( "result" , result );
	//cv::setMouseCallback("mask", get_pixel_value, &mask);
	
	cv::resize(mask, mask, cv::Size(915, 915), cv::INTER_LINEAR);
	cv::imshow( "mask" , mask );
	
	cv::resize(b04, b04, cv::Size(915, 915), cv::INTER_LINEAR);
	cv::imshow( "b04" , b04 );
	
	cv::resize(scl, scl, cv::Size(915, 915), cv::INTER_LINEAR);
	cv::imshow( "scl" , scl );
	
	// Press  ESC on keyboard to exit
	char c=(char)cv::waitKey(0);
	if(c==27) cv::destroyAllWindows();
	
    return 0;
	
}
