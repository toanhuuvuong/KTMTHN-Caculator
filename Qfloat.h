#ifndef __QFLOAT_H__
#define __QFLOAT_H__

#include"Qint.h"

class Qfloat
{
private:
	// 1/. Đặt tên lại kiểu dữ liệu lưu trữ Qint -------------------------------------------------------------------------------
	typedef unsigned int value_type;
	typedef const value_type const_value_type;

	// 2/. Thuộc tính ----------------------------------------------------------------------------------------------------------
	value_type m_data[4];

	// 3/. Hàm private ở mức lớp -----------------------------------------------------------------------------------------------

	//-- Nhóm hàm thao tác trên bit
	static value_type onBit(const_value_type &x, const_value_type &n, const_value_type &i)
	{
		if (i <= n - 1 && i >= 0)
			return x | (1 << (n - 1 - i));
		return x;
	}
	static value_type offBit(const_value_type &x, const_value_type &n, const_value_type &i)
	{
		if (i <= n - 1 && i >= 0)
			return x & (~(1 << (n - 1 - i)));
		return x;
	}
	static value_type readBit(const_value_type &x, const_value_type &n, const_value_type &i)
	{
		if (i <= n - 1 && i >= 0)
		{
			if ((x & (1 << (n - 1 - i))) == 0)
				return 0;
			return 1;
		}
		return 2;
	}

	//-- Nhóm hàm tính bù 1, bù 2
	static char* onesComplement(const char* x)
	{
		int length = strlen(x);
		char* result = new char[length + 1];
		for (int i = 0; i < length; i++)
			result[i] = '0';
		result[length] = '\0';
		if (isBinString(x))
		{
			while (length--)
			{
				if (x[length] == '0')
					result[length] = '1';
			}
		}
		else
			std::cerr << "Error input :(" << std::endl;
		return result;
	}
	static char* twosComplement(const char* x)
	{
		std::string ones = onesComplement(x); // Bù 1
		int length = ones.size();
		std::string bit1(length, '0');
		bit1[length - 1] = '1';
		// Lấy bù 1 + bit 1 = bù 2
		char* twos = new char[length + 1]; bool memory = false;
		for (int i = length - 1; i >= 0; i--)
		{
			if (memory == false)
			{
				if (ones[i] == '1' && bit1[i] == '1')
				{
					twos[i] = '0';
					memory = true;
				}
				else if (ones[i] == '0' && bit1[i] == '0')
					twos[i] = '0';
				else
					twos[i] = '1';
			}
			else
			{
				if (ones[i] == '1' && bit1[i] == '1')
					twos[i] = '1';
				else if (ones[i] == '0' && bit1[i] == '0')
				{
					twos[i] = '1';
					memory = false;
				}
				else
					twos[i] = '0';
			}
		}
		twos[length] = '\0';
		return twos;
	}

	//-- Nhóm hàm kiểm tra tính hợp lệ của chuỗi hệ 2, 10, 16
	static bool isDecString(const char *str)
	{
		int length = strlen(str);
		while (length--)
		{
			if ((str[length] < '0' || str[length] > '9') && str[length] != '-' && str[length] != '.')
				return false;
		}
		return true;
	}
	static bool isBinString(const char *str)
	{
		int length = strlen(str);
		while (length--)
		{
			if (str[length] != '0' && str[length] != '1')
				return false;
		}
		return true;
	}
	static bool isHexString(const char *str)
	{
		int length = strlen(str);
		while (length--)
		{
			if ((str[length] < '0' || str[length] > '9') && (str[length] < 'A' || str[length] > 'F') && (str[length] < 'a' || str[length] > 'f'))
				return false;
		}
		return true;
	}

