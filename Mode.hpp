#ifndef HPP_MODE
#define HPP_MODE

#include <string>

#if defined(__APPLE__) || defined(__MACOSX)
#include <OpenCL/cl.h>
#else
#include <CL/cl.h>
#endif

enum HashTarget {
	ADDRESS,
	CONTRACT,
	HASH_TARGET_COUNT
};

class Mode {
	private:
		Mode();

	public:
		static Mode matching(bool tron, const std::string strHex);
		static Mode range(bool tron, const cl_uchar min, const cl_uchar max);
		static Mode leading(bool tron, const char charLeading);
		static Mode leadingRange(bool tron, const cl_uchar min, const cl_uchar max);
		static Mode mirror(bool tron);

		static Mode benchmark(bool tron);
		static Mode zeros(bool tron);
		static Mode zeroBytes(bool tron);
		static Mode letters(bool tron);
		static Mode numbers(bool tron);
		static Mode doubles(bool tron);

		std::string name;

		std::string kernel;

		HashTarget target;
		// kernel transform fn name
		std::string transformKernel() const;
		// Address, Contract, ...
		std::string transformName() const;

		cl_uchar data1[34];
		cl_uchar data2[34];
		cl_uchar score;
};

#endif /* HPP_MODE */
