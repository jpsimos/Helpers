#ifndef __HELPERS_H
#define __HELPERS_H

/*
	Helpers.h - A utility class that is designed to make the life of engineer easier.
	Author: Jacob A Psimos
*/

#include <atomic>
#include <chrono>
#include <condition_variable>
#include <cstdarg>
#include <functional>
#include <memory>
#include <mutex>
#include <stack>
#include <stdexcept>
#include <string>
#include <thread>
#include <vector>
#include <limits>
#include <random>

// Definitions
#define CRC32_DEFAULT_BIT_REFLECTED_POLYNOMIAL   0xEDB88320
#define CRC32_DEFAULT                            0xFFFFFFFF
#define CRC32_XOR                                0xFFFFFFFF
#define CRC16_DEFAULT_BIT_REFLECTED_POLYNOMIAL   0xA001
#define CRC16_DEFAULT                            0xFFFF
#define CRC16_XOR                                0x0000

/* Macros */
#define ISPOWEROF2(x)   !(((x) != 0) && ((x) & ((x) - 1)))
#define ISODD(x)        !!((x) & 1)
#define ISEVEN(x)       !!((~(x)) & 1)

namespace Helpers
{
	namespace Text
	{
		bool CStrEq(const char* __restrict__ a, const char* __restrict__ b);
		std::string Stringf(const char* __restrict__ const fmt, ...);
		std::string StringReplace(std::string str, const std::string& what, const std::string& with);
		std::vector<std::string> StringSplit(const std::string& str, const std::string& delim);
		std::string StringToLower(std::string str);
		std::string StringToUpper(std::string str);
		bool StringBeginsWith(const std::string& str, const std::string& begins);
		bool StringEndsWith(const std::string& str, const std::string& ends);
		std::string::size_type StringStreamLength(std::stringstream& sstream);
		std::string::size_type StringStreamLength(std::ostringstream& sstream);
		std::string HexTable(const void* ptr, const unsigned int num, const unsigned int voffset = 0U);
		void PrintHexTable(const void* ptr, const unsigned int num, const unsigned int voffset = 0U);
	}

	namespace Random
	{
		extern std::random_device randomDevice;
		extern std::default_random_engine randomEngine;

		void SeedRandom(unsigned long s);

		template<typename IntType>
		IntType Random(const IntType inclMin, const IntType inclMax)
		{
			std::uniform_int_distribution<IntType> uniformDist(inclMin, inclMax);
			return uniformDist(randomEngine);
		}
	} // namespace Random

	namespace Numeric
	{
		uint16_t BitReflect16(uint16_t val);
		uint32_t BitReflect32(uint32_t val);
		int BCDToDec(const int bcd);
		int DecToBCD(const int dec);
		bool IsDoubleEqual(const double a, const double b, const double epsilon = 0.000001);
	} // namespace Numeric

	namespace Checksum
	{
		namespace CRC32
		{
			uint32_t Calculate(const void* data, const size_t dataSizeBytes, const uint32_t polynomial = CRC32_DEFAULT_BIT_REFLECTED_POLYNOMIAL);
			uint32_t Recalculate(uint32_t crc, const void* data, const size_t dataSizeBytes, const uint32_t polynomial = CRC32_DEFAULT_BIT_REFLECTED_POLYNOMIAL);
		}
		namespace CRC16
		{
			uint16_t Calculate(const void* data, const size_t dataSizeBytes, const uint16_t polynomial = CRC16_DEFAULT_BIT_REFLECTED_POLYNOMIAL);
			uint16_t Recalculate(uint16_t crc, const void* data, const size_t dataSizeBytes, const uint16_t polynomial = CRC16_DEFAULT_BIT_REFLECTED_POLYNOMIAL);
		}
	} // namespace Checksum

	namespace Time
	{
		long long Micros();
		long long Millis();
		double Seconds();
		double MicrosToSeconds(long long micros);
		double MillisToSeconds(long long millis);
		long long SecondsToMicros(double seconds);
		long long SecondsToMillis(double seconds);
		void WaitSeconds(double seconds);
		void WaitMillis(long long millis);
		void WaitMicros(long long micros);

		class Stopwatch
		{
		public:
			Stopwatch();
			Stopwatch(const int micros);
			Stopwatch(const unsigned int micros);
			Stopwatch(const long long micros);
			Stopwatch(const unsigned long long micros);
			Stopwatch(const double seconds);
			~Stopwatch() {}
			void Reset();
			void Set(const long long micros);
			void Set(const unsigned long long micros);
			void Set(const int micros);
			void Set(const unsigned int micros);
			void Set(const double seconds);
			long long Get() const;
			long long GetMillis() const;
			double GetSeconds() const;
			bool HasExceded(const long long micros) const;
			bool HasExceded(const unsigned long long micros) const;
			bool HasExceded(const int micros) const;
			bool HasExceded(const unsigned int micros) const;
			bool HasExceded(const double seconds) const;
			void operator=(const long long& value);
			void operator=(const unsigned long long& value);
			void operator=(const int& value);
			void operator=(const unsigned int& value);
			void operator=(const double& value);
			bool operator>(const long long& value);
			bool operator>(const unsigned long long& value);
			bool operator>(const int& value);
			bool operator>(const unsigned int& value);
			bool operator>(const double& value);
			bool operator<(const long long& value);
			bool operator<(const unsigned long long& value);
			bool operator<(const int& value);
			bool operator<(const unsigned int& value);
			bool operator<(const double& value);
			bool operator>=(const long long& value);
			bool operator>=(const unsigned long long& value);
			bool operator>=(const int& value);
			bool operator>=(const unsigned int& value);
			bool operator>=(const double& value);
			bool operator<=(const long long& value);
			bool operator<=(const unsigned long long& value);
			bool operator<=(const int& value);
			bool operator<=(const unsigned int& value);
			bool operator<=(const double& value);
			bool operator==(const long long& value);
			bool operator==(const unsigned long long& value);
			bool operator==(const int& value);
			bool operator==(const unsigned int& value);
			bool operator==(const double& value);

			operator long long() const {
				return Get();
			}

			operator unsigned long long() const {
				return static_cast<unsigned long long>(Get());
			}

			operator int() const {
				return static_cast<int>(Get());
			}

			operator unsigned int() const {
				return static_cast<unsigned int>(Get());
			}

			operator double() const {
				return GetSeconds();
			}

		private:
			long long m_beganAt;
		};
	}

	namespace Threading
	{
		class Timer
		{
		public:
			typedef std::function<bool(Timer*, void*)> TimerUserFunc;

		public:
			explicit Timer();
			~Timer();
			void Begin(const unsigned int intervalMillisecs, TimerUserFunc userFunc, void* userArg = nullptr);
			void End();
			bool Running();

		private:
			bool WasStopRequested() const;
			static void InternalTimerFunc(Timer* timer);

		private:
			std::atomic_bool m_stopRequested;
			std::atomic_bool m_started;
			std::atomic_bool m_running;
			unsigned int m_intervalMillisecs;
			void* m_userArg;
			TimerUserFunc m_userFunc;
			std::unique_ptr<std::thread> m_timerThread;
			std::mutex m_timerMutex;
			std::condition_variable m_timerStopCond;
		};
	}
}

#endif