	//-- Nhóm hàm tính toán trên chuỗi hệ 10
	static char* plusDecString(const char* X, const char* Y)
	{
		std::string Z, tempX, tempY; // Tổng và 2 số hạng dùng để tính toán
		bool isNegative = false; // check dấu của tổng
		if (*X == '-' && *Y == '-')
		{
			// Loại bỏ đi dấu trừ của X và Y
			tempX = &X[1];
			tempY = &Y[1];
			isNegative = true; // Tổng 2 số âm là số âm
		}
		else if (*X != '-' && *Y == '-')
		{
			// Loại bỏ dấu trừ của Y
			tempX = X;
			tempY = &Y[1];
			return minusDecString(tempX.c_str(), tempY.c_str()); // Thực hiện phép toán X - Y
		}
		else if (*X == '-' && *Y != '-')
		{
			// Loại bỏ dấu trừ của X
			tempX = &X[1];
			tempY = Y;
			return minusDecString(tempY.c_str(), tempX.c_str()); // Thực hiện phép toán Y - X
		}
		else
		{
			tempX = X;
			tempY = Y;
		}
		// Xử lí cho 2 số hạng cùng chiều dài
		int lengthX = tempX.size(), lengthY = tempY.size();
		if (lengthX > lengthY)
		{
			tempY.insert(0, lengthX - lengthY, '0');
			lengthY = lengthX;
		}
		else
		{
			tempX.insert(0, lengthY - lengthX, '0');
			lengthX = lengthY;
		}
		// Tính tổng
		int memory = 0; // memory là số nhớ
		for (int i = lengthX - 1; i > 0; i--)
		{
			Z += (tempX[i] + tempY[i] - 2 * '0' + memory) % 10 + '0';
			memory = (tempX[i] + tempY[i] - 2 * '0' + memory) / 10;
		}
		if (tempX[0] + tempY[0] - 2 * '0' + memory > 9)
		{
			Z += (tempX[0] + tempY[0] - 2 * '0' + memory) % 10 + '0';
			Z += (tempX[0] + tempY[0] - 2 * '0' + memory) / 10 + '0';
		}
		else
			Z += (tempX[0] + tempY[0] - 2 * '0' + memory) + '0';
		// Đảo chuỗi
		lengthX = Z.size();
		for (int i = 0; i < lengthX / 2; i++)
			std::swap(Z[i], Z[lengthX - i - 1]);
		// Xóa các số 0 ở đầu không cần thiết
		while (Z[0] == '0' && Z.size() > 1)
			Z.erase(0, 1);
		// Thêm dấu trừ (nếu có)
		if (isNegative)
			Z.insert(0, 1, '-');
		// Cấp phát vùng nhớ trả ra kết quả
		char* result = new char[Z.size() + 1];
		memcpy(result, Z.c_str(), Z.size() + 1);
		return result;
	}
	static char* minusDecString(const char* X, const char* Y)
	{
		std::string Z, tempX, tempY; // Hiệu và 2 số hạng dùng để tính toán
		bool isNegative = false; // check dấu của hiệu
		if (*X == '-' && *Y == '-')
		{
			tempX = &Y[1];
			tempY = &X[1];
		}
		else if (*X != '-' && *Y == '-')
		{
			tempX = X;
			tempY = &Y[1];
			return plusDecString(tempX.c_str(), tempY.c_str());
		}
		else if (*X == '-' && *Y != '-')
			return plusDecString(X, Y);
		else
		{
			tempX = X;
			tempY = Y;
		}
		// Xử lí cho số trừ và số bị trừ cùng chiều dài
		int lengthX = tempX.size(), lengthY = tempY.size();
		if (lengthX > lengthY)
		{
			tempY.insert(0, lengthX - lengthY, '0');
			lengthY = lengthX;
		}
		else
		{
			tempX.insert(0, lengthY - lengthX, '0');
			lengthX = lengthY;
		}
		// Trường hợp số bé trừ số lớn
		if (tempX < tempY)
		{
			std::swap(tempX, tempY);
			isNegative = true;
		}
		// Tính hiệu
		int memory = 0;
		for (int i = lengthX - 1; i >= 0; i--)
		{
			if (tempX[i] >= tempY[i] + memory)
			{
				Z += (tempX[i] - tempY[i] - memory) + '0';
				memory = 0;
			}
			else
			{
				Z += (10 + tempX[i] - tempY[i] - memory) + '0';
				memory = 1;
			}
		}
		// Đảo chuỗi
		lengthX = Z.size();
		for (int i = 0; i < lengthX / 2; i++)
			std::swap(Z[i], Z[lengthX - i - 1]);
		// Xóa các số 0 ở đầu không cần thiết
		while (Z[0] == '0' && Z.size() > 1)
			Z.erase(0, 1);
		// Thêm dấu trừ (nếu có)
		if (isNegative)
			Z.insert(0, 1, '-');
		// Cấp phát vùng nhớ trả ra kết quả
		char* result = new char[Z.size() + 1];
		memcpy(result, Z.c_str(), Z.size() + 1);
		return result;
	}
	static char* multiplyDecString(const char* X, const char* Y)
	{
		std::string Z, tempX, tempY;
		bool isNegative = false;
		if (*X == '-' && *Y == '-')
		{
			tempX = &X[1];
			tempY = &Y[1];
		}
		else if (*X != '-' && *Y == '-')
		{
			tempX = X;
			tempY = &Y[1];
			isNegative = true;
		}
		else if (*X == '-' && *Y != '-')
		{
			tempX = &X[1];
			tempY = Y;
			isNegative = true;
		}
		else
		{
			tempX = X;
			tempY = Y;
		}
		// Chuẩn hóa tempX và tempY (xóa các số 0 đầu không cần thiết)
		while (tempX[0] == '0' && tempX.size() > 1)
			tempX.erase(0, 1);
		while (tempY[0] == '0' && tempY.size() > 1)
			tempY.erase(0, 1);

		if (tempX == "0" || tempY == "0")
			return "0";
		int lengthX = tempX.size(), lengthY = tempY.size();
		// Tính tích
		std::string P; int memory;
		Z = "0";
		for (int i = lengthY - 1; i >= 0; i--)
		{
			P.resize(0); memory = 0;
			for (int j = lengthX - 1; j > 0; j--)
			{
				P += ((tempY[i] - '0')*(tempX[j] - '0') + memory) % 10 + '0';
				memory = ((tempY[i] - '0')*(tempX[j] - '0') + memory) / 10;
			}
			if ((tempY[i] - '0')*(tempX[0] - '0') + memory > 9)
			{
				P += ((tempY[i] - '0')*(tempX[0] - '0') + memory) % 10 + '0';
				P += ((tempY[i] - '0')*(tempX[0] - '0') + memory) / 10 + '0';
			}
			else
				P += ((tempY[i] - '0')*(tempX[0] - '0') + memory) + '0';
			// Thêm số 0 sau chuỗi
			P.insert(0, lengthY - i - 1, '0');
			// Đảo chuỗi
			int lengthZ = P.size();
			for (int i = 0; i < lengthZ / 2; i++)
				std::swap(P[i], P[lengthZ - i - 1]);
			Z = plusDecString(Z.c_str(), P.c_str()); // Z = Z + P
		}
		// Xóa các số 0 ở đầu không cần thiết
		while (Z[0] == '0' && Z.size() > 1)
			Z.erase(0, 1);
		// Thêm dấu - (nếu có)
		if (isNegative)
			Z.insert(0, 1, '-');
		// Cấp phát vùng nhớ trả ra kết quả
		char* result = new char[Z.size() + 1];
		memcpy(result, Z.c_str(), Z.size() + 1);
		return result;
	}
	static char* divideDecString(const char* X, const char* Y)
	{
		std::string Z, tempX, tempY;
		bool isNegative = false;
		if (*X == '-' && *Y == '-')
		{
			tempX = &X[1];
			tempY = &Y[1];
		}
		else if (*X != '-' && *Y == '-')
		{
			tempX = X;
			tempY = &Y[1];
			isNegative = true;
		}
		else if (*X == '-' && *Y != '-')
		{
			tempX = &X[1];
			tempY = Y;
			isNegative = true;
		}
		else
		{
			tempX = X;
			tempY = Y;
		}
		int lengthX = tempX.size(), lengthY = tempY.size();
		if (tempY == "0")
		{
			std::cerr << "Error: divide by zero" << std::endl;
			return "";
		}
		if ((tempX < tempY && lengthX == lengthY) || lengthX < lengthY)
			return "0";
		if (tempX == tempY)
			return "1";

		std::string S, P, i;
		int countSinkingNumber = 0;
		S.assign(tempX, 0, lengthY);
		while (true)
		{
			int lengthS = S.size();
			if ((S < tempY && lengthS == lengthY) || lengthS < lengthY)
			{
				if (lengthY + countSinkingNumber >= lengthX)
					break;
				S += tempX[lengthY + countSinkingNumber];
				countSinkingNumber++;
				// Xóa các số 0 ở đầu không cần thiết
				while (S[0] == '0' &&  S.size() > 1)
					S.erase(0, 1);
				lengthS = S.size();
			}
			for (i = "0"; i <= "9"; i[0]++)
			{
				P = multiplyDecString(i.c_str(), tempY.c_str());
				if ((P > S && P.size() == lengthS) || P.size() > lengthS)
				{
					i[0]--;
					Z += i[0];
					P = multiplyDecString(i.c_str(), tempY.c_str());
					S = minusDecString(S.c_str(), P.c_str());
					break;
				}
				if (i[0] == '9')
				{
					Z += i[0];
					S = minusDecString(S.c_str(), P.c_str());
					break;
				}
			}
		}
		// Thêm dấu - (nếu có)
		if (isNegative)
			Z.insert(0, 1, '-');
		// Cấp phát vùng nhớ trả ra kết quả
		char* result = new char[Z.size() + 1];
		memcpy(result, Z.c_str(), Z.size() + 1);
		return result;
	}
	static char* exponentiationDecString(const char* X, const_value_type &exp)
	{
		std::string Z = "1";
		for (int i = 0; i < exp; i++)
			Z = multiplyDecString(Z.c_str(), X);
		// Cấp phát vùng nhớ trả ra kết quả
		char* result = new char[Z.size() + 1];
		memcpy(result, Z.c_str(), Z.size() + 1);
		return result;
	}

