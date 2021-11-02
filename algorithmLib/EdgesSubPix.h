#ifndef __EDGES_SUBPIX_H__
#define __EDGES_SUBPIX_H__
#include <opencv2/opencv.hpp>
#include <vector>

struct Contour
{
    std::vector<cv::Point2f> points;
    std::vector<float> direction;  
    std::vector<float> response;
};
// only 8-bit
CV_EXPORTS void EdgesSubPix(cv::Mat &gray, double alpha, int low, int high,
                            std::vector<Contour> &contours, cv::OutputArray hierarchy,
                            int mode, std::vector<std::vector<cv::Point>> &contours_1);

CV_EXPORTS void EdgesSubPix(cv::Mat &gray, double alpha, int low, int high, 
                           std::vector<Contour> &contours, std::vector<std::vector<cv::Point>> &contours_1);

#endif // __EDGES_SUBPIX_H__
