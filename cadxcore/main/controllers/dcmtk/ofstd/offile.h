/*
 *  
 *  $Id: offile.h $
 *  Ginkgo CADx Project
 *
 *  Code addapted from DCMTK
 *
 *
 *  Copyright (C) 2006-2009, OFFIS
 *
 *  This software and supporting documentation were developed by
 *
 *    Kuratorium OFFIS e.V.
 *    Healthcare Information and Communication Systems
 *    Escherweg 2
 *    D-26121 Oldenburg, Germany
 *
 *  THIS SOFTWARE IS MADE AVAILABLE,  AS IS,  AND OFFIS MAKES NO  WARRANTY
 *  REGARDING  THE  SOFTWARE,  ITS  PERFORMANCE,  ITS  MERCHANTABILITY  OR
 *  FITNESS FOR ANY PARTICULAR USE, FREEDOM FROM ANY COMPUTER DISEASES  OR
 *  ITS CONFORMITY TO ANY SPECIFICATION. THE ENTIRE RISK AS TO QUALITY AND
 *  PERFORMANCE OF THE SOFTWARE IS WITH THE USER.
 *
 *  Module:  ofstd
 *
 *  Author:  Marco Eichelberg
 *
 *  Purpose: C++ wrapper class for stdio FILE functions
 *
 *  Last Update:      $Author: joergr $
 *  Update Date:      $Date: 2009-03-12 11:37:54 $
 *  CVS/RCS Revision: $Revision: 1.8 $
 *  Status:           $State: Exp $
 *
 *  CVS/RCS Log at end of file
 *
 */

#ifndef GINKGOCADX_OFFILE_H
#define GINKGOCADX_OFFILE_H

#include <dcmtk/config/osconfig.h>
#include <dcmtk/ofstd/oftypes.h>    /* for class OFBool */
#include <dcmtk/ofstd/ofstring.h>   /* for class OFString */

#define INCLUDE_UNISTD
#define INCLUDE_CSTDIO
#define INCLUDE_CSTRING
#define INCLUDE_CSTDARG
#define INCLUDE_CERRNO
#define INCLUDE_CWCHAR
#include <dcmtk/ofstd/ofstdinc.h>

BEGIN_EXTERN_C
#ifdef HAVE_SYS_STAT_H
#include <sys/stat.h>   /* needed for struct _stati64 on Win32 */
#endif
END_EXTERN_C

/* When using the ISO C++ include files such as <cstdio>, <cstdarg> etc.,
* all ANSI C functions like fopen() are declared in namespace std,
* (e.g. we have to use std::fopen()), but non-ANSI Posix functions remain
* in global namespace, e.g. we have to use ::fopen64().
* To make things even more difficult, not all compilers really declare
* ANSI C functions in namespace std in accordance with the C++ standard.
* Yes, this is ugly.
*/

/* Find out whether current operating system needs explicit function calls
* to handle large file support
*/
#ifdef _LARGEFILE64_SOURCE
// Mac OSX defines _LARGEFILE64_SOURCE but anyhow expects implicit 64 bit calls
#if !(defined(__MACH__) && defined(__APPLE__))
#define EXPLICIT_LFS_64
#endif
#endif

#ifdef _WIN32
// On Win32 systems, we use Win32 specific definitions
typedef __int64 offile_off_t;
typedef fpos_t offile_fpos_t;
#else
#ifdef EXPLICIT_LFS_64
// Explicit LFS (LFS64)
typedef fpos64_t offile_fpos_t;
typedef off64_t offile_off_t;
#else
// Implicit LFS or no LFS
#ifdef HAVE_FSEEKO
typedef off_t offile_off_t;
#else
typedef long offile_off_t;
#endif
typedef fpos_t offile_fpos_t;
#endif
#endif

// the type we use to store the last error.
typedef int offile_errno_t;

