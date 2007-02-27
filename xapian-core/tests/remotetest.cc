/** @file remotetest.cc
 *  @brief tests for the remote backend
 */
/* Copyright 1999,2000,2001 BrightStation PLC
 * Copyright 2001,2002 Ananova Ltd
 * Copyright 2002,2003,2004,2005,2006,2007 Olly Betts
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of the
 * License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301
 * USA
 */

#include <config.h>
#include "omdebug.h"
#include "testsuite.h"
#include "testutils.h"
#include "backendmanager.h"
#include <xapian/database.h>
#include <xapian/enquire.h>
#include "utils.h"
#include <string>

#include <sys/types.h>
//#include <signal.h> // for kill()

using namespace std;

// Directory which the data is stored in.
string datadir;

// #######################################################################
// # Start of test cases.

#if 0
// test a tcp match when the remote end dies
static bool test_tcpdead1()
{
    BackendManager backendmanager;
    backendmanager.set_dbtype("quartz");
    backendmanager.set_datadir(datadir);
    Xapian::Database dbremote = backendmanager.get_database("apitest_simpledata");

    int pid = fork();
    if (pid == 0) {
	// child code
	char *args[] = {
	    "../bin/xapian-tcpsrv",
	    "--one-shot",
	    "--quiet",
	    "--port",
	    "1237",
	    ".quartz/db=apitest_simpledata",
	    NULL
	};
	// FIXME: we run this directly so we know the pid of the xapian-tcpsrv
	// parent - below we assume the child is the next pid (which isn't
	// necessarily true)
	execv("../bin/.libs/lt-xapian-tcpsrv", args);
	// execv only returns if it couldn't start xapian-tcpsrv
	exit(1);
    } else if (pid < 0) {
	// fork() failed
	FAIL_TEST("fork() failed");
    }

    sleep(3);

    // parent code:
    Xapian::Database db(Xapian::Remote::open("127.0.0.1", 1237);

    Xapian::Enquire enq(db);

    // FIXME: this assumes fork-ed child xapian-tcpsrv process is the pid after
    // the parent - it could be another process has that pid and we would end
    // up killing that process if it was owned by this user.  Also this would
    // cause a spurious test failure.
    if (kill(pid + 1, SIGTERM) == -1) {
	FAIL_TEST("Couldn't send signal to child");
    }

    sleep(3);

//    tout << pid << endl;
//    system("ps x | grep omtcp");

    time_t t = time(NULL);
    try {
	enq.set_query(Xapian::Query("word"));
	Xapian::MSet mset(enq.get_mset(0, 10));
    }
    catch (const Xapian::NetworkError &e) {
	time_t t2 = time(NULL) - t;
	if (t2 > 1) {
	    FAIL_TEST("Client took too long to notice server died (" +
		      om_tostring(t2) + " secs)");
	}
	return true;
    }
    FAIL_TEST("Client didn't get exception when server died");
    return false;
}
#endif

// #######################################################################
// # End of test cases.

test_desc tests[] = {
// disable until we can work out how to kill the right process cleanly
    //{"tcpdead1",	test_tcpdead1},
    {0,			0},
};

int main(int argc, char **argv)
{
    test_driver::parse_command_line(argc, argv);
    string srcdir = test_driver::get_srcdir();

    datadir = srcdir + "/../tests/testdata/";

    return test_driver::run(tests);
}
