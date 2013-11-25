#include "Packing.h"

/***************************
		Packing
***************************/

#include <stdint.h>

namespace Oyster
{
	namespace Network
	{
		namespace Packing
		{
			//bool (1-bit)
			void Pack(unsigned char buffer[], bool i)
			{
				*buffer++ = i;
			}

			//char (8-bit)
			void Pack(unsigned char buffer[], char i)
			{
				*buffer++ = i;
			}

			void Pack(unsigned char buffer[], unsigned char i)
			{
				*buffer++ = i;
			}

			//short (16-bit)
			void Pack(unsigned char buffer[], short i)
			{
				*buffer++ = i >> 8;
				*buffer++ = (char)i;
			}

			void Pack(unsigned char buffer[], unsigned short i)
			{
				*buffer++ = i >> 8;
				*buffer++ = (char)i;
			}

			//int (32-bit)
			void Pack(unsigned char buffer[], int i)
			{
				*buffer++ = i >> 24;
				*buffer++ = i >> 16;
				*buffer++ = i >> 8;
				*buffer++ = i;
			}

			void Pack(unsigned char buffer[], unsigned int i)
			{
				*buffer++ = i >> 24;
				*buffer++ = i >> 16;
				*buffer++ = i >> 8;
				*buffer++ = i;
			}

			//__int64 (64-bit)
			void Pack(unsigned char buffer[], __int64 i)
			{
				*buffer++ = (char)(i >> 56);
				*buffer++ = (char)(i >> 48);
				*buffer++ = (char)(i >> 40);
				*buffer++ = (char)(i >> 32);
				*buffer++ = (char)(i >> 24);
				*buffer++ = (char)(i >> 16);
				*buffer++ = (char)(i >> 8);
				*buffer++ = (char)i;
			}

			void Pack(unsigned char buffer[], unsigned __int64 i)
			{
				*buffer++ = (char)(i >> 56);
				*buffer++ = (char)(i >> 48);
				*buffer++ = (char)(i >> 40);
				*buffer++ = (char)(i >> 32);
				*buffer++ = (char)(i >> 24);
				*buffer++ = (char)(i >> 16);
				*buffer++ = (char)(i >> 8);
				*buffer++ = (char)i;
			}

			//floating point (32, 64-bit)
			void Pack(unsigned char buffer[], float i)
			{
				int tempFloat = Pack754(i, 32, 8);
				Pack(buffer, tempFloat);
			}

			void Pack(unsigned char buffer[], double i)
			{
				__int64 tempDouble = Pack754(i, 64, 11);
				Pack(buffer, tempDouble);
			}

			//string
			void Pack(unsigned char buffer[], char str[])
			{
				short len = strlen(str);
				Pack(buffer, len);
				buffer += 2;
				memcpy(buffer, str, len);
			}

			void Pack(unsigned char buffer[], std::string& str)
			{
				short len = str.length();
				Pack(buffer, len);
				buffer += 2;
				memcpy(buffer, str.c_str(), len);
			}

			unsigned __int64 Pack754(long double f, unsigned bits, unsigned expbits)
			{
				long double fnorm;
				int shift;
				long long sign, exp, significand;
				unsigned significandbits = bits - expbits - 1; // -1 for sign bit

				if (f == 0.0) 
					return 0; // get this special case out of the way

				// check sign and begin normalization
				if (f < 0) 
				{
					sign = 1;
					fnorm = -f;
				}
				else 
				{
					sign = 0;
					fnorm = f;
				}

				// get the normalized form of f and track the exponent
				shift = 0;
				while(fnorm >= 2.0)
				{
					fnorm /= 2.0;
					shift++;
				}

				while(fnorm < 1.0)
				{
					fnorm *= 2.0;
					shift--;
				}

				fnorm = fnorm - 1.0;

				// calculate the binary form (non-float) of the significand data
				significand = fnorm * ((1LL << significandbits) + 0.5f);

				// get the biased exponent
				exp = shift + ((1 << (expbits - 1)) - 1); // shift + bias

				// return the final answer
				return (sign << (bits - 1)) | (exp << (bits - expbits - 1)) | significand;
			}