/** this class provides a simple C++ encapsulation layer for stdio FILE pointers.
*  All stdio functions on files are directly mapped into member functions.
*  The handling of large files (64 bit file systems) is transparent. Instead
*  of type off_t, fseek() and ftell() use offile_off_t which is a 64 bit type
*  if available on the underlying platform. Similarly, getpos() and setpos() use
*  type offile_fpos_t, which is defined appropriately.
*  This class provides both of_fclose() and of_pclose(), but these are equivalent -
*  the code always closes pipes with pclose() and files with of_fclose().
*  Finally, an abstraction for errno is provided. Error codes should always
*  be retrieves using methods getLastError() and getLastErrorString() which
*  on Unix platforms are based on errno and strerror/strerror_r, but may be based
*  on other mechanisms on platforms where errno does not exist.
*/
class GinkgoOFFile
{
public:
	/// default constructor, creates an object that is not associated with any file.
	GinkgoOFFile(): file_(NULL), popened_(OFFalse), lasterror_(0) {}

	/** create object for given stdio FILE
	*  @param f stdio FILE
	*/
	GinkgoOFFile(FILE *f): file_(f), popened_(OFFalse), lasterror_(0) {}

	/// destructor. Closes file if still open.
	~GinkgoOFFile()
{
		if (file_) of_fclose();
	}

	/** opens the file whose name is the string pointed to by path and associates
	*  a stream with it.
	*  @param filename path to file
	*  @param modes "r", "w" or "a" with possible modifiers "+", "b"
	*  @return true if stream was successfully created, false otherwise, in which case
	*   the error code is set.
	*/
	OFBool of_fopen(const char *filename, const char *modes)
	{
		if (file_) of_fclose();
#ifdef EXPLICIT_LFS_64
		file_ = ::fopen64(filename, modes);
#else
		file_ = ::fopen(filename, modes);
#endif
		if (file_) popened_ = OFFalse; else of_storeLastError();
		return (file_ != NULL);
	}

#ifdef _WIN32
	/** opens the file whose name is the wide character string pointed to by path and associates
	*  a stream with it. This function is Win32 specific and only exists on WinNT and newer.
	*  @param filename Unicode filename path to file
	*  @param modes "r", "w" or "a" with possible modifiers "+", "b", as a wide character string
	*  @return true if stream was successfully created, false otherwise, in which case the error code is set.
	*/
	OFBool of_wfopen(const wchar_t *filename, const wchar_t *modes)
	{
		if (file_) of_fclose();
		file_ = _wfopen(filename, modes);
		if (file_) popened_ = OFFalse; else of_storeLastError();
		return (file_ != NULL);
	}
#endif

	/** associates a stream with the existing file descriptor, fildes. The mode
	*  of the stream (one of the values "r", "r+", "w", "w+", "a", "a+") must be
	*  compatible with the mode of the file descriptor. The file position
	*  indicator of the new stream is set to that belong� ing to fildes, and the
	*  error and end-of-file indicators are cleared. Modes "w" or "w+" do not
	*  cause truncation of the file. The file descriptor is not dup'ed, and
	*  will be closed when the stream created by fdopen is closed. The result of
	*  applying fdopen to a shared memory object is undefined.
	*  @param fd file descriptor
	*  @param modes "r", "w" or "a" with possible modifiers "+", "b"
	*  @return true if stream was successfully created, false otherwise, in which case the error code is set.
	*/
	OFBool of_fdopen(int fd, const char *modes)
	{
		if (file_) of_fclose();
		file_ = ::fdopen(fd, modes);
		if (file_) popened_ = OFFalse; else of_storeLastError();
		return (file_ != NULL);
	}

	/** opens a process by creating a pipe, forking, and invoking the shell.
	*  Since a pipe is by definition unidirectional, the type argument may
	*  specify only reading or writing, not both; the resulting stream is
	*  correspondingly read-only or write-only. If the object was already
	*  associated with another file or pipe, that one is closed.
	*  @param command shell command line
	*  @param modes "r" or "w"
	*  @return true if pipe was successfully created, false otherwise
	*/
	OFBool of_popen(const char *command, const char *modes)
	{
		if (file_) of_fclose();
#ifdef _WIN32
		file_ = _popen(command, modes);
#else
		file_ = ::popen(command, modes);
#endif
		if (file_) popened_ = OFTrue; else of_storeLastError();
		return (file_ != NULL);
	}

