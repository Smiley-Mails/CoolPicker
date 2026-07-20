/*/////////////////////////////////////////////////////////////////
 Functions to return the minimum or maximum of three data types.

 Classes passed to these functions must have implemented
 the < and > operator (i.e. less than and greater than operator)
/////////////////////////////////////////////////////////////////*/
#ifndef _min_max_
#define _min_max_

template<class T> T Minimum(T a, T b, T c);
template<class T> T Maximum(T a, T b, T c);


#endif