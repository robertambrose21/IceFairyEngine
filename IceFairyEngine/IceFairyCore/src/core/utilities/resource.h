#ifndef __ice_fairy_resource_h__
#define __ice_fairy_resource_h__

#include <string>
#include <fstream>

#include "icexception.h"

// TODO: Consider returning data as a char*/const char* rather than a string
namespace IceFairy {
	/*! \brief Thrown when the given resource does not exist. */
	class ResourceDoesNotExistException : public ICException {
	public:
		/*! \internal */
		ResourceDoesNotExistException()
			: ICException("The given resource does not exist.") {
		}
	};

	/*! \brief Thrown when the buffer sized specified in \ref Resource::GetData is not large enough. */
	class ResourceBufferTooSmallException : public ICException {
	public:
		/*! \internal */
		ResourceBufferTooSmallException()
			: ICException("The given buffer is too small for this resource.") {
		}
	};

	/*! \brief Resource class for loading persistant files.
	 *
	 * Sample usage:
	 * \code{.cpp}
	 * IceFairy::Resource file("a file.txt");
	 * size_t len = file.GetFileLength();
	 * char* buffer = new char[len+1];
	 * file.GetData(buffer, len);
	 * IceFairy::Logger("The file '%s' reads: %s\n", file.GetFilename(), buffer);
	 * delete[] buffer;
	 * \endcode
	 */
	class Resource {
	public:
		/*! \brief Opens a resource from a given file.
		*
		* \param filename The file to open
		* \throws ResourceDoesNotExistException
		*/
		Resource(std::string filename);

		/*! \brief Gets data from the resource.
		 *
		 * \param buffer The buffer to allocate the resource data to.
		 * \param dataLength The size of the buffer.
		 * \throws ResourceBufferTooSmallException
		 */
		void        GetData(char* buffer, size_t dataLength);
		/*! \returns All the data in the resource as an STL string. */
		std::string GetData(void);

		/*! \returns The filename of the resource. */
		std::string GetFilename(void);
		/*! \returns The length of the resource file. */
		size_t      GetFileLength(void) const;

	private:
		std::string filename;
		size_t      fileLength;
	};
}

#endif /* __ice_fairy_resource_h__ */