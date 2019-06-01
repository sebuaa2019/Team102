#include <gtest.h>
#include <assert.h>
#include <iostream>
#include "main.h"
#include "move.h"

TEST(moveto, test_no_waypoint) {

    nState = STATE_GOTO;
    cliGetWPName.call(srvName) = false;
    moveto();
    assert(nState == STATE_ASK);
}

/*TEST(moveto, test_failto__waypoint) {

    nState = STATE_GOTO;
    cliGetWPName.call(srvName) = true;
    moveto();
    assert(nState == STATE_ASK);
}*/

TEST(moveto, test_success_to) {

    nState = STATE_GOTO;
    cliGetWPName.call(srvName) = true;
    moveto();
    assert(nState == STATE_GRAB);
    assert(nDelay == 0);
}

TEST(moveback, test_no_master) {

    nState = STATE_COMEBACK;
    cliGetWPName.call(srvName) = false;
    moveback();
    assert(nState == STATE_ASK);
}

/*TEST(moveback, test_failto__master) {

    nState = STATE_COMEBACK;
    cliGetWPName.call(srvName) = true;
    moveback();
    assert(nState == STATE_ASK);
}*/

TEST(moveback, test_success_back) {

    nState = STATE_COMEBACK;
    cliGetWPName.call(srvName) = true;
    moveback();
    assert(nState == STATE_PASS);
    assert(nDelay == 0);
}

int main(int argc, char **argv) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
