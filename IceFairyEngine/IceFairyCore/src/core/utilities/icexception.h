#ifndef __ice_fairy_exception_h__
#define __ice_fairy_exception_h__

#define ICE_FAIRY_UNDEFINED_EXCEPTION "Undefined ICException."

#include <string>

namespace IceFairy {
	/*! \brief Base exception class for entire IceFairy application. */
	class ICException : public std::exception {
	public:
		/*! \brief Construct the ICException with the default undefined messsage. */
		ICException();
		/*! \brief Construct the ICException.
		 *
		 * \param message The message to supply to the user when the exception is thrown.
		 */
		ICException(std::string message);
		virtual ~ICException();

		/*! \internal */
		virtual const char* what() const throw();

	private:
		std::string message;
	};
}

#endif /* __ice_fairy_exception_h__ */