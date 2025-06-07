/*
	Test.cpp - main unit test for Helpers
	Author: Jacob A Psimos
*/

#include <chrono>
#include <iostream>
#include "Helpers.h"

using namespace Helpers;

// Prototypes
bool Test_Macros();
bool Test_Text();
bool Test_Random();
bool Test_Numeric();
bool Test_Checksum();
bool Test_Time();
bool Test_Threading();

// ----------------------------------------------------------------------

int main(int argc, char* argv[])
{
	bool macrosPass = Test_Macros();
	std::cout << "Test_Macros " << (macrosPass ? "Passed" : "Failed") << "\n";

	bool textPass = Test_Text();
	std::cout << "Test_Text " << (textPass ? "Passed" : "Failed") << "\n";

	bool randomPass = Test_Random();
	std::cout << "Test_Random " << (randomPass ? "Passed" : "Failed") << "\n";

	bool numericPass = Test_Numeric();
	std::cout << "Test_Numeric " << (numericPass ? "Passed" : "Failed") << "\n";

	bool checksumPass = Test_Checksum();
	std::cout << "Test_Checksum " << (checksumPass ? "Passed" : "Failed") << "\n";

	bool timePass = Test_Time();
	std::cout << "Test_Time " << (timePass ? "Passed" : "Failed") << "\n";

	bool threadingPass = Test_Threading();
	std::cout << "Test_Threading " << (threadingPass ? "Passed" : "Failed") << "\n";
	
	return 0;
}

/*
 * Test_Macros() test case for the macros in the Helpers namespace
 */
bool Test_Macros()
{
	// Calculate the first sixteen powers of two starting at 2 and determine validity of ISPOWEROF2 macro.
	int powerOfTwo;
	int notPowerOfTwo;

	for(int i = 1; i <= 16; i++)
	{
		powerOfTwo = 1 << i;
		notPowerOfTwo = powerOfTwo + 1;

		if(!ISPOWEROF2(powerOfTwo) || ISPOWEROF2(notPowerOfTwo))
		{
			return false;
		}

		if(ISPOWEROF2(-powerOfTwo) || ISPOWEROF2(-notPowerOfTwo))
		{
			return false;
		}
	}

	int odd = 1;
	int even = 2;

	for(int i = 0; i < 10; i++)
	{
		if(!ISODD(odd) || !ISEVEN(even) || ISODD(even) || ISEVEN(odd))
		{
			return false;
		}

		odd = odd + 2;
		even = even + 2;
	}
	return true;
}

/*
 * Test_Text() test case for the Helpers::Text namespace
 */
bool Test_Text()
{
	std::string testString = "One Two Three";

	if(Text::StringReplace(testString, "Two", "2  ") != "One 2   Three")
	{
			return false;
	}

	if(Text::StringReplace(testString, "One", "1  ") != "1   Two Three")
	{
			return false;
	}

	if(Text::StringReplace(testString, "ee", "33") != "One Two Thr33")
	{
			return false;
	}

	std::vector<std::string> stringSplitTest = Text::StringSplit(testString, " ");

	if(stringSplitTest.size() != 3 || stringSplitTest[0] != "One" || stringSplitTest[1] != "Two" || stringSplitTest[2] != "Three")
	{
		return false;
	}

	if(Text::StringToLower(testString) != "one two three")
	{
		return false;
	}

	if(Text::StringToUpper(testString) != "ONE TWO THREE")
	{
		return false;
	}

	if(!Text::StringBeginsWith(testString, "One") || Text::StringBeginsWith(testString, "one"))
	{
		return false;
	}

	if(!Text::StringEndsWith(testString, "Three") || Text::StringEndsWith(testString, "three"))
	{
		return false;
	}

	testString = "one two three\n";

	if(!Text::StringEndsWith(testString, "\n"))
	{
		return false;
	}

	return true;
}

/*
 * Test_Random test cases for the Helpers::Random namespace
 */
bool Test_Random()
{
	return true;
}

/*
 * Test_Numeric() test case for the Helpers::Numeric namespace
 */
