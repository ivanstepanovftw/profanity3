#include "Mode.hpp"
#include <stdexcept>

Mode::Mode() : score(0) {

}

Mode Mode::benchmark(bool tron) {
	Mode r;
	r.name = "benchmark";
	r.kernel = "profanity_score_benchmark";
	return r;
}

Mode Mode::zeros(bool tron) {
	Mode r = range(tron, 0, 0);
	r.name = "zeros";
	return r;
}

static std::string::size_type hexValueNoException(char c) {
	if (c >= 'A' && c <= 'F') {
		c -= 'A' - 'a';
	}

	const std::string hex = "0123456789abcdef";
	const std::string::size_type ret = hex.find(c);
	return ret;
}

static std::string::size_type hexValue(char c) {
	const std::string::size_type ret = hexValueNoException(c);
	if(ret == std::string::npos) {
		throw std::runtime_error("bad hex value");
	}

	return ret;
}

Mode Mode::matching(bool tron, const std::string strHex) {
	Mode r;
	r.name = "matching";
	if (tron) {
		r.kernel = "profanity_score_matching_tron";
	} else {
		r.kernel = "profanity_score_matching";
	}

	std::fill( r.data1, r.data1 + sizeof(r.data1), cl_uchar(0) );
	std::fill( r.data2, r.data2 + sizeof(r.data2), cl_uchar(0) );

	auto index = 0;

	if (tron) {
		// Process first part until '*'
		while (index < strHex.size() && strHex[index] != '*') {
			r.data1[index] = strHex[index] != '?';
			r.data2[index] = strHex[index];
			++index;
		}

		// Skip the '*'
		if (index < strHex.size() && strHex[index] == '*') {
			++index;
		}

		// Process the remaining part after '*'
		size_t strIndex = 34 - (strHex.size() - index);
		for (size_t i = index; i < strHex.size(); ++i) {
			r.data1[strIndex] = strHex[i] != '?';
			r.data2[strIndex] = strHex[i];
			++strIndex;
		}

		// printf("r.data1:\n");
		// for (int i = 0; i < 34; i++) {
		// 	printf("%02x", r.data1[i]);
		// }
		// printf("\n");
		// printf("r.data2:\n");
		// for (int i = 0; i < 34; i++) {
		// 	printf("%02x", r.data2[i]);
		// }
		// printf("\n");

	} else {
		for( size_t i = 0; i < strHex.size(); i += 2 ) {
			const auto indexHi = hexValueNoException(strHex[i]);
			const auto indexLo = i + 1 < strHex.size() ? hexValueNoException(strHex[i+1]) : std::string::npos;

			const auto valHi = (indexHi == std::string::npos) ? 0 : indexHi << 4;
			const auto valLo = (indexLo == std::string::npos) ? 0 : indexLo;

			const auto maskHi = (indexHi == std::string::npos) ? 0 : 0xF << 4;
			const auto maskLo = (indexLo == std::string::npos) ? 0 : 0xF;

			r.data1[index] = maskHi | maskLo;
			r.data2[index] = valHi | valLo;

			++index;
		}
	}

	return r;
}

Mode Mode::leading(bool tron, const char charLeading) {

	Mode r;
	r.name = "leading";
	if (tron) {
		r.kernel = "profanity_score_leading_tron";
		r.data1[0] = static_cast<cl_uchar>(charLeading);
	} else {
		r.kernel = "profanity_score_leading";
		r.data1[0] = static_cast<cl_uchar>(hexValue(charLeading));
	}
	return r;
}

Mode Mode::range(bool tron, const cl_uchar min, const cl_uchar max) {
	Mode r;
	r.name = "range";
	r.kernel = "profanity_score_range";
	r.data1[0] = min;
	r.data2[0] = max;
	return r;
}

Mode Mode::zeroBytes(bool tron) {
	Mode r;
	r.name = "zeroBytes";
	r.kernel = "profanity_score_zerobytes";
	return r;
}

Mode Mode::letters(bool tron) {
	Mode r = range(tron, 10, 15);
	r.name = "letters";
	return r;
}

Mode Mode::numbers(bool tron) {
	Mode r = range(tron, 0, 9);
	r.name = "numbers";
	return r;
}

std::string Mode::transformKernel() const {
	switch (this->target) {
		case ADDRESS:
			return "";
		case CONTRACT:
			return "profanity_transform_contract";
		default:
			throw "No kernel for target";
	}
}

std::string Mode::transformName() const {
	switch (this->target) {
		case ADDRESS:
			return "Address";
		case CONTRACT:
			return "Contract";
		default:
			throw "No name for target";
	}
}

Mode Mode::leadingRange(bool tron, const cl_uchar min, const cl_uchar max) {
	Mode r;
	r.name = "leadingrange";
	r.kernel = "profanity_score_leadingrange";
	r.data1[0] = min;
	r.data2[0] = max;
	return r;
}

Mode Mode::mirror(bool tron) {
	Mode r;
	r.name = "mirror";
	r.kernel = "profanity_score_mirror";
	return r;
}

Mode Mode::doubles(bool tron) {
	Mode r;
	r.name = "doubles";
	r.kernel = "profanity_score_doubles";
	return r;
}