	/** opens the file whose name is the string pointed to by path and associates
	*  the stream pointed maintained by this object with it. The original stream
	*  (if it exists) is closed. The mode argument is used just as in the fopen
	*  function. The primary use of the freopen function is to change the file
	*  associated with a standard text stream (stderr, stdin, or stdout).
	*  @param filename path to file
	*  @param modes "r", "w" or "a" with possible modifiers "+", "b"
	*  @return true if stream was successfully created, false otherwise, in which case the error code is set.
	*/
	OFBool of_freopen(const char *filename, const char *modes)
	{
#ifdef EXPLICIT_LFS_64
		file_ = ::freopen64(filename, modes, file_);
#else
		file_ = ::freopen(filename, modes, file_);
#endif
		if (file_) popened_ = OFFalse; else of_storeLastError();
		return (file_ != NULL);
	}

	/** generates a unique temporary filename. The temporary file is then opened
	*  in binary read/write (w+b) mode. The file will be automatically deleted
	*  when it is closed or the program terminates normally.
	*  @return true if stream was successfully created, false otherwise, in which case the error code is set.
	*/
	OFBool of_tmpfile()
	{
		if (file_) of_fclose();
#ifdef EXPLICIT_LFS_64
		file_ = ::tmpfile64();
#else
		file_ = ::tmpfile();
#endif
		if (file_) popened_ = OFFalse; else of_storeLastError();
		return (file_ != NULL);
	}

	/** dissociates the named stream from its underlying file or set of functions.
	*  If the stream was being used for output, any buffered data is written
	*  first, using fflush. Independent of the return value of this method,
	*  any further access (including another call to of_fclose()) to the stream
	*  maintained by this object results in undefined behaviour.
	*  @return 0 upon success, EOF otherwise, in which case the error code is set.
	*/
	int of_fclose()
	{
		int result = 0;
		if (file_)
		{
			if (popened_)
			{
#ifdef _WIN32
				result = _pclose(file_);
#else
				result = ::pclose(file_);
#endif
			}
			else
			{
				result = ::fclose(file_);
			}
			if (result == 0) file_ = NULL;
		}
		if (result) of_storeLastError();
		return result;
	}

	/** waits for the associated process (created with popen) to terminate and
	*  returns the exit status of the command as returned by wait4.
	*  In this implementation, fclose and pclose can be used synonymously.
	*  @return process ID of the child which exited, or -1 on error, in which case the error code is set
	*/
	int of_pclose() { return of_fclose(); }

	/** writes n elements of data, each size bytes long, to the stream, obtaining
	*  them from the location given by ptr. Returns the number of items successfully written
	*  (i.e., not the number of characters).  If an error occurs the return value is a short
	*  item count (or zero).
	*  @param ptr pointer to buffer
	*  @param size size of item
	*  @param n number of items
	*  @return number of items written
	*/
	size_t of_fwrite(const void *ptr, size_t size, size_t n)
	{
		return ::fwrite(ptr, size, n, file_);
	}

	/** reads n elements of data, each size bytes long, from the stream, storing
	*  them at the location given by ptr. Returns the number of items successfully
	*  read (i.e., not the number of characters).  If an error occurs, or the
	*  end-of-file is reached, the return value is a short item count (or zero).
	*  fread does not distinguish between end-of-file and error, and callers must
	*  use feof and ferror to determine which occurred.
	*  @param ptr pointer to buffer
	*  @param size size of item
	*  @param n number of items
	*  @return number of items read
	*/
	size_t of_fread(void *ptr, size_t size, size_t n)
	{
		return ::fread(ptr, size, n, file_);
	}

