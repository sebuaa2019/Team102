#include <gtest.h>
#include <iostream>
#include <assert.h>
#include <main.hpp>
#include <voice.hpp>
#include <start.hpp>

TEST(InitKeyword, voice_test_1){
	assert(arKeyword.size() == 9);
}


TEST(FindKeyword, voice_test_2){
    const std::string str1 = "bring me water";
    string ans1 = FindKeyword(str1);
    assert(ans1 == "water");

    const std::string str2 = "bring me food";
    string ans1 = FindKeyword(str2);
    assert(ans1 == "");
}

TEST(AddNewWaypoint, voice_test_3){
	int backup = add_waypoint_pub.size();
	const std::string str1 = "water";
    AddNewWaypoint(str1);
    assert(add_waypoint_pub.size() == backup + 1);

	backup = add_waypoint_pub.size();
    const std::string str2 = "coffee";
    AddNewWaypoint(str2);
    assert(add_waypoint_pub.size() == backup + 1);
}

TEST(Speak, voice_test_4){
	const std::string str1 = ". Hello, team 102!";
	Speak(str1);
	assert(spk_msg.arg == ". Hello, team 102!");
	
	const std::string str2 = ". This is your water, enjoy please.";
	Speak(str2);
	assert(spk_msg.arg == ". This is your water, enjoy please.");
}

TEST(KeywordCB, voice_test_5){
	const std_msgs::String::ConstPtr& msg1;
	msg1->data = "Stop following.";
	KeywordCB(msg1);
	assert(nState == STATE_ASK);
	
	nState = STATE_FOLLOW;
	msg1->data = "Stop following. Bring me water";
	KeywordCB(msg1);
	assert(nState == STATE_GOTO);
	
	nState = STATE_FOLLOW;
	msg1->data = "This is water";
	KeywordCB(msg1);
	assert(nState == STATE_FOLLOW);
}

int main(int argc, char **argv) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}


