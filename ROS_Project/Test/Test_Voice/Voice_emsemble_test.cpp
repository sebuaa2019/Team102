#include <gtest.h>
#include <iostream>
#include <assert.h>
#include <main.hpp>
#include <voice.hpp>
#include <start.hpp>


TEST(EMSEMBLE_VOICE_MODEL, voice_test_6)
{
	const std_msgs::String::ConstPtr& msg1;
	msg1->data = "Stop following, bring me coffee.";
	
    ROS_WARN("------ Keyword = %s ------ EMSEMBLE TEST FOR VOICE MODEL", msg->data.c_str());
    
    if(nState == STATE_FOLLOW)
    {
        // 从识别结果句子中查找物品（航点）关键词
        string strKeyword = FindKeyword(msg->data);
        int nLenOfKW = strlen(strKeyword.c_str());
        
        assert(nLenOfKW == 6);
        
        if(nLenOfKW > 0)
        {
            // 发现物品（航点）关键词
            int backup = add_waypoint_pub.size();
            AddNewWaypoint(strKeyword);
            
            assert(add_waypoint_pub.size() == backup + 1);
            
            string strSpeak = strKeyword + " . OK. I have memoried. Next one , please"; 
            Speak(strSpeak);
            
            assert(spk_msg.arg == "coffee . OK. I have memoried. Next one , please");
        }

        // 停止跟随的指令
        int nFindIndex = msg->data.find("top follow");
        
        assert(nFindIndex >= 0);
        
        if(nFindIndex >= 0)
        {
            FollowSwitch(false, 0);
            
            int backup = add_waypoint_pub.size();
            AddNewWaypoint("master");
            assert(add_waypoint_pub.size() == backup + 1);
            
            nState = STATE_ASK;
            nDelay = 0;
            Speak("OK. What do you want me to fetch?");
            
            assert(spk_msg.arg == "OK. What do you want me to fetch?");
        }
    }

    if(nState == STATE_ASK)
    {
        // 从识别结果句子中查找物品（航点）关键词
        string strKeyword = FindKeyword(msg->data);
        int nLenOfKW = strlen(strKeyword.c_str());
        
        assert(nLenOfKW == 6);
        
        if(nLenOfKW > 0)
        {
            // 发现物品（航点）关键词
            strGoto = strKeyword;
            string strSpeak = strKeyword + " . OK. I will go to get it for you."; 
            
            Speak(strSpeak);
            assert(spk_msg.arg == "coffee . OK. I will go to get it for you.");
            
            nState = STATE_GOTO;
        }
    }
}

int main(int argc, char **argv) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}


