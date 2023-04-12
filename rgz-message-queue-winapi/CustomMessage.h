#pragma once
#include <Windows.h>
#include <string>


struct CustomMessage {
	int ID;
	LPWSTR mText;
	LPWSTR sendTime;
	LPWSTR StrText;
public:
	CustomMessage() {
		ID = 0;
		mText = new wchar_t[31];
		sendTime = new wchar_t[9];
		StrText = new wchar_t[80];
	}
	~CustomMessage() {
		delete[] this->StrText;
		delete[] this->sendTime;
		delete[] this->mText;
	}
	LPWSTR getID() {
		wchar_t tID[5];
		wcscpy_s(tID, 5, std::to_wstring(this->ID).c_str());
		return tID;
	}
	void setID(const int& val) { this->ID = val; }

	LPWSTR getText() { return this->mText; }
	void setText(const LPWSTR& text) { wcscpy_s(this->mText, 31, text); }

	LPWSTR getTime() { return this->sendTime; }
	void setTime() {
		SYSTEMTIME st;
		GetLocalTime(&st);
		std::string temp = std::to_string(st.wHour) + ":" + std::to_string(st.wMinute) + ":" + std::to_string(st.wSecond);
		size_t convertedChars = 0;
		mbstowcs_s(&convertedChars, this->sendTime, 9, temp.c_str(), _TRUNCATE);
	}

	void DumpToStr() {
		this->StrText[0] = '\0';
		wcscat_s(this->StrText, 80, this->getID());
		wcscat_s(this->StrText, 80, L") ");
		wcscat_s(this->StrText, 80, this->getTime());
		wcscat_s(this->StrText, 80, L" - ");
		wcscat_s(this->StrText, 80, this->getText());
	}

};