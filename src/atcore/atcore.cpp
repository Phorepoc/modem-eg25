/* Copyright The Phorepoc Authors
 * SPDX-License-Identifier: EPL-1.0 */

// atcore.cpp -- see atcore.h. Freestanding: no library calls.
#include "atcore.h"

namespace pheat {

namespace {

bool StartsWith(const char* s, int len, const char* p)
	{
	int i = 0;
	for (; p[i]; ++i)
		if (i >= len || s[i] != p[i])
			return false;
	return true;
	}

int StrLen(const char* p) { int n = 0; while (p[n]) ++n; return n; }

bool Equals(const char* s, int len, const char* p) { return StrLen(p) == len && StartsWith(s, len, p); }

// A cursor over "a,b,"c",d" argument lists.
struct Args
	{
	const char* s; int len; int pos;
	Args(const char* aS, int aLen, int aPos) : s(aS), len(aLen), pos(aPos)
		{ while (pos < len && s[pos] == ' ') ++pos; }
	bool AtEnd() const { return pos >= len; }
	void SkipComma() { if (pos < len && s[pos] == ',') ++pos; while (pos < len && s[pos] == ' ') ++pos; }
	// Decimal (or hex when aHex / the field is quoted hex, as LAC/CI are). Returns false if empty.
	bool Num(long& v, bool aHex = false)
		{
		bool quoted = pos < len && s[pos] == '"';
		if (quoted) { ++pos; aHex = true; }
		int start = pos; v = 0;
		while (pos < len)
			{
			char c = s[pos]; int d;
			if (c >= '0' && c <= '9') d = c - '0';
			else if (aHex && c >= 'A' && c <= 'F') d = c - 'A' + 10;
			else if (aHex && c >= 'a' && c <= 'f') d = c - 'a' + 10;
			else break;
			v = v * (aHex ? 16 : 10) + d; ++pos;
			}
		bool got = pos > start;
		if (quoted && pos < len && s[pos] == '"') ++pos;
		SkipComma();
		return got;
		}
	bool Int(int& v) { long l; if (!Num(l)) return false; v = (int)l; return true; }
	// Quoted string into out (always NUL-terminated).
	bool Str(char* out, int max)
		{
		int n = 0;
		if (pos >= len || s[pos] != '"') { out[0] = 0; return false; }
		++pos;
		while (pos < len && s[pos] != '"') { if (n < max - 1) out[n++] = s[pos]; ++pos; }
		out[n] = 0;
		if (pos < len) ++pos;
		SkipComma();
		return true;
		}
	int Count() const
		{
		int n = AtEnd() ? 0 : 1; bool q = false;
		for (int i = pos; i < len; ++i) { if (s[i] == '"') q = !q; else if (!q && s[i] == ',') ++n; }
		return n;
		}
	};

int Prefix(const char* s, int len, const char* p) { return StartsWith(s, len, p) ? StrLen(p) : -1; }

} // namespace

bool IsFinal(LineKind k)
	{
	switch (k)
		{
	case EOk: case EError: case ECmeError: case ECmsError: case ENoCarrier: case EBusy:
	case ENoAnswer: case ENoDialtone: case EConnect: case EPrompt:
		return true;
	default:
		return false;
		}
	}

LineKind Classify(const char* s, int len, int* code)
	{
	if (len == 0) return EEmpty;
	if (Equals(s, len, "OK")) return EOk;
	if (Equals(s, len, "ERROR")) return EError;
	int p;
	if ((p = Prefix(s, len, "+CME ERROR:")) >= 0 || (p = Prefix(s, len, "+CMS ERROR:")) >= 0)
		{
		if (code) { Args a(s, len, p); long v; *code = a.Num(v) ? (int)v : -1; }
		return s[3] == 'E' ? ECmeError : ECmsError;
		}
	if (Equals(s, len, "NO CARRIER")) return ENoCarrier;
	if (Equals(s, len, "BUSY")) return EBusy;
	if (Equals(s, len, "NO ANSWER")) return ENoAnswer;
	if (Equals(s, len, "NO DIALTONE")) return ENoDialtone;
	if (StartsWith(s, len, "CONNECT")) return EConnect;
	if (Equals(s, len, ">") || Equals(s, len, "> ")) return EPrompt;
	if (Equals(s, len, "RING")) return ERing;
	if (Equals(s, len, "RDY")) return ERdy;
	if (Equals(s, len, "POWERED DOWN")) return EPoweredDown;
	struct { const char* p; LineKind k; } const tab[] = {
		{"+CRING:", ECring}, {"+CLIP:", EClip}, {"+CREG:", ECreg}, {"+CGREG:", ECgreg},
		{"+CEREG:", ECereg}, {"+CSQ:", ECsq}, {"+CLCC:", EClcc}, {"+CMTI:", ECmti},
		{"+CMT:", ECmt}, {"+CPIN:", ECpin}, {"+COPS:", ECops}, {"+QIND:", EQind} };
	for (unsigned i = 0; i < sizeof(tab) / sizeof(tab[0]); ++i)
		if (StartsWith(s, len, tab[i].p)) return tab[i].k;
	return EOther;
	}

bool LineFramer::Feed(char c)
	{
	if (c == '\r' || c == '\n')
		{
		if (iLen == 0 && !iOverflow) return false;   // blank / second half of CRLF
		iBuf[iLen] = 0; iOutLen = iLen; iLen = 0; iOverflow = false;
		return true;
		}
	if (iLen < KMaxLine) iBuf[iLen++] = c; else iOverflow = true;   // truncate, keep framing
	if (iLen == 2 && iBuf[0] == '>' && iBuf[1] == ' ')
		{
		iBuf[2] = 0; iOutLen = 2; iLen = 0;
		return true;
		}
	return false;
	}

bool ParseReg(const char* s, int len, bool isResponse, Reg& out)
	{
	int i = 0;
	while (i < len && s[i] != ':') ++i;
	if (i >= len) return false;
	Args a(s, len, i + 1);
	out.stat = out.act = -1; out.lac = out.ci = -1;
	// A response carries <n> first; a URC does not. A response with only "<n>,<stat>" or a URC
	// with "<stat>,<lac>,<ci>[,<act>]" -- the count tells them apart when the caller cannot.
	int n = a.Count();
	if (isResponse || n == 2 || n == 5) { int dummy; if (!a.Int(dummy)) return false; }
	if (!a.Int(out.stat)) return false;
	long v;
	if (a.Num(v, true)) out.lac = v;
	if (a.Num(v, true)) out.ci = v;
	int act; if (a.Int(act)) out.act = act;
	return true;
	}

bool ParseCsq(const char* s, int len, Csq& out)
	{
	int p = Prefix(s, len, "+CSQ:");
	if (p < 0) return false;
	Args a(s, len, p);
	if (!a.Int(out.rssi) || !a.Int(out.ber)) return false;
	out.dbm = (out.rssi >= 0 && out.rssi <= 31) ? -113 + 2 * out.rssi : 0;
	return true;
	}

bool ParseClcc(const char* s, int len, Clcc& out)
	{
	int p = Prefix(s, len, "+CLCC:");
	if (p < 0) return false;
	Args a(s, len, p);
	out.number[0] = 0; out.type = -1;
	if (!a.Int(out.id) || !a.Int(out.dir) || !a.Int(out.stat) || !a.Int(out.mode) || !a.Int(out.mpty))
		return false;
	if (a.Str(out.number, sizeof(out.number))) a.Int(out.type);
	return true;
	}

bool ParseCmti(const char* s, int len, Cmti& out)
	{
	int p = Prefix(s, len, "+CMTI:");
	if (p < 0) return false;
	Args a(s, len, p);
	return a.Str(out.mem, sizeof(out.mem)) && a.Int(out.index);
	}

PinState ParseCpin(const char* s, int len)
	{
	int p = Prefix(s, len, "+CPIN:");
	if (p < 0) return EPinUnknown;
	while (p < len && s[p] == ' ') ++p;
	const char* r = s + p; int rl = len - p;
	if (Equals(r, rl, "READY")) return EPinReady;
	if (Equals(r, rl, "SIM PIN")) return EPinSim;
	if (Equals(r, rl, "SIM PUK")) return EPinPuk;
	if (Equals(r, rl, "NOT INSERTED")) return EPinNotInserted;
	return EPinUnknown;
	}

} // namespace pheat
