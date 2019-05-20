//#include<gtest.h>
#include<iostream>
#include<assert.h>
//#include"GrabMoniter.h"




TEST(GrabSwitch, my_test_1) {
   //开启抓取模式
   GrabSwitch(true);
   assert(behavior_msg.data == "grab start");
   //关闭抓取模式
   GrabSwitch(false);
   assert(behavior_msg.data == "grab stop");
}

TEST(PassSwitch, my_test_2) {
   //开启抓取模式
   PassSwitch(true);
   assert(behavior_msg.data == "pass start");
   //关闭抓取模式
   PassSwitch(false);
   assert(behavior_msg.data == "pass stop");
}

TEST(PassSwitch, my_test_3) {
   //开启递给模式
   PassSwitch(true);
   assert(behavior_msg.data == "pass start");
   //关闭递给模式
   PassSwitch(false);
   assert(behavior_msg.data == "pass stop");
}

TEST(GrabResultCallback, my_test_4) {
   const std_msgs::String::ConstPtr& res1 = "done";
   const std_msgs::String::ConstPtr& res2 = "not yet";
   //未传递完成信息
   GrabResultCallback(res2);
   assert(bGrabDone != true);
   //传递完成信息
   GrabResultCallback(res1);
   assert(bGrabDone == true);
}

TEST(PassResultCallback, my_test_5) {
   const std_msgs::String::ConstPtr& res1 = "done";
   const std_msgs::String::ConstPtr& res2 = "not yet";
   //未传递完成信息
   PassResultCallback(res2);
   assert(bPassDone != true);
   //传递完成信息
   PassResultCallback(res1);
   assert(bPassDone == true);
}

TEST(GrabObj, my_test_6) {
    //开始抓取动作前
    nDelay = 0;
    bGrabDone = false;
    GrabObj();
    assert(bGrabDone == false);
    assert(nDelay == 1);

    //进行抓取动作中
    nDelay = 10;
    bGrabDone = false;
    GrabObj();
    assert(bGrabDone == false);
    assert(nDelay == 11);

    //结束抓取动作
    nDelay = 10;
    bGrabDone = true;
    GrabObj();
    assert(nState == STATE_COMEBACK);
    assert(nDelay == 11);
}

TEST(PassObj(), my_test_6) {
    //开始递给动作前
    nDelay = 0;
    bPassDone = false;
    PassObj();
    assert(bPassDone == false);
    assert(nDelay == 1);

    //进行递给动作中
    nDelay = 10;
    bPassDone = false;
    PassObj();
    assert(bPassDone == false);
    assert(nDelay == 11);

    //结束递给动作
    nDelay = 10;
    bPassDone = true;
    PassObj();
    assert(nState == STATE_ASK);
    assert(nDelay == 11);
}

int main(int argc, char **argv) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
