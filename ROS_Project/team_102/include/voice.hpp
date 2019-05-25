#ifndef VOICE_H_INCLUDED
#define VOICE_H_INCLUDED

void InitKeyword();
void KeywordCB(const std_msgs::String::ConstPtr & msg);
void FollowSwitch(bool inActive, float inDist);
void Speak(std::string inStr);
void AddNewWaypoint(std::string inStr);
void InitKeyword();

#endif // VOICE_H_INCLUDED
