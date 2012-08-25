#include "skwurly.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>


int param_compare(const void* a, const void* b) {

		return strcmp((*(param*)a).start, (*(param*)b).start);

}

char* url_sort(const char* url){

	char* cursor;
	int len = 0, count = 0;

	// initial scan to see what needs to be done
	while (*url != '\0') {
		// count the string length
		// can use this to get back to first char in url
		++len;

		// count the number of param delimeters
		if (*url == '&') {
			++count;
		}
		// add one for '?' and point cursor to it for future parsing
		else if (*url == '?' && cursor == NULL) {
			cursor = (char*) url;
			++count;
		}

		// next char
		++url;
	}

	// return the url if there is not enough to sort or empty string
	if (count < 2 || len == 0) {
		return (char*) url-len;
	}

	// initialize the params offset array
	param params[count];

	// reuse count to track what param we're on
	count = -1;

	// scan from first '?' to end of string to build params set
	while (*cursor != '\0') {

		// note the start of the param
		if (*cursor == '&' || *cursor == '?') {
			params[++count].start = cursor+1;
			params[count].length = 0;
		}
		// count the length of the param
		else if (count > -1) {
			++params[count].length;
		}

		// next char
		++cursor;
	}

	// sort the array in place in reverse order for
	// easier iteration later on
	qsort(params, count+1, sizeof(param), param_compare);

	// initialize our return value
	char* sorted_url = (char*) malloc(len + 1);

	// restart cursor at beginning of new string
	cursor = sorted_url;

	// write the url up to the first '?'
	// reset pointer to beginning of string
	url -= len;
	while (*url != '?' && *url != '\0') {
		*cursor++ = *url++;
	}
	*cursor++ = '?';

	// build the sorted url
	for (int i = 0; i <= count; ++i) {

		while (params[i].length-- > 0) {

			// write the value
			// advance the cursor
			*cursor++ = *params[i].start++;

		}

		// add the delimiter in manually
		*cursor++ = '&';
	}

	// replace last char (extra &) with null terminator
	*(cursor-1) = '\0';

	return sorted_url;
}




int main(int argc, char** argv){
	char* out = NULL;
	out = url_sort("http://localhost/test?1");
	printf("%s\n",out );
	out = url_sort("http://localhost/test?2&aaa=111");
	printf("%s\n",out );
	out = url_sort("http://localhost/test?ddd=&bbb=222&3&aaa=111");
	printf("%s\n",out );
	out = url_sort("http://localhost/test?ddd=444&bbb=222&ccc=333&aaa=111");
	printf("%s\n",out );
	out = url_sort("http://localhost/test?ccc=333&ccc=111&ddd=444&bbb=222");
	printf("%s\n",out );
	out = url_sort("http://localhost/test?bbb=222&ccc=333&ddd=444&aaa=111");
	printf("%s\n",out );
	out = url_sort("http://localhost/test?bbb=222&ccc=ßß∞§•øø&ddd=444&aaa=111&bbbb=1234&ccccccc=0");
	printf("%s\n",out );
	out = url_sort("http://localhost/test?bbb=222&cbcc=333&ddd=444&dddf=444&aaa=111&bbbb=1234&ccccccc=0&zaaaa=89&ccc=987987");
	printf("%s\n",out );
	return 0;
}
