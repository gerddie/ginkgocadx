#pragma once
/*
*  
*  $Id: autoptr.h $
*  Ginkgo CADx Project
*
*  Code based on yasper
* ==========================
*
* yasper - A non-intrusive reference counted pointer.
*	    Version: 1.04
*
*  Many ideas borrowed from Yonat Sharon and
*  Andrei Alexandrescu.
*
* (zlib license)
* ----------------------------------------------------------------------------------
* Copyright (C) 2005-2007 Alex Rubinsteyn
*
* This software is provided 'as-is', without any express or implied
* warranty.  In no event will the authors be held liable for any damages
* arising from the use of this software.
*
* Permission is granted to anyone to use this software for any purpose,
* including commercial applications, and to alter it and redistribute it
* freely, subject to the following restrictions:
*
* 1. The origin of this software must not be misrepresented; you must not
*    claim that you wrote the original software. If you use this software
*    in a product, an acknowledgment in the product documentation would be
*    appreciated but is not required.
* 2. Altered source versions must be plainly marked as such, and must not be
*    misrepresented as being the original software.
* 3. This notice may not be removed or altered from any source distribution.
*
* -----------------------------------------------------------------------------------
*
* Send all questions, comments and bug reports to:
* Alex Rubinsteyn (alex.rubinsteyn {at-nospam} gmail {dot} com)
*/
#ifndef AUTOPTR_H
#define AUTOPTR_H
#include <iostream>
#include <exception>
#include <string>
#include <api/api.h>
#include <api/ilock.h>
#include <api/iexception.h>

namespace GNC {

	//----------------------------------------------------------------------------------------------------

	class EXTAPI PtrCounter : public GNC::GCS::ILockable
	{
	public:
		PtrCounter(unsigned c = 1);

		unsigned count;
	};
}

namespace GNC {
	namespace GCS {


		//----------------------------------------------------------------------------------------------------

		class EXTAPI NullPointerException : public GNC::GCS::IException
		{
		public:
			NullPointerException() throw();
			~NullPointerException() throw();
		};


		template <typename X>
		class Ptr : public ILockable
		{

		public:
			typedef X element_type;

			/*
			Ptr needs to be its own friend so Ptr< X > and Ptr< Y > can access
			each other's private data members
			*/
			template <class Y> friend class Ptr;
			/**
			default constructor
			- don't create Counter
			**/
			Ptr() : rawPtr(0), counter(0) { }

			/**
			Construct from a raw pointer
			**/
			Ptr(X* raw, PtrCounter* c = 0) : rawPtr(0), counter(0)
			{
				if (raw)
				{
					if (c) {
						GNC::GCS::ILocker clock(c);
						acquire(c);
						rawPtr = raw;
					}
					else {
						counter = new PtrCounter;
						rawPtr = raw;
					}
				}
			}

			template <typename Y>
			explicit Ptr(Y* raw, PtrCounter* c = 0) : rawPtr(0), counter(0)
			{
				if (raw)
				{
					if (c) {
						GNC::GCS::ILocker clock(c);
						acquire(c);
						rawPtr = static_cast<X*>( raw );
					}
					else {
						rawPtr = static_cast<X*>( raw );
						counter = new PtrCounter;
					}
				}
			}


			/*
			Copy constructor
			*/
			Ptr(const Ptr< X >& otherPtr)
			{
				GNC::GCS::ILocker olock((Ptr<X>*) &otherPtr);

				if (otherPtr.counter) {
					GNC::GCS::ILocker clock(otherPtr.counter);
					acquire( otherPtr.counter );
					rawPtr = otherPtr.rawPtr;
				}
				else {
					counter = NULL;
					rawPtr = NULL;
				}
			}

			template <typename Y>
			explicit Ptr(const Ptr< Y >& otherPtr) : rawPtr(0), counter(0)
			{
				GNC::GCS::ILocker olock( (Ptr<Y>*) &otherPtr );

				if (otherPtr.counter) {
					GNC::GCS::ILocker clock(otherPtr.counter);

					acquire(otherPtr.counter);
					rawPtr = static_cast<X*>( otherPtr.rawPtr );
				}
			}