	//-- Nhóm hàm chuẩn hóa dãy bits

	//-- Chuẩn hóa số nguyên (xóa các bit 0 đầu)
	static char* standardInteger(const char* x)
	{
		std::string tempX = x;
		while (tempX[0] == '0' && tempX.size() > 1)
			tempX.erase(0, 1);
		char* result = new char[tempX.size() + 1];
		memcpy(result, tempX.c_str(), tempX.size() + 1);
		return result;
	}
	//-- Chuẩn hóa phần thập phân sau dấu chấm động (mặc định 112 bits)
	static char* standardFraction(const char* x)
	{
		std::string tempX = x;
		int length = tempX.size();
		if (length < sizeSignificand)
			tempX += std::string(sizeSignificand - length, '0');
		else if (length > sizeSignificand)
			tempX = std::string(&tempX[0], &tempX[sizeSignificand]);
		char* result = new char[sizeSignificand + 1];
		memcpy(result, tempX.c_str(), sizeSignificand + 1);
		return result;
	}
	//-- Chuẩn hóa phần mũ (mặc định 15 bits)
	static char* standardExponent(const char* x)
	{
		std::string tempX = x;
		int length = tempX.size();
		if (length < sizeExponent)
			tempX.insert(0, std::string(sizeExponent - length, '0'));
		else if (length > sizeExponent)
			tempX = std::string(&tempX[length - sizeExponent]);
		char* result = new char[sizeExponent + 1];
		memcpy(result, tempX.c_str(), sizeExponent + 1);
		return result;
	}

public:
	// 1/. Các hằng số ở mức lớp ------------------------------------------------------------------------------------------------
	static const int sizeSignificand;
	static const int sizeExponent;
	static const int sizeType;
	static const int sizeValueType;
	static const int nData;
	static const Qint minExponent;
	static const Qint maxExponent;
	static const Qint maxSignificand;

