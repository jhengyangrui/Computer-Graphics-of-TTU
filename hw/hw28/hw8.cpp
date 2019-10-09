#include "svl/svl.h"
#include <iostream>
#include <stdio.h>
#pragma comment (lib, "svl-dbg.lib")

#include <windows.h>
LARGE_INTEGER freq, before, after;
void main()
{
	Mat m,q;
	int i, j;

	int dim[5] = {100,50,100,5,10};
	int time[5];

	QueryPerformanceFrequency (&freq);

	for (int ii = 0; ii < 5; ii++){
		int size = dim[ii];

		m.SetSize(size , size);
		for (i = 0; i < m.Rows(); i++ )
			for (j = 0; j < m.Cols(); j++ )
				m[i][j] = i+j;

			q = m;
	QueryPerformanceCounter (&before);
	m = m *q;
	QueryPerformanceCounter(&after);

	time[ii] = ((double)(after.QuadPart - before.QuadPart))/freq.QuadPart;
	}
	for (ii = 0; ii < 5; ii++)
		printf("%e\n",time[ii]);
}
