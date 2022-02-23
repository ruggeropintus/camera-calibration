#include <cstdio>
#include <fstream>

class CalibrateCameraErrorHandler {
   private:
	int m_argc;
	const char** m_argv;

   public:
	CalibrateCameraErrorHandler(int argc, const char** argv);

	void isOpen(const std::fstream& ifs) const;

   private:
	void checkArgCount() const;
	void checkInputArgs() const;
	void showUsageAndExit() const;
};