	// 2/. Constructor và destructor --------------------------------------------------------------------------------------------
	Qfloat()
	{
		for (int i = 0; i < nData; i++)
			m_data[i] = 0;
	}
	Qfloat(const Qfloat &other)
	{
		for (int i = 0; i < nData; i++)
			m_data[i] = other.m_data[i];
	}
	Qfloat(const Qint& x)
	{
		std::string decString = Qint::binToDecString(Qint::decToBin(x)); //Lấy chuỗi số nguyên
		*this = binToDec(decStringToBin(decString.c_str())); //hàm chuyển đổi chuỗi số nguyên thành số thực
	}
	Qfloat(const char* x)
	{
		if (isDecString(x))
			*this = binToDec(decStringToBin(x));
	}
	Qfloat& operator=(const Qfloat& other)
	{
		for (int i = 0; i < nData; i++)
			m_data[i] = other.m_data[i];
		return *this;
	}
	~Qfloat() {}

	// 3/. Quá tải toán tử [] --------------------------------------------------------------------------------------------------
	value_type& operator[](const_value_type &i)
	{
		if (i < nData && i >= 0)
			return m_data[i];
		std::cerr << "Error out of range :(" << std::endl;
		return m_data[0];
	}

	// 4/. Các hàm làm tròn -----------------------------------------------------------------------------------------------------

	// 5/. Các hàm chuyển hệ ở mức lớp ------------------------------------------------------------------------------------------

	//--Phần thập phân sau dấu chấm -> hệ 2
	static char* fractionToBin(const char* x)
	{
		std::string tempX = x, tempResult;
		int length = tempX.size();
		for (int i = 0; i < sizeSignificand; i++)
		{
			tempX = multiplyDecString(tempX.c_str(), "2");
			if (tempX.size() > length)
			{
				tempResult += "1";
				tempX.erase(0, 1);
			}
			else
				tempResult += "0";
		}
		char* result = new char[sizeSignificand + 1];
		memcpy(result, tempResult.c_str(), sizeSignificand + 1);
		return result;
	}
	//--Hệ 2 -> phần thập phân sau dấu chấm
	static char* binToFraction(const char* x)
	{
		std::string tempResult = "0";
		int length = strlen(x);
		for (int i = 0; i < length; i++)
		{
			if (x[i] == '1')
			{
				std::string xI(1, x[i]);
				tempResult = plusDecString(tempResult.c_str(), multiplyDecString(exponentiationDecString("5", i + 1), exponentiationDecString("10", length - i - 1)));
			}
		}
		while (tempResult.size() < sizeSignificand)
			tempResult.insert(0, 1, '0');
		char* result = new char[sizeSignificand + 1];
		memcpy(result, tempResult.c_str(), sizeSignificand + 1);
		return result;
	}
	//-- Hệ 10 float -> 2 
	static char* decToBin(const Qfloat &x)
	{
		char* result = new char[sizeType + 1];
		for (int i = 0; i < sizeType; i++)
			result[i] = readBit(x.m_data[i / sizeValueType], sizeValueType, i % sizeValueType) + '0';
		result[sizeType] = '\0';
		return result;
	}
	//-- Hệ 10 -> DecString
	static char* decToDecString(const Qfloat &x)
	{
		return binToDecString(decToBin(x));
	}
	//-- Hệ 2 -> 10 float
	static Qfloat binToDec(const char *x)
	{
		Qfloat result;
		if (isBinString(x))
		{
			int length = strlen(x);
			if (length < sizeType || length > sizeType)
				std::cerr << "Error input :(" << std::endl;
			for (int i = 0; i < length; i++)
			{
				if (x[length - i - 1] == '1')
					result.m_data[(sizeType - i - 1) / sizeValueType] = onBit(result.m_data[(sizeType - i - 1) / sizeValueType], sizeValueType, (sizeType - i - 1) % sizeValueType);
			}
		}
		else
			std::cerr << "Error input :(" << std::endl;
		return result;
	}
	//-- Chuỗi hệ 10 float -> 2
	static char* decStringToBin(const char* x)
	{
		char* result = new char[sizeType + 1];
		for (int i = 0; i < sizeType; i++)
			result[i] = '0';
		result[sizeType] = '\0';

		if (isDecString(x))
		{
			std::string tempX = x, tempResult, integer, fraction, exponent;
			bool isNegative = false;
			if (*x == '-')
			{
				tempX = &x[1];
				isNegative = true;
			}
			// Tìm vị trí dấu chấm
			int iPoint = tempX.find_first_of('.');
			if (iPoint == std::string::npos)
			{
				// Không tìm thấy (chỉ có số nguyên)
				integer = tempX; 
				fraction = "0";
			}
			else
			{
				// Phần nguyên trước dấu chấm, phần thập phân sau dấu chấm
				integer = std::string(&tempX[0], &tempX[iPoint]);
				fraction = &tempX[iPoint + 1];
			}
			// Chuyển phần nguyên và phần thập phân sang hệ 2 (và chuẩn hóa phần nguyên)
			integer = standardInteger(Qint::decStringToBin(integer.c_str()));
			fraction = fractionToBin(fraction.c_str());
			int exp = 0, iFirstBit1; // phần mũ hệ 10 & vị trí bit 1 xuất hiện đầu tiên
			if (integer == "0")
			{
				if (strcmp(standardFraction(fraction.c_str()), std::string(sizeSignificand, '0').c_str()) == 0) // dạng 0.0
				{
					tempResult = std::string(sizeType, '0');
					memcpy(result, tempResult.c_str(), sizeType + 1);
					return result;
				}
				else
				{
					// Dịch phải dấu chấm động
					iFirstBit1 = fraction.find_first_of('1');
					exp = (-1) * iFirstBit1 - 1; // mũ âm
					fraction = standardFraction(&fraction[iFirstBit1 + 1]);
				}
			}
			else
			{
				// Dịch trái dấu chấm động
				iFirstBit1 = 0; // vì interger đã được chuẩn hóa
				exp = integer.size() - 1; // mũ dương
				fraction.insert(0, &integer[1]);
				fraction = standardFraction(fraction.c_str());
			}
			// Chuyển phần mũ sang hệ 2 (và chuẩn hóa)
			exponent = standardExponent(Qint::decStringToBin(std::to_string(exp + (int)pow(2, sizeExponent - 1) - 1).c_str()));
			// Kết quả cuối cùng = 1 bit dấu | 15 bits exponent | 112 bits fraction
			tempResult = (isNegative) ? "1" : "0";
			tempResult += exponent + fraction;
			memcpy(result, tempResult.c_str(), sizeType + 1);
		}
		else
			std::cerr << "Error input :(" << std::endl;

		return result;
	}
	//-- Hệ 2 -> chuỗi hệ 10 float
	static char* binToDecString(const char* x)
	{
		char* result = NULL;

		if (isBinString(x))
		{
			if (strcmp(x, std::string(sizeType, '0').c_str()) == 0)
			{
				result = new char[4];
				memcpy(result, "0.0", 4);
			}
			else
			{
				std::string tempResult, integer, fraction = &x[sizeExponent + 1], exponent = std::string(&x[1], &x[sizeExponent + 1]);
				bool isNegative = false;
				if (*x == '1')
					isNegative = true;
				exponent.insert(0, 1, '0'); // Mặc định số mũ (biểu diễn bias) luôn dương nên luôn mang dấu + 
				int exp = std::stoi(Qint::binToDecString(exponent.c_str())) - (int)pow(2, sizeExponent - 1) + 1;

				if (exp < 0)
				{
					// Mũ âm (phần nguyên = 0), dịch trái dấu chấm động 
					integer = "0";
					exp *= (-1);
					fraction = std::string(exp - 1, '0') + "1" + fraction;
					fraction = binToFraction(standardFraction(fraction.c_str()));
				}
				else if (exp >= 0)
				{
					// Mũ dương (phần nguyên != 0), dịch phải dấu chấm động 
					integer += "01"; integer += std::string(&fraction[0], &fraction[exp]); // Mặc định phần nguyên biểu diễn luôn dương
					integer = Qint::binToDecString(integer.c_str());
					fraction = &fraction[exp];
					fraction = binToFraction(standardFraction(fraction.c_str()));
				}

				// Kết quả cuối cùng integer.fraction
				tempResult = (isNegative) ? "-" : "";
				tempResult += integer + "." + fraction;
				result = new char[tempResult.size() + 1];
				memcpy(result, tempResult.c_str(), tempResult.size() + 1);
			}
		}
		else
			std::cerr << "Error input :(" << std::endl;

		return result;
	}

