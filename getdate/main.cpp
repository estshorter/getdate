#include <stdio.h>
#include <windows.h>
#include <Sysinfoapi.h>

WORD getYearLastTwoDigits(WORD year) {
	return year - (year / 100) * 100;
}

int main(int argc, char* argv[])
{
	SYSTEMTIME lt;
	GetLocalTime(&lt);
	auto year_short = getYearLastTwoDigits(lt.wYear);

	constexpr SIZE_T BUF_SIZE = 10;

	HGLOBAL hg;
	char* str_buf;

	hg = GlobalAlloc(GHND, BUF_SIZE);
	if (hg == NULL) {
		printf("Error on GlobalAlloc");
		return 1;
	}

	str_buf = static_cast<char*>(GlobalLock(hg));
	if (str_buf == NULL) {
		printf("Error on GlobalLock");
		return 1;
	}

	if (snprintf(str_buf, BUF_SIZE, "%02d%02d%02d", year_short, lt.wMonth, lt.wDay) < 0) {
		GlobalUnlock(hg);
		printf("Error on snprintf");
		return 1;
	}
	if (GlobalUnlock(hg) != 0) {
		printf("Error on GlobalUnlock");
		return 1;
	}
	if (!OpenClipboard(NULL)) {
		printf("Error on OpenClipboard");
		return 1;
	}
	if (!EmptyClipboard()) {
		CloseClipboard();
		printf("Error on EmptyClipboard");
		return 1;
	}
	if (SetClipboardData(CF_TEXT, hg) == NULL) {
		CloseClipboard();
		printf("Error on SetClipboardData");
		return 1;
	}
	if (!CloseClipboard()) {
		printf("Error on CloseClipboard");
		return 1;
	}
}