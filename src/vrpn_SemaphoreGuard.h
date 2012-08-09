/** @file
	@brief Header

	@date 2012

	@author
	Ryan Pavlik
	<rpavlik@iastate.edu> and <abiryan@ryand.net>
	http://academic.cleardefinition.com/
	Iowa State University Virtual Reality Applications Center
	Human-Computer Interaction Graduate Program

*/

//           Copyright Iowa State University 2012.
//  Distributed under the Boost Software License, Version 1.0.
//     (See accompanying file LICENSE_1_0.txt or copy at
//           http://www.boost.org/LICENSE_1_0.txt)

#pragma once
#ifndef INCLUDED_vrpn_SemaphoreGuard_h_GUID_123751CE_5F4A_449D_3840_FF8B1008B4E2
#define INCLUDED_vrpn_SemaphoreGuard_h_GUID_123751CE_5F4A_449D_3840_FF8B1008B4E2


// Internal Includes
// - none

// Library/third-party includes
#include <vrpn_Shared.h>

// Standard includes
#include <stdexcept>

class vrpn_SemaphoreGuard {
	public:
		vrpn_SemaphoreGuard(vrpn_Semaphore & sem)
			: _sem(sem)
			, _acquired(false) {}

		bool acquire() {
			if (_acquired) {
				throw std::logic_error("Already acquired");
			}
			int ret = _sem.p();
			if (ret == 1) {
				_acquired = true;
				return true;
			} else {
				throw std::runtime_error("Fail in vrpn_Semaphore::p()");
			}
		}

		bool tryAcquire() {
			if (_acquired) {
				throw std::logic_error("Already acquired");
			}
			int ret = _sem.condP();
			if (ret == 1) {
				_acquired = true;
				return true;
			} else if (ret == 0) {
				return false;
			} else {
				throw std::runtime_error("Fail in vrpn_Semaphore::condP()");
			}
		}

		void release() {
			if (_acquired) {
				int ret = _sem.v();
				if (ret == 0) {
					_acquired = false;
				} else {
					throw std::runtime_error("Fail in vrpn_Semaphore::v()");
				}
			} else {
				throw std::logic_error("Can't release - not acquired");
			}
		}

		bool haveAcquired() const {
			return _acquired;
		}

		~vrpn_SemaphoreGuard() {
			if (_acquired) {
				try {
					release();
				} catch (std::exception &) {
					/// @todo How to signal failure here?
				}
			}
		}
	private:
		vrpn_Semaphore & _sem;
		bool _block;
		bool _acquired;

};

#endif // INCLUDED_vrpn_SemaphoreGuard_h_GUID_123751CE_5F4A_449D_3840_FF8B1008B4E2