	// 6/. Các hàm nhập/xuất ----------------------------------------------------------------------------------------------------
	static void scanQfloat(Qfloat &x)
	{
		std::string decString;
		do
		{
			std::cin >> decString;
		} while (isDecString(decString.c_str()) == false);
		x = binToDec(decStringToBin(decString.c_str()));
	}
	static void printQfloat(const Qfloat &x)
	{
		std::cout << binToDecString(decToBin(x));
	}
	friend std::istream& operator>>(std::istream &inDev, Qfloat &x)
	{
		std::string decString;
		inDev >> decString;
		x = binToDec(decStringToBin(decString.c_str()));

		return inDev;
	}
	friend std::ostream& operator<<(std::ostream &outDev, const Qfloat &x)
	{
		outDev << binToDecString(decToBin(x));
		return outDev;
	}

	// 7/. Quá tải toán tử so sánh  ---------------------------------------------------------------------------------------------
	bool operator==(const Qfloat &x)
	{
		for (int i = 0; i < nData; i++)
		{
			if (m_data[i] != x.m_data[i])
				return false;
		}
		return true;
	}
	bool operator!=(const Qfloat &x)
	{
		return !(*this == x);
	}
	bool operator>(const Qfloat &x)
	{
		std::string decString1 = binToDecString(decToBin(*this)), decString2 = binToDecString(decToBin(x));
		int iPoint1 = decString1.find_first_of('.'), iPoint2 = decString2.find_first_of('.');
		std::string integer1 = std::string(&decString1[0], &decString1[iPoint1]), fraction1 = &decString1[iPoint1 + 1];
		std::string integer2 = std::string(&decString2[0], &decString2[iPoint2]), fraction2 = &decString2[iPoint2 + 1];

		if (integer1[0] == '-' && integer2[0] != '-')
			return false;
		if (integer1[0] != '-' && integer2[0] == '-')
			return true;
		if (integer1[0] != '-' && integer2[0] != '-')
		{
			if (integer1 > integer2)
				return true;
			if (integer1 < integer2)
				return false;
			if (fraction1 > fraction2)
				return true;
			if (fraction1 < fraction2)
				return false;
			return false;
		}
		if (integer1 > integer2)
			return false;
		if (integer1 < integer2)
			return true;
		if (fraction1 > fraction2)
			return false;
		if (fraction1 < fraction2)
			return true;
		return false;
	}
	bool operator<=(const Qfloat &x)
	{
		return !(*this > x);
	}
	bool operator<(const Qfloat &x)
	{
		return (*this <= x && *this != x);
	}
	bool operator>=(const Qfloat &x)
	{
		return !(*this < x);
	}