			/*
			Destructor
			*/
			~Ptr()
			{			
				GNC::GCS::ILocker lock(this);
				release();
			}

			/*
			Assignment to another Ptr
			*/

			Ptr& operator=(const Ptr< X >& otherPtr)
			{						

				if (this != &otherPtr)
				{
					release();

					if (otherPtr.counter) {
						GNC::GCS::ILocker cLock(otherPtr.counter);
						acquire(otherPtr.counter);
						rawPtr = static_cast<X*> (otherPtr.rawPtr);
					}
					else {
						rawPtr = NULL;
						counter = NULL;
					}


				}

				return *this;
			}

			template <typename Y>
			Ptr& operator=(const Ptr< Y >& otherPtr)
			{
				GNC::GCS::ILocker lock(this);
				GNC::GCS::ILocker olock((Ptr<Y>*) &otherPtr);

				if ( this != (Ptr< X >*) &otherPtr )
				{
					release();

					if (otherPtr.counter) {
						GNC::GCS::ILocker clock (otherPtr.counter);

						acquire(otherPtr.counter);
						rawPtr = static_cast<X*> (otherPtr.rawPtr);
					}
				}
				return *this;
			}

			/*
			Assignment to raw pointers is really dangerous business.
			If the raw pointer is also being used elsewhere,
			we might prematurely delete it, causing much pain.
			Use sparingly/with caution.
			*/

			Ptr& operator=(X* raw)
			{
				GNC::GCS::ILocker lock(this);
				if (raw)
				{
					release();

					PtrCounter* c = new PtrCounter;
					GNC::GCS::ILocker lock(c);

					counter = c;
					rawPtr = raw;

				}
				return *this;
			}

			template <typename Y>
			Ptr& operator=(Y* raw)
			{
				GNC::GCS::ILocker lock(this);
				if (raw)
				{
					release();

					PtrCounter* c = new PtrCounter();

					GNC::GCS::ILocker lock(c);

					counter = c;
					rawPtr = static_cast<X*>(raw);

				}
				return *this;
			}

			/*
			assignment to long to allow Ptr< X > = NULL,
			also allows raw pointer assignment by conversion.
			Raw pointer assignment is really dangerous!
			If the raw pointer is being used elsewhere,
			it will get deleted prematurely.
			*/
			Ptr& operator=(long num)
			{
				GNC::GCS::ILocker lock(this);
				if (num == 0)  //pointer set to null
				{
					release();
				}

				else //assign raw pointer by conversion
				{
					release();
					PtrCounter* c = new PtrCounter();

					GNC::GCS::ILocker lock(c);

					counter = c;
					rawPtr = reinterpret_cast<X*>(num);

				}

				return *this;
			}

			/*
			Member Access
			*/
			X* operator->() const
			{
				return GetRawPointer();
			}


			/*
			Dereference the pointer
			*/
			X& operator* () const
			{
				return *GetRawPointer();
			}


			/*
			Conversion/casting operators
			*/


			operator bool() const
			{
				return IsValid();
			}


			/*
			implicit casts to base types of the
			the pointer we're storing
			*/

			template <typename Y>
			operator Y*() const
			{
				return static_cast<Y*>(rawPtr);
			}

			template <typename Y>
			operator const Y*() const
			{
				return static_cast<const Y*>(rawPtr);
			}

			template <typename Y>
			operator Ptr<Y>()
			{
				//new Ptr must also take our counter or else the reference counts
				//will go out of sync
				return Ptr<Y>(rawPtr, counter);
			}


			/*
			Provide access to the raw pointer
			*/

			X* GetRawPointer() const
			{
				X* r = NULL;

				r = rawPtr;

				if (r == 0) {
					throw new GNC::GCS::NullPointerException;
				}

				return r;
			}


			/*
			Is there only one reference on the counter?
			*/
			bool IsUnique() const
			{
				bool unique = false;

				if (counter) {
					unique = (counter->count == 1);
				}
				else {
					unique = true;
				}

				return unique;
			}

