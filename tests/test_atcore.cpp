/* Copyright The Phorepoc Authors
 * SPDX-License-Identifier: EPL-1.0 */

// Host test for src/atcore: transcripts in EG25-G shape (3GPP 27.007 syntax). Build: tests/run.sh.
#include "../src/atcore/atcore.h"
#include <cstdio>
#include <cstring>

using namespace pheat;
static int fails = 0;
#define CHECK(c) do { if (!(c)) { std::printf("FAIL %s:%d %s\n", __FILE__, __LINE__, #c); ++fails; } } while (0)
static LineKind K(const char* s, int* c = 0) { return Classify(s, (int)std::strlen(s), c); }

int main()
	{
	int code = 0;
	CHECK(K("OK") == EOk && IsFinal(EOk));
	CHECK(K("+CME ERROR: 10", &code) == ECmeError && code == 10);
	CHECK(K("+CMS ERROR: 500", &code) == ECmsError && code == 500);
	CHECK(K("+CME ERROR: SIM not inserted", &code) == ECmeError && code == -1);
	CHECK(K("RING") == ERing && !IsFinal(ERing));
	CHECK(K("+CEREG: 1,\"1A2B\",\"01C3D04\",7") == ECereg);
	CHECK(K("+QIND: SMS DONE") == EQind);
	CHECK(K("Quectel") == EOther);

	LineFramer f; const char* in = "\r\nRDY\r\n\r\nOK\r\n> ";
	const char* want[] = {"RDY", "OK", "> "}; int n = 0;
	for (const char* p = in; *p; ++p)
		if (f.Feed(*p)) { CHECK(n < 3 && std::strcmp(f.Line(), want[n]) == 0); ++n; }
	CHECK(n == 3);

	Reg r;
	const char* resp = "+CREG: 2,1,\"2F1A\",\"0C3D4\",7";
	CHECK(ParseReg(resp, (int)std::strlen(resp), true, r) && r.stat == 1 && r.lac == 0x2F1A && r.ci == 0xC3D4 && r.act == 7);
	const char* urc = "+CREG: 5,\"2F1A\",\"0C3D4\",2";
	CHECK(ParseReg(urc, (int)std::strlen(urc), false, r) && r.stat == 5 && r.act == 2);
	const char* shortResp = "+CREG: 2,4";
	CHECK(ParseReg(shortResp, (int)std::strlen(shortResp), true, r) && r.stat == 4 && r.lac == -1);

	Csq q; CHECK(ParseCsq("+CSQ: 28,99", 11, q) && q.dbm == -57);
	CHECK(ParseCsq("+CSQ: 99,99", 11, q) && q.dbm == 0);

	Clcc c; const char* cl = "+CLCC: 1,1,4,0,0,\"+447700900123\",145";
	CHECK(ParseClcc(cl, (int)std::strlen(cl), c) && c.stat == 4 && c.dir == 1 && std::strcmp(c.number, "+447700900123") == 0 && c.type == 145);
	Cmti m; const char* mt = "+CMTI: \"SM\",3";
	CHECK(ParseCmti(mt, (int)std::strlen(mt), m) && std::strcmp(m.mem, "SM") == 0 && m.index == 3);
	CHECK(ParseCpin("+CPIN: SIM PIN", 14) == EPinSim);
	CHECK(ParseCpin("+CPIN: READY", 12) == EPinReady);

	std::printf(fails ? "atcore: %d FAILED\n" : "atcore: all passed\n", fails);
	return fails ? 1 : 0;
	}
