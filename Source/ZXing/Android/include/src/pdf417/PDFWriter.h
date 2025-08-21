/*
* Copyright 2016 Huy Cuong Nguyen
* Copyright 2016 ZXing authors
*/
// SPDX-License-Identifier: Apache-2.0

#pragma once

#include "CharacterSet.h"

#include <string>
#include <memory>

namespace ZXing {

class BitMatrix;

namespace Pdf417 {

enum class Compaction;
class Encoder;

/**
* @author Jacob Haynes
* @author qwandor@google.com (Andrew Walbran)
*/
class Writer
{
public:
	Writer();
	Writer(Writer &&) noexcept;
	~Writer();

	Writer& setMargin(int margin) { _margin = margin; return *this; }

	Writer& setErrorCorrectionLevel(int ecLevel) { _ecLevel = ecLevel; return *this; }

	/**
	* Sets max/min row/col values
	*
	*  maxCols maximum allowed columns
	*  minCols minimum allowed columns
	*  maxRows maximum allowed rows
	*  minRows minimum allowed rows
	*/
	Writer& setDimensions(int minCols, int maxCols, int minRows, int maxRows);

	/**
	*  compaction compaction mode to use
	*/
	Writer& setCompaction(Compaction compaction);

	/**
	*  compact if true, enables compaction
	*/
	Writer& setCompact(bool compact);

	/**
	*  encoding sets character encoding to use
	*/
	Writer& setEncoding(CharacterSet encoding);


	BitMatrix encode(const std::wstring& contents, int width, int height) const;
	BitMatrix encode(const std::string& contents, int width, int height) const;

private:
	int _margin = -1;
	int _ecLevel = -1;
	std::unique_ptr<Encoder> _encoder;
};

} // Pdf417
} // ZXing
