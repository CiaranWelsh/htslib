//
// Created by Ciaran on 26/11/2020.
//

#ifndef HTSLIB_CMAKE_SSIZE_T_TYPEDEF_H
#define HTSLIB_CMAKE_SSIZE_T_TYPEDEF_H

/**
 * On windows we do not have the ssize_t typedef. So we define it here
 * as per https://stackoverflow.com/questions/34580472/alternative-to-ssize-t-on-posix-unconformant-systems
 */

#ifndef HAVE_SSIZE_T // new configured variable defined for port to windows

#if defined(_MSC_VER)
//typedef ssize_t __int64 ;
#endif

#endif //HAVE_SSIZE_T

#endif //HTSLIB_CMAKE_SSIZE_T_TYPEDEF_H
