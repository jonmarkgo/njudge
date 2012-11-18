/*
 * datetime.h
 *
 *  Created on: Nov 18, 2012
 *      Author: cjw
 */

#ifndef DATETIME_H_
#define DATETIME_H_

time_t datetime_epoc_from_string(const char* str);
time_t datetime_epoc_utc(time_t time);

#endif /* DATETIME_H_ */
