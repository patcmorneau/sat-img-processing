#include <iostream>
#include "opencv2/opencv.hpp"
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <vector>
#include <math.h>
#include <fstream>
#include <algorithm>
#include <filesystem>





int main(int argc, const char* argv[]) {
    
    if (argc < 2) {
		std::cerr << "usage: ./prosatcimg dirPath\n";
		return -1;
	}
	
	std::string dirPath = argv[1];
	const std::filesystem::path path{dirPath};
	
	std::map<std::string,std::string> files;
	
	for (auto const& dir_entry : std::filesystem::directory_iterator{path}) 
    {
        if (dir_entry.is_regular_file()){
			std::filesystem::path filePath = dir_entry.path();
			//std::cout<<filePath.filename()<<"\n";
			files.insert({filePath.filename(), dir_entry.path()});
        }


    }
    /*
    for(auto &el : files){
    	std::cout<<el.first<<" "<<el.second<<"\n";
    }
    */
    
    std::string image = files.begin()->second;
    std::cout<<image<<"\n";
    
    cv::Mat img = cv::imread(image);
	cv::imshow( "Frame", img );
	// Press  ESC on keyboard to exit
	char c=(char)cv::waitKey(0);
	if(c==27) cv::destroyAllWindows();
    
    return 0;
	
}