	/** forces a write of all user-space buffered data for the given output or
	*  update stream via the stream's underlying write function. The open status
	*  of the stream is unaffected.
	*  @return 0 upon success, EOF otherwise, in which case the error code is set.
	*/
	int of_fflush()
	{
		int result = ::fflush(file_);
		if (result) of_storeLastError();
		return result;
	}

	/** reads the next character from stream and returns it as an unsigned char
	*  cast to an int, or EOF on end of file or error.
	*  @return next character from stream or EOF
	*/
	int of_fgetc() { return ::fgetc(file_); }

	/** The three types of buffering available are unbuffered, block buffered, and
	*  line buffered. When an output stream is unbuffered, information appears on
	*  the destination file or terminal as soon as written; when it is block
	*  buffered many characters are saved up and written as a block; when it is
	*  line buffered characters are saved up until a newline is output or input
	*  is read from any stream attached to a terminal device (typically stdin).
	*  Normally all files are block buffered. if a stream refers to a  terminal
	*  (as stdout normally does) it is line buffered. The standard error  stream
	*  stderr is always unbuffered by default. this function allows to set the
	*  mode of the stream to line buffered.
	*  @return 0 upon success, nonzero otherwise, in which case the error code may be set
	*
	*/
	void of_setlinebuf()
	{
#ifdef _WIN32
		of_setvbuf(NULL, _IOLBF, 0);
#else
		::setlinebuf(file_);
#endif
	}

	/** sets the file position indicator for the stream pointed to by stream to
	*  the beginning of the file. This is equivalent to fseek(0, SEEK_SET)
	*  except that the error indicator for the stream is also cleared.
	*/
	void of_rewind() { ::rewind(file_); }

	/** clears the end-of-file and error indicators for the stream
	*/
	void of_clearerr() { ::clearerr(file_); }

	/** tests the end-of-file indicator for the stream, returning non-zero if it
	*  is set. The end-of-file indicator can only be cleared by the function
	*  clearerr. This method is called eof, not feof, because feof() is a macro
	*  on Win32 and, therefore, cannot be used as a method name.
	*  @return non-zero if EOF, zero otherwise
	*/
	int of_eof() const
	{
#ifdef _WIN32
		// feof is a macro on Win32. Macros never have namespaces.
		return feof(file_);
#else
		return ::feof(file_);
#endif
	}

	/** tests the error indicator for the stream, returning non-zero if it is set.
	*  This method is named error, not ferror, because ferror() is a macro
	*  on Win32 and, therefore, cannot be used as a method name.
	*  The error indicator can only be reset by the clearerr function.
	*  @return non-zero if error flag is set, zero otherwise
	*/
	int of_error()
	{
#ifdef _WIN32
		// ferror is a macro on Win32. Macros never have namespaces.
		return ferror(file_);
#else
		return ::ferror(file_);
#endif
	}

	/** returns the low-level file descriptor associated with the stream
	*  @return low-level file descriptor associated with stream
	*/
	int of_fileno() { return ::fileno(file_); }

	/** The three types of buffering available are unbuffered, block buffered, and
	*  line buffered. When an output stream is unbuffered, information appears on
	*  the destination file or terminal as soon as written; when it is block
	*  buffered many characters are saved up and written as a block; when it is
	*  line buffered characters are saved up until a newline is output or input
	*  is read from any stream attached to a terminal device (typically stdin).
	*  Normally all files are block buffered. if a stream refers to a  terminal
	*  (as stdout normally does) it is line buffered. The standard error  stream
	*  stderr is always unbuffered by default. This function allows to set the
	*  mode of the stream to unbuffered (if buf is NULL) or block buffered.
	*  @param buf pointer to buffer of size BUFSIZ as declared in cstdio, or NULL
	*  @return 0 upon success, nonzero otherwise, in which case the error code may be set
	*/
	void of_setbuf(char *buf) { ::setbuf(file_, buf); }