			/******************************
					Unpacking
			******************************/

			//bool (1-bit)
			bool Unpackb(unsigned char buffer[])
			{
				return (bool)buffer;
			}

			//char (8-bit)
			char Unpackc(unsigned char buffer[])
			{
				if(*buffer <= 0x7f)
				{
					return *buffer;
				}
				else
				{
					return (-1 - (unsigned char)(0xffu - *buffer));
				}
			}

			unsigned char UnpackC(unsigned char buffer[])
			{
				return *buffer;
			}

			//short (16-bit)
			short Unpacks(unsigned char buffer[])
			{
				short i = ((short)buffer[0] << 8) | buffer[1];

				if(i > 0x7fffu)
				{
					i = -1 - (unsigned short)(0xffffu - i);
				}

				return i;
			}

			unsigned short UnpackS(unsigned char buffer[])
			{
				return ((unsigned int)buffer[0] << 8) | buffer[1];
			}

			//int (32-bit)
			int Unpacki(unsigned char buffer[])
			{
				int i = ((int)buffer[0] << 24) | 
						((int)buffer[1] << 16) |
						((int)buffer[2] << 8)  |
						((int)buffer[3]);

				if(i > 0x7fffffffu)
				{
					i = -1 - (int)(0xffffffffu - i);
				}

				return i;
			}

			unsigned int UnpackI(unsigned char buffer[])
			{
				return ((unsigned int)buffer[0] << 24) |
					   ((unsigned int)buffer[1] << 16) |
					   ((unsigned int)buffer[2] << 8)  |
					   ((unsigned int)buffer[3]);
			}

			//__int64 (64-bit)
			__int64 Unpacki64(unsigned char buffer[])
			{
				__int64 i = ((__int64)buffer[0] << 56) |
							((__int64)buffer[1] << 48) |
							((__int64)buffer[2] << 40) |
							((__int64)buffer[3] << 32) |
							((__int64)buffer[4] << 24) |
							((__int64)buffer[5] << 16) |
							((__int64)buffer[6] << 8)  |
							(buffer[7]);

				if(i > 0x7fffffffffffffffu)
				{
					i = -1 - (__int64)(0xffffffffffffffffu - i);
				}

				return i;
			}

			unsigned __int64 UnpackI64(unsigned char buffer[])
			{

				return ((__int64)buffer[0] << 56) |
					   ((__int64)buffer[1] << 48) |
					   ((__int64)buffer[2] << 40) |
					   ((__int64)buffer[3] << 32) |
					   ((__int64)buffer[4] << 24) |
					   ((__int64)buffer[5] << 16) |
					   ((__int64)buffer[6] << 8)  |
					   ((__int64)buffer[7]);
			}

			//floating point (32, 64-bit)
			float Unpackf(unsigned char buffer[])
			{
				int tempFloat = Unpacki(buffer);
				return Unpack754(tempFloat, 32, 8);
			}

			double Unpackd(unsigned char buffer[])
			{
				__int64 tempDouble = Unpacki64(buffer);
				return Unpack754(tempDouble, 64, 11);
			}

			//string
			std::string UnpackStr(unsigned char buffer[])
			{
				short len = UnpackS(buffer);
				std::string temp;
				temp.resize(len);

				buffer += 2;
				for(int i = 0; i < len; i++)
				{
					temp[i] = buffer[i];
				}

				return temp;
			}

