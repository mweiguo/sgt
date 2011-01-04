/* $HeadURL: http://192.168.10.26/svn/svnsw/trunk/sw/primace/sgrenderer/sgr_global.h $ $Revision: 12504 $ $Author: wgmao $ */
/**
 * Copyright (c) 2005-2010 Agate Logic, Inc.  All rights reserved.
 * 
 * Agate Logic, Inc. Confidential.
 * 
 * No part of this code may be reproduced, distributed, transmitted,
 * transcribed, stored in a retrieval system, or translated into any
 * human or computer language, in any form or by any means, electronic,
 * mechanical, magnetic, manual, or otherwise, without the express
 * written permission of Agate Logic, Inc.
 */
#ifndef SGR_GLOBAL_H
#define SGR_GLOBAL_H

/* #include <Qt/qglobal.h> */

/* //#ifdef WIN32 */
/* #ifdef SGR_EXPORT */
/* # define SGR_DLL Q_DECL_EXPORT */
/* #else */
/* # define SGR_DLL Q_DECL_IMPORT */
/* #endif */
/* //#else */
/* //# define SGR_DLL */
/* //#endif */

#ifdef _MSC_VER //MSVC compiler

  #ifdef SGR_EXPORT
    #define SGR_DLL __declspec(dllexport)
  #else
    #define SGR_DLL __declspec(dllimport)
  #endif //  SGR_EXPORT

#else // _MSC_VER

  #define SGR_DLL

#endif



#endif // GVF_GLOBAL_H
