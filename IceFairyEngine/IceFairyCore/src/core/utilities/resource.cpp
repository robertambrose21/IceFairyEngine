#include "resource.h"

using namespace IceFairy;

Resource::Resource(std::string filename)
	: filename(filename) {
	std::ifstream file;
	file.open(filename, std::ios::in | std::ios::binary);

	if (file.good()) {
		file.seekg(0, file.end);
		fileLength = (size_t) file.tellg();
		file.close();
	} else {
		file.close();
		throw ResourceDoesNotExistException();
	}
}

void Resource::GetData(char* buffer, size_t dataLength) {
	if (dataLength <= fileLength) {
		std::fstream file;
		file.open(filename, std::ios::in | std::ios::binary);
		file.read(buffer, dataLength);
		buffer[dataLength] = '\0';
		file.close();
	} else {
		throw ResourceBufferTooSmallException();
	}
}

std::string Resource::GetData(void) {
	char* buffer = new char[fileLength + 1];
	GetData(buffer, fileLength);
	std::string data = buffer;
	delete[] buffer;
	return data;
}

std::string Resource::GetFilename(void) {
	return filename;
}

size_t Resource::GetFileLength(void) const {
	return fileLength;
}