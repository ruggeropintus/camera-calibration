#include <cstdio>
#include <opencv2/core.hpp>

class FindChessboardCornersErrorHandler {
   private:
	int m_argc;
	const char** m_argv;

   public:
	FindChessboardCornersErrorHandler(int argc, const char** argv);

	void isOpen(const cv::Mat& image) const;
	void areCornersFound(const bool cornersFound) const;

   private:
	void checkArgCount() const;
	void showUsageAndExit() const;
};