	/** The three types of buffering available are unbuffered, block buffered, and
	*  line buffered. When an output stream is unbuffered, information appears on
	*  the destination file or terminal as soon as written; when it is block
	*  buffered many characters are saved up and written as a block; when it is
	*  line buffered characters are saved up until a newline is output or input
	*  is read from any stream attached to a terminal device (typically stdin).
	*  Normally all files are block buffered. if a stream refers to a  terminal
	*  (as stdout normally does) it is line buffered. The standard error  stream
	*  stderr is always unbuffered by default. This function allows to set the
	*  stream mode.
	*  @param buf pointer to buffer, may be NULL
	*  @param modes _IONBF (unbuffered) _IOLBF (line buffered) or _IOFBF (fully buffered)
	*  @param n size of buffer, in bytes
	*  @return 0 upon success, nonzero otherwise, in which case the error code may be set
	*/
	int of_setvbuf(char * buf, int modes, size_t n)
	{
		int result = ::setvbuf(file_, buf, modes, n);
		if (result) of_storeLastError();
		return result;
	}

	/** The three types of buffering available are unbuffered, block buffered, and
	*  line buffered. When an output stream is unbuffered, information appears on
	*  the destination file or terminal as soon as written; when it is block
	*  buffered many characters are saved up and written as a block; when it is
	*  line buffered characters are saved up until a newline is output or input
	*  is read from any stream attached to a terminal device (typically stdin).
	*  Normally all files are block buffered. if a stream refers to a  terminal
	*  (as stdout normally does) it is line buffered. The standard error  stream
	*  stderr is always unbuffered by default. This function allows to set the
	*  mode of the stream to unbuffered (if buf is NULL) or block buffered.
	*  @param buf pointer to buffer
	*  @param size size of buffer, in bytes
	*  @return 0 upon success, nonzero otherwise, in which case the error code may be set
	*/
	void of_setbuffer(char *buf, size_t size)
	{
#ifdef _WIN32
		of_setvbuf(NULL, buf ? _IOFBF : _IONBF, size);
#else
		:: setbuffer(file_, buf, size);
#endif
	}

	/** writes the character c, cast to an unsigned char, to stream.
	*  @param c character
	*  @return the character written as an unsigned char cast to an int or EOF on error
	*/
	int of_fputc(int c) { return ::fputc(c, file_); }

	/** reads in at most one less than n characters from stream and stores them
	*  into the buffer pointed to by s. Reading stops after an EOF or a newline.
	*  If a newline is read, it is stored into the buffer. A '\0' is stored after
	*  the last character in the buffer.
	*  @param s pointer to buffer of size n
	*  @param n buffer size
	*  @return pointer to string
	*/
	char* of_fgets(char *s, int n) { return ::fgets(s, n, file_); }

	/** writes the string s to stream, without its trailing '\0'.
	*  @param s string to be written
	*  @return a non-negative number on success, or EOF on error.
	*/
	int of_fputs(const char *s) { return ::fputs(s, file_); }

	/** pushes c back to stream, cast to unsigned char, where it is available for
	*  subsequent read operations. Pushed - back characters will be returned in
	*  reverse order; only one pushback is guaranteed.
	*  @param c character to push back
	*  @return c on success, or EOF on error.
	*/
	int of_ungetc(int c) { return ::ungetc(c, file_); }

