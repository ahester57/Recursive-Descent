#ifndef FSATABLE_H_
#define FSATABLE_H_

enum STATE fsatable(const enum STATE state, const char nextchar);
enum STATE operatortable(const int state, const char nextchar);
enum STATE delimtable(const int state, const char nextchar);

#endif