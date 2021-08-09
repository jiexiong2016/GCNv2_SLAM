/*	
 * File: FileModes.h
 * Project: DUtils library
 * Author: Dorian Galvez
 * Date: April 2010
 * Description: types used with file managers
 * License: see the LICENSE.txt file
 *
 */

#pragma once
#ifndef __D_FILE_MODES__
#define __D_FILE_MODES__

namespace DUtils {

/// Opening file modes
enum FILE_MODES {
	READ = 1,
	WRITE = 2,
	APPEND = 4,
	WRITE_APPEND = 6 // == WRITE & APPEND
};

}

#endif