			long double Unpack754(unsigned __int64 i, unsigned bits, unsigned expbits)
			{
				long double result;
				long long shift;
				unsigned bias;
				unsigned significandbits = bits - expbits - 1; // -1 for sign bit

				if (i == 0) 
					return 0.0;

				// pull the significand
				result = (i&((1LL << significandbits) - 1)); // mask
				result /= (1LL << significandbits); // convert back to float
				result += 1.0f; // add the one back on

				// deal with the exponent
				bias = (1 << (expbits - 1)) - 1;
				shift = ((i >> significandbits) & ((1LL << expbits) - 1)) - bias;
				while(shift > 0)
				{
					result *= 2.0;
					shift--;
				}
				while(shift < 0)
				{
					result /= 2.0;
					shift++;
				}

				// sign it
				result *= (i >> (bits - 1)) & 1 ? -1.0 : 1.0;

				return result;
			}
		}
	}
}

/*
int32_t pack(unsigned char* buffer, char* format, ...)
{
	va_list ap;
	int16_t h;
	int32_t l;
	int8_t c;
	float f;
	double d;
	char* s;
	int32_t size = 0, len;

	va_start(ap, format);

	for(; *format != '\0'; format++)
	{
		switch(*format)
		{
		case 'h':	// 16-bit
			size += 2;
			h = (int16_t)va_arg(ap, int);
			packi16(buffer, h);
			buffer += 2;
			break;
		case 'l':	// 32-bit
			size += 4;
			l = va_arg(ap, int32_t);
			packi32(buffer, l);
			buffer += 4;
			break;
		case 'c':	// 8-bit
			size += 1;
			c = (int8_t)va_arg(ap, int);
			*buffer++ = (c >> 0)&0xff;
			break;
		case 'f':	// float (32-bit)
			size += 4;
			f = (float)va_arg(ap, double);
			//l = pack754(f, 32, 8);
			packi32(buffer, l);
			buffer += 4;
			break;
		case 'd':	// double (64-bit)
			size += 8;
			d = (float)va_arg(ap, double);
			//l = pack754(f, 64, 11);
			packi32(buffer, l);
			buffer += 4;
			break;
		case 's':	// string
			s = va_arg(ap, char*);
			len = strlen(s);
			size += len + 2;
			packi16(buffer, len);
			buffer += 2;
			memcpy(buffer, s, len);
			buffer += len;
			break;
		}
	}

	va_end(ap);

	return size;
}
*/

/*
void unpack(unsigned char* buffer, char* format, ...)
{
	va_list ap;
	int16_t* h;
	int32_t* l;
	int32_t pf;
	int64_t pd;
	int8_t* c;
	float* f;
	double* d;
	char* s;
	int32_t len, count, maxstrlen = 0;

	va_start(ap, format);

	for(; *format != '\0'; format++)
	{
		switch(*format)
		{
		case 'h': // 16-bit
            h = va_arg(ap, int16_t*);
            *h = unpacki16(buffer);
            buffer += 2;
            break;
        case 'l': // 32-bit
            l = va_arg(ap, int32_t*);
            *l = unpacki32(buffer);
            buffer += 4;
            break;
        case 'c': // 8-bit
            c = va_arg(ap, int8_t*);
            *c = *buffer++;
            break;
        case 'f': // float
            f = va_arg(ap, float*);
            pf = unpacki32(buffer);
            buffer += 4;
            //*f = unpack754(pf, 32, 8);
            break;
		case 'd':	// double (64-bit)
			d = va_arg(ap, double*);
            pd = unpacki64(buffer);
            buffer += 8;
            //*d = unpack754(pf, 64, 11);
            break;
        case 's': // string
            s = va_arg(ap, char*);
            len = unpacki16(buffer);
            buffer += 2;
            if (maxstrlen > 0 && len > maxstrlen) count = maxstrlen - 1;
            else count = len;
            memcpy(s, buffer, count);
            s[count] = '\0';
            buffer += len;
            break;
        default:
            if (isdigit(*format)) // track max str len
			{
                maxstrlen = maxstrlen * 10 + (*format-'0');
			}
		}

		if(!isdigit(*format))
			maxstrlen = 0;
	}

	va_end(ap);
}*/