BEGIN: NOOP
	LOAD 0
	STORE T0
	LOAD T0
	STORE x
BLOC0: NOOP
	READ x
LIN0: NOOP
	LOAD 0
	STORE T1
	LOAD x
	STORE T2
	LOAD T2
	SUB T1
	BRZNEG LOUT0
BLOC1: NOOP
	LOAD x
	STORE T3
	WRITE T3
	LOAD 1
	STORE T4
	LOAD x
	STORE T6
	LOAD T6
	SUB T4
	STORE T4
	LOAD T4
	STORE x
	BR LIN0
LOUT0: NOOP
	STOP
	x 0
	T0 0
	T1 0
	T2 0
	T3 0
	T4 0
	T5 0
	T6 0
	T7 0
	T8 0
