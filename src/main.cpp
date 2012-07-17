#include <opencv2/highgui/highgui.hpp>
#include <opencv2/core/core.hpp>
#include <cv.h>

#include <iostream>
#include <fstream>
#include <string>


int  g_slider_position = 0;

cv::VideoCapture g_capture;

using namespace std;


void onTrackbarSlide(int pos, void*){
    g_capture.set( CV_CAP_PROP_POS_FRAMES, pos );
}

int main( int argc, char** argv ){

    std::string videoName, videoOut;

    if( argc < 2 ){
        std::cout << "To use this program type in your shell:\n./VidExtract <InputVideo>" << std::endl;
        return 1;
    }
    else if( argc == 2 )
    {
        videoOut = "extractedVideo.asf";
    }
    else if( argc == 3 )
    {
        videoOut = argv[2];
    }
    else
    {
        std::cout << "To use this program type in your shell:\n./VidExtract <InputVideo> <OutputVideo>" << std::endl;
        return 1;
    }

    char code = (char)-1;

    videoName = argv[1];
    g_capture = videoName;
    cv::Mat frame, frame_out;
    int frameWidth  = (int)(g_capture.get(CV_CAP_PROP_FRAME_WIDTH));
    int frameHeight = (int)(g_capture.get(CV_CAP_PROP_FRAME_HEIGHT));
    int frames      = (int)(g_capture.get(CV_CAP_PROP_FRAME_COUNT));

    cv::VideoWriter output( videoOut, CV_FOURCC( 'D', 'I', 'V', '4' ), 10., cv::Size(frameWidth, frameHeight), true);

    cv::namedWindow( videoName, CV_WINDOW_AUTOSIZE );

    if( frames != 0 )
        cv::createTrackbar( "Position", videoName, &g_slider_position, frames, onTrackbarSlide, NULL);

    std::cout << "Left-click on the video, and press r/R to start the extraction, or q/Q to quit" << std::endl;

    bool recording = false;

    while(1) {
        g_capture >> frame;

        if(frame.empty()) break;

        g_capture >> frame_out;

        if( recording )   output << frame_out;

        std::stringstream ss;
        ss << "Frame " << g_slider_position << "/" << frames;
        cv::putText( frame, (ss.str()), cv::Point(0,20), CV_FONT_HERSHEY_COMPLEX_SMALL, 1, cv::Scalar(0,0,0), 2, CV_AA );
        cv::putText( frame, (ss.str()), cv::Point(0,20), CV_FONT_HERSHEY_COMPLEX_SMALL, 1, cv::Scalar(255,255,255), 1, CV_AA );
        cv::imshow( videoName, frame );
        cv::setTrackbarPos( "Position", videoName, ++g_slider_position );

        code = (char)cv::waitKey(30);
        if( code == 27 || code == 'q' || code == 'Q' ) break;
        if( code == 'r' || code == 'R' )
        {
            recording = true;
            std::cout << "Recording, press s/S to stop" << std::endl;
        }
        if( code == 's' || code == 'S' )
        {
            recording = false;
            std::cout << "Stop recording" << std::endl;
        }
    }
    return 0;
}

