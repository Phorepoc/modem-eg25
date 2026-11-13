/* Copyright The Phorepoc Authors
 * SPDX-License-Identifier: EPL-1.0 */

// atcore.h -- the portable AT core of the EG25-G telephony adaptation.
//
// Pure C++ (no Symbian, no libc beyond nothing at all): line framing of the modem's byte stream,
// classification of each line (final result / intermediate / unsolicited), and parsers for the
// responses the licensee TSY (pheltsy) maps onto CTSY completions. Written from 3GPP TS 27.007 /
// 27.005 and the Quectel EG25-G AT manual; no Linux/ModemManager/oFono code is used.
// The same file builds into the Symbian TSY, the x86 scripted AT peer and the host tests.
#ifndef PHE_ATCORE_H
#define PHE_ATCORE_H

namespace pheat {

enum LineKind {
	EEmpty = 0,
	EOk, EError, ECmeError, ECmsError,          // final results
	ENoCarrier, EBusy, ENoAnswer, ENoDialtone, EConnect,
	EPrompt,                                    // "> " of AT+CMGS
	ERing, ECring, EClip,                       // call URCs
	ECreg, ECgreg, ECereg,                      // registration (URC or response, same shape)
	ECsq, EClcc, ECmti, ECmt, ECpin, ECops,
	EQind, ERdy, EPoweredDown,
	EOther                                      // any other "+X:" or info text
};

// True for kinds that end the command in flight.
bool IsFinal(LineKind k);

// Classify one complete line (without CR/LF). For ECmeError/ECmsError, *code gets the number
// (or -1 when the modem sent text under AT+CMEE=2; *code is optional).
LineKind Classify(const char* line, int len, int* code = 0);

// Byte-stream framer: feed raw bytes, get whole lines. Handles CR, LF, CRLF, and the bare
// "> " SMS prompt (which has no line end).
class LineFramer {
public:
	enum { KMaxLine = 512 };
	LineFramer() : iLen(0), iOverflow(false) {}
	// Feed one byte. Returns true when a line is complete; then Line()/Length() hold it until
	// the next Feed().
	bool Feed(char c);
	const char* Line() const { return iBuf; }
	int Length() const { return iOutLen; }
private:
	char iBuf[KMaxLine + 1];
	int iLen;
	int iOutLen;
	bool iOverflow;
};

// +CREG / +CGREG / +CEREG: "[<n>,]<stat>[,<lac>,<ci>[,<AcT>]]" (URC form has no <n>).
struct Reg { int stat; long lac; long ci; int act; };      // -1 = absent
bool ParseReg(const char* line, int len, bool isResponse, Reg& out);

// +CSQ: <rssi>,<ber>. dBm = -113 + 2*rssi (0..31), 99 = unknown -> dbm 0.
struct Csq { int rssi; int ber; int dbm; };
bool ParseCsq(const char* line, int len, Csq& out);

// +CLCC: <id>,<dir>,<stat>,<mode>,<mpty>[,"<number>",<type>]
struct Clcc { int id; int dir; int stat; int mode; int mpty; char number[48]; int type; };
bool ParseClcc(const char* line, int len, Clcc& out);

// +CMTI: "<mem>",<index>
struct Cmti { char mem[4]; int index; };
bool ParseCmti(const char* line, int len, Cmti& out);

// +CPIN: <code> -> the kinds pheltsy maps to security events.
enum PinState { EPinUnknown, EPinReady, EPinSim, EPinPuk, EPinNotInserted };
PinState ParseCpin(const char* line, int len);

} // namespace pheat

#endif
