#pragma once

#include <opencv2/calib3d.hpp>
#include <opencv2/highgui.hpp>

namespace camera_calibration {

class ChessboardPattern {
   public:
	void create(const char* imageFName, const cv::Size& size);	// From image
	bool isOk() const { return m_CornersFound; }
	void write(const char* outputFName) const;
	void writeImage(const char* outputFName) const;
	void read(const char* fName);  // From txt file

   protected:
	void readImage(const char* imageFName);
	void findChessboardCorners();
	cv::Mat convertToGray(const cv::Mat& image) const;
	void clear();

   private:
	cv::Mat m_Image;
	cv::Size m_Size;
	cv::Mat m_Corners;
	bool m_CornersFound = false;
};

}  // namespace camera_calibration