	// 8/. Quá tải toán tử +, -, *, /  ------------------------------------------------------------------------------------------
	Qfloat operator+(const Qfloat &X)
	{
		Qfloat zero = Qfloat("0");
		if (zero == *this)
			return X;
		if (zero == X)
			return *this;
		// Chuyển 2 toán hạng sang nhị phân
		std::string bin1 = decToBin(*this), bin2 = decToBin(X);
		bool isNegative = false;
		if (bin1[0] == '0' && bin2[0] == '1')
		{
			bin2[0] = '0';
			return (*this - binToDec(bin2.c_str()));
		}
		if (bin1[0] == '1' && bin2[0] == '0')
		{
			bin1[0] = '0';
			return (Qfloat(X) - binToDec(bin1.c_str()));
		}
		if (bin1[0] == '1' && bin2[0] == '1')
			isNegative = true;
		// Tách từng toán hạng ra thành 2 phần: phần trị và phần mũ
		Qint exp1(std::string(&bin1[1], &bin1[16]).c_str(), 2), exp2(std::string(&bin2[1], &bin2[16]).c_str(), 2);
		Qint sig1(std::string(&bin1[16]).c_str(), 2), sig2(std::string(&bin2[16]).c_str(), 2);
		// Chuẩn hóa thành dạng 0.F
		sig1 = sig1 >> 1; sig1[0] = onBit(sig1[0], sizeValueType, sizeType - sizeSignificand);
		sig2 = sig2 >> 1; sig2[0] = onBit(sig2[0], sizeValueType, sizeType - sizeSignificand);
		exp1 = exp1 + Qint("1", 10);
		exp2 = exp2 + Qint("1", 10);
		// Mũ có bằng nhau?
		while (exp1 != exp2)
		{
			if (exp1 < exp2)
			{
				exp1 = exp1 + Qint("1", 10);
				sig1 = sig1 >> 1;
				if (strcmp(&(Qint::decToBin(sig1)[sizeType - sizeSignificand]), std::string(sizeSignificand, '0').c_str()) == 0)
					return X;
			}
			else
			{
				exp2 = exp2 + Qint("1", 10);
				sig2 = sig2 >> 1;
				if (strcmp(&(Qint::decToBin(sig2)[sizeType - sizeSignificand]), std::string(sizeSignificand, '0').c_str()) == 0)
					return *this;
			}
		}
		// Cộng 2 phần trị
		Qint sig = sig1 + sig2;
		if (strcmp(&(Qint::decToBin(sig)[sizeType - sizeSignificand]), std::string(sizeSignificand, '0').c_str()) == 0)
			return zero;
		// Nếu phần trị overflow
		if (sig > maxSignificand)
		{
			sig = sig >> 1;
			exp1 = exp1 + Qint("1", 10);
			// Nếu phần mũ overflow
			if (exp1 > maxExponent)
			{
				std::cerr << "Error overflow :(" << std::endl;
				return zero;
			}
		}
		// Chuẩn hóa từ dạng không chuẩn -> dạng chuẩn
		while (readBit(sig[0], sizeValueType, sizeType - sizeSignificand) == 0)
		{
			sig = sig << 1;
			exp1 = exp1 - Qint("1", 10);
		}
		sig = sig << 1;
		exp1 = exp1 - Qint("1", 10);
		// Kết quả cuối cùng
		std::string tempResult = (isNegative) ? "1" : "0";
		tempResult += std::string(&Qint::decToBin(exp1)[sizeType - sizeExponent]);
		tempResult += std::string(&Qint::decToBin(sig)[sizeType - sizeSignificand]);
		return binToDec(tempResult.c_str());
	}
	Qfloat operator-(const Qfloat &X)
	{
		Qfloat zero = Qfloat("0");
		if (zero == *this)
			return X;
		if (zero == X)
			return *this;
		// Chuyển 2 toán hạng sang nhị phân
		std::string bin1 = decToBin(*this), bin2 = decToBin(X);
		bool isNegative = false;
		if (bin1[0] == '0' && bin2[0] == '1')
		{
			bin2[0] = '0';
			return (*this + binToDec(bin2.c_str()));
		}
		if (bin1[0] == '1' && bin2[0] == '0')
		{
			bin2 = '1';
			return (Qfloat(X) + binToDec(bin2.c_str()));
		}
		if (bin1[0] == '1' && bin2[0] == '1')
		{
			bin1[0] == '0';
			bin2[0] == '0';
			std::swap(bin1, bin2);
		}
		if (Qint(bin1.c_str(), 2) < Qint(bin2.c_str(), 2))
		{
			std::swap(bin1, bin2);
			isNegative = true;
		}

		// Tách từng toán hạng ra thành 2 phần: phần trị và phần mũ
		Qint exp1(std::string(&bin1[1], &bin1[16]).c_str(), 2), exp2(std::string(&bin2[1], &bin2[16]).c_str(), 2);
		Qint sig1(std::string(&bin1[16]).c_str(), 2), sig2(std::string(&bin2[16]).c_str(), 2);
		// Chuẩn hóa thành dạng 0.F
		sig1 = sig1 >> 1; sig1[0] = onBit(sig1[0], sizeValueType, sizeType - sizeSignificand);
		sig2 = sig2 >> 1; sig2[0] = onBit(sig2[0], sizeValueType, sizeType - sizeSignificand);
		exp1 = exp1 + Qint("1", 10);
		exp2 = exp2 + Qint("1", 10);
		// Mũ có bằng nhau?
		while (exp1 != exp2)
		{
			if (exp1 < exp2)
			{
				exp1 = exp1 + Qint("1", 10);
				sig1 = sig1 >> 1;
				if (strcmp(&(Qint::decToBin(sig1)[sizeType - sizeSignificand]), std::string(sizeSignificand, '0').c_str()) == 0)
					return X;
			}
			else
			{
				exp2 = exp2 + Qint("1", 10);
				sig2 = sig2 >> 1;
				if (strcmp(&(Qint::decToBin(sig2)[sizeType - sizeSignificand]), std::string(sizeSignificand, '0').c_str()) == 0)
					return *this;
			}
		}
		// Trừ 2 phần trị
		Qint sig = sig1 - sig2;
		if (strcmp(&(Qint::decToBin(sig)[sizeType - sizeSignificand]), std::string(sizeSignificand, '0').c_str()) == 0)
			return zero;
		// Nếu phần trị overflow
		if (sig > maxSignificand)
		{
			sig = sig >> 1;
			exp1 = exp1 + Qint("1", 10);
			// Nếu phần mũ overflow
			if (exp1 > maxExponent)
			{
				std::cerr << "Error overflow :(" << std::endl;
				return zero;
			}
		}
		// Chuẩn hóa từ dạng không chuẩn -> dạng chuẩn
		while (readBit(sig[0], sizeValueType, sizeType - sizeSignificand) == 0)
		{
			sig = sig << 1;
			exp1 = exp1 - Qint("1", 10);
		}
		sig = sig << 1;
		exp1 = exp1 - Qint("1", 10);
		// Kết quả cuối cùng
		std::string tempResult = (isNegative) ? "1" : "0";
		tempResult += std::string(&Qint::decToBin(exp1)[sizeType - sizeExponent]);
		tempResult += std::string(&Qint::decToBin(sig)[sizeType - sizeSignificand]);
		return binToDec(tempResult.c_str());
	}
	Qfloat operator*(const Qfloat &X)
	{
		Qfloat zero = Qfloat("0");
		if (zero == *this || zero == X)
			return zero;
		// Chuyển 2 toán hạng sang nhị phân
		std::string bin1 = decToBin(*this), bin2 = decToBin(X);
		// Tách từng toán hạng ra thành 2 phần: phần trị và phần mũ
		std::string exp1 = bin1.substr(1, sizeExponent), exp2 = bin2.substr(1, sizeExponent);
		std::string sig1 = bin1.substr(sizeExponent + 1, sizeType / 2 - 2), sig2 = bin2.substr(sizeExponent + 1, sizeType / 2 - 2);
		sig1.insert(0, 1, '1');
		sig2.insert(0, 1, '1');
		// Nhân hai phần trị
		std::string sig = Qint::decToBin(Qint(sig1.c_str(), 2) * Qint(sig2.c_str(), 2));
		// Chuẩn hóa
		std::string tempResult;
		for (int i = 0; i < sizeType; i++)
		{
			if (sig[i] == '1')
			{
				tempResult = std::to_string(4 - i - 1 - 16383);
				std::string exp = Qint::decToBin(Qint(exp1.c_str(), 2) + Qint(exp2.c_str(), 2) + Qint(tempResult.c_str(), 10));
				exp = exp.substr(sizeSignificand + 1, sizeExponent);
				sig = sig.substr(i + 1, sizeSignificand);
				if (bin1[0] != bin2[0]) // Khác dấu 
					tempResult = "1";
				else // Cùng dấu
					tempResult = "0";
				tempResult += exp + sig;
				return binToDec(tempResult.c_str());
			}
		}
		return zero;
	}
	Qfloat operator/(const Qfloat &X)
	{
		Qfloat result, zero;
		if (zero == *this)
			return zero;
		if (zero == X)
		{
			std::cerr << "Error divide by zero :(" << std::endl;
			return zero;
		}
		// Chuyển 2 toán hạng sang nhị phân
		std::string bin1 = decToBin(*this), bin2 = decToBin(X);
		// Tách từng toán hạng ra thành 2 phần: phần trị và phần mũ
		std::string exp1 = bin1.substr(1, sizeExponent), exp2 = bin2.substr(1, sizeExponent);
		std::string sig1 = bin1.substr(sizeExponent + 1, sizeSignificand - 1), sig2 = bin2.substr(sizeExponent + 1, sizeSignificand - 1);
		sig1.insert(0, "01"); sig1 += std::string(sizeExponent, '0');
		sig2.insert(0, "01"); sig2 += std::string(sizeExponent, '0');
		// Tính hiệu 2 mũ
		std::string exp = Qint::binToDecString(Qint::decToBin(Qint(exp1.c_str(), 2) - Qint(exp2.c_str(), 2) + Qint("16383", 10)));
		if (Qint(exp.c_str(), 10) > maxExponent)
		{
			std::cerr << "Error overflow :(" << std::endl;
			return zero;
		}
		if (Qint(exp.c_str(), 10) < minExponent)
		{
			std::cerr << "Error underflow :(" << std::endl;
			return zero;
		}
		// Chia hai phần trị
		Qint tempSig1(sig1.c_str(), 2);
		Qint tempSig2(sig2.c_str(), 2);
		// Dịch trái sau mỗi lần chia
		std::string tempResult;
		tempSig1 = tempSig1 >> 1;
		tempSig2 = tempSig2 >> 1;
		for (int i = 0; i < sizeSignificand; i++)
		{
			Qint temp = tempSig1 / tempSig2;
			tempResult.push_back(temp[3] + 48);
			if (temp[3] == 1)
				tempSig1 = tempSig1 - tempSig2;
			if (readBit(tempSig1[0], 32, 1) == 0)
				tempSig1 = tempSig1 << 1;
		}
		// Gán dấu
		if (bin1[0] != bin2[0])
			result.m_data[0] = onBit(result.m_data[0], sizeValueType, 0);
		// Gán phần mũ
		int E = std::stoi(exp);
		while (tempResult[0] == '0')
		{
			tempResult.erase(0, 1);
			E--;
		}
		exp = Qint::decStringToBin(std::to_string(E).c_str());
		exp = &exp[sizeType - sizeExponent];
		for (int i = 1; i < sizeExponent + 1; i++)
		{
			if (exp[i - 1] == '1')
				result.m_data[i / 32] = onBit(result.m_data[i / 32], sizeValueType, i % 32);
		}
		// Gán phần trị
		tempResult.erase(0, 1);
		for (int i = sizeExponent + 1; i < sizeType; i++)
		{
			if ((i - sizeExponent) < tempResult.size())
			{
				if (tempResult[i - 16] == '1')
					result.m_data[i / 32] = onBit(result.m_data[i / 32], sizeValueType, i % 32);
			}
		}
		return result;
	}