	/** sets the file position indicator for the stream pointed to by stream. The
	*  new position, measured in bytes, is obtained by adding offset bytes to the
	*  position specified by whence. If whence is set to SEEK_SET, SEEK_CUR, or
	*  SEEK_END, the offset is relative to the start of the file, the current
	*  position indicator, or end-of-file, respectively. A successful call to the
	*  fseek function clears the end-of- file indicator for the stream and undoes
	*  any effects of the ungetc function on the same stream.
	*  @param off offset to seek to
	*  @param whence SEEK_SET, SEEK_CUR, or SEEK_END
	*  @return 0 upon success, -1 otherwise in which case the error code is set.
	*/
	int of_fseek(offile_off_t off, int whence)
	{
		int result;
#ifdef _WIN32
		// Windows does not have a 64-bit fseek.
		// We emulate fseek through fsetpos, which does exist on Windows.
		// fpos_t is (hopefully always) defined as __int64 on this platform
		offile_fpos_t off2 = off;
		fpos_t pos;
		struct _stati64 buf;
		switch (whence)
		{
		case SEEK_END:
			// flush write buffer, if any, so that the file size is correct
			::fflush(file_);
#if 0
			// Python implementation based on _lseeki64(). May be unsafe because
			// there is no guarantee that fflush also empties read buffers.
			::fflush(file_);
			if (_lseeki64(:: fileno(file_), 0, 2) == -1)
			{
				of_storeLastError();
				return -1;
			}
			// fall through
#else
			// determine file size (using underlying file descriptor). This should be safe.
			if (_fstati64(:: fileno(file_), &buf) == -1)
			{
				of_storeLastError();
				return -1;
			}

			// fsetpos position is offset + file size.
			off2 += buf.st_size;
			break;
#endif
		case SEEK_CUR:
			if (::fgetpos(file_, &pos) != 0)
			{
				of_storeLastError();
				return -1;
			}

			off2 += pos;
			break;
		case SEEK_SET:
			/* do nothing */
			break;
		}
		result = of_fsetpos(&off2);
#elif defined(__BEOS__)
		result = :: _fseek(fp, offset, whence);
#else
#ifdef HAVE_FSEEKO
#ifdef EXPLICIT_LFS_64
		result = ::fseeko64(file_, off, whence);
#else
		result = ::fseeko(file_, off, whence);
#endif
#else
		result = ::fseek(file_, off, whence);
#endif
#endif
		if (result) of_storeLastError();
		return result;
	}

	/** obtains the current value of the file position indicator for the stream pointed to by the stream.
	*  @return current file position
	*/
	offile_off_t of_ftell()
	{
#ifdef _WIN32
		// Windows does not have a 64-bit ftell, and _telli64 cannot be used
		// because it operates on file descriptors and ignores FILE buffers.
		// We emulate ftell through fgetpos, which does exist on Windows.
		// fpos_t is (hopefully always) defined as __int64 on this platform.
		offile_fpos_t pos;
		if (of_fgetpos(&pos) != 0)
		{
			of_storeLastError();
			return -1;
		}
		return pos;
#else
		offile_off_t result;
#ifdef HAVE_FSEEKO
#ifdef EXPLICIT_LFS_64
		result = ::ftello64(file_);
#else
		result = ::ftello(file_);
#endif
#else
		result = ::ftell(file_);
#endif
		if (result < 0) of_storeLastError();
		return result;
#endif
	}

	/** alternate interface equivalent to ftell, storing the current value of the
	*  file offset into the object referenced by pos. On some non-UNIX systems an
	*  fpos_t object may be a complex object and these routines may be the only
	*  way to portably reposition a text stream.
	*  @param pos pointer to offile_fpos_t structure
	*  @return 0 upon success, -1 otherwise in which case the error code is set.
	*/
	int of_fgetpos(offile_fpos_t *pos)
	{
		int result;
#ifdef EXPLICIT_LFS_64
		result = ::fgetpos64(file_, pos);
#else
		result = ::fgetpos(file_, pos);
#endif
		if (result) of_storeLastError();
		return result;
	}

	/** alternate interface equivalent to fseek (with whence set to SEEK_SET),
	*  setting the current value of the file offset from the object referenced by
	*  pos. On some non-UNIX systems an fpos_t object may be a complex object and
	*  these routines may be the only way to portably reposition a text stream.
	*  @param pos pointer to offile_fpos_t structure
	*  @return 0 upon success, -1 otherwise in which case the error code is set.
	*/
	int of_fsetpos(offile_fpos_t *pos)
	{
		int result;
#ifdef EXPLICIT_LFS_64
		result = ::fsetpos64(file_, pos);
#else
		result = ::fsetpos(file_, pos);
#endif
		if (result) of_storeLastError();
		return result;
	}