bool Test_Numeric()
{
	const int orig = 123456789;
	int dec = orig;
	int bcd = Numeric::DecToBCD(dec);
	dec = Numeric::BCDToDec(bcd);

	if(orig != dec)
	{
		return false;
	}

	uint16_t bitReflectedTestValue16 = 0x8005;
	uint16_t testValue16 = 0xA001;

	if(bitReflectedTestValue16 != Numeric::BitReflect16(testValue16))
	{
		return false;
	}

	uint32_t bitReflectedTestValue32 = 0x4C11DB7;
	uint32_t testValue32 = 0xEDB88320;

	if(bitReflectedTestValue32 != Numeric::BitReflect32(testValue32))
	{
		return false;
	}

	return true;
}

/*
 * Test_Checksum() test case for the Helpers::Checksum namespace
 */
bool Test_Checksum()
{
	std::string testString = "testing 123 456";
	std::string testStringAppend = "789";

	uint32_t crc32 = Checksum::CRC32::Calculate(testString.c_str(), testString.length(), CRC32_DEFAULT_BIT_REFLECTED_POLYNOMIAL);

	if(0x19C09AA3 != crc32)
	{
		return false;
	}

	crc32 = Checksum::CRC32::Recalculate(crc32, testStringAppend.c_str(), testStringAppend.length(), CRC32_DEFAULT_BIT_REFLECTED_POLYNOMIAL);

	if(0x5D0B5DF4 != crc32)
	{
		return false;
	}

	uint16_t crc16 = Checksum::CRC16::Calculate(testString.c_str(), testString.length(), CRC16_DEFAULT_BIT_REFLECTED_POLYNOMIAL);

	if(0xDF98 != crc16)
	{
		return false;
	}

	crc16 = Checksum::CRC16::Recalculate(crc16, testStringAppend.c_str(), testStringAppend.length(), CRC16_DEFAULT_BIT_REFLECTED_POLYNOMIAL);

	if(0xC3BB != crc16)
	{
		return false;
	}

	return true;
}

/*
 * Test_Time() test case for the Helpers::Time namespace
 */
bool Test_Time()
{
	Time::Stopwatch stopwatchTest;

	long long timeToWraparound = std::numeric_limits<long long>::max() - Time::Micros();
	timeToWraparound /= 1000LL;
	timeToWraparound += 100LL;

	std::cout << "timeToWraparound = " << timeToWraparound << "\n";

	for(unsigned int count = 0; count <= static_cast<unsigned int>(timeToWraparound) / 10; count++)
	{
		Time::WaitMillis(10);
	}

	std::cout << "timeWaited = " << stopwatchTest.GetMillis() << "\n";

	std::this_thread::sleep_for(std::chrono::milliseconds(100));

	long long micros = stopwatchTest;

	std::this_thread::sleep_for(std::chrono::milliseconds(100));

	if(stopwatchTest <= micros)
	{
		return false;
	}

	double seconds = stopwatchTest.GetSeconds();

	if(!(micros < Time::SecondsToMicros(seconds)))
	{
		return false;
	}

	stopwatchTest = 0;

	if(stopwatchTest.HasExceded(1000 * 250) || stopwatchTest >= 0.250)
	{
		return false;
	}

	std::this_thread::sleep_for(std::chrono::milliseconds(251));

	if(!stopwatchTest.HasExceded(1000 * 250) || stopwatchTest <= 0.250)
	{
		return false;
	}

	return true;
}

/*
 * Test_Thredding() test case for the Helpers::Threading namespace
 */
bool Test_Threading()
{
	long long beganAtMillis = Time::Millis();
	unsigned int timerTickCount = 0;
	unsigned int timerInterval = 123;
	Threading::Timer timerTest;

	timerTest.Begin(timerInterval,
		[&](Threading::Timer* timerPtr, void* arg) -> bool{
			unsigned int* timerTickCountPtr = (unsigned int*)arg;
			*timerTickCountPtr = *timerTickCountPtr + 1;
			if(*timerTickCountPtr == 3)
			{
				return false;
			}
			return true;
		},
	&timerTickCount);

	while(timerTest.Running())
	{
		std::this_thread::sleep_for(std::chrono::milliseconds(10));
	}

	if(timerTickCount != 3 || Time::Millis() - beganAtMillis > timerTickCount * timerInterval)
	{
		return false;
	}

	return true;
}

