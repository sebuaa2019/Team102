#include <main.hpp>
#include <voice.hpp>
#include <start.hpp>

// ���Ӻ���ؼ���
void InitKeyword()
{
    arKeyword.push_back("start");   //�����˿�ʼ�����ĵص�,���Ҫ��ȥ
    arKeyword.push_back("water");
    arKeyword.push_back("tea");
    arKeyword.push_back("cola");
    arKeyword.push_back("milk");
    arKeyword.push_back("coffee");
    arKeyword.push_back("juice");
    arKeyword.push_back("wine");
    arKeyword.push_back("whiskey");
}

// �Ӿ�������arKeyword����ڵĹؼ���
static string FindKeyword(string inSentence)
{
    string res = "";
    int nSize = arKeyword.size();
    for(int i=0;i<nSize;i++)
    {
        int nFindIndex = inSentence.find(arKeyword[i]);
        if(nFindIndex >= 0)
        {
            res = arKeyword[i];
            break;
        }
    }
    return res;
}

// �������˵�ǰλ�ñ���Ϊ�º���
void AddNewWaypoint(std::string inStr)
{
    tf::TransformListener listener;
    tf::StampedTransform transform;
    try
    {
        listener.waitForTransform("/map","/base_footprint",  ros::Time(0), ros::Duration(10.0) );
        listener.lookupTransform("/map","/base_footprint", ros::Time(0), transform);
    }
    catch (tf::TransformException &ex) 
    {
        ROS_ERROR("[lookupTransform] %s",ex.what());
        return;
    }

    float tx = transform.getOrigin().x();
    float ty = transform.getOrigin().y();
    tf::Stamped<tf::Pose> p = tf::Stamped<tf::Pose>(tf::Pose(transform.getRotation() , tf::Point(tx, ty, 0.0)), ros::Time::now(), "map");
    geometry_msgs::PoseStamped new_pos;
    tf::poseStampedTFToMsg(p, new_pos);

    waterplus_map_tools::Waypoint new_waypoint;
    new_waypoint.name = inStr;
    new_waypoint.pose = new_pos.pose;
    add_waypoint_pub.publish(new_waypoint);

    ROS_WARN("[New Waypoint] %s ( %.2f , %.2f )" , new_waypoint.name.c_str(), tx, ty);
}

// ����˵��
void Speak(std::string inStr)
{
    spk_msg.arg = inStr;
    spk_pub.publish(spk_msg);
}

bool Request::speech(char *fp) {
    Speak(std::string(fp));
}

// ����ʶ������������
void KeywordCB(const std_msgs::String::ConstPtr & msg)
{
    ROS_WARN("------ Keyword = %s ------",msg->data.c_str());
    if(nState == STATE_FOLLOW)
    {
        // ��ʶ���������в�����Ʒ�����㣩�ؼ���
        string strKeyword = FindKeyword(msg->data);
        int nLenOfKW = strlen(strKeyword.c_str());
        if(nLenOfKW > 0)
        {
            // ������Ʒ�����㣩�ؼ���
            AddNewWaypoint(strKeyword);
            string strSpeak = strKeyword + " . OK. I have memoried. Next one , please"; 
            Speak(strSpeak);
        }

        // ֹͣ�����ָ��
        int nFindIndex = msg->data.find("top follow");
        if(nFindIndex >= 0)
        {
            FollowSwitch(false, 0);
            AddNewWaypoint("master");
            nState = STATE_ASK;
            nDelay = 0;
            Speak("OK. What do you want me to fetch?");
        }
    }

    if(nState == STATE_ASK)
    {
        // ��ʶ���������в�����Ʒ�����㣩�ؼ���
        string strKeyword = FindKeyword(msg->data);
        int nLenOfKW = strlen(strKeyword.c_str());
        if(nLenOfKW > 0)
        {
            // ������Ʒ�����㣩�ؼ���
            strGoto = strKeyword;
            string strSpeak = strKeyword + " . OK. I will go to get it for you."; 
            Speak(strSpeak);
            nState = STATE_GOTO;
        }
    }
}