	/** print formatted string into stream, see printf(3)
	*  @param format format string
	*  @param ... further parameters according to format string
	*  @return number of characters printed
	*/
	int of_fprintf(const char *format, ...)
	{
		int result = 0;
		va_list ap;
		va_start(ap, format);
		result = ::vfprintf(file_, format, ap);
		va_end(ap);
		return result;
	}

	/** print formatted string into stream, see printf(3)
	*  @param format format string
	*  @param arg list of further parameters according to format string
	*  @return number of characters printed
	*/
	int of_vfprintf(const char *format, va_list arg)
	{
		return ::vfprintf(file_, format, arg);
	}

	// we cannot emulate fscanf because we would need vfscanf for this
	// purpose, which does not exist, e.g. on Win32.

	/** return FILE pointer managed by this object. This allows the user
	*  to call some stdio functions that are not encapsulated in this class
	*  (but possibly should be).
	*  @return pointer to FILE structure managed by this object
	*/
	FILE *of_file() { return file_; }

	/** return true if this object is currently associated with a stream, false otherwise
	*  @return true if this object is currently associated with a stream, false otherwise
	*/
	OFBool of_open() const { return file_ != NULL; }

	/** return last error code for this stream
	*  @return last error code for this stream
	*/
	offile_errno_t of_getLastError() const { return lasterror_; }

	/** return string describing last error code for this stream
	*  @param s string describing last error code for this stream returned in this parameter
	*/
	void of_getLastErrorString(OFString& s) const
	{
#ifdef HAVE_PROTOTYPE_STRERROR_R
		char buf[1000];
		buf[0] = 0; // be paranoid and initialize the buffer to empty string.

		// two incompatible interfaces for strerror_r with different return types exist.
#ifdef HAVE_CHARP_STRERROR_R
		// we're using the GNU specific version that returns the result, which may
		// or may not be a pointer to buf
		s = strerror_r(lasterror_, buf, 1000);
#else
		// we're using the X/OPEN version that always stores the result in buf.
		(void) strerror_r(lasterror_, buf, 1000);
		s = buf;
#endif
#else
		// we only have strerror() which is thread unsafe on Posix platforms, but thread safe on Windows.
		s = ::strerror(lasterror_);
#endif
	}

	// Cygwin does not support the wide character functions
#ifndef __CYGWIN__

	/** When mode is zero, the fwide function determines the current orientation
	*  of stream. It returns a value > 0 if stream is wide-character oriented,
	*  i.e.  if wide character I/O is permitted but char I/O is disallowed. It
	*  returns a  value < 0 if stream is byte oriented, i.e. if char I/O is
	*  permitted but wide  character I/O is disallowed. It returns zero if stream
	*  has no orientation yet;  in this case the next I/O operation might change
	*  the orientation (to byte  oriented if it is a char I/O operation, or to
	*  wide-character oriented if it  is a wide character I/O operation).
	*  Once a stream has an orientation, it cannot be changed and persists until
	*  the stream is closed.
	*  When mode is non-zero, the fwide function first attempts to set stream's
	*  orientation (to wide-character oriented if mode > 0, or to byte oriented
	*  if mode < 0). It then returns a value denoting the current orientation, as
	*  above.
	*  @param mode mode of operation for fwide
	*  @return orientation of stream
	*/
	int of_fwide(int mode)
	{
		return ::fwide(file_, mode);
	}

	/** reads a wide character from stream and returns it. If the end of stream is
	*  reached, or if ferror(stream) becomes true, it returns WEOF. If a wide
	*  character conversion error occurs, it sets the error code to EILSEQ and returns
	*  WEOF.
	*  @return next character from stream or WEOF
	*/
	wint_t of_fgetwc()
	{
		wint_t result = ::fgetwc(file_);
		if (result == WEOF) of_storeLastError();
		return result;
	}

	/** writes the wide character wc to stream. If ferror(stream) becomes true, it returns WEOF.
	*  If a wide character conversion error occurs, it sets the error code to EILSEQ and returns WEOF.
	*  Otherwise it returns wc.
	*  @param wc wide character to write to stream
	*  @return character written or WEOF
	*/
	wint_t of_fputwc(wchar_t wc)
	{
		wint_t result = ::fputwc(wc, file_);
		if (result == WEOF) of_storeLastError();
		return result;
	}