			bool IsValid() const
			{
				bool valid = false;

				if (counter) {
					valid = (rawPtr != NULL);
				}

				return valid;
			}

			unsigned GetCount() const
			{
				int count = 0;

				if (counter) {
					count = counter->count;
				}
				return 0;
			}

		private:
			X* rawPtr;

			PtrCounter* counter;

			// increment the count
			void acquire(PtrCounter* c)
			{
				counter = c;
				(c->count)++;
			}

			// decrement the count, delete if it is 0
			void release()
			{
				if (counter)
				{
					PtrCounter* c = counter;

					bool doDelete = false;

					X* r = NULL;

					{
						GNC::GCS::ILocker lock(c);

						r = rawPtr;

						(c->count)--;

						if (c->count == 0)
						{
							counter = NULL;
							rawPtr = NULL;

							doDelete = true;
						}
					}

					if (doDelete) {
						delete c;
						delete r;
					}
				}

			}
		};


		template <typename X, typename Y>
		bool operator==(const Ptr< X >& lPtr, const Ptr< Y >& rPtr)
		{
			return lPtr.GetRawPointer() == rPtr.GetRawPointer();
		}

		template <typename X, typename Y>
		bool operator==(const Ptr< X >& lPtr, Y* raw)
		{
			return lPtr.GetRawPointer() == raw ;
		}

		template <typename X>
		bool operator==(const Ptr< X >& lPtr, long num)
		{
			if (num == 0 && !lPtr.IsValid())  //both pointer and address are null
			{
				return true;
			}

			else //convert num to a pointer, compare addresses
			{
				return lPtr == reinterpret_cast<X*>(num);
			}

		}

		template <typename X, typename Y>
		bool operator!=(const Ptr< X >& lPtr, const Ptr< Y >& rPtr)
		{
			return ( !operator==(lPtr, rPtr) );
		}

		template <typename X, typename Y>
		bool operator!=(const Ptr< X >& lPtr, Y* raw)
		{
			return ( !operator==(lPtr, raw) );
		}

		template <typename X>
		bool operator!=(const Ptr< X >& lPtr, long num)
		{
			return (!operator==(lPtr, num) );
		}

		template <typename X, typename Y>
		bool operator&&(const Ptr< X >& lPtr, const Ptr< Y >& rPtr)
		{
			return lPtr.IsValid() &&  rPtr.IsValid();
		}

		template <typename X>
		bool operator&&(const Ptr< X >& lPtr, bool rval)
		{
			return lPtr.IsValid() && rval;
		}

		template <typename X>
		bool operator&&(bool lval, const Ptr< X >& rPtr)
		{
			return lval &&  rPtr.IsValid();
		}

		template <typename X, typename Y>
		bool operator||(const Ptr< X >& lPtr, const Ptr< Y >& rPtr)
		{
			return lPtr.IsValid() || rPtr.IsValid();
		}

		template <typename X>
		bool operator||(const Ptr< X >& lPtr, bool rval)
		{
			return lPtr.IsValid() || rval;
		}

		template <typename X>
		bool operator||(bool lval, const Ptr< X >& rPtr)
		{
			return lval || rPtr.IsValid();
		}

		template <typename X>
		bool operator!(const Ptr< X >& p)
		{
			return (!p.IsValid());
		}


		/* less than comparisons for storage in containers */
		template <typename X, typename Y>
		bool operator< (const Ptr< X >& lPtr, const Ptr < Y >& rPtr)
		{
			return lPtr.GetRawPointer() < rPtr.GetRawPointer();
		}

		template <typename X, typename Y>
		bool operator< (const Ptr< X >& lPtr, Y* raw)
		{
			return lPtr.GetRawPointer() < raw;
		}

		template <typename X, typename Y>
		bool operator< (X* raw, const Ptr< Y >& rPtr)
		{
			return raw < rPtr.GetRawPointer();
		}
	}
}
#endif