	// 9/. Các toán tử ép kiểu  -------------------------------------------------------------------------------------------------
	operator Qint() const
	{
		std::string floatString(binToDecString(decToBin(*this))); //Lấy chuỗi số thực
		int iPoint = floatString.find_first_of('.');
		floatString = std::string(floatString, 0, iPoint); //Cắt lấy phần nguyên 
		return 	Qint::binToDec(Qint::decStringToBin(floatString.c_str())); //Trả về kiểu Qint
	}

	// 10/. Các hàm đọc xuất file txt theo cấu trúc ------------------------------------------------------------------------------
	static void workFile(const char* inFileName, const char* outFileName)
	{
		std::ifstream inFile(inFileName);
		if (!inFile)
		{
			std::cerr << "Error read file :(" << std::endl;
			return;
		}
		std::ofstream outFile(outFileName);
		// 1 dòng tối đa có 4 tham số
		std::string str1, str2, str3, str4;
		std::string opt[4] = { "+", "-", "*", "/" };
		bool isOpt;
		while (!inFile.eof())
		{
			inFile >> str1;
			inFile >> str2;
			inFile >> str3;
			// Kiểm tra str3 có phải là toán tử
			isOpt = false;
			for (int i = 0; i < 4; i++)
			{
				if (str3 == opt[i])
				{
					isOpt = true;
					break;
				}
			}
			if (isOpt) // Phép tính toán
			{
				inFile >> str4;
				// Tính toán
				Qfloat opr1(str2.c_str()), opr2(str4.c_str()), result;
				if (str3 == "+")
					result = opr1 + opr2;
				else if (str3 == "-")
					result = opr1 - opr2;
				else if (str3 == "*")
					result = opr1 * opr2;
				else if (str3 == "/")
					result = opr1 / opr2;
				else
					std::cerr << "Error operator :(" << std::endl;
				// Ghi xuống file xuất
				if (str1 == "2")
					outFile << decToBin(result) << std::endl;
				else if (str1 == "10")
					outFile << result << std::endl;
				else
					std::cerr << "Error p1 :(" << std::endl;
			}
			else // Phép chuyển hệ
			{
				if (str1 == "2")
				{
					if (str2 == "10")
						outFile << binToDec(str3.c_str()) << std::endl;
					else
						std::cerr << "Error p2 :(" << std::endl;
				}
				else if (str1 == "10")
				{
					if (str2 == "2")
						outFile << decToBin(Qfloat(str3.c_str())) << std::endl;
					else
						std::cerr << "Error p2 :(" << std::endl;
				}
				else
					std::cerr << "Error p1 :(" << std::endl;
			}
		}

		outFile.close();
		inFile.close();
	}
};
const int Qfloat::sizeSignificand = 112;
const int Qfloat::sizeExponent = 15;
const int Qfloat::sizeValueType = 32;
const int Qfloat::sizeType = 128;
const int Qfloat::nData = 4;
const Qint Qfloat::minExponent = Qint("-1", 10); // -1
const Qint Qfloat::maxExponent = Qint("32766", 10); // 2*(2^14 - 1)
const Qint Qfloat::maxSignificand = Qint(minusDecString(exponentiationDecString("2", 112), "1"), 10); // 2^112 - 1

#endif