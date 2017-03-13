#ifndef EASY_H
#define EASY_H
#define SafeDeletePtr(ptr) do{  \
	if (ptr) {                  \
		delete ptr;             \
		ptr = nullptr;          \
	}                           \
} while(0);
#define SafeDeleteArray(ptr) do {   \
	if (ptr) {                      \
		delete [] ptr;              \
		ptr = nullptr;              \
	}                               \
} while(0);
#define ArrayLen(array) (int)(sizeof(array) / sizeof(array[0]))

#endif // EASY_H
