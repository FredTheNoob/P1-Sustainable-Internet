#include <stdio.h>

#include "CuTest.h"

//CuSuite* StrUtilGetSuite();
CuSuite* simGetSuite();
CuSuite* websiteGetSuite();

void RunAllTests(void)
{
	CuString *output = CuStringNew();
	CuSuite* suite = CuSuiteNew();

    CuSuiteAddSuite(suite, simGetSuite());
    CuSuiteAddSuite(suite, websiteGetSuite());

	CuSuiteRun(suite);
	CuSuiteSummary(suite, output);
	CuSuiteDetails(suite, output);
	printf("%s\n", output->buffer);
}

int main(void)
{
	RunAllTests();
}
