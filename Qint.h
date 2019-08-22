#ifndef __QINT_H__
#define __QINT_H__

#include<iostream>
#include<fstream>
#include<string>
#include<conio.h>

class Qint
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
			if ((str[length] < '0' || str[length] > '9') && str[length] != '-')
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

public:
	// 1/. Các hằng số ở mức lớp ------------------------------------------------------------------------------------------------
	static const int sizeType;
	static const int sizeValueType;
	static const int nData;

	// 2/. Constructor và destructor --------------------------------------------------------------------------------------------
	Qint()
	{
		for (int i = 0; i < nData; i++)
			m_data[i] = 0;
	}
	Qint(const Qint &other)
	{
		for (int i = 0; i < nData; i++)
			m_data[i] = other.m_data[i];
	}
	Qint(const char* x, const_value_type &n)
	{
		if (n == 2 && isBinString(x))
			*this = binToDec(x);
		else if (n == 10 && isDecString(x))
			*this = binToDec(decStringToBin(x));
		else if (n == 16 && isHexString(x))
			*this = binToDec(hexToBin(x));
		else
			std::cerr << "Error input :(" << std::endl;
	}
	Qint& operator=(const Qint& other)
	{
		for (int i = 0; i < nData; i++)
			m_data[i] = other.m_data[i];
		return *this;
	}
	~Qint() {}

	// 3/. Quá tải toán tử [] --------------------------------------------------------------------------------------------------
	value_type& operator[](const_value_type &i)
	{
		if (i < nData && i >= 0)
			return m_data[i];
		std::cerr << "Error out of range :(" << std::endl;
		return m_data[0];
	}

	// 4/. Các hàm chuyển hệ ở mức lớp ------------------------------------------------------------------------------------------

	//-- Hệ 10 -> 2 
	static char* decToBin(const Qint &x)
	{
		char* result = new char[sizeType + 1];
		for (int i = 0; i < sizeType; i++)
			result[i] = readBit(x.m_data[i / sizeValueType], sizeValueType, i % sizeValueType) + '0';
		result[sizeType] = '\0';
		return result;
	}
	//-- Hệ 10 -> DecString
	static char* decToDecString(const Qint &x)
	{
		return binToDecString(decToBin(x));
	}
	//-- Hệ 2 -> 10
	static Qint binToDec(const char *x)
	{
		Qint result;
		if (isBinString(x))
		{
			int length = strlen(x);
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
	//-- Chuỗi hệ 10 -> 2
	static char* decStringToBin(const char* x)
	{
		char* result = new char[sizeType + 1];
		for (int i = 0; i < sizeType; i++)
			result[i] = '0';
		result[sizeType] = '\0';
		if (isDecString(x))
		{
			std::string tempX = x, tempResult;
			bool isNegative = false;
			if (*x == '-')
			{
				tempX = &x[1];
				isNegative = true;
			}
			while (tempX != "0")
			{
				tempResult += minusDecString(tempX.c_str(), multiplyDecString(divideDecString(tempX.c_str(), "2"), "2")); // tempResult += tempX % 2
				tempX = divideDecString(tempX.c_str(), "2"); // tempX = tempX / 2
			}
			int length = tempResult.size();
			if (length > sizeType)
			{
				std::cerr << "Error overflow :(" << std::endl;
				return result;
			}
			else
			{
				// Đảo chuỗi
				for (int i = 0; i < length / 2; i++)
					std::swap(tempResult[i], tempResult[length - i - 1]);
				// Chép vào result từ bit thứ 128 trở xuống
				for (int i = 0; i < length; i++)
					result[sizeType - i - 1] = tempResult[length - i - 1];
				// Bù 2 (nếu có)
				if (isNegative)
					result = twosComplement(result);
			}
		}
		else
			std::cerr << "Error input :(" << std::endl;
		return result;
	}
	//-- Hệ 2 -> chuỗi hệ 10
	static char* binToDecString(const char* x)
	{
		if (isBinString(x))
		{
			std::string tempResult = "0";
			int length = strlen(x);
			if (*x == '1')
				tempResult = minusDecString(tempResult.c_str(), exponentiationDecString("2", length - 1));
			for (int i = 1; i < length; i++)
			{
				if (x[i] == '1')
					tempResult = plusDecString(tempResult.c_str(), exponentiationDecString("2", length - i - 1));
			}
			char* result = new char[tempResult.size() + 1];
			memcpy(result, tempResult.c_str(), tempResult.size() + 1);
			return result;
		}
		else
			std::cerr << "Error input :(" << std::endl;
		return NULL;
	}
	//-- Hệ 2 -> 16
	static char* binToHex(const char *x) 
	{
		char* bin[16] = { "0000", "0001", "0010", "0011", "0100", "0101", "0110", "0111", "1000", "1001", "1010", "1011", "1100", "1101", "1110", "1111" }; 
		char hex[16] = { '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'A', 'B', 'C', 'D', 'E', 'F' };

		std::string tempResult, tempX = x;
		int length = tempX.size(); 
		if (length != length / 4 * 4)
		{
			int distance = length - length / 4 * 4;
			length += distance;
			while (distance--)
				tempX.insert(0, 1, '0');
		}
		for (int i = 0; i < length; i += 4) 
		{
			std::string bI(&tempX[i], &tempX[i + 4]);
			for (int j = 0; j < 16; j++)
			{
				if (bI == bin[j])
				{
					tempResult += hex[j];
					break;
				}
			}
		}
		char* result = new char[length / 4 + 1];
		memcpy(result, tempResult.c_str(), length / 4 + 1);
		return result;
	}
	//-- Hệ 16 -> 2
	static char* hexToBin(const char *x)
	{
		char* bin[16] = { "0000", "0001", "0010", "0011", "0100", "0101", "0110", "0111", "1000", "1001", "1010", "1011", "1100", "1101", "1110", "1111" };
		char hex[16] = { '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'A', 'B', 'C', 'D', 'E', 'F' };

		std::string tempResult;
		int length = strlen(x);
		for (int i = 0; i < length; i++)
		{
			for (int j = 0; j < 16; j++)
			{
				if (tolower(x[i]) == tolower(hex[j]))
				{
					tempResult += bin[j];
					break;
				}
			}
		}
		char* result = new char[length * 4 + 1];
		memcpy(result, tempResult.c_str(), length * 4 + 1);
		return result;
	}
	//-- Hệ 10 -> 16
	static char* decToHex(const Qint &x)
	{
		return binToHex(decToBin(x));
	}

	// 5/. Các hàm nhập/xuất ----------------------------------------------------------------------------------------------------
	static void scanQint(Qint &x)
	{
		std::string decString;
		do
		{
			std::cin >> decString;
		} while (isDecString(decString.c_str()) == false);
		x = binToDec(decStringToBin(decString.c_str()));
	}
	static void printQint(const Qint &x)
	{
		std::cout << binToDecString(decToBin(x));
	}
	friend std::istream& operator>>(std::istream &inDev, Qint &x)
	{
		std::string decString;
		inDev >> decString;
		x = binToDec(decStringToBin(decString.c_str()));
		return inDev;
	}
	friend std::ostream& operator<<(std::ostream &outDev, const Qint &x)
	{
		outDev << binToDecString(decToBin(x));
		return outDev;
	}

	// 6/. Quá tải toán tử so sánh  ---------------------------------------------------------------------------------------------
	bool operator==(const Qint &x)
	{
		for (int i = 0; i < nData; i++)
		{
			if (m_data[i] != x.m_data[i])
				return false;
		}
		return true;
	}
	bool operator!=(const Qint &x)
	{
		return !(*this == x);
	}
	bool operator>(const Qint &x)
	{
		value_type MSB1 = readBit(m_data[0], sizeValueType, 0), MSB2 = readBit(x.m_data[0], sizeValueType, 0);
		if (MSB1 ^ MSB2)
			return !MSB1;
		for (int i = 0; i < nData; i++)
		{
			if (m_data[i] > x.m_data[i])
				return true;
			if (m_data[i] < x.m_data[i])
				return false;
		}
		return false;
	}
	bool operator<=(const Qint &x)
	{
		return !(*this > x);
	}
	bool operator<(const Qint &x)
	{
		return (*this <= x && *this != x);
	}
	bool operator>=(const Qint &x)
	{
		return !(*this < x);
	}

	// 7/. Quá tải toán tử dịch & xoay ------------------------------------------------------------------------------------------
	Qint operator<<(const_value_type &n)
	{
		Qint result = *this;
		for (int i = 0; i < n; i++)           
		{
			for (int j = 0; j < nData - 1; j++)    
			{
				result.m_data[j] = result.m_data[j] << 1;  
				result.m_data[j] = result.m_data[j] | readBit(result.m_data[j + 1], sizeValueType, 0);       
			}
			result.m_data[nData - 1] = result.m_data[nData - 1] << 1;
		}
		return result;
	}
	Qint operator>>(const_value_type &n)
	{
		Qint result = *this;
		value_type MSB = readBit(m_data[0], sizeValueType, 0);
		for (int i = 0; i < n; i++)
		{
			for (int j = nData - 1; j > 0; j--)
			{
			    result.m_data[j] = result.m_data[j] >> 1;
				result.m_data[j] = result.m_data[j] | (readBit(result.m_data[j - 1], sizeValueType, sizeValueType - 1) << sizeValueType - 1);
			}
			result.m_data[0] = result.m_data[0] >> 1;
			if (MSB == 1)
				result.m_data[0] = onBit(result.m_data[0], sizeValueType, 0);
		}
		return result;
	}
	Qint rol(const_value_type &n)
	{
		Qint result = *this;
		for (int i = 0; i < n; i++)
		{
			value_type MSB = readBit(result.m_data[0], sizeValueType, 0);
			for (int j = 0; j < nData - 1; j++)
			{
				result.m_data[j] = result.m_data[j] << 1;
				result.m_data[j] = result.m_data[j] | readBit(result.m_data[j + 1], sizeValueType, 0);
			}
			result.m_data[nData - 1] = result.m_data[nData - 1] << 1;
			result.m_data[nData - 1] = result.m_data[nData - 1] | MSB;
		}
		return result;
	}
	Qint ror(const_value_type &n)
	{
		Qint result = *this;
		for (int i = 0; i < n; i++)
		{
			value_type LSB = readBit(result.m_data[nData - 1], sizeValueType, sizeValueType - 1);
			for (int j = nData - 1; j > 0; j--)
			{
				result.m_data[j] = result.m_data[j] >> 1;
				result.m_data[j] = result.m_data[j] | (readBit(result.m_data[j - 1], sizeValueType, sizeValueType - 1) << (sizeValueType - 1));
			}
			result.m_data[0] = result.m_data[0] >> 1;
			result.m_data[0] = result.m_data[0] | (LSB << (sizeValueType - 1));
		}
		return result;
	}

	// 8/. Quá tải toán tử logic số học ------------------------------------------------------------------------------------------
	Qint operator&(const Qint &x)
	{
		Qint z;
		for (int i = 0; i < nData; i++)
			z.m_data[i] = m_data[i] & x.m_data[i];
		return z;
	}
	Qint operator|(const Qint &x)
	{
		Qint z;
		for (int i = 0; i < nData; i++)
			z.m_data[i] = m_data[i] | x.m_data[i];
		return z;
	}
	Qint operator^(const Qint &x)
	{
		Qint z;
		for (int i = 0; i < nData; i++)
			z.m_data[i] = m_data[i] ^ x.m_data[i];
		return z;
	}
	Qint operator~()
	{
		Qint z;
		for (int i = 0; i < nData; i++)
			z.m_data[i] = ~m_data[i];
		return z;
	}

	// 9/. Quá tải toán tử +, -, *, / --------------------------------------------------------------------------------------------
	Qint operator+(const Qint& X)
	{
		Qint result; bool memory = 0;
		value_type bit, bitX;
		for (int i = sizeType - 1; i >= 0; i--)
		{
			bit = readBit(m_data[i / sizeValueType], sizeValueType, i % sizeValueType);
			bitX = readBit(X.m_data[i / sizeValueType], sizeValueType, i % sizeValueType);
			if (memory == 0)
			{
				if (bit == 1 && bitX == 1)
				{
					result.m_data[i / sizeValueType] = offBit(result.m_data[i / sizeValueType], sizeValueType, i % sizeValueType);
					memory = 1;
				}
				else if(bit == 0 && bitX == 0)
					result.m_data[i / sizeValueType] = offBit(result.m_data[i / sizeValueType], sizeValueType, i % sizeValueType);
				else
					result.m_data[i / sizeValueType] = onBit(result.m_data[i / sizeValueType], sizeValueType, i % sizeValueType);
			}
			else
			{
				if (bit == 1 && bitX == 1)
					result.m_data[i / sizeValueType] = onBit(result.m_data[i / sizeValueType], sizeValueType, i % sizeValueType);
				else if (bit == 0 && bitX == 0)
				{
					result.m_data[i / sizeValueType] = onBit(result.m_data[i / sizeValueType], sizeValueType, i % sizeValueType);
					memory = 0;
				}
				else
					result.m_data[i / sizeValueType] = offBit(result.m_data[i / sizeValueType], sizeValueType, i % sizeValueType);
			}
		}
		if ((bit == 1 && bitX == 1 && readBit(result.m_data[0], sizeValueType, 0) == 0) ||
			(bit == 0 && bitX == 0 && readBit(result.m_data[0], sizeValueType, 0) == 1))
			std::cerr << "Error overflow :(" << std::endl;
		return result;
	}
	Qint operator-(const Qint& X)
	{
		return (*this + Qint(twosComplement(decToBin(X)), 2));
	}
	Qint operator*(const Qint& X)
	{
		std::string A(sizeType, '0'), Q = decToBin(*this), M = decToBin(X), AQQ1 = A + Q + "0";
		int count = sizeType;
		while (count-- > 0)
		{
			if (strcmp(&AQQ1[2 * sizeType - 1], "10") == 0)
				A = decToBin(Qint(A.c_str(), 2) - Qint(M.c_str(), 2));
			else if (strcmp(&AQQ1[2 * sizeType - 1], "01") == 0)
				A = decToBin(Qint(A.c_str(), 2) + Qint(M.c_str(), 2));
			AQQ1.replace(0, sizeType, A);
			// AQQ1 >> 1
			for (int i = 2 * sizeType; i > 0; i--)
				AQQ1[i] = AQQ1[i - 1];
			A = AQQ1.substr(0, sizeType);
		}
		AQQ1.pop_back();
		return binToDec(AQQ1.c_str());
	}
	Qint operator/(const Qint& X)
	{
		std::string A(sizeType, '0'), Q = decToBin(*this), M = decToBin(X), AQ;
		int count = sizeType;
		bool isNegative = false;
		if (Q[0] == '1' && M[0] == '1')
		{
			Q = twosComplement(Q.c_str());
			M = twosComplement(M.c_str());
		}
		else if (Q[0] == '0' && M[0] == '1')
		{
			isNegative = true;
			M = twosComplement(M.c_str());
		}
		else if (Q[0] == '1' && M[0] == '0')
		{
			isNegative = true;
			Q = twosComplement(Q.c_str());
		}
		AQ = A + Q;
		while (count--)
		{
			// AQ << 1
			for (int i = 0; i < 2 * sizeType - 1; i++)
				AQ[i] = AQ[i + 1];
			AQ[2 * sizeType - 1] = '0';
			A = AQ.substr(0, sizeType);
			A = decToBin(Qint(A.c_str(), 2) - Qint(M.c_str(), 2));
			if (A[0] == '1')
				A = decToBin(Qint(A.c_str(), 2) + Qint(M.c_str(), 2));
			else
			{
				AQ[2 * sizeType - 1] = '1';
				AQ.replace(0, sizeType, A);
			}
			
		}
		Q = &AQ[sizeType];
		if (isNegative)
			Q = twosComplement(Q.c_str());
		return binToDec(Q.c_str());
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
		std::string opt[11] = { "+", "-", "*", "/", "<<", ">>", "rol", "ror", "&", "|", "^" };
		bool isOpt;
		while (!inFile.eof())
		{
			inFile >> str1;
			inFile >> str2;
			inFile >> str3;
			if (str2 == "~")
			{
				Qint opr(str3.c_str(), std::stoi(str1)), result;
				result = ~opr;
				// Ghi xuống file xuất
				if (str1 == "2")
					outFile << decToBin(result) << std::endl;
				else if (str1 == "10")
					outFile << result << std::endl;
				else if (str1 == "16")
					outFile << decToHex(result) << std::endl;
				else
					std::cerr << "Error p1 :(" << std::endl;
			}
			else
			{
				// Kiểm tra str3 có phải là toán tử
				isOpt = false;
				for (int i = 0; i < 11; i++)
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
					Qint opr1(str2.c_str(), std::stoi(str1)), opr2(str4.c_str(), std::stoi(str1)), result;
					if (str3 == "+")
						result = opr1 + opr2;
					else if (str3 == "-")
						result = opr1 - opr2;
					else if (str3 == "*")
						result = opr1 * opr2;
					else if (str3 == "/")
						result = opr1 / opr2;
					else if (str3 == "<<")
						result = opr1 << std::stoi(str4);
					else if (str3 == ">>")
						result = opr1 >> std::stoi(str4);
					else if (str3 == "rol")
						result = opr1.rol(std::stoi(str4));
					else if (str3 == "ror")
						result = opr1.ror(std::stoi(str4));
					else if (str3 == "&")
						result = opr1 & opr2;
					else if (str3 == "|")
						result = opr1 | opr2;
					else if (str3 == "^")
						result = opr1 ^ opr2;
					else
						std::cerr << "Error operator :(" << std::endl;
					// Ghi xuống file xuất
					if (str1 == "2")
						outFile << decToBin(result) << std::endl;
					else if (str1 == "10")
						outFile << result << std::endl;
					else if (str1 == "16")
						outFile << decToHex(result) << std::endl;
					else
						std::cerr << "Error p1 :(" << std::endl;
				}
				else // Phép chuyển hệ
				{
					if (str1 == "2")
					{
						if (str2 == "10")
							outFile << binToDec(str3.c_str()) << std::endl;
						else if (str2 == "16")
							outFile << binToHex(str3.c_str()) << std::endl;
						else
							std::cerr << "Error p2 :(" << std::endl;
					}
					else if (str1 == "10")
					{
						if (str2 == "2")
							outFile << decToBin(Qint(str3.c_str(), 10)) << std::endl;
						else if (str2 == "16")
							outFile << decToHex(Qint(str3.c_str(), 10)) << std::endl;
						else
							std::cerr << "Error p2 :(" << std::endl;
					}
					else if (str1 == "16")
					{
						if (str2 == "2")
							outFile << hexToBin(str3.c_str()) << std::endl;
						else if (str2 == "10")
							outFile << binToDec(hexToBin(str3.c_str())) << std::endl;
						else
							std::cerr << "Error p2 :(" << std::endl;
					}
					else
						std::cerr << "Error p1 :(" << std::endl;
				}
			}
		}

		outFile.close();
		inFile.close();
	}
};
const int Qint::sizeType = 128;
const int Qint::sizeValueType = 32;
const int Qint::nData = 4;

#endif