	/** pushes back a wide character onto stream and returns it. If wc is WEOF, it
	*  returns WEOF. If wc is an invalid wide character, it sets errno to EILSEQ
	*  and returns WEOF. If wc is a valid wide character, it is pushed back  onto
	*  the stream and thus becomes available for future wide character read
	*  operations. The file-position indicator is decremented by one or more.
	*  The end-of-file indicator is cleared. The backing storage of the file is
	*  not affected. Note: wc need not be the last wide character read from the
	*  stream; it can be any other valid wide character. If the implementation
	*  supports multiple push-back operations in a row, the pushed-back wide
	*  characters will be read in reverse order; however, only one level of
	*  push-back is guaranteed.
	*  @param wc wide character to put back to stream
	*  @return character put back or WEOF
	*/
	wint_t of_ungetwc(wint_t wc)
	{
		wint_t result = ::ungetwc(wc, file_);
		if (result == WEOF) of_storeLastError();
		return result;
	}

	/** print formatted wide string into stream, see wprintf(3)
	*  @param format format string
	*  @param ... further parameters according to format string
	*  @return number of characters printed
	*/
	int of_fwprintf(const wchar_t *format, ...)
	{
		int result = 0;
		va_list ap;
		va_start(ap, format);
		result = ::vfwprintf(file_, format, ap);
		va_end(ap);
		return result;
	}

	/** print formatted wide string into stream, see printf(3)
	*  @param format format string
	*  @param arg list of further parameters according to format string
	*  @return number of characters printed
	*/
	int of_vfwprintf(const wchar_t *format, va_list arg)
	{
		return ::vfwprintf(file_, format, arg);
	}

	// we cannot emulate fwscanf because we would need vfwscanf for this
	// purpose, which does not exist, e.g. on Win32.

#endif /* __CYGWIN__ */

private:

	// private undefined copy constructor
	GinkgoOFFile(const GinkgoOFFile &arg);

	// private undefined assignment operator
	GinkgoOFFile &operator=(const GinkgoOFFile &arg);

	/// the file maintained by this object
	FILE *file_;

	/// a flag indicating whether or not this object was created with popen().
	OFBool popened_;

	/// the last error code for operations of this stream
	offile_errno_t lasterror_;

	/// store last error code. For now we simply store the content of errno.
	inline void of_storeLastError()
	{
		lasterror_ = errno;
	}

};


#endif

/*
* CVS/RCS Log:
* $Log: offile.h,v $
* Revision 1.8  2009-03-12 11:37:54  joergr
* Fixed various Doxygen API documentation issues.
*
* Revision 1.7  2009-02-13 12:56:18  joergr
* Added private undefined copy constructor and assignment operator in order to
* avoid compiler warnings (reported by gcc with additional flags).
*
* Revision 1.6  2009-01-30 13:49:01  joergr
* Replaced checking of macro WIN32 by _WIN32.
*
* Revision 1.5  2008-05-29 10:37:11  meichel
*  Fixed compile error on Cygwin where no wide-char FILE functions are available
*
* Revision 1.4  2008-02-07 16:57:46  meichel
* Class OFFile now makes use of HAVE_CHARP_STRERROR_R to use the
*   correct version of strerror_r.
*
* Revision 1.3  2007/06/06 13:55:58  onken
* Fixed compilation for Mac OS X with making large file support function calls
* implicit for this OS (Mac OS X misleadingly defines _LARGEFILE64_SOURCE).
*
* Revision 1.2  2007/02/19 16:03:47  meichel
* Added constructor to class OFFile that takes FILE * as argument.
*
* Revision 1.1  2006/08/21 12:40:44  meichel
* Added new class OFFile that provides a simple encapsulation layer for
*   FILE based stream I/O and, in particular, provides long file support
*   (LFS) if available on the underlying operating system platform through
*   a single API.
*
*
*/
