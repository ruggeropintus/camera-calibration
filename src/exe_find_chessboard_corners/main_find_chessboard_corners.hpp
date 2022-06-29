#pragma once

#include <camera_calibration/chessboard_pattern.hpp>
#include <exe_find_chessboard_corners/find_chessboard_corners_error_handler.hpp>

using ChessboardPattern = camera_calibration::ChessboardPattern;

void exportChessboardCorners(const char* fName,
							 const ChessboardPattern& chessboardPattern);
void exportChessboardCornersImage(const char* fName,
								  const ChessboardPattern& chessboardPattern);
std::string getOutputFName(const std::string_view& strView,
						   const std::string_view& suffixView,
						   const std::string_view& extView);

static const FindChessboardCornersErrorHandler* errorHandlerPtr = nullptr;

void exportChessboardCorners(const char* fName,
							 const ChessboardPattern& chessboardPattern) {
	const std::string outputFName = getOutputFName(fName, "", "cbc");
	chessboardPattern.write(outputFName.c_str());
}

void exportChessboardCornersImage(const char* fName,
								  const ChessboardPattern& chessboardPattern) {
	std::string outputFName = getOutputFName(fName, "cbc", "jpg");
	chessboardPattern.writeImage(outputFName.c_str());
}

std::string getOutputFName(const std::string_view& strView,
						   const std::string_view& suffixView,
						   const std::string_view& extView) {
	std::string outputFName;
	outputFName.reserve(strView.size() + 1 + suffixView.size() + 1 +
						extView.size());
	outputFName.append(strView.substr(0, strView.find('.')));
	if (suffixView.size() > 0) outputFName.append("_");
	outputFName.append(suffixView);
	outputFName.append(".");
	outputFName.append(extView);
	return outputFName;
}
