#include <cstdio>
#include <opencv2/core.hpp>

class FindChessboardCornersErrorHandler {
   private:
	int m_argc;
	const char** m_argv;

   public:
	FindChessboardCornersErrorHandler(int argc, const char** argv);

	void isChessboardPatternOK(const bool isOk) const;

   private:
	void checkArgCount() const;
	void showUsageAndExit() const;
};